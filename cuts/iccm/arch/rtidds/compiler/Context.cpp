// $Id$

#include "Context.h"

#include "Stub_File.h"

#include "ast_component.h"
#include "ast_module.h"
#include "ast_root.h"
#include "ast_sequence.h"
#include "ast_typedef.h"

#include "utl_identifier.h"
#include "utl_string.h"

#include "fe_extern.h"
#include "global_extern.h"
#include "nr_extern.h"
#include "idl_defines.h"

#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

namespace iCCM
{
namespace RTIDDS
{

//
// Context
//
Context::Context (void)
: typeseq_suffix_ ("Seq")
{

}

//
// ~Context
//
Context::~Context (void)
{

}

//
// post_produce
//
void Context::post_produce (void)
{
  iCCM::Stub_File stub_file;
  idl_global->root ()->ast_accept (&stub_file);
}

//
// prep_be_arg
//
void Context::prep_be_arg (char * s)
{
  static const char typeseq_suffix[] = "typeseq_suffix=";

  char* last = 0;

  for (char * arg = ACE_OS::strtok_r (s, ",", &last);
       0 != arg;
       arg = ACE_OS::strtok_r (0, ",", &last))
  {
    if (ACE_OS::strstr (arg, typeseq_suffix) == arg)
    {
      char * val = arg + sizeof (typeseq_suffix) - 1;
      this->typeseq_suffix_ = val;
    }
    else
    {
      // Pass control to the base class.
      BE_GlobalData::prep_be_arg (s);
    }
  }
}

//
// get_stub_file_prefix
//
const ACE_CString & Context::get_stub_file_prefix (void) const
{
  static const ACE_CString value ("RTIDDS_");
  return value;
}

//
// get_context_template_header_filename
//
const ACE_CString & Context::
get_context_template_header_filename (void) const
{
  static const ACE_CString value ("cuts/iccm/servant/Context_T.h");
  return value;
}

//
// get_servant_template_header_filename
//
const ACE_CString & Context::
get_servant_template_header_filename (void) const
{
  static const ACE_CString value ("cuts/iccm/arch/rtidds/servant/RTIDDS_Servant_T.h");
  return value;
}

//
// get_context_typename
//
const ACE_CString & Context::get_context_typename (void) const
{
  static const ACE_CString value ("iCCM::Context_T");
  return value;
}

//
// get_servant_template_typename
//
const ACE_CString & Context::get_servant_template_typename (void) const
{
  static const ACE_CString value ("iCCM::RTIDDS_Servant_T");
  return value;
}

//
// get_publisher_typename
//
const ACE_CString & Context::get_publisher_typename (void) const
{
  static const ACE_CString value ("iCCM::RTIDDS_Publisher");
  return value;
}

//
// get_publisher_template_typename
//
const ACE_CString & Context::get_publisher_template_typename (void) const
{
  static const ACE_CString value ("iCCM::RTIDDS_Publisher_T");
  return value;
}

//
// get_publisher_table_typename
//
const ACE_CString & Context::
get_publisher_table_typename (void) const
{
  static const ACE_CString value ("iCCM::RTIDDS_Publisher_Table");
  return value;
}

//
// get_publisher_table_template_typename
//
const ACE_CString & Context::
get_publisher_table_template_typename (void) const
{
  static const ACE_CString value ("iCCM::RTIDDS_Publisher_Table_T");
  return value;
}

//
// get_publisher_table_template_typename
//
const ACE_CString & Context::
get_consumer_template_typename (void) const
{
  static const ACE_CString value ("iCCM::RTIDDS_EventConsumer_T");
  return value;
}

}
}
