// $Id$

#include "BE_IDL_Graph_Builder.h"
#include "BE_IDL_Graph.h"

#include "boost/bind.hpp"

#include <algorithm>

struct visit_all
{
public:
template <typename T>
void operator () (T & collection, PICML::Visitor * visitor) const
{
  for (auto item : collection)
    item->accept (visitor);
}
};

//
// CUTS_BE_IDL_Graph_Builder
//
CUTS_BE_IDL_Graph_Builder::
CUTS_BE_IDL_Graph_Builder (CUTS_BE_IDL_Graph & graph)
: graph_ (graph)
{

}

//
// ~CUTS_BE_IDL_Graph_Builder
//
CUTS_BE_IDL_Graph_Builder::~CUTS_BE_IDL_Graph_Builder (void)
{

}

//
// visit_RootFolder
//
void CUTS_BE_IDL_Graph_Builder::
visit_RootFolder (PICML::RootFolder_in root)
{
  visit_all () (root->get_InterfaceDefinitions (), this);
}

//
// visit_InterfaceDefinitions
//
void CUTS_BE_IDL_Graph_Builder::
visit_InterfaceDefinitions (PICML::InterfaceDefinitions_in folder)
{
  visit_all () (folder->get_Files (), this);
}

//
// visit_File
//
void CUTS_BE_IDL_Graph_Builder::
visit_File (PICML::File_in file)
{
  // Find this node in the graph then visit the <file>.
  if (this->graph_.find (file->name (), this->current_node_) == -1)
    return;

  // We are trying to keep from parsing this file more than once.
  // So, if we are have already visited this file, then we do
  // not need to do it again!! Just a waste of time.
  if (!this->current_node_->flags_[CUTS_BE_IDL_Node::IDL_VISITED])
  {
    // Set the visit flag. See above for reason.
    this->current_node_->flags_[CUTS_BE_IDL_Node::IDL_VISITED] = true;
    this->current_node_->file_ = file;

    visit_all () (file->get_FileRefs (), this);

    // Save the active file and visit its contents.
    this->active_file_ = file;
    visit_all () (file->get_Events (), this);
    visit_all () (file->get_Objects (), this);
    visit_all () (file->get_Aggregates (), this);
    visit_all () (file->get_Components (), this);
    visit_all () (file->get_Packages (), this);

    while (!this->pending_files_.empty ())
    {
      // Get the first file in the collection.
      File_Set::iterator iter = this->pending_files_.begin ();
      PICML::File file = *iter;

      // Remove the file from the collection and visit it.
      this->pending_files_.erase (iter);
      file->accept (this);
    }
  }
}

//
// visit_FileRef
//
void CUTS_BE_IDL_Graph_Builder::
visit_FileRef (PICML::FileRef_in f)
{
  // Find the node with the name of the parent.
  PICML::File file = f->refers_to_File ();

  CUTS_BE_IDL_Node * node = 0;
  this->graph_.find (file->name (), node);

  if (node != 0)
  {
    // Update this new's references.
    node->file_ = file;
    this->current_node_->references_.insert (node);

    // We still need to parse this file.
    this->pending_files_.insert (file);
  }
}

//
// visit_Package
//
void CUTS_BE_IDL_Graph_Builder::
visit_Package (PICML::Package_in package)
{
    visit_all () (package->get_Events (), this);
    visit_all () (package->get_Objects (), this);
    visit_all () (package->get_Aggregates (), this);
    visit_all () (package->get_Components (), this);
    visit_all () (package->get_Packages (), this);
}

//
// visit_Component
//
void CUTS_BE_IDL_Graph_Builder::
visit_Component (PICML::Component_in component)
{
  // Set the flag for this node.
  this->current_node_->has_components_ = true;

  // Determine if the component has output events.
  visit_all () (component->get_OutEventPorts (), this);

  // Determine if the component has input events.
  visit_all () (component->get_InEventPorts (), this);

  // Determine if the component has any receptacles.
  visit_all () (component->get_RequiredRequestPorts (), this);

  // Determine if the component has any facets.
  visit_all () (component->get_ProvidedRequestPorts (), this);

  // Determine the dependency for supported interfaces.
  visit_all () (component->get_Supportss (), this);

  // Determine the dependency for attributes. We get the read-only
  // kind since it will return both read-only and read-write
  // attributes.
  visit_all () (component->get_ReadonlyAttributes (), this);

  // If this component is a <subtype> of another component, there is a
  // chance that it is located in another file. If this is the case
  // then we have to update its references.
  if (component->is_subtype ())
    this->visit_NamedType (PICML::NamedType::_narrow (component->basetype ()));
}

