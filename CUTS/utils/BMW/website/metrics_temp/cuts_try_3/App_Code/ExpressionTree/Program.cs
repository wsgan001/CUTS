using System;
using System.Collections.Generic;
using System.Text;

using edu.vanderbilt.Trees;

/// @author James Edmondson

namespace ExpressionTreeTest
{
  class Program
  {
    static void Main (string[] args)
      {
        /// initialize interpreter parts
        Interpreter_Context context = new Interpreter_Context ();
        Interpreter interpreter = new Interpreter ();

        /// Expression to evaluate
        String throughput = "1.data.sent / 1.test.duration";
        String throughput_ids = "$_T.data.sent / $_T.test.duration";

        String mios_throughput = "1.mios.sent / 1.test.duration";
        String mios_throughput_full = "$_T.mios.sent / $_T.test.duration";

        /// non-initialized variables - filled in later
        
        Expression_Tree tree;

        /// set the test id.

        context.set ("$_T", 1);

        /// set some static variables
        /// "$_T.x" = 7000
        /// "$_T.y" = 20

        context.set ("1.data.sent", 7000);
        context.set ("1.test.duration", 20);

        context.set ("1.mios.sent", 40);
        
        /// "$_T.z" = " 1.x / 1.y"
        /// Run interpreter to figure out z's value

        tree = interpreter.interpret (ref context,throughput);

        /// set the evaluated throughput

        context.set ("1.data.throughput", tree.evaluate ());

        /// print out the test results.

        String data_through_modified = throughput_ids.Replace ("$_T", context.get ("$_T").ToString ());

        System.Console.WriteLine ("1.data.sent = " + context.get ("1.data.sent"));
        System.Console.WriteLine ("1.mios.sent = " + context.get ("1.mios.sent"));
        System.Console.WriteLine ("1.test.duration = " + context.get ("1.test.duration"));
        System.Console.WriteLine ("1.data.throughput = " + throughput + " = "
          + tree.ToString () + "= " + context.get ("1.data.throughput"));
        System.Console.WriteLine ("$_T.data.throughput = " + data_through_modified + " = "
          + tree.ToString () + "= " + context.get (context.get ("$_T") + ".data.throughput"));
        System.Console.WriteLine ();

        /// "$_T.z" = " 1.x / 1.y"
        /// Run interpreter to figure out z's value

        tree = interpreter.interpret (ref context, mios_throughput);

        /// set the evaluated throughput

        context.set ("1.mios.throughput", tree.evaluate ());

        System.Console.WriteLine ("1.mios.throughput = " + mios_throughput + " = "
          + tree.ToString () + "= " + context.get ("1.mios.throughput"));
        System.Console.WriteLine ("$_T.mios.throughput = " + mios_throughput_full + " = "
          + tree.ToString () + "= " + context.get (context.get ("$_T") + ".mios.throughput"));
        System.Console.WriteLine ();

      }
  }
}