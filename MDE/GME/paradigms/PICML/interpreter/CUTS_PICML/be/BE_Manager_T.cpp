// $Id$

#include "PICML/PICML.h"
#include "BE_Workspace_Generator_T.h"
#include "BE_Impl_Generator_T.h"
#include "BE_Preprocessor_T.h"

//
// CUTS_BE_Manager_T
//
template <typename T, typename CONTEXT>
CUTS_BE_Manager_T <T, CONTEXT>::CUTS_BE_Manager_T (void)
{

}

//
// ~CUTS_BE_Manager_T
//
template <typename T, typename CONTEXT>
CUTS_BE_Manager_T <T, CONTEXT>::~CUTS_BE_Manager_T (void)
{

}

//
// _create
//
template <typename T, typename CONTEXT>
CUTS_BE_Manager * CUTS_BE_Manager_T <T, CONTEXT>::_create (void)
{
  return new T ();
}

//
// handle
//
template <typename T, typename CONTEXT>
bool CUTS_BE_Manager_T <T, CONTEXT>::handle (const PICML::RootFolder_in root)
{
  CUTS_BE_Preprocessor_T <CONTEXT> preprocessor;

  // Initialize the backend generator.
  CUTS_BE_Initialize_T <CONTEXT> init_gen (this->context_);
  init_gen.generate (root);

  // Generate the executor implementation.
  CUTS_BE_Impl_Generator_T <CONTEXT> impl_gen (this->context_, preprocessor);
  impl_gen.generate (root);

  // Generate the workspace/project files.
  CUTS_BE_Workspace_Generator_T <architecture_type> workspace (this->context_);
  workspace.generate (preprocessor.impls ());

  // Finalize the backend generator.
  CUTS_BE_Finalize_T <CONTEXT> fini_gen (this->context_);
  fini_gen.generate (root);

  return true;
}
