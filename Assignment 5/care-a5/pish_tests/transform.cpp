#include <iostream>
#include <string>
using namespace std;

// Pre: Input is parsed with a pretty printer.
int main() {
    string line;
    // Remove header row.
    getline(cin, line);
    while (getline(cin, line)) {
        if (line.length() > 0) {
            if (line.back() == '}') {
                line.clear();
            } else if (line.back() == '{') {
                line.pop_back();
                line.back() = ':';
            } else {
                line.pop_back();
                // For python3.
                const auto n = line.find("print ");
                if (n != string::npos) {
                    line[n + 5] = '(';
                    line.push_back(')');
                }
            }
        }
        cout << line << '\n';
    }
}
