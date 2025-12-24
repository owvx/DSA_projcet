#include "compression/compress.h"
#include "Format/format.h"
#include "XmlTojson/XmlTojson.h"
#include "mini/Mini.h"
#include "network_analysis/network_analysis.h"
#include "Search/Search.h"
#include "errorHandling/ErrorHandling.h"
#include "XmlToGraph/XmlToGraph.h"
#include "XMLParser.h"
#include "generalHeader.h"


// Simple helper to get argument value following a flag
static string getArgValue(int argc, char* argv[], const string& flag) {
	for (int i = 1; i < argc - 1; ++i) {
		if (flag == argv[i]) return string(argv[i+1]);
	}
	return string();
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << "Usage: xml_editor <command> [options]\n";
		return 1;
	}

	string cmd = argv[1];

	if (cmd == "compress") {
		string in = getArgValue(argc, argv, "-i");
		string out = getArgValue(argc, argv, "-o");
		if (in.empty() || out.empty()) { cout << "compress requires -i and -o\n"; return 1; }
		compress(in, out);
	}
	else if (cmd == "decompress") {
		string in = getArgValue(argc, argv, "-i");
		string out = getArgValue(argc, argv, "-o");
		if (in.empty() || out.empty()) { cout << "decompress requires -i and -o\n"; return 1; }
		decompress(in, out);
	}
	else if (cmd == "format") {
		string in = getArgValue(argc, argv, "-i");
		string out = getArgValue(argc, argv, "-o");
		if (in.empty() || out.empty()) { cout << "format requires -i and -o\n"; return 1; }
		XmlNode* root = parseXMLFile(in);
		if (!root) { cerr << "Cannot parse input file\n"; return 1; }
		ofstream output(out);
		if (!output.is_open()) { cerr << "Cannot open output file\n"; freeXmlTree(root); return 1; }
		FormatXMLFromFile(in, output);
		freeXmlTree(root);
	}
	else if (cmd == "mini") {
		string in = getArgValue(argc, argv, "-i");
		string out = getArgValue(argc, argv, "-o");
		if (in.empty() || out.empty()) { cout << "mini requires -i and -o\n"; return 1; }
		minifying(in, out);
	}
	else if (cmd == "json") {
		string in = getArgValue(argc, argv, "-i");
		string out = getArgValue(argc, argv, "-o");
		if (in.empty() || out.empty()) { cout << "json requires -i and -o\n"; return 1; }

		XmlNode* root = parseXMLFile(in);
		if (!root) { cerr << "Cannot parse input file\n"; return 1; }
		ofstream output(out);
		if (!output.is_open()) { cerr << "Cannot open output file\n"; freeXmlTree(root); return 1; }
		convertXMLtoJSONFromTree(root, output);
		freeXmlTree(root);
		cout << "XML file converted successfully\n";
	}
	else if (cmd == "verify") {
		string in = getArgValue(argc, argv, "-i");
		string out = getArgValue(argc, argv, "-o");

		if (in.empty()) { cout << "verify requires -i <input_file>\n"; return 1; }
		ifstream input(in);
		if (!input.is_open()) { cerr << "Cannot open input file\n"; return 1; }

		// If no output provided, use a sensible default
		if (out.empty()) {
			out = "output_file_without_errors.xml";
			cout << "No -o provided, writing results to " << out << "\n";
		}

		ofstream output(out);
		if (!output.is_open()) { cerr << "Cannot open output file\n"; input.close(); return 1; }

		error_handling(input, output);
		input.close();
		output.close();

	}
	else if (cmd == "draw"){
		string in = getArgValue(argc, argv, "-i");
		string out = getArgValue(argc, argv, "-o");
		if (in.empty() || out.empty()) { cout << "draw requires -i and -o\n"; return 1; }
		XmlNode* root = parseXMLFile(in);
		if (!root) { cerr << "Cannot parse input file\n"; return 1; }
		map<int,user> users = xmlToGraphFromTree(root);
		dotFileInput(users, "dotFile");
		visualizeGraph("dotFile", out);
		freeXmlTree(root);
	}
	else if (cmd == "suggest") {
		string in = getArgValue(argc, argv, "-i");
		string id = getArgValue(argc, argv, "-id");
		if (in.empty() || id.empty()) { cout << "suggest requires -i and -id\n"; return 1; }
		map<int,user> users = xmlToGraph(in);
		vector<pair<string, int>> suggestedFollowers = SuggestFollowers(users, id);
		if (suggestedFollowers.empty()) cout << "No Followers To Suggest!\n";
		else {
			cout << "Name id\n";
			for (auto &i : suggestedFollowers) cout << i.first << " " << i.second << "\n";
		}
	}
	else if (cmd == "most_influencer") {
		string in = getArgValue(argc, argv, "-i");
		if (in.empty()) { cout << "most_influencer requires -i\n"; return 1; }
		map<int,user> users = xmlToGraph(in);
		vector<pair<string, int>> mostInfluencer = MostInfluencer(users);
		cout << "Name id\n";
		for (auto &i : mostInfluencer) cout << i.first << " " << i.second << "\n";
	}
	else if (cmd == "most_active") {
		string in = getArgValue(argc, argv, "-i");
		if (in.empty()) { cout << "most_active requires -i\n"; return 1; }
		map<int,user> users = xmlToGraph(in);
		vector<pair<string, int>> Active_Users = MostActive(users);
		cout << "Name id\n";
		for (auto &i : Active_Users) cout << i.first << " " << i.second << "\n";
	}
	else if (cmd == "mutual") {
		string in = getArgValue(argc, argv, "-i");
		string ids = getArgValue(argc, argv, "-ids");
		if (in.empty() || ids.empty()) { cout << "mutual requires -i and -ids\n"; return 1; }
		map<int,user> users = xmlToGraph(in);
		vector<string> idList;
		stringstream ss(ids);
		string token;
		while (getline(ss, token, ',')) idList.push_back(token);
		vector<pair<string, int>> Mutual_Users = MutualUsers(users, idList);
		if (Mutual_Users.empty()) cout << "No Mutual Found!\n";
		else { cout << "Name id\n"; for (auto &i : Mutual_Users) cout << i.first << " " << i.second << "\n"; }
	}
	else if (cmd == "search") {
		string in = getArgValue(argc, argv, "-i");
		if (in.empty()) { cout << "search requires -i and either -w <word> or -t <topic>\n"; return 1; }
		if (argc >= 3) {
			// find -w or -t
			bool handled = false;
			for (int i = 2; i < argc; ++i) {
				string a = argv[i];
				if (a == "-w" && i+1 < argc) {
					XmlNode* root = parseXMLFile(in);
					auto res = SearchByWordFromTree(root, string(argv[i+1]));
					if (res.empty()) cout << "No Words Found!\n";
					else { cout << "ID: Post where the word is mentioned:\n"; for (auto &p: res) cout << p.first << " " << p.second << "\n"; }
					freeXmlTree(root);
					handled = true; break;
				}
				else if (a == "-t" && i+1 < argc) {
					XmlNode* root = parseXMLFile(in);
					auto res = SearchByTopicFromTree(root, string(argv[i+1]));
					if (res.empty()) cout << "No Topics Found!\n";
					else { cout << "ID: Post where the topic is mentioned:\n"; for (auto &p: res) cout << p.first << " " << p.second << "\n"; }
					freeXmlTree(root);
					handled = true; break;
				}
			}
			if (!handled) { cout << "search requires -w <word> or -t <topic>\n"; return 1; }
		}
	}
	else {
		cout << "Invalid command\n";
		return 1;
	}

	return 0;
}





//xml_editor json -i input_file.xml -o output_file_json.json

//xml_editor mini -i input_file.xml -o output_file_minified.xml

//xml_editor compress -i input_file.xml -o output_file_compressed.comp

//xml_editor decompress -i output_file_compressed.comp -o output_file_decompressed.xml

//xml_editor format -i inputPrettify.xml -o output_file_prettified.xml

//xml_editor verify -i inputErrorHandling.xml -f -o output_file_without_errors.xml



//xml_editor draw -i inputNetworkAnalysis.xml -o output_file_graphviz.jpg

//xml_editor suggest -i inputNetworkAnalysis.xml -id 1

//xml_editor most_influencer -i inputNetworkAnalysis.xml

//xml_editor mutual -i inputNetworkAnalysis.xml -ids 1,2

//xml_editor mutual -i inputNetworkAnalysis.xml -ids 1,2,3

//xml_editor most_active -i inputNetworkAnalysis.xml

//xml_editor search -w word -i inputNetworkAnalysis.xml

//xml_editor search -t topic -i inputNetworkAnalysis.xml

//xml_editor search -w lorem -i inputNetworkAnalysis.xml

//xml_editor search -t economy -i inputNetworkAnalysis.xmls

