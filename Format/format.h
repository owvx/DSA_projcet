#pragma once
#include "../generalHeader.h"
#include "../XMLTree.h"

#include "../XMLParser.h"

//helper functions
void debug_tree(XmlNode* node, int level);

string trim(const std::string& str);

void print_node(XmlNode* node, ostringstream& out, int level);


void FormatXML(vector<string> input_xml, ofstream &outputFile);

// New convenience helpers
void FormatXMLFromFile(const string& inputFilename, ofstream &outputFile);
