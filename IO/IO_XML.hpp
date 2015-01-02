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

struct Input
{
  std::vector<char> buffer;
  rapidxml::xml_node<> * node;

  Input() {};
  Input(std::vector<char> t_buffer, rapidxml::xml_node<> *t_node)
    : buffer(t_buffer), node(t_node)
  {}

  // Loads settings structure from the specified XML file
  void load(const std::string &filename)
  {
    std::ifstream ifFilename(filename);
    std::vector<char> t_buffer((std::istreambuf_iterator<char>(ifFilename)), std::istreambuf_iterator<char>());
    buffer = t_buffer;
    buffer.push_back('\0');
    rapidxml::xml_document<> doc;
    doc.parse<0>(&buffer[0]);
    node = doc.first_node("Input");
  }

  inline std::string getName() { return std::string(node->name()); }

  inline std::string getString()
  {
    std::string str(buffer.begin(), buffer.end());
    return str;
  }

  template <class T>
  inline T getAttribute(std::string name)
  {
    const char* i = node->first_attribute(name.c_str())->value();
    if (i == NULL) {
      std::cerr << "ERROR: " << name << " not found in input!" << std::endl;
      abort();
    } else
      return convertConstChar<T>(i);
  }

  template <class T>
  inline T getAttribute(std::string name, T deflt)
  {
    const char* i = node->first_attribute(name.c_str())->value();
    if (i == NULL)
      return deflt;
    else
      return convertConstChar<T>(i);
  }

  inline Input getChild(std::string name)
  {
    Input in(buffer,node->first_node(name.c_str()));
    return in;
  }

  inline std::vector<Input> getChildList(std::string name) {
    std::vector<Input> ins;
    for (rapidxml::xml_node<> *child=node->first_node(name.c_str()); child; child=child->next_sibling()) {
      Input in(buffer,child);
      ins.push_back(in);
    }
    return ins;
  }

  inline std::vector<Input> getChildList() {
    std::vector<Input> ins;
    for (rapidxml::xml_node<> *child=node->first_node(); child; child=child->next_sibling()) {
      Input in(buffer,child);
      ins.push_back(in);
    }
    return ins;
  }

};

}}

#endif
