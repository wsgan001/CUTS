// This file was generated by:
// $Id$

#include "HelloWorldImpl.h"
#include "cuts/arch/ccm/CCM_Events_T.h"

namespace HelloWorldImpl
{
  //
  // HelloWorld
  //
  HelloWorld::HelloWorld (void)
  {
    this->periodic_EventCreator_.init (this, &HelloWorld::periodic_EventCreator);
    this->periodic_EventCreator_.configure (CUTS_Periodic_Event::PE_CONSTANT, 1);
    this->register_object (&this->periodic_EventCreator_);
  }

  //
  // ~HelloWorld
  //
  HelloWorld::~HelloWorld (void)
  {
  }

  //
  // sink: input_message
  //
  void HelloWorld::push_input_message (::Message * ev)
  {
    this->cpu1_.run (30);
    CUTS_CCM_Event_T <OBV_Message> __event_100000014__;
    __event_100000014__->content (ev->content ());
    this->ctx_->push_output_message (__event_100000014__.in ());

    ACE_UNUSED_ARG (ev);
  }

  //
  // PeriodicEvent: EventCreator
  //
  void HelloWorld::periodic_EventCreator (void)
  {
    CUTS_CCM_Event_T <OBV_Message> __event_100000019__;
    __event_100000019__->content (this->message_.c_str ());
    this->ctx_->push_handle_message_ex (__event_100000019__.in ());
  }

  //
  // attribute setter: message
  //
  void HelloWorld::message (const char * message)
  {
    this->message_ = message;
  }

  //
  // attribute getter: message
  //
  char * HelloWorld::message (void)
  {
    ::CORBA::String_var s =
      ::CORBA::string_dup (this->message_.c_str ());
    return s._retn ();
  }

  //
  // attribute getter: readonly_message
  //
  char * HelloWorld::readonly_message (void)
  {
    ::CORBA::String_var s =
      ::CORBA::string_dup (this->readonly_message_.c_str ());
    return s._retn ();
  }
}

//
// create_HelloWorld_Impl
//
::Components::EnterpriseComponent_ptr 
create_HelloWorld_Impl (void)
{
  ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

  ACE_NEW_RETURN (retval, 
                  ::HelloWorldImpl::HelloWorld (),
                  ::Components::EnterpriseComponent::_nil ());

  return retval;
}


// end of auto-generated file

