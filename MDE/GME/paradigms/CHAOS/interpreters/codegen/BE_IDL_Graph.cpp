// $Id$

#include "StdAfx.h"
#include "BE_IDL_Graph.h"
#include "boost/bind.hpp"
#include "Uml.h"

//
// CUTS_BE_IDL_Graph
//
CUTS_BE_IDL_Graph::CUTS_BE_IDL_Graph (void)
{

}

//
// ~CUTS_BE_IDL_Graph
//
CUTS_BE_IDL_Graph::~CUTS_BE_IDL_Graph (void)
{

}

//
// Visit_RootFolder
//
void CUTS_BE_IDL_Graph::
Visit_RootFolder (const CHAOS::RootFolder & root)
{
  typedef std::set <CHAOS::InterfaceDefinitions> IDefs_Set;
  IDefs_Set defs = root.InterfaceDefinitions_kind_children ();

  for (auto def : defs)
    def.Accept (*this);
}

//
// Visit_InterfaceDefinitions
//
void CUTS_BE_IDL_Graph::
Visit_InterfaceDefinitions (const CHAOS::InterfaceDefinitions & folder)
{
  typedef std::set <CHAOS::File> File_Set;
  File_Set files = folder.File_kind_children ();

  for (auto file : files)
    file.Accept (*this);
}

//
// Visit_File
//
void CUTS_BE_IDL_Graph::
Visit_File (const CHAOS::File & file)
{
  // Find this node in the graph then visit the <file>.
  if (this->find (file.name (), this->current_node_) == -1)
    return;

  // We are trying to keep from parsing this file more than once.
  // So, if we are have already visited this file, then we do
  // not need to do it again!! Just a waste of time.
  if (!this->current_node_->flags_[CUTS_BE_IDL_Node::IDL_VISITED])
  {
    // Set the visit flag. See above for reason.
    this->current_node_->flags_[CUTS_BE_IDL_Node::IDL_VISITED] = true;
    this->current_node_->file_ = file;

    // Save the active file and visit its contents.
    this->active_file_ = file;
    this->visit_file_and_package_contents (file);

    while (!this->pending_files_.empty ())
    {
      // Get the first file in the collection.
      File_Set::iterator iter = this->pending_files_.begin ();
      CHAOS::File file = *iter;

      // Remove the file from the collection and visit it.
      this->pending_files_.erase (iter);
      file.Accept (*this);
    }
  }
}

//
// visit_file_and_package_contents
//
void CUTS_BE_IDL_Graph::
visit_file_and_package_contents (const Udm::Object & obj)
{
  // Visit all the events at this level.
  typedef std::vector <CHAOS::Event> Event_Set;

  Event_Set events =
    Udm::ChildrenAttr <CHAOS::Event> (
    obj.__impl (), Udm::NULLCHILDROLE);

  if (!events.empty ())
    this->current_node_->has_events_ = true;

  for (auto event : events)
    event.Accept (*this);

  // Visit all the objects at this level.
  typedef std::vector <CHAOS::Object> Object_Set;

  Object_Set objects =
    Udm::ChildrenAttr <CHAOS::Object> (
    obj.__impl (), Udm::NULLCHILDROLE);

  for (auto object : objects)
    object.Accept (*this);

  // Visit all the components at this level.
  typedef std::vector <CHAOS::Component> Component_Set;

  Component_Set components =
    Udm::ChildrenAttr <CHAOS::Component> (
    obj.__impl (), Udm::NULLCHILDROLE);

  for (auto component : components)
    component.Accept (*this);

  // Visit all the packages at this level.
  typedef std::set <CHAOS::Package> Package_Set;

  Package_Set packages =
    Udm::ChildrenAttr <CHAOS::Package> (
    obj.__impl (), Udm::NULLCHILDROLE);

  for (auto package : packages)
    package.Accept (*this);
}

//
// Visit_Package
//
void CUTS_BE_IDL_Graph::
Visit_Package (const CHAOS::Package & package)
{
  this->visit_file_and_package_contents (package);
}

