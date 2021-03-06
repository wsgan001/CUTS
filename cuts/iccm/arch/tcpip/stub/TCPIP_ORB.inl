// -*- C++ -*-
// $Id$

//
// TCPIP_ORB
//
CUTS_INLINE
TCPIP_ORB::TCPIP_ORB (void)
: listen_addr_ (CUTS_TCPIP_DEFAULT_ENDPOINT),
  obj_mgr_ (this),
  acceptor_ (obj_mgr_)
{
  CUTS_TCPIP_TRACE ("TCPIP_ORB::TCPIP_ORB (void)");
}

//
// ~TCPIP_ORB
//
CUTS_INLINE
TCPIP_ORB::~TCPIP_ORB (void)
{
  CUTS_TCPIP_TRACE ("TCPIP_ORB::~TCPIP_ORB (void)");
}

//
// the_OM
//
CUTS_INLINE
CUTS_TCPIP_Servant_Manager & TCPIP_ORB::the_OM (void)
{
  CUTS_TCPIP_TRACE ("TCPIP_ORB::the_OM (void)");
  return this->obj_mgr_;
}

//
// shutdown
//
CUTS_INLINE
int TCPIP_ORB::shutdown (void)
{
  CUTS_TCPIP_TRACE ("TCPIP_ORB::shutdown (void)");
  return this->reactor_->end_reactor_event_loop ();
}

//
// run
//
CUTS_INLINE
int TCPIP_ORB::run (void)
{
  CUTS_TCPIP_TRACE ("TCPIP_ORB::run (void)");
  return this->reactor_->run_reactor_event_loop ();
}

//
// listen_addr
//
CUTS_INLINE
const CUTS_TCPIP_Acceptor::PEER_ADDR & TCPIP_ORB::listen_addr (void) const
{
  return this->listen_addr_;
}
