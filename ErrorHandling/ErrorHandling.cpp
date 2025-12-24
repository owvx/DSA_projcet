#include "ErrorHandling.h"

// Write XML content to a file (keeps the same external behavior)
void saveXMLToFile(const string& fileName, const string& xmlContent) {
    ofstream ofs(fileName);
    if (ofs) {
        ofs << xmlContent;
        ofs.close();
        cout << "XML content saved to " << fileName << endl;
    } else {
        cout << "Error: Unable to open file " << fileName << endl;
    }
}

// Simple error handler: reads the entire input, attempts to repair missing
// opening/closing tags using a stack-based heuristic, and writes the
// repaired XML to outputfile. API and call signature unchanged.
void error_handling(ifstream & inputfile, ofstream & outputfile) {
    if (!inputfile.is_open()) {
        cerr << "Error: Could not open file " << endl;
        return;
    }

    // Read whole file into a single string while preserving line breaks
    string content;
    string ln;
    while (getline(inputfile, ln)) {
        content += ln;
        content.push_back('\n');
    }
    inputfile.close();

    const char openChar = '<';
    const char closeChar = '>';

    vector<string> openStack;        // opening tags as strings, e.g. "<tag>"
    vector<int>    openLineNums;     // corresponding line numbers
    int errorsFound = 0;

    size_t cursor = 0;
    int lineNo = 1;

    auto extract_tag = [&](size_t s, size_t e)->string { return content.substr(s, e - s + 1); };
    auto tag_name = [&](const string &t)->string {
        if (t.size() <= 2) return string();
        size_t i = (t.size() > 1 && t[1] == '/') ? 2 : 1;
        string nm;
        for (; i < t.size(); ++i) {
            char c = t[i];
            if (c == '>' || isspace((unsigned char)c) || c == '/') break;
            nm.push_back(c);
        }
        return nm;
    };

    auto getLineAt = [&](size_t pos)->int { return 1 + (int)count(content.begin(), content.begin() + min(pos, content.size()), '\n'); };

    while (cursor < content.size()) {
        if (content[cursor] == '\n') ++lineNo;
        // find next '<'
        size_t s = content.find(openChar, cursor);
        if (s == string::npos) break;
        // find next '>' after s
        size_t e = content.find(closeChar, s);
        if (e == string::npos) {
            // malformed tail; stop processing
            cerr << "MALFORMED XML FILE: missing closing '>'" << endl;
            break;
        }

        string tag = extract_tag(s, e);
        bool isClosing = (tag.size() >= 2 && tag[1] == '/');

        if (!isClosing) {
            // opening tag
            openStack.push_back(tag);
            openLineNums.push_back(lineNo);
        } else {
            // closing tag
            string closingName = tag_name(tag);
            if (!openStack.empty()) {
                string topName = tag_name(openStack.back());
                if (topName == closingName) {
                    // matched normally
                    openStack.pop_back();
                    openLineNums.pop_back();
                } else {
                    // search for matching opening in the stack
                    int found = -1;
                    for (int i = (int)openStack.size() - 1; i >= 0; --i) {
                        if (tag_name(openStack[i]) == closingName) { found = i; break; }
                    }
                    if (found != -1) {
                        // we have an earlier opening; insert closings for intervening opens
                        for (int k = (int)openStack.size() - 1; k > found; --k) {
                            string toClose = "</" + tag_name(openStack[k]) + ">";
                            // report missing closing for this opened tag
                            int openedLine = (k < (int)openLineNums.size()) ? openLineNums[k] : getLineAt(s);
                            cout << "Missing closing tag for <" << tag_name(openStack[k]) << "> opened at line " << openedLine << endl;
                            // insert closing before current closing tag position
                            content.insert(s, toClose);
                            ++errorsFound;
                        }
                        // erase intervening opens
                        openStack.erase(openStack.begin() + found + 1, openStack.end());
                        openLineNums.erase(openLineNums.begin() + found + 1, openLineNums.end());
                        // now consume the matching opening
                        if (!openStack.empty()) { openStack.pop_back(); openLineNums.pop_back(); }
                    } else {
                        // no matching opening: insert an opening before this closing
                        string openingTag = "<" + closingName + ">";
                        // find insertion point: last '>' before s
                        size_t insertPos = content.rfind(closeChar, s);
                        if (insertPos == string::npos) insertPos = 0; else insertPos = insertPos + 1;
                        content.insert(insertPos, openingTag);
                        int closingLine = getLineAt(s);
                        cout << "Missing opening tag for </" << closingName << "> at line " << closingLine << endl;
                        ++errorsFound;
                    }
                }
            } else {
                // stack empty: insert opening before this closing
                string openingTag = "<" + closingName + ">";
                size_t insertPos = content.rfind(closeChar, s);
                if (insertPos == string::npos) insertPos = 0; else insertPos = insertPos + 1;
                content.insert(insertPos, openingTag);
                int closingLine = getLineAt(s);
                cout << "Missing opening tag for </" << closingName << "> at line " << closingLine << endl;
                ++errorsFound;
            }
        }

        cursor = e + 1;
    }

    // For any remaining opening tags, try to insert closings after their occurrence
    while (!openStack.empty()) {
        string openingTag = openStack.back();
        string expectedClose = "</" + tag_name(openingTag) + ">";
        // find the opening occurrence
        size_t pos = content.find(openingTag);
        if (pos != string::npos) {
            size_t after = content.find(closeChar, pos);
            if (after != string::npos) {
                size_t nextTagPos = content.find(openChar, after + 1);
                if (nextTagPos == string::npos) {
                    // append at end
                    content.append(expectedClose);
                } else {
                    string nextTag = content.substr(nextTagPos, content.find(closeChar, nextTagPos) - nextTagPos + 1);
                    if (nextTag != expectedClose) {
                        content.insert(after + 1, expectedClose);
                    }
                }
            } else {
                content.append(expectedClose);
            }
        } else {
            // not found; append at end
            content.append(expectedClose);
        }
        ++errorsFound;
        openStack.pop_back();
        openLineNums.pop_back();
    }

    cout << "There are " << errorsFound << " errors." << endl;

    // write repaired content
    outputfile << content;
    outputfile.close();
}