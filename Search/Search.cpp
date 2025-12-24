#include "Search.h"
#include "../XMLParser.h"

// Helper to get child by name
static XmlNode* findChild(XmlNode* n, const string& name) {
    if (!n) return nullptr;
    for (XmlNode* c : n->children) if (c && c->name == name) return c;
    return nullptr;
}

vector<pair<int,string>> SearchByWordFromTree(XmlNode* root, const string& word) {
    vector<pair<int,string>> found;
    if (!root) return found;
    for (XmlNode* uNode : root->children) {
        if (!uNode || uNode->name != "user") continue;
        XmlNode* idNode = findChild(uNode, "id");
        int id = idNode && !idNode->value.empty() ? stoi(idNode->value) : 0;
        XmlNode* posts = findChild(uNode, "posts");
        if (!posts) continue;
        for (XmlNode* postNode : posts->children) {
            XmlNode* body = findChild(postNode, "body");
            if (!body) continue;
            string bodyText = body->value;
            string lc = bodyText;
            transform(lc.begin(), lc.end(), lc.begin(), ::tolower);
            string w = word;
            transform(w.begin(), w.end(), w.begin(), ::tolower);
            int pos = lc.find(w);
            if (pos != string::npos) {
                string out = bodyText;
                out.insert(pos, "{");
                out.insert(pos + w.size() + 1, "}");
                found.push_back({id, out});
            }
        }
    }
    return found;
}

vector<pair<int,string>> SearchByTopicFromTree(XmlNode* root, const string& topic) {
    vector<pair<int,string>> found;
    if (!root) return found;
    for (XmlNode* uNode : root->children) {
        if (!uNode || uNode->name != "user") continue;
        XmlNode* idNode = findChild(uNode, "id");
        int id = idNode && !idNode->value.empty() ? stoi(idNode->value) : 0;
        XmlNode* posts = findChild(uNode, "posts");
        if (!posts) continue;
        for (XmlNode* postNode : posts->children) {
            XmlNode* body = findChild(postNode, "body");
            XmlNode* topics = findChild(postNode, "topics");
            if (!body || !topics) continue;
            string bodyText = body->value;
            for (XmlNode* tnode : topics->children) {
                if (!tnode) continue;
                string t = tnode->value;
                string lc = t;
                transform(lc.begin(), lc.end(), lc.begin(), ::tolower);
                string tgt = topic;
                transform(tgt.begin(), tgt.end(), tgt.begin(), ::tolower);
                int pos = lc.find(tgt);
                if (pos != string::npos) {
                    string out = bodyText + "\n" + t;
                    found.push_back({id, out});
                }
            }
        }
    }
    return found;

}

// keep existing file-based helpers for compatibility (they still exist)
vector<pair<int,string>> SearchByWordFromFile( string& filename,  string& word) {
    XmlNode* root = parseXMLFile(filename);
    if (!root) return {};
    auto res = SearchByWordFromTree(root, word);
    freeXmlTree(root);
    return res;
}

vector<pair<int,string>> SearchByTopicFromFile(const string& filename, const string& topic) {
    XmlNode* root = parseXMLFile(filename);
    if (!root) return {};
    auto res = SearchByTopicFromTree(root, topic);
    freeXmlTree(root);
    return res;
}

