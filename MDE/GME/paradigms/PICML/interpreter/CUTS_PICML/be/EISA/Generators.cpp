// $Id$

#include "Generators.h"
#include "../BE_Options.h"
#include "../BE_Impl_Node.h"
#include "boost/bind.hpp"
#include <sstream>
#include <algorithm>

#define CUTS_BE_EISA \
  ACE_Singleton <CUTS_BE_Eisa, ACE_Null_Mutex>::instance ()

#define EXEC_SUFFIX       "_exec"
#define SVNT_SUFFIX       "_svnt"
#define STUB_SUFFIX       "_stub"
#define SKEL_SUFFIX       "_skel"

#define CLIENT_SUFFIX     "C"
#define SERVER_SUFFIX     "S"

/**
 * Get the basename of a filename.
 *
 * @param[in]     filename      The source filename.
 * @return        Basename of the filename.
 */
static std::string basename (const std::string & filename)
{
  std::string::size_type npos = filename.find_last_of ("/\\");
  return filename.substr (npos + 1);
}

/**
 * @struct Element_Name_End_With
 *
 * Functor for checking if a element's name ends with a
 * specific string.
 */
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


///////////////////////////////////////////////////////////////////////////////
// workspace generators

//
// CUTS_BE_Workspace_File_Open_T
//
bool CUTS_BE_Workspace_File_Open_T <CUTS_BE_Eisa>::
generate (const std::string & name)
{
  // Construct the full pathname of the workspace.
  std::ostringstream pathname;
  pathname
    << CUTS_BE_OPTIONS ()->output_directory_
    << "/" << name << ".mwc";

  // Open the workspace file for writing.
  CUTS_BE_EISA->workspace_file_.open (pathname.str ().c_str ());
  return CUTS_BE_EISA->workspace_file_.is_open ();
}

//
// CUTS_BE_Workspace_Begin_T
//
bool CUTS_BE_Workspace_Begin_T <CUTS_BE_Eisa>::
generate (const std::string & name)
{
  CUTS_BE_EISA->workspace_file_
    << "// -*- mwc -*-" << std::endl
    << std::endl
    << "//===================================================================" << std::endl
    << "//" << std::endl
    << "// @file     " << name << ".mwc" << std::endl
    << "//" << std::endl
    << "// This file was generated by" << std::endl
    << "// $Id$" << std::endl
    << "//" << std::endl
    << "// Please do not modify the contents of this file under" << std::endl
    << "// any circumstances. Doing so can result in failure to" << std::endl
    << "// successfully build parts of (or the entire) workspace." << std::endl
    << "//" << std::endl
    << "// @author    James H. Hill" << std::endl
    << "//" << std::endl
    << "//===================================================================" << std::endl
    << std::endl
    << "workspace (" << name << ") {" << std::endl
    << "  // -- initialize the includes for the project." << std::endl
    << "  cmdline += -include $EISA_ROOT/MPC/config \\" << std::endl
    << "             -include $CUTS_ROOT/MPC/config \\" << std::endl
    << "             -include $XSC_ROOT/MPC/config \\" << std::endl
    << "             -feature_file $CUTS_ROOT/default.features \\" << std::endl
    << "             -relative PROJECT_ROOT=" << std::endl
    << std::endl;

  return true;
}

//
// CUTS_BE_Workspace_Project_Include_T <, CUTS_BE_Impl_Node>
//
bool CUTS_BE_Workspace_Project_Include_T <
CUTS_BE_Eisa, CUTS_BE_Impl_Node>::generate (const CUTS_BE_Impl_Node & node)
{
  CUTS_BE_EISA->workspace_file_
    << "  " << node.name_ << "_exec.mpc" << std::endl;

  return true;
}

//
// CUTS_BE_Workspace_Project_Include_T <, CUTS_BE_IDL_Node>
//
bool CUTS_BE_Workspace_Project_Include_T <
CUTS_BE_Eisa, CUTS_BE_IDL_Node>::generate (const CUTS_BE_IDL_Node & node)
{
  CUTS_BE_EISA->workspace_file_
    << "  " << node.name_ << "_stub.mpc" << std::endl;

  return true;
}

