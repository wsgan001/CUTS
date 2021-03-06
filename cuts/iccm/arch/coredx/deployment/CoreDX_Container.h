// -*- C++ -*-

//=============================================================================
/**
 *  @file         CoreDX_Container.h
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

#ifndef _ICCM_COREDX_CONTAINER_H_
#define _ICCM_COREDX_CONTAINER_H_

#include "cuts/iccm/arch/dds/deployment/DDS_Container.h"

#include "../servant/CoreDX_Servant.h"
#include "CoreDX_Container_Strategy.h"

namespace iCCM
{

// Forward decl.
class CoreDX_Component_Instance_Handler;

/**
 * @class CoreDX_Container
 *
 * iCCM container implementation for the CoreDX architecture.
 */
class ICCM_COREDX_DEPLOYMENT_HANDLERS_Export CoreDX_Container :
  public DDS_Container <CoreDX_Deployment>
{
public:
  /// Type definition of the base type.
  typedef DDS_Container <CoreDX_Deployment> base_type;

  /**
   * Initializing constructor.
   *
   * @param[in]           handler           Pointer to instance handler
   * @param[in]           poa               PortableServer for container
   */
  CoreDX_Container (
    CoreDX_Component_Instance_Handler * handler,
     ::PortableServer::POA_ptr the_POA);

  /// Destructor.
  virtual ~CoreDX_Container (void);
};

}

#if defined (__CUTS_INLINE__)
#include "CoreDX_Container.inl"
#endif

#endif  // !defined _ICCM_CoreDX_CONTAINER_H_
