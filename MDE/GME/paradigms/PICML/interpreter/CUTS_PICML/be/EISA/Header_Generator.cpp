// $Id$

#include "Header_Generator.h"

#if !defined (__CUTS_INLINE__)
#include "Header_Generator.inl"
#endif

#include "Var_Type.h"
#include "Retn_Type.h"
#include "In_Type.h"

#include "../BE_Options.h"
#include "../BE_Impl_Node.h"

// UDM headers
#include "Uml.h"

// BOOST headers
#include "boost/bind.hpp"

// STL headers
#include <algorithm>
#include <fstream>
#include <sstream>

//
// Element_Name_End_With
//
template <typename T>
struct Element_Name_End_With
{
  /**
   * Initializing constructor.
   *
   * @param[in]     substring       Target substring in question.
   */
  Element_Name_End_With (const std::string & substring)
    : substring_ (substring) { }

  bool operator () (const T & element)
  {
    std::string name = element.name ();
    std::string::size_type npos = name.rfind (this->substring_);

    if (npos == std::string::npos)
      return false;

    return npos + this->substring_.length () == name.length ();
  }

private:
  /// Substring in question.
  const std::string & substring_;
};

//
// CUTS_EISA_Header_Generator
//
CUTS_EISA_Header_Generator::
CUTS_EISA_Header_Generator (void)
{

}

//
// ~CUTS_EISA_Header_Generator
//
CUTS_EISA_Header_Generator::
~CUTS_EISA_Header_Generator (void)
{

}

//
// open_fle
//
bool CUTS_EISA_Header_Generator::
open_file (const PICML::ComponentImplementationContainer & container)
{
  // Get the entry point for the node.
  this->get_impl_entry_point (container);

  // Construct the name of the file.
  std::ostringstream ostr;
  ostr
    << CUTS_BE_OPTIONS ()->output_directory_
    << "/" << container.name ()
    << CUTS_BE_OPTIONS ()->exec_suffix_
    << ".h";

  // Open the file and pass control to the base class.
  this->out_.open (ostr.str ().c_str ());

  if (!this->out_.is_open ())
    return false;

  this->open_file_i ();
  return this->out_.good ();
}

//
// write_impl_begin
//
void CUTS_EISA_Header_Generator::
write_impl_begin (const PICML::MonolithicImplementation & monoimpl,
                  const PICML::Component & component)
{
  // Pass control to the parent class.
  this->_super::write_impl_begin (monoimpl, component);

  // We need to forward declare all the facets since we aren't
  // going to generate their implementation until after the component's
  // implementation is generated.
  typedef std::vector <PICML::ProvidedRequestPort> Facet_Set;
  Facet_Set facets = component.ProvidedRequestPort_kind_children ();

  std::for_each (facets.begin (),
                 facets.end (),
                 boost::bind (&CUTS_EISA_Header_Generator::write_forward_decl,
                              boost::ref (this),
                              _1));
}

//
// write_method_begin
//
void CUTS_EISA_Header_Generator::
write_InEventPort_begin (const PICML::InEventPort & sink)
{
  this->out_
    << single_line_comment ("EventSink: " + (std::string)sink.name ())
    << "virtual void ";

  this->_super::write_InEventPort_begin (sink);
}

//
// write_method_end
//
void CUTS_EISA_Header_Generator::
write_InEventPort_end (const PICML::InEventPort & sink)
{
  this->out_ << ";" << std::endl;
}

//
// write_method_begin
//
void CUTS_EISA_Header_Generator::
write_PeriodicEvent_begin (const PICML::PeriodicEvent & periodic)
{
  this->out_
    << single_line_comment ("PeriodicEvent: " + (std::string)periodic.name ())
    << "void ";

  this->_super::write_PeriodicEvent_begin (periodic);
}

//
// write_method_end
//
void CUTS_EISA_Header_Generator::
write_PeriodicEvent_end (const PICML::PeriodicEvent & sink)
{
  this->out_ << ";" << std::endl;
}

