#include "Parse.h"
#include "Scanner.h"
#include "hash.h"
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

template <class T>
using Stack = std::vector<T>;

constexpr int TAB_STOP = 4;

class var {
public:
    string key;

    // * * * add some more attributes and methods here * * * //
};

// * * * declare more things here * * * //

int run(const StmtsNode* stmts) {

    // TODO
    cout << "Running TODO.\n";
    return sizeof(stmts);
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
                + toktotext(sctok) + "\n";
        }
        // assignment statement
        else if (curr->stmt->tok == asgntok) {
            output += tab + curr->stmt->ident + " " + toktotext(asgntok) + " "
                + repr_expn(curr->stmt->expn) + toktotext(sctok) + "\n";
        }
        // while statement: tab level increases for sub Stmts
        else if (curr->stmt->tok == whiletok) {
            output += tab + toktotext(whiletok) + " " + repr_expn(curr->stmt->expn) + " "
                + toktotext(lctok) + "\n" + repr_stmts(curr->stmt->stmts, indent_level + 1) + tab
                + toktotext(rctok) + "\n";
        }
        // if/elif/else statement: tab level increases for sub Stmts
        else if (curr->stmt->tok == iftok) {
            const StmtNode* elif = curr->stmt;
            while (elif != nullptr) {
                output += tab + toktotext(elif->tok)
                    + ((elif->tok == elsetok) ? "" : (" " + repr_expn(elif->expn))) + " "
                    + toktotext(lctok) + "\n" + repr_stmts(elif->stmts, indent_level + 1) + tab
                    + toktotext(rctok) + "\n";
                elif = elif->elif;
            }
        }
        // break statement
        else if (curr->stmt->tok == breaktok) {
            output += tab + toktotext(breaktok) + toktotext(sctok) + "\n";
        } else {
            assert(false && "Unknown statement");
        }

        // next node in the chain
        curr = curr->stmts;
    }
    return output;
}

int main() {
    Set<var> memmap;

    const StmtsNode* parsed_tree = nullptr;
    try {
        parsed_tree = Parse(cin);
    } catch (const string& s) {
        cout << "Parse error: " << s << "\n";
        return EXIT_FAILURE;
    }

    cout << "Parse Successful!\n"
         << string(50, '-') << "\n"
         << repr_stmts(parsed_tree)
         << string(50, '-') << endl;
    run(parsed_tree);
    delete parsed_tree;
    return EXIT_SUCCESS;
}
