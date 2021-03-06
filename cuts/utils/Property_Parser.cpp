// $Id$

#include "Property_Parser.h"

//
// CUTS_Property_Parser
//
CUTS_Property_Parser::
CUTS_Property_Parser (CUTS_Property_Map & map)
: prop_map_ (map)
{

}

//
// ~CUTS_Property_Parser
//
CUTS_Property_Parser::~CUTS_Property_Parser (void)
{

}

//
// parse
//
bool CUTS_Property_Parser::parse_str (const ACE_CString & str)
{
  return this->parse (str.c_str ());
}

//
// parse
//
bool CUTS_Property_Parser::parse (const char * str)
{
  CUTS_Property_Parser_Grammar grammar (this->prop_map_);

  boost::spirit::classic::parse_info < > info =
    boost::spirit::classic::parse (str,
                                   grammar >> !boost::spirit::classic::end_p,
                                   boost::spirit::classic::space_p);

  return info.full;
}
