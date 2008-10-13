// -*- C# -**

using System;
using System.Collections;
using System.Data;
using System.Data.Common;
using System.Text.RegularExpressions;

using CUTS.Data;
using CUTS.Graph;

namespace CUTS.Data.UnitTesting
{
  /**
   * @class UnitTestEvaluator
   *
   * Data abstraction for evaluating a unit test.
   */
  public class UnitTestEvaluator
  {
    /**
     * Database connection for the evaluator.
     */
    private DbProviderFactory factory_;

    private DbConnection conn_;

    private String location_;

    /**
     * Initializing constructor.
     */
    public UnitTestEvaluator (DbProviderFactory factory, String location)
    {
      this.factory_ = factory;
      this.location_ = location;
    }

    /**
     * Destructor.
     */
    ~UnitTestEvaluator ()
    {
      this.Close ();
    }

    /**
     * Open the connection to the database for the evaluator.
     *
     * @param[in]       connstr       Connection string.
     */
    public void Open (string connstr)
    {
      if (this.conn_ == null)
        this.conn_ = this.factory_.CreateConnection ();

      // Close the current database connection.
      this.Close ();

      // Open a new connection to the database.
      this.conn_.ConnectionString = connstr;
      this.conn_.Open ();
    }

    /**
     * Close the evaluator.
     */
    public void Close ()
    {
      if (this.conn_.State == ConnectionState.Open)
        this.conn_.Close ();
    }

    /**
     * Re-evaluate a unit test.
     */
    public DataTable Reevaluate (int test,
                                 int utid,
                                 bool aggr,
                                 out string[] group_name,
                                 out string eval)
    {
      // Create a new variable table for this unit test.
      UnitTestVariableTable vtable = new UnitTestVariableTable (this.location_);

      try
      {
        // Open the variable table.
        vtable.Open (test, utid);
        DbTransaction transaction = vtable.BeginTransaction ();

        try
        {
          // Get the variable table for this unit test. This table is then sent
          // back to the database for processing, i.e., to execute the user-defined
          // expression to evaluate the unit test.
          this.create_variable_table (ref vtable);

          // Commit the transaction.
          transaction.Commit ();
        }
        catch (Exception)
        {
          transaction.Rollback ();
          throw;
        }

        return this.evaluate_i (vtable, aggr, out group_name, out eval);
      }
      finally
      {
        vtable.Close ();
      }
    }

    /**
     * Evaluate the unit test of a specific test. When evaluating the unit
     * test, the user has the option of either aggregating the results, or
     * getting back the raw data for the result.
     *
     * @param[in]       test       Test number of interest
     * @param[in]       utid       Unit test of interest
     * @param[in]       aggr       Apply aggregation function
     * @param[out]      eval       Evaluation function
     */
    public DataTable Evaluate (int test,
                               int utid,
                               bool aggr,
                               out string[] group_name,
                               out string eval)
    {
      // Create a new variable table for this unit test.
      UnitTestVariableTable vtable =
        new UnitTestVariableTable (this.location_);

      try
      {
        // Open the variable table.
        vtable.Open (test, utid);
        DataTable table = vtable.Data;

        // Begin a new transaction.
        return this.evaluate_i (vtable, aggr, out group_name, out eval);
      }
      finally
      {
        vtable.Close ();
      }
    }

