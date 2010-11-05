// -*- C++ -*-

#ifndef _HELLOWORLD_SVNT_H_
#define _HELLOWORLD_SVNT_H_

#include "HelloWorldEC.h"
#include "HelloWorldS.h"

#include "cuts/arch/tcpip/ccm/TCPIP_CCM_Context_T.h"
#include "cuts/arch/tcpip/ccm/TCPIP_CCM_Remote_Endpoint_T.h"
#include "cuts/arch/tcpip/ccm/TCPIP_CCM_Servant_T.h"
#include "cuts/arch/tcpip/ccm/TCPIP_CCM_Subscriber_Table_T.h"

#include "HelloWorld_svnt_export.h"

// Forward decl.
class CUTS_TCPIP_Servant_Manager;

// Forward decl.
class HelloWorld_Servant;

typedef CUTS_TCPIP_CCM_Context_T < ::CCM_HelloWorld_Context, HelloWorld_Servant > HelloWorld_Servant_Context_Base;

/**
 * @class HelloWorld_Servant_Context
 */
class HelloWorld_Servant_Context : public HelloWorld_Servant_Context_Base
{
  public:
  // default constructor
  HelloWorld_Servant_Context (HelloWorld_Servant & parent);

  // destructor
  virtual ~HelloWorld_Servant_Context (void);

  public:
  // push method for output ev port: output_message
  virtual void push_output_message (::Message * ev);

  CUTS_TCPIP_CCM_Remote_Endpoint & endpoint_output_message (void);

  private:
  CUTS_TCPIP_CCM_Remote_Endpoint_T < ::Message > output_message_;

  public:
  // push method for output ev port: handle_message_ex
  virtual void push_handle_message_ex (::Message * ev);

  CUTS_TCPIP_CCM_Subscriber_Table & endpoints_handle_message_ex (void);

  private:
  CUTS_TCPIP_CCM_Subscriber_Table_T < ::Message > handle_message_ex_;
};


// Type definition of the this->servant_'s base class
typedef CUTS_TCPIP_CCM_Servant_T < 
  HelloWorld_Servant,
  HelloWorld_Servant_Context,
  CIAO_HelloWorld_Impl::HelloWorld_Exec,
  ::POA_HelloWorld > HelloWorld_Servant_Base;

/**
 * @class HelloWorld_Servant
 */
class HelloWorld_Servant : public HelloWorld_Servant_Base
{
  public:
  // default constructor
  HelloWorld_Servant (const char * name, executor_type::_ptr_type executor);

  // destructor
  virtual ~HelloWorld_Servant (void);

  void connect_output_message (::MessageConsumer_ptr);

  ::MessageConsumer_ptr disconnect_output_message (void);

  ::Components::Cookie * subscribe_handle_message_ex (::MessageConsumer_ptr);

  ::MessageConsumer_ptr unsubscribe_handle_message_ex (::Components::Cookie *);

  public:
  ::MessageConsumer_ptr get_consumer_input_message (void);

  static int tcpip_input_message (HelloWorld_Servant * svnt,
                                  CUTS_TCPIP_InputCDR & input);

  private:
  CUTS_TCPIP_CCM_EventConsumer input_message_consumer_;

  public:
  virtual void message (const char * message);

  virtual char * message (void);

  virtual char * readonly_message (void);

  // Set the component's attributes
  virtual void set_attributes (const ::Components::ConfigValues & config);
};

extern "C" HELLOWORLD_SVNT_Export
::PortableServer::Servant 
create_HelloWorld_Servant (const char *, ::Components::EnterpriseComponent_ptr);


#endif  // !defined _HELLOWORLD_SVNT_H_
