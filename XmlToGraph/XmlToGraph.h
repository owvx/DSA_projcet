#pragma once
#include "../generalHeader.h"
#include "../XMLTree.h"
// Representation of users and posts for graph representation

class post {
public:
    string body;
    vector<string>topics;
};
class user {
public:
    int id;
    string name;
    vector<post>posts;
    vector<int>followers;
    user();
};

// Original API (file-based) preserved. Implementation now uses the parsed tree.
map<int,user> xmlToGraph(const string& inputName); //convert xml file to adjacency list

// New API: build graph from an already-parsed XmlNode tree
map<int,user> xmlToGraphFromTree(XmlNode* root);

void dotFileInput(map<int,user> users, const string& outputName);
void visualizeGraph(const string& dotFile, const string& outputFile);
