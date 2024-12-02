#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

using namespace std;

// TokenType, Token, Lexer, and SymbolTable remain unchanged

class IntermediateCodeGenerator {
public:
    vector<string> instructions;
    int tempCount = 0;
    int labelCount = 0;

    string newTemp() {
        return "t" + to_string(tempCount++);
    }

    string newLabel() {
        return "L" + to_string(labelCount++);
    }

    void addInstruction(const string &instr) {
        instructions.push_back(instr);
    }

    void printInstructions() {
        for (const auto &instr : instructions) {
            cout << instr << endl;
        }
    }
};

class Parser {
public:
    Parser(const vector<Token> &tokens, SymbolTable &symTable, IntermediateCodeGenerator &icg)
        : tokens(tokens), pos(0), symTable(symTable), icg(icg) {}

    void parseProgram() {
        while (tokens[pos].type != T_EOF) {
            parseStatement();
        }
    }

private:
    vector<Token> tokens;
    size_t pos;
    SymbolTable &symTable;
    IntermediateCodeGenerator &icg;

    void parseStatement() {
        if (tokens[pos].type == T_INT) {
            parseDeclaration();
        } else if (tokens[pos].type == T_ID) {
            parseAssignment();
        } else if (tokens[pos].type == T_IF) {
            parseIfStatement();
        } else if (tokens[pos].type == T_RETURN) {
            parseReturnStatement();
        } else if (tokens[pos].type == T_LBRACE) {
            parseBlock();
        } else {
            cout << "Syntax error: unexpected token '" << tokens[pos].value << "' at line " << tokens[pos].lineNumber << endl;
            exit(1);
        }
    }

    void parseDeclaration() {
        expect(T_INT);
        string varName = expectAndReturnValue(T_ID);
        symTable.declareVariable(varName, "int");
        expect(T_SEMICOLON);
    }

    void parseAssignment() {
        string varName = expectAndReturnValue(T_ID);
        symTable.getVariableType(varName);
        expect(T_ASSIGN);
        string expr = parseExpression();
        icg.addInstruction(varName + " = " + expr);
        expect(T_SEMICOLON);
    }

    void parseIfStatement() {
        expect(T_IF);
        expect(T_LPAREN);
        string cond = parseExpression();
        expect(T_RPAREN);

        string trueLabel = icg.newLabel();
        string falseLabel = icg.newLabel();

        icg.addInstruction("if " + cond + " goto " + trueLabel);
        icg.addInstruction("goto " + falseLabel);
        icg.addInstruction(trueLabel + ":");
        parseStatement();

        if (tokens[pos].type == T_ELSE) {
            string endLabel = icg.newLabel();
            icg.addInstruction("goto " + endLabel);
            icg.addInstruction(falseLabel + ":");
            expect(T_ELSE);
            parseStatement();
            icg.addInstruction(endLabel + ":");
        } else {
            icg.addInstruction(falseLabel + ":");
        }
    }

    void parseReturnStatement() {
        expect(T_RETURN);
        string expr = parseExpression();
        icg.addInstruction("return " + expr);
        expect(T_SEMICOLON);
    }

    void parseBlock() {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF) {
            parseStatement();
        }
        expect(T_RBRACE);
    }

    string parseExpression() {
        string term = parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            TokenType op = tokens[pos++].type;
            string nextTerm = parseTerm();
            string temp = icg.newTemp();
            icg.addInstruction(temp + " = " + term + (op == T_PLUS ? " + " : " - ") + nextTerm);
            term = temp;
        }
        return term;
    }

    string parseTerm() {
        string factor = parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV) {
            TokenType op = tokens[pos++].type;
            string nextFactor = parseFactor();
            string temp = icg.newTemp();
            icg.addInstruction(temp + " = " + factor + (op == T_MUL ? " * " : " / ") + nextFactor);
            factor = temp;
        }
        return factor;
    }

    string parseFactor() {
        if (tokens[pos].type == T_NUM) {
            return tokens[pos++].value;
        } else if (tokens[pos].type == T_ID) {
            return tokens[pos++].value;
        } else if (tokens[pos].type == T_LPAREN) {
            expect(T_LPAREN);
            string expr = parseExpression();
            expect(T_RPAREN);
            return expr;
        } else {
            cout << "Syntax error: unexpected token '" << tokens[pos].value << "' at line " << tokens[pos].lineNumber << endl;
            exit(1);
        }
    }

    void expect(TokenType type) {
        if (tokens[pos].type != type) {
            cout << "Syntax error: expected '" << type << "' at line " << tokens[pos].lineNumber << endl;
            exit(1);
        }
        pos++;
    }

    string expectAndReturnValue(TokenType type) {
        string value = tokens[pos].value;
        expect(type);
        return value;
    }
};

int main() {
    string src = R"(
        int x;
        x = 10;
        int y;
        y = 20;
        int sum;
        sum = x + y * 3;
        if (5 > 3) {
            x = 20;
        } else {
            x = 30;
        }
        return sum;
    )";

    Lexer lexer(src);
    vector<Token> tokens = lexer.tokenize();

    SymbolTable symTable;
    IntermediateCodeGenerator icg;
    Parser parser(tokens, symTable, icg);

    parser.parseProgram();
    icg.printInstructions();

    return 0;
}