    /**
     * Helper method to create the variable table for a unit test. This
     * will create the columns for the variable table, as well as fill in
     * all the rows - while maintaining causility in the data.
     *
     * @param[in]       test          Test of interest
     * @param[in]       utid          Unit test to evaluate
     * @param[out]      vtable        Target variable table
     */
    private void create_variable_table (ref UnitTestVariableTable vtable)
    {
      if (this.conn_ == null || this.conn_.State != ConnectionState.Open)
        throw new Exception ("Connection to database is not open");

      // Initialize the database objects.
      DataSet ds = new DataSet ();
      DbCommand command = this.conn_.CreateCommand ();
      DbDataAdapter adapter = this.factory_.CreateDataAdapter ();
      adapter.SelectCommand = command;

      // Create a parameter for the command.
      DbParameter p1 = command.CreateParameter ();

      // Initialize the parameter.
      p1.ParameterName = "?utid";
      p1.DbType = DbType.Int32;
      p1.Value = vtable.UnitTestNumber;

      command.Parameters.Add (p1);

      // Get the variabes for this unit test.
      command.CommandText = "CALL cuts.select_unit_test_variables (?utid)";
      adapter.Fill (ds, "variables");

      // Get the relations for this unit test.
      command.CommandText = "CALL cuts.select_unit_test_relations_as_set (?utid)";
      adapter.Fill (ds, "relations");

      // Get the cuasality derived from the relations.
      command.CommandText = "CALL cuts.select_unit_test_causal_relations (?utid)";
      adapter.Fill (ds, "causality");

      // Get all the log formats for this unit test.
      command.CommandText = "CALL cuts.select_unit_test_log_formats (?utid)";
      adapter.Fill (ds, "logformats");

      // First, create a graph using the log formats and their
      // relations. The log formats will be the nodes, and the
      // relations will be the edges.
      CUTS.Graph.DirectedGraph log_graph = new CUTS.Graph.DirectedGraph ();

      foreach (DataRow row in ds.Tables["logformats"].Rows)
      {
        // Create a node for the log format.
        int lfid = (int)row["lfid"];
        string name = String.Format ("LF{0}", lfid);
        CUTS.Graph.Node node = new CUTS.Graph.Node (name);

        // Set the properties of the node.
        node.property ("lfid", lfid);
        node.property ("regex.csharp", row["csharp_regex"]);
        node.property ("regex.mysql", row["icase_regex"]);

        // Insert the node into the graph.
        log_graph.insert_node (node);
      }

      ArrayList relation_set = new ArrayList ();

      foreach (DataRow row in ds.Tables["causality"].Rows)
      {
        // Get the name of the source/target nodes.
        string source = "LF" + row["cause"];
        string target = "LF" + row["effect"];

        // Create an edge between the two nodes.
        log_graph.create_edge (source, target);

        // Get the source node of the causality.
        CUTS.Graph.Node source_node = log_graph.node (source);

        Relation relation;
        string filter = String.Format ("relid = {0}", row["relid"]);
        DataRow[] relations = ds.Tables["relations"].Select (filter, "rel_index ASC");

        if (relations.Length != 0)
        {
          // Create a new relation for this log format.
          relation = new Relation ();

          foreach (DataRow equality in relations)
          {
            string lhs_eq = (string)equality["lhs"];
            string rhs_eq = (string)equality["rhs"];

            // Insert the next equality into the relation.
            relation.add (lhs_eq, rhs_eq);
          }

          // Store the relation with the source node.
          ArrayList r;

          if (source_node.properties.ContainsKey ("relation"))
          {
            r = (ArrayList)source_node.property ("relation");
          }
          else
          {
            r = new ArrayList ();
            source_node.property ("relation", r);
          }

          // Add the relation to this node's relation set.
          r.Add (relation);

          // Save the relation.
          if (!relation_set.Contains (relation))
            relation_set.Add (relation);
        }
      }

      // Do a topological sort of the log formats. This will give us the
      // order in which we must process the formats to extract the correct
      // log messages, and ensure causality.
      CUTS.Graph.Node[] sorted_node_list =
        CUTS.Graph.Algorithm.topological_sort (log_graph);

      // Create the variables for this unit test. This involves getting
      // the variables from the database, and creating data table based
      // on the variable types and their names.
      Hashtable vars = new Hashtable ();

      foreach (DataRow row in ds.Tables["variables"].Rows)
        vars.Add (row["fq_name"], row["vartype"]);

      vtable.Create (vars, true);
      vtable.CreateIndices ((Relation[])relation_set.ToArray (typeof (Relation)));

      // Prepare the command that will be used to select the log messages
      // based on the log formats of the current unit test.
      DbCommand logdata_command = (DbCommand)this.conn_.CreateCommand ();
      adapter.SelectCommand = logdata_command;

      logdata_command.CommandText =
        "CALL cuts.select_log_data_asc_by_test_number (?test, ?lfid)";

      DbParameter test_param = logdata_command.CreateParameter ();
      test_param.ParameterName = "?test";
      test_param.DbType = DbType.Int32;
      test_param.Value = vtable.TestNumber;
      logdata_command.Parameters.Add (test_param);

      DbParameter lfid_param = logdata_command.CreateParameter ();
      lfid_param.ParameterName = "?lfid";
      lfid_param.DbType = DbType.Int32;
      logdata_command.Parameters.Add (lfid_param);

      // Iterate over each of the log formats and select the log messages
      // for the current test that match the format. The log message are
      // returned in order of [hostname, msgtime].
      DataTable logdata = new DataTable ();
      Hashtable variables = new Hashtable ();

      foreach (CUTS.Graph.Node node in sorted_node_list)
      {
        // Set the parameter value for the log format.
        int lfid = (int)node.property ("lfid");
        lfid_param.Value = lfid;

        // Select the variables for this log format.
        string lfid_filter = String.Format ("lfid = {0}", lfid);

        DataRow[] log_variables = ds.Tables["variables"].Select (lfid_filter);
        Hashtable log_vars = new Hashtable ();
        ArrayList rel_list = new ArrayList ();

        foreach (DataRow info in log_variables)
        {
          log_vars.Add (info["varname"], info["fq_name"]);
          rel_list.Add (info["fq_name"]);
        }

        // Create a regular expression to locate the variables in the
        // each of the log messages.
        String csharp_regex = (String)node.property ("regex.csharp");
        Regex regex = new Regex (csharp_regex);

        // Get the log data for this log format.
        if (logdata.Rows.Count != 0)
          logdata.Clear ();

        adapter.Fill (logdata);

        // Get the relation for this log format.
        ArrayList relations = (ArrayList)node.property ("relation");

        // Create a data entry object for this log format.
        LogFormatDataEntry entry;

        if (relations == null)
        {
          entry = new LogFormatDataEntry (vtable,
                                          (String[])rel_list.ToArray (typeof (String)));

          this.process_log_messages (entry, regex, log_vars, logdata);
        }
        else
        {
          foreach (Relation relation in relations)
          {
            entry = new LogFormatDataEntry (vtable,
                                            (String[])rel_list.ToArray (typeof (String)),
                                            relation);

            this.process_log_messages (entry, regex, log_vars, logdata);
          }
        }

        //// Clear the variable hash table.
        //if (variables.Count > 0)
        //  variables.Clear ();


        //foreach (Relation relation in relations)
        //{
        //  while (reader.Read ())
        //  {
        //    // Apply the C# regular expression to the log message.
        //    String varname, fq_name;
        //    String message = (String)reader["message"];
        //    Match variable_match = variable_regex.Match (message);

        //    // Get all the variables from this log message.
        //    foreach (DictionaryEntry v in log_vars)
        //    {
        //      // Get the name of the variable.
        //      varname = (String)v.Key;
        //      fq_name = (String)v.Value;

        //      // Get the value of the variable from the match.
        //      variables[fq_name] =
        //        variable_match.Groups[varname].Captures[0].Value;
        //    }

        //    // Process the set of variables.
        //    entry.Process (variables);
        //  }

        //  // Close the reader for the next set of data.
        //  reader.Close ();
        //}
      }
    }

