#include "Parse.h"
#include "Scanner.h"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

namespace {
template <class T>
using Stack = std::vector<T>;

constexpr int TAB_STOP = 4, M = 3061, SCALE = 225;

////////////////////////////////////////
//----- carried over from hash.h -----//
////////////////////////////////////////

// Hash Map: {string: int}
class Map {
public:
    Map();
    ~Map();
    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;

    void insert(const string& key, int value);
    int at(const string& key) const;

private:
    struct var {
        string key;
        int value;
    };
    var* arr_[M];
};

Map::Map() { fill(arr_, arr_ + M, nullptr); }
Map::~Map() {
    for (var* p : arr_) {
        delete p;
    }
}

int hash(const string& str, int base = 64, int max_val = M) {
    int hashed_val = 0;
    for (char c : str) {
        hashed_val = (hashed_val * base + c) % max_val;
    }
    return ((SCALE % max_val) * hashed_val) % max_val;
}

// Pre: key not empty
void Map::insert(const string& key, int value) {
    static_assert(M > 1, "Array with min size = 2 is needed for this function's loop");
    assert(key != "");
    const int init_index = hash(key);
    int index = init_index;
    for (int i = 1; i < M; i++) {
        auto& curr = arr_[index];
        if (curr == nullptr) {
            curr = new var { key, value };
            return;
        } else if (curr->key == key) {
            curr->value = value;
            return;
        }
        const int next_index = (init_index + i * i) % M;
        if (next_index == index) {
            throw length_error("Can't insert key: " + key);
        }
        index = next_index;
    }
}

// Pre: key not empty
int Map::at(const string& key) const {
    static_assert(M > 1, "Array with min size = 2 is needed for this function's loop");
    assert(key != "");
    const int init_index = hash(key);
    int index = init_index;
    for (int i = 1; i < M; i++) {
        const var* const curr = arr_[index];
        if (curr == nullptr) {
            break;
        } else if (curr->key == key) {
            return curr->value;
        }
        const int next_index = (init_index + i * i) % M;
        if (next_index == index) {
            break;
        }
        index = next_index;
    }
    throw out_of_range("Key not found: " + key);
    return -1;
}

int evaluate(const ExpnNode* expn, Map& var_map, int result = 0) {
    assert(expn != nullptr);

    // TODO

}

class break_stmt : public bad_exception {
    // NEVER use exceptions just to signal info.
};

void run(const StmtsNode* stmts, Map& var_map) {
    // traverse StmtsNode* like it is a linked list
    const StmtsNode* curr = stmts;
    while (curr != nullptr) {
        assert(curr->stmt != nullptr);
        // print statement
        if (curr->stmt->tok == printtok) {
            cout << evaluate(curr->stmt->expn, var_map) << '\n';
        }
        // assignment statement
        else if (curr->stmt->tok == asgntok) {

            // TODO ?
            var_map.insert(curr->stmt->ident, evaluate(curr->stmt->expn, var_map));
        }
        // while statement
        else if (curr->stmt->tok == whiletok) {

            // TODO ?
            while (evaluate(curr->stmt->expn, var_map)) {
                try {
                    run(curr->stmt->stmts, var_map);
                } catch (const break_stmt&) {
                    break;
                }
            }
        }
        // if/elif/else statement
        else if (curr->stmt->tok == iftok) {
            const StmtNode* elif = curr->stmt;
            while (elif != nullptr) {

                // TODO

                if (evaluate(elif->expn, var_map)) {
                    run(elif->stmts, var_map);
                }
                elif = elif->elif;
            }
        }
        // break statement
        else if (curr->stmt->tok == breaktok) {

            // TODO ?
            throw break_stmt {};
        } else {
            assert(false && "Unknown statement");
        }
        // next node in the chain
        curr = curr->stmts;
    }
}

///////////////////////////////////////////////
//----- carried over from testParse.cpp -----//
///////////////////////////////////////////////

// Desc:  Generate bracketed infix via inorder traversal
string repr_expn(const ExpnNode* root) {
    if (root == nullptr)
        return "";
    if (root->text != "")
        return root->text;
    if (root->left_operand == nullptr) {
        return "(" + toktotext(root->tok) + ((root->tok == nottok) ? " " : "")
            + repr_expn(root->right_operand) + ")";
    } else {
        return "(" + repr_expn(root->left_operand) + " " + toktotext(root->tok) + " "
            + repr_expn(root->right_operand) + ")";
    }
}

// Desc:  Generate pretty printed pish, by traversing like a linked list.
//        Parameter indent_level is the indentation level.
string repr_stmts(const StmtsNode* root, int indent_level = 0) {
    // generate leading tab
    const string space(TAB_STOP, ' ');
    string tab;
    for (int i = 0; i < indent_level; i++)
        tab += space;

    // traverse StmtsNode* like it is a linked list
    const StmtsNode* curr = root;
    string output; // pretty-printed version

    while (curr != nullptr) {
        assert(curr->stmt != nullptr);

        // print statement
        if (curr->stmt->tok == printtok) {
            output += tab + toktotext(printtok) + " " + repr_expn(curr->stmt->expn)
                + toktotext(sctok) + '\n';
        }
        // assignment statement
        else if (curr->stmt->tok == asgntok) {
            output += tab + curr->stmt->ident + " " + toktotext(asgntok) + " "
                + repr_expn(curr->stmt->expn) + toktotext(sctok) + '\n';
        }
        // while statement: tab level increases for sub Stmts
        else if (curr->stmt->tok == whiletok) {
            output += tab + toktotext(whiletok) + " " + repr_expn(curr->stmt->expn) + " "
                + toktotext(lctok) + '\n' + repr_stmts(curr->stmt->stmts, indent_level + 1) + tab
                + toktotext(rctok) + '\n';
        }
        // if/elif/else statement: tab level increases for sub Stmts
        else if (curr->stmt->tok == iftok) {
            const StmtNode* elif = curr->stmt;
            while (elif != nullptr) {
                output += tab + toktotext(elif->tok)
                    + ((elif->tok == elsetok) ? "" : (" " + repr_expn(elif->expn))) + " "
                    + toktotext(lctok) + '\n' + repr_stmts(elif->stmts, indent_level + 1) + tab
                    + toktotext(rctok) + '\n';
                elif = elif->elif;
            }
        }
        // break statement
        else if (curr->stmt->tok == breaktok) {
            output += tab + toktotext(breaktok) + toktotext(sctok) + '\n';
        } else {
            assert(false && "Unknown statement");
        }

        // next node in the chain
        curr = curr->stmts;
    }
    return output;
}
} // namespace

int main() {
    const StmtsNode* parsed_tree = nullptr;
    try {
        parsed_tree = Parse(cin);
    } catch (const string& s) {
        cout << "Parse error: " << s << '\n';
        return EXIT_FAILURE;
    }

    cout << "Parse Successful!\n"
         << string(50, '-') << '\n'
         << repr_stmts(parsed_tree)
         << string(50, '-') << endl;
    Map var_map;
    run(parsed_tree, var_map);
    delete parsed_tree;
    return EXIT_SUCCESS;
}
