// -*- C++ -*-
// $Id$

//
// CUTS_BE_Type_Map
//
CUTS_INLINE
CUTS_BE_Type_Map::CUTS_BE_Type_Map (void)
{

}

//
// ~CUTS_BE_Type_Map
//
CUTS_INLINE
CUTS_BE_Type_Map::~CUTS_BE_Type_Map (void)
{

}

CUTS_INLINE
std::string CUTS_BE_Type_Map::value (const PICML::PredefinedType_in type)
{
  return this->predefined_type_map_[type->meta ()->name ()];
}
