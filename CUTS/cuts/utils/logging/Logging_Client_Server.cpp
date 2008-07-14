// $Id$

#include "Logging_Client_Server.h"
#include "TestLoggerClient_i.h"
#include "tao/IORTable/IORTable.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include <sstream>

static const char * __HELP__ =
"Logging client daemon for submitting log messages to a CUTS database\n"
"\n"
"USAGE: cutslog_d [OPTIONS]\n"
"\n"
"OPTIONS:\n"
"  --database=HOSTNAME   location of CUTS database (default='localhost')\n"
"  -t, --timeout=TIME    timeout interval to flush message queue\n"
"                        in seconds (default=30)\n"
"\n"
"  -v, --verbose         print verbose infomration\n" 
"  --debug               print debugging information\n"
"  -h, --help            print this help message\n";

//
// CUTS_Logging_Client_Server
//
CUTS_Logging_Client_Server::CUTS_Logging_Client_Server (void)
: server_addr_ ("localhost"),
  timeout_ (30)
{

}

//
// CUTS_Logging_Client_Server
//
CUTS_Logging_Client_Server::~CUTS_Logging_Client_Server (void)
{

}

//
// run_main
//
int CUTS_Logging_Client_Server::run_main (int argc, char * argv [])
{
  if (this->parse_args (argc, argv) == -1)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%T - %M - failed to parse command-line arguments\n"),
                       -1);
  }

  try
  {
    // Get a reference to the <RootPOA>
    ACE_DEBUG ((LM_DEBUG, 
                "%T - %M - resolving initial reference to RootPOA\n"));

    CORBA::Object_var obj = this->orb_->resolve_initial_references ("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());

    // Activate the RootPOA's manager.
    ACE_DEBUG ((LM_DEBUG, 
                "%T - %M - getting reference to POAManager\n"));
    PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
    mgr->activate ();

    // Create a new test logger client.
    ACE_NEW_THROW_EX (this->client_, 
                      CUTS_TestLoggerClient_i (), 
                      CORBA::NO_MEMORY ());

    // Activate the manager and transfer ownership.
    CUTS::TestLoggerClient_var client = this->client_->_this ();
    this->servant_ = this->client_;

    // Configure the test logger client.
    this->client_->database_server_address (this->server_addr_);

    ACE_Time_Value interval (this->timeout_);
    this->client_->timeout_interval (interval);

    // Register the test manager with the IORTable for the ORB.
    if (this->register_with_iortable () == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%T - %M - failed to register with IOR table\n"));
    }

    // Run the ORB's main event loop.
    ACE_DEBUG ((LM_DEBUG, 
                "%T - %M - running the server's main event loop\n"));

    this->orb_->run ();

    // Destroy the RootPOA.
    ACE_DEBUG ((LM_DEBUG, "%T - %M - destroying the RootPOA\n"));
    root_poa->destroy (true, true);

    // Destroy the ORB.
    ACE_DEBUG ((LM_DEBUG, "%T - %M - destroying the ORB\n"));
    this->orb_->destroy ();
    return 0;
  }
  catch (const CORBA::Exception & ex)
  {
    ACE_ERROR ((LM_ERROR,
                "%T - %M - %s\n",
                ex._info ().c_str ()));
  }
  catch (...)
  {
    ACE_ERROR ((LM_ERROR,
                "%T - %M - caught unknown exception\n"));
  }

  return -1;
}

//
// shutdown
//
int CUTS_Logging_Client_Server::shutdown (void)
{
  // Stop the ORB's main event loop.
  this->orb_->shutdown (true);
  return 0;
}

//
// parse_args
//
int CUTS_Logging_Client_Server::parse_args (int argc, char * argv [])
{
  // Initialize the ORB.
  this->orb_ = CORBA::ORB_init (argc, argv);

  // Parse the remaining command-line arguments.
  const char * opts = ACE_TEXT ("vht:");
  ACE_Get_Opt get_opt (argc, argv, opts);

  get_opt.long_option ("database", ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("timeout", 't', ACE_Get_Opt::ARG_REQUIRED);

  get_opt.long_option ("verbose", 'v', ACE_Get_Opt::NO_ARG);
  get_opt.long_option ("debug", ACE_Get_Opt::NO_ARG);
  get_opt.long_option ("help", 'h', ACE_Get_Opt::NO_ARG);

  char ch;

  while ((ch = get_opt ()) != EOF)
  {
    switch (ch)
    {
    case 0:
      if (ACE_OS::strcmp ("verbose", get_opt.long_option ()) == 0)
      {
        u_long mask = 
          ACE_Log_Msg::instance ()->priority_mask (ACE_Log_Msg::PROCESS);

        mask |= LM_INFO;

        ACE_Log_Msg::instance ()->priority_mask (mask, ACE_Log_Msg::PROCESS);
      }
      else if (ACE_OS::strcmp ("debug", get_opt.long_option ()) == 0)
      {
        u_long mask =
          ACE_Log_Msg::instance ()->priority_mask (ACE_Log_Msg::PROCESS);

        mask |= LM_DEBUG;

        ACE_Log_Msg::instance ()->priority_mask (mask, ACE_Log_Msg::PROCESS);
      }
      else if (ACE_OS::strcmp ("database", get_opt.long_option ()) == 0)
      {
        this->server_addr_ = get_opt.opt_arg ();
      }
      else if (ACE_OS::strcmp ("timeout", get_opt.long_option ()) == 0)
      {
        std::istringstream istr (get_opt.opt_arg ());
        istr >> this->timeout_;
      }
      else if (ACE_OS::strcmp ("help", get_opt.long_option ()) == 0)
      {
        this->print_help ();
      }

      break;

    case 't':
      {
        std::istringstream istr (get_opt.opt_arg ());
        istr >> this->timeout_;
      }
      break;

    case 'v':
      {
        u_long mask =
          ACE_Log_Msg::instance ()->priority_mask (ACE_Log_Msg::PROCESS);

        mask |= LM_INFO;

        ACE_Log_Msg::instance ()->priority_mask (mask, ACE_Log_Msg::PROCESS);
      }
      break;

    case 'h':
      this->print_help ();
      break;
    }
  }

  return 0;
}

//
// register_with_iortable
//
int CUTS_Logging_Client_Server::register_with_iortable (void)
{
  try
  {
    // Locate the IORTable for the application.
    CORBA::Object_var obj = this->orb_->resolve_initial_references ("IORTable");
    IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in ());

    if (::CORBA::is_nil (ior_table.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%T - %M - failed to resolve IOR table\n"),
                         -1);
    }

    // Get the IOR string for the test manager.
    CUTS::TestLoggerClient_var client = this->client_->_this ();
    CORBA::String_var obj_str  = this->orb_->object_to_string (client.in ());
    CORBA::String_var ior_name = CORBA::string_dup ("CUTS/TestLoggerClient");

    // Bind the object to the IOR table.
    ACE_DEBUG ((LM_DEBUG, 
                "%T - %M - registering test logger client with localhost\n"));

    ior_table->bind (ior_name.in (), obj_str.in ());
    return 0;
  }
  catch (const CORBA::Exception & ex)
  {
    ACE_ERROR ((LM_ERROR,
                "%T - %M - %s\n",
                ex._info ().c_str ()));
  }

  return -1;
}

//
// print_help
//
void CUTS_Logging_Client_Server::print_help (void)
{
  std::cout << ::__HELP__ << std::endl;
  ACE_OS::exit (0);
}