// $IdCHAOS_Publisher_Table_T.cpp 3289 2012-02-10 03:05:18Z hillj $

namespace iCCM
{

//
// CHAOS_Publisher_Table_T
//
template <typename EVENT>
CUTS_INLINE
CHAOS_Publisher_Table_T <EVENT>::CHAOS_Publisher_Table_T (void)
{

}

//
// ~CHAOS_Publisher_Table_T
//
template <typename EVENT>
CUTS_INLINE
CHAOS_Publisher_Table_T <EVENT>::~CHAOS_Publisher_Table_T (void)
{

}

//
// allocate_event
//
template <typename EVENT>
CUTS_INLINE
EVENT * CHAOS_Publisher_Table_T <EVENT>::allocate_event (void)
{
  if (this->impl_ != 0)
    return dynamic_cast <EVENT *> (this->impl_->allocate_event ());
  return 0;
}

//
// send_event
//
template <typename EVENT>
CUTS_INLINE
void CHAOS_Publisher_Table_T <EVENT>::send_event (EVENT * ev)
{
  if (this->impl_ != 0)
    this->impl_->send_event (ev);
}

}
