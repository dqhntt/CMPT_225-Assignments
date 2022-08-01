#include "Scanner.h"
using namespace std;

// Desc:  Display a Token
ostream& operator<<(ostream& os, const Token& token) {
    if (token.type == Token::eof)
        os << "eof";
    else if (token.type == Token::error)
        os << "err";
    else if (token.type == Token::integer)
        os << token.value;
    else
        os << token.text;

    return os;
} // os << Token

// Desc:  Constructor
Scanner::Scanner(istream& is)
    : iStream_ { is }
    , buffer_ { '\0', '\0' }
{ }

// Desc:  Return the next token
Token Scanner::getnext() {
    Token ret;
    if (buffer_[0] == 0) {
        buffer_[0] = iStream_.get();
    }

    // collapse whitespace
    while (isspace(buffer_[0]) || (buffer_[0] == '\r') || (buffer_[0] == '\n')) {
        buffer_[0] = iStream_.get();
        if (iStream_.eof())
            break;
    }

    // case 1: eof
    if (iStream_.eof()) {
        ret.type = Token::eof;
        return ret;
    }

    // case 2: numerical-   [0-9]+
    if (isdigit(buffer_[0])) {
        ret.type = Token::integer;
        ret.text = buffer_;
        buffer_[0] = iStream_.get();
        while (isdigit(buffer_[0])) {
            ret.text += buffer_;
            buffer_[0] = iStream_.get();
        }
        ret.value = stoi(ret.text);
        if (isspace(buffer_[0]) || (buffer_[0] == '\r') || (buffer_[0] == '\n'))
            buffer_[0] = '\0';
        return ret;
    }

    // case 3: symbol
    ret.text = buffer_;
    if (buffer_[0] == '+') {
        ret.type = Token::plus;
    } else if (buffer_[0] == '-') {
        ret.type = Token::minus;
    } else if (buffer_[0] == '*') {
        ret.type = Token::asterisk;
    } else if (buffer_[0] == '/') {
        ret.type = Token::slash;
    } else if (buffer_[0] == '(') {
        ret.type = Token::leftParen;
    } else if (buffer_[0] == ')') {
        ret.type = Token::rightParen;
    } else {
        ret.type = Token::error;
    }
    buffer_[0] = '\0';
    return ret;
}
