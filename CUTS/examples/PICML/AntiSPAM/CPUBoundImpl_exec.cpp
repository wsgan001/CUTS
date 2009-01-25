// This file was generated by
// $Id$

#include "CPUBoundImpl_exec.h"
#include "cuts/CCM_Events_T.h"

namespace CIDL_CPUBoundImpl
{
  //
  // EventSink: push_InPortC
  //
  void CPUBound::push_InPortC (antispam::NullEvent * ev)
  {
    this->push_InPortC_.handle_event (ev);
  }

  //
  // push_InPortC_i
  //
  void CPUBound::push_InPortC_i (antispam::NullEvent * ev)
  {
    this->eventCountC_ ++;

    this->logEventC_ = this->eventCountC_ % this->logRate_ == 0;
    if (this->logEventC_)
    {
      this->logger_.log (LM_INFO, "%s port C is preprocessing event %d at %d", this->instName_.c_str (), this->eventCountC_, ACE_OS::gettimeofday ().msec ());
    }

    this->mycpu_.profile_run ("preC");
    if (this->logEventC_)
    {
      this->logger_.log (LM_INFO, "%s port C is postprocessing event %d at %d", this->instName_.c_str (), this->eventCountC_, ACE_OS::gettimeofday ().msec ());
    }

    CUTS_CCM_Event_T <OBV_antispam::NullEvent> __event_100000023__;
    __event_100000023__->eventNumber (ev->eventNumber ());
    __event_100000023__->sourceName (ev->sourceName ());
    this->ctx_->push_OutPortC (__event_100000023__.in ());

    this->mycpu_.profile_run ("postC");
    if (this->logEventC_)
    {
      this->logger_.log (LM_INFO, "%s port C is finalizing event %d at %d", this->instName_.c_str (), this->eventCountC_, ACE_OS::gettimeofday ().msec ());
    }

    ACE_UNUSED_ARG (ev);
  }

  //
  // EventSink: push_InPortE
  //
  void CPUBound::push_InPortE (antispam::NullEvent * ev)
  {
    this->push_InPortE_.handle_event (ev);
  }

  //
  // push_InPortE_i
  //
  void CPUBound::push_InPortE_i (antispam::NullEvent * ev)
  {
    this->eventCountE_ ++;

    this->logEventE_ = this->eventCountE_ % this->logRate_ == 0;
    if (this->logEventE_)
    {
      this->logger_.log (LM_INFO, "%s port E is preprocessing event %d at %d", this->instName_.c_str (), this->eventCountE_, ACE_OS::gettimeofday ().msec ());
    }

    this->mycpu_.profile_run ("preE");
    if (this->logEventE_)
    {
      this->logger_.log (LM_INFO, "%s port E is postprocessing event %d at %d", this->instName_.c_str (), this->eventCountE_, ACE_OS::gettimeofday ().msec ());
    }

    CUTS_CCM_Event_T <OBV_antispam::NullEvent> __event_100000035__;
    __event_100000035__->eventNumber (ev->eventNumber ());
    __event_100000035__->sourceName (ev->sourceName ());
    this->ctx_->push_OutPortE (__event_100000035__.in ());

    this->mycpu_.profile_run ("postE");
    if (this->logEventE_)
    {
      this->logger_.log (LM_INFO, "%s port E is finalizing event %d at %d", this->instName_.c_str (), this->eventCountE_, ACE_OS::gettimeofday ().msec ());
    }

    ACE_UNUSED_ARG (ev);
  }

  //
  // EventSink: push_InPortB
  //
  void CPUBound::push_InPortB (antispam::NullEvent * ev)
  {
    this->push_InPortB_.handle_event (ev);
  }

