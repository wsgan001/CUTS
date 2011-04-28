// -*- C++ -*-

//=============================================================================
/**
 * @file      BE_Impl_Generator_T.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BE_IMPL_GENERATOR_T_H_
#define _CUTS_BE_IMPL_GENERATOR_T_H_

#include "PICML/PICML.h"
#include "BE_Preprocessor_T.h"

/**
 * @class CUTS_BE_Impl_Generator_T
 *
 * Implementation generator for CUTS. This class is parameterized
 * by a backend trait that determines what is generated. This class
 * only contains the parsing logic for generating the inplementation.
 * The backend traits that are used when generating the implemenation
 * can be found in CUTS_BE_Traits.
 */
template <typename CONTEXT>
class CUTS_BE_Impl_Generator_T : public PICML::Visitor
{
public:
  /// Type definition of the architecture for the code generator
  typedef typename CONTEXT::architecture_type architecture_type;

  /// Type definition of the behavior.
  typedef typename CONTEXT::behavior_type behavior_type;

  /// Default constructor.
  CUTS_BE_Impl_Generator_T (CONTEXT & context, CUTS_BE_Preprocessor_T <CONTEXT> & pp);

  /// Destructor.
  virtual ~CUTS_BE_Impl_Generator_T (void);

  void generate (const PICML::RootFolder & root);

  /// Visit a RootFolder element.
  void Visit_RootFolder (
    const PICML::RootFolder & root);

  /// Visit a ComponentImplementations folder.
  void Visit_ComponentImplementations (
    const PICML::ComponentImplementations & impls);

  /// Visit a ComponentImplementationContainer element.
  void Visit_ComponentImplementationContainer (
    const PICML::ComponentImplementationContainer & container);

  /// Visit a MonolithicImplementation element.
  void Visit_MonolithicImplementation (
    const PICML::MonolithicImplementation & monoimpl);

  /// Visit a Component element.
  void Visit_Component (
    const PICML::Component & component);

  void Visit_Supports (
    const PICML::Supports & supports);

  /// Visit a Component element.
  void Visit_Object (
    const PICML::Object & object);

  /// Visit a InEventPort element.
  void Visit_InEventPort (
    const PICML::InEventPort & sink);

  /// Visit a ProvidedRequestPort element.
  void Visit_ProvidedRequestPort (
    const PICML::ProvidedRequestPort & facet);

  /// Visit a ProvidedRequestPort element.
  void Visit_ProvidedRequestPort_impl (
    const PICML::ProvidedRequestPort & facet);

  /// Visit a ProvidedRequestPort element.
  void Visit_PeriodicEvent (
    const PICML::PeriodicEvent & periodic);

  /// Visit a ReadonlyAttribute element.
  void Visit_ReadonlyAttribute (
    const PICML::ReadonlyAttribute  & ro_attr);

  /// Visit an Attribute element.
  void Visit_Attribute (
    const PICML::Attribute & attr);

  /// Visit a Variable element.
  void Visit_Variable (
    const PICML::Variable & variable);

  /// Visit a WorkerType element.
  void Visit_WorkerType (
    const PICML::WorkerType & type);

  void Visit_OnewayOperation (
    const PICML::OnewayOperation & oneway);

  void Visit_TwowayOperation (
    const PICML::TwowayOperation & twoway);

  void Visit_ComponentFactory (
    const PICML::ComponentFactory & factory);

  void Visit_ComponentFactory_inherits (
    const PICML::Inherits & inherits);

  void Visit_FactoryOperation (
    const PICML::FactoryOperation & factory_op);

  void Visit_MonolithprimaryArtifact (
    const PICML::MonolithprimaryArtifact & primary);

  void Visit_ImplementationArtifactReference (
    const PICML::ImplementationArtifactReference & artref);

protected:
  void Visit_Include (
    const std::string & include);

  void Visit_ReadonlyAttribute_Variable (
    const PICML::ReadonlyAttribute & attr);

  void Visit_PeriodicEvent_Variable (
    const PICML::PeriodicEvent & periodic);

  void Visit_ComponentImplementation (
    const PICML::ComponentImplementation &);

private:
  /// Target context for the generator.
  CONTEXT & context_;

  CUTS_BE_Preprocessor_T <CONTEXT> & pp_;

  /// Current monolithic implementation.
  PICML::MonolithicImplementation monoimpl_;

  /// Helper method to write the variables for the component.
  void write_variables_i (const PICML::Component & component);

  bool get_component_factory (const PICML::Component & component,
                              PICML::ComponentFactory & factory);

  // prevent the following operations
  CUTS_BE_Impl_Generator_T (const CUTS_BE_Impl_Generator_T &);
  const CUTS_BE_Impl_Generator_T & operator = (const CUTS_BE_Impl_Generator_T &);
};

#if defined (__CUTS_INLINE__)
#include "BE_Impl_Generator_T.inl"
#endif

#include "BE_Impl_Generator_T.cpp"

#endif  // !defined _CUTS_BE_IMPL_GENERATOR_T_H_
