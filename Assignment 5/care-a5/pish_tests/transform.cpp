#include <iostream>
#include <string>
using namespace std;

// Pre: Input is parsed with a pretty printer.
int main(int argc, char* argv[]) {
    const bool py2 = (argc >= 2 && argv[1][0] == '2');
    string line;
    // Remove header row.
    if (!getline(cin, line)) {
        return 1;
    }
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
                if (!py2) {
                    const auto n = line.find("print ");
                    if (n != string::npos) {
                        line[n + 5] = '(';
                        line.push_back(')');
                    }
                }
            }
        }
        cout << line << '\n';
    }
}
