#include "Scanner.h"
#include "Stack.h" // GENERIC STACK
#include <iostream>

namespace {
Token compute(const Token& x, const Token& y, TokenType op) {
    int result = -1;
    switch (op) {
    case TokenType::plus:
        result = x.value + y.value;
        break;
    case TokenType::minus:
        result = x.value - y.value;
        break;
    case TokenType::asterisk:
        result = x.value * y.value;
        break;
    case TokenType::slash:
        result = x.value / y.value;
        break;
    default:
        throw std::invalid_argument("Invalid operator");
    }
    return { TokenType::integer, std::to_string(result), result };
}

Token popAndCompute(Stack<Token>& numstack, Stack<Token>& opstack) {
    const Token y = numstack.pop();
    const Token x = numstack.pop();
    const TokenType topOp = opstack.pop().type;
    return compute(x, y, topOp);
}
} // namespace

int main() {
    Scanner scanner(std::cin);
    Stack<Token> numstack, opstack; // 2x Stacks of type Token
    Token currToken = scanner.getnext();

    while (currToken.type != TokenType::eof || !opstack.isEmpty()) {
        switch (currToken.type) {
        case TokenType::integer: {
            numstack.push(std::move(currToken));
            currToken = scanner.getnext();
            break;
        }
        case TokenType::leftParen: {
            opstack.push(std::move(currToken));
            currToken = scanner.getnext();
            break;
        }
        case TokenType::rightParen: {
            if (opstack.peek().type == TokenType::leftParen) {
                opstack.pop();
                currToken = scanner.getnext();
            } else {
                numstack.push(popAndCompute(numstack, opstack));
            }
            break;
        }
        case TokenType::plus:
        case TokenType::minus:
        case TokenType::eof: {
            const TokenType topOp = !opstack.isEmpty() ? opstack.peek().type : TokenType::error;
            if (!opstack.isEmpty()
                && (topOp == TokenType::plus || topOp == TokenType::minus
                    || topOp == TokenType::asterisk || topOp == TokenType::slash)) {
                numstack.push(popAndCompute(numstack, opstack));
            } else {
                opstack.push(std::move(currToken));
                currToken = scanner.getnext();
            }
            break;
        }
        case TokenType::asterisk:
        case TokenType::slash: {
            const TokenType topOp = !opstack.isEmpty() ? opstack.peek().type : TokenType::error;
            if (!opstack.isEmpty() && (topOp == TokenType::asterisk || topOp == TokenType::slash)) {
                numstack.push(popAndCompute(numstack, opstack));
            } else {
                opstack.push(std::move(currToken));
                currToken = scanner.getnext();
            }
            break;
        }
        default:;
        }
    } // while

    std::cout << numstack.pop() << std::endl;
}

// token = scanner.getnext();
// // pretty printer coding demo.  please remove before coding
// while (token.tt != TokenType::eof) {
//     switch (token.tt) {
//     case TokenType::integer:
//     case TokenType::leftParen:
//     case TokenType::rightParen:
//         std::cout << token;
//         break;
//     case TokenType::plus:
//     case TokenType::minus:
//     case TokenType::asterisk:
//     case TokenType::slash:
//         std::cout << ' ' << token << ' ';
//         break;
//     default:;
//     }
//     token = scanner.getnext();
// }
// std::cout << std::endl;
// // end pretty printer
