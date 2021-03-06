// $Id: Dmac_Log_Format_Graph.h

#ifndef _CUTS_DMAC_LOG_FORMAT_GRAPH_H_
#define _CUTS_DMAC_LOG_FORMAT_GRAPH_H_

#include <string>
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/SString.h"
#include "boost/graph/adjacency_list.hpp"
#include "Dmac_Log_Format.h"
#include "Dmac_Execution.h"
#include "ace/Date_Time.h"
#include "Dmac_Candidate_Relation.h"
#include "Dmac_Utils.h"
#include "Dmac_export.h"

/**
 * @class CUTS_Dmac_Log_Format_Graph_Traits
 *
 * Trait class for CUTS_Dmac_Log_Format_Graph
 */

class CUTS_DMAC_Export CUTS_Dmac_Log_Format_Graph_Traits
{
public:
  struct log_format_t
  {
    typedef boost::vertex_property_tag kind;
  };

  typedef
    boost::property <boost::vertex_name_t, ACE_CString,
    boost::property <log_format_t, CUTS_Dmac_Log_Format *> >
    property_type;

};

/// Type defintion of the graph type
typedef
  boost::adjacency_list <boost::vecS,
                         boost::vecS,
                         boost::directedS,
                         CUTS_Dmac_Log_Format_Graph_Traits::property_type >
                         CUTS_Dmac_Log_Format_Graph_Type;


struct corelation_result
{
  double prob1;
  double prob2;
  bool corelated;
};

/**
 * @class CUTS_Dmac_Log_Format_Graph
 *
 * Represent the Dataflow model
 */

class CUTS_DMAC_Export CUTS_Dmac_Log_Format_Graph
{
public:

  /// Type definition to the vertex descriptor
  typedef boost::graph_traits <CUTS_Dmac_Log_Format_Graph_Type>::vertex_descriptor vertex_descriptor;

  /// Type definition to the vertex descriptor
  typedef boost::graph_traits <CUTS_Dmac_Log_Format_Graph_Type>::vertex_iterator vertex_iterator;

  /// Type definition of the edge descriptor
  typedef boost::graph_traits <CUTS_Dmac_Log_Format_Graph_Type>::edge_descriptor edge_descriptor;

  /// Default Constructor
  CUTS_Dmac_Log_Format_Graph (void);

  /// Desturctor
  ~CUTS_Dmac_Log_Format_Graph (void);

  /**
   * extend_graph
   *
   * @param[in]     log_formats     The set of abstract log formats
   * @param[in]     execution       The Execution Context.
   */
  void extend_graph (std::vector <CUTS_Dmac_Log_Format *> & log_formats,
                     CUTS_Dmac_Execution * execution);

  /**
   * find_inter_ec_relations
   *
   * @param[in]     log_formats     The set of abstract log formats
   * @param[in]     database        The test database.
   */
  void find_inter_ec_relations (std::vector <CUTS_Dmac_Log_Format *> & log_formats,
                                CUTS_Test_Database & testdata);

  void print_candidate_relations (void);

  bool is_reachable (CUTS_Dmac_Log_Format * lf1,
                     CUTS_Dmac_Log_Format * lf2);

  void populate_domain (const char * file_name,
                        CUTS_DMAC_UTILS::int_double_map & knowledge);

private:

  /// This method checks whether we need to add a log format pair
  bool check_for_addition (CUTS_DMAC_UTILS::int_pair & pair,
                           CUTS_DMAC_UTILS::int_vector & lf_order_list);

  void extend_graph (CUTS_Dmac_Log_Format * cause_lf,
                     CUTS_Dmac_Log_Format * effect_lf);

  void connect_inter_ec_log_formats (CUTS_Dmac_Log_Format * cause_lf,
                                     CUTS_Dmac_Log_Format * effect_lf);

  /// is_correlated
  void check_corelation (CUTS_Dmac_Log_Format * lf1,
                         CUTS_Dmac_Log_Format * lf2,
                         CUTS_Test_Database & testdata,
                         corelation_result & result);

  /// calculate_probability
  double calculate_probability (std::vector <double> & lf1_time_records,
                                std::vector <double> & lf2_time_records);

  // The dataflow graph
  CUTS_Dmac_Log_Format_Graph_Type graph_;

  /// Type definition of the vertex cache
  typedef ACE_Hash_Map_Manager <ACE_CString,
                                vertex_descriptor,
                                ACE_Null_Mutex> VERTEX_MAP;

  /// Local cache of the vertices
  VERTEX_MAP vertices_;

  /// Set of candidate relations
  std::vector <CUTS_Dmac_Candidate_Relation> cand_relations_;

};

#endif  // !defined _CUTS_DMAC_LOG_FORMAT_GRAPH_H_