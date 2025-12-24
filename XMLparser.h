#pragma once
#include "XMLTree.h"

// Tokenize file and parse into XmlNode tree
vector<string> ReadXml(ifstream& in);
XmlNode* parse_node(const vector<string>& s, int& idx);
XmlNode* parseXMLFile(const string& filename);

// Utility to free a tree allocated with raw pointers
void freeXmlTree(XmlNode* node);
