// -*- C++ -*-

//=============================================================================
/**
 *  @file         TCPIP_Servant_T.h
 *
 *  $Id$
 *
 *  @author       James H. Hill
 */
//=============================================================================

#ifndef _TCPIP_SERVANT_T_H_
#define _TCPIP_SERVANT_T_H_

#include "TCPIP_Servant.h"
#include "TCPIP_Servant_VTable_T.h"

// Forward decl.
class CUTS_TCPIP_Component;

/**
 * @class CUTS_TCPIP_Servant_T
 */
template <typename T, typename CTX, typename EXEC>
class CUTS_TCPIP_Servant_T :
  public CUTS_TCPIP_Servant
{
public:
  /// Type definition of the servant type.
  typedef T servant_type;

  /// Type definition of the context type.
  typedef CTX context_type;

  /// Type definition of the executor type.
  typedef EXEC executor_type;

  // Method for handling the event. It will dispatch the events
  // to the correct method.
  virtual int handle_event (ACE_UINT32 id, CUTS_TCPIP_InputCDR & input);

protected:
  /**
   * Initializing constructor
   *
   * @param[in]           servant         Target servant.
   */
  CUTS_TCPIP_Servant_T (T * servant, typename EXEC::_ptr_type exec);

  /// Destructor.
  virtual ~CUTS_TCPIP_Servant_T (void);

  /// Type definition of the virtual table.
  typedef CUTS_TCPIP_Servant_VTable_T <T> vtable_type;

  /// Virtual table for the servant.
  static vtable_type vtable_;

private:
  /// Pointer to the servant.
  T * servant_;

protected:
  /// The actual context for the servant.
  ACE_Auto_Ptr <CTX> ctx_;

  /// The implemenation for this servant.
  typename EXEC::_var_type impl_;
};

#if defined (__CUTS_INLINE__)
#include "TCPIP_Servant_T.inl"
#endif

#include "TCPIP_Servant_T.cpp"

#endif  // !defined _TCPIP_SERVANT_T_H_
