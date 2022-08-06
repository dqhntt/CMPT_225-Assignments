#pragma once
#include "Scanner.h"
#include <istream>
#include <string>

class ExpnNode;  // read declaration below
class StmtNode;  // read declaration below
class StmtsNode; // read declaration below

StmtsNode* Parse(std::istream&);

// Desc:  A node in an expression tree.
//  Inv:  All internal nodes have two children, unless the
//        operator is unary minus or unary not, in which
//        case they only have the right child.
//  Inv:  All leaf nodes are either identifiers or integers,
//        i.e., tok == ident || tok == integer
class ExpnNode {
public:
    ExpnNode *left_operand, *right_operand;
    TokenType tok;
    std::string text;

    /////////////////////////////////////////////////////////////
    //  you will not need to call any of the methods shown below
    /////////////////////////////////////////////////////////////

    // Desc:  constructor for leaf node (integer or identifier)
    ExpnNode(TokenType tok, const std::string& val)
        : left_operand(nullptr)
        , right_operand(nullptr)
        , tok(tok)
        , text(val)
    { }
    // Desc:  constructor for interior node
    ExpnNode(ExpnNode* left, ExpnNode* right, TokenType op)
        : left_operand(left)
        , right_operand(right)
        , tok(op)
        , text()
    { }
    // Desc:  recursive destructor
    ~ExpnNode() {
        delete left_operand;
        delete right_operand;
    }
};

// Desc:  A node in a parse tree, representing a single statement.
class StmtNode {
public:
    TokenType tok;     //  Token: one of: printtok, asgntok, whiletok,
                       //                 iftok, eliftok, elsetok, breaktok
    std::string ident; //  Identifier: equals "" unless tok == asgntok
    ExpnNode* expn;    //  Expression: nullptr if tok == breaktok
    StmtsNode* stmts;  //  Statement[s]: nullptr if tok == printtok, asgntok or breaktok
    StmtNode* elif;    //  nullptr unless tok == iftok or eliftok

    /////////////////////////////////////////////////////////////
    // you will not need to call any of the methods shown below
    /////////////////////////////////////////////////////////////

    // Desc:  constructor
    StmtNode(
        TokenType tok, const std::string& ident, ExpnNode* expn, StmtsNode* stmts, StmtNode* elif)
        : tok(tok)
        , ident(ident)
        , expn(expn)
        , stmts(stmts)
        , elif(elif)
    { }
    // Desc:  recursive destructor
    ~StmtNode();
};

// Desc:  A node in a parse tree, representing a sequence of statements.
//  Inv:  ->stmt is the first statement
//  Inv:  ->stmts is the statements following the first
class StmtsNode {
public:
    StmtNode* stmt;   // first of a list of instructions, never nullptr
    StmtsNode* stmts; // remainder of list of instructions, nullptr if empty

    /////////////////////////////////////////////////////////////
    // you will not need to call any of the methods shown below
    /////////////////////////////////////////////////////////////

    // Desc:  constructor
    StmtsNode(StmtNode* stmt, StmtsNode* stmts)
        : stmt(stmt)
        , stmts(stmts)
    { }
    // Desc:  recursive destructor
    ~StmtsNode() {
        delete stmt;
        delete stmts;
    }
};
