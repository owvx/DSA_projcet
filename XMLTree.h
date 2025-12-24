#pragma once
#include "generalHeader.h"

struct XmlNode {
    string name;
    string value;
    vector<XmlNode*> children;
    map<string,string> attrs; // attribute name -> value
    XmlNode() : name(""), value("") {}
};

// XML node structure shared across modules. Parsing functions live in
// `pretty/PrettifyXml.cpp` for now and can be moved to a parser module.
