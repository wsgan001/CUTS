using System;
using System.Data;
using System.ComponentModel;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;

namespace CUTS
{
  public partial class SystemPerformance : System.Web.UI.UserControl
  {
    protected void Page_Load(object sender, EventArgs e)
    {
      if (!this.Page.IsPostBack)
        this.DataBind();
    }

    #region Data Binding Properties
    /**
     * Data source property for the control. The data source must
     * expose the IListSource or IEnumerable interface. Any other
     * type of interfaces will not be accepted.
     */
    public object DataSource
    {
      set
      {
        if (value is IListSource || value is IEnumerable || value == null)
        {
          this.data_source_ = value;
        }
        else
        {
          throw new ArgumentException("Object must implement IEnumerable or IListSource interface.",
                                      "DataSource");
        }
      }

      get
      {
        return this.data_source_;
      }
    }

    /**
     * Data member property for the dataset. Since the data source is
     * expected to have a IListSource interface, then the data member
     * determines what collection in the data source to use.
     */
    public string DataMember
    {
      get
      {
        return this.data_member_;
      }

      set
      {
        this.data_member_ = value;
      }
    }

    public string DataMemberBaseline
    {
      get { return this.data_member_baseline_; }
      set { this.data_member_baseline_ = value; }
    }

    public string DataComponentName
    {
      get { return this.data_component_name_; }
      set { this.data_component_name_ = value; }
    }

    public string DataComponentNameBaseline
    {
      set { this.data_component_name_baseline_ = value; }
      get { return this.data_component_name_baseline_; }
    }

    public string DataCategoryName
    {
      get { return this.data_category_name_; }
      set { this.data_category_name_ = value; }
    }

    public string DataHostname
    {
      set { this.data_hostname_ = value; }
      get { return this.data_hostname_; }
    }

    public string DataHostnameBaseline
    {
      get { return this.data_hostname_baseline_; }
      set { this.data_hostname_baseline_ = value; }
    }

    public string DataMetricType
    {
      get { return this.data_metric_type_; }
      set { this.data_metric_type_ = value; }
    }

    public string DataMetricTypeBaseline
    {
      set { this.data_metric_type_baseline_ = value; }
      get { return this.data_metric_type_baseline_; }
    }

    public string DataSrcName
    {
      get { return this.data_src_name_; }
      set { this.data_src_name_ = value; }
    }

    public string DataSrcNameBaseline
    {
      get { return this.data_src_name_baseline_; }
      set { this.data_src_name_baseline_ = value; }
    }

    public string DataDstName
    {
      get { return this.data_dst_name_; }
      set { this.data_dst_name_ = value; }
    }

    public string DataDstNameBaseline
    {
      get { return this.data_dst_name_baseline_; }
      set { this.data_dst_name_baseline_ = value; }
    }

    public string DataEventCount
    {
      get { return this.data_event_count_; }
      set { this.data_event_count_ = value; }
    }

    public string DataEventCountBaseline
    {
      get { return this.data_event_count_baseline_; }
      set { this.data_event_count_baseline_ = value; }
    }

    public string DataBestTime
    {
      get { return this.data_best_time_; }
      set { this.data_best_time_ = value; }
    }

    public string DataBestTimeBaseline
    {
      get { return this.data_best_time_baseline_; }
      set { this.data_best_time_baseline_ = value; }
    }

    public string DataAvgTime
    {
      get { return this.data_avg_time_; }
      set { this.data_avg_time_ = value; }
    }

    public string DataAvgTimeBaseline
    {
      get { return this.data_avg_time_baseline_; }
      set { this.data_avg_time_baseline_ = value; }
    }

    public string DataWorstTime
    {
      get { return this.data_worst_time_; }
      set { this.data_worst_time_ = value; }
    }

    public string DataWorstTimeBaseline
    {
      get { return this.data_worst_time_baseline_; }
      set { this.data_worst_time_baseline_ = value; }
    }

    public string CategoryHeading
    {
      get { return this.category_heading_; }
      set { this.category_heading_ = value; }
    }
    #endregion

