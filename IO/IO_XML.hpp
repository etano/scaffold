#ifndef SCAFFOLD_IO_INPUT
#define SCAFFOLD_IO_INPUT

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "rapidxml.hpp"

namespace scaffold { namespace IO {

template <class T>
inline T convertConstChar(const char * val) { return val; }
template <>
inline std::string convertConstChar(const char * val) { return std::string(val); }
template <>
inline double convertConstChar(const char * val) { return atof(val); }
template <>
inline float convertConstChar(const char * val) { return atof(val); }
template <>
inline int convertConstChar(const char * val) { return atoi(val); }
template <>
inline unsigned int convertConstChar(const char * val) { return atoi(val); }

struct Node
{
  std::string name;
  std::vector<Node> childNodes;
  std::map<std::string,std::string> attributes;
};

struct Input
{
  // Original buffer
  std::vector<char> buffer;

  // My node
  Node node;

  // Constructors
  Input() {};
  Input(Node &t_node, std::vector<char> &t_buffer)
    : node(t_node), buffer(t_buffer)
  {};

  // Loads settings structure from the specified XML file
  void load(const std::string &filename)
  {
    std::ifstream ifFilename(filename);
    std::vector<char> t_buffer((std::istreambuf_iterator<char>(ifFilename)), std::istreambuf_iterator<char>());
    buffer = t_buffer;
    buffer.push_back('\0');
    rapidxml::xml_document<> doc;
    doc.parse<0>(&buffer[0]);
    rapidxml::xml_node<> *rapidxml_node = doc.first_node("Input");
    loadXML(rapidxml_node, node);
  }

  void loadXML(rapidxml::xml_node<> *rapidxml_node, Node &my_node)
  {
    // Load name
    my_node.name = std::string(rapidxml_node->name());

    // Load attributes
    for (rapidxml::xml_attribute<> *attr = rapidxml_node->first_attribute(); attr; attr = attr->next_attribute())
      my_node.attributes[std::string(attr->name())] = std::string(attr->value());

    // Load child nodes
    for (rapidxml::xml_node<> *rapidxml_child=rapidxml_node->first_node(); rapidxml_child; rapidxml_child=rapidxml_child->next_sibling()) {
      Node childNode;
      loadXML(rapidxml_child, childNode);
      my_node.childNodes.push_back(childNode);
    }

  }

  void printXML(Node &my_node)
  {
    // Print name
    std::cout << "name: " << my_node.name << std::endl;

    // Print attributes
    for(auto attr: my_node.attributes)
      std::cout << " " << attr.first << " : " << attr.second << std::endl;

    // Print children
    for(auto& child: my_node.childNodes)
      printXML(child);
  }

  inline std::string getName() { return node.name; }

  inline std::string getString()
  {
    std::string str(buffer.begin(), buffer.end());
    return str;
  }

  template <class T>
  inline T getAttribute(std::string name)
  {
    if (node.attributes.find(name) == node.attributes.end()) {
      std::cerr << "ERROR: Attribute " << name << " not found in node " << getName() << "!" << std::endl;
      abort();
    } else
      return convertConstChar<T>(node.attributes[name].c_str());
  }

  template <class T>
  inline T getAttribute(std::string name, T deflt)
  {
    if (node.attributes.find(name) == node.attributes.end())
      return deflt;
    else
      return convertConstChar<T>(node.attributes[name].c_str());
  }

  inline Input getChild(std::string name)
  {
    for(auto& child: node.childNodes) {
      if (child.name == name) {
        Input in(child,buffer);
        return in;
      }
    }
    std::cerr << "ERROR: Child " << name << " not found in node " << getName() << "!" << std::endl;
    abort();
  }

  inline std::vector<Input> getChildList(std::string name) {
    std::vector<Input> ins;
    for(auto& child: node.childNodes) {
      if (child.name == name) {
        Input in(child,buffer);
        ins.push_back(in);
      }
    }
    return ins;
  }

  inline std::vector<Input> getChildList() {
    std::vector<Input> ins;
    for(auto& child: node.childNodes) {
      Input in(child,buffer);
      ins.push_back(in);
    }
    return ins;
  }

};

}}

#endif
