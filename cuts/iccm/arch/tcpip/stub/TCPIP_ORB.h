// -*- C++ -*-

//=============================================================================
/**
 *  @file         TCPIP_ORB.h
 *
 *  $Id$
 *
 *  @author       James H. Hill
 */
//=============================================================================

#ifndef _CUTS_TCPIP_ORB_H_
#define _CUTS_TCPIP_ORB_H_

#include "ace/SOCK_Acceptor.h"
#include "TCPIP_Acceptor.h"
#include "TCPIP_Servant_Manager.h"

// Forward decl.
class CUTS_TCPIP_Servant_Manager;

/**
 * @class CUTS_TCPIP_ORB
 */
class CUTS_TCPIP_Export TCPIP_ORB
{
public:
  /// Default constructor.
  TCPIP_ORB (void);

  /// Destructor.
  virtual ~TCPIP_ORB (void);

  /**
   * Initialize the ORB.
   */
  int init (int argc, char * argv []);

  /**
   * Get the object manager for the ORB.
   *
   * @return        Reference to the object manager.
   */
  CUTS_TCPIP_Servant_Manager & the_OM (void);

  int run (void);

  int shutdown (void);

  /// Address the ORB is listening.
  const CUTS_TCPIP_Acceptor::PEER_ADDR & listen_addr (void) const;

private:
  /// Helper method to parse the command-line arguments.
  int parse_args (int argc, char * argv []);

  /// IP-address to listen for incoming connections.
  CUTS_TCPIP_Acceptor::PEER_ADDR listen_addr_;

  /// Object manager for the ORB.
  CUTS_TCPIP_Servant_Manager obj_mgr_;

  /// Acceptor for receiving TCP/IP connections.
  CUTS_TCPIP_Acceptor acceptor_;

  /// Reactor for the ORB.
  ACE_Auto_Ptr <ACE_Reactor> reactor_;
};

#if defined (__CUTS_INLINE__)
#include "TCPIP_ORB.inl"
#endif

#endif  // !defined _CUTS_TCPIP_ORB_H_
