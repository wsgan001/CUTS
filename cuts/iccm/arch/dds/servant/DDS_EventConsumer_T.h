// -*- C++ -*-

//=============================================================================
/**
 *  @file         DDS_EventConsumer_T.h
 *
 *  $Id$
 *
 *  @author       James H. Hill
 */
//=============================================================================

#ifndef _ICCM_DDS_EVENTCONSUMER_T_H_
#define _ICCM_DDS_EVENTCONSUMER_T_H_

#include "DDS_EventConsumer.h"
#include "DDS_Event_Listener_T.h"

namespace iCCM
{

/**
 * @class DDS_EventConsumer_T
 */
template <typename T, typename SERVANT, typename EVENT>
class DDS_EventConsumer_T : public DDS_EventConsumer <T>
{
public:
  /// Type definition of the servant type.
  typedef SERVANT servant_type;

  /// Type definition of the event type.
  typedef EVENT event_type;

  /// Type definition of the listener type.
  typedef DDS_Event_Listener_T <T, SERVANT, EVENT> listener_type;

  /// Type definition of the servant callback method.
  typedef typename listener_type::DESERIALIZE_METHOD DESERIALIZE_METHOD;

  typedef typename listener_type::event_traits_type event_traits_type;

  /**
   * Initializing constructor.
   *
   * @param[in]         servant         The parent servant.
   * @param[in]         callback        Deserialized method on servant.
   */
  DDS_EventConsumer_T (SERVANT * servant,
                       DESERIALIZE_METHOD callback);

  /// Destructor.
  virtual ~DDS_EventConsumer_T (void);

  // Configure the event consumer object.
  virtual void configure (subscriber_ptr_type subscriber,
                          const topicqos_type & topic_qos,
                          const datareaderqos_type & qos);

  /**
   * Add a topic to the event consumer. This will results in a new
   * event listener if the topic has not already been added.
   */
  virtual void add_topic (const char * topic);

  /**
   * Remove a topic from the event consumer. This will delete the
   * associated event listener. If the topic does not exist, then
   * nothing happens.
   */
  virtual void remove_topic (const char * topic);

private:
  /// Servant to pass event.
  SERVANT * servant_;

  /// Method for deserializing an event.
  DESERIALIZE_METHOD callback_;

  /// The concrete reader for this consumer.
  typename event_traits_type::reader_var_type reader_;

  /// Collection of registered listeners.
  ACE_Hash_Map_Manager <ACE_CString,
                        listener_type *,
                        ACE_RW_Thread_Mutex> listeners_;
};

}

#if defined (__CUTS_INLINE__)
#include "DDS_EventConsumer_T.inl"
#endif

#include "DDS_EventConsumer_T.cpp"

#endif  // !definef _CUTS_OPENSPLICE_EVENTCONSUMER_T_H_