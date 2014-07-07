// -*- C++ -*-

//=============================================================================
/**
 *  @file Portico_Component_Instance_Handler.h
 *
 *  $Id$
 *
 *  Code generated by iCCM skeleton project generator that is include
 *  in the CUTS (http://cuts.cs.iupui.edu) and developed by:
 *
 *      Software Engineering & Distributed System (SEDS) Group
 *      Indiana Univesity-Purdue University Indianapolis
 *      Indianapolis, IN 46202
 */
//=============================================================================

#ifndef _ICCM_PORTICO_COMPONENT_INSTANCE_HANDLER_H_
#define _ICCM_PORTICO_COMPONENT_INSTANCE_HANDLER_H_

#include "cuts/iccm/deployment/Component_Instance_Handler_T.h"
#include "Portico_Container.h"

namespace iCCM
{

/**
 * @class Portico_Component_Instance_Handler
 *
 * Portico implementation of the deployment handlers for component
 * instances.
 */
class  ICCM_PORTICO_DEPLOYMENT_HANDLERS_Export Portico_Component_Instance_Handler :
  public Component_Instance_Handler_T <Portico_Component_Instance_Handler,
                                      ::DAnCE::InstanceDeploymentHandler,
                                      Portico_Container>
{
public:
  struct FED
  {
    /// FED file.
    std::string file_;

    /// Target execution for the federate.
    std::string execution_;
  };

  /// Type definition of the base type.
  typedef Component_Instance_Handler_T <Portico_Component_Instance_Handler,
                                       ::DAnCE::InstanceDeploymentHandler,
                                       Portico_Container> base_type;

  /// Default constructor.
  Portico_Component_Instance_Handler (void);

  /// Destructor.
  virtual ~Portico_Component_Instance_Handler (void);

  /**
   * Configure the instance handler. The deployment properties passed
   * into this function are the ones attached to the Node in the
   * deployment model.
   *
   * @param[in]           config          Configuration properties
   */
  virtual void configure (const Deployment::Properties & config);

  /// Close the instance handler.
  virtual void close (void);

  /// Get a pointer to the RTI ambassador.
  const FED & get_fedinfo (void) const;

private:
  /// RTI FED information.
  FED fedinfo_;
};

}

extern "C"
ICCM_PORTICO_DEPLOYMENT_HANDLERS_Export
::DAnCE::InstanceDeploymentHandler_ptr
create_iCCM_Portico_Component_Instance_Handler (void);

#if defined (__CUTS_INLINE__)
#include "Portico_Component_Instance_Handler.inl"
#endif

#endif  // !defined _ICCM_Portico_COMPONENT_INSTANCE_HANDLER_H_