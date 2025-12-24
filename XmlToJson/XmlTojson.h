#pragma once
#include "../generalHeader.h"
#include "../XMLTree.h"

void indent(int indentation, ofstream &outputFile);
void closeTag(string &jsonString,stack<char>&st);
void arrayTag(const string& tagName,string &jsonString,stack<char>&st);
void sentenceArrayTag(const string& tagName, const string& sentence, string &jsonString, stack<char>&st) ;
void sentenceTag(const string& tagName, const string& sentence,string &jsonString);
void normalTag(const string& tagName,string &jsonString,stack<char>&st);
void justSentence(const string& sentence, string &jsonString);

void convertXMLtoJSONFromTree(XmlNode* root, ofstream &outputFile);
void convertXMLtoJSON(ifstream &file, ofstream &outputFile );

