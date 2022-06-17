#include "Scanner.h"
#include "Stack.h" // GENERIC STACK
#include <cassert>
#include <iostream>

namespace {
//  Pre:  `op` is one of these 4 math operators: { + - * / }
// Post:  Returns token with the result of [lhs] [op] [rhs]
Token compute(const Token& lhs, const Token& rhs, TokenType op) {
    int result = -1;
    switch (op) {
    case TokenType::plus:
        result = lhs.value + rhs.value;
        break;
    case TokenType::minus:
        result = lhs.value - rhs.value;
        break;
    case TokenType::asterisk:
        result = lhs.value * rhs.value;
        break;
    case TokenType::slash:
        result = lhs.value / rhs.value;
        break;
    default:
        throw std::invalid_argument("Invalid operator");
    }
    return { TokenType::integer, std::to_string(result), result };
}

// Pre:  numstack has 2 or more tokens.
//       opstack has 1 or more tokens.
Token popAndCompute(Stack<Token>& numstack, Stack<Token>& opstack) {
    assert(!numstack.isEmpty());
    const Token rhs = numstack.pop();

    assert(!numstack.isEmpty());
    const Token lhs = numstack.pop();

    assert(!opstack.isEmpty());
    const TokenType topOp = opstack.pop().type;

    return compute(lhs, rhs, topOp);
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
