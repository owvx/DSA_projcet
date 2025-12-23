#include "compress.h"

class Dictionary {
private:
	vector<string> seqs;
	map<string, unsigned short> codes;
public:
	Dictionary() {
		seqs.reserve(4096);
		for (int i = 0; i < 256; ++i) {
			string s(1, static_cast<char>(i));
			seqs.push_back(s);
			codes.emplace(s, static_cast<unsigned short>(i));
		}
	}
	int get_size() const { return seqs.size(); }
	string get_seq(unsigned short code) const {
		if (code < seqs.size()) return seqs[code];
		return string();
	}
	bool exists(const string& s) const { return codes.find(s) != codes.end(); }
	unsigned short get_code(const string& s) const {
		auto it = codes.find(s);
		if (it != codes.end()) return it->second;
		return static_cast<unsigned short>(-1);
	}
	void add_to_dict(const string& s) {
		unsigned short key = static_cast<unsigned short>(seqs.size());
		seqs.push_back(s);
		codes.emplace(s, key);
	}
};

vector<unsigned short int> compress_helper(vector<char> uncompressed_file) {
	Dictionary dict;
	vector<unsigned short int> compressed;
	string s;

	for (int i = 0; i < uncompressed_file.size(); ++i) {
		char c = uncompressed_file[i];
		string sc = s + c;
		if (dict.exists(sc)) {
			s = sc;
		} else {
			if (!s.empty()) {
				compressed.push_back(dict.get_code(s));
			} else {
				string single(1, c);
				compressed.push_back(dict.get_code(single));
				continue;
			}
			dict.add_to_dict(sc);
			s = string(1, c);
		}
	}
	if (!s.empty()) compressed.push_back(dict.get_code(s));
	return compressed;
}

vector<char> decompress_helper(vector<unsigned short> compressed_file) {
	Dictionary dict;
	vector<char> decompressed;
	string prev;

	for (int i = 0; i < compressed_file.size(); ++i) {
		unsigned short k = compressed_file[i];
		if (k >= dict.get_size()) {
			if (k > dict.get_size()) {
				cerr << "corrupted file\n";
				return {};
			}
			// k == dict.get_size() handled below
		}

		string entry;
		if (k == dict.get_size()) {
			// special LZW case
			entry = prev + prev[0];
		} else {
			entry = dict.get_seq(k);
		}

		decompressed.insert(decompressed.end(), entry.begin(), entry.end());

		if (!prev.empty()) {
			string newEntry = prev + entry[0];
			dict.add_to_dict(newEntry);
		}
		prev = entry;
	}
	return decompressed;
}

vector<char> parse_xml(const string& file_name) {
	ifstream file(file_name, ios::binary);
	if (!file) {
		cerr << "error: cannot open file " << file_name << endl;
		return {};
	}
	vector<char> content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	file.close();

	if (content.size() >= 3 &&
		static_cast<unsigned char>(content[0]) == 0xEF &&
		static_cast<unsigned char>(content[1]) == 0xBB &&
		static_cast<unsigned char>(content[2]) == 0xBF) {
		content.erase(content.begin(), content.begin() + 3);
	}
	return content;
}

vector<unsigned short> parse_comp(const string& file_name) {
	ifstream in(file_name, ios::binary);
	if (!in) {
		cerr << "error: cannot open file " << file_name << endl;
		return {};
	}
	vector<unsigned short> data;
	unsigned short value;
	while (in.read(reinterpret_cast<char*>(&value), sizeof(value))) data.push_back(value);
	in.close();
	return data;
}

void save_compressed_file(const vector<unsigned short>& compressed_data, const string& filename) {
	ofstream out(filename, ios::binary);
	if (!out) {
		cerr << "error: unable to open file for writing!" << endl;
		return;
	}
	out.write(reinterpret_cast<const char*>(compressed_data.data()), static_cast<streamsize>(compressed_data.size() * sizeof(unsigned short)));
	out.close();
}

void save_decompressed_xml(vector<char> decmpressed_file, const std::string& filename) {
	ofstream outfile(filename, ios::binary);
	if (!outfile.is_open()) {
		cerr << "failed to open the file for writing: " << filename << endl;
		return;
	}
	if (!decmpressed_file.empty())
		outfile.write(decmpressed_file.data(), static_cast<streamsize>(decmpressed_file.size()));
	outfile.close();
}

void compress(const string& input_name, const string& output_name) {
	vector<char> filecontent = parse_xml(input_name);
	if (filecontent.empty()) {
		cerr << "nothing to compress or failed to read input\n";
		return;
	}
	vector<unsigned short int> filecompressed = compress_helper(filecontent);
	save_compressed_file(filecompressed, output_name);
	cout << "compressed successfully\n";
}

void decompress(const string& input_name, const string& output_name) {
	vector<unsigned short> data = parse_comp(input_name);
	if (data.empty()) {
		cerr << "nothing to decompress or failed to read input\n";
		return;
	}
	vector<char> decompressedfile = decompress_helper(data);
	save_decompressed_xml(decompressedfile, output_name);
	cout << "decompressed successfully\n";
}

