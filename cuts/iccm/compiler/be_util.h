/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_util.h
 *
 *  $Id$
 *
 *  Static helper methods used by multiple visitors.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_BE_UTIL_H
#define TAO_BE_UTIL_H

class AST_Generator;

class be_util
{
public:
  /// Special BE arg call factored out of DRV_args.
  static void prep_be_arg (char *s);

  /// Checks made after parsing args.
  static void arg_post_proc (void);

  /// Display usage of BE-specific options.
  static void usage (void);

  /// Create an AST node generator.
  static AST_Generator * generator_init (void);
};

#endif // if !defined

