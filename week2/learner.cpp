#include <algorithm>
#include <string>
#include <set>
#include <vector>
#include <sstream>
#include <iostream>
#include "profile.h"


using namespace std;

class Learner {
private:
    set<string> dict;

public:
    int Learn(const vector<string>& words) {
        LOG_DURATION("learn");
        int newWords = 0;
        pair<set<string>::iterator,bool> ret;
        for (const auto& word : words) {
            ret = dict.insert(word);
            if (ret.second == true) {
                ++newWords;
            }
        }
        return newWords;
    }

    vector<string> KnownWords() {
        LOG_DURATION("know");
        vector<string> res(dict.begin(), dict.end());
        return res;
    }
};


int main() {
    LOG_DURATION("main");
    Learner learner;
    string line;
    while (getline(cin, line)) {
        vector<string> words;
        stringstream ss(line);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }
        cout << learner.Learn(words) << "\n";
    }
    for (auto word : learner.KnownWords()) {
        cout << word << "\n";
    }
    return 0;
}

