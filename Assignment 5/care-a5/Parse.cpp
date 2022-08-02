#include "Parse.h"
#include <iostream>
using namespace std;

void match(TokenType);
StmtsNode* ParseS();
StmtsNode* ParseStmts();
StmtNode* ParseStmt();
ExpnNode* ParseE();
ExpnNode* ParseLAE();
ExpnNode* ParseRE();
ExpnNode* ParseAE();
ExpnNode* ParseT();
ExpnNode* ParseF();

Scanner* SCANNER = nullptr;
Token NEXT_TOKEN;

StmtsNode* Parse(istream& str) {
    SCANNER = new Scanner(str);
    NEXT_TOKEN = SCANNER->getnext();
    StmtsNode* ret = ParseS();
    delete SCANNER;
    return ret;
}

///////////////////////////////////////////////////////////////
//                     -descent parsing-
///////////////////////////////////////////////////////////////

void match(TokenType tt) {
    if (NEXT_TOKEN.tt != tt) {
        throw("Expected " + toktotext(tt) + " but scanned " + toktotext(NEXT_TOKEN.tt)
            + " on line " + to_string(NEXT_TOKEN.line));
    }
    NEXT_TOKEN = SCANNER->getnext();
}

StmtsNode* ParseS() {
    StmtsNode* ret = ParseStmts();
    try {
        match(eof);
    } catch (...) {
        delete ret;
        throw;
    }
    return ret;
}

StmtsNode* ParseStmts() {
    //------------------------------------------------
    //  <Stmts> -> <Stmt>*
    //  strategy: create a linked list of StmtNode *
    //------------------------------------------------
    StmtsNode ret(nullptr, nullptr);
    StmtsNode* cur = &ret;
    while ((NEXT_TOKEN.tt == printtok) || (NEXT_TOKEN.tt == ident) || (NEXT_TOKEN.tt == whiletok)
        || (NEXT_TOKEN.tt == iftok) || (NEXT_TOKEN.tt == breaktok)) {
        cur->stmts = new StmtsNode(ParseStmt(), nullptr);
        cur = cur->stmts;
    }
    cur = ret.stmts;
    ret.stmts = nullptr;
    return cur;
}

StmtNode* ParseStmt() {
    if (NEXT_TOKEN.tt == printtok) {
        //------------------------
        //  <Stmt> -> print <E> ;
        //------------------------
        match(printtok);
        ExpnNode* E = ParseE();
        match(sctok);
        return new StmtNode(printtok, "", E, nullptr, nullptr);
    } else if (NEXT_TOKEN.tt == ident) {
        //--------------------------
        //  <Stmt> -> ident = <E> ;
        //--------------------------
        string text = NEXT_TOKEN.text;
        match(ident);
        match(asgntok);
        ExpnNode* E = ParseE();
        match(sctok);
        return new StmtNode(asgntok, text, E, nullptr, nullptr);
    } else if (NEXT_TOKEN.tt == whiletok) {
        //----------------------------------
        //  <Stmt> -> while <E> { <Stmts> }
        //----------------------------------
        match(whiletok);
        ExpnNode* E = ParseE();
        match(lctok);
        StmtsNode* Stmts = ParseStmts();
        match(rctok);
        return new StmtNode(whiletok, "", E, Stmts, nullptr);
    } else if (NEXT_TOKEN.tt == iftok) {
        //------------------------------------------------------------------------------------
        //  <Stmt> -> if <E> { <Stmts> } (elif <E> { <Stmts> })* (epsilon | else { <Stmts> })
        //------------------------------------------------------------------------------------
        match(iftok);
        ExpnNode* E = ParseE();
        match(lctok);
        StmtsNode* Stmts = ParseStmts();
        match(rctok);
        StmtNode* ret = new StmtNode(iftok, "", E, Stmts, nullptr);
        StmtNode* cur = ret;
        while (NEXT_TOKEN.tt == eliftok) {
            match(eliftok);
            ExpnNode* E = ParseE();
            match(lctok);
            StmtsNode* Stmts = ParseStmts();
            match(rctok);
            cur->elif = new StmtNode(eliftok, "", E, Stmts, nullptr);
            cur = cur->elif;
        }
        if (NEXT_TOKEN.tt == elsetok) {
            match(elsetok);
            match(lctok);
            StmtsNode* Stmts = ParseStmts();
            match(rctok);
            cur->elif = new StmtNode(elsetok, "", new ExpnNode(integer, "1"), Stmts, nullptr);
        }
        return ret;
    } else if (NEXT_TOKEN.tt == breaktok) {
        match(breaktok);
        match(sctok);
        return new StmtNode(breaktok, "", nullptr, nullptr, nullptr);
    }
    return nullptr;
}

