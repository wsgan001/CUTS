// $Id$

#include "Project_Generator.h"

#if !defined (__CUTS_INLINE__)
#include "Project_Generator.inl"
#endif

#include "../../BE_algorithm.h"
#include "../../BE_Options.h"
#include "../../BE_Impl_Node.h"
#include "boost/bind.hpp"

///////////////////////////////////////////////////////////////////////////////
// CUTS_BE_Project_File_Open_T

void CUTS_BE_Project_File_Open_T <CUTS_BE_CIAO::Context, CUTS_BE_Impl_Node>::
generate (const CUTS_BE_Impl_Node & node)
{
  std::string pathname (CUTS_BE_OPTIONS ()->output_directory_);
  pathname += "/" + node.name_ + ".mpc";

  this->ctx_.project_.open (pathname.c_str ());
}

///////////////////////////////////////////////////////////////////////////////
// CUTS_BE_Project_File_Begin_T

void CUTS_BE_Project_File_Begin_T <CUTS_BE_CIAO::Context, CUTS_BE_Impl_Node>::
generate (const CUTS_BE_Impl_Node &)
{
  this->ctx_.project_
    << CUTS_BE_CPP::single_line_comment ("This project file was generated by")
    << CUTS_BE_CPP::single_line_comment ("$Id$")
    << std::endl;
}

//
// CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_Impl_Node>
//
void
CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_Impl_Node>::
generate (const CUTS_BE_Impl_Node & node)
{
  // Let's reset the <visited_nodes_> object, if necessary.
  if (!this->visited_nodes_.empty ())
    this->visited_nodes_.clear ();

  this->generate_impl_project (node);
}

//
// generate_impl_project
//
void CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_Impl_Node>::
generate_impl_project (const CUTS_BE_Impl_Node & node)
{
  std::string impl_basename = node.exec_artifact_.name ();
  std::string container_name (node.container_.name ());

  // Create the export name for the project.
  std::string impl_export (impl_basename);

  std::transform (impl_export.begin (),
                  impl_export.end (),
                  impl_export.begin (),
                  &::toupper);

  // Generate the executor project.
  this->ctx_.project_
    << "project (" << impl_basename
    << ") : cuts_coworker_exec {" << std::endl
    << "  sharedname    = " << impl_basename << std::endl
    << std::endl
    << "  dynamicflags += " << impl_export << "_BUILD_DLL" << std::endl
    << std::endl
    << "  prebuild = perl -- $(ACE_ROOT)/bin/generate_export_file.pl "
    << impl_export << " > $(PROJECT_ROOT)/"
    << impl_basename << "_export.h" << std::endl;

  if (!node.references_.empty ())
  {
    visited_nodes_.clear ();

    this->ctx_.project_
      << "  after += " << node.impl_interface_.name () << "_exec";

    this->ctx_.project_
      << std::endl
      << std::endl;

    // Clear the visited nodes so we can iterate over them once
    // more. This time we are generating the libs declaration.
    visited_nodes_.clear ();

    this->ctx_.project_
      << "  libs += " << node.impl_interface_.name () << "_exec";

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                                this,
                                _1,
                                "",
                                "_stub"));

    this->ctx_.project_
      << std::endl
      << std::endl;
  }

  generate_mpc_i (node);

  this->ctx_.project_
    // Generate the source files.
    << "  Source_Files {" << std::endl
    << "    " << node.name_ << ".cpp" << std::endl
    << "  }" << std::endl
    << std::endl
    // Generate the header files.
    << "  Header_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << std::endl
    << "  Inline_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << std::endl
    << "  IDL_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << "}" << std::endl
    << std::endl;
}

//
// generate_stub_listing
//
void
CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_Impl_Node>::
generate_listing (const CUTS_BE_IDL_Node * node,
                  const char * prefix,
                  const char * postfix)
{
  IDL_Node_Set::const_iterator iter = this->visited_nodes_.find (node);

  if (iter != visited_nodes_.end ())
    return;

  this->visited_nodes_.insert (node);

  this->ctx_.project_
    << " \\" << std::endl
    << "    " << prefix << node->name_ << postfix;

  std::for_each (node->references_.begin (),
                 node->references_.end (),
                 boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                              this,
                              _1,
                              prefix,
                              postfix));
}

