#include <iostream>
#include <string>
#include <vector>

std::vector<int> createZArray(std::string s) {
        // Create Z-array of corresponding
        // size and initialize it with zeros.
        int n = s.size();
        std::vector<int> z(n);
        // Create two pointers marking start and end of most recently
        // found substring identical to a prefix of the whole string.
        int start = 0;
        int end = 0;
        // Iterate through Z-array entry by entry from left to right starting
        // at index 1. Each entry already calculated contains the length of a
        // substring starting at the same index which is identical to a prefix
        // of the whole string. Accordingly, the processing starts at index 1
        // because the entry at index 0 is meaningless (the whole string
        // is always a substring of itself).
        for (int pos = 1; pos < n; pos++) {
                // Current position "pos" is fixed for this iteration. The
                // most recently found substring marked by "start" and "end"
                // is always chosen to be identical to some prefix. Therefore,
                // if "pos" points to an entry within this substring, it is
                // possible to reuse precomputed information in the prefix.
                // So initialize the current position's Z-value either with
                // its precomputed information (z[pos - start]) or with 0
                // in case of a substring which does not include the current
                // entry, i.e. when pos > end.
                z[pos] = std::max(0, std::min(z[pos - start], (end - pos + 1)));
                // Check if most recently found substring can be extended.
                // Therefore, look at the character right next to the ending
                // mark of current substring. If its index is smaller than
                // the maximum Z-array index (pos + z[pos] < n) and its
                // corresponding character matches that of the prefix
                // (s[z[pos]] == s[pos + z[pos]]), the substring can be
                // extended by updating pointers as well as current Z-value.
                while ((pos + z[pos] < n) && (s[z[pos]] == s[pos + z[pos]])) {
                        start = pos;
                        end = pos + z[pos];
                        z[pos]++;
                }
        }
        return z;
}

void printResult(std::string s, std::vector<int> z) {
        // Print values of Z-array in single line.
        for (int i = 0; i < z.size(); i++) {
                std::cout << z[i] << " ";
        }
        std::cout << "\n";
        // Print string characters directly underneath.
        for (int i = 0; i < s.size(); i++) {
                std::cout << s[i] << " ";
        }
        std::cout << std::endl;
}

int main() {
        // First test string.
        std::string s1("acbacdacbacbacda");
        std::vector<int> z1 = createZArray(s1);
        printResult(s1, z1);

        // Second test string.
        std::string s2("att#hattivatti");
        std::vector<int> z2 = createZArray(s2);
        printResult(s2, z2);

        return 0;
}
