// $Id$

#include "Unite_App.h"

#include "Dataflow_Graph.h"
#include "Dataflow_Graph_Builder.h"
#include "Dataset_Result.h"
#include "Unite_Config_File.h"
#include "Unite_Datagraph_File.h"
#include "Unite_Aspect_File.h"
#include "Unite_Test.h"
#include "Unite_Test_Builder.h"
#include "Dataflow_Graph_Analyzer.h"

#include "presentation/console/Console_Presentation_Service.h"
#include "cuts/utils/testing/Test_Database.h"

#include "ace/Get_Opt.h"
#include "ace/Env_Value_T.h"
#include "ace/High_Res_Timer.h"

#include "XSC/utils/XML_Error_Handler.h"

static const char * __HELP__ =
"UNITE - a QoS unit test evaluation engine\n"
"\n"
"USAGE: cuts-unite [OPTIONS]\n"
"\n"
"General options:\n"
"  -f, --datafile=FILE       CUTS database that contains the syestem traces\n"
"  -c, --config=FILE         configuration file for evaluation\n"
"  --datagraph=FILE          override existing datagraph in configuration\n"
"  --aspect=FILE             apply aspect to evaluation\n"
"\n"
"  --repo=PATH               directory of dataset repo (default=:memory:)\n"
"\n"
"  --show-trend              show the data trend for the test\n"
"\n"
"Service options:\n"
"  --disable=NAME            disable service with id NAME\n"
"\n"
"Output options:\n"
"  -h, --help                print this help message\n"
"  -v, --verbose             print more information\n"
"  -d, --debug               print debugging information\n";

class load_service
{
public:
  typedef ::CUTS::XML::serviceList::service_iterator::value_type value_type;

  load_service (CUTS_Unite_Presentation_Service_Manager & mgr)
    : mgr_ (mgr)
  {

  }

  void operator () (const value_type & value) const
  {
    this->mgr_.load_service (value->id ().c_str (),
                             value->location ().c_str (),
                             value->classname ().c_str (),
                             value->params_p () ? value->params ().c_str () : 0);
  }

private:
#if __GNUC__ > 4 || \
    (__GNUC__ == 4 && (__GNUC_MINOR__ > 5))
  CUTS_Unite_Presentation_Service_Manager & mgr_;
#else
  mutable CUTS_Unite_Presentation_Service_Manager & mgr_;
#endif
};


//
// CUTS_Unite_App
//
CUTS_Unite_App::CUTS_Unite_App (void)
: repo_location_ (":memory:"),
  show_trend_ (false)
{
  this->svc_mgr_.open (ACE_TEXT ("cuts-unite"),
                       ACE_DEFAULT_LOGGER_KEY,
                       false,
                       true,
                       true);

  this->svc_mgr_.process_directive (ace_svc_desc_CUTS_Console_Presentation_Service);
}

//
// CUTS_Unite_App
//
CUTS_Unite_App::~CUTS_Unite_App (void)
{

}

//
// run_main
//
int CUTS_Unite_App::run_main (int argc, char * argv [])
{
  try
  {
    ACE_High_Res_Timer timer;
    timer.start ();

    if (this->parse_args (argc, argv) == -1)
      return -1;

    // Load the configuration file.
    CUTS_Unite_Config_File config_file;
    ::CUTS::XML::testConfig config ("", "", "", ::CUTS::XML::datagraphLink (""));

    XSC::XML::XML_Error_Handler error_handler;
    config_file->setErrorHandler (&error_handler);

    if (config_file.read (this->config_.c_str ()))
      config_file >>= config;

    // Load the services.
    if (config.services_p ())
      this->load_services (config.services ());

    // Construct the binary version of the unit test.
    CUTS_Unite_Test unite_test;
    CUTS_Unite_Test_Builder builder;

    if (!builder.build (config, unite_test))
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%T (%t) - %M - failed to build unit test in %s\n"),
                         this->config_.c_str ()),
                         -1);

    // Load the XML document that contains the log formats.
    // Process the log formats.
    CUTS_Unite_Datagraph_File datagraph_file;
    datagraph_file->setErrorHandler (&error_handler);

    ::CUTS::XML::datagraphType datagraph ("");

    if (this->datagraph_.empty ())
    {
      if (datagraph_file.read (config.datagraph ().location ().c_str ()))
        datagraph_file >>= datagraph;
    }
    else
    {
      if (datagraph_file.read (this->datagraph_.c_str ()))
        datagraph_file >>= datagraph;
    }

    // Build the graph for this unit test.
    CUTS_Dataflow_Graph graph;
    CUTS_Dataflow_Graph_Builder graph_builder;

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%T (%t) - %M - building datagraph; please wait...\n")));

    if (!graph_builder.build (datagraph, graph))
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%T (%t) - %M - failed to build unit test graph %s\n"),
                         datagraph.name ().c_str ()),
                         -1);


    // Finally, load the aspect, if applicable, and convert it into a
    // WHERE clause for the SQL statement.
    ACE_Auto_Ptr <CUTS_UNITE_Aspect> aspect;

    if (!this->aspect_file_.empty ())
    {
      CUTS_UNITE_Aspect * temp = 0;
      ACE_NEW_RETURN (temp, CUTS_UNITE_Aspect (), -1);

      aspect.reset (temp);

      // Open the XML document for reading.
      CUTS_Unite_Aspect_File aspect_file;
      aspect_file->setErrorHandler (&error_handler);

      if (!aspect_file.read (this->aspect_file_.c_str ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%T (%t) - %M - failed to load aspect file %s\n"),
                           this->aspect_file_.c_str ()),
                           -1);

      // Load the information from the file.
      ::CUTS::XML::aspectType aspect_type ("");
      aspect_file >>= aspect_type;

      // Store the infomration from the XML file.
      aspect->condition_ = aspect_type.condition ().c_str ();

      if (aspect_type.viewpoint_p ())
      {
        aspect->viewpoint_.lower_ = aspect_type.viewpoint ().before ();
        aspect->viewpoint_.upper_ = aspect_type.viewpoint ().after ();
      }
    }

    //ACE_High_Res_Timer timer;
    //timer.start ();

    // Construct the variable table
    CUTS_Dataset_Repo * repo = this->construct_vtable (graph);

    ACE_Auto_Ptr <CUTS_Dataset_Repo> auto_clean (repo);

    CUTS_Dataset_Result result (*repo);

    int retval = result.evaluate (unite_test,
                                  graph.name (),
                                  aspect.get (),
                                  !this->show_trend_);

    if (0 != retval)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%T (%t) - %M - failed to evaluate test %s [vtable=%s]\n"),
                         unite_test.name ().c_str (),
                         graph.name ().c_str ()),
                         -1);

    timer.stop ();

    ACE_Time_Value elapsed;
    timer.elapsed_time (elapsed);

    std::cout << "Evaluation time: "
              << elapsed.sec () << "." << elapsed.usec () << std::endl;

    // Present the results to the end-user.
    this->svc_mgr_.handle_result (result);

    return 0;
  }
  catch (const ADBC::Exception & ex)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%T (%t) - %M - %s (%N:%l)\n"),
                ex.message ().c_str ()));
  }

  return -1;
}

