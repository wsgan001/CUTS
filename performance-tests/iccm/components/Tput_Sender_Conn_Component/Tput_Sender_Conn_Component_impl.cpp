// This file was generated by:
// $Id$

#include "Tput_Sender_Conn_Component_impl.h"
#include "ace/streams.h"

namespace CIAO_Sender_Conn_Component_Impl
{
  //
  // Sender_Conn_Component
  //
  Sender_Conn_Component::Sender_Conn_Component (void)
    : datasize_ (0)
  {
  }

  //
  // ~Sender_Conn_Component
  //
  Sender_Conn_Component::~Sender_Conn_Component (void)
  {
  }

  //
  // ccm_activate
  //
  void Sender_Conn_Component::ccm_activate (void)
  {
    ACE_ERROR ((LM_DEBUG,
                ACE_TEXT ("%T (%t) - %M - running test with data size\n"),
                this->datasize_));

    switch (this->datasize_)
    {
    case 8:
      this->task_.reset (
        new SenderComponent_Task_T <::Size8DataConnector::Writer, Size8Data> (
        this->ctx_->get_connection_write_size8data_data ()));
      break;

    case 16:
      this->task_.reset (
        new SenderComponent_Task_T <::Size16DataConnector::Writer, Size16Data> (
        this->ctx_->get_connection_write_size16data_data ()));
      break;

    case 32:
      this->task_.reset (
        new SenderComponent_Task_T <::Size32DataConnector::Writer, Size32Data> (
        this->ctx_->get_connection_write_size32data_data ()));
      break;

    case 64:
      this->task_.reset (
        new SenderComponent_Task_T <::Size64DataConnector::Writer, Size64Data> (
        this->ctx_->get_connection_write_size64data_data ()));
      break;

    default:
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%T (%t) - %M - unknown data size %d\n"),
                  this->datasize_));
    }

    if (0 != this->task_.get ())
      this->task_->activate ();

    // Start the timer.
    this->timer_.start ();
  }

  //
  // ccm_passivate
  //
  void Sender_Conn_Component::ccm_passivate (void)
  {
    // Store the task, and stop the timer.
    this->timer_.stop ();

    if (0 != this->task_.get ())
    {
      this->task_->deactivate ();
      this->task_->wait ();
    }

    // Calculate the events/second.
    ACE_hrtime_t elapsed;
    this->timer_.elapsed_time (elapsed);

    if (0 != this->task_.get ())
    {
      double tput = ((double)this->task_->count () / (double)elapsed) * (double)ACE_ONE_SECOND_IN_NSECS;

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("# events sent: %d\n")
                  ACE_TEXT ("sender throughput: %f\n"),
                  this->task_->count (),
                  tput));
    }
  }

  //
  // datasize
  //
  void Sender_Conn_Component::datasize (CORBA::UShort val)
  {
    this->datasize_ = val;
  }

  //
  // datasize
  //
  CORBA::UShort Sender_Conn_Component::datasize (void)
  {
    return this->datasize_;
  }
}

//
// create_Tput_Sender_Conn_Component_Impl
//
::Components::EnterpriseComponent_ptr
create_Tput_Sender_Conn_Component_Impl (void)
{
  ::Components::EnterpriseComponent_ptr retval =
    ::Components::EnterpriseComponent::_nil ();

  ACE_NEW_RETURN (retval,
                  ::CIAO_Sender_Conn_Component_Impl::Sender_Conn_Component (),
                  ::Components::EnterpriseComponent::_nil ());

  return retval;
}