//
// write_method
//
void CUTS_EISA_Header_Generator::
write_method (const PICML::OutEventPort & source)
{
  this->out_
    << single_line_comment ("EventSource: " + (std::string)source.name ())
    << "virtual void ";

  this->_super::write_method (source);

  this->out_
    << ";"
    << std::endl;
}

//
// write_method
//
void CUTS_EISA_Header_Generator::
write_method (const PICML::RequiredRequestPort & receptacle)
{
  PICML::Object object = PICML::Object::Cast (receptacle.ref ());

  this->out_
    << single_line_comment ("Receptacle: " + (std::string)receptacle.name ())
    << "virtual " << scope (object, "::") << object.name () << "_ptr "
    << std::endl << "  ";

  this->_super::write_method (receptacle);

  this->out_
    << ";"
    << std::endl;
}

//
// write_ProvidedRequestPort_begin
//
void CUTS_EISA_Header_Generator::
write_ProvidedRequestPort_begin (const PICML::ProvidedRequestPort & facet)
{
  PICML::Object object = PICML::Object::Cast (facet.ref ());
  std::string scope = this->scope (object, "::", true);

  this->out_
    << single_line_comment ("Facet: " + (std::string)facet.name ())
    << "virtual " << scope << "CCM_" << object.name ()
    << "_ptr " << std::endl;

  this->_super::write_ProvidedRequestPort_begin (facet);
}

//
// write_method_end
//
void CUTS_EISA_Header_Generator::
write_ProvidedRequestPort_end (const PICML::ProvidedRequestPort & facet)
{
  this->out_ << ";" << std::endl;
}

//
// write_set_session_context
//
void CUTS_EISA_Header_Generator::
write_set_session_context (const PICML::Component & component)
{
  this->out_
    << single_line_comment ("Environment: set_session_context")
    << "virtual void ";

  this->_super::write_set_session_context (component);

  this->out_
    << ";" << std::endl;
}

//
// write_ciao_preactivate
//
void CUTS_EISA_Header_Generator::
write_ciao_preactivate (const PICML::Component & component)
{
  this->out_
    << single_line_comment ("Environment: preactivate")
    << "virtual void ";

  this->_super::write_ciao_preactivate (component);

  this->out_
    << ";" << std::endl;
}

//
// write_cmm_activate
//
void CUTS_EISA_Header_Generator::
write_ccm_activate (const PICML::Component & component)
{
  this->out_
    << single_line_comment ("Environment: activate")
    << "virtual void ";

  this->_super::write_ccm_activate (component);

  this->out_
    << ";" << std::endl;
}

//
// write_ciao_postactivate
//
void CUTS_EISA_Header_Generator::
write_ciao_postactivate (const PICML::Component & component)
{
  this->out_
    << single_line_comment ("Environment: postactivate")
    << "virtual void ";

  this->_super::write_ciao_postactivate (component);

  this->out_
    << ";" << std::endl;
}

//
// write_ccm_passivate
//
void CUTS_EISA_Header_Generator::
write_ccm_passivate (const PICML::Component & component)
{
  this->out_
    << single_line_comment ("Environment: passivate")
    << "virtual void ";

  this->_super::write_ccm_passivate (component);

  this->out_
    << ";" << std::endl;
}

//
// write_ccm_remove
//
void CUTS_EISA_Header_Generator::
write_ccm_remove (const PICML::Component & component)
{
  this->out_
    << single_line_comment ("Environment: remove")
    << "virtual void ";

  this->_super::write_ccm_remove (component);

  this->out_
    << ";" << std::endl;
}