//
// visit_OutEventPort
//
void CUTS_BE_IDL_Graph_Builder::
visit_OutEventPort (PICML::OutEventPort_in port)
{
  PICML::EventType et = port->refers_to_EventType ();

  if (et->meta ()->name () == PICML::Event::impl_type::metaname)
    et->accept (this);
}

//
// visit_InEventPort
//
void CUTS_BE_IDL_Graph_Builder::
visit_InEventPort (PICML::InEventPort_in port)
{
  PICML::EventType et = port->refers_to_EventType ();

  if (et->meta ()->name () == PICML::Event::impl_type::metaname)
    et->accept (this);
}

//
// visit_RequiredRequestPort
//
void CUTS_BE_IDL_Graph_Builder::
visit_RequiredRequestPort (PICML::RequiredRequestPort_in port)
{
  PICML::Provideable provides = port->refers_to_Provideable ();
  this->visit_Providable (provides);
}

//
// visit_ProvidedRequestPort
//
void CUTS_BE_IDL_Graph_Builder::
visit_ProvidedRequestPort (PICML::ProvidedRequestPort_in port)
{
  PICML::Provideable provides = port->refers_to_Provideable ();
  this->visit_Providable (provides);
}

//
// visit_Provideable
//
void CUTS_BE_IDL_Graph_Builder::
visit_Providable (PICML::Provideable_in provides)
{
  this->visit_NamedType (PICML::NamedType::_narrow (provides));
}

//
// visit_Object
//
void CUTS_BE_IDL_Graph_Builder::
visit_Object (PICML::Object_in object)
{
  this->current_node_->has_interfaces_ = true;
  this->visit_NamedType (object);
}

//
// visit_Event
//
void CUTS_BE_IDL_Graph_Builder::
visit_Event (PICML::Event_in evt)
{
  if (this->NamedType_parent (evt) == this->active_file_)
    this->current_node_->has_events_ = true;

  this->visit_NamedType (evt);

  visit_all () (evt->get_Members (), this);
}

//
// visit_Aggregate
//
void CUTS_BE_IDL_Graph_Builder::
visit_Aggregate (PICML::Aggregate_in a)
{
  // Visit the members in this aggregate.
  visit_all () (a->get_Members (), this);

  if (a->has_Key ())
  {
    PICML::Key key = a->get_Key ();

    if (this->NamedType_parent (a) == this->active_file_)
      this->current_node_->has_dds_events_ = true;
  }
}

//
// visit_NamedType
//
void CUTS_BE_IDL_Graph_Builder::
visit_NamedType (PICML::NamedType_in type)
{
  // Get the parent of the named type.
  PICML::File parent = this->NamedType_parent (type);

  // Determine the location of this named type. If this event is not
  // in the current file then, when we need to add it to the list of
  // depends.
  CUTS_BE_IDL_Node * node = 0;

  if (this->active_file_ != parent)
  {
    // Find the node with the name of the parent.
    this->graph_.find (parent->name (), node);

    if (node != 0)
      this->current_node_->references_.insert (node);

    // Ok, now we have to parse the <parent> before quiting.
    this->pending_files_.insert (parent);
  }
}

//
// visit_Supports
//
void CUTS_BE_IDL_Graph_Builder::
visit_Supports (PICML::Supports_in supports)
{
  supports->refers_to_Object ()->accept (this);
}

//
// visit_ReadonlyAttribute
//
void CUTS_BE_IDL_Graph_Builder::
visit_ReadonlyAttribute (PICML::ReadonlyAttribute_in readonly)
{
  if (!readonly->has_AttributeMember ())
    return;
  
  PICML::AttributeMember member = readonly->get_AttributeMember ();

  // The model is valid since we have a child!!!!
  PICML::MemberType type = member->refers_to_MemberType ();

  try
  {
    // Try and extact the named type so we can update our
    // dependencies.
    PICML::NamedType named_type = PICML::NamedType::_narrow (type);
    this->visit_NamedType (named_type);
  }
  catch (GAME::Mga::Exception &)
  {
    /* bad cast: not a NamedType */
  }
}

//
// NamedType_parent
//
PICML::File CUTS_BE_IDL_Graph_Builder::
NamedType_parent (const PICML::NamedType_in type)
{
  GAME::Mga::Object parent = type->parent ();

  while (parent->meta ()->name () != PICML::File::impl_type::metaname)
    parent = parent->parent ();

  return parent;
}

//
// visit_Member
//
void CUTS_BE_IDL_Graph_Builder::visit_Member (PICML::Member_in m)
{
  PICML::MemberType mt = m->refers_to_MemberType ();

  try
  {
    this->visit_NamedType (PICML::NamedType::_narrow (mt));
  }
  catch (GAME::Mga::Exception &)
  {
    return;
  }
}
