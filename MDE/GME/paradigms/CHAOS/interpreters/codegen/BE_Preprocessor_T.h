// -*- C++ -*-

//=============================================================================
/**
 * @file      BE_Preprocessor_T.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_BE_PREPROCESSOR_T_H_
#define _CUTS_BE_PREPROCESSOR_T_H_

#include "BE_IDL_Graph.h"
#include "BE_Impl_Graph_T.h"
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"

/**
 * @class CUTS_BE_Preprocessor_T
 *
 * Preprocessor object for CHAOS models. This object provides
 * methods for preprocessing stub and implemenation models. The
 * preprocessed data is stored in the respective singleton
 * objects, i.e., CUTS_BE_IDL_Graph
 */
template <typename T>
class CUTS_BE_Preprocessor_T :
  public CHAOS::Visitor
{
public:
  /// Default constructor.
  CUTS_BE_Preprocessor_T (void);

  /// Destructor.
  ~CUTS_BE_Preprocessor_T (void);

  /**
   * Preprocess a file.
   *
   * @param[in]     file            Source file.
   * @retval        true            Success
   * @retval        false           Failure
   */
  bool preprocess (const CHAOS::File & file);

  /**
   * Preprocess a file.
   *
   * @param[in]     file            Source file.
   * @retval        true            Success
   * @retval        false           Failure
   */
  bool preprocess (const CHAOS::File & file, const CUTS_BE_IDL_Node * & node);

  /**
   * Preprocess an implementation.
   *
   * @param[in]     container       Source implementation.
   * @retval        true            Success
   * @retval        false           Failure
   */
  bool preprocess (
    const CHAOS::ComponentImplementationContainer & container);

  /**
   * Remove an implementation from the proprocessor.
   *
   * @param[in]     container       The target containe to remove.
   */
  void remove (
    const CHAOS::ComponentImplementationContainer & container);

  /**
   * Get a reference to the preprocessed implementations. Clients
   * are not able to modify the implementation graph.
   *
   * @return  Reference to a implementation graph.
   */
  const CUTS_BE_Impl_Graph_T <T> & impls (void) const;

  /**
   * Get a reference to the preprocessed implementations. Clients
   * are not able to modify the implementation graph.
   *
   * @return  Reference to a implementation graph.
   */
  CUTS_BE_Impl_Graph_T <T> & impls (void);

  /**
   * Get a reference to the preprocessed stub (i.e., interface
   * definition files). Clients are not able to modify the
   * implementation graph.
   *
   * @return  Reference to a stub graph.
   */
  const CUTS_BE_IDL_Graph & stubs (void) const;

  /**
   * Get a reference to the preprocessed stub (i.e., interface
   * definition files). Clients are not able to modify the
   * implementation graph.
   *
   * @return  Reference to a stub graph.
   */
  CUTS_BE_IDL_Graph & stubs (void);

private:
  /// The IDL graph.
  CUTS_BE_IDL_Graph idl_graph_;

  /// The implementation graph.
  CUTS_BE_Impl_Graph_T <T> impl_graph_;

  // prevent the following operations
  CUTS_BE_Preprocessor_T (const CUTS_BE_Preprocessor_T &);
  const CUTS_BE_Preprocessor_T & operator = (const CUTS_BE_Preprocessor_T &);
};

/// Definition for the preprocessor singleton.
#define CUTS_BE_PREPROCESSOR(TYPE) \
  ACE_Singleton <CUTS_BE_Preprocessor_T <TYPE>, \
                 ACE_Null_Mutex>::instance ()

#if defined (__CUTS_INLINE__)
#include "BE_Preprocessor_T.inl"
#endif

#include "BE_Preprocessor_T.cpp"

#endif  // !defined _CUTS_BE_PREPROCESSOR_T_H_
