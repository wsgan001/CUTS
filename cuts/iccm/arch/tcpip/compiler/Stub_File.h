// -*- C++ -*-

//=============================================================================
/**
 *  @file       Stub_File.h
 *
 *  $Id$
 *
 *  @author     James H. Hill
 */
//=============================================================================

#ifndef _ICCM_STUB_FILE_H_
#define _ICCM_STUB_FILE_H_

#include "Scope_Visitor.h"
#include <fstream>

namespace iCCM
{

/**
 * @class Stub_File
 *
 * Main visitor that converts a TAO type model into CCM event model.
 */
class Stub_File : public Scope_Visitor
{
public:
  /// Default constructor.
  Stub_File (void);

  /// Destructor.
  virtual ~Stub_File (void);

  // used methods
  virtual int visit_root (AST_Root *node);

private:
  std::ofstream hfile_;
  std::ofstream sfile_;
};

}

#endif  // !defined _ICCM_STUB_FILE_H_
