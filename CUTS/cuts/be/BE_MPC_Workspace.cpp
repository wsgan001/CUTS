// $Id$

#include "BE_MPC_Workspace.h"

#if !defined (__CUTS_INLINE__)
#include "BE_MPC_Workspace.inl"
#endif

#include "BE_Options.h"
#include <sstream>

//
// open_workspace_file
//
bool CUTS_BE_MPC_Workspace::open_workspace_file (void)
{
  // Construct the name of the workspace.
  this->workspace_name_ = CUTS_BE_OPTIONS ()->project_name_ + "_CUTS";

  // Construct the name of the workspace file.
  std::ostringstream ostr;
  ostr
    << CUTS_BE_OPTIONS ()->output_directory_
    << "/" << this->workspace_name_ << ".mwc";

  // Open the workspace file for writing.
  this->outfile_.open (ostr.str ().c_str ());
  return this->outfile_.is_open ();
}

//
// write_prologue
//
void CUTS_BE_MPC_Workspace::write_prologue (void)
{
  this->outfile_
    << "//==================================================================="
    << std::endl
    << "// Makefile, Project, and Workspace Creator File" << std::endl
    << "//" << std::endl
    << "// This file was generated by" << std::endl
    << "// $Id$" << std::endl
    << "//" << std::endl
    << "// Please do not modify the contents of this file under" << std::endl
    << "// any circumstances. Doing so can result in failure to" << std::endl
    << "// successfully build parts of (or the entire) workspace." << std::endl
    << "//==================================================================="
    << std::endl
    << std::endl
    << "workspace (" << this->workspace_name_ << ") {" << std::endl
    << "  // -- initialize the includes for the project." << std::endl
    << "  cmdline += -include $CUTS_ROOT/etc/config \\" << std::endl
    << "             -feature_file $CUTS_ROOT/default.features \\" << std::endl
    << "             -relative PROJECT_ROOT=" << std::endl
    << std::endl;
}
