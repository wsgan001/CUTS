// -*- C++ -*-

#ifndef _CUTS_OPENSPLICE_CONFIGURATOR_H_
#define _CUTS_OPENSPLICE_CONFIGURATOR_H_

#include "ccm/CCM_StandardConfiguratorC.h"
#include "OpenSplice_ComponentServer_export.h"

// Forward decl.
class CUTS_OpenSplice_Servant;

/**
 * @class CUTS_OpenSplice_Configurator
 */
class CUTS_OPENSPLICE_COMPONENTSERVER_Export CUTS_OpenSplice_Configurator
{
public:
  /// Default constructor.
  CUTS_OpenSplice_Configurator (void);

  /// Destructor.
  ~CUTS_OpenSplice_Configurator (void);

  int configure (CUTS_OpenSplice_Servant * servant,
		 const ::Components::ConfigValues & config);	      
};

#endif  // !defined _CUTS_OPENSPLICE_CONFIGURATOR_
