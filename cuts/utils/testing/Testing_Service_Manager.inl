// -*- C++ -*-
// $Id$

//
// CUTS_Testing_Service_Manager
//
CUTS_INLINE
CUTS_Testing_Service_Manager::
CUTS_Testing_Service_Manager (CUTS_Testing_App_Base * app)
: test_app_ (app),
  is_open_ (true)
{
  CUTS_TEST_TRACE ("CUTS_Testing_Service_Manager::CUTS_Testing_Service_Manager (CUTS_Testing_App &)");
}

//
// ~CUTS_Testing_Service_Manager
//
CUTS_INLINE
CUTS_Testing_Service_Manager::~CUTS_Testing_Service_Manager (void)
{
  CUTS_TEST_TRACE ("CUTS_Testing_Service_Manager::~CUTS_Testing_Service_Manager (void)");
  this->close ();
}