  //
  // push_InPortB_i
  //
  void CPUBound::push_InPortB_i (antispam::NullEvent * ev)
  {
    this->eventCountB_ ++;

    this->logEventB_ = this->eventCountB_ % this->logRate_ == 0;
    if (this->logEventB_)
    {
      this->logger_.log (LM_INFO, "%s port B is preprocessing event %d at %d", this->instName_.c_str (), this->eventCountB_, ACE_OS::gettimeofday ().msec ());
    }

    this->mycpu_.profile_run ("preB");
    if (this->logEventB_)
    {
      this->logger_.log (LM_INFO, "%s port B is postprocessing event %d at %d", this->instName_.c_str (), this->eventCountB_, ACE_OS::gettimeofday ().msec ());
    }

    CUTS_CCM_Event_T <OBV_antispam::NullEvent> __event_100000017__;
    __event_100000017__->eventNumber (ev->eventNumber ());
    __event_100000017__->sourceName (ev->sourceName ());
    this->ctx_->push_OutPortB (__event_100000017__.in ());

    this->mycpu_.profile_run ("postB");
    if (this->logEventB_)
    {
      this->logger_.log (LM_INFO, "%s port B is finalizing event %d at %d", this->instName_.c_str (), this->eventCountB_, ACE_OS::gettimeofday ().msec ());
    }

    ACE_UNUSED_ARG (ev);
  }

  //
  // EventSink: push_InPortA
  //
  void CPUBound::push_InPortA (antispam::NullEvent * ev)
  {
    this->push_InPortA_.handle_event (ev);
  }

  //
  // push_InPortA_i
  //
  void CPUBound::push_InPortA_i (antispam::NullEvent * ev)
  {
    this->eventCountA_ ++;

    this->logEventA_ = this->eventCountA_ % this->logRate_ == 0;
    if (this->logEventA_)
    {
      this->logger_.log (LM_INFO, "%s port A is preprocessing event %d at %d", this->instName_.c_str (), this->eventCountA_, ACE_OS::gettimeofday ().msec ());
    }

    this->mycpu_.profile_run ("preA");
    if (this->logEventA_)
    {
      this->logger_.log (LM_INFO, "%s port A is postprocessing event %d at %d", this->instName_.c_str (), this->eventCountA_, ACE_OS::gettimeofday ().msec ());
    }

    CUTS_CCM_Event_T <OBV_antispam::NullEvent> __event_100000011__;
    __event_100000011__->sourceName (ev->sourceName ());
    __event_100000011__->eventNumber (ev->eventNumber ());
    this->ctx_->push_OutPortA (__event_100000011__.in ());

    this->mycpu_.profile_run ("postA");
    if (this->logEventA_)
    {
      this->logger_.log (LM_INFO, "%s port A is finalizing event %d at %d", this->instName_.c_str (), this->eventCountA_, ACE_OS::gettimeofday ().msec ());
    }

    ACE_UNUSED_ARG (ev);
  }

  //
  // EventSink: push_InPortD
  //
  void CPUBound::push_InPortD (antispam::NullEvent * ev)
  {
    this->push_InPortD_.handle_event (ev);
  }

  //
  // push_InPortD_i
  //
  void CPUBound::push_InPortD_i (antispam::NullEvent * ev)
  {
    this->eventCountD_ ++;

    this->logEventD_ = this->eventCountD_ % this->logRate_ == 0;
    if (this->logEventD_)
    {
      this->logger_.log (LM_INFO, "%s port D is preprocessing event %d at %d", this->instName_.c_str (), this->eventCountD_, ACE_OS::gettimeofday ().msec ());
    }

    this->mycpu_.profile_run ("preD");
    if (this->logEventD_)
    {
      this->logger_.log (LM_INFO, "%s port D is postprocessing event %d at %d", this->instName_.c_str (), this->eventCountD_, ACE_OS::gettimeofday ().msec ());
    }

    CUTS_CCM_Event_T <OBV_antispam::NullEvent> __event_100000029__;
    __event_100000029__->eventNumber (ev->eventNumber ());
    __event_100000029__->sourceName (ev->sourceName ());
    this->ctx_->push_OutPortD (__event_100000029__.in ());

    this->mycpu_.profile_run ("postD");
    if (this->logEventD_)
    {
      this->logger_.log (LM_INFO, "%s port D is finalizing event %d at %d", this->instName_.c_str (), this->eventCountD_, ACE_OS::gettimeofday ().msec ());
    }

    ACE_UNUSED_ARG (ev);
  }

  //
  // logRate [getter]
  //
  ::CORBA::Long CPUBound::logRate (void)
  {
    return this->logRate_;
  }

