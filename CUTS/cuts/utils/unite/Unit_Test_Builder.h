// -*- C++ -*-

//=============================================================================
/**
 * @file        Unite_Config_File.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _CUTS_UNITE_UNIT_TEST_BUILDER_H_
#define _CUTS_UNITE_UNIT_TEST_BUILDER_H_

#include "cuts-unite.h"
#include "Unite_Xml_export.h"

// Forward decl.
class CUTS_Unit_Test;

// Forward decl.
class CUTS_Log_Format;

// Forward decl.
class CUTS_Log_Format_Relation;

/**
 * @class CUTS_Unit_Test_Builder
 *
 * Helper class that converts the datagraph in a XML file into
 * a CUTS_Unit_Test object.
 */
class CUTS_UNITE_XML_Export CUTS_Unit_Test_Builder
{
public:
  /// Default constructor.
  CUTS_Unit_Test_Builder (void);

  /// Destructor.
  ~CUTS_Unit_Test_Builder (void);

  /**
   * Build the unit test.
   *
   * @param[in]       config        Source configuration
   * @param[out]      test          Target unit test object.
   */
  bool build (const CUTS::testConfig & config, CUTS_Unit_Test & test);
};

#endif  // !defined _CUTS_UNITE_UNIT_TEST_BUILDER_H_
