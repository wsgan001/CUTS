// $Id$
//
// ****              Code generated by the                        ****
// ****  Component Integrated ACE ORB (CIAO) CIDL Compiler v0.6.8 ****
// CIAO has been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// CIDL Compiler has been developed by:
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about CIAO is available at:
//    http://www.dre.vanderbilt.edu/CIAO

#ifndef CIAO_GLUE_SESSION_HELLOWORLD_SVNT_H
#define CIAO_GLUE_SESSION_HELLOWORLD_SVNT_H

#include /**/ "ace/pre.h"

#include "HelloWorld_svnt_export.h"

#include "HelloWorldEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Containers/Container_BaseC.h"
#include "ciao/Contexts/Context_Impl_T.h"
#include "ciao/Servants/Servant_Impl_T.h"
#include "ciao/Servants/Home_Servant_Impl_T.h"

#include "HelloWorldS.h"

namespace CIDL_HelloWorld_Impl
{
  class HelloWorld_Servant;

  class HELLOWORLD_SVNT_Export HelloWorld_Context
    : public virtual ::CIAO::Context_Impl < ::CCM_HelloWorld_Context, HelloWorld_Servant, ::HelloWorld>
  {
    public:
    // We will allow the servant glue code we generate to access our state.
    friend class HelloWorld_Servant;

    // Type definition of the base type
    typedef ::CIAO::Context_Impl < ::CCM_HelloWorld_Context, HelloWorld_Servant, ::HelloWorld> base_type;

    // Type definition of the CCM context type
    typedef base_type::context_type context_type;

    // Type definition of the servant type
    typedef base_type::servant_type servant_type;

    // Type definition of the component type
    typedef base_type::component_type component_type;

    HelloWorld_Context (
      ::Components::CCMHome_ptr h,
      ::CIAO::Container_ptr c,
      HelloWorld_Servant *sv);

    virtual ~HelloWorld_Context (void);

    // Operations for HelloWorld receptacles and event sources,
    // defined in ::CCM_HelloWorld_Context.

    virtual void
    push_handle_message (
      ::Message *ev);

    // CIAO-specific.

    static HelloWorld_Context *
    _narrow ( ::Components::SessionContext_ptr p);

    protected:
    // Methods that manage this component's connections and consumers.

    virtual void
    connect_handle_message (
      ::MessageConsumer_ptr c);

    virtual ::MessageConsumer_ptr
    disconnect_handle_message ();

    protected:
    ::MessageConsumer_var
    ciao_emits_handle_message_consumer_;
  };

  class HELLOWORLD_SVNT_Export HelloWorld_Servant
    : public virtual CIAO::Servant_Impl<
        POA_HelloWorld,
        ::CCM_HelloWorld,
        HelloWorld_Context
      >
  {
    public:

    typedef ::CCM_HelloWorld _exec_type;

    HelloWorld_Servant (
      ::CCM_HelloWorld_ptr executor,
      ::Components::CCMHome_ptr h,
      const char *ins_name,
      ::CIAO::Home_Servant_Impl_Base *hs,
      ::CIAO::Container_ptr c);

    virtual ~HelloWorld_Servant (void);

    virtual void
    set_attributes (const ::Components::ConfigValues &descr
    );

    // Supported operations.

    // Public port operations.

    // Servant class for the handle_message consumer.
    class HELLOWORLD_SVNT_Export MessageConsumer_handle_message_Servant
    : public virtual POA_MessageConsumer
    {
      public:
      MessageConsumer_handle_message_Servant (
        ::CCM_HelloWorld_ptr executor,
        ::CCM_HelloWorld_Context_ptr c);

      virtual ~MessageConsumer_handle_message_Servant (void);

      virtual void
      push_Message (
        ::Message *evt);

      // Inherited from ::Components::EventConsumerBase.
      virtual void
      push_event ( ::Components::EventBase *ev);

      // CIAO-specific in ::Components::EventConsumerBase.
      virtual void
      ciao_push_event ( ::Components::EventBase *ev,
                       const char * source_id,
                       ::CORBA::TypeCode_ptr tc);

      // CIAO-specific in ::Components::EventConsumerBase.
      virtual CORBA::Boolean
      ciao_is_substitutable (const char *event_repo_id);

      // Get component implementation.
      virtual CORBA::Object_ptr
      _get_component ();

      protected:
      ::CCM_HelloWorld_var
      executor_;

      ::CCM_HelloWorld_Context_var
      ctx_;
    };

    virtual ::MessageConsumer_ptr
    get_consumer_handle_message ();

    virtual void
    connect_handle_message (
      ::MessageConsumer_ptr c);

    virtual ::MessageConsumer_ptr
    disconnect_handle_message ();

    // Component attribute operations.

    // Operations for Receptacles interface.

    virtual ::Components::Cookie *
    connect (const char *name,
             CORBA::Object_ptr connection);

    virtual CORBA::Object_ptr
    disconnect (const char *name,
                ::Components::Cookie *ck);

    virtual ::Components::ReceptacleDescriptions *
    get_all_receptacles ();

    // Operations for Events interface.

    virtual ::Components::Cookie *
    subscribe (const char *publisher_name,
               ::Components::EventConsumerBase_ptr subscriber);

    virtual ::Components::EventConsumerBase_ptr
    unsubscribe (const char *publisher_name,
                 ::Components::Cookie *ck);

    virtual void
    connect_consumer (const char *emitter_name,
                      ::Components::EventConsumerBase_ptr consumer);

    virtual ::Components::EventConsumerBase_ptr
    disconnect_consumer (const char *source_name);

    virtual ::Components::PublisherDescriptions *
    get_all_publishers ();

    virtual ::Components::EmitterDescriptions *
    get_all_emitters ();

    // CIAO specific operations on the servant. 
    CORBA::Object_ptr
    get_facet_executor (const char *name);

    private:

    ::MessageConsumer_var
    consumes_handle_message_;

    const char *ins_name_;

    private:

    void
    populate_port_tables ();

    ::Components::EventConsumerBase_ptr
    get_consumer_handle_message_i ();
  };

  extern "C" HELLOWORLD_SVNT_Export ::PortableServer::Servant
  create_HelloWorld_Servant (
    ::Components::EnterpriseComponent_ptr p,
    CIAO::Container_ptr c,
    const char *ins_name);

  class HELLOWORLD_SVNT_Export HelloWorldHome_Servant
    : public virtual
        ::CIAO::Home_Servant_Impl<
            ::POA_HelloWorldHome,
            ::CCM_HelloWorldHome,
            HelloWorld_Servant
          >
  {
    public:

    HelloWorldHome_Servant (
      ::CCM_HelloWorldHome_ptr exe,
      const char *ins_name,
      ::CIAO::Container_ptr c);

    virtual ~HelloWorldHome_Servant (void);

    // Home operations.
    // Home factory and finder operations.

    // Attribute operations.
  };

  extern "C" HELLOWORLD_SVNT_Export ::PortableServer::Servant
  create_HelloWorldHome_Servant (
    ::Components::HomeExecutorBase_ptr p,
    CIAO::Container_ptr c,
    const char *ins_name);
}

#include /**/ "ace/post.h"

#endif /* CIAO_GLUE_SESSION_HELLOWORLD_SVNT_H */