//
// genereate_mpc_i
//
void CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_Impl_Node>::
generate_mpc_i (const CUTS_BE_Impl_Node & node)
{
  this->generate_mpc_values ("includes", node.include_paths_);
  this->generate_mpc_values ("libpaths", node.lib_paths_);
  this->generate_mpc_values ("libs", node.lib_);
}

//
// generate_mpc_values
//
void CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_Impl_Node>::
generate_mpc_values (const std::string & heading,
                     const CUTS_String_Set & listing)
{
  if (listing.empty ())
    return;

  this->ctx_.project_
    << "  " << heading << " +=";

  for (CUTS_String_Set::const_iterator iter = listing.begin ();
       iter != listing.end ();
       iter ++)
  {
    this->ctx_.project_
      << " \\" << std::endl
      << "    " << iter->c_str ();
  }

  this->ctx_.project_
    << std::endl
    << std::endl;
}


//
// CUTS_BE_Project_File_End_T
//
void
CUTS_BE_Project_File_End_T <CUTS_BE_CIAO::Context, CUTS_BE_Impl_Node>::
generate (const CUTS_BE_Impl_Node & node)
{
  this->ctx_.project_
    << CUTS_BE_CPP::single_line_comment("end of auto-generated project file")
    << std::endl;
}

//
// CUTS_BE_Project_File_Close_T
//
void
CUTS_BE_Project_File_Close_T <CUTS_BE_CIAO::Context, CUTS_BE_Impl_Node>::
generate (void)
{
  if (this->ctx_.project_.is_open ())
    this->ctx_.project_.close ();
}

///////////////////////////////////////////////////////////////////////////////
// project generators [CUTS_BE_IDL_Node]

///////////////////////////////////////////////////////////////////////////////
// CUTS_BE_Project_File_Open_T

void CUTS_BE_Project_File_Open_T <CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::
generate (const CUTS_BE_IDL_Node & node)
{
  // Construct the name of the project file.
  std::string pathname (CUTS_BE_OPTIONS ()->output_directory_);
  pathname += "/" + std::string (node.file_.name ()) + ".mpc";

  // Open the project for writing.
  this->ctx_.project_.open (pathname.c_str ());
}

///////////////////////////////////////////////////////////////////////////////
// CUTS_BE_Project_File_Begin_T

void CUTS_BE_Project_File_Begin_T <CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::
generate (const CUTS_BE_IDL_Node &)
{
  this->ctx_.project_
    << CUTS_BE_CPP::single_line_comment ("This project file was generated by")
    << CUTS_BE_CPP::single_line_comment ("$Id$")
    << std::endl;
}

//
// CUTS_BE_Project_Write_T <C>
//
void
CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::
generate (const CUTS_BE_IDL_Node & node)
{
  this->generate_idlgen_project (node);
  this->generate_stub_project (node);
  this->generate_skel_project (node);

  if (CUTS_BE::has_component (node.file_))
  {
    this->generate_eidl_project (node);
    this->generate_exec_project (node);
    this->generate_svnt_project (node);
  }
}