//
// CUTS_BE_Workspace_End_T
//
bool CUTS_BE_Workspace_End_T <CUTS_BE_Eisa>::
generate (const std::string & name)
{
  CUTS_BE_EISA->workspace_file_
    << "}" << std::endl
    << std::endl
    << "// end of auto-generated workspace" << std::endl;

  return true;
}

//
// CUTS_BE_Workspace_File_Close_T
//
void CUTS_BE_Workspace_File_Close_T <CUTS_BE_Eisa>::generate (void)
{
  if (CUTS_BE_EISA->workspace_file_.is_open ())
    CUTS_BE_EISA->workspace_file_.close ();
}

///////////////////////////////////////////////////////////////////////////////
// project generators (CUTS_BE_Impl_Node)

//
// CUTS_BE_Project_File_Open_T
//
bool CUTS_BE_Project_File_Open_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
generate (const CUTS_BE_Impl_Node & node)
{
  std::ostringstream pathname;
  pathname
    << CUTS_BE_OPTIONS ()->output_directory_
    << "/" << node.name_ << "_exec.mpc";

  CUTS_BE_EISA->project_file_.open (pathname.str ().c_str ());
  return CUTS_BE_EISA->project_file_.is_open ();
}

//
// CUTS_BE_Project_File_Begin_T
//
bool CUTS_BE_Project_File_Begin_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
generate (const CUTS_BE_Impl_Node & node)
{
  CUTS_BE_EISA->project_file_
    << "// This project file was generated by" << std::endl
    << "// $Id$"
    << std::endl
    << std::endl;

  return true;
}

//
// visited_nodes_
//
CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::IDL_Node_Set
CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::visited_nodes_;

//
// svnt_project_
//
std::string CUTS_BE_Project_Write_T <
CUTS_BE_Eisa, CUTS_BE_Impl_Node>::svnt_project_;

//
// svnt_artifact_
//
std::string CUTS_BE_Project_Write_T <
CUTS_BE_Eisa, CUTS_BE_Impl_Node>::svnt_artifact_;

//
// CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>
//
bool CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
generate (const CUTS_BE_Impl_Node & node)
{
  // Let's reset the <visited_nodes_> object, if necessary.
  if (visited_nodes_.empty ())
    visited_nodes_.clear ();

  // Generate the executor and servant projects.
  generate_skel_project (node);
  generate_svnt_project (node);
  generate_exec_project (node);

  return true;
}