//
// Visit_Component
//
void CUTS_BE_IDL_Graph::
Visit_Component (const CHAOS::Component & component)
{
  // Determine if the component has output events.
  typedef std::vector <CHAOS::OutEventPort> OutEventPort_Set;
  OutEventPort_Set oep_set = component.OutEventPort_kind_children ();

  for (auto oep : oep_set)
    oep.Accept (*this);

  // Determine if the component has input events.
  typedef std::vector <CHAOS::InEventPort> InEventPort_Set;
  InEventPort_Set iep_set = component.InEventPort_kind_children ();

  for (auto iep : iep_set)
    iep.Accept (*this);

  // Determine if the component has any receptacles.
  typedef std::vector <CHAOS::RequiredRequestPort> Receptacle_Set;
  Receptacle_Set receptacles = component.RequiredRequestPort_kind_children ();

  for (auto receptacle : receptacles)
    receptacle.Accept (*this);

  // Determine if the component has any facets.
  typedef std::vector <CHAOS::ProvidedRequestPort> Facet_Set;
  Facet_Set facets = component.ProvidedRequestPort_kind_children ();

  for (auto facet : facets)
    facet.Accept (*this);

  // Determine the dependency for supported interfaces.
  typedef std::vector <CHAOS::Supports> Supports_Set;
  Supports_Set supports = component.Supports_children ();

  for (auto support : supports)
    support.Accept (*this);

  // Determine the dependency for attributes. We get the read-only
  // kind since it will return both read-only and read-write
  // attributes.
  typedef std::vector <CHAOS::ReadonlyAttribute> Readonly_Set;
  Readonly_Set attrs = component.ReadonlyAttribute_kind_children ();

  for (auto attr : attrs)
    attr.Accept (*this);

  // If this component is a <subtype> of another component, there is a
  // chance that it is located in another file. If this is the case
  // then we have to update its references.
  if (component.isSubtype ())
  {
    CHAOS::NamedType subtype =
      CHAOS::NamedType::Cast (component).Archetype ();

    this->Visit_NamedType (subtype);
  }
}

//
// Visit_OutEventPort
//
void CUTS_BE_IDL_Graph::
Visit_OutEventPort (const CHAOS::OutEventPort & port)
{
  CHAOS::Event event = port.ref ();

  if (event != Udm::null)
    event.Accept (*this);
}

//
// Visit_InEventPort
//
void CUTS_BE_IDL_Graph::
Visit_InEventPort (const CHAOS::InEventPort & port)
{
  CHAOS::Event event = port.ref ();

  if (event != Udm::null)
    event.Accept (*this);
}

//
// Visit_RequiredRequestPort
//
void CUTS_BE_IDL_Graph::
Visit_RequiredRequestPort (const CHAOS::RequiredRequestPort & port)
{
  CHAOS::Provideable provides = port.ref ();

  if (provides != Udm::null)
    this->Visit_Providable (provides);
}

//
// Visit_ProvidedRequestPort
//
void CUTS_BE_IDL_Graph::
Visit_ProvidedRequestPort (const CHAOS::ProvidedRequestPort & port)
{
  CHAOS::Provideable provides = port.ref ();

  if (provides != Udm::null)
    this->Visit_Providable (provides);
}

//
// Visit_Provideable
//
void CUTS_BE_IDL_Graph::
Visit_Providable (const CHAOS::Provideable & provides)
{
  this->Visit_NamedType (CHAOS::NamedType::Cast (provides));
}

//
// Visit_Object
//
void CUTS_BE_IDL_Graph::
Visit_Object (const CHAOS::Object & object)
{
  this->Visit_NamedType (CHAOS::NamedType::Cast (object));
}

//
// Visit_Event
//
void CUTS_BE_IDL_Graph::
Visit_Event (const CHAOS::Event & event)
{
  this->Visit_NamedType (CHAOS::NamedType::Cast (event));
}

//
// Visit_NamedType
//
void CUTS_BE_IDL_Graph::
Visit_NamedType (const CHAOS::NamedType & type)
{
  // Get the parent of the named type.
  CHAOS::File parent = this->NamedType_parent (type);

  // Determine the location of this NamedType. If this event is not
  // in the current file then, when we need to add it to the list of
  // depends.
  CUTS_BE_IDL_Node * node = 0;

  if (this->active_file_ != parent)
  {
    // Find the node with the name of the parent.
    this->find (parent.name (), node);

    if (node != 0)
      this->current_node_->references_.insert (node);

    // Ok, now we have to parse the <parent> before quiting.
    this->pending_files_.insert (parent);
  }
}

//
// Visit_Supports
//
void CUTS_BE_IDL_Graph::
Visit_Supports (const CHAOS::Supports & supports)
{
  CHAOS::Object object = CHAOS::Object::Cast (supports.ref ());

  if (object != Udm::null)
    object.Accept (*this);
}

//
// Visit_ReadonlyAttribute
//
void CUTS_BE_IDL_Graph::
Visit_ReadonlyAttribute (const CHAOS::ReadonlyAttribute & readonly)
{
  CHAOS::AttributeMember member = readonly.AttributeMember_child ();

  if (member != Udm::null)
  {
    // The model is valid since we have a child!!!!
    CHAOS::MemberType type = member.ref ();

    try
    {
      // Try and extact the named type so we can update our
      // dependencies.
      CHAOS::NamedType named_type = CHAOS::NamedType::Cast (type);
      this->Visit_NamedType (named_type);
    }
    catch (udm_exception)
    {
      /* bad cast: not a NamedType */
    }
  }
}

//
// NamedType_parent
//
CHAOS::File CUTS_BE_IDL_Graph::
NamedType_parent (const CHAOS::NamedType & type)
{
  CHAOS::MgaObject parent = type.parent ();

  while (parent.type () != CHAOS::File::meta)
    parent = CHAOS::MgaObject::Cast (parent.parent ());

  return CHAOS::File::Cast (parent);
}
