
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl CUTS_RTIDDS
// ------------------------------
#ifndef CUTS_RTIDDS_EXPORT_H
#define CUTS_RTIDDS_EXPORT_H

#include "cuts/config.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (CUTS_RTIDDS_HAS_DLL)
#  define CUTS_RTIDDS_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && CUTS_RTIDDS_HAS_DLL */

#if !defined (CUTS_RTIDDS_HAS_DLL)
#  define CUTS_RTIDDS_HAS_DLL 1
#endif /* ! CUTS_RTIDDS_HAS_DLL */

#if defined (CUTS_RTIDDS_HAS_DLL) && (CUTS_RTIDDS_HAS_DLL == 1)
#  if defined (CUTS_RTIDDS_BUILD_DLL)
#    define CUTS_RTIDDS_Export ACE_Proper_Export_Flag
#    define CUTS_RTIDDS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define CUTS_RTIDDS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* CUTS_RTIDDS_BUILD_DLL */
#    define CUTS_RTIDDS_Export ACE_Proper_Import_Flag
#    define CUTS_RTIDDS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define CUTS_RTIDDS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* CUTS_RTIDDS_BUILD_DLL */
#else /* CUTS_RTIDDS_HAS_DLL == 1 */
#  define CUTS_RTIDDS_Export
#  define CUTS_RTIDDS_SINGLETON_DECLARATION(T)
#  define CUTS_RTIDDS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* CUTS_RTIDDS_HAS_DLL == 1 */

// Set CUTS_RTIDDS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (CUTS_RTIDDS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define CUTS_RTIDDS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define CUTS_RTIDDS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !CUTS_RTIDDS_NTRACE */

#if (CUTS_RTIDDS_NTRACE == 1)
#  define CUTS_RTIDDS_TRACE(X)
#else /* (CUTS_RTIDDS_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define CUTS_RTIDDS_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (CUTS_RTIDDS_NTRACE == 1) */

#endif /* CUTS_RTIDDS_EXPORT_H */

// End of auto generated file.
