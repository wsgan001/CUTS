#ifndef TEST_H
#define TEST_H

// Forward declarations.
//
namespace CUTS
{
  class serviceDescription;
  class serviceList;
  class testFile;
  class processOptions;
  class CommandOptions;
  class CommandList;
}

#include <memory>
#include <string>
#include <list>
#include "XMLSchema/Types.hpp"
#include "XMLSchema/id_map.hpp"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Null_Mutex.h"
#include "ace/TSS_T.h"
#include "ace/ace_wchar.h"
#include "ace/Singleton.h"

namespace CUTS
{
  class serviceDescription : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    public:
    typedef ACE_Refcounted_Auto_Ptr < ::CUTS::serviceDescription, ACE_Null_Mutex > _ptr;

    // location
    // 
    public:
    ::XMLSchema::string< char > const& location () const;
    void location (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > location_;

    // entryPoint
    // 
    public:
    ::XMLSchema::string< char > const& entryPoint () const;
    void entryPoint (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > entryPoint_;

    // params
    // 
    public:
    bool params_p () const;
    ::XMLSchema::string< char > const& params () const;
    void params (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > params_;

    // id
    // 
    public:
    ::XMLSchema::ID< char > const& id () const;
    ::XMLSchema::ID< char >& id ();
    void id (::XMLSchema::ID< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::ID< char > > id_;

    public:
    serviceDescription (::XMLSchema::string< char > const& location__,
                        ::XMLSchema::string< char > const& entryPoint__,
                        ::XMLSchema::ID< char > const& id__);

    serviceDescription (::XSCRT::XML::Element< char > const&);
    serviceDescription (serviceDescription const& s);

    serviceDescription&
    operator= (serviceDescription const& s);

    private:
    char regulator__;
  };


  class serviceList : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    public:
    typedef ACE_Refcounted_Auto_Ptr < ::CUTS::serviceList, ACE_Null_Mutex > _ptr;

    // service
    // 
    public:
    typedef ::std::list< ACE_Refcounted_Auto_Ptr < ::CUTS::serviceDescription, ACE_Null_Mutex > >::iterator service_iterator;
    typedef ::std::list< ACE_Refcounted_Auto_Ptr < ::CUTS::serviceDescription, ACE_Null_Mutex > >::const_iterator service_const_iterator;
    service_iterator begin_service ();
    service_iterator end_service ();
    service_const_iterator begin_service () const;
    service_const_iterator end_service () const;
    void add_service ( ACE_Refcounted_Auto_Ptr < ::CUTS::serviceDescription, ACE_Null_Mutex > const& );
    XSCRT::Type* get_service_ptr ( std::basic_string<char> idref );
    void set_service_ptr (std::basic_string<char> idref );
    size_t count_service (void) const;

    protected:
    ::std::list< ACE_Refcounted_Auto_Ptr < ::CUTS::serviceDescription, ACE_Null_Mutex > > service_;

    public:
    serviceList ();

    serviceList (::XSCRT::XML::Element< char > const&);
    serviceList (serviceList const& s);

    serviceList&
    operator= (serviceList const& s);

    private:
    char regulator__;
  };


  class testFile : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    public:
    typedef ACE_Refcounted_Auto_Ptr < ::CUTS::testFile, ACE_Null_Mutex > _ptr;

    // startup
    // 
    public:
    bool startup_p () const;
    ::CUTS::processOptions const& startup () const;
    void startup (::CUTS::processOptions const& );

    protected:
    ::std::auto_ptr< ::CUTS::processOptions > startup_;

    // testops
    // 
    public:
    bool testops_p () const;
    ::CUTS::CommandList const& testops () const;
    void testops (::CUTS::CommandList const& );

    protected:
    ::std::auto_ptr< ::CUTS::CommandList > testops_;

    // shutdown
    // 
    public:
    bool shutdown_p () const;
    ::CUTS::processOptions const& shutdown () const;
    void shutdown (::CUTS::processOptions const& );

    protected:
    ::std::auto_ptr< ::CUTS::processOptions > shutdown_;

    // services
    // 
    public:
    bool services_p () const;
    ::CUTS::serviceList const& services () const;
    void services (::CUTS::serviceList const& );

    protected:
    ::std::auto_ptr< ::CUTS::serviceList > services_;

    public:
    testFile ();

    testFile (::XSCRT::XML::Element< char > const&);
    testFile (testFile const& s);

    testFile&
    operator= (testFile const& s);

    private:
    char regulator__;
  };


  class processOptions : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    public:
    typedef ACE_Refcounted_Auto_Ptr < ::CUTS::processOptions, ACE_Null_Mutex > _ptr;

    // executable
    // 
    public:
    ::XMLSchema::string< char > const& executable () const;
    void executable (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > executable_;

    // arguments
    // 
    public:
    bool arguments_p () const;
    ::XMLSchema::string< char > const& arguments () const;
    void arguments (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > arguments_;

    // workingdirectory
    // 
    public:
    bool workingdirectory_p () const;
    ::XMLSchema::string< char > const& workingdirectory () const;
    void workingdirectory (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > workingdirectory_;

    // output
    // 
    public:
    bool output_p () const;
    ::XMLSchema::string< char > const& output () const;
    void output (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > output_;

    // error
    // 
    public:
    bool error_p () const;
    ::XMLSchema::string< char > const& error () const;
    void error (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > error_;

    public:
    processOptions (::XMLSchema::string< char > const& executable__);

    processOptions (::XSCRT::XML::Element< char > const&);
    processOptions (processOptions const& s);

    processOptions&
    operator= (processOptions const& s);

    private:
    char regulator__;
  };


  class CommandOptions : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    public:
    typedef ACE_Refcounted_Auto_Ptr < ::CUTS::CommandOptions, ACE_Null_Mutex > _ptr;

    // executable
    // 
    public:
    ::XMLSchema::string< char > const& executable () const;
    void executable (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > executable_;

    // arguments
    // 
    public:
    bool arguments_p () const;
    ::XMLSchema::string< char > const& arguments () const;
    void arguments (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > arguments_;

    // workingdirectory
    // 
    public:
    bool workingdirectory_p () const;
    ::XMLSchema::string< char > const& workingdirectory () const;
    void workingdirectory (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > workingdirectory_;

    // output
    // 
    public:
    bool output_p () const;
    ::XMLSchema::string< char > const& output () const;
    void output (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > output_;

    // error
    // 
    public:
    bool error_p () const;
    ::XMLSchema::string< char > const& error () const;
    void error (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > error_;

    // id
    // 
    public:
    ::XMLSchema::ID< char > const& id () const;
    ::XMLSchema::ID< char >& id ();
    void id (::XMLSchema::ID< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::ID< char > > id_;

    // delay
    // 
    public:
    bool delay_p () const;
    ::XMLSchema::double_ const& delay () const;
    ::XMLSchema::double_& delay ();
    void delay (::XMLSchema::double_ const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::double_ > delay_;

    // waitforcompletion
    // 
    public:
    bool waitforcompletion_p () const;
    ::XMLSchema::boolean const& waitforcompletion () const;
    ::XMLSchema::boolean& waitforcompletion ();
    void waitforcompletion (::XMLSchema::boolean const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::boolean > waitforcompletion_;

    public:
    CommandOptions (::XMLSchema::string< char > const& executable__,
                    ::XMLSchema::ID< char > const& id__);

    CommandOptions (::XSCRT::XML::Element< char > const&);
    CommandOptions (CommandOptions const& s);

    CommandOptions&
    operator= (CommandOptions const& s);

    private:
    char regulator__;
  };


  class CommandList : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    public:
    typedef ACE_Refcounted_Auto_Ptr < ::CUTS::CommandList, ACE_Null_Mutex > _ptr;

    // command
    // 
    public:
    typedef ::std::list< ACE_Refcounted_Auto_Ptr < ::CUTS::CommandOptions, ACE_Null_Mutex > >::iterator command_iterator;
    typedef ::std::list< ACE_Refcounted_Auto_Ptr < ::CUTS::CommandOptions, ACE_Null_Mutex > >::const_iterator command_const_iterator;
    command_iterator begin_command ();
    command_iterator end_command ();
    command_const_iterator begin_command () const;
    command_const_iterator end_command () const;
    void add_command ( ACE_Refcounted_Auto_Ptr < ::CUTS::CommandOptions, ACE_Null_Mutex > const& );
    XSCRT::Type* get_command_ptr ( std::basic_string<char> idref );
    void set_command_ptr (std::basic_string<char> idref );
    size_t count_command (void) const;

    protected:
    ::std::list< ACE_Refcounted_Auto_Ptr < ::CUTS::CommandOptions, ACE_Null_Mutex > > command_;

    public:
    CommandList ();

    CommandList (::XSCRT::XML::Element< char > const&);
    CommandList (CommandList const& s);

    CommandList&
    operator= (CommandList const& s);

    private:
    char regulator__;
  };
}

namespace CUTS
{
  namespace reader
  {
    ::CUTS::testFile
    test (xercesc::DOMDocument const*);
  }
}

#include "test.inl"

#endif // TEST_H
