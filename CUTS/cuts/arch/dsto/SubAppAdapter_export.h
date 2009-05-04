
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SubAppAdapter
// ------------------------------
#ifndef SUBAPPADAPTER_EXPORT_H
#define SUBAPPADAPTER_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (SUBAPPADAPTER_HAS_DLL)
#  define SUBAPPADAPTER_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && SUBAPPADAPTER_HAS_DLL */

#if !defined (SUBAPPADAPTER_HAS_DLL)
#  define SUBAPPADAPTER_HAS_DLL 1
#endif /* ! SUBAPPADAPTER_HAS_DLL */

#if defined (SUBAPPADAPTER_HAS_DLL) && (SUBAPPADAPTER_HAS_DLL == 1)
#  if defined (SUBAPPADAPTER_BUILD_DLL)
#    define SubAppAdapter_Export ACE_Proper_Export_Flag
#    define SUBAPPADAPTER_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define SUBAPPADAPTER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* SUBAPPADAPTER_BUILD_DLL */
#    define SubAppAdapter_Export ACE_Proper_Import_Flag
#    define SUBAPPADAPTER_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define SUBAPPADAPTER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* SUBAPPADAPTER_BUILD_DLL */
#else /* SUBAPPADAPTER_HAS_DLL == 1 */
#  define SubAppAdapter_Export
#  define SUBAPPADAPTER_SINGLETON_DECLARATION(T)
#  define SUBAPPADAPTER_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* SUBAPPADAPTER_HAS_DLL == 1 */

// Set SUBAPPADAPTER_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (SUBAPPADAPTER_NTRACE)
#  if (ACE_NTRACE == 1)
#    define SUBAPPADAPTER_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define SUBAPPADAPTER_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !SUBAPPADAPTER_NTRACE */

#if (SUBAPPADAPTER_NTRACE == 1)
#  define SUBAPPADAPTER_TRACE(X)
#else /* (SUBAPPADAPTER_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define SUBAPPADAPTER_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (SUBAPPADAPTER_NTRACE == 1) */

#endif /* SUBAPPADAPTER_EXPORT_H */

// End of auto generated file.