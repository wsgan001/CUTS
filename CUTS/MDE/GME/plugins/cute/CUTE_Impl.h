// -*- C++ -*-

//=============================================================================
/**
 * @file        CUTE_Impl.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _CUTE_PLUGIN_IMPL_H_
#define _CUTE_PLUGIN_IMPL_H_

#include "game/be/Plugin_Impl.h"
#include "ace/SString.h"
#include "ace/Unbounded_Set.h"
#include "Interpreter_List.h"

// Forward decl.
class CUTS_Property_Map;

/**
 * @class CUTS_CUTE
 *
 * Implementation of the CUTE plugin.
 */
class CUTS_CUTE :
  public GAME::Plugin_Impl
{
public:
  /// Default constructor.
  CUTS_CUTE (void);

  /// Destructor.
  virtual ~CUTS_CUTE (void);

  /**
   * Handle the InvokeEx method.
   */
  int invoke_ex (GAME::Project project,
                 GAME::FCO_in fco,
                 std::vector <GAME::FCO> & selected,
                 long flags);

private:
  void get_interpreters (GAME::Project proj, CUTS_CUTE_Interpreter_List & list);

  int handle_interpret (GAME::Project project,
                        GAME::FCO_in fco,
                        std::vector <GAME::FCO> & selected,
                        long flags,
                        const char * config_file,
                        const char * prog_id,
                        const char * parameters);

  int handle_generate (GAME::Project project,
                       GAME::FCO_in fco,
                       std::vector <GAME::FCO> & selected,
                       long flags,
                       const char * config_file);
};

#endif  // !defined _CUTE_PLUGIN_IMPL_H_