//
// generate_exec_project
//
void CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
generate_exec_project (const CUTS_BE_Impl_Node & node)
{
  // We need to first locate the artifact that ends with
  // exec_suffix_. If we can't find one, then we need to
  // just abort!!
  CUTS_BE_Impl_Node::Artifact_Set::const_iterator iter_exec =
    std::find_if (node.artifacts_.begin (), node.artifacts_.end (),
                  Element_Name_End_With <
                    CUTS_BE_Impl_Node::Artifact_Set::value_type> (
                    CUTS_BE_OPTIONS ()->exec_suffix_));

  CUTS_BE_Impl_Node::Artifact_Set::const_iterator iter_svnt =
    std::find_if (node.artifacts_.begin (), node.artifacts_.end (),
                  Element_Name_End_With <
                    CUTS_BE_Impl_Node::Artifact_Set::value_type> (
                    SVNT_SUFFIX));

  if (iter_exec == node.artifacts_.end () ||
      iter_svnt == node.artifacts_.end ())
  {
    return;
  }

  // Generate the export_file file for the project.
  std::string impl_project = iter_exec->name ();
  std::string export_filename = node.name_ + CUTS_BE_OPTIONS ()->exec_suffix_;

  std::string skel_project = node.name_ + SKEL_SUFFIX;

  std::replace (skel_project.begin (), skel_project.end (), '/', '_');
  std::replace (skel_project.begin (), skel_project.end (), '\\', '_');

  // Create the export name for the project.
  std::string exec_export = impl_project;
  std::transform (exec_export.begin (),
                  exec_export.end (),
                  exec_export.begin (),
                  &toupper);

  // Generate the executor project.
  CUTS_BE_EISA->project_file_
    << "project (" << std::string (iter_exec->name ())
    << ") : eisa_xml, eisa_component_exec, eisa_cuts_defaults {" << std::endl
    << "  sharedname   = " << basename (iter_exec->location ()) << std::endl
    << std::endl
    << "  dynamicflags = " << exec_export << "_BUILD_DLL" << std::endl
    << std::endl
    << "  prebuild = perl -- $(ACE_ROOT)/bin/generate_export_file.pl "
    << exec_export << " > $(PROJECT_ROOT)/"
    << export_filename << "_export.h" << std::endl;

  if (!node.references_.empty ())
  {
    // Clear the nodes that have already be seen.
    visited_nodes_.clear ();

    // Setup the initial dependency information.
    CUTS_BE_EISA->project_file_
      << std::endl
      << "  after += " << svnt_project_ << " " << skel_project;

    // Generate the remaining dependencies.
    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T <
                                  CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
                                  generate_stub_listing, _1));

    CUTS_BE_EISA->project_file_
      << std::endl
      << std::endl;

    // Clear the visited nodes so we can iterate over them once
    // more. This time we are generating the libs declaration.
    visited_nodes_.clear ();

    CUTS_BE_EISA->project_file_
      << "  libs += " << svnt_artifact_ << " " << skel_project;

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T <
                                  CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
                                  generate_stub_listing, _1));

    CUTS_BE_EISA->project_file_
      << std::endl
      << std::endl;
  }

  // Generate the default MPC information.
  generate_mpc_i (node);

  CUTS_BE_EISA->project_file_
    // Generate the source files.
    << "  Source_Files {" << std::endl
    << "    " << node.basename_ <<  CUTS_BE_OPTIONS ()->exec_suffix_
    << ".cpp" << std::endl
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
// generate_svnt_project
//
void CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
generate_svnt_project (const CUTS_BE_Impl_Node & node)
{
  CUTS_BE_Impl_Node::Artifact_Set::const_iterator iter_svnt =
    std::find_if (node.artifacts_.begin (), node.artifacts_.end (),
                  Element_Name_End_With <
                    CUTS_BE_Impl_Node::Artifact_Set::value_type> (
                    SVNT_SUFFIX));

  if (iter_svnt == node.artifacts_.end ())
    return;

  // Construct the names of the servant and skeleton project.
  std::string svnt_project = node.name_ + SVNT_SUFFIX;
  std::string skel_project = node.name_ + SKEL_SUFFIX;
  std::replace (skel_project.begin (), skel_project.end (), '/', '_');
  std::replace (skel_project.begin (), skel_project.end (), '\\', '_');

  // Create the export name for the project.
  std::string svnt_export = iter_svnt->name ();
  std::transform (svnt_export.begin (),
                  svnt_export.end (),
                  svnt_export.begin (),
                  &toupper);

  // Save the name for the servant project.
  svnt_project_ = std::string (iter_svnt->name ());
  svnt_artifact_ = basename (iter_svnt->location ());

  CUTS_BE_EISA->project_file_
    << "project (" << svnt_project_
    << ") : eisa_component_svnt, eisa_cuts_defaults {" << std::endl
    << "  sharedname   = " << basename (iter_svnt->location ()) << std::endl
    << std::endl
    << "  dynamicflags = " << svnt_export << "_BUILD_DLL" << std::endl
    << std::endl
    << "  prebuild = perl -- $(ACE_ROOT)/bin/generate_export_file.pl "
    << svnt_export << " > $(PROJECT_ROOT)/" << svnt_project << "_export.h" << std::endl
    << std::endl;

  // Generate the STUB dependencies for this node.
  if (!node.references_.empty ())
  {
    visited_nodes_.clear ();

    CUTS_BE_EISA->project_file_
      << "  after += " << skel_project;

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T <
                                  CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
                                  generate_stub_listing, _1));

    CUTS_BE_EISA->project_file_
      << std::endl
      << std::endl;

    // Clear the visited nodes so we can iterate over them once
    // more. This time we are generating the libs declaration.
    visited_nodes_.clear ();

    CUTS_BE_EISA->project_file_
      << "  libs += " << skel_project;

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T <
                                  CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
                                  generate_stub_listing, _1));

    CUTS_BE_EISA->project_file_
      << std::endl
      << std::endl;
  }

  CUTS_BE_EISA->project_file_
    // Generate the CIDL files
    << "  CIDL_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << std::endl

    // Generate the IDL files
    << "  IDL_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << std::endl

    // Generate the source files
    << "  Source_Files {" << std::endl
    << "    " << node.basename_ << SVNT_SUFFIX << ".cpp" << std::endl
    << "  }" << std::endl
    << std::endl

    // Generate the header files
    << "  Header_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << "}" << std::endl
    << std::endl;
}

