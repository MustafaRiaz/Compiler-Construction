#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <fstream>
using namespace std;

enum TokenType {
    T_INT, T_FLOAT, T_DOUBLE, T_STRING, T_BOOL, T_CHAR,
    T_ID, T_NUM, T_NUM_FLOAT, T_IF, T_ELSE, T_RETURN,
    T_ASSIGN, T_PLUS, T_MINUS, T_MUL, T_DIV,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE,
    T_SEMICOLON, T_GT, T_LT, T_EQ, T_NEQ, T_EOF,
    T_WHILE, T_FOR, T_AND, T_OR
};

struct Token {
    TokenType type;
    string value;
    int lineNum;
    int colNum;
    Token(TokenType tt, string v) {
        type = tt;
        value = v;
    }
};

string tokenTypeToString(TokenType type) {
    switch(type) {
        case T_INT: return "int";
        case T_FLOAT: return "float";
        case T_DOUBLE: return "double";
        case T_STRING: return "string";
        case T_BOOL: return "bool";
        case T_CHAR: return "char";
        case T_ID: return "identifier";
        case T_NUM: return "number";
        case T_NUM_FLOAT: return "float number";
        case T_ASSIGN: return "assignment (=)";
        case T_PLUS: return "plus (+)";
        case T_MINUS: return "minus (-)";
        case T_MUL: return "multiply (*)";
        case T_DIV: return "divide (/)";
        case T_LPAREN: return "left parenthesis (()";
        case T_RPAREN: return "right parenthesis ())";
        case T_LBRACE: return "left brace ({)";
        case T_RBRACE: return "right brace (})";
        case T_SEMICOLON: return "semicolon (;)";
        case T_GT: return "greater than (>)";
        case T_LT: return "less than (<)";
        case T_EQ: return "equal (==)";
        case T_NEQ: return "not equal (!=)";
        case T_IF: return "if";
        case T_ELSE: return "else";
        case T_RETURN: return "return";
        case T_WHILE: return "while";
        case T_FOR: return "for";
        case T_AND: return "logical and (&&)";
        case T_OR: return "logical or (||)";
        case T_EOF: return "end of file";
        default: return "Unknown Token";
    }
}

struct TACInstruction {
    string op;        // Operator
    string arg1;      // First argument
    string arg2;      // Second argument
    string result;    // Result
    
    TACInstruction(string op, string arg1, string arg2, string result)
        : op(op), arg1(arg1), arg2(arg2), result(result) {}
        
    string toString() const {
        if (op == "copy")
            return result + " = " + arg1;
        else if (op == "label")
            return arg1 + ":";
        else if (op == "goto")
            return "goto " + arg1;
        else if (op == "if")
            return "if " + arg1 + " goto " + result;
        else if (op == "ifFalse")
            return "ifFalse " + arg1 + " goto " + result;
        else
            return result + " = " + arg1 + " " + op + " " + arg2;
    }
};

class TACGenerator {
private:
    vector<TACInstruction> instructions;
    int tempCount;
    int labelCount;
    
    string newTemp() {
        return "t" + to_string(tempCount++);
    }
    
    string newLabel() {
        return "L" + to_string(labelCount++);
    }

public:
    TACGenerator() : tempCount(0), labelCount(0) {}
    
    string generateExpression(Token& token) {
        if (token.type == T_NUM || token.type == T_NUM_FLOAT) {
            return token.value;
        } else if (token.type == T_ID) {
            return token.value;
        }
        return "";
    }
    
    string generateBinaryOp(string op, string left, string right) {
        string temp = newTemp();
        instructions.push_back(TACInstruction(op, left, right, temp));
        return temp;
    }
    
    void generateAssignment(string id, string expr) {
        instructions.push_back(TACInstruction("copy", expr, "", id));
    }
    
    string generateRelationalOp(string op, string left, string right) {
        string temp = newTemp();
        instructions.push_back(TACInstruction(op, left, right, temp));
        return temp;
    }
    
    pair<string, string> generateIfHeader(string condition) {
        string trueLabel = newLabel();
        string falseLabel = newLabel();
        instructions.push_back(TACInstruction("if", condition, "", trueLabel));
        instructions.push_back(TACInstruction("goto", falseLabel, "", ""));
        instructions.push_back(TACInstruction("label", trueLabel, "", ""));
        return {trueLabel, falseLabel};
    }
    
    void generateIfEnd(string falseLabel) {
        instructions.push_back(TACInstruction("label", falseLabel, "", ""));
    }
    
