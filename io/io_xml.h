#ifndef SCAFFOLD_IO_IO_XML_H_
#define SCAFFOLD_IO_IO_XML_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "rapidxml.h"

namespace scaffold { namespace io {

template <class T>
inline T ConvertConstChar(const char * val) { return val; }
template <>
inline std::string ConvertConstChar(const char * val) { return std::string(val); }
template <>
inline double ConvertConstChar(const char * val) { return atof(val); }
template <>
inline float ConvertConstChar(const char * val) { return atof(val); }
template <>
inline int ConvertConstChar(const char * val) { return atoi(val); }
template <>
inline unsigned int ConvertConstChar(const char * val) { return atoi(val); }
template <>
inline bool ConvertConstChar(const char * val) { return atoi(val); }

struct Node
{
  std::string name;
  std::vector<Node> child_nodes;
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
  void Load(const std::string &filename)
  {
    std::ifstream if_filename(filename);
    std::vector<char> t_buffer((std::istreambuf_iterator<char>(if_filename)), std::istreambuf_iterator<char>());
    buffer = t_buffer;
    buffer.push_back('\0');
    rapidxml::xml_document<> doc;
    doc.parse<0>(&buffer[0]);
    rapidxml::xml_node<> *rapidxml_node = doc.first_node("Input");
    LoadXML(rapidxml_node, node);
  }

  void LoadXML(rapidxml::xml_node<> *rapidxml_node, Node &my_node)
  {
    // Load name
    my_node.name = std::string(rapidxml_node->name());

    // Load attributes
    for (rapidxml::xml_attribute<> *attr = rapidxml_node->first_attribute(); attr; attr = attr->next_attribute())
      my_node.attributes[std::string(attr->name())] = std::string(attr->value());

    // Load child nodes
    for (rapidxml::xml_node<> *rapidxml_child=rapidxml_node->first_node(); rapidxml_child; rapidxml_child=rapidxml_child->next_sibling()) {
      Node childNode;
      LoadXML(rapidxml_child, childNode);
      my_node.child_nodes.push_back(childNode);
    }

  }

  void PrintXML(Node &my_node)
  {
    // Print name
    std::cout << "name: " << my_node.name << std::endl;

    // Print attributes
    for(auto attr: my_node.attributes)
      std::cout << " " << attr.first << " : " << attr.second << std::endl;

    // Print children
    for(auto& child: my_node.child_nodes)
      PrintXML(child);
  }

  inline std::string GetName() { return node.name; }

  inline std::string GetString()
  {
    std::string str(buffer.begin(), buffer.end());
    return str;
  }

  template <class T>
  inline T GetAttribute(std::string name)
  {
    if (node.attributes.find(name) == node.attributes.end()) {
      std::cerr << "ERROR: Attribute " << name << " not found in node " << GetName() << "!" << std::endl;
      abort();
    } else
      return ConvertConstChar<T>(node.attributes[name].c_str());
  }

  template <class T>
  inline T GetAttribute(std::string name, T deflt)
  {
    if (node.attributes.find(name) == node.attributes.end())
      return deflt;
    else
      return ConvertConstChar<T>(node.attributes[name].c_str());
  }

  inline Input GetChild(std::string name)
  {
    for(auto& child: node.child_nodes) {
      if (child.name == name) {
        Input in(child,buffer);
        return in;
      }
    }
    // Could not find child
    Input null_in;
    null_in.node.name = "null";
    return null_in;
  }

  inline std::vector<Input> GetChildList(std::string name) {
    std::vector<Input> ins;
    for(auto& child: node.child_nodes) {
      if (child.name == name) {
        Input in(child,buffer);
        ins.push_back(in);
      }
    }
    return ins;
  }

  inline std::vector<Input> GetChildList() {
    std::vector<Input> ins;
    for(auto& child: node.child_nodes) {
      Input in(child,buffer);
      ins.push_back(in);
    }
    return ins;
  }

};

}}

#endif // SCAFFOLD_IO_IO_XML_H_
