#pragma once
#include "../generalHeader.h"

vector<unsigned short int> compress_helper(vector<char> uncompressed_file);

vector<char> decompress_helper(vector<unsigned short> compressed_file);

vector<char> parse_xml(const string& file_name);

vector<unsigned short> parse_comp(const string& file_name);

void save_compressed_file(const vector<unsigned short>& compressed_data, const string& filename);

void save_decompressed_xml(vector<char> decmpressed_file, const std::string& filename);

void compress(const string& input_name, const string& output_name);

void decompress(const string& input_name, const string& output_name);
