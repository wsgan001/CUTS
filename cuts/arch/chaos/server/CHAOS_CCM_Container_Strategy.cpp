// -*- C++ -*-
// $Id$

#include "CHAOS_CCM_Container_Strategy.h"

#if !defined (__CUTS_INLINE__)
#include "CHAOS_CCM_Container_Strategy.inl"
#endif

#include "CHAOS_CCM_Container.h"
#include "CHAOS_CCM_ComponentServer.h"
#include "../ccm/CHAOS_CCM_Servant.h"

//
// CUTS_CHAOS_CCM_Container_Strategy
//
CUTS_CHAOS_CCM_Container_Strategy::
CUTS_CHAOS_CCM_Container_Strategy (CUTS_CHAOS_CCM_Container & container)
: CUTS_CHAOS_CCM_Container_Strategy_Base (container)
{
}

//
// configure_servant
//
void
CUTS_CHAOS_CCM_Container_Strategy::
configure_servant (::PortableServer::Servant servant,
                   const ::Components::ConfigValues & config)
{
  // Register the servant with the TCP/IP object manager.
  CUTS_CHAOS_CCM_Servant * chaos_servant =
    dynamic_cast <CUTS_CHAOS_CCM_Servant *> (servant);

  if (0 == chaos_servant)
    return;

  ACE_DEBUG ((LM_DEBUG,
	      "%T (%t) - %M - activating the TCP/IP servant\n"));

  int retval = 
    this->container_.server ()->the_ORB ().the_OM ().activate_object (chaos_servant);

  if (0 != retval)
    ACE_ERROR ((LM_ERROR,
		"%T (%t) - %M - failed to activate TCP/IP servant\n"));

  // Configure the OpenSplice servant.
  if (0 != this->ospl_configurator_.configure (chaos_servant, config))
    ACE_ERROR ((LM_ERROR,
		"%T (%t) - %M - failed to configure the servant\n"));

  ACE_DEBUG ((LM_DEBUG, 
	      "%T - %M - done configuring the servant\n"));
}

//
// configure_servant
//
void
CUTS_CHAOS_CCM_Container_Strategy::
remove_servant (::PortableServer::Servant servant)
{
  // Unregister the servant with the TCP/IP object manager.
  CUTS_CHAOS_CCM_Servant * chaos_servant =
    dynamic_cast <CUTS_CHAOS_CCM_Servant *> (servant);

  if (0 != chaos_servant)
  {
    ACE_DEBUG ((LM_DEBUG,
                "%T (%t) - %M - deactivating the TCP/IP servant\n"));

    const ACE_Utils::UUID & uuid = chaos_servant->the_UUID ();
    this->container_.server ()->the_ORB ().the_OM ().deactivate_object (uuid);
  }

  // INSERT OPENSPLICE HERE
}
