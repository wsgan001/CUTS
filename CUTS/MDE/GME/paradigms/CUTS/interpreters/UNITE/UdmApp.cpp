/*
Copyright (c) Vanderbilt University, 2000-2001
ALL RIGHTS RESERVED
Vanderbilt University disclaims all warranties with regard to this
software, including all implied warranties of merchantability
and fitness.  In no event shall Vanderbilt University be liable for
any special, indirect or consequential damages or any damages
whatsoever resulting from loss of use, data or profits, whether
in an action of contract, negligence or other tortious action,
arising out of or in connection with the use or performance of
this software.
*/


// UdmApp.cpp: implementation of the CUdmApp class.
// This file was automatically generated as UdmApp.cpp
// by UDM Interpreter Wizard on Monday, May 13, 2002 13:45:42

// Tiham�r Levendovszky 05-13-02

#include "stdafx.h"
#pragma warning( disable : 4290 )
#include <afxdlgs.h> // For CFileDialog
#include "resource.h"
#include <stdlib.h>
#include <sstream>

// Xerces includes
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

// Utility includes
#include "Utils/XercesString.h"

#include "UdmStatic.h"
#include "UmlExt.h"
#include "UdmUtil.h"

#include "UdmApp.h"
#include "UdmConfig.h"

#include "paradigm/CUTS.h"
#include "UniteVisitor.h"
#include "boost/bind.hpp"

using xercesc::XMLPlatformUtils;
using xercesc::XMLException;
using xercesc::DOMException;
using xercesc::XMLString;
using Utils::XStr;

extern void dummy(void); // Dummy function for UDM meta initialization

// static variable decl.
std::string CUdmApp::output_path_;

bool CUdmApp::interactive_ = true;

// Initialization function. The framework calls it before preparing the
// backend. Initialize here the settings in the config static object.
// Return 0 if successful.
int CUdmApp::Initialize()
{
  return 0;
}

/*
Remarks to CUdmApp::UdmMain(...):
0. The p_backend points to an already open backend, and the framework
closes it automatically. DO NOT OPEN OR CLOSE IT! To commit changes
use p_backend->CommitEditSequence(). To abort changes use
p_backend->AbortEditSequence(). To save changes to a different file
use p_backend->SaveAs() or p_backend->CloseAs().

1. Focus is the currently open model.

2. The possible values for param (from GME DecoratorLib.h
component_startmode_enum):
GME_MAIN_START    =   0,
GME_BROWSER_START    =   1,
GME_CONTEXT_START    =   2,
GME_EMBEDDED_START    =   3,
GME_MENU_START    =  16,
GME_BGCONTEXT_START  =  18,
GME_ICON_START    =  32,
METAMODEL_CHECK_SYNTAX  = 101

3. The framework catches all the exceptions and reports the error in a
message box, clean up and close the transactions aborting the changes.
You can override this behavior by catching udm_exception. Use
udm_exception::what() to form an error message.
*/

/***********************************************/
/* Main entry point for Udm-based Interpreter  */
/***********************************************/

void CUdmApp::UdmMain (Udm::DataNetwork* p_backend,        // Backend pointer
                       Udm::Object focusObject,            // Focus object
                       set <Udm::Object> selected,         // Selected objects
                       long param)                         // Parameters
{
  try
  {
    XMLPlatformUtils::Initialize();

    try
    {
      // We only need to ask for the output path it wasn't already
      // set before hand. The only want to can be set before hand
      // is by some external application invoking put_ComponentParameter
      // method for the interpreter. :o)

      int disable_optimize = 0;
  
      CUTS::UniteVisitor visitor (CUdmApp::output_path_);
      CUTS::RootFolder root = CUTS::RootFolder::Cast (p_backend->GetRootObject());
      root.Accept (visitor);
    }
    catch(udm_exception &e)
    {
      AfxMessageBox ("Caught UDM Exception: " + CString (e.what()));
      return;
    }
    catch (const DOMException& e)
    {
      const unsigned int maxChars = 2047;
      XMLCh errText[maxChars + 1];

      std::stringstream estream;
      estream << "DOMException code: " << e.code << std::endl;
      if (xercesc::DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
      {
        std::string message (XMLString::transcode (errText));
        estream << "Message is: " << message << std::endl;
      }
      AfxMessageBox (estream.str().c_str());
      return;
    }
    XMLPlatformUtils::Terminate();
  }
  catch (const XMLException& e)
  {
    std::string message (XMLString::transcode (e.getMessage()));
    AfxMessageBox (message.c_str());
    return;
  }

  if (CUdmApp::interactive_)
  {
    ::AfxMessageBox ("Descriptor files were successfully generated!",
                     MB_OK | MB_ICONINFORMATION);
  }

  return;
}

//
// SetParameter
//
void CUdmApp::
SetParameter (const std::string & name, const std::string & value)
{
  if (name == "output")
  {
    CUdmApp::output_path_ = value;
  }
  else if (name == "non-interactive")
  {
    CUdmApp::interactive_ = false;
  }
}

#ifdef _DEBUG

/*****************************************************/
/* Debug time helper function. If the object has an  */
/* attribute called "name", this function retreives  */
/* it to help you to find it in the model during the */
/* application development.  Usualy every GME Object   */
/* has a "name" attribute. If an object hapens not   */
/* to have it,function retreives <no name specified>.*/
/*****************************************************/

string CUdmApp::ExtractName(Udm::Object ob)
{
  Uml::Class cls= ob.type();
  set<Uml::Attribute> attrs=cls.attributes();

  // Adding parent attributes
  set<Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
  attrs.insert(aattrs.begin(),aattrs.end());

  for(set<Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++)
  {
    if(string(ai->type())=="String")
    {
      string str=ai->name();
      if(str=="name")
      {
        string value=ob.getStringAttr(*ai);
        if(value.empty())value="<empty string>";
        return value;
      }
    }
  }
  return string("<no name specified>");
}

#endif // _DEBUG