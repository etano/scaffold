#ifndef INPUTFILE_H
#define INPUTFILE_H

#include <string>
#include <iostream>
#include "../types.h"
#include "xmlParser.h"

template <class T>
inline T convertConstChar(const char * val) { return val; }
template <>
inline std::string convertConstChar(const char * val) { return std::string(val); }
template <>
inline RealType convertConstChar(const char * val) { return atof(val); }
template <>
inline int convertConstChar(const char * val) { return atoi(val); }

class Input
{
public:
  void load(const std::string &filename);
  XMLNode xNode;

  inline std::string getName() { return std::string(xNode.getName()); }

  inline Input getChild(std::string name)
  {
    Input in;
    in.xNode = xNode.getChildNode(name.c_str());
    return in;
  }

  inline std::string getString()
  {
    std::string xmlString = std::string(xNode.createXMLString(true));
    return xmlString;
  }

  inline std::string getText(std::string name)
  {
    std::string str = std::string(getChild(name).getText());
    return str;
  }

  inline std::string getText()
  {
    std::string str = std::string(xNode.getText());
    return str;
  }

  template <class T>
  inline T getAttribute(std::string name)
  {
    const char* i = xNode.getAttribute(name.c_str());
    if (i == NULL) {
      std::cerr << "ERROR: " << name << " not found in input!" << std::endl;
      abort();
    } else
      return convertConstChar<T>(i);
  }

  template <class T>
  inline T getAttribute(std::string name, T deflt)
  {
    const char* i = xNode.getAttribute(name.c_str());
    if (i == NULL)
      return deflt;
    else
      return convertConstChar<T>(i);
  }

  inline std::vector<Input> getChildList(std::string name) {
    std::vector<Input> ins;
    int n = xNode.nChildNode(name.c_str());
    for (int i=0; i<n; i++) {
      Input in;
      in.xNode = xNode.getChildNode(name.c_str(), i);
      ins.push_back(in);
    }
    return ins;
  }

  inline std::vector<Input> getChildList() {
    std::vector<Input> ins;
    int n = xNode.nChildNode();
    for (int i=0; i<n; i++) {
      Input in;
      in.xNode = xNode.getChildNode(i);
      ins.push_back(in);
    }
    return ins;
  }


};

#endif