    #region Control Factories
    private void CreateControlHeirarchy(bool useviewstate)
    {
      // Get an interface pointer to the execution times and baseline
      // tables provided via the datasource.
      IEnumerable execution = this.GetDataSource(this.data_member_);
      IEnumerable baseline = null;

      if (this.data_member_baseline_ != null)
        baseline = this.GetDataSource(this.data_member_baseline_);

      if (execution != null)
      {
        CUTS.ComponentPerformanceGrid grid = null;
        CUTS.ComponentPerformanceCategory category = null;
        CUTS.PortPerformance inport = null;

        foreach (object item in execution)
        {
          PropertyDescriptorCollection props = TypeDescriptor.GetProperties(item);

          if (this.data_component_name_ == null)
            throw new Exception("DataComponentName property not defined.");

          string value = this.GetDataValue(ref props, item, this.data_component_name_);
          grid = this.FindComponentPerformanceGrid(value);

          // Create the control for the category of the component. We do
          // not have to check if the category is for the component since
          // the grid will be correct.
          if (this.data_category_name_ == null)
            throw new Exception("DataCategoryName property not defined.");

          value = this.GetDataValue(ref props, item, this.data_category_name_);
          category = grid.FindCategory(value);

          if (this.category_heading_ != null)
            category.CategoryName = this.category_heading_;

          // Before we can continue reading the metrics, we need to get
          // the metric's type. This is either 'queue' or 'process'.
          if (this.data_metric_type_ == null)
            throw new Exception("DataMetricType property not defined.");

          string metric_type = this.GetDataValue(ref props, item, this.data_metric_type_);

          if (metric_type != "process" && metric_type != "queue")
            throw new Exception("DataMetricType must have value 'queue' or 'process'");

          string hostname = string.Empty;

          if (this.data_member_baseline_ != null)
          {
            if (this.data_hostname_ == null)
              throw new Exception("DataHostname property not defined");

            hostname = this.GetDataValue(ref props, item, this.data_hostname_);
          }

          // We have now moved onto the inputs for the component. This
          // is determined by the source name.
          if (this.data_src_name_ == null)
            throw new Exception("DataSrcName property not defined.");

          value = this.GetDataValue(ref props, item, this.data_src_name_);
          inport = category.FindPortPerformance(value);

          // Ok, let's get the performance metrics for this item. We can
          // do this because regardless of what metrics we are processing,
          // they will all have the following performance metrics.
          CUTS.PerformanceTimes perf = new CUTS.PerformanceTimes();
          this.GetPerformanceMetrics(ref props, item, ref perf);

          if (metric_type == "process")
          {
            //// We have now moved onto the exit points for the input. This
            //// is determined by the destination name.
            //if (this.data_dst_name_ == null)
            //  throw new Exception("DataDstName property not defined.");

            //value = this.GetDataValue(ref props, item, this.data_dst_name_);

            // Locate baseline metrics for the current performance metrics. This
            // can be the overall queue/process time, or the exit point times.
            CUTS.PerformanceTimes perf_baseline = null;

            if (baseline != null)
            {
              this.GetBaselineMetrics(baseline,
                                      hostname,
                                      grid.Title,
                                      metric_type,
                                      inport.PortName,
                                      value,
                                      out perf_baseline);
            }

            //if (value != String.Empty)
            //{
            //  CUTS.ExitPoint ep = new ExitPoint();

            //  // Set the values of the exit point. This includes the
            //  // current observed time and the baseline time.
            //  ep.Name = value;
            //  ep.Performance = perf;

            //  if (baseline != null)
            //    ep.BaselinePerformance = perf_baseline;

            //  // Insert the exit point in the exit times.
            //  inport.InsertExitPoint(ep);
            //}
            //else
            {
              // Save the performance metric in the service time.
              inport.ServiceTime = perf;

              if (baseline != null)
                inport.ServiceTimeBaseline = perf_baseline;
            }
          }
          else
          {
            // Check to see if there are baseline metrics for the
            // queueing time for this port.
            CUTS.PerformanceTimes perf_baseline = null;

            if (baseline == null)
            {
              this.GetBaselineMetrics(baseline,
                                      hostname,
                                      grid.Title,
                                      metric_type,
                                      inport.PortName,
                                      null,
                                      out perf_baseline);
            }

            // Save the performance metric in the queuing time.
            inport.QueuingTime = perf;

            if (baseline != null)
              inport.QueuingTimeBaseline = perf_baseline;
          }
        }
      }
    }

    protected override void CreateChildControls()
    {
      this.Controls.Clear();
      this.CreateControlHeirarchy(true);
    }

