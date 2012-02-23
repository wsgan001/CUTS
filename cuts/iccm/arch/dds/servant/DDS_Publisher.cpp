// $Id$

#if !defined (__CUTS_INLINE__)
#include "DDS_Publisher.inl"
#endif

namespace iCCM
{

//
// ~DDS_Publisher
//
template <typename T>
DDS_Publisher <T>::~DDS_Publisher (void)
{
  if (!T::_is_nil (this->abs_writer_))
  {
    typedef typename T::publisher_var_type publisher_var_type;
    typedef typename T::returncode_type returncode_type;

    // Destroy the data writer owned by this event producer.
    publisher_var_type publisher = this->abs_writer_->get_publisher ();
    returncode_type retcode = publisher->delete_datawriter (this->abs_writer_);

    if (retcode != 0)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%T (%t) - %M - failed to delete data writer [retcode = %d]\n"),
                  retcode));
  }
}

}