//
// generate_skel_project
//
void CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
generate_skel_project (const CUTS_BE_Impl_Node & node)
{
  // The skeleton project for a component needs to ensure that there
  // is a servant project present. This is because the skeleton project
  // is responsible for generating the servant source files.
  CUTS_BE_Impl_Node::Artifact_Set::const_iterator iter_svnt =
    std::find_if (node.artifacts_.begin (), node.artifacts_.end (),
                  Element_Name_End_With <
                    CUTS_BE_Impl_Node::Artifact_Set::value_type> (
                    SVNT_SUFFIX));

  if (iter_svnt == node.artifacts_.end ())
    return;

  // Generator the export file for the CIAO servant project.
  std::string skel_project_name = node.name_ + SKEL_SUFFIX;
  std::string skel_export_file = node.name_ + SKEL_SUFFIX + "_export.h";

  std::replace (skel_project_name.begin (), skel_project_name.end (), '/', '_');
  std::replace (skel_project_name.begin (), skel_project_name.end (), '\\', '_');

  // Create the export name for the project.
  std::string skel_export = skel_project_name;
  std::transform (skel_export.begin (),
                  skel_export.end (),
                  skel_export.begin (),
                  &toupper);

  // Generator the export file for the CIAO servant project.
  std::string svnt_export_file = node.name_ + SVNT_SUFFIX + "_export.h";
  std::string svnt_export = iter_svnt->name ();

  std::transform (svnt_export.begin (),
                  svnt_export.end (),
                  svnt_export.begin (),
                  &toupper);

  CUTS_BE_EISA->project_file_
    << "project (" << skel_project_name
    << ") : eisa_component_skel, eisa_cuts_defaults {" << std::endl
    << "  sharedname   = " << skel_project_name << std::endl
    << std::endl
    << "  dynamicflags = " << skel_export << "_BUILD_DLL" << std::endl
    << std::endl
    << "  prebuild = perl -- $(ACE_ROOT)/bin/generate_export_file.pl "
    << skel_export << " > $(PROJECT_ROOT)/" << skel_export_file << std::endl
    << std::endl
    << "  cidlflags -= --" << std::endl
    << "  cidlflags += --svnt-export-macro " << svnt_export << "_Export"
    << " --svnt-export-include " << basename (svnt_export_file) << " --"
    << std::endl << std::endl
    << "  idlflags += -Wb,export_macro=" << skel_export << "_Export \\" << std::endl
    << "              -Wb,export_include=" << basename (skel_export_file)
    << std::endl
    << std::endl;

  // Generate the STUB dependencies for this node.
  if (!node.references_.empty ())
  {
    visited_nodes_.clear ();

    CUTS_BE_EISA->project_file_
      << "  after +=";

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T <
                                  CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
                                  generate_stub_listing, _1));

    CUTS_BE_EISA->project_file_
      << std::endl
      << std::endl;

    // Clear the visited nodes so we can iterate over them once
    // more. This time we are generating the libs declaration.
    visited_nodes_.clear ();

    CUTS_BE_EISA->project_file_
      << "  libs +=";

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T <
                                  CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
                                  generate_stub_listing, _1));

    CUTS_BE_EISA->project_file_
      << std::endl
      << std::endl;
  }

  CUTS_BE_EISA->project_file_
    // Generate the CIDL files
    << "  CIDL_Files {" << std::endl
    << "    " << node.basename_ << ".cidl" << std::endl
    << "  }" << std::endl
    << std::endl

    // Generate the IDL files
    << "  IDL_Files {" << std::endl
    << "    " << node.basename_ << "E.idl" << std::endl
    << "  }" << std::endl
    << std::endl

    // Generate the source files
    << "  Source_Files {" << std::endl
    << "    " << node.basename_ << "EC.cpp" << std::endl
    << "  }" << std::endl
    << std::endl

    // Generate the header files
    << "  Header_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << "}" << std::endl
    << std::endl;
}

