// -*- C++ -*-

//=============================================================================
/**
 *  @file         Servant_T.h
 *
 *  $Id$
 *
 *  @author       James H. Hill
 */
//=============================================================================

#ifndef _ICCM_SERVANT_T_H_
#define _ICCM_SERVANT_T_H_

#include "ccm/CCM_ObjectS.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/SString.h"
#include "ace/UUID.h"
#include <set>

#include "cuts/config.h"
#include "cuts/iccm/servant/Receptacle.h"

namespace iCCM
{

// Forward decl.
class EventConsumer;

// Forward decl.
class Publisher;

// Forward decl.
class Publisher_Table;

/**
 * @class Servant_T
 */
template <typename T, typename CONTEXT, typename EXECUTOR, typename POA_EXEC, typename SERVANT_BASE>
class Servant_T :
  public POA_EXEC,
  public SERVANT_BASE
{
public:
  /// Type definition of the context type.
  typedef CONTEXT context_type;

  /// Type definition of the executor type.
  typedef EXECUTOR executor_type;

  typedef typename SERVANT_BASE::eventconsumer_type eventconsumer_type;
  typedef typename SERVANT_BASE::publisher_type publisher_type;
  typedef typename SERVANT_BASE::publisher_table_type publisher_table_type;

  /**
   * Initializing constructor.
   *
   * @param[in]       svnt_mgr        Manager of the servant.
   * @param[in]       executor        Executor component for servant.
   */
  Servant_T (T * this_,
             const char * name,
             ::PortableServer::POA_ptr poa,
             typename EXECUTOR::_ptr_type exec);

  /// Destructor.
  virtual ~Servant_T (void);

  /// Activate the component.
  virtual void activate_component (void);

  /// Passivate the component.
  virtual void passivate_component (void);

  // event-related methods
  virtual ::Components::Cookie *
    subscribe (const char * publisher_name,
                ::Components::EventConsumerBase_ptr subscriber);

  virtual ::Components::EventConsumerBase_ptr
    unsubscribe (const char * publisher_name, ::Components::Cookie * ck);

  virtual Components::EventConsumerBase_ptr
    get_consumer (const char *);

  virtual void
    connect_consumer (const char *, Components::EventConsumerBase_ptr);

  virtual Components::EventConsumerBase_ptr
    disconnect_consumer (const char *);

#if !defined (CCM_LW)
  virtual Components::ConsumerDescriptions *
    get_all_consumers (void);

  virtual Components::ConsumerDescriptions *
    get_named_consumers (const Components::NameList &);

  virtual Components::EmitterDescriptions *
    get_all_emitters (void);

  virtual Components::EmitterDescriptions *
    get_named_emitters (const Components::NameList &);

  virtual Components::PublisherDescriptions *
    get_all_publishers (void);

  virtual Components::PublisherDescriptions *
    get_named_publishers (const Components::NameList &);
#endif

  // facet/receptacle methods

  ::CORBA::Object_ptr provide_facet (const char *);

#if !defined (CCM_LW)
  ::Components::FacetDescriptions * get_all_facets (void);

  ::Components::FacetDescriptions * get_named_facets (const Components::NameList &);
#endif

  virtual ::Components::Cookie * connect (const char *, ::CORBA::Object_ptr);

  virtual ::CORBA::Object_ptr disconnect (const char *, ::Components::Cookie *);

#if !defined (CCM_LW)
  ::Components::ConnectionDescriptions * get_connections (const char *);

  ::Components::ReceptacleDescriptions * get_all_receptacles (void);

  ::Components::ReceptacleDescriptions * get_named_receptacles (const Components::NameList &);
#endif


#if !defined (CCM_LW)
  ::CORBA::Boolean same_component (::CORBA::Object_ptr);

  ::CORBA::IRObject_ptr get_component_def (void);

  ::Components::PrimaryKeyBase * get_primary_key (void);

  ::Components::ComponentPortDescription * get_all_ports (void);
#endif

  ::Components::CCMHome_ptr get_ccm_home (void);

  virtual void configuration_complete (void);

  virtual void remove (void);

  int get_consumer (const char * name,
                    typename SERVANT_BASE::eventconsumer_type * & result) const;

  int get_publisher (const char * name,
                     typename SERVANT_BASE::publisher_type * & result) const;

  int get_publisher_table (const char * name,
                           typename SERVANT_BASE::publisher_table_type * & result) const;

  virtual void add_eventconsumer (const char * name,
                                  typename SERVANT_BASE::eventconsumer_type * consumer);
  virtual void add_eventconsumer (const char * name, EventConsumer * consumer);

  virtual void add_publisher (const char * name,
                              typename SERVANT_BASE::publisher_type * publisher);
  virtual void add_publisher (const char * name, Publisher * publisher);

  virtual void add_publisher_table (const char * name,
                                    typename SERVANT_BASE::publisher_table_type * publisher_table);
  virtual void add_publisher_table (const char * name, Publisher_Table * publisher_table);

protected:
  /// Collection of consumers for the servant.
  typedef ACE_Hash_Map_Manager <ACE_CString,
                                typename SERVANT_BASE::eventconsumer_type *,
                                ACE_RW_Thread_Mutex>
                                consumer_map_type;

  consumer_map_type consumers_;

  /// Collection of publish endpoints.
  typedef ACE_Hash_Map_Manager <ACE_CString,
                                typename SERVANT_BASE::publisher_table_type *,
                                ACE_RW_Thread_Mutex>
                                publishes_map_type;

  publishes_map_type publishes_;

  /// Collection of emits endpoints.
  typedef ACE_Hash_Map_Manager <ACE_CString,
                                typename SERVANT_BASE::publisher_type *,
                                ACE_RW_Thread_Mutex>
                                emits_map_type;

  emits_map_type emits_;

  /// Collection of receptacle endpoints.
  typedef ACE_Hash_Map_Manager <ACE_CString,
                               Receptacle *,
                               ACE_RW_Thread_Mutex>
                               receptacle_map_type;

  receptacle_map_type receptacles_;


  /// Collection of facet endpoints.
  typedef std::pair < ::CORBA::Object_var, ACE_Utils::UUID > facets_values_type;
  typedef ACE_Hash_Map_Manager <ACE_CString,
                                facets_values_type,
                                ACE_RW_Thread_Mutex>
                                facets_map_type;

  facets_map_type facets_;

  /// The actual context for the servant.
  ACE_Auto_Ptr <CONTEXT> ctx_;

  /// The implemenation for this servant.
  typename EXECUTOR::_var_type impl_;

  // Helper method for adding facets
  void add_facet (const char *, ::CORBA::Object_ptr);

private:
  // Helper method to create the port POA.
  void initialize_the_port_POA (const char *, ::PortableServer::POA_ptr);

protected:
  // Helper method to deactivate all ports.
  void deactivate_ports (void);

  // Helper method to activate all ports.
  void activate_ports (void);

private:
  /// The POA for activating the ports.
  ::PortableServer::POA_var port_POA_;
};

/**
 * Standard method for creating an iCCM servant.
 *
 * @param[in]       name      Name of the servant
 * @param[in]       p         Pointer to the executor
 */
template <typename EXECUTOR, typename SERVANT>
::PortableServer::Servant
create_servant (const char * name,
                ::PortableServer::POA_ptr port_POA,
                ::Components::EnterpriseComponent_ptr p);


}

#if defined (__CUTS_INLINE__)
#include "Servant_T.inl"
#endif

#include "Servant_T.cpp"

#endif  // !defined _ICCM_SERVANT_T_H_