    public CUTS.ComponentPerformanceGrid FindComponentPerformanceGrid(string name)
    {
      CUTS.ComponentPerformanceGrid grid = null;

      foreach (Control item in this.Controls)
      {
        if (item is CUTS.ComponentPerformanceGrid)
        {
          grid = (CUTS.ComponentPerformanceGrid)item;

          if (grid.Title == name)
            return grid;
        }
      }

      // Create a new component performance grid.
      Control control = this.LoadControl("~/controls/ComponentPerformanceGrid.ascx");
      grid = (CUTS.ComponentPerformanceGrid)control;
      grid.Title = name;

      this.Controls.Add(grid);
      return grid;
    }

    /**
     * Locates the baseline metrics for the component in the collection
     * of provided items.
     */
    private void GetBaselineMetrics(IEnumerable baseline,
                                    string hostname,
                                    string instance,
                                    string metric_type,
                                    string srcname,
                                    string dstname,
                                    out CUTS.PerformanceTimes perf)
    {
      if (baseline != null)
      {
        foreach (object item in baseline)
        {
          // Get the properties for this item.
          PropertyDescriptorCollection props = TypeDescriptor.GetProperties(item);

          // Check the hostname of the item.
          string value = this.GetDataValue(ref props, item, this.data_hostname_baseline_);
          if (value != hostname)
            continue;

          // Check the instance name of the item.
          value = this.GetDataValue(ref props, item, this.data_component_name_baseline_);
          if (value != instance)
            continue;

          // Check the metric type of the item.
          value = this.GetDataValue(ref props, item, this.data_metric_type_baseline_);
          if (value != metric_type)
            continue;

          // Check the source name of the item.
          value = this.GetDataValue(ref props, item, this.data_src_name_baseline_);
          if (value != srcname)
            continue;

          // Check the destination name of the item.
          value = this.GetDataValue(ref props, item, this.data_dst_name_baseline_);
          if (value != dstname)
            continue;

          // Yes, we actually found the baseline metric for this instance.
          // We can now get the baseline performance metrics.
          perf = new CUTS.PerformanceTimes();
          this.GetPerformanceMetricsBaseline(ref props, item, ref perf);
          return;
        }
      }

      // By default, if we reach this point then we didn't find
      // baseline metrics for the specified component.
      perf = null;
    }
    #endregion

    #region Data Discovery Routines (DDR)
    private void GetPerformanceMetrics(ref PropertyDescriptorCollection props,
                                       object item,
                                       ref CUTS.PerformanceTimes perf)
    {
      if (this.data_event_count_ == null)
        throw new Exception("DataEventCount property not defined.");
      perf.Count = long.Parse(this.GetDataValue(ref props, item, this.data_event_count_));

      if (this.data_best_time_ == null)
        throw new Exception("DataBestTime property not defined.");
      perf.Minimum = long.Parse(this.GetDataValue(ref props, item, this.data_best_time_));

      if (this.data_avg_time_ == null)
        throw new Exception("DataAvgTime property not defined.");
      perf.Average = double.Parse(this.GetDataValue(ref props, item, this.data_avg_time_));

      if (this.data_worst_time_ == null)
        throw new Exception("DataWorstTime property not defined.");
      perf.Maximum = long.Parse(this.GetDataValue(ref props, item, this.data_worst_time_));
    }

    private void GetPerformanceMetricsBaseline(ref PropertyDescriptorCollection props,
                                               object item,
                                               ref CUTS.PerformanceTimes perf)
    {
      if (this.data_event_count_baseline_ == null)
        throw new Exception("DataEventCountBaseline property not defined.");
      perf.Count = long.Parse(this.GetDataValue(ref props, item, this.data_event_count_baseline_));

      if (this.data_best_time_baseline_ == null)
        throw new Exception("DataBestTimeBaseline property not defined.");
      perf.Minimum = long.Parse(this.GetDataValue(ref props, item, this.data_best_time_baseline_));

      if (this.data_avg_time_baseline_ == null)
        throw new Exception("DataAvgTimeBaseline property not defined.");
      perf.Average = double.Parse(this.GetDataValue(ref props, item, this.data_avg_time_baseline_));

      if (this.data_worst_time_baseline_ == null)
        throw new Exception("DataWorstTimeBaseline property not defined.");
      perf.Maximum = long.Parse(this.GetDataValue(ref props, item, this.data_worst_time_baseline_));
    }

