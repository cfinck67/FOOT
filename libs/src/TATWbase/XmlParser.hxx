#ifndef XMLREADER_H
#define XMLREADER_H
#include "TXMLEngine.h"
#include "TString.h"


/* naming
<element attribute=1>
content
</element>
*/
class XmlParser
{
  /**
  * Object to handle xml parsing engine
  */
  TXMLEngine *_XMLEngine;
  /**
  * Pointer to the actual xmlfile
  */
  XMLDocPointer_t _xmldoc;
public:
  /**
  * Create the TXMLEngine
  */
  XmlParser();
  /**
  * Desctructor takes care of destroying the _XMLEngine and _xmldoc
  */
  ~XmlParser();
  /*
  * Load xml file
  */
  void ReadFile(std::string FileName);
  /**
   * Show the content of the xml file
   */
  void PrintXmlContent();
  /*
  * Get all the first generation child  of a node (StartingNode) whose name is (NodeName)
  */
  std::vector<XMLNodePointer_t> GetChildNodesByName(
                                XMLNodePointer_t StartingNode,
                                std::string NodeName);
  /*
  * Return the content of a node as Int_t
  */
  Int_t GetContentAsInt(std::string Name,XMLNodePointer_t Node);
  /*
  * Return the content of a node as std::string
  */
  std::string GetContentAsString(std::string Name,XMLNodePointer_t Node);
  /**
  * Return the content of a node as a double
  */
  Double_t GetContentAsDouble(std::string Name,XMLNodePointer_t Node);
  /**
  * Return the pointer to the document main node
  */
  XMLNodePointer_t GetMainNode();
  /**
   * export to file
   */
  XMLNodePointer_t AddElement(std::string Name, XMLNodePointer_t ParentNode);
  XMLNodePointer_t CreateMainNode(std::string Name);
  void AddElementWithContent(std::string Name, XMLNodePointer_t ParentNode, std::string Value);
  void ExportToFile(std::string Filename,XMLDocPointer_t mainnode);

};
#endif
