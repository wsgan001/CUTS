// $Id$

#include "Node_Daemon_Event_Handler.h"
#include "Node_Daemon_i.h"

//
// Node_Daemon_Event_Handler
//
Node_Daemon_Event_Handler::
Node_Daemon_Event_Handler (CUTS_Node_Daemon_i & daemon)
: daemon_ (daemon),
  active_ (false)
{
  this->reactor (&this->reactor_);
}

//
// ~Node_Daemon_Event_Handler
//
Node_Daemon_Event_Handler::~Node_Daemon_Event_Handler (void)
{
  this->reactor (0);
}

//
// handle_exit
//
int Node_Daemon_Event_Handler::handle_exit (ACE_Process * process)
{
  if (process != 0)
  {
    this->daemon_.unmanage (process->getpid ());
  }
  else
  {
    ACE_ERROR ((LM_WARNING,
                "*%T (%t) - %M - unknown process has exited\n"));
  }

  return 0;
}

//
// handle_exit
//
int Node_Daemon_Event_Handler::
handle_timeout (const ACE_Time_Value & tv, const void * act)
{
  this->daemon_.clean ();

  ACE_UNUSED_ARG (tv);
  ACE_UNUSED_ARG (act);
  return 0;
}

//
// svc
//
int Node_Daemon_Event_Handler::svc (void)
{
  this->reactor ()->owner (ACE_OS::thr_self ());

  while (this->active_)
    this->reactor ()->handle_events ();

  return 0;
}

//
// activate
//
bool Node_Daemon_Event_Handler::activate (void)
{
  int retval = ACE_Task_Base::activate ();
  this->active_ = retval != -1;

  return this->active_;
}

//
// deactivate
//
void Node_Daemon_Event_Handler::deactivate (void)
{
  // Notify the reactor to stop waiting for events.
  this->active_ = false;
  this->reactor ()->notify ();

  // Wait for all threads to exit.
  this->wait ();
}
