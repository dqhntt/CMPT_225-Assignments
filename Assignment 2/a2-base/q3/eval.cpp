#include "Scanner.h"
#include "Stack.h" // GENERIC STACK
#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>

namespace {
//  Pre:  `op` is one of these 4 math operators: { + - * / }
// Post:  Returns token with the result of [lhs] [op] [rhs]
Token compute(const Token& lhs, const Token& rhs, Token::Type op) {
    int result = -1;
    switch (op) {
    case Token::plus:
        result = lhs.value + rhs.value;
        break;
    case Token::minus:
        result = lhs.value - rhs.value;
        break;
    case Token::asterisk:
        result = lhs.value * rhs.value;
        break;
    case Token::slash:
        result = lhs.value / rhs.value;
        break;
    default:
        throw std::invalid_argument("Invalid operator");
    }
    return { Token::integer, std::to_string(result), result };
}

// Pre:  numstack has 2 or more tokens.
//       opstack has 1 or more tokens.
Token popAndCompute(Stack<Token>& numstack, Stack<Token>& opstack) {
    assert(!numstack.isEmpty());
    const Token rhs = numstack.pop();

    assert(!numstack.isEmpty());
    const Token lhs = numstack.pop();

    assert(!opstack.isEmpty());
    const Token::Type topOp = opstack.pop().type;

    return compute(lhs, rhs, topOp);
}

//  Pre:  expression is well-formed valid math infix expression.
// Post:  Returns result of expression.
int evaluateInfix(const std::string& expression) {
    std::istringstream iss(expression);

    Scanner scanner(iss);
    Stack<Token> numstack, opstack; // 2x Stacks of type Token
    Token currToken = scanner.getnext();

    while (currToken.type != Token::eof || !opstack.isEmpty()) {
        switch (currToken.type) {
        case Token::integer: {
            numstack.push(std::move(currToken));
            currToken = scanner.getnext();
            break;
        }
        case Token::leftParen: {
            opstack.push(std::move(currToken));
            currToken = scanner.getnext();
            break;
        }
        case Token::rightParen: {
            if (opstack.peek().type == Token::leftParen) {
                opstack.pop();
                currToken = scanner.getnext();
            } else {
                numstack.push(popAndCompute(numstack, opstack));
            }
            break;
        }
        case Token::plus:
        case Token::minus:
        case Token::eof: {
            const Token::Type topOp = !opstack.isEmpty() ? opstack.peek().type : Token::error;
            if (!opstack.isEmpty()
                && (topOp == Token::plus || topOp == Token::minus || topOp == Token::asterisk
                    || topOp == Token::slash)) {
                numstack.push(popAndCompute(numstack, opstack));
            } else {
                opstack.push(std::move(currToken));
                currToken = scanner.getnext();
            }
            break;
        }
        case Token::asterisk:
        case Token::slash: {
            const Token::Type topOp = !opstack.isEmpty() ? opstack.peek().type : Token::error;
            if (!opstack.isEmpty() && (topOp == Token::asterisk || topOp == Token::slash)) {
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
    return numstack.pop().value;
}

int evaluateInfixHelper(std::istream& is) {
    std::string expression;
    std::getline(is, expression);
    return evaluateInfix(expression);
}

void infixTest() {
    std::unordered_map<std::string, int> testCases;
    testCases[" 0 "] = 0;
    testCases["0 - 0"] = 0;
    testCases["0 + 0"] = 0;
    testCases["0 * 0"] = 0;
    testCases["0 * 1"] = 0;
    testCases["0- 1"] = -1;
    testCases["0/1"] = 0;
    testCases["1 +1"] = 2;
    testCases["1 + 2 + 3 + 4"] = 10;
    testCases["1 - 2 * 3 + 4"] = -1;
    testCases["1 - 2 - 3 - 4 - 0  "] = -8;
    testCases["(25 - 16) * (8 + 6)/3"] = 42;
    testCases["1 + ( 7 * 6 ) / 3 + 6"] = 21;
    testCases["( 48 / 8 ) + 7 * 2 - 4 * 5"] = 0;
    testCases["0 - 2147483647"] = -2147483647;
    testCases["2147483647 + 0 * 0"] = 2147483647;
    testCases["2147483647 - 2147483647 - 2147483647"] = -2147483647;
    testCases["1 * 2 * 3 * 4 * 5 / ( 1 * 2 * 3 * 4 * 5 ) - 5"] = -4;
    testCases
        ["((((((((1-5)+5)*10 + 5)+(2-5))/4 + 5* 10 - 50) * (1-60) + 77)* (1-2))/2 - 50 ) + 1 * 10"]
        = 10;
    testCases["((((((1 - 10) + 5) + (2-20) * 50)+4)/300 + (1 + 5)/2 + (1 - 3)*10)/2 + (1- 21) *2 / "
              "2)/15 + (10-9) * 2 + 40"]
        = 40;
    for (const auto& test : testCases) {
        const int currResult = evaluateInfix(test.first);
        if (currResult != test.second) {
            throw std::runtime_error("Assertion failed. Expecting: " + std::to_string(test.second)
                + " | Got: " + std::to_string(currResult) + "\nEvaluating: " + test.first);
        };
    }
}
} // namespace

int main() {
    infixTest();
    std::cout << evaluateInfixHelper(std::cin) << std::endl;
}
