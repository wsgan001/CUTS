// $Id$

#if !defined (__CUTS_INLINE__)
#include "DDS_Stateful_Writer_T.inl"
#endif  // !defined __CUTS_INLINE__

namespace iCCM
{

///////////////////////////////////////////////////////////////////////////////
// class DDS_Stateful_Writer_T

//
// DDS_Stateful_Writer_T
//
template <typename T, typename EVENT>
CUTS_INLINE
DDS_Stateful_Writer_T <T, EVENT>::
DDS_Stateful_Writer_T (typename T::datawriter_ptr_type writer)
: writer_ (T::template _writer_cast < typename event_traits_type::writer_type > (writer))
{

}

///////////////////////////////////////////////////////////////////////////////
// class DDS_Unregistered_Instance_Writer_T

//
// DDS_Unregistered_Instance_Writer_T
//
template <typename T, typename EVENT>
DDS_Unregistered_Instance_Writer_T <T, EVENT>::
DDS_Unregistered_Instance_Writer_T (typename T::datawriter_ptr_type writer)
: DDS_Stateful_Writer_T <T, EVENT> (writer)
{

}

//
// push_event
//
template <typename T, typename EVENT>
void DDS_Unregistered_Instance_Writer_T <T, EVENT>::send_event (EVENT * ev)
{
  // Convert the CORBA event into a DDS event.
  typedef typename event_traits_type::downcall_event_type downcall_event_type;
  downcall_event_type * downcall = dynamic_cast <downcall_event_type *> (ev);

  if (0 != ev)
  {
    typedef typename T::returncode_type returncode_type;
    #ifdef ICCM_DDS_USES_POINTERS
      returncode_type retcode = this->writer_->write (&downcall->dds_event (), T::HANDLE_NIL);
    #else
      returncode_type retcode = this->writer_->write (downcall->dds_event (), T::HANDLE_NIL);
    #endif

    if (retcode != T::RETCODE_OK)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%T (%t) - %M - failed to write event [retcode=%d]\n"),
                  retcode));
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%T (%t) - %M - invalid event type\n")));
  }
}

//
// allocate_event
//
template <typename T, typename EVENT>
EVENT * DDS_Unregistered_Instance_Writer_T <T, EVENT>::allocate_event (void)
{
  typedef typename event_traits_type::downcall_event_type downcall_event_type;

  downcall_event_type * ev = 0;
  ACE_NEW_THROW_EX (ev,
                    downcall_event_type (),
                    ::CORBA::NO_MEMORY ());

  return ev;
}

///////////////////////////////////////////////////////////////////////////////
// class DDS_Registered_Instance_Writer_T

//
// DDS_Registered_Instance_Writer_T
//
template <typename T, typename EVENT>
DDS_Registered_Instance_Writer_T <T, EVENT>::
DDS_Registered_Instance_Writer_T (typename T::datawriter_ptr_type writer)
: DDS_Stateful_Writer_T <T, EVENT> (writer),
  inst_ (T::HANDLE_NIL)
{

}

//
// activate
//
template <typename T, typename EVENT>
void DDS_Registered_Instance_Writer_T <T, EVENT>::activate (void)
{
  this->register_instance ();
}

//
// passivate
//
template <typename T, typename EVENT>
void DDS_Registered_Instance_Writer_T <T, EVENT>::passivate (void)
{
  this->unregister_instance ();
}

//
// register_instance
//
template <typename T, typename EVENT>
void DDS_Registered_Instance_Writer_T <T, EVENT>::register_instance (void)
{
#ifdef ICCM_DDS_USES_POINTERS
  this->inst_ = this->writer_->register_instance (&this->event_.dds_event ());
#else
  this->inst_ = this->writer_->register_instance (this->event_.dds_event ());
#endif
}

//
// unregister_instance
//
template <typename T, typename EVENT>
void DDS_Registered_Instance_Writer_T <T, EVENT>::unregister_instance (void)
{
#ifdef ICCM_DDS_USES_POINTERS
  this->writer_->unregister_instance (&this->event_.dds_event (), this->inst_);
#else
  this->writer_->unregister_instance (this->event_.dds_event (), this->inst_);
#endif
}

//
// push_event
//
template <typename T, typename EVENT>
void DDS_Registered_Instance_Writer_T <T, EVENT>::send_event (EVENT * ev)
{
  typedef typename T::returncode_type returncode_type;
  #ifdef ICCM_DDS_USES_POINTERS
    returncode_type retcode = this->writer_->write (&this->event_.dds_event (), this->inst_);
  #else
    returncode_type retcode = this->writer_->write (this->event_.dds_event (), this->inst_);
  #endif

  if (retcode != T::RETCODE_OK)
  {
    // Sending failed, register instance and try again
    this->register_instance ();
    #ifdef ICCM_DDS_USES_POINTERS
      retcode = this->writer_->write (&this->event_.dds_event (), this->inst_);
    #else
      retcode = this->writer_->write (this->event_.dds_event (), this->inst_);
    #endif

    if (retcode != T::RETCODE_OK)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%T (%t) - %M - failed to write event [retcode=%d]\n"),
                  retcode));
    }
  }

  ACE_UNUSED_ARG (ev);
}

//
// allocate_event
//
template <typename T, typename EVENT>
EVENT * DDS_Registered_Instance_Writer_T <T, EVENT>::allocate_event (void)
{
  this->event_._add_ref ();
  return &this->event_;
}

}