//
// parse_args
//
int CUTS_Unite_App::parse_args (int argc, char * argv [])
{
  const char * optstr = "hc:f:vd";

  ACE_Get_Opt get_opt (argc, argv, optstr);

  get_opt.long_option ("aspect", ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("config", 'c', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("datafile", 'f', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("datagraph", ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("repo", ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("show-trend");
  get_opt.long_option ("disable", ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("help", 'h');
  get_opt.long_option ("verbose", 'v');
  get_opt.long_option ("debug", 'd');

  int ch;

  while ((ch = get_opt ()) != EOF)
  {
    switch (ch)
    {
    case 0:
      if (ACE_OS::strcmp (get_opt.long_option (), "config") == 0)
      {
        this->config_ = get_opt.opt_arg ();
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "aspect") == 0)
      {
        this->aspect_file_ = get_opt.opt_arg ();
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "datafile") == 0)
      {
        this->datafile_ = get_opt.opt_arg ();
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "datagraph") == 0)
      {
        this->datagraph_ = get_opt.opt_arg ();
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "repo") == 0)
      {
        this->repo_location_ = get_opt.opt_arg ();
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "help") == 0)
      {
        this->print_help ();
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "show-trend") == 0)
      {
        this->show_trend_ = true;
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "aspect") == 0)
      {
        this->aspect_file_ = get_opt.opt_arg ();
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "disable") == 0)
      {
        this->disables_.insert (get_opt.opt_arg ());
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "verbose") == 0)
      {
        u_long mask =
          ACE_Log_Msg::instance ()->priority_mask (ACE_Log_Msg::PROCESS);

        mask |= LM_INFO;

        ACE_Log_Msg::instance ()->priority_mask (mask, ACE_Log_Msg::PROCESS);
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "debug") == 0)
      {
        u_long mask =
          ACE_Log_Msg::instance ()->priority_mask (ACE_Log_Msg::PROCESS);

        mask |= LM_DEBUG;

        ACE_Log_Msg::instance ()->priority_mask (mask, ACE_Log_Msg::PROCESS);
      }

      break;

    case 'c':
      this->config_ = get_opt.opt_arg ();
      break;

    case 'f':
      this->datafile_ = get_opt.opt_arg ();
      break;

    case 'h':
      this->print_help ();
      break;

    case 'v':
      {
        u_long mask =
          ACE_Log_Msg::instance ()->priority_mask (ACE_Log_Msg::PROCESS);

        mask |= LM_INFO;

        ACE_Log_Msg::instance ()->priority_mask (mask, ACE_Log_Msg::PROCESS);
      }
      break;

    case 'd':
      {
        u_long mask =
          ACE_Log_Msg::instance ()->priority_mask (ACE_Log_Msg::PROCESS);

        mask |= LM_DEBUG;

        ACE_Log_Msg::instance ()->priority_mask (mask, ACE_Log_Msg::PROCESS);
      }
      break;
    }
  }

  return 0;
}

//
// print_help
//
void CUTS_Unite_App::print_help (void)
{
  std::cerr << __HELP__ << std::endl;
  ACE_OS::exit (0);
}

//
// load_services
//
void CUTS_Unite_App::
load_services (const ::CUTS::XML::serviceList & list)
{
  std::for_each (list.begin_service (),
                 list.end_service (),
                 load_service (this->svc_mgr_));

  // Disable the service listed on the command-line.
  string_set::CONST_ITERATOR iter (this->disables_);

  for (; !iter.done (); ++ iter)
    this->svc_mgr_.suspend ((*iter).c_str ());
}

//
// split_graph
//

CUTS_Dataset_Repo * CUTS_Unite_App::construct_vtable (CUTS_Dataflow_Graph & graph)
{
  CUTS_Test_Database testdata;

  if (!testdata.open (this->datafile_))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%T (%t) - %M - failed to open %s\n"),
                this->datafile_.c_str ()));

  CUTS_Dataflow_Graph_Analyzer analyzer (&graph);
  analyzer.analyze (testdata, this->repo_location_);

  return analyzer.join (testdata, this->repo_location_);
}