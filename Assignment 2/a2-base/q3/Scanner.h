#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <iostream>
#include <string>

///////////////////////////////
// TokenType
//
//  symbols:  +, -, *, /, (, )
// literals:  integer
//  special:  eof, err
///////////////////////////////
enum class TokenType { plus, minus, asterisk, slash, leftParen, rightParen, integer, error, eof };

//---------//
//  Token  //
//---------//
struct Token {
    TokenType type;
    std::string text;
    int value;
};

// Desc: Display a Token
std::ostream& operator<<(std::ostream& os, const Token& token);

// Desc: Token scanner.  Given an input stream, will return a sequence
//       of Tokens via successive calls to .getnext();
class Scanner {
public:
    Scanner(std::istream& is);
    Token getnext();

private:
    std::istream& iStream_;
    char buffer_[2];
};

#endif // _SCANNER_H_
