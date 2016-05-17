// -*- C++ -*-

#ifndef _ICCM_CHAOS_DEPLOYMENT_HANDLERS_EXPORT_H_
#define _ICCM_CHAOS_DEPLOYMENT_HANDLERS_EXPORT_H_

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (ICCM_CHAOS_DEPLOYMENT_HANDLERS_HAS_DLL)
#  define ICCM_CHAOS_DEPLOYMENT_HANDLERS_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && ICCM_CHAOS_DEPLOYMENT_HANDLERS_HAS_DLL */

#if !defined (ICCM_CHAOS_DEPLOYMENT_HANDLERS_HAS_DLL)
#  define ICCM_CHAOS_DEPLOYMENT_HANDLERS_HAS_DLL 1
#endif /* ! ICCM_CHAOS_DEPLOYMENT_HANDLERS_HAS_DLL */

#if defined (ICCM_CHAOS_DEPLOYMENT_HANDLERS_HAS_DLL) && (ICCM_CHAOS_DEPLOYMENT_HANDLERS_HAS_DLL == 1)
#  if defined (ICCM_CHAOS_DEPLOYMENT_HANDLERS_BUILD_DLL)
#    define ICCM_CHAOS_DEPLOYMENT_HANDLERS_Export ACE_Proper_Export_Flag
#    define ICCM_CHAOS_DEPLOYMENT_HANDLERS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define ICCM_CHAOS_DEPLOYMENT_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* ICCM_CHAOS_DEPLOYMENT_HANDLERS_BUILD_DLL */
#    define ICCM_CHAOS_DEPLOYMENT_HANDLERS_Export ACE_Proper_Import_Flag
#    define ICCM_CHAOS_DEPLOYMENT_HANDLERS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define ICCM_CHAOS_DEPLOYMENT_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* ICCM_CHAOS_DEPLOYMENT_HANDLERS_BUILD_DLL */
#else /* ICCM_CHAOS_DEPLOYMENT_HANDLERS_HAS_DLL == 1 */
#  define ICCM_CHAOS_DEPLOYMENT_HANDLERS_Export
#  define ICCM_CHAOS_DEPLOYMENT_HANDLERS_SINGLETON_DECLARATION(T)
#  define ICCM_CHAOS_DEPLOYMENT_HANDLERS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* ICCM_CHAOS_DEPLOYMENT_HANDLERS_HAS_DLL == 1 */

// Set ICCM_CHAOS_DEPLOYMENT_HANDLERS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (ICCM_CHAOS_DEPLOYMENT_HANDLERS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define ICCM_CHAOS_DEPLOYMENT_HANDLERS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define ICCM_CHAOS_DEPLOYMENT_HANDLERS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !ICCM_CHAOS_DEPLOYMENT_HANDLERS_NTRACE */

#if (ICCM_CHAOS_DEPLOYMENT_HANDLERS_NTRACE == 1)
#  define ICCM_CHAOS_DEPLOYMENT_HANDLERS_TRACE(X)
#else /* (ICCM_CHAOS_DEPLOYMENT_HANDLERS_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define ICCM_CHAOS_DEPLOYMENT_HANDLERS_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (ICCM_CHAOS_DEPLOYMENT_HANDLERS_NTRACE == 1) */

#endif /* _ICCM_CHAOS_DEPLOYMENT_HANDLERS_EXPORT_H_ */

// End of auto generated file.
