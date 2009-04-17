// -*- C++ -*-

//=============================================================================
/**
 *  @file         TCPIP_Servant.h
 *
 *  $Id$
 *
 *  @author       James H. Hill
 */
//=============================================================================

#ifndef _CUTS_TCPIP_SERVANT_H_
#define _CUTS_TCPIP_SERVANT_H_

#include "TCPIP_Object_T.h"

/**
 * @class CUTS_TCPIP_Servant
 *
 * Base class for all servant objects.
 */
class CUTS_TCPIP_Export CUTS_TCPIP_Servant :
  public CUTS_TCPIP_MT_Object
{
public:
  // Friend decl.
  friend class CUTS_TCPIP_Servant_Manager;

  /**
   * Get the UUID assigned to the object. If the object has not
   * be activated, then the UUID will be ACE_Utils::NIL_UUID.
   * Whenever the object is deactivated, then the UUID  will be
   * ACE_Utils::NIL_UUID.
   */
  const ACE_Utils::UUID & the_UUID (void) const;

  /**
   * Handle an event received on the TCP/IP port.
   *
   * @param[in]         id          Id of the event
   * @param[in]         ev          Message block containing event data
   * @retval            0           Success
   * @retval            -1          Failure
   */
  virtual int handle_event (ACE_UINT32 id, ACE_Message_Block & ev) = 0;

protected:
  /// Default constructor.
  CUTS_TCPIP_Servant (void);

  /// Destructor.
  virtual ~CUTS_TCPIP_Servant (void);

private:
  /// UUID assigned to the objeect.
  ACE_Utils::UUID uuid_;
};

#if defined (__CUTS_INLINE__)
#include "TCPIP_Servant.inl"
#endif

#endif  // !defined _CUTS_TCPIP_SERVANT_H_
