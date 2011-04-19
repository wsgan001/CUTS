// -*- C++ -*-

//=============================================================================
/**
 *  @file         OpenSplice_Publisher_Table_T.h
 *
 *  $Id$
 *
 *  @author       James H. Hill
 */
//=============================================================================

#ifndef _CUTS_OPENSPLICE_CCM_PUBLISHER_TABLE_T_H_
#define _CUTS_OPENSPLICE_CCM_PUBLISHER_TABLE_T_H_

#include "ace/Hash_Map_Manager.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/UUID.h"

#include "OpenSplice_Publisher_T.h"
#include "OpenSplice_Publisher_Table.h"

/**
 * @class CUTS_OpenSplice_CCM_Publisher_Table_T
 */
template <typename EVENT>
class CUTS_OpenSplice_CCM_Publisher_Table_T :
  public CUTS_OpenSplice_CCM_Publisher_Table
{
public:
  typedef CUTS_OpenSplice_Traits_T <EVENT> traits_type;

  typedef CUTS_OpenSplice_CCM_Publisher_T <EVENT> subscriber_type;

  CUTS_OpenSplice_CCM_Publisher_Table_T (void);

  virtual ~CUTS_OpenSplice_CCM_Publisher_Table_T (void);

  virtual ::Components::Cookie * subscribe (::Components::EventConsumerBase_ptr);

  virtual ::Components::EventConsumerBase_ptr unsubscribe (::Components::Cookie *);

  void send_event (typename traits_type::corba_event_type * ev);

protected:
  typedef ACE_Hash_Map_Manager <ACE_Utils::UUID,
                                subscriber_type *,
                                ACE_RW_Thread_Mutex> table_type;

  table_type table_;
};

#include "OpenSplice_Publisher_Table_T.cpp"

#endif  // !defined _CUTS_OPENSPLICE_CCM_PUBLISHER_TABLE_T_H_