using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using MySql.Data.MySqlClient;
using Actions;
using System.Text.RegularExpressions;


public partial class Log_Formats : System.Web.UI.Page
{
  private CUTS.BMW_Master m_;
  private static double Default_Width_ = 800.0;

  protected void Page_Load ( object sender, EventArgs e )
  {
    m_ = (CUTS.BMW_Master)Master;
    if (IsPostBack)
      return;

    load_data();
  }

  private void load_data ()
  {
    /**
     * Hamilton:
     *
     * Please update this so that it uses a SQL stored procedure.
     */
    // Get all the test from the database.
    string sql = "SELECT * FROM logformatdesc;";
    DataTable dt = ExecuteMySqlAdapter( sql );

    // Data Bind
    this._table.DataSource = dt;
    this._table.DataBind();

    // Check for info
    if (dt.Rows.Count == 0)
      m_.AddNewMessage_Info("There are no log messages created yet. Please " +
        "add one.");

    // Ensure Widths
    if (this._table.Width.Value < Default_Width_)
      this._table.Width = new Unit( Default_Width_ );
    if (this.txt_New_LF.Width.Value < Default_Width_)
      this.txt_New_LF.Width = new Unit( Default_Width_ );

    // Make the ID column small
    this._table.Columns[0].ItemStyle.Width = new Unit( 10 );
    this._table.Columns[0].HeaderStyle.Width = new Unit( 10 );
    this._table.Columns[0].FooterStyle.Width = new Unit( 10 );

    // Setup the button better
    this.btn_New_LF.Height = this.txt_New_LF.Height;

  }


  protected void OnClick_btn_New_LF ( object sender, EventArgs e )
  {
    if (this.txt_New_LF.Text.Length < 3)
    {
      m_.AddNewMessage_Error( "The Log Format must be at least 3 characters!" );
      return;
    }
    try
    {
      string text = txt_New_LF.Text;
      string pattern = @"((?<lead>[-0-9a-z :;']+)(?<middle>{int (?<varname>[0-9a-z' ]+)}))";
      MatchCollection matches = Regex.Matches( text, pattern, RegexOptions.IgnoreCase );

      string lfmt = "", icase_regex = "", cs_regex = "";
      lfmt += txt_New_LF.Text;
      ArrayList vars = new ArrayList();
      foreach (Match NextMatch in matches)
      {
        string lead = NextMatch.Groups["lead"].ToString();
        string mid = NextMatch.Groups["middle"].ToString();
        string varname = NextMatch.Groups["varname"].ToString();
        icase_regex += lead + Regex.Replace( mid, "{int.+?}", "[[:digit:]]+", RegexOptions.IgnoreCase );

        // the group automatically names the captured variables correctly
        // You must use @ to escape the slashes in C#
        // You have to use two \\ to escape the \ in mySql
        // End result is stored and retrieved (as desired) as (?<varname>\d+)
        string group = "(?<" + varname + @">\\d+)";
        cs_regex += lead + Regex.Replace( mid, "{int.+?}", group, RegexOptions.IgnoreCase );
        vars.Add( varname );
      }

      LogFormatActions lf = new LogFormatActions();
      lf.Insert_LF( lfmt, icase_regex, cs_regex, vars.ToArray() );

      /**
       * Hamilton:
       *
       * Please update this so that it only prints a success message
       * without the actual log format that was entered. If the log format
       * is TOO long, then this will not display properly!!
       *
       * Moreove, you are only entering on message, so the user will no
       * what the message format is. ;-)
       */
      m_.AddNewMessage_Success( "'" + txt_New_LF.Text + "' added Successfully!" );

      this.load_data();
    }
    catch
    {
      m_.AddNewMessage_Error( "There was a problem adding '" +
        txt_New_LF.Text + "'. This probably means it is already created." );
    }
  }

    private DataTable ExecuteMySqlAdapter(string sql)
    {
        MySqlConnection conn = new MySqlConnection(ConfigurationManager.AppSettings["MySQL"]);
        conn.Open();

        MySqlDataAdapter da = new MySqlDataAdapter(sql, conn);
        DataSet ds = new DataSet();
        da.Fill(ds);

        conn.Close();
        return ds.Tables[0];
    }
}