//
// write_method_begin
//
void CUTS_EISA_Header_Generator::
write_ReadonlyAttribute_begin (const PICML::ReadonlyAttribute & attr)
{
  PICML::AttributeMember member = attr.AttributeMember_child ();

  if (member != Udm::null)
  {
    PICML::MemberType mtype = member.ref ();

    if (mtype != Udm::null)
    {
      // Write the getter method for the attribute.
      this->out_
        << single_line_comment ("get " + (std::string)attr.name ())
        << "virtual "
        << EISA_RETURN_TYPE (mtype)
        << " ";

      if (this->is_variable_type (mtype))
        this->out_ << "* ";

      this->_super::write_ReadonlyAttribute_begin (attr);
    }
  }
}

//
// write_method_end
//
void CUTS_EISA_Header_Generator::
write_ReadonlyAttribute_end (const PICML::ReadonlyAttribute & attr)
{
  this->out_ << ";" << std::endl;
}

//
// write_method_begin
//
void CUTS_EISA_Header_Generator::
write_Attribute_begin (const PICML::Attribute & attr)
{
  if (std::string (attr.name ()) != "configuration")
  {
    // Generate the getter method for the attribute.
    PICML::Attribute temp_attr (attr);
    PICML::ReadonlyAttribute ro = PICML::ReadonlyAttribute::Cast (temp_attr);

    this->write_ReadonlyAttribute_begin (ro);
    this->write_ReadonlyAttribute_end (ro);

    // Close the getter method and generate the setter method
    // for the current attribute.
    this->out_
      << single_line_comment ("set " + (std::string)attr.name ())
      << "virtual void ";

    this->_super::write_Attribute_begin (attr);
  }
}

//
// write_method_end
//
void CUTS_EISA_Header_Generator::
write_Attribute_end (const PICML::Attribute & attr)
{
  if (std::string (attr.name ()) != "configuration")
    this->out_ << ";" << std::endl;
}

//
// write_variables_begin
//
void CUTS_EISA_Header_Generator::
write_variables_begin (const PICML::Component & component)
{
  this->out_ << "private:" << std::endl;
}

//
// write_forward_decl
//
void CUTS_EISA_Header_Generator::
write_forward_decl (const PICML::ProvidedRequestPort & facet)
{
  this->out_
    << single_line_comment ("Forward decl.")
    << "class " << facet.name () << "_i;"
    << std::endl;
}

//
// write_TwowayOperation_begin
//
void CUTS_EISA_Header_Generator::
write_TwowayOperation_begin (const PICML::TwowayOperation & twoway)
{
  PICML::ReturnType return_type = twoway.ReturnType_child ();

  this->out_
    << single_line_comment ("twoway: " + (std::string) twoway.name ());

  if (return_type != Udm::null)
  {
    PICML::MemberType type = return_type.ref ();

    if (type != Udm::null)
    {
      this->out_ << EISA_RETURN_TYPE (type);

      if (this->is_variable_type (type))
        this->out_ << " *";
    }
  }
  else
  {
    this->out_ << "void";
  }

  this->out_ << " ";

  this->_super::write_TwowayOperation_begin (twoway);
}

//
// write_TwowayOperation_begin
//
void CUTS_EISA_Header_Generator::
write_TwowayOperation_end (const PICML::TwowayOperation & twoway)
{
  this->out_ << ";";
}

//
// write_OnewayOperation_begin
//
void CUTS_EISA_Header_Generator::
write_OnewayOperation_begin (const PICML::OnewayOperation & oneway)
{
  this->out_
    << single_line_comment ("oneway: " + (std::string) oneway.name ())
    << "void ";

  this->_super::write_OnewayOperation_begin (oneway);
}

//
// write_OnewayOperation_end
//
void CUTS_EISA_Header_Generator::
write_OnewayOperation_end (const PICML::OnewayOperation & oneway)
{
  this->out_ << ";";
}

