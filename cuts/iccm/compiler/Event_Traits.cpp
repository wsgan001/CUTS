// $Id$

#include "Event_Traits.h"

#include "be_extern.h"
#include "be_global.h"

#include "ast_module.h"
#include "ast_root.h"
#include "ast_eventtype.h"
#include "utl_identifier.h"

#include "CCF/CodeGenerationKit/IndentationCxx.hpp"
#include "CCF/CodeGenerationKit/IndentationImplanter.hpp"

namespace iCCM
{

//
// Event_Traits
//
Event_Traits::Event_Traits (std::ofstream & hfile, std::ofstream & sfile)
: hfile_ (hfile),
  sfile_ (sfile)
{

}

//
// ~Event_Traits
//
Event_Traits::~Event_Traits (void)
{

}

//
// visit_root
//
int Event_Traits::visit_root (AST_Root * node)
{
  return this->visit_scope (node);
}

//
// visit_module
//
int Event_Traits::visit_module (AST_Module * node)
{
  return this->visit_scope (node);
}

//
// visit_eventtype
//
int Event_Traits::visit_eventtype (AST_EventType * node)
{
  if (!be_global->is_dds_eventtype (node))
    return 0;

  Indentation::Implanter <Indentation::Cxx, char> h_implanter (this->hfile_);
  const char * full_name = node->full_name ();

  ACE_CString dds_event;
  be_global->get_dds_eventtype (node, dds_event);

  this->hfile_
    << "/**" << std::endl
    << " * @class CUTS_OpenSplice_CCM_Traits_T < ::" << full_name << " >" << std::endl
    << " *" << std::endl
    << " * Trait information for " << full_name << "Event event type." << std::endl
    << " */" << std::endl
    << "template < >" << std::endl
    << "class CUTS_OpenSplice_CCM_Traits_T < ::" << full_name << " >"
    << "{"
    << "public:" << std::endl
    << "// traits for the reader(s)" << std::endl
    << "typedef ::" << dds_event << "DataReader reader_type;"
    << "typedef ::" << dds_event << "DataReader_ptr reader_type_ptr;"
    << "typedef ::" << dds_event << "DataReader_var reader_type_var;"
    << std::endl
    << "// traits for the writer(s)" << std::endl
    << "typedef ::" << dds_event << "DataWriter writer_type;"
    << "typedef ::" << dds_event << "DataWriter_ptr writer_type_ptr;"
    << "typedef ::" << dds_event << "DataWriter_var writer_type_var;"
    << std::endl
    << "// traits for the type support system" << std::endl
    << "typedef ::" << dds_event << "TypeSupport dds_typesupport_type;"
    << "typedef ::" << dds_event << "TypeSupport_var dds_typesupport_type_var;"
    << "typedef ::" << dds_event << "TypeSupport_ptr dds_typesupport_type_ptr;"
    << std::endl
    << "// event mapping types" << std::endl
    << "typedef ::" << dds_event << " dds_event_type;"
    << "typedef ::" << dds_event << "Seq dds_event_sequence_type;"
    << "typedef ::" << full_name << "Upcall upcall_event_type;"
    << "typedef ::" << full_name << "Downcall downcall_event_type;"
    << "typedef ::" << full_name << " corba_event_type;"
    << "};";

  return 0;
}

}