//
// genereate_mpc_i
//
void CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
generate_mpc_i (const CUTS_BE_Impl_Node & node)
{
  generate_mpc_values ("includes", node.include_paths_);
  generate_mpc_values ("libpaths", node.lib_paths_);
  generate_mpc_values ("libs", node.lib_);
}

//
// generate_mpc_values
//
void CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
generate_mpc_values (const std::string & heading,
                     const CUTS_String_Set & listing)
{
  if (listing.empty ())
    return;

  CUTS_BE_EISA->project_file_
    << "  " << heading << " +=";

  for (CUTS_String_Set::const_iterator iter = listing.begin ();
       iter != listing.end ();
       iter ++)
  {
    CUTS_BE_EISA->project_file_
      << " \\" << std::endl
      << "    " << iter->c_str ();
  }

  CUTS_BE_EISA->project_file_
    << std::endl
    << std::endl;
}

//
// CUTS_BE_Project_Write_T
//
void CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
generate_stub_listing (const CUTS_BE_IDL_Node * node)
{
  IDL_Node_Set::const_iterator iter = visited_nodes_.find (node);

  if (iter == visited_nodes_.end ())
  {
    visited_nodes_.insert (node);

    CUTS_BE_EISA->project_file_
      << " \\" << std::endl
      << "    " << node->basename_ << STUB_SUFFIX;

    std::for_each (node->references_.begin (),
                   node->references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T <
                                  CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
                                  generate_stub_listing, _1));
  }
}

//
// CUTS_BE_Project_File_End_T
//
bool CUTS_BE_Project_File_End_T <CUTS_BE_Eisa, CUTS_BE_Impl_Node>::
generate (const CUTS_BE_Impl_Node & node)
{
  CUTS_BE_EISA->project_file_
    << "// end of auto-generated project file" << std::endl;

  return true;
}

//
// CUTS_BE_Project_File_Close_T
//
void CUTS_BE_Project_File_Close_T <
CUTS_BE_Eisa, CUTS_BE_Impl_Node>::generate (void)
{
  if (CUTS_BE_EISA->project_file_.is_open ())
    CUTS_BE_EISA->project_file_.close ();
}

///////////////////////////////////////////////////////////////////////////////
// project generators (CUTS_BE_IDL_Node)

//
// CUTS_BE_Project_File_Open_T
//
bool CUTS_BE_Project_File_Open_T <CUTS_BE_Eisa, CUTS_BE_IDL_Node>::
generate (const CUTS_BE_IDL_Node & node)
{
  // Clear the state of the output file.
  if (!CUTS_BE_EISA->project_file_.good ())
    CUTS_BE_EISA->project_file_.clear ();

  // Construct the name of the project file.
  std::ostringstream pathname;
  pathname
    << CUTS_BE_OPTIONS ()->output_directory_
    << "/" << node.name_ << "_stub.mpc";

  // Open the project for writing.
  CUTS_BE_EISA->project_file_.open (pathname.str ().c_str ());
  return CUTS_BE_EISA->project_file_.is_open ();
}

//
// CUTS_BE_Project_File_Begin_T
//
bool CUTS_BE_Project_File_Begin_T <CUTS_BE_Eisa, CUTS_BE_IDL_Node>::
generate (const CUTS_BE_IDL_Node & node)
{
  CUTS_BE_EISA->project_file_
    << "// This project file was generated by" << std::endl
    << "// $Id$"
    << std::endl
    << std::endl;

  return true;
}

//
// visited_nodes_
//
CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_IDL_Node>::IDL_Node_Set
CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_IDL_Node>::visited_nodes_;

