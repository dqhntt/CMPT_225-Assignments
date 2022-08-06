#include "Parse.h"
#include "Scanner.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

namespace {

constexpr int TAB_STOP = 4, M = 3061, SCALE = 225;

////////////////////////////////////////
//----- carried over from hash.h -----//
////////////////////////////////////////

// Set<T> is ridiculous. Repurposing it.

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
        const string& key;
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

// Pre: key is not empty
//      key is an lvalue
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

class div_by_zero_error : public domain_error {
public:
    div_by_zero_error() : domain_error("Division by zero error") { }
};

int evaluate(const ExpnNode* expn, Map& var_map) {
    if (expn == nullptr) {
        return 0; // Unary minus / plus.
    }
    // Leaves.
    if (expn->tok == integer) {
        return stoi(expn->text);
    }
    if (expn->tok == ident) {
        return var_map.at(expn->text);
    }
    // Left-hand side.
    const int lhs = evaluate(expn->left_operand, var_map);
    if (expn->tok == ortok) {
        // Implicit short-circuit evaluation.
        return lhs || evaluate(expn->right_operand, var_map);
    }
    if (expn->tok == andtok) {
        // Implicit short-circuit evaluation.
        return lhs && evaluate(expn->right_operand, var_map);
    }
    if (expn->tok == asttok) {
        return (lhs == 0) ? 0 : lhs * evaluate(expn->right_operand, var_map);
    }
    if (expn->tok == slashtok) {
        if (lhs == 0) {
            return 0;
        }
        const int rhs = evaluate(expn->right_operand, var_map);
        if (rhs == 0) {
            throw div_by_zero_error {};
        }
        return floor(static_cast<double>(lhs) / rhs);
    }
    // Right-hand side.
    const int rhs = evaluate(expn->right_operand, var_map);
    switch (expn->tok) {
    case pltok:
        return lhs + rhs;
    case mitok:
        return lhs - rhs;
    case lttok:
        return lhs < rhs;
    case gttok:
        return lhs > rhs;
    case eqtok:
        return lhs == rhs;
    case netok:
        return lhs != rhs;
    case letok:
        return lhs <= rhs;
    case getok:
        return lhs >= rhs;
    case nottok:
        return !rhs;
    default:
        assert(false && "Unknown token in expression");
        return -1;
    }
}

class break_stmt : public runtime_error {
public:
    // NEVER use exceptions just to signal normal info.
    using runtime_error::runtime_error;
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
            var_map.insert(curr->stmt->ident, evaluate(curr->stmt->expn, var_map));
        }
        // while statement
        else if (curr->stmt->tok == whiletok) {
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
                if (elif->tok == iftok || elif->tok == eliftok) {
                    if (evaluate(elif->expn, var_map)) {
                        run(elif->stmts, var_map);
                        break;
                    }
                } else {
                    run(elif->stmts, var_map);
                }
                elif = elif->elif;
            }
        }
        // break statement
        else if (curr->stmt->tok == breaktok) {
            throw break_stmt("break;");
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
        cerr << "Parse error: " << s << '\n';
        return EXIT_FAILURE;
    }

    // cout << "Parse Successful!\n"
    //      << string(50, '-') << '\n'
    //      << repr_stmts(parsed_tree)
    //      << string(50, '-') << endl;

    Map var_map;
    try {
        run(parsed_tree, var_map);
    } catch (const break_stmt& brk) {
        delete parsed_tree;
        cerr << "Parse error: Misplaced " << brk.what() << '\n';
        return EXIT_FAILURE;
    } catch (const exception& e) {
        delete parsed_tree;
        cerr << "EXCEPTION: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    delete parsed_tree;
    return EXIT_SUCCESS;
}
