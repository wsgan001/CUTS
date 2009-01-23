#ifndef MODELGEN_H
#define MODELGEN_H

// Forward declarations.
//
namespace CUTS
{
  class Generator_Description;
  class Configuration;
  class Generators;
}

#include <memory>
#include <list>
#include "XMLSchema/Types.hpp"

namespace CUTS
{
  class Generator_Description : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    // location
    // 
    public:
    ::XMLSchema::string< char > const& location () const;
    void location (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > location_;

    // id
    // 
    public:
    ::XMLSchema::ID< char > const& id () const;
    ::XMLSchema::ID< char >& id ();
    void id (::XMLSchema::ID< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::ID< char > > id_;

    public:
    Generator_Description (::XMLSchema::string< char > const& location__,
                           ::XMLSchema::ID< char > const& id__);

    Generator_Description (::XSCRT::XML::Element< char > const&);
    Generator_Description (Generator_Description const& s);

    Generator_Description&
    operator= (Generator_Description const& s);

    private:
    char regulator__;
  };


  class Configuration : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    // backend
    // 
    public:
    bool backend_p () const;
    ::CUTS::Generators const& backend () const;
    void backend (::CUTS::Generators const& );

    protected:
    ::std::auto_ptr< ::CUTS::Generators > backend_;

    public:
    Configuration ();

    Configuration (::XSCRT::XML::Element< char > const&);
    Configuration (Configuration const& s);

    Configuration&
    operator= (Configuration const& s);

    private:
    char regulator__;
  };


  class Generators : public ::XSCRT::Type
  {
    typedef ::XSCRT::Type Base;

    // generator
    // 
    public:
    typedef ::std::list< ::CUTS::Generator_Description >::iterator generator_iterator;
    typedef ::std::list< ::CUTS::Generator_Description >::const_iterator generator_const_iterator;
    generator_iterator begin_generator ();
    generator_iterator end_generator ();
    generator_const_iterator begin_generator () const;
    generator_const_iterator end_generator () const;
    void add_generator (::CUTS::Generator_Description const& );
    size_t count_generator (void) const;

    protected:
    ::std::list< ::CUTS::Generator_Description > generator_;

    public:
    Generators ();

    Generators (::XSCRT::XML::Element< char > const&);
    Generators (Generators const& s);

    Generators&
    operator= (Generators const& s);

    private:
    char regulator__;
  };
}

namespace CUTS
{
  namespace reader
  {
    ::CUTS::Configuration
    modelgen (xercesc::DOMDocument const*);
  }
}

#include "modelgen.inl"

#endif // MODELGEN_H