    pair<string, string> generateWhileHeader() {
        string startLabel = newLabel();
        string endLabel = newLabel();
        instructions.push_back(TACInstruction("label", startLabel, "", ""));
        return {startLabel, endLabel};
    }
    
    void generateWhileEnd(string startLabel, string endLabel) {
        instructions.push_back(TACInstruction("goto", startLabel, "", ""));
        instructions.push_back(TACInstruction("label", endLabel, "", ""));
    }
    
    void generateWhileCondition(string condition, string endLabel) {
        instructions.push_back(TACInstruction("ifFalse", condition, "", endLabel));
    }
    
    void printTAC() const {
        cout << "\n=== Three Address Code ===\n";
        for (size_t i = 0; i < instructions.size(); i++) {
            cout << i << ": " << instructions[i].toString() << endl;
        }
        cout << "========================\n";
    }
};

struct Symbol {
    string name;
    TokenType type;
    int scopeLevel;
    bool initialized;
    
    Symbol(string n = "", TokenType t = T_ID, int s = 0, bool i = false) 
        : name(n), type(t), scopeLevel(s), initialized(i) {}
};

class SymbolTable {
private:
    vector<map<string, Symbol>> scopes;
    int currentScope;
    
public:
    SymbolTable() {
        // Initialize global scope
        scopes.push_back(map<string, Symbol>());
        currentScope = 0;
    }

    void enterScope() {
        scopes.push_back(map<string, Symbol>());
        currentScope++;
    }

    void exitScope() {
        if (currentScope > 0) {
            scopes.pop_back();
            currentScope--;
        }
    }

    bool declare(const string& name, TokenType type) {
        // Check if symbol already exists in current scope
        if (scopes[currentScope].find(name) != scopes[currentScope].end()) {
            return false; // Variable already declared in current scope
        }
        
        Symbol symbol(name, type, currentScope, false);
        scopes[currentScope][name] = symbol;
        return true;
    }

    bool initialize(const string& name) {
        // Search for symbol from current scope up to global scope
        for (int i = currentScope; i >= 0; i--) {
            auto it = scopes[i].find(name);
            if (it != scopes[i].end()) {
                it->second.initialized = true;
                return true;
            }
        }
        return false; // Symbol not found
    }

    Symbol* lookup(const string& name) {
        // Search for symbol from current scope up to global scope
        for (int i = currentScope; i >= 0; i--) {
            auto it = scopes[i].find(name);
            if (it != scopes[i].end()) {
                return &(it->second);
            }
        }
        return nullptr; // Symbol not found
    }

    void printSymbolTable() const {
        cout << "\n=== Symbol Table ===\n";
        for (size_t i = 0; i < scopes.size(); i++) {
            cout << "Scope Level " << i << ":\n";
            for (const auto& symbol : scopes[i]) {
                cout << "  Name: " << symbol.second.name 
                     << ", Type: " << tokenTypeToString(symbol.second.type)
                     << ", Initialized: " << (symbol.second.initialized ? "yes" : "no")
                     << "\n";
            }
        }
        cout << "==================\n";
    }
};

class Lexer {
private:
    string src;
    size_t pos;
    int lineNum;
    int colNum;

    void skipComment() {
        pos += 2;
        colNum += 2;
        
        while (pos < src.size() && src[pos] != '\n') {
            pos++;
            colNum++;
        }
    }

public:
    Lexer(const string &src) {
        this->src = src;
        this->pos = 0;
        this->lineNum = 1;
        this->colNum = 1;
    }

    string consumeNumber() {
        size_t start = pos;
        while (pos < src.size() && isdigit(src[pos])) {
            pos++;
            colNum++;
        }
        return src.substr(start, pos - start);
    }

    string consumeFloatNumber() {
        size_t start = pos;
        while (pos < src.size() && (isdigit(src[pos]) || src[pos] == '.')) {
            pos++;
            colNum++;
        }
        return src.substr(start, pos - start);
    }

    string consumeWord() {
        size_t start = pos;
        while (pos < src.size() && (isalnum(src[pos]) || src[pos] == '_')) {
            pos++;
            colNum++;
        }
        return src.substr(start, pos - start);
    }

