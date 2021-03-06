// $Id$

#include "CAPI_Event_Impl_Generator.h"
#include "CAPI_Ctx.h"
#include <sstream>

//
// CUTS_BE_CAPI_Event_Impl_Generator
//
CUTS_BE_CAPI_Event_Impl_Generator::
CUTS_BE_CAPI_Event_Impl_Generator (const std::string & outdir)
: outdir_ (outdir)
{

}

//
// ~CUTS_BE_CAPI_Event_Impl_Generator
//
CUTS_BE_CAPI_Event_Impl_Generator::~CUTS_BE_CAPI_Event_Impl_Generator (void)
{

}

//
// Visit_Event
//
void CUTS_BE_CAPI_Event_Impl_Generator::
Visit_Event (const PICML::Event & ev)
{
  std::string fq_name = CUTS_BE_Java::fq_type (ev, "/", false);

  if (fq_name == "cuts/jbi/client/JbiAnyEvent")
    return;

  // Save the name of the class.
  this->type_ = CUTS_BE_Java::classname (ev.SpecifyIdTag ());
  this->impl_ = this->type_ + "Impl";

  // Construct the filename for the implementation.
  std::ostringstream filename;
  filename << this->outdir_ << "/" << fq_name
           << "/" << this->impl_ << ".java";

  this->outfile_.open (filename.str ().c_str ());

  if (this->outfile_.is_open ())
  {
    this->formatter_.reset (new _formatter_type (this->outfile_));

    // Write the preamble for the file.
    this->outfile_ << "package " << CUTS_BE_Java::fq_type (ev, ".", false) << ";";
    this->write_includes ();

    // Begin the class definition.
    this->outfile_ << "public class " << this->impl_ << std::endl
                   << "  extends JbiEvent < " << this->type_ << " > {";

    // Write the constructors for the file.
    this->write_constructors ();

    this->write_getter_methods ();

    // Write the static part of the event.
    this->write_static (ev);

    // End the class definition.
    this->outfile_ << "}";

    // Close the output file.
    this->formatter_.reset (0);
    this->outfile_.close ();
  }
}

//
// write_includes
//
void CUTS_BE_CAPI_Event_Impl_Generator::write_includes (void)
{
  static const char * str = "import cuts.jbi.client.JbiEvent;"
                            "import org.infospherics.jbi.client.exception.*;"
                            "import org.infospherics.jbi.client.InfoObject;"
                            "import org.exolab.castor.mapping.*;"
                            "import org.exolab.castor.mapping.xml.*;"
                            "import org.exolab.castor.xml.Marshaller;"
                            "import org.exolab.castor.xml.Unmarshaller;"
                            "import org.exolab.castor.xml.MarshalException;"
                            "import org.exolab.castor.xml.ValidationException;"
                            "import java.io.StringWriter;"
                            "import java.io.StringReader;"
                            "import java.io.IOException;";

  this->outfile_ << str << std::endl;
}

//
// write_constructors
//
void CUTS_BE_CAPI_Event_Impl_Generator::write_constructors (void)
{
  this->outfile_ << "public " << this->impl_ << " () {"
                 << "}"
                 << std::endl
                 << "public " << this->impl_ << " (" << this->type_
                 << " metadata) {"
                 << "super (metadata);"
                 << "}"
                 << std::endl
                 << "public " << this->impl_ << " (" << this->type_
                 << " metadata, byte [] payload) {"
                 << "super (metadata, payload);"
                 << "}"
                 << std::endl
                 << "public " << this->impl_ << " (InfoObject mio) {"
                 << "super (mio);"
                 << "}";
}

//
// write_static
//
void CUTS_BE_CAPI_Event_Impl_Generator::
write_static (const PICML::Event & event)
{
  this->outfile_ << std::endl
                 << "/// Mapping file for this event type." << std::endl
                 << "private static Mapping mapping_ = new Mapping ();" << std::endl
                 << "/// XML -> Java unmarshaller for this event type." << std::endl
                 << "private static Unmarshaller unmarshaller_ = new Unmarshaller ();" << std::endl
                 << "/// Java -> XML marshaller for this event type." << std::endl
                 << "private static Marshaller marshaller_ = new Marshaller ();" << std::endl
                 << "static {"
                 << "try {"
                 << "// Get the class for the type." << std::endl
                 << "Class thisClass = " << CUTS_BE_Java::classname (event.SpecifyIdTag ())
                 << ".class;" << std::endl
                 << "// Construct the name of the mapping file. This is necessary" << std::endl
                 << "// since Castor likes to construct *bad* tags." << std::endl
                 << "String packageName = thisClass.getPackage ().getName ();"
                 << "String mappingFile = packageName.replace ('.', '/') + \"/mapping.xml\";" << std::endl
                 << "// Load the mapping file for the type." << std::endl
                 << this->impl_ << ".mapping_.loadMapping (thisClass.getClassLoader ().getResource (mappingFile));"
                 << std::endl
                 << "// Set the mappings for the marshaller/unmarshaller." << std::endl
                 << this->impl_ << ".marshaller_.setMapping (" << this->impl_ << ".mapping_);"
                 << this->impl_ << ".marshaller_.setValidation (false);"
                 << std::endl
                 << this->impl_ << ".unmarshaller_.setMapping (" << this->impl_ << ".mapping_);"
                 << this->impl_ << ".marshaller_.setValidation (false);"
                 << "}"
                 << "catch (Exception ex) {"
                 << "ex.printStackTrace ();"
                 << "}"
                 << "}";
}

//
// write_getter_methods
//
void CUTS_BE_CAPI_Event_Impl_Generator::write_getter_methods (void)
{
  this->outfile_ << std::endl
                 << "public " << this->type_ << " getMetadata ()" << std::endl
                 << "  throws ObjectUnavailableException, TimeoutException," << std::endl
                 << "         InstantiationException, IllegalAccessException," << std::endl
                 << "         MarshalException, ValidationException {"
                 << "// Optimization for the fast path." << std::endl
                 << "if (this.metadata_ != null)" << std::endl
                 << "  return this.metadata_;"
                 << std::endl
                 << "// Create a metadata object if it does not exist." << std::endl
                 << "this.metadata_ = new " << this->type_ << " ();"
                 << std::endl
                 << "StringReader reader;" << std::endl
                 << "if (this.infoObject_ != null)" << std::endl
                 << "  reader = new StringReader (this.infoObject_.getMetadata ());"
                 << "else" << std::endl
                 << "  reader = new StringReader (this.metadataString_);"
                 << std::endl
                 << "this.metadata_ = (" << this->type_ << ") this.unmarshaller_.unmarshal (reader);"
                 << "return this.metadata_;"
                 << "}"
                 << std::endl
                 << "public String getMetadataString ()" << std::endl
                 << "  throws ObjectUnavailableException, TimeoutException," << std::endl
                 << "         MarshalException, ValidationException, IOException {"
                 << "if (this.infoObject_ != null) {"
                 << "// First, check if an MIO object exists." << std::endl
                 << "return this.infoObject_.getMetadata ();"
                 << "}"
                 << "else if (this.metadata_ != null) {"
                 << "// Next, fallback to the object model." << std::endl
                 << "StringWriter writer = new StringWriter ();" << std::endl
                 << "this.marshaller_.setWriter (writer);"
                 << "this.marshaller_.marshal (this.metadata_);"
                 << std::endl
                 << "return writer.toString ();"
                 << "}"
                 << "else {"
                 << "// Otherwise, just return the XML string." << std::endl
                 << "return this.metadataString_;"
                 << "}"
                 << "}";
}
