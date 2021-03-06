// -*- C++ -*-

//=============================================================================
/**
 * @file        Component_Impl_Generator.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BE_CPP_COMPONENT_IMPL_GENERATOR_H_
#define _CUTS_BE_CPP_COMPONENT_IMPL_GENERATOR_H_

#include <ostream>
#include "PICML/PICML.h"
#include "PICML/Visitor.h"
#include "Cpp_export.h"

namespace CUTS_BE_CPP
{
/**
 * @class Base_Member_Init
 *
 * Visitor that is responsible for generating the base member
 * initialization section for a class.
 */
class CUTS_BE_CPP_Export Base_Member_Init : public PICML::Visitor
{
public:
  /**
   * Initializing constructor
   *
   * @param[in]       out           Target output stream.
   */
  Base_Member_Init (std::ostream & out);

  /// Destructor
  virtual ~Base_Member_Init (void);

  virtual void visit_Component (PICML::Component_in component);

  virtual void visit_Variable (PICML::Variable_in variable);

private:
  /// Target output stream.
  std::ostream & out_;
};

class CUTS_BE_CPP_Export Initialize_Entity : public PICML::Visitor
{
public:
  /**
   * Initializing constructor
   *
   * @param[in]       out           Target output stream.
   */
  Initialize_Entity (std::ostream & out);

  /// Destructor
  virtual ~Initialize_Entity (void);

  virtual void visit_PeriodicEvent (PICML::PeriodicEvent_in periodic);

  virtual void visit_InEventPort (PICML::InEventPort_in in);

  virtual void visit_ApplicationTask (PICML::ApplicationTask_in apptask);

private:
  /// Target output stream.
  std::ostream & out_;
};
};

#endif  // !define _CUTS_BE_CPP_COMPONENT_IMPL_GENERATOR_H_