    private void process_log_messages (LogFormatDataEntry entry,
                                       Regex regex,
                                       Hashtable variables,
                                       DataTable logdata)
    {
      String varname, fq_name, message;
      Match match;
      Hashtable parameter = new Hashtable ();

      foreach (DataRow row in logdata.Rows)
      {
        if (parameter.Count != 0)
          parameter.Clear ();

        // Apply the C# regular expression to the log message.
        message = (String)row["message"];
        match = regex.Match (message);

        // Get all the variables from this log message.
        foreach (DictionaryEntry var in variables)
        {
          // Get the name of the variable.
          varname = (String)var.Key;
          fq_name = (String)var.Value;

          // Get the value of the variable from the match.
          parameter[fq_name] = match.Groups[varname].Captures[0].Value;
        }

        // Process the set of variables.
        entry.Process (parameter);
      }
    }

    private DataTable evaluate_i (UnitTestVariableTable vtable,
                                  bool aggr,
                                  out string[] group_name,
                                  out string eval)
    {
      // SQL statements for extracting data to construct the final SQL
      // that calculates the result for the unit test.
      string sql_eval =
        "SELECT evaluation AS eval, REPLACE(evaluation, '.', '_') AS eval_escaped, aggregration_function AS aggr " +
        "FROM cuts.unit_tests WHERE utid = ?utid";

      DbCommand command = this.conn_.CreateCommand ();
      command.CommandText = sql_eval;

      DbParameter p1 = command.CreateParameter ();

      p1.ParameterName = "?utid";
      p1.DbType = DbType.Int32;
      p1.Value = vtable.UnitTestNumber;
      command.Parameters.Add (p1);

      // Execute the SQL statement.
      DataSet ds = new DataSet ();
      DbDataAdapter adapter = this.factory_.CreateDataAdapter ();
      adapter.SelectCommand = command;

      command.CommandText = sql_eval;
      adapter.Fill (ds, "evaluation");

      // Get the evaluation and aggregation functions.
      string eval_stmt = (string)ds.Tables["evaluation"].Rows[0]["eval"];
      string eval_escaped_stmt = (string)ds.Tables["evaluation"].Rows[0]["eval_escaped"];
      string aggr_stmt = (string)ds.Tables["evaluation"].Rows[0]["aggr"];

      // Get the groupings for the unit test. At this point, we need
      // to create a comma separate list of the group variables. This will
      // be used to construct the GROUP BY statement.
      string sql_grouping = "CALL cuts.select_unit_test_grouping (?utid)";
      command.CommandText = sql_grouping;
      adapter.Fill (ds, "groupings");

      ArrayList group_list = new ArrayList ();

      foreach (DataRow row in ds.Tables["groupings"].Rows)
      {
        string name = (string)row["fq_name"];
        group_list.Add (name.Replace (".", "_"));
      }

      group_name = (string[])group_list.ToArray (typeof (string));
      string grouping = String.Join (",", group_name);

      // Finally, construct the entire SQL statement for the evaluation.
      string sql_result;

      if (aggr)
      {
        sql_result = "SELECT ";

        if (grouping != String.Empty)
          sql_result += grouping + ",";

        sql_result +=
          aggr_stmt + "(" + eval_escaped_stmt + ") AS result FROM vtable";

        eval = aggr_stmt + "(" + eval_stmt + ")";
      }
      else
      {
        sql_result = "SELECT ";

        if (grouping != String.Empty)
          sql_result += grouping + ",";

        sql_result +=
          eval_escaped_stmt + " AS result FROM vtable";

        eval = eval_stmt;
      }

      if (grouping != String.Empty)
      {
        if (aggr)
          sql_result += " GROUP BY " + grouping;
        else
          sql_result += " ORDER BY " + grouping;
      }

      DataTable table = new DataTable ("result");
      vtable.Evaluate (sql_result, ref table);

      return table;
    }
  }
}