    string consumeCharacter() {
        size_t start = pos;
        pos++; // skip opening quote
        colNum++;
        while (pos < src.size() && src[pos] != '\'') {
            pos++;
            colNum++;
        }
        if (pos < src.size()) {
            pos++; // skip closing quote
            colNum++;
        }
        return src.substr(start + 1, pos - start - 2);
    }

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < src.size()) {
            char current = src[pos];

            if (current == '/' && pos + 1 < src.size() && src[pos + 1] == '/') {
                skipComment();
                continue;
            }

            if (isspace(current)) {
                if (current == '\n') {
                    lineNum++;
                    colNum = 1;
                } else {
                    colNum++;
                }
                pos++;
                continue;
            }

            if (isdigit(current)) {
                if (src[pos + 1] == '.') {
                    tokens.push_back(Token(T_NUM_FLOAT, consumeFloatNumber()));
                } else {
                    tokens.push_back(Token(T_NUM, consumeNumber()));
                }
                tokens.back().lineNum = lineNum;
                tokens.back().colNum = colNum;
                continue;
            }

            if (isalpha(current)) {
                string word = consumeWord();
                TokenType type = T_ID;
                if (word == "int") type = T_INT;
                else if (word == "float") type = T_FLOAT;
                else if (word == "double") type = T_DOUBLE;
                else if (word == "string") type = T_STRING;
                else if (word == "bool") type = T_BOOL;
                else if (word == "char") type = T_CHAR;
                else if (word == "if") type = T_IF;
                else if (word == "Agar") type = T_IF;
                else if (word == "else") type = T_ELSE;
                else if (word == "Magar") type = T_ELSE;
                else if (word == "return") type = T_RETURN;
                else if (word == "while") type = T_WHILE;
                else if (word == "for") type = T_FOR;

                tokens.push_back(Token(type, word));
                tokens.back().lineNum = lineNum;
                tokens.back().colNum = colNum;
                continue;
            }

            Token token(T_EOF, "");
            switch (current) {
                case '=':
                    if (src[pos + 1] == '=') {
                        token = Token(T_EQ, "==");
                        pos++;
                        colNum++;
                    } else {
                        token = Token(T_ASSIGN, "=");
                    }
                    break;
                case '+': token = Token(T_PLUS, "+"); break;
                case '-': token = Token(T_MINUS, "-"); break;
                case '*': token = Token(T_MUL, "*"); break;
                case '/': token = Token(T_DIV, "/"); break;
                case '(': token = Token(T_LPAREN, "("); break;
                case ')': token = Token(T_RPAREN, ")"); break;
                case '{': token = Token(T_LBRACE, "{"); break;
                case '}': token = Token(T_RBRACE, "}"); break;
                case ';': token = Token(T_SEMICOLON, ";"); break;
                case '>': token = Token(T_GT, ">"); break;
                case '<': token = Token(T_LT, "<"); break;
                case '&':
                    if (src[pos + 1] == '&') {
                        token = Token(T_AND, "&&");
                        pos++;
                        colNum++;
                    } else {
                        cout << "Unexpected character: " << current << " at line " << lineNum << ", col " << colNum << endl;
                        exit(1);
                    }
                    break;
                case '|':
                    if (src[pos + 1] == '|') {
                        token = Token(T_OR, "||");
                        pos++;
                        colNum++;
                    } else {
                        cout << "Unexpected character: " << current << " at line " << lineNum << ", col " << colNum << endl;
                        exit(1);
                    }
                    break;
                case '!':
                    if (src[pos + 1] == '=') {
                        token = Token(T_NEQ, "!=");
                        pos++;
                        colNum++;
                    } else {
                        cout << "Unexpected character: " << current << " at line " << lineNum << ", col " << colNum << endl;
                        exit(1);
                    }
                    break;
                default:
                    cout << "Unexpected character: " << current << " at line " << lineNum << ", col " << colNum << endl;
                    exit(1);
            }
            
            token.lineNum = lineNum;
            token.colNum = colNum;
            tokens.push_back(token);

            pos++;
            colNum++;
        }

        Token eofToken(T_EOF, "");
        eofToken.lineNum = lineNum;
        eofToken.colNum = colNum;
        tokens.push_back(eofToken);
        return tokens;
    }
};

class Parser {
private:
    TACGenerator tacGen;
    vector<Token> tokens;
    size_t pos = 0;
    SymbolTable symbolTable;

public:
    explicit Parser(const vector<Token>& tokens) : tokens(tokens) {}

    class ParseError : public runtime_error {
    public:
        explicit ParseError(const string& msg) : runtime_error(msg) {}
    };

    void parseStatement() {
        switch (tokens[pos].type) {
            case T_INT: case T_FLOAT: case T_DOUBLE: case T_STRING:
            case T_BOOL: case T_CHAR:
                parseDeclaration();
                break;
            case T_ID:
                parseAssignment();
                break;
            case T_IF: case T_WHILE:
                parseIfOrWhileStatement();
                break;
            case T_RETURN:
                parseReturnStatement();
                break;
            case T_LBRACE:
                parseBlock();
                break;
            default:
                error("Unexpected token: " + tokens[pos].value);
        }
    }

