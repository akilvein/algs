#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

inline int commonPrefixLen(const char *s1, const char *s2) {
    int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i]) {
        i++;
    }
    
    return i;
}

int countStringSuffixes(string s) {
    vector<const char *> suffixes(s.size());

    for (int i = 0; i < s.size(); i++) {
        suffixes[i] = s.data() + i;
    }

    sort(suffixes.begin(), suffixes.end(),
        [](const char *strA, const char *strB) { return std::strcmp(strA, strB) < 0; }
        );

    int counter = strlen(suffixes[0]);
    for (int i = 1; i < suffixes.size(); i++) {
        counter += (strlen(suffixes[i]) - commonPrefixLen(suffixes[i], suffixes[i - 1]));
    }
    
    return counter;
}
