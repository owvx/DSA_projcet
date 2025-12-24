#include "XMLParser.h"
#include <cctype>

string trim_local(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && isspace(*start)) ++start;
    auto end = str.end();
    do { --end; } while (distance(start, end) > 0 && isspace(*end));
    return string(start, end+1);
}

vector<string> ReadXml(ifstream& in) {
    vector<string> input_xml;
    string line, xmlContent = "";
    while (getline(in, line)) {
        xmlContent += line;
    }
    in.close();

    string cur = "";
    for (int i = 0; i < xmlContent.size();) {
        if (xmlContent[i] == '<') {
            string t = trim_local(cur);
            if (!t.empty()) input_xml.push_back(t);
            cur.clear();

            int j = i;
            while (j < xmlContent.size() && xmlContent[j] != '>') j++;
            if (j < xmlContent.size()) {
                string tag = xmlContent.substr(i, j - i + 1);
                input_xml.push_back(tag);
                i = j + 1;
            } else {
                break;
            }
        } else {
            cur.push_back(xmlContent[i]);
            ++i;
        }
    }
    if (!cur.empty()) {
        string t = trim_local(cur);
        if (!t.empty()) input_xml.push_back(t);
    }
    return input_xml;
}

XmlNode* parse_node(const vector<string>& s, int& idx) {
    if (idx >= s.size()) return nullptr;
    if (!s[idx].empty() && s[idx][0] != '<') {
        XmlNode* textNode = new XmlNode();
        textNode->name = "";
        textNode->value = trim_local(s[idx]);
        idx++;
        return textNode;
    }
    if (s[idx].size() >= 2 && s[idx][0] == '<' && s[idx][1] == '/') {
        idx++;
        return nullptr;
    }
    if (s[idx].size() >= 2 && s[idx][0] == '<') {
        string tag = s[idx];
        bool selfClosing = false;
        if (tag.size() >= 2 && tag[tag.size()-2] == '/') selfClosing = true;

        string name = "";
        for (int i = 1; i < tag.size(); ++i) {
            char c = tag[i];
            if (c == '>' || isspace(c) || c == '/') break;
            name.push_back(c);
        }

        XmlNode* xmlNode = new XmlNode();
        xmlNode->name = name;

        idx++; // consume opening tag

        if (selfClosing) return xmlNode;

        while (idx < s.size()) {
            if (s[idx].size() >= 3 && s[idx][0] == '<' && s[idx][1] == '/') {
                string closeName = "";
                for (int k = 2; k < s[idx].size(); ++k) {
                    if (s[idx][k] == '>') break;
                    closeName.push_back(s[idx][k]);
                }
                if (closeName == xmlNode->name) {
                    idx++; // consume closing tag
                    break;
                } else {
                    idx++;
                    continue;
                }
            }
            XmlNode* child = parse_node(s, idx);
            if (child) xmlNode->children.push_back(child);
        }
        return xmlNode;
    }
    return nullptr;
}

XmlNode* parseXMLFile(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Failed to open input file: " << filename << endl;
        return nullptr;
    }
    vector<string> tokens = ReadXml(in);
    
    int idx = 0;
    XmlNode* root = parse_node(tokens, idx);
    return root;
}

void freeXmlTree(XmlNode* node) {
    if (!node) return;
    for (XmlNode* c : node->children) freeXmlTree(c);
    delete node;
}
