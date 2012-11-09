// This file was generated by:
// $Id$

#include "Hello_SenderImpl.h"
#include "cuts/arch/ccm/CCM_Events_T.h"

namespace Hello_SenderImpl
{
  //
  // Sender
  //
  Sender::Sender (void)
  {
    this->periodic_trigger_.init (this, &Sender::periodic_trigger);
    this->periodic_trigger_.configure (CUTS_Periodic_Event::PE_CONSTANT, 0.5);
    this->register_object (&this->periodic_trigger_);
  }

  //
  // ~Sender
  //
  Sender::~Sender (void)
  {
  }

  //
  // sink: read_message
  //
  void Sender::push_read_message (::Hello::ReadMessage * ev)
  {
    local_message("Hello again");
    ::Hello::PushMessage_var __event_100000028__ = this->ctx_->new_push_message_event ();
    __event_100000028__->data (local_message());
    this->ctx_->push_push_message (__event_100000028__.in ());

    ACE_UNUSED_ARG (ev);
  }

  //
  // PeriodicEvent: trigger
  //
  void Sender::periodic_trigger (void)
  {
    local_message("Trigger Hello");
    ::Hello::TimeOut_var __event_100000026__ = this->ctx_->new_click_out_event ();
    __event_100000026__->data (local_message());
    this->ctx_->push_click_out (__event_100000026__.in ());
  }

  //
  // attribute setter: local_message
  //
  void Sender::local_message (const char * local_message)
  {
    this->local_message_ = local_message;
  }

  //
  // attribute getter: local_message
  //
  char * Sender::local_message (void)
  {
    ::CORBA::String_var s =
      ::CORBA::string_dup (this->local_message_.c_str ());
    return s._retn ();
  }
}

//
// create_Hello_Sender_Impl
//
::Components::EnterpriseComponent_ptr
create_Hello_Sender_Impl (void)
{
  ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

  ACE_NEW_RETURN (retval,
                  ::Hello_SenderImpl::Sender (),
                  ::Components::EnterpriseComponent::_nil ());

  return retval;
}


// end of auto-generated file

