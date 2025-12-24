#pragma once
#include "../generalHeader.h"

void error_handling(ifstream & inputfile,ofstream &outputfile);
void saveXMLToFile(const string& fileName, const string& xmlContent);