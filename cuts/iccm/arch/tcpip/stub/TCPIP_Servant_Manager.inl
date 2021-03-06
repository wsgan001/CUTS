// -*- C++ -*-

//
// CUTS_TCPIP_Servant_Manager
//
CUTS_INLINE
CUTS_TCPIP_Servant_Manager::CUTS_TCPIP_Servant_Manager (TCPIP_ORB * orb)
: orb_ (orb)
{
  ACE_Utils::UUID_GENERATOR::instance ()->init ();
}

//
// ~CUTS_TCPIP_Servant_Manager
//
CUTS_INLINE
CUTS_TCPIP_Servant_Manager::~CUTS_TCPIP_Servant_Manager (void)
{

}

//
// find_object
//
CUTS_INLINE
int
CUTS_TCPIP_Servant_Manager::
find_object (const ACE_Utils::UUID & uuid, iCCM::TCPIP_Servant * & object) const
{
  return this->active_objects_.find (uuid, object);
}
