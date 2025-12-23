#pragma once
#include "../generalHeader.h"
#include "../XMLTree.h"

void minifying(const string& fileName , string output);

// Tree-based API
void minifyingFromTree(XmlNode* root, const string& outputFile);

