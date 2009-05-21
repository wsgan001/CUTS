// $Id$

#include "Input_Stream_Source_Generator.h"
#include "Input_Stream_Aggr_Member_Generator.h"
#include "Input_Stream_Event_Member_Generator.h"
#include "boost/bind.hpp"
#include <algorithm>

namespace CUTS_BE_TCPIP
{
//
// Input_Stream_Source_Generator
//
Input_Stream_Source_Generator::
Input_Stream_Source_Generator (std::ostream & out)
: out_ (out)
{

}

//
// ~Input_Stream_Source_Generator
//
Input_Stream_Source_Generator::~Input_Stream_Source_Generator (void)
{

}

//
// Visit_Event
//
void Input_Stream_Source_Generator::Visit_Event (const PICML::Event & ev)
{
  this->out_ << "ACE_CDR::Boolean operator >> (CUTS_TCPIP_InputCDR & stream, " << ev.name () << " & ev)"
             << "{";

  std::set <PICML::Member> members = ev.Member_children ();
  Input_Stream_Event_Member_Generator emg (this->out_);

  std::for_each (members.begin (),
                 members.end (),
                 boost::bind (&PICML::Member::Accept, _1, boost::ref (emg)));

  this->out_ << "return stream.good_bit ();"
             << "}";
}

//
// Visit_Aggregate
//
void Input_Stream_Source_Generator::
Visit_Aggregate (const PICML::Aggregate & aggr)
{
  this->out_ << "ACE_CDR::Boolean operator >> (CUTS_TCPIP_InputCDR & stream, " << aggr.name () << " & val)"
             << "{";

  Input_Stream_Aggr_Member_Generator amg (this->out_);
  std::set <PICML::Member> members = aggr.Member_children ();

  std::for_each (members.begin (),
                 members.end (),
                 boost::bind (&PICML::Member::Accept, _1, boost::ref (amg)));

  this->out_ << "return stream.good_bit ();"
             << "}";
}

//
// Visit_Collection
//
void Input_Stream_Source_Generator::
Visit_Collection (const PICML::Collection & coll)
{
  std::string name = coll.name ();

  this->out_ << "ACE_CDR::Boolean operator >> (CUTS_TCPIP_InputCDR & stream, " << coll.name () << " & coll)"
             << "{"
             << "// set the length of the collection" << std::endl
             << "size_t length;"
             << "stream >> length;"
             << "coll.length (length);"
             << std::endl
             << "// extract the values of the collection" << std::endl
             << name << "::value_type * iter = coll.get_buffer ();"
             << name << "::value_type * iter_end = iter + length;"
             << std::endl
             << "while (iter != iter_end)" << std::endl
             << "  stream >> *iter ++;"
             << std::endl
             << "return stream.good_bit ();"
             << "}";
}

}