//
// write_factory_impl_begin
//
void CUTS_EISA_Header_Generator::
write_factory_impl_begin (const PICML::ComponentFactory & factory,
                          const PICML::MonolithicImplementation & impl,
                          const PICML::Component & type)
{
  std::string factory_impl = (std::string) factory.name () + "_i";

  this->out_
    << "class " << factory_impl << " :" << std::endl
    << "  public virtual " << factory.name () << "_Exec {"
    << "public:" << std::endl
    << single_line_comment ("default constructor")
    << factory_impl << " (void);"
    << std::endl
    << single_line_comment ("destructor")
    << "virtual ~" << factory_impl << " (void);"
    << std::endl
    << single_line_comment ("default creation method")
    << "virtual ::Components::EnterpriseComponent_ptr create (void);";
}

//
// write_FactoryOperation_begin
//
void CUTS_EISA_Header_Generator::
write_FactoryOperation_begin (const PICML::FactoryOperation & factory_op)
{
  this->out_
    << single_line_comment ("factory operation: " + (std::string) factory_op.name ())
    << "virtual ::Components::EnterpriseComponent_ptr" << std::endl
    << "  ";

  this->_super::write_FactoryOperation_begin (factory_op);
}

//
// write_FactoryOperation_end
//
void CUTS_EISA_Header_Generator::
write_FactoryOperation_end (const PICML::FactoryOperation & factory_op)
{
  this->out_
    << ";";
}

//
// write_factory_impl_end
//
void CUTS_EISA_Header_Generator::
write_factory_impl_end (const PICML::ComponentFactory & factory,
                        const PICML::MonolithicImplementation & impl,
                        const PICML::Component & type)
{
  // Generate the export file for the implementation.
  PICML::ComponentImplementationContainer container =
    PICML::ComponentImplementationContainer::Cast (impl.parent ());

  CUTS_BE_Impl_Node::Artifact_Set::const_iterator iter_exec =
    std::find_if (this->node_->artifacts_.begin (),
                  this->node_->artifacts_.end (),
                  Element_Name_End_With <
                    CUTS_BE_Impl_Node::Artifact_Set::value_type> (
                    CUTS_BE_OPTIONS ()->exec_suffix_));

  // Close off the class definition.
  this->out_
    << "};";

  this->_super::write_factory_impl_end (factory, impl, type);

  // Construct the export macro and export filename.
  std::string exportfile =
    std::string (container.name ()) + CUTS_BE_OPTIONS ()->exec_suffix_;

  std::string export_macro = iter_exec->name ();
  std::transform (export_macro.begin (),
                  export_macro.end (),
                  export_macro.begin (),
                  &toupper);

  this->out_
    << "#include \"" << exportfile << "_export.h\"" << std::endl
    << std::endl
    << single_line_comment (this->entry_point_)
    << "extern \"C\" " << export_macro << "_Export" << std::endl
    << "::Components::HomeExecutorBase_ptr " << std::endl
    << this->entry_point_ + " (void);"
    << std::endl;
}

//
// write_impl_begin
//
void CUTS_EISA_Header_Generator::
write_object_impl_begin (const PICML::Component & component,
                         const PICML::ProvidedRequestPort & facet)
{
  PICML::Object object = PICML::Object::Cast (facet.ref ());

  std::string facet_name = (std::string) facet.name () + "_i";
  std::string object_name = object.name ();
  std::string scope = this->scope (object, "::");

  this->out_
    << single_line_comment ("Facet: " + (std::string) facet.name ())
    << "class " << facet_name << " :" << std::endl
    << "  public " << scope << "CCM_" << object_name
    << " {"
    << "public:" << std::endl
    << single_line_comment ("Default constructor")
    << facet_name << " (void);"
    << std::endl
    << single_line_comment ("Destructor")
    << "virtual ~" << facet_name << " (void);"
    << std::endl;
}

//
// write_impl_end
//
void CUTS_EISA_Header_Generator::
write_object_impl_end (const PICML::Component & component,
                       const PICML::ProvidedRequestPort & facet)
{
  this->out_ << "};";
}
