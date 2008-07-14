// $Id$

#include "TestLoggerClient_i.h"
#include "Test_Log_Message_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/OS_NS_unistd.h"

//
// CUTS_TestLoggerClient_i
//
CUTS_TestLoggerClient_i::CUTS_TestLoggerClient_i (void)
{
  // Get the hostname of the logging client.
  char hostname[1024];
  ACE_OS::hostname (hostname, sizeof (hostname));
  ACE_INET_Addr inet ((u_short)0, hostname, AF_ANY);

  this->hostname_.reset (ACE_OS::strdup (inet.get_host_name ()));
}

//
// ~CUTS_TestLoggerClient_i
//
CUTS_TestLoggerClient_i::~CUTS_TestLoggerClient_i (void)
{

}

//
// log
//
void CUTS_TestLoggerClient_i::log (const CUTS::LogMessage & msg)
{
  // Locate the handler for the test.
  CUTS_Test_Log_Message_Handler * handler = 0;
  int retval = this->handler_map_.find (msg.test, handler);

  if (retval == 0)
  {
    try
    {
      // First, dupliate the string. We store it in an auto pointer for
      // now just in case the new_message () method below fails. This
      // will ensure that the string's resources are released.
      ACE_Auto_String_Free msg_str (ACE_OS::strdup (msg.message.in ()));

      // Create a new log message for the message.
      CUTS_Log_Message * message = handler->new_message ();

      // Initialize the contents of the messages making sure to release
      // the auto string pointer.
      if (message != 0)
      {
        message->message_.reset (msg_str.release ());
        message->severity_ = msg.priority;
        message->timestamp_ = msg.timestamp;

        // Pass the message to the handler.
        handler->handle_message (message);
      }
      else
      {
        ACE_ERROR ((LM_ERROR,
                    "%T - %M - message creation failed; dropping a message "
                    "for test %d\n",
                    msg.test));
      }
    }
    catch (const ACE_bad_alloc & ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "%T - %M - %s; dropping a message for test %d\n",
                  ex.what (),
                  msg.test));

      throw CORBA::NO_MEMORY ();
    }
  }
}

//
// register_test
//
void CUTS_TestLoggerClient_i::
register_test (CORBA::Long new_test, CORBA::Long old_test)
{
  // First, unregister
  if (old_test != -1)
    this->unregister_test_i (old_test);

  if (new_test != -1)
  {
    ACE_DEBUG ((LM_INFO, 
                "%T - %M - registering an application for test %d\n",
                new_test));

    // Check if the test is already in the mapping.
    if (this->handler_map_.find (new_test) == 0)
      return;

    // We need to create a new handler for the test.
    CUTS_Test_Log_Message_Handler * handler = 0;

    ACE_NEW_THROW_EX (handler, 
                      CUTS_Test_Log_Message_Handler (this->conn_, 
                                                     new_test,
                                                     this->hostname_.get ()),
                      CORBA::NO_MEMORY ());
    ACE_Auto_Ptr <CUTS_Test_Log_Message_Handler> auto_clean (handler);

    // Try to insert the handler into the mapping. There is a chance
    // that another process beat us to this process.
    int retval = this->handler_map_.bind (new_test, handler);

    if (retval != -1)
    {
      if (retval == 1)
      {
        // Since another thread beat us to the registration, we need to
        // get the existing handler and increment the reference count.
        if (this->handler_map_.find (new_test, handler) == 0)
        {
          handler->increment ();
        }
        else
        {
          ACE_ERROR ((LM_ERROR,
                      "%T - %M - failed to locate handler for test %d; %m\n",
                      new_test));
        }
      }
      else
      {
        // Be sure to release the handler that actually registered itself
        // with the mapping. Otherwise, we will delete it and have a dangling
        // pointer in the mapping.
        auto_clean.release ();  

        // Start the handler.
        ACE_DEBUG ((LM_INFO,
                    "%T - %M - starting a new handler for test %d\n",
                    new_test));

        handler->start (this->timeout_);
      }
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
                  "%T - %M - failed to create handler for test %d; %m\n",
                  new_test));
    }
  }
}

//
// unregister_test
//
void CUTS_TestLoggerClient_i::unregister_test (CORBA::Long test)
{
  if (test != -1)
    this->unregister_test_i (test);
}

//
// unregister_test_i
//
void CUTS_TestLoggerClient_i::unregister_test_i (long test)
{
  map_type::ENTRY * entry = 0;
  int retval = this->handler_map_.find (test, entry);

  if (retval == 0)
  {
    ACE_DEBUG ((LM_INFO,
                "%T - %M - unregistering an application for test %d\n",
                test));

    // Decrement the reference count.
    CUTS_Test_Log_Message_Handler * handler = entry->item ();

    if (handler != 0)
    {
      long refcount = handler->decrement ();

      // Only when the reference count reaches 0 can we remove the handler
      // from the mapping. This means there are no more known loggers for
      // this test.
      if (refcount == 0)
      {
        this->handler_map_.unbind (entry);

        // Finally, we need to delete the handler's resources.
        ACE_DEBUG ((LM_INFO,
                    "%T - %M - no more applications registered for test %d\n",
                    test));

        handler->stop ();
        delete handler;
      }
    }
  }
}

//
// database_server_address
//
void CUTS_TestLoggerClient_i::
database_server_address (const ACE_CString & addr)
{
  this->server_addr_ = addr;

  try
  {
    // Establish a connection with the database.
    this->conn_.connect (CUTS_USERNAME, 
                         CUTS_PASSWORD,
                         this->server_addr_.c_str ());
  }
  catch (const CUTS_DB_Exception & ex)
  { 
    ACE_ERROR ((LM_ERROR,
                "%T - %M - %s\n",
                ex.message ().c_str ()));
  }
  catch (...)
  {
    ACE_ERROR ((LM_ERROR,
                "%T - %M - caught unknown exception; failed to connect to "
                "database on %s\n",
                this->server_addr_.c_str ()));
  }
}

//
// timeout_interval
//
void CUTS_TestLoggerClient_i::
timeout_interval (const ACE_Time_Value & timeout)
{
  this->timeout_ = timeout;
}