ExpnNode* ParseE() {
    //---------------------------
    //  <E> -> <LAE> (or <LAE>)*
    //---------------------------
    ExpnNode* ret = ParseLAE();
    while (true) {
        if (NEXT_TOKEN.tt == ortok) {
            match(NEXT_TOKEN.tt);
            ret = new ExpnNode(ret, ParseLAE(), ortok);
        } else
            return ret;
    }
}

ExpnNode* ParseLAE() {
    //----------------------------
    //  <LAE> -> <RE> (and <RE>)*
    //----------------------------
    ExpnNode* ret = ParseRE();
    while (true) {
        if (NEXT_TOKEN.tt == andtok) {
            match(NEXT_TOKEN.tt);
            ret = new ExpnNode(ret, ParseRE(), andtok);
        } else
            return ret;
    }
}

ExpnNode* ParseRE() {
    //--------------------------------------
    //  <RE> -> <AE> (epsilon | <rop> <AE>)
    //--------------------------------------
    int notct = 0;
    while (NEXT_TOKEN.tt == nottok) {
        notct++;
        match(nottok);
    }
    ExpnNode* ret = ParseAE();
    if ((NEXT_TOKEN.tt == lttok) || (NEXT_TOKEN.tt == gttok) || (NEXT_TOKEN.tt == eqtok)
        || (NEXT_TOKEN.tt == netok) || (NEXT_TOKEN.tt == letok) || (NEXT_TOKEN.tt == getok)) {
        TokenType tmp = NEXT_TOKEN.tt;
        match(NEXT_TOKEN.tt);
        ret = new ExpnNode(ret, ParseAE(), tmp);
    }
    while (notct > 2)
        notct -= 2;
    if (notct == 1) {
        return new ExpnNode(nullptr, ret, nottok);
    } else if (notct == 2) {
        return new ExpnNode(nullptr, new ExpnNode(nullptr, ret, nottok), nottok);
    } else {
        return ret;
    }
}

ExpnNode* ParseAE() {
    //-----------------------------
    //  <AE> -> <T> ((+ | -) <T>)*
    //-----------------------------
    ExpnNode* ret = ParseT();
    while (true) {
        if ((NEXT_TOKEN.tt == pltok) || (NEXT_TOKEN.tt == mitok)) {
            TokenType tmp = NEXT_TOKEN.tt;
            match(NEXT_TOKEN.tt);
            ret = new ExpnNode(ret, ParseT(), tmp);
        } else
            return ret;
    }
}

ExpnNode* ParseT() {
    //-----------------------------
    //  <T> -> <F> ((* | /) <F>)*
    //-----------------------------
    ExpnNode* ret = ParseF();
    while (true) {
        if ((NEXT_TOKEN.tt == asttok) || (NEXT_TOKEN.tt == slashtok)) {
            TokenType tmp = NEXT_TOKEN.tt;
            match(NEXT_TOKEN.tt);
            ret = new ExpnNode(ret, ParseF(), tmp);
        } else {
            return ret;
        }
    }
}

ExpnNode* ParseF() {
    //----------------------------------------------
    //  <F> -> (+ | -)* (integer | ident | ( <E> ))
    //----------------------------------------------
    int sign = 0;
    ExpnNode* ret = nullptr;
    while (true) {
        if (NEXT_TOKEN.tt == pltok) {
            match(pltok);
        } else if (NEXT_TOKEN.tt == mitok) {
            match(mitok);
            sign = (sign + 1) % 2;
        } else if ((NEXT_TOKEN.tt == integer) || (NEXT_TOKEN.tt == ident)) {
            ret = new ExpnNode(NEXT_TOKEN.tt, NEXT_TOKEN.text);
            match(NEXT_TOKEN.tt);
            if (sign != 0) {
                ret = new ExpnNode(nullptr, ret, mitok);
            }
            return ret;
        } else if (NEXT_TOKEN.tt == lptok) {
            match(lptok);
            ret = ParseE();
            match(rptok);
            return ret;
        } else {
            delete ret;
            throw("Expected literal but scanned " + toktotext(NEXT_TOKEN.tt) + " on line "
                + to_string(NEXT_TOKEN.line));
        }
    }
}

StmtNode::~StmtNode() {
    delete expn;
    delete stmts;
    delete elif;
}
