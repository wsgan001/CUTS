// -*- C++ -*-

//=============================================================================
/**
 *  @file         Portico_Publisher_Table.h
 *
 *  $Id$
 *
 *  Code generated by iCCM skeleton project generator that is include
 *  in the CUTS (http://cuts.cs.iupui.edu) and developed by:
 *
 *      Software Engineering & Distributed System (SEDS) Group
 *      Indiana Univesity-Purdue University Indianapolis
 *      Indianapolis, IN 46202
 */
//=============================================================================

#ifndef _ICCM_PORTICO_PUBLISHER_TABLE_H_
#define _ICCM_PORTICO_PUBLISHER_TABLE_H_

#include "cuts/iccm/servant/Publisher_Table.h"

#include "Portico_svnt_export.h"

#include "ace/UUID.h"
#include "RTI.hh"

namespace iCCM
{

/**
 * @class Portico_Publisher_Table
 */
class ICCM_PORTICO_SVNT_Export Portico_Publisher_Table :
  public Publisher_Table
{
public:
  /// Default constructor.
  Portico_Publisher_Table (void);

  /// Destructor.
  virtual ~Portico_Publisher_Table (void);

  /**
   * Initialize this publisher's handle.
   *
   * @param[in]           rtiamb            Pointer to the ambassador
   */
  virtual void init (RTI::RTIambassador * rtiamb) = 0;

  /// Finalized the publisher table.
  virtual void fini (void);

protected:
  /// Pointer to the ambassador.
  RTI::RTIambassador * rtiamb_;

  /// Object instance this table is publishing.
  RTI::ObjectHandle obj_handle_;

  /// Tag assigned to this publisher table.
  ACE_Utils::UUID tag_;
};

}

#if defined (__CUTS_INLINE__)
#include "Portico_Publisher_Table.inl"
#endif

#endif  // !defined _ICCM_PORTICO_PUBLISHER_TABLE_H_