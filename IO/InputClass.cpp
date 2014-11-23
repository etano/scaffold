#include "InputClass.h"

// Loads settings structure from the specified XML file
void Input::load(const std::string &filename)
{
  xNode = XMLNode::openFileHelper(filename.c_str(), "Input");
}

