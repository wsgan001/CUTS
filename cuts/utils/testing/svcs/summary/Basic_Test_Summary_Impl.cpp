// $Id$

#include "Basic_Test_Summary_Impl.h"

#if !defined (__CUTS_INLINE__)
#include "Basic_Test_Summary_Impl.inl"
#endif

#include "cuts/utils/testing/Testing_App.h"
#include "ace/UUID.h"
#include "ace/SString.h"
#include <sstream>

//
// generate
//
int CUTS_Basic_Test_Summary_Impl::
generate (std::ostream & out, const CUTS_Testing_App_Base & app) const
{
  std::ostringstream ostr;

  // Buffer the test summary before output it.
  ostr
    << "CUTS Test Summary" << std::endl
    << "=============================================================" << std::endl
    << "  Name       : " << app.options ().name_.c_str () << std::endl
    << "  UUID       : " << app.options ().uuid_.to_string ()->c_str  () << std::endl
    << "  Start Time : ";

  if (app.options ().start_ != ACE_Time_Value::zero)
    ostr << app.options ().start_ << std::endl;
  else
    ostr << "N/A" << std::endl;

  ostr
    << "  Stop Time  : ";

  if (app.options ().stop_ != ACE_Time_Value::zero)
    ostr << app.options ().stop_  << std::endl;
  else
    ostr << "N/A" << std::endl;

  // Write the string to the output stream.
  out << ostr.str () << std::endl;

  return 0;
}
