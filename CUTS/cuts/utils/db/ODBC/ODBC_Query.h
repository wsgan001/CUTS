// -*- C++ -*-

//=============================================================================
/**
 * @file    ODBC_Query.h
 *
 * $Id$
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_ODBC_QUERY_H_
#define _CUTS_ODBC_QUERY_H_

#include "ODBC_Base.h"
#include "ODBC_Parameter_List.h"
#include "cuts/utils/db/DB_Query.h"
#include "ace/Auto_Ptr.h"

// Forward decl.
class ODBC_Record;

//=============================================================================
/**
 * @class ODBC_Query
 *
 * @brief Wrapper class for using ODBC statement handles.
 *
 * This class contains common operations used by CUTS for executing
 * SQL statements using ODBC. One cannot instantiate an object of this
 * class alone. Instead, it can only be created by an ODBC_Connection
 * object. This prevents the creation of dangling ODBC_Query objects
 * (those not associated with a connection).
 */
//=============================================================================

class CUTS_ODBC_Export ODBC_Query :
  public ODBC_Base,
  public CUTS_DB_Query
{
  /// Friend decl.
  friend class ODBC_Record;

  /// Friend decl.
  friend class ODBC_Parameter_List;

  /// Friend decl.
  friend class ODBC_Parameter;

public:
  /**
   * Initializing constructor.
   *
   * @param[in]    conn     Parent database connection.
   */
  ODBC_Query (HDBC conn);

  /// Destructor.
  virtual ~ODBC_Query (void);

  /// Execute a prepare query.
  virtual void execute_no_record (void);

  /**
   * Execute a query.
   *
   * @param[in]     query       Query to execute.
   */
  virtual void execute_no_record (const char * query);

  /**
   * Prepare an SQL query for execution.
   *
   * @param[in]     query       NULL-terminate SQL query string.
   */
  virtual void prepare (const char * query);

  /**
   * Get the last insert id. This method is only value if an
   * insert was made to a table with an \a auto_increment field.
   *
   * @return The last id inserted.
   */
  virtual long last_insert_id (void);

  /// Cancel the current operation.
  virtual void cancel (void);

  /**
   * Execute a query. This method is useful with the query is known
   * to return results that will consist of multiple data rows, and
   * columns. The client has the responsibility of delete the record
   * once its done with it.
   *
   * @return  Pointer to a record.
   */
  virtual CUTS_DB_Record * execute (const char * query);

  /**
   * Execute a prepared query. This method is useful with the query is
   * known to return results that will consist of multiple data rows,
   * and columns. The client has the responsibility of delete the record
   * once its done with it.
   *
   * @return  Pointer to a record.
   */
  virtual CUTS_DB_Record * execute (void);

  /// Get a read-only list of the parameters.
  virtual const ODBC_Parameter_List & parameters (void) const;

  /// Get a list of the parameters.
  virtual ODBC_Parameter_List & parameters (void);

  /// Reset the query string.
  void reset (void);

private:
  /// Reset the statement.
  void reset_i (void);

  /// Get the pointer to the record.
  ODBC_Record * record_i (void);

  /// handle to the ODBC statement
  HSTMT stmt_;

  /// The state of the cursor.
  int cursor_open_;

  /// Pointer to the parameters for the method.
  ODBC_Parameter_List params_;
};

#if defined (__CUTS_INLINE__)
#include "ODBC_Query.inl"
#endif

#endif  // !defined _CUTS_ODBC_QUERY_H_