//
// generate_idlgen_project
//
void
CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::
generate_idlgen_project (const CUTS_BE_IDL_Node & node)
{
  // Generate the export file for the project.
  std::string macro_basename (node.name_);

  std::transform (macro_basename.begin (),
                  macro_basename.end (),
                  macro_basename.begin (),
                  &toupper);

  bool has_component = CUTS_BE::has_component (node.file_);
  bool has_events = CUTS_BE::has_events (node.file_);

  // Generate the project.
  this->ctx_.project_
    << "project (" << node.name_ << "_IDL_Gen) : ";

  this->ctx_.project_
    << (has_component || has_events ? "component" : "ciao") << "idldefaults {" << std::endl
    << "  custom_only = 1" << std::endl
    << std::endl
    << "  idlflags += -Wb,stub_export_macro=" << macro_basename << "_STUB_Export \\" << std::endl
    << "              -Wb,stub_export_include=" << node.name_ << "_stub_export.h \\" << std::endl
    << "              -Wb,skel_export_macro=" << macro_basename << "_SKEL_Export \\" << std::endl
    << "              -Wb,skel_export_include=" << node.name_ << "_skel_export.h \\" << std::endl;

  if (has_component)
  {
    this->ctx_.project_
      << "              -Wb,exec_export_macro=" << macro_basename << "_EXEC_Export \\" << std::endl
      << "              -Wb,exec_export_include=" << node.name_ << "_exec_export.h \\" << std::endl
      << "              -Wb,svnt_export_macro=" << macro_basename << "_SVNT_Export \\" << std::endl
      << "              -Wb,svnt_export_include=" << node.name_ << "_svnt_export.h \\" << std::endl;
  }

  this->ctx_.project_
    << "              -Sa -Sal" << std::endl;

  if (!has_component || !this->generate_svnt_)
    this->ctx_.project_ << "  idlflags -= -Gsv" << std::endl;

  this->ctx_.project_
    << std::endl
    << "  IDL_Files {" << std::endl
    << "    " << node.name_ << ".idl" << std::endl
    << "  }" << std::endl
    << "}" << std::endl
    << std::endl;
}

//
// generate_stub_project
//
void
CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::
generate_stub_project (const CUTS_BE_IDL_Node & node)
{
  // Generate the export file for the project.
  std::string stub_name (node.name_);
  stub_name += "_stub";

  // Construct the export name for the project.
  std::string stub_export (stub_name);
  std::transform (stub_export.begin (),
                  stub_export.end (),
                  stub_export.begin (),
                  &toupper);

  std::string stub_export_macro = stub_export + "_Export";
  std::string stub_export_file = stub_name + "_export.h";

  // Generate the project.
  this->ctx_.project_
    << "project (" << stub_name << ") : avoids_ace_for_tao, " << this->stub_base_ << ", cuts_codegen_defaults {" << std::endl
    << "  sharedname    = " << stub_name << std::endl
    << "  dynamicflags += " << stub_export << "_BUILD_DLL" << std::endl
    << "  after        += " << node.file_.name () << "_IDL_Gen " << this->stub_after_ << std::endl
    << std::endl;

  if (!node.references_.empty ())
  {
    this->visited_nodes_.clear ();

    // Generate the dependencies for this project. This will be
    // all the client projects of the references for this node.
    this->ctx_.project_
      << std::endl
      << "  // listing of project dependencies" << std::endl
      << "  after        +=";

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                                this,
                                _1,
                                "_stub"));

    // Generate the import libraries for this project. This will be
    // all client project of the references for this node. The will
    // also be the same as those specified in the "after" tag.

    this->visited_nodes_.clear ();

    this->ctx_.project_
      << std::endl
      << std::endl
      << "  libs  +=";

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                                this,
                                _1,
                                "_stub"));

    this->ctx_.project_
      << std::endl;
  }

  this->ctx_.project_
    << std::endl
    // Generate the IDL files for this project.
    << "  IDL_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << std::endl
    // Generate the source files for this project.
    << "  Source_Files {" << std::endl
    << "    " << node.name_ << "C.cpp" << std::endl;

  for (CUTS_String_Set::iterator iter = this->stub_files_.begin (), iter_end = this->stub_files_.end ();
       iter != iter_end;
       ++ iter)
  {
    this->ctx_.project_ << "    " << *iter << std::endl;
  }

  this->ctx_.project_
    << "  }" << std::endl
    << std::endl
    // Generate the header files for this project.
    << "  Header_Files {" << std::endl
    << "    " << node.name_ << "C.h" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  Inline_Files {" << std::endl
    << "    " << node.name_ << "C.inl" << std::endl
    << "  }" << std::endl
    << "}" << std::endl
    << std::endl;
}

