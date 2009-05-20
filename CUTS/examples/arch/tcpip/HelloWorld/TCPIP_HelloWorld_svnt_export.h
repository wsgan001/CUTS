
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl TCPIP_HELLOWORLD_SVNT
// ------------------------------
#ifndef TCPIP_HELLOWORLD_SVNT_EXPORT_H
#define TCPIP_HELLOWORLD_SVNT_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (TCPIP_HELLOWORLD_SVNT_HAS_DLL)
#  define TCPIP_HELLOWORLD_SVNT_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && TCPIP_HELLOWORLD_SVNT_HAS_DLL */

#if !defined (TCPIP_HELLOWORLD_SVNT_HAS_DLL)
#  define TCPIP_HELLOWORLD_SVNT_HAS_DLL 1
#endif /* ! TCPIP_HELLOWORLD_SVNT_HAS_DLL */

#if defined (TCPIP_HELLOWORLD_SVNT_HAS_DLL) && (TCPIP_HELLOWORLD_SVNT_HAS_DLL == 1)
#  if defined (TCPIP_HELLOWORLD_SVNT_BUILD_DLL)
#    define TCPIP_HELLOWORLD_SVNT_Export ACE_Proper_Export_Flag
#    define TCPIP_HELLOWORLD_SVNT_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define TCPIP_HELLOWORLD_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* TCPIP_HELLOWORLD_SVNT_BUILD_DLL */
#    define TCPIP_HELLOWORLD_SVNT_Export ACE_Proper_Import_Flag
#    define TCPIP_HELLOWORLD_SVNT_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define TCPIP_HELLOWORLD_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* TCPIP_HELLOWORLD_SVNT_BUILD_DLL */
#else /* TCPIP_HELLOWORLD_SVNT_HAS_DLL == 1 */
#  define TCPIP_HELLOWORLD_SVNT_Export
#  define TCPIP_HELLOWORLD_SVNT_SINGLETON_DECLARATION(T)
#  define TCPIP_HELLOWORLD_SVNT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* TCPIP_HELLOWORLD_SVNT_HAS_DLL == 1 */

// Set TCPIP_HELLOWORLD_SVNT_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (TCPIP_HELLOWORLD_SVNT_NTRACE)
#  if (ACE_NTRACE == 1)
#    define TCPIP_HELLOWORLD_SVNT_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define TCPIP_HELLOWORLD_SVNT_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !TCPIP_HELLOWORLD_SVNT_NTRACE */

#if (TCPIP_HELLOWORLD_SVNT_NTRACE == 1)
#  define TCPIP_HELLOWORLD_SVNT_TRACE(X)
#else /* (TCPIP_HELLOWORLD_SVNT_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define TCPIP_HELLOWORLD_SVNT_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (TCPIP_HELLOWORLD_SVNT_NTRACE == 1) */

#endif /* TCPIP_HELLOWORLD_SVNT_EXPORT_H */

// End of auto generated file.
