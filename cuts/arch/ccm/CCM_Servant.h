// -*- C++ -*-

//=============================================================================
/**
 *  @file         TCPIP_CCM_Servant.h
 *
 *  $Id$
 *
 *  @author       James H. Hill
 */
//=============================================================================

#ifndef _CUTS_CCM_SERVANT_H_
#define _CUTS_CCM_SERVANT_H_

#include "tao/PortableServer/Servant_Base.h"
#include "ccm/CCM_StandardConfiguratorC.h"
#include "ace/SString.h"
#include "CCM_export.h"

/**
 * @class CUTS_CCM_Servant
 *
 * Base class for the CCM servant. This class defines the required
 * methods for interfacing with the CUTS_CCM_Container object.
 */
class CUTS_CCM_Export CUTS_CCM_Servant :
  public virtual ::PortableServer::ServantBase
{
public:
  /**
   * Default constructor.
   */
  CUTS_CCM_Servant (const char * name);

  /// Destructor.
  virtual ~CUTS_CCM_Servant (void);

  /// Method for activating a component.
  virtual void activate_component (void);

  /// Method for passivating a component.
  virtual void passivate_component (void);

  /// Method for removing a component.
  virtual void remove (void);

  /**
   * Get the instance's name.
   *
   * @return        Name of the instance.
   */
  virtual const ACE_CString & name (void) const;

  virtual void set_attributes (const ::Components::ConfigValues &);

protected:
  /// Name of the CCM servant.
  ACE_CString name_;

private:
  CUTS_CCM_Servant (void);
};

#if defined (__CUTS_INLINE__)
#include "CCM_Servant.inl"
#endif

#endif  // !defined _CUTS_TCPIP_CCM_SERVANT_H_
