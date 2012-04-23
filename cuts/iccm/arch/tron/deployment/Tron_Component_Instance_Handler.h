// -*- C++ -*-

//=============================================================================
/**
 *  @file Tron_Component_Instance_Handler.h
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

#ifndef _ICCM_TRON_COMPONENT_INSTANCE_HANDLER_H_
#define _ICCM_TRON_COMPONENT_INSTANCE_HANDLER_H_

#include "cuts/iccm/deployment/Component_Instance_Handler_T.h"
#include "Tron_Container.h"
#include "Tron_ORB_Initializer.h"
#include "ace/Process.h"
#include "TestAdapterCallback_i.h"
#include "cuts/Servant_Manager_T.h"


namespace iCCM
{

/**
 * @class Tron_Component_Instance_Handler
 *
 * Tron implementation of the deployment handlers for component
 * instances.
 */
class  ICCM_TRON_DEPLOYMENT_HANDLERS_Export Tron_Component_Instance_Handler :
    public virtual ::DAnCE::InstanceDeploymentHandler,
    public virtual ::CORBA::LocalObject
{
public:
  /// Default constructor.
  Tron_Component_Instance_Handler (void);

  /// Destructor.
  virtual ~Tron_Component_Instance_Handler (void);

  /// {@ Proxy Methods
  virtual char * instance_type (void);

  ::CORBA::StringSeq * dependencies (void);

  virtual void install_instance (const ::Deployment::DeploymentPlan & plan,
                                 ::CORBA::ULong instanceRef,
                                 ::CORBA::Any_out instance_reference);

  virtual void activate_instance (const ::Deployment::DeploymentPlan & ,
                                  ::CORBA::ULong ,
                                  const ::CORBA::Any &);

  virtual void passivate_instance (const ::Deployment::DeploymentPlan & ,
                                   ::CORBA::ULong ,
                                   const ::CORBA::Any &);

  virtual void remove_instance (const ::Deployment::DeploymentPlan & plan,
                                ::CORBA::ULong instanceRef,
                                const ::CORBA::Any & instance_reference);

  virtual void provide_endpoint_reference (const ::Deployment::DeploymentPlan &,
                                           ::CORBA::ULong,
                                           ::CORBA::Any_out);

  virtual void connect_instance (const ::Deployment::DeploymentPlan & plan,
                                 ::CORBA::ULong connectionRef,
                                 const ::CORBA::Any & provided_reference);

  virtual void disconnect_instance (const ::Deployment::DeploymentPlan & plan,
                                    ::CORBA::ULong connectionRef);

  virtual void instance_configured (const ::Deployment::DeploymentPlan &,
                                    ::CORBA::ULong);

  virtual void close (void);

  /**
   * Configure the instance handler. The deployment properties passed
   * into this function are the ones attached to the Node in the
   * deployment model.
   *
   * @param[in]           config          Configuration properties
   */
  virtual void configure (const Deployment::Properties & config);

  /// @}
private:
  /**
   * Spawn the tron process using the provided propery as arguments.
   *
   * @param[in]           prop            CLI arguments
   */
  int spawn_tron_process (const ::Deployment::Property & prop);

  /// Activate the test adapter callback
  void activate_test_adapter_callback (void);

  /// The tron process
  ACE_Process tron_process_;

  /// The test adapter callback
  TestAdapterCallback_i tac_;

  /// Manger for the test adapter callback
  CUTS_Servant_Manager_T < TestAdapterCallback_i > tac_mgr_;

  /// Initialization state
  bool init_complete_;

  /// Number of instances
  ACE_Atomic_Op < ACE_Thread_Mutex, size_t > instance_count_;
};

}

extern "C"
ICCM_TRON_DEPLOYMENT_HANDLERS_Export
::DAnCE::InstanceDeploymentHandler_ptr
create_iCCM_Tron_Component_Instance_Handler (void);

#if defined (__CUTS_INLINE__)
#include "Tron_Component_Instance_Handler.inl"
#endif

#endif  // !defined _ICCM_Tron_COMPONENT_INSTANCE_HANDLER_H_
