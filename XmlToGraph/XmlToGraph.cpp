#include "XMLtograph.h"
#include "../XMLParser.h"

user::user() {
    id = 0;
    name = "";
}

static XmlNode* findChild(XmlNode* n, const string& name) {
    if (!n) return nullptr;
    for (XmlNode* c : n->children) if (c && c->name == name) return c;
    return nullptr;
}

static string getNodeText(XmlNode* n) {
    if (!n) return string();
    if (!n->value.empty()) return n->value;
    string out;
    for (XmlNode* c : n->children) {
        if (c) {
            if (!c->value.empty()) out += c->value;
            else {
                string inner = getNodeText(c);
                if (!inner.empty()) out += inner;
            }
        }
    }
    // trim whitespace
    size_t start = out.find_first_not_of(" \t\n\r");
    size_t end = out.find_last_not_of(" \t\n\r");
    if (start == string::npos) return string();
    return out.substr(start, end - start + 1);
}

// Build users map from parsed XmlNode tree
map<int,user> xmlToGraphFromTree(XmlNode* root) {
 map<int,user> users;
    if (!root) return users;

    

    // Expect root->name == "users" or root contains users as children
    for (XmlNode* uNode : root->children) {
        if (!uNode || uNode->name != "user") continue;
        user usr;
        XmlNode* idNode = findChild(uNode, "id");
        string idText = getNodeText(idNode);
        if (!idText.empty()) usr.id = stoi(idText);
        XmlNode* nameNode = findChild(uNode, "name");
        string nameText = getNodeText(nameNode);
        if (!nameText.empty()) usr.name = nameText;

        XmlNode* postsNode = findChild(uNode, "posts");
        if (postsNode) {
            for (XmlNode* postNode : postsNode->children) {
                if (!postNode || postNode->name != "post") continue;
                post p;
                XmlNode* bodyNode = findChild(postNode, "body");
                string bodyText = getNodeText(bodyNode);
                if (!bodyText.empty()) p.body = bodyText;
                XmlNode* topicsNode = findChild(postNode, "topics");
                if (topicsNode) {
                    for (XmlNode* topicNode : topicsNode->children) {
                        if (!topicNode || topicNode->name != "topic") continue;
                        p.topics.push_back(getNodeText(topicNode));
                    }
                }
                usr.posts.push_back(p);
            }
        }

        XmlNode* followersNode = findChild(uNode, "followers");
        if (followersNode) {
            
            for (XmlNode* fNode : followersNode->children) {
                if (!fNode) { continue; }
                if (fNode->name != "follower") { continue; }
                XmlNode* fid = findChild(fNode, "id");
                string fidText = getNodeText(fid);
                if (!fidText.empty()) usr.followers.push_back(stoi(fidText));
            }
        }

        if (usr.id != 0) {
            users[usr.id] = usr;
            cerr << "[xmlToGraphFromTree] added user id=" << usr.id << " name='" << usr.name << "' followers=" << usr.followers.size() << "\n";
        } else {
            cerr << "[xmlToGraphFromTree] skipping user with no id\n";
        }
    }

    return users;
}

map<int,user> xmlToGraph(const string& inputName) {
    XmlNode* root = parseXMLFile(inputName);
    if (!root) return {};
    map<int,user> users = xmlToGraphFromTree(root);
    freeXmlTree(root);
    return users;
}

void dotFileInput(map<int,user> users, const string& outputName) {
    ofstream outputFile(outputName);
    outputFile << "digraph test{" << endl;
    for (auto &u : users) {
        outputFile << "\"" << u.second.name << "\"" << endl;
        for (int followerId : u.second.followers) {
            auto it = users.find(followerId);
            if (it == users.end()) continue;
            outputFile << "\"" << u.second.name << "\" -> \"" << it->second.name << "\"" << endl;
        }
    }
    outputFile << "}";
    outputFile.close();
}

void visualizeGraph(const string& dotFile, const string& outputFile) {
    string command = "dot -Tjpg " + dotFile + " -o " + outputFile;
    if (system(command.c_str()) == 0) {
        cout << "Graph visualized: " << outputFile << endl;
    } else {
        cerr << "Error visualizing graph." << endl;
    }
}