    private string GetDataValue(ref PropertyDescriptorCollection props,
                                object item,
                                string propname)
    {
      PropertyDescriptor desc = props.Find(propname, true);

      if (desc != null)
      {
        object value = desc.GetValue(item);
        return desc.Converter.ConvertToString(value);
      }
      else
      {
        string errmsg = String.Format("Data source does not contain '{0}' field.",
                                      propname);
        throw new Exception(errmsg);
      }
    }

    public override void DataBind()
    {
      // Signal the base class to handle its data binding event.
      base.OnDataBinding(EventArgs.Empty);

      // Clear all the child controls and their view state. We are
      // then going to track the view state of the new controls.
      this.Controls.Clear();

      if (this.HasChildViewState)
        this.ClearChildViewState();

      if (this.IsTrackingViewState)
        this.TrackViewState();

      this.CreateControlHeirarchy(false);
      this.ChildControlsCreated = true;
    }

    protected IEnumerable GetDataSource(string member)
    {
      // We can just return if we are a NULL object.
      if (this.data_source_ == null)
        return null;

      // First, let use check see if the data source is already
      // an enumerable type. If so, we can just return it. :)
      IEnumerable enumerable = this.data_source_ as IEnumerable;

      if (enumerable != null)
        return enumerable;

      // Ok, so we aren't working with an enumerable directly. Maybe,
      // we have to dig a little deeper to get the enumerable interface.
      IListSource listsrc = this.data_source_ as IListSource;

      if (listsrc != null)
      {
        IList list_member = listsrc.GetList();

        // Maybe there is a chance that we are not working with a list
        // that has a collection of list. If this is the case, then we
        // can return this list as an enumerable.
        if (listsrc.ContainsListCollection == false)
          return (IEnumerable)list_member;

        ITypedList typed_list = list_member as ITypedList;

        if (typed_list != null)
        {
          // Get the properties for the default type list. We are mainly
          // concerned with the display name of each property.
          PropertyDescriptorCollection props = typed_list.GetItemProperties(null);

          if (props != null && props.Count > 0)
          {
            // We are going to select the property description based on
            // the data member. If there isn't a data member present, then
            // we are going to select the first property description.
            // Otherwise, we are going to attempt to locate the data member
            // in the collection.
            PropertyDescriptor desc;

            if (member == null || member == String.Empty)
              desc = props[0];
            else
              desc = props.Find(member, false);

            if (desc != null)
            {
              object listrow = list_member[0];
              object list = desc.GetValue(listrow);

              if (list is IEnumerable)
                return (IEnumerable)list;
            }
            else
            {
              throw new Exception("Data source does not contain specified data member.");
            }
          }
          else
          {
            throw new Exception("Data source does not contains any bindable data members.");
          }
        }
      }
      return null;
    }

    private PropertyDescriptor [] GetColumnPropertyDescriptors(object item)
    {
      ArrayList props = new ArrayList();
      PropertyDescriptorCollection propdesc = TypeDescriptor.GetProperties(item);

      foreach (PropertyDescriptor desc in propdesc)
      {
        Type proptype = desc.PropertyType;
        TypeConverter converter = TypeDescriptor.GetConverter(proptype);

        if (converter != null && converter.CanConvertTo(typeof(string)))
          props.Add(desc);
      }

      PropertyDescriptor [] columns = new PropertyDescriptor[props.Count];
      props.CopyTo (columns, 0);

      return columns;
    }
    #endregion

    #region Member Variables
    private string category_heading_;

    /// The data set for the system performance metrics.
    private object data_source_;

    /// The data member for the current snapshot.
    private string data_member_;
    private string data_member_baseline_;

    private string data_component_name_;
    private string data_component_name_baseline_;

    private string data_category_name_;

    private string data_hostname_;
    private string data_hostname_baseline_;

    private string data_metric_type_;
    private string data_metric_type_baseline_;

    private string data_src_name_;
    private string data_src_name_baseline_;

    private string data_dst_name_;
    private string data_dst_name_baseline_;

    private string data_event_count_;
    private string data_event_count_baseline_;

    private string data_best_time_;
    private string data_best_time_baseline_;

    private string data_avg_time_;
    private string data_avg_time_baseline_;

    private string data_worst_time_;
    private string data_worst_time_baseline_;

    #endregion
  }
}
