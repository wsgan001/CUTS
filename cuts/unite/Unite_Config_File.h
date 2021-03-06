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

#ifndef _CUTS_UNITE_CONFIG_FILE_H_
#define _CUTS_UNITE_CONFIG_FILE_H_

#include "cuts-unite.h"
#include "XSC/utils/XML_Schema_Resolver.h"
#include "XSCRT/utils/File_Reader_T.h"

/**
 * @ingroup UNITE_XML
 *
 * @class CUTS_Unite_Config_File
 *
 * Helper class for parsing a UNITE configuration file.
 */
class CUTS_UNITE_XML_Export CUTS_Unite_Config_File :
  public XSCRT::utils::File_Reader_T < ::CUTS::XML::testConfig >
{
public:
  /// Type definition of the XML reader type.
  typedef XSCRT::utils::File_Reader_T < ::CUTS::XML::testConfig > reader_type;

  /// Default constructor.
  CUTS_Unite_Config_File (void);

  /// Destructor.
  ~CUTS_Unite_Config_File (void);

private:
  void configure (void);

  XSC::XML::Basic_Resolver < > br_;

  XSC::XML::XML_Schema_Resolver < ::XSC::XML::Basic_Resolver < > > resolver_;
};

#endif  // !defined _CUTS_UNITE_CONFIG_FILE_H_
