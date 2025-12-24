#include "format.h"

//helper functions
void debug_tree(XmlNode* node, int level = 0) {
    if (!node) return;
    // Indentation for hierarchy
    for (int i = 0; i < level; ++i) {
        cout << "  ";
    }
    // Print node details
    cout << "Node: " << node->name;
    if (!node->value.empty()) {
        cout << ", Value: " << node->value;
    }
    cout << endl;
    // Recurse for children
    for (XmlNode* child : node->children) {
        debug_tree(child, level + 1);
    }
}

string trim( std::string& str) {
    auto start = str.begin();
    while (start != str.end() && isspace(*start)) {
        ++start;
    }
    auto end = str.end();
    do {--end;
    } while (distance(start, end) > 0 && isspace(*end));
    return string(start, end+1);
}


void print_node(XmlNode* node, ostringstream& out, int level =0) {
    if (!node) return; // Base case: If the node is null, return immediately
    // If this node has no name it's a text/value node — print value only
    if (node->name.empty()) {
        for (int i = 0; i < level; ++i) out << "   ";
        if (!node->value.empty()) out << node->value << endl;
        return;
    }
    // Indent the output based on the current tree depth (level)
    for (int i = 0; i < level; ++i) {
        out << "   "; // Add spaces for pretty-printing
    }
    // Print the opening tag of the current XML node
    out << "<" << node->name << ">";
    // If the node has a value, print it directly after the opening tag
    if (!node->value.empty()) {
        out << node->value;
    }
    // If the node has children, handle them recursively
    if (!node->children.empty()) {
        out << endl; // Add a newline after the opening tag for better readability
        // Recursively print each child node, increasing the depth level
        for (XmlNode* child : node->children) {
            print_node(child, out, level + 1);
        }
        // Indent the closing tag to align with the opening tag
        for (int i = 0; i < level; ++i) {
            out << "   ";
        }
    }
    // Print the closing tag of the current XML node
    out << "</" << node->name << ">" << endl;
}


void FormatXML(vector<string> input_xml, ofstream &outputFile) {
    XmlNode* rootNode;
    int idx=0;
    rootNode = parse_node(input_xml,idx);
    ostringstream xmlOutput;
    print_node(rootNode, xmlOutput);

    if (!outputFile.is_open()) {
        cerr << "Failed to open output file for writing!" << endl;
        //return 1;
    }
    outputFile << xmlOutput.str();
    outputFile.close();

    cout << "XML file successfully saved to output.xml" << endl;
}

void FormatXMLFromFile(const string& inputFilename, ofstream &outputFile) {
    XmlNode* root = parseXMLFile(inputFilename);
    if (!root) {
        cerr << "Failed to parse XML file: " << inputFilename << endl;
        return;
    }
    ostringstream xmlOutput;
    print_node(root, xmlOutput);
    if (!outputFile.is_open()) {
        cerr << "Failed to open output file for writing!" << endl;
        return;
    }
    outputFile << xmlOutput.str();
    outputFile.close();
    cout<< "XML file successfully formatted and saved to output." << endl;
    freeXmlTree(root);
}
