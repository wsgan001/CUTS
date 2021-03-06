// This file was generated by
// $Id$

#include "NullEventGeneratorImpl_exec.h"
#include "cuts/CCM_Events_T.h"

namespace CIDL_NullEventGeneratorImpl
{
  //
  // eventGenerator
  //
  void NullEventGenerator::periodic_eventGenerator (void)
  {
    ++ this->eventNumber_;

    CUTS_CCM_Event_T <OBV_antispam::NullEvent> __event_100000047__;
    __event_100000047__->sourceName (this->instName_.c_str ());
    __event_100000047__->eventNumber (this->eventNumber_);

    ACE_Time_Value tv = ACE_OS::gettimeofday ();
    this->ctx_->push_NullEvent (__event_100000047__.in ());

    this->logger_.log (LM_INFO, "%s sent event %d at %d",
                       this->instName_.c_str (),
                       this->eventNumber_,
                       tv.msec ());
  }

  //
  // testName [getter]
  //
  char * NullEventGenerator::testName (void)
  {
    ::CORBA::String_var s =
      ::CORBA::string_dup (this->testName_.c_str ());
    return s._retn ();
  }

  //
  // testName [setter]
  //
  void NullEventGenerator::testName (const char * testName)
  {
    this->testName_ = testName;
  }

  //
  // publishHertz [getter]
  //
  ::CORBA::Double NullEventGenerator::publishHertz (void)
  {
    return this->publishHertz_;
  }

  //
  // publishHertz [setter]
  //
  void NullEventGenerator::publishHertz (::CORBA::Double publishHertz)
  {
    this->publishHertz_ = publishHertz;
  }

  //
  // instName [getter]
  //
  char * NullEventGenerator::instName (void)
  {
    ::CORBA::String_var s =
      ::CORBA::string_dup (this->instName_.c_str ());
    return s._retn ();
  }

  //
  // instName [setter]
  //
  void NullEventGenerator::instName (const char * instName)
  {
    this->instName_ = instName;
  }

  //
  // Environment: activate
  //
  void NullEventGenerator::ccm_activate (void)
  {
    this->logger_.configure (20000);
    this->logger_.connect_using_location (this->testName_);
    this->logger_.log (LM_INFO, "generator %s hertz is %f", this->instName_.c_str (), this->publishHertz_);

    // configure the event generators
    this->periodic_eventGenerator_.configure (CUTS_Periodic_Event::PE_EXPONENTIAL, this->publishHertz_);

    // pass control to base class
    base_type::ccm_activate ();
  }

  //
  // NullEventGenerator
  //
  NullEventGenerator::NullEventGenerator (void)
  : eventNumber_ (0)
  {
    this->periodic_eventGenerator_.init (this, &type::periodic_eventGenerator);
    this->register_object (&this->periodic_eventGenerator_);
  }

  //
  // ~NullEventGenerator
  //
  NullEventGenerator::~NullEventGenerator (void)
  {
  }

  //
  // NullEventGeneratorHome_i
  //
  NullEventGeneratorHome_i::NullEventGeneratorHome_i (void)
  {
  }

  //
  // ~NullEventGeneratorHome_i
  //
  NullEventGeneratorHome_i::~NullEventGeneratorHome_i (void)
  {
  }

  //
  // create
  //
  ::Components::EnterpriseComponent_ptr
    NullEventGeneratorHome_i::create (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (retval,
                      NullEventGenerator (),
                      ::CORBA::NO_MEMORY ());

    return retval;
  }
}

//
// create_antispam_NullEventGeneratorHome_Impl
//
::Components::HomeExecutorBase_ptr
create_antispam_NullEventGeneratorHome_Impl (void)
{
  ::Components::HomeExecutorBase_ptr retval =
    ::Components::HomeExecutorBase::_nil ();

  ACE_NEW_RETURN (retval,
                  ::CIDL_NullEventGeneratorImpl::NullEventGeneratorHome_i,
                  ::Components::HomeExecutorBase::_nil ());

  return retval;
}