//
// generate_skel_project
//
void
CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::
generate_skel_project (const CUTS_BE_IDL_Node & node)
{
  // Generate the export file for the project.
  std::string skel_name = node.basename_ + "_skel";

  // Construct the export name for the project.
  std::string skel_export = skel_name;
  std::transform (skel_export.begin (),
                  skel_export.end (),
                  skel_export.begin (),
                  &toupper);

  std::string skel_export_macro = skel_export + "_Export";
  std::string skel_export_file = skel_name + "_export.h";

  // Generate the project.
  this->ctx_.project_
    << "project (" << skel_name << ") : ccm_svnt, avoids_ace_for_tao, cuts_codegen_defaults {" << std::endl
    << "  sharedname   = " << skel_name << std::endl
    << "  dynamicflags = " << skel_export << "_BUILD_DLL" << std::endl
    << std::endl
    << "  after       += " << node.name_ << "_stub" << std::endl
    << "  libs        += " << node.name_ << "_stub" << std::endl
    << std::endl;

  if (!node.references_.empty ())
  {
    this->visited_nodes_.clear ();

    // Generate the dependencies for this project. This will be
    // all the client projects of the references for this node.
    this->ctx_.project_
      << std::endl
      << "  after +=";

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                                this,
                                _1,
                                "_skel"));

    // Generate the import libraries for this project. This will be
    // all client project of the references for this node. The will
    // also be the same as those specified in the "after" tag.

    this->visited_nodes_.clear ();

    this->ctx_.project_
      << std::endl
      << std::endl
      << "  libs  +=";

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                                this,
                                _1,
                                "_skel"));

    this->visited_nodes_.clear ();

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                                this,
                                _1,
                                "_stub"));

    this->ctx_.project_
      << std::endl;
  }

  this->ctx_.project_
    << std::endl
    // Generate the IDL files for this project.
    << "  IDL_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << std::endl

    // Generate the source files for this project.
    << "  Source_Files {" << std::endl
    << "    " << node.name_ << "S.cpp" << std::endl
    << "  }" << std::endl
    << std::endl
    // Generate the header files for this project.
    << "  Header_Files {" << std::endl
    << "    " << node.name_ << "S.h" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  Inline_Files {" << std::endl
    << "    " << node.name_ << "S.inl" << std::endl
    << "  }" << std::endl
    << "}" << std::endl
    << std::endl;
}

//
// generate_eidl_project
//
void CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::
generate_eidl_project (const CUTS_BE_IDL_Node & node)
{
  std::string name (node.file_.name ());

  this->ctx_.project_
    << "project (" << name << "_EIDL_Gen) : ciaoidldefaults {" << std::endl
    << "  custom_only = 1" << std::endl
    << std::endl
    << "  after    += " << name << "_IDL_Gen" << std::endl
    << std::endl;

  std::string export_basename (name);
  std::transform (export_basename.begin (),
                  export_basename.end (),
                  export_basename.begin (),
                  &::toupper);

  this->ctx_.project_
    << "  idlflags += -Wb,export_macro=" << export_basename << "_EXEC_Export \\" << std::endl
    << "              -Wb,export_include=" << name << "_exec_export.h \\" << std::endl
    << "              -Sa -Sal -SS" << std::endl
    << std::endl
    << "  IDL_Files {" << std::endl
    << "    " << name << "E.idl" << std::endl
    << "  }" << std::endl
    << "}" << std::endl
    << std::endl;
}

