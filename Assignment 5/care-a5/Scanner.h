#pragma once
#include <iostream>
#include <string>

// Desc:  TokenType identifies the token
//
// keywords:  and, or, not, print, if, while
//            (elif, else, break - in extensions only)
//  symbols:  =, +, -, *, /, (, ), <, >, ==, !=, <=, >=, {, }, ;
// literals:  integer, ident (must begin with _[a-z][A-Z])
//  special:  eof, err
enum TokenType {
    whiletok, // while
    iftok,    // if
    eliftok,  // elif
    elsetok,  // else
    printtok, // print
    breaktok, // break
    asgntok,  // =
    pltok,    // +
    mitok,    // -
    asttok,   // *
    slashtok, // /
    lptok,    // (
    rptok,    // )
    lttok,    // <
    gttok,    // >
    eqtok,    // ==
    netok,    // !=
    letok,    // <=
    getok,    // >=
    ortok,    // or
    andtok,   // and
    nottok,   // not
    lctok,    // {
    rctok,    // }
    sctok,    // ;
    ident,    // identifier
    integer,
    errtok, // error
    eof
};

std::string toktotext(TokenType);
std::ostream& operator<<(std::ostream&, TokenType);

////////////////////////////////////////////////////////////
//     -you can ignore everything beyond this point-
////////////////////////////////////////////////////////////

class Token {
public:
    TokenType tt;
    int line; // first line of input is line 1
    std::string text;
};

class Scanner {
public:
    Scanner(std::istream&);
    Token getnext();

private:
    int line;
    std::istream& str;
    char buf[2];
};
