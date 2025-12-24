#pragma once
#include "../generalHeader.h"
#include "../XMLTree.h"

using namespace std;

vector<pair<int,string>> SearchByWordFromTree(XmlNode* root, const string& word);
vector<pair<int,string>> SearchByTopicFromTree(XmlNode* root, const string& topic);

vector<pair<int,string>> SearchByWord(vector<string>input_xml,string word);

vector<pair<int,string>> SearchByTopic(vector<string>input_xml,string topic);

vector<pair<int,string>> SearchByWordFromFile(const string& filename, const string& word);
vector<pair<int,string>> SearchByTopicFromFile(const string& filename, const string& topic);

