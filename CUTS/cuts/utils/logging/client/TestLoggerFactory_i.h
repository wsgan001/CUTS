// -*- C++ -*-

//=============================================================================
/**
 * @file          TestLoggerFactory_i.h
 *
 * $Id$
 *
 * @author        James H. Hill
 */
//=============================================================================

#ifndef _CUTS_TEST_LOGGER_FACTORY_I_H_
#define _CUTS_TEST_LOGGER_FACTORY_I_H_

#include "clientS.h"
#include "Logging_Client_export.h"
#include "ace/UUID.h"

/**
 * @class CUTS_TestLoggerFactory_i
 *
 * Implementation of the CUTS::TestLoggerClient interface.
 */
class CUTS_TestLoggerFactory_i :
  public POA_CUTS::TestLoggerFactory
{
public:
  /**
   * Initializing constructor.
   *
   * @param[in]       test_number     Test number for the factory
   * @param[in]       poa             POA this object is activated in
   */
  CUTS_TestLoggerFactory_i (const ACE_Utils::UUID & test_uuid,
                            CUTS::TestLoggerServer_ptr server,
                            PortableServer::POA_ptr poa);

  /// Destructor.
  virtual ~CUTS_TestLoggerFactory_i (void);

  /**
   * Factory method for creating a new test logger for the
   * associated test.
   *
   * @return        The
   */
  virtual CUTS::TestLogger_ptr create (void);

  /**
   * Destory the specified logger. This will decrement is reference
   * count and remove it from the listing, if necessary.
   *
   * @param[in]       logger        Target logger to destroy.
   */
  virtual void destroy (CUTS::TestLogger_ptr logger);

  /**
   * Get the POA assigned to the test logger factory.
   *
   * @return          The assigned POA.
   */
  virtual PortableServer::POA_ptr _default_POA (void);

  const ACE_Utils::UUID & test_uuid (void) const;

  CUTS::TestLoggerServer_ptr server (void);

  void server (CUTS::TestLoggerServer_ptr s);

private:
  /// Test number for the factory.
  ACE_Utils::UUID test_uuid_;

  /// Number of loggers.
  size_t log_count_;

  /// POA that the test logger factory activated in.
  PortableServer::POA_var default_POA_;

  /// POA that the test logger factory activated in.
  PortableServer::POA_var logger_POA_;

  /// Lock for synchronizing assessing the servant collection.
  ACE_Thread_Mutex lock_;

  /// Reference to the logger server.
  CUTS::TestLoggerServer_var server_;
};

#if defined (__CUTS_INLINE__)
#include "TestLoggerFactory_i.inl"
#endif

#endif  // !defined _CUTS_TEST_LOGGER_FACTORY_I_H_
