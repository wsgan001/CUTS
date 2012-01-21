// -*- C++ -*-

//=============================================================================
/**
 *  @file       Servant_Context.h
 *
 *  $Id$
 *
 *  @author     James H. Hill
 */
//=============================================================================

#ifndef _ICCM_SERVANT_CONTEXT_H_
#define _ICCM_SERVANT_CONTEXT_H_

#include "Scope_Visitor.h"
#include <fstream>

namespace iCCM
{

/**
 * @class Servant_Context
 *
 * Main visitor that converts a DDS type model into CCM event model.
 */
class Servant_Context : public Scope_Visitor
{
public:
  /// Default constructor.
  Servant_Context (std::ofstream & hfile, std::ofstream & sfile);

  /// Destructor.
  virtual ~Servant_Context (void);

  // used methods
  virtual int visit_component (AST_Component * node);

private:
  std::ofstream & hfile_;
  std::ofstream & sfile_;
};

}

#endif  // !defined _ICCM_EXECUTOR_IDL_FILE_