  //
  // logRate [setter]
  //
  void CPUBound::logRate (::CORBA::Long logRate)
  {
    this->logRate_ = logRate;
  }

  //
  // testName [getter]
  //
  char * CPUBound::testName (void)
  {
    ::CORBA::String_var s =
      ::CORBA::string_dup (this->testName_.c_str ());
    return s._retn ();
  }

  //
  // testName [setter]
  //
  void CPUBound::testName (const char * testName)
  {
    this->testName_ = testName;
  }

  //
  // instName [getter]
  //
  char * CPUBound::instName (void)
  {
    ::CORBA::String_var s =
      ::CORBA::string_dup (this->instName_.c_str ());
    return s._retn ();
  }

  //
  // instName [setter]
  //
  void CPUBound::instName (const char * instName)
  {
    this->instName_ = instName;
  }

  //
  // Environment: activate
  //
  void CPUBound::ccm_activate (void)
  {
    this->logger_.configure (20000);
    this->logger_.connect_using_location (this->testName_);
    this->mycpu_.load_profile (this->instName_ + ".cpu");
    this->logger_.log (LM_INFO, "%s: preA=%f; postA=%f; preB=%f; postB=%f; preC=%f; postC=%f; preD=%f; postD=%f; preE=%f; postE=%f", this->instName_.c_str (), this->mycpu_.workload ("preA"), this->mycpu_.workload ("postA"), this->mycpu_.workload ("preB"), this->mycpu_.workload ("postB"), this->mycpu_.workload ("preC"), this->mycpu_.workload ("postC"), this->mycpu_.workload ("preD"), this->mycpu_.workload ("postD"), this->mycpu_.workload ("preE"), this->mycpu_.workload ("postE"));

    // pass control to base class
    base_type::ccm_activate ();
  }

  //
  // CPUBound
  //
  CPUBound::CPUBound (void)
  : logEventB_ (false),
    logEventC_ (false),
    eventCountE_ (0),
    logEventD_ (false),
    logEventE_ (false),
    eventCountC_ (0),
    logEventA_ (false),
    eventCountD_ (0),
    eventCountB_ (0),
    eventCountA_ (0)
  {
    this->push_InPortA_.init (this, &type::push_InPortA_i);
    this->register_object (&this->push_InPortA_);
    this->push_InPortA_.affinity_mask (1);
    this->push_InPortB_.init (this, &type::push_InPortB_i);
    this->register_object (&this->push_InPortB_);
    this->push_InPortB_.affinity_mask (1);
    this->push_InPortE_.init (this, &type::push_InPortE_i);
    this->register_object (&this->push_InPortE_);
    this->push_InPortE_.affinity_mask (1);
    this->push_InPortC_.init (this, &type::push_InPortC_i);
    this->register_object (&this->push_InPortC_);
    this->push_InPortC_.affinity_mask (1);
    this->push_InPortD_.init (this, &type::push_InPortD_i);
    this->register_object (&this->push_InPortD_);
    this->push_InPortD_.affinity_mask (1);
  }

  //
  // ~CPUBound
  //
  CPUBound::~CPUBound (void)
  {
  }

  //
  // CPUBoundHome_i
  //
  CPUBoundHome_i::CPUBoundHome_i (void)
  {
  }

  //
  // ~CPUBoundHome_i
  //
  CPUBoundHome_i::~CPUBoundHome_i (void)
  {
  }

  //
  // create
  //
  ::Components::EnterpriseComponent_ptr 
    CPUBoundHome_i::create (void) 
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (retval,
                      CPUBound (),
                      ::CORBA::NO_MEMORY ());

    return retval;
  }
}

//
// create_antispam_CPUBoundHome_Impl
//
::Components::HomeExecutorBase_ptr
create_antispam_CPUBoundHome_Impl (void) 
{
  ::Components::HomeExecutorBase_ptr retval =
    ::Components::HomeExecutorBase::_nil ();

  ACE_NEW_RETURN (retval,
                  ::CIDL_CPUBoundImpl::CPUBoundHome_i,
                  ::Components::HomeExecutorBase::_nil ());

  return retval;
}

