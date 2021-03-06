// $Id$

#if !defined (__CUTS_INLINE__)
#include "QpidPB_Servant_T.inl"
#endif

namespace iCCM
{

//
// configure_broker
//
template <typename T, typename CONTEXT, typename EXECUTOR, typename POA_EXEC>
void QpidPB_Servant_T <T, CONTEXT, EXECUTOR, POA_EXEC>::
configure_broker (const std::string & host, int port)
{
  // Configure the broker on the publishers (publishes and emits)
  typename emits_map_type::ITERATOR emits_iter (this->emits_);
  for (; !emits_iter.done (); ++emits_iter)
    emits_iter->item ()->configure_broker (host, port);

  typename publishes_map_type::ITERATOR publishes_iter (this->publishes_);
  for (; !publishes_iter.done (); ++publishes_iter)
    publishes_iter->item ()->configure_broker (host, port);
}

//
// set_queues
//
template <typename T, typename CONTEXT, typename EXECUTOR, typename POA_EXEC>
void QpidPB_Servant_T <T, CONTEXT, EXECUTOR, POA_EXEC>::
set_queues (bool prefix_name)
{
  std::string prefix;

  if (prefix_name)
  {
    prefix = this->name_.c_str ();
    prefix += ":";
  }

  // Configure the broker on the publishers (publishes and emits)
  typename emits_map_type::ITERATOR emits_iter (this->emits_);
  for (; !emits_iter.done (); ++emits_iter)
    emits_iter->item ()->queue (prefix + emits_iter->key ().c_str ());

  typename publishes_map_type::ITERATOR publishes_iter (this->publishes_);
  for (; !publishes_iter.done (); ++publishes_iter)
    publishes_iter->item ()->queue (prefix + publishes_iter->key ().c_str ());
}

}
