#include "Parse.h"
#include "Scanner.h"
#include "hash.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
using namespace std;

constexpr int TAB_STOP = 4;

///////////////////////////////////////////////
//----- carried over from testParse.cpp -----//
///////////////////////////////////////////////

// Desc:  These 3 functions return a string which is a pretty-printed
//        version of the subtree rooted there.
string printExpn(ExpnNode* root);
string printStmts(StmtsNode* root, int ilevel);
string print(StmtsNode* root);

// Desc:  Generate pretty printed pish of the parse tree rooted at root.
string print(StmtsNode* root) {
    return printStmts(root, 0); // call helper
}

// Desc:  Generate pretty printed pish, by traversing like a linked list.
//        Parameter ilevel is the indentation level.
string printStmts(StmtsNode* root, int ilevel) {
    // generate leading tab
    const string space(TAB_STOP, ' ');
    string tab;
    for (int i = 0; i < ilevel; i++)
        tab += space;

    // traverse StmtsNode * like it is a linked list
    StmtsNode* curr = root;
    string output; // pretty-printed version

    while (curr != nullptr) {
        assert(curr->stmt != nullptr);

        // print statement
        if (curr->stmt->tok == printtok) {
            output += tab + toktotext(printtok) + " " + printExpn(curr->stmt->expn) + toktotext(sctok)
                + "\n";
        }
        // assignment statement
        else if (curr->stmt->tok == asgntok) {
            output += tab + curr->stmt->ident + " " + toktotext(asgntok) + " "
                + printExpn(curr->stmt->expn) + toktotext(sctok) + "\n";
        }
        // while statement: tab level increases for subStmts
        else if (curr->stmt->tok == whiletok) {
            output += tab + toktotext(whiletok) + " " + printExpn(curr->stmt->expn) + " "
                + toktotext(lctok) + "\n" + printStmts(curr->stmt->stmts, ilevel + 1) + tab
                + toktotext(rctok) + "\n";
        }
        // if/elif/else statement: tab level increases for subStmts
        else if (curr->stmt->tok == iftok) {
            StmtNode* elif = curr->stmt;
            while (elif != nullptr) {
                output += tab + toktotext(elif->tok)
                    + ((elif->tok == elsetok) ? "" : (" " + printExpn(elif->expn))) + " "
                    + toktotext(lctok) + "\n" + printStmts(elif->stmts, ilevel + 1) + tab
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

// Desc:  Generate bracketed infix via postorder traversal
string printExpn(ExpnNode* root) {
    if (root == nullptr)
        return "";
    if (root->text != "")
        return root->text;
    if (root->left_operand == nullptr) {
        return "(" + toktotext(root->tok) + ((root->tok == nottok) ? " " : "")
            + printExpn(root->right_operand) + ")";
    } else {
        return "(" + printExpn(root->left_operand) + " " + toktotext(root->tok) + " "
            + printExpn(root->right_operand) + ")";
    }
}

int main() {
    StmtsNode* pTree = nullptr;
    try {
        pTree = Parse(cin);
    } catch (const string& s) {
        cout << "Parse error: " << s << endl;
        delete pTree;
        return 1;
    }

    cout << "Parse Successful!\n";
    cout << print(pTree) << endl;
    delete pTree;
}
