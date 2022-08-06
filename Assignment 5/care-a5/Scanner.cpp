#include "Scanner.h"
using namespace std;

string operator+(const string& lhs, char rhs) {
    return lhs + &rhs;
}

string toktotext(TokenType tt) {
    switch (tt) {
    case whiletok: return "while";
    case iftok: return "if";
    case eliftok: return "elif";
    case elsetok: return "else";
    case printtok: return "print";
    case breaktok: return "break";
    case asgntok: return "=";
    case pltok: return "+";
    case mitok: return "-";
    case asttok: return "*";
    case slashtok: return "/";
    case lptok: return "(";
    case rptok: return ")";
    case lttok: return "<";
    case gttok: return ">";
    case eqtok: return "==";
    case netok: return "!=";
    case letok: return "<=";
    case getok: return ">=";
    case ortok: return "or";
    case andtok: return "and";
    case nottok: return "not";
    case lctok: return "{";
    case rctok: return "}";
    case sctok: return ";";
    case ident: return "identifier";
    case integer: return "integer";
    case eof: return "eof";
    default: return "err";
    }
}

ostream& operator<<(ostream& lhs, TokenType rhs) {
    lhs << toktotext(rhs);
    return lhs;
}

Scanner::Scanner(istream& is)
    : line { 1 }
    , str { is }
    , buf { '\0', '\0' }
{ }

int newl(char c) {
    return (c == '\n' || c == '\r');
}

Token Scanner::getnext() {
    Token ret;
    if (buf[0] == '\0') {
        buf[0] = str.get();
        line += newl(buf[0]);
    }

    // collapse whitespace
    while (isspace(buf[0]) || (buf[0] == '\n')) {
        buf[0] = str.get();
        line += newl(buf[0]);
        if (str.eof())
            break;
    }

    // case 1: eof
    if (str.eof()) {
        ret.tt = eof;
        ret.line = line;
        return ret;
    }

    // case 2: identifier-  begins with a-zA-z_, may contain 0-9
    if ((buf[0] == '_') || isalpha(buf[0])) {
        ret.tt = ident;
        ret.line = line;
        ret.text = buf;
        buf[0] = str.get();
        line += newl(buf[0]);
        while ((buf[0] == '_') || isalpha(buf[0]) || isdigit(buf[0])) {
            ret.text += buf;
            buf[0] = str.get();
            line += newl(buf[0]);
        }
        if (isspace(buf[0]) || (buf[0] == '\n'))
            buf[0] = '\0';
        if (ret.text == "while") ret.tt = whiletok;
        else if (ret.text == "if") ret.tt = iftok;
        else if (ret.text == "elif") ret.tt = eliftok;
        else if (ret.text == "else") ret.tt = elsetok;
        else if (ret.text == "print") ret.tt = printtok;
        else if (ret.text == "break") ret.tt = breaktok;
        else if (ret.text == "or") ret.tt = ortok;
        else if (ret.text == "and") ret.tt = andtok;
        else if (ret.text == "not") ret.tt = nottok;
        return ret;
    }

    // case 3: numerical-   [0-9]+
    if (isdigit(buf[0])) {
        ret.tt = integer;
        ret.line = line;
        ret.text = buf;
        buf[0] = str.get();
        line += newl(buf[0]);
        while (isdigit(buf[0])) {
            ret.text += buf;
            buf[0] = str.get();
            line += newl(buf[0]);
        }
        if (isspace(buf[0]) || (buf[0] == '\n'))
            buf[0] = '\0';
        return ret;
    }

    // case 4: symbol- anything not covered above (which isn't whitespace)
    ret.text = buf; 
    ret.line = line; 
    if (buf[0] == '+') { ret.tt = pltok; buf[0] = '\0'; }
    else if (buf[0] == '-') { ret.tt = mitok; buf[0] = '\0'; }
    else if (buf[0] == '*') { ret.tt = asttok; buf[0] = '\0'; }
    else if (buf[0] == '/') { ret.tt = slashtok; buf[0] = '\0'; }
    else if (buf[0] == '(') { ret.tt = lptok; buf[0] = '\0'; }
    else if (buf[0] == ')') { ret.tt = rptok; buf[0] = '\0'; }
    else if (buf[0] == '{') { ret.tt = lctok; buf[0] = '\0'; }
    else if (buf[0] == '}') { ret.tt = rctok; buf[0] = '\0'; }
    else if (buf[0] == ';') { ret.tt = sctok; buf[0] = '\0'; }
    else if (buf[0] == '<') {
        buf[0] = str.get();
        if (buf[0] == '=') { ret.tt = letok; ret.text += buf[0]; buf[0] = '\0'; }
        else { ret.tt = lttok; }
    } else if (buf[0] == '>') {
        buf[0] = str.get();
        if (buf[0] == '=') { ret.tt = getok; ret.text += buf[0]; buf[0] = '\0'; }
        else { ret.tt = gttok; }
    } else if (buf[0] == '=') {
        buf[0] = str.get();
        if (buf[0] == '=') { ret.tt = eqtok; ret.text += buf[0]; buf[0] = '\0'; }
        else { ret.tt = asgntok; }
    } else if (buf[0] == '!') {
        buf[0] = str.get();
        if (buf[0] == '=') { ret.tt = netok; ret.text += buf[0]; buf[0] = '\0'; }
        else { ret.tt = errtok; }
    } else {
        ret.tt = errtok; buf[0] = '\0';
    }
    return ret;
}
