#include "Minifying.h"
#include "../XMLParser.h"

static void minifyNode(XmlNode* node, ostream &out) {
    if (!node) return;
    if (node->name.empty()) {
        string v = node->value;
        int a = v.find_first_not_of(" \t\n\r");
        int b = v.find_last_not_of(" \t\n\r");
        if (a == string::npos) return;
        out << v.substr(a, b - a + 1);
        return;
    }
    out << "<" << node->name << ">";
    for (XmlNode* c : node->children) minifyNode(c, out);
    out << "</" << node->name << ">";
}

void minifyingFromTree(XmlNode* root, const string& outputFile) {
    if (!root) return;
    ofstream out(outputFile);
    if (!out.is_open()) { cerr << "Could not open output file: " << outputFile << endl; return; }
    minifyNode(root, out);
    out.close();
}

void minifying(const string& fileName, string output) {
    XmlNode* root = parseXMLFile(fileName);
    if (!root) { cerr << "Failed to parse input for minifying: " << fileName << endl; return; }
    minifyingFromTree(root, output);
    freeXmlTree(root);
    cout << "File '" << output << "' has been minified successfully." << endl;
}