    void parseIfOrWhileStatement() {
        TokenType type = tokens[pos].type;
        expect(type);  // T_IF or T_WHILE
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();

        if (type == T_IF && tokens[pos].type == T_ELSE) {
            expect(T_ELSE);
            parseStatement();
        }
    }

    void parseBlock() {
        expect(T_LBRACE);
        symbolTable.enterScope();
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF) {
            parseStatement();
        }
        expect(T_RBRACE);
        symbolTable.exitScope();
    }

    void parseDeclaration() {
        TokenType varType = tokens[pos++].type;
        string varName = tokens[pos].value;

        if (!symbolTable.declare(varName, varType)) {
            error("Variable '" + varName + "' already declared in current scope");
        }

        expect(T_ID);

        if (tokens[pos].type == T_ASSIGN) {
            expect(T_ASSIGN);
            parseExpression();
            symbolTable.initialize(varName);
        }

        expect(T_SEMICOLON);
    }

    void parseAssignment() {
        string varName = tokens[pos++].value;

        if (!symbolTable.lookup(varName)) {
            error("Variable '" + varName + "' not declared");
        }

        expect(T_ASSIGN);
        tacGen.generateAssignment(varName, parseExpression());
        symbolTable.initialize(varName);
        expect(T_SEMICOLON);
    }

    void parseReturnStatement() {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    string parseExpression() { return parseLogicalOr(); }

    string parseLogicalOr() { return parseBinaryOp(&Parser::parseLogicalAnd, T_OR, "||"); }

    string parseLogicalAnd() { return parseBinaryOp(&Parser::parseEquality, T_AND, "&&"); }

    string parseEquality() {
        return parseBinaryOp(&Parser::parseRelational, {T_EQ, T_NEQ}, {"==", "!="});
    }

    string parseRelational() {
        return parseBinaryOp(&Parser::parseTerm, {T_GT, T_LT}, {">", "<"});
    }

    string parseTerm() {
        return parseBinaryOp(&Parser::parseFactor, {T_PLUS, T_MINUS}, {"+", "-"});
    }

    string parseFactor() {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_NUM_FLOAT ||
            tokens[pos].type == T_ID || tokens[pos].type == T_CHAR) {
            return tokens[pos++].value;
        } else if (tokens[pos].type == T_LPAREN) {
            expect(T_LPAREN);
            string result = parseExpression();
            expect(T_RPAREN);
            return result;
        } else {
            error("Unexpected token in factor");
            return "";
        }
    }

    void parseProgram() {
        while (tokens[pos].type != T_EOF) {
            parseStatement();
        }
        symbolTable.printSymbolTable();
        tacGen.printTAC();
        cout << "Parsing completed successfully! No Syntax Error\n";
    }

    void expect(TokenType type) {
        if (tokens[pos].type == type) {
            pos++;
        } else {
            error("Expected " + tokenTypeToString(type) +
                  " but found '" + tokens[pos].value + "'");
        }
    }

private:
    template <typename Fn>
    string parseBinaryOp(Fn next, TokenType opType, const string& opSymbol) {
        string left = (this->*next)();
        while (tokens[pos].type == opType) {
            pos++;
            string right = (this->*next)();
            left = tacGen.generateBinaryOp(opSymbol, left, right);
        }
        return left;
    }

    template <typename Fn>
    string parseBinaryOp(Fn next, const vector<TokenType>& opTypes, const vector<string>& opSymbols) {
        string left = (this->*next)();
        while (auto it = find(opTypes.begin(), opTypes.end(), tokens[pos].type);
               it != opTypes.end()) {
            string opSymbol = opSymbols[it - opTypes.begin()];
            pos++;
            string right = (this->*next)();
            left = tacGen.generateBinaryOp(opSymbol, left, right);
        }
        return left;
    }

    [[noreturn]] void error(const string& message) {
        throw ParseError("Error at line " + to_string(tokens[pos].lineNum) +
                         ", column " + to_string(tokens[pos].colNum) + ": " + message);
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <source_file>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error: Cannot open file " << argv[1] << endl;
        return 1;
    }

    string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    Lexer lexer(code);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);

    try {
        parser.parseProgram();
    } catch (const exception& e) {
        cerr << "Error during parsing: " << e.what() << endl;
        return 1;
    }

    return 0;
}
