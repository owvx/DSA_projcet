#include "network_analysis.h"

vector<pair<string,int>> MostInfluencer(map<int,user> users) {
    int mx = 0;
    vector<pair<string,int>> result;
    for ( auto &p : users) {
         user &u = p.second;
        if (u.followers.size() > mx) mx = u.followers.size();
    }
    for ( auto &p : users) {
         user &u = p.second;
        if (u.followers.size() == mx) result.push_back({u.name, u.id});
    }
    return result;
}

vector<pair<string,int>> MutualUsers(map<int,user> users,vector<string>v) {
    vector<pair<string,int>> Mutual_Users;
    map<int,int> mp; // count how many of the provided ids follow a given user

    for ( auto &p : users) {
         user &u = p.second;
        if (find(v.begin(), v.end(), to_string(u.id)) != v.end()) {
            for (int fid : u.followers) mp[fid]++;
        }
    }

    for ( auto &entry : mp) {
        if (entry.second == v.size()) {
            auto it = users.find(entry.first);
            if (it != users.end()) Mutual_Users.push_back({it->second.name, it->second.id});
        }
    }
    return Mutual_Users;
}

vector<pair<string,int>> MostActive(map<int,user> users) {
    map<int,int> mp;
    vector<pair<string,int>> result;

    for ( auto &p : users) {
        int uid = p.first;
         user &u = p.second;
        for (int fid : u.followers) {
            mp[uid]++;
            mp[fid]++;
        }
    }

    int mx = 0;
    for ( auto &e : mp) if (e.second > mx) mx = e.second;
    for ( auto &e : mp) {
        if (e.second == mx) {
            auto it = users.find(e.first);
            if (it != users.end()) result.push_back({it->second.name, it->second.id});
        }
    }
    return result;
}

vector<pair<string,int>> SuggestFollowers(map<int,user> users ,string idd) {
    vector<pair<string,int>> suggested_users;
    map<int,int> mp;
    int id = 0;
    try { id = stoi(idd); } catch (...) { return suggested_users; }

    auto itUser = users.find(id);
    if (itUser == users.end()) return suggested_users;

    // mark existing followers
    for (int fid : itUser->second.followers) mp[fid] = 1;

    // avoid duplicates in suggestions
    set<int> added;

    for (int follower_id : itUser->second.followers) {
        auto itFollower = users.find(follower_id);
        if (itFollower == users.end()) continue;
        for (int j : itFollower->second.followers) {
            if (j == id) continue;
            if (mp.find(j) != mp.end()) continue; // already a follower or marked
            if (added.find(j) != added.end()) continue;
            auto itCand = users.find(j);
            if (itCand == users.end()) continue;
            suggested_users.push_back({itCand->second.name, j});
            added.insert(j);
        }
    }
    return suggested_users;
}