//
// generate_exec_project
//
void CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::
generate_exec_project (const CUTS_BE_IDL_Node & node)
{
  std::string name (node.file_.name ());
  std::string project_name = name + "_exec";

  std::string macro_basename (name);
  std::transform (macro_basename.begin (),
                  macro_basename.end (),
                  macro_basename.begin (),
                  &::toupper);

  this->ctx_.project_
    << "project (" << project_name << ") : ciao_executor, cuts_codegen_defaults {" << std::endl
    << "  sharedname    = " << project_name << std::endl
    << "  dynamicflags += " << macro_basename << "_EXEC_BUILD_DLL" << std::endl
    << std::endl
    << "  after += " << name << "_EIDL_Gen " << name << "_stub";

  std::for_each (node.references_.begin (),
                 node.references_.end (),
                 boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                              this,
                              _1,
                              "_stub"));

  this->visited_nodes_.clear ();
  this->ctx_.project_
    << std::endl
    << std::endl
    << "  libs  += " << name << "_stub";

  std::for_each (node.references_.begin (),
                 node.references_.end (),
                 boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                              this,
                              _1,
                              "_stub"));

  this->ctx_.project_
    << std::endl
    << std::endl
    << "  Source_Files {" << std::endl
    << "    " << name << "EC.cpp" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  Inline_Files {" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  Header_Files {" << std::endl
    << "    " << name << "EC.h" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  IDL_Files {" << std::endl
    << "  }" << std::endl
    << "}" << std::endl
    << std::endl;
}

//
// generate_svnt_project
//
void
CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::
generate_svnt_project (const CUTS_BE_IDL_Node & node)
{
  // Construct the names of the servant and skeleton project.
  std::string name (node.file_.name ());
  std::string svnt_project = name + "_svnt";

  // Create the export name for the project.
  std::string export_basename (svnt_project);
  std::transform (export_basename.begin (),
                  export_basename.end (),
                  export_basename.begin (),
                  &toupper);

  this->ctx_.project_
    << "project (" << svnt_project << ") : " << this->svnt_base_ << ", cuts_codegen_defaults {" << std::endl
    << "  sharedname   = " << svnt_project << std::endl
    << "  dynamicflags = " << export_basename << "_BUILD_DLL" << std::endl
    << std::endl
    << "  after += " << name << "_exec " << name << "_stub";

  // Generate the STUB dependencies for this node.
  this->visited_nodes_.clear ();

  std::for_each (node.references_.begin (),
                 node.references_.end (),
                 boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                              this,
                              _1,
                              "_stub"));

  this->ctx_.project_
    << std::endl
    << std::endl;

  // Clear the visited nodes so we can iterate over them once
  // more. This time we are generating the libs declaration.

  this->ctx_.project_
    << "  libs += "
    << name << "_exec "
    << name << "_stub "
    << name << "_skel";

  this->visited_nodes_.clear ();
  std::for_each (node.references_.begin (),
                 node.references_.end (),
                 boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                              this,
                              _1,
                              "_stub"));

  this->visited_nodes_.clear ();
  std::for_each (node.references_.begin (),
                 node.references_.end (),
                 boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                              this,
                              _1,
                              "_skel"));

  this->ctx_.project_
    << std::endl
    << std::endl;

  this->ctx_.project_
    // Generate the source files
    << "  Source_Files {" << std::endl
    << "    " << name << "_svnt.cpp" << std::endl
    << "  }" << std::endl
    << std::endl
    // Generate the header files
    << "  Header_Files {" << std::endl
    << "    " << name << "_svnt.h" << std::endl
    << "  }" << std::endl
    << std::endl
    << "  Inline_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << "  IDL_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << "}" << std::endl
    << std::endl;
}


//
// generate_listing
//
void CUTS_BE_Project_Write_T <CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::
generate_listing (const CUTS_BE_IDL_Node * node, const char * type)
{
  IDL_Node_Set::const_iterator iter = visited_nodes_.find (node);

  if (iter == visited_nodes_.end ())
  {
    this->visited_nodes_.insert (node);

    this->ctx_.project_
      << " \\" << std::endl
      << "    " << node->name_ << type;

    std::for_each (node->references_.begin (),
                   node->references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T::generate_listing,
                                this,
                                _1,
                                type));
  }
}

//
// CUTS_BE_Project_File_End_T
//
bool CUTS_BE_Project_File_End_T <CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::
generate (const CUTS_BE_IDL_Node & node)
{
  this->ctx_.project_
    << "// end of auto-generated project file" << std::endl;

  return true;
}

//
// CUTS_BE_Project_File_Close_T
//
void CUTS_BE_Project_File_Close_T <
CUTS_BE_CIAO::Context, CUTS_BE_IDL_Node>::generate (void)
{
  if (this->ctx_.project_.is_open ())
    this->ctx_.project_.close ();
}
