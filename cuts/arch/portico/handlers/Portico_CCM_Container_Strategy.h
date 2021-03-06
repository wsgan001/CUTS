// -*- C++ -*-

//=============================================================================
/**
 *  @file         Portico_CCM_Container_Strategy.h
 *
 *  $Id$
 *
 *  Code generated by iCCM skeleton project generator that is include
 *  in the CUTS (http://cuts.cs.iupui.edu) and developed by:
 *
 *      Software Engineering & Distributed System (SEDS) Group
 *      Indiana Univesity-Purdue University Indianapolis
 *      Indianapolis, IN 46202
 *      USA
 */
//=============================================================================

#ifndef _PORTICO_CCM_CONTAINER_STRATEGY_H_
#define _PORTICO_CCM_CONTAINER_STRATEGY_H_

#include "cuts/arch/ccm/CCM_Container_Strategy_T.h"
#include "Portico_CCM_Deployment_Handlers_export.h"

// Forward decl.
class Portico_CCM_Container;

// Forward decl.
class Portico_CCM_Servant;

/// Type defintion of Portico_CCM_Container_Strategy base type.
typedef
  CUTS_CCM_Container_Strategy_T <Portico_CCM_Container>
                                 Portico_CCM_Container_Strategy_Base;

/**
 * @class Portico_CCM_Container_Strategy
 *
 * Implementation of the CCM container startegy for %(archName)s.
 */
class Portico_CCM_Container_Strategy :
  public Portico_CCM_Container_Strategy_Base
{
public:
  /**
   * Initializing constructor
   *
   * @param[in]     container       Container hosting the strategy
   */
  Portico_CCM_Container_Strategy (Portico_CCM_Container & container);

  /// Destructor.
  virtual ~Portico_CCM_Container_Strategy (void);

  /**
   * Callback for installing a servant. Implementing this method is
   * optional. It primarily is used to do architecture-specific installation
   * procedure above and beyond DAnCE.
   *
   * @param[in]     servant         Servant to install
   */
  virtual void
    install_servant (::PortableServer::Servant servant);

  /**
   * Configure the servant. The configuration values are those
   * attached to a component instance.
   *
   * @param[in]     servant         Servant to configure
   * @param[in]     values          Configuration values
   */
  virtual void configure_servant (::PortableServer::Servant servant,
                                  const ::Components::ConfigValues & values);

  /**
   * Callback for removing a servant. Implementing this method is
   * optional. If you implement install_servant (), then it is
   * recommended that you implement this method to provide consist
   * management of the servants.
   *
   * @param[in]     servant         Servant to install
   */
  virtual void remove_servant (::PortableServer::Servant servant);
};

#if defined (__CUTS_INLINE__)
#include "Portico_CCM_Container_Strategy.inl"
#endif

#endif  // !defined _PORTICO_CCM_CONTAINER_STRATEGY_H_