//
// CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_IDL_Node>
//
bool CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_IDL_Node>::
generate (const CUTS_BE_IDL_Node & node)
{
  // Generate the export file for the project.
  std::string stub_name = node.basename_ + STUB_SUFFIX;

  // Construct the export name for the project.
  std::string stub_export = stub_name;
  std::transform (stub_export.begin (),
                  stub_export.end (),
                  stub_export.begin (),
                  &toupper);

  std::string stub_export_macro = stub_export + "_Export";
  std::string stub_export_file = stub_name + "_export.h";

  // Generate the project.
  CUTS_BE_EISA->project_file_
    << "project (" << stub_name << ") : eisa_component_stub, eisa_cuts_defaults {" << std::endl
    << "  sharedname   = " << stub_name << std::endl
    << std::endl
    << "  dynamicflags = " << stub_export << "_BUILD_DLL" << std::endl
    << std::endl
    << "  prebuild = perl -- $(ACE_ROOT)/bin/generate_export_file.pl "
    << stub_export << " > $(PROJECT_ROOT)/" << stub_export_file << std::endl
    << std::endl
    << "  idlflags += -Wb,export_macro=" << stub_export_macro << " \\" << std::endl
    << "              -Wb,export_include=" << stub_export_file << std::endl;

  if (!node.references_.empty ())
  {
    visited_nodes_.clear ();

    // Generate the dependencies for this project. This will be
    // all the client projects of the references for this node.
    CUTS_BE_EISA->project_file_
      << std::endl
      << "  after +=";

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T <
                                  CUTS_BE_Eisa, CUTS_BE_IDL_Node>::
                                  generate_stub_listing, _1));

    // Generate the import libraries for this project. This will be
    // all client project of the references for this node. The will
    // also be the same as those specified in the "after" tag.

    visited_nodes_.clear ();

    CUTS_BE_EISA->project_file_
      << std::endl
      << std::endl
      << "  libs  +=";

    std::for_each (node.references_.begin (),
                   node.references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T <
                                  CUTS_BE_Eisa, CUTS_BE_IDL_Node>::
                                  generate_stub_listing, _1));

    CUTS_BE_EISA->project_file_
      << std::endl;
  }

  CUTS_BE_EISA->project_file_
    << std::endl
    // Generate the IDL files for this project.
    << "  IDL_Files {" << std::endl
    << "    " << node.basename_ << ".idl" << std::endl
    << "  }" << std::endl
    << std::endl

    // Generate the source files for this project.
    << "  Source_Files {" << std::endl
    << "    " << node.basename_ << SERVER_SUFFIX << ".cpp" << std::endl
    << "    " << node.basename_ << CLIENT_SUFFIX << ".cpp" << std::endl
    << "  }" << std::endl
    << std::endl
    // Generate the header files for this project.
    << "  Header_Files {" << std::endl
    << std::endl
    << "  }" << std::endl
    << "}" << std::endl
    << std::endl;

  return true;
}

//
// generate_stub_listing
//
void CUTS_BE_Project_Write_T <CUTS_BE_Eisa, CUTS_BE_IDL_Node>::
generate_stub_listing (const CUTS_BE_IDL_Node * node)
{
  IDL_Node_Set::const_iterator iter = visited_nodes_.find (node);

  if (iter == visited_nodes_.end ())
  {
    visited_nodes_.insert (node);

    CUTS_BE_EISA->project_file_
      << " \\" << std::endl
      << "    " << node->basename_ << STUB_SUFFIX;

    std::for_each (node->references_.begin (),
                   node->references_.end (),
                   boost::bind (&CUTS_BE_Project_Write_T <
                                  CUTS_BE_Eisa, CUTS_BE_IDL_Node>::
                                  generate_stub_listing, _1));
  }
}

//
// CUTS_BE_Project_File_End_T
//
bool CUTS_BE_Project_File_End_T <CUTS_BE_Eisa, CUTS_BE_IDL_Node>::
generate (const CUTS_BE_IDL_Node & node)
{
  CUTS_BE_EISA->project_file_
    << "// end of auto-generated project file" << std::endl;

  return true;
}

//
// CUTS_BE_Project_File_Close_T
//
void CUTS_BE_Project_File_Close_T <
CUTS_BE_Eisa, CUTS_BE_IDL_Node>::generate (void)
{
  if (CUTS_BE_EISA->project_file_.is_open ())
    CUTS_BE_EISA->project_file_.close ();
}
