#include "XMLTojson.h"
#include "../XMLParser.h"
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

static bool isTextNode(XmlNode* n) { return n && n->name.empty(); }

// Recursive converter: convert XmlNode tree to pretty JSON
static void nodeToJson(XmlNode* node, ofstream &out, int indentLevel=0) {
    if (!node) return;
    if (isTextNode(node)) { out << '"' << node->value << '"'; return; }

    map<string, vector<XmlNode*>> groups;
    for (XmlNode* c : node->children) groups[c->name].push_back(c);

    out << "{";
    bool first = true;
    for (auto &kv : groups) {
        const string &tag = kv.first;
        auto &vec = kv.second;
        if (!first) out << ",";
        out << '\n';
        for (int i = 0; i < indentLevel+1; ++i) out << "    ";
        out << '"' << (tag.empty() ? string("_text") : tag) << "\": ";
        if (vec.size() > 1) {
            out << "[";
            for (size_t i = 0; i < vec.size(); ++i) {
                if (i) out << ", ";
                nodeToJson(vec[i], out, indentLevel+2);
            }
            out << "]";
        } else {
            nodeToJson(vec[0], out, indentLevel+1);
        }
        first = false;
    }
    out << '\n';
    for (int i = 0; i < indentLevel; ++i) out << "    ";
    out << "}";
}

void convertXMLtoJSONFromTree(XmlNode* root, ofstream &outputFile) {
    if (!root) return;
    if (!root->name.empty()) {
        outputFile << '{' << '"' << root->name << "\": ";
        nodeToJson(root, outputFile, 0);
        outputFile << '}';
    } else {
        nodeToJson(root, outputFile, 0);
    }
    outputFile << endl;
}

void convertXMLtoJSON(ifstream &file, ofstream &outputFile ){
    string tmp = "prettified_for_json.tmp";
    ofstream tmpOut(tmp);
    string line;
    while (getline(file, line)) tmpOut << line << '\n';
    tmpOut.close();
    XmlNode* root = parseXMLFile(tmp);
    if (!root) return;
    convertXMLtoJSONFromTree(root, outputFile);
    freeXmlTree(root);
}


