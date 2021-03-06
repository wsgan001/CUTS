// -*- C++ -*-

//=============================================================================
/**
 *  @file         EventConsumer.h
 *
 *  $Id$
 *
 *  @author       James H. Hill
 */
//=============================================================================

#ifndef _CUTS_ICCM_EVENTCONSUMER_H_
#define _CUTS_ICCM_EVENTCONSUMER_H_

#include "ccm/CCM_EventConsumerBaseS.h"

#include "Servant_export.h"
#include "PortProperties.h"

namespace iCCM
{

class ICCM_SERVANT_Export EventConsumer_Listener
{
public:
  EventConsumer_Listener (void);
  virtual ~EventConsumer_Listener (void) = 0;

  virtual void handle_event (::Components::EventBase *) = 0;
};

/**
 * @class EventConsumer
 *
 * Base class for all event consumer's that will be interfacing
 * with the Servant_T object.
 */
class ICCM_SERVANT_Export EventConsumer :
  public virtual POA_Components::EventConsumerBase
{
protected:
  /// Default constructor.
  EventConsumer (void);

public:
  /// Destructor.
  virtual ~EventConsumer (void);

  /// Method for sending a CORBA event.
  virtual void push_event (::Components::EventBase *);

  /// Method for activating the consumer
  virtual void activate (void);

  /// Method for passivating the consumer
  virtual void passivate (void);

  /// Configure this consumer's task
  virtual void configure_task (PortProperties * props);

  void listener (EventConsumer_Listener * listener);

private:
  /// Registered listener for events.
  EventConsumer_Listener * listener_;

  // prevent the following operations
  EventConsumer (const EventConsumer &);
  const EventConsumer & operator = (const EventConsumer &);
};

}

#if defined (__CUTS_INLINE__)
#include "EventConsumer.inl"
#endif

#endif  // !defined _CUTS_ICCM_EVENTCONSUMER_H_
