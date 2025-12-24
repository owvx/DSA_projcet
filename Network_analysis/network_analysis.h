#pragma once
#include "../generalHeader.h"
#include "../graphRepresentation/XMLToGraph.h"


vector<pair<string,int>> MostInfluencer(map<int,user> users);

vector<pair<string,int>> MutualUsers(map<int,user>users,vector<string>v);

vector<pair<string,int>> MostActive(map<int,user>users);

vector<pair<string,int>> SuggestFollowers(map<int,user>users,string idd);

