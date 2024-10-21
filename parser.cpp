#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

enum TokenType {
    T_INT, T_FLOAT, T_DOUBLE, T_STRING, T_BOOL, T_CHAR,
    T_ID, T_NUM, T_IF, T_ELSE, T_WHILE, T_FOR, T_BREAK, T_CONTINUE, T_RETURN, 
    T_ASSIGN, T_PLUS, T_MINUS, T_MUL, T_DIV, 
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE,  
    T_SEMICOLON, T_GT, T_EOF, 
};

struct Token {
    TokenType type;
    string value;
    int line; // Add a line number to the Token struct
};

class Lexer {

private:
    string src;
    size_t pos;
    int line;  // Track the current line number

public:
    Lexer(const string &src) {
        this->src = src;
        this->pos = 0;
        this->line = 1;  // Initialize line number
    }

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < src.size()) {
            char current = src[pos];
            
            if (isspace(current)) {
                if (current == '\n') line++;  // Increment line on newline
                pos++;
                continue;
            }
            if (isdigit(current)) {
                tokens.push_back(Token{T_NUM, consumeNumber(), line});
                continue;
            }
            if (isalpha(current)) {
        string word = consumeWord();
        
        if (word == "int") tokens.push_back(Token{T_INT, word});
        else if (word == "float") tokens.push_back(Token{T_FLOAT, word});
        else if (word == "double") tokens.push_back(Token{T_DOUBLE, word});
        else if (word == "string") tokens.push_back(Token{T_STRING, word});
        else if (word == "bool") tokens.push_back(Token{T_BOOL, word});
        else if (word == "char") tokens.push_back(Token{T_CHAR, word});
        else if (word == "anj") tokens.push_back(Token{T_IF, word});   // Replaced 'if' with 'anj'
        else if (word == "unj") tokens.push_back(Token{T_ELSE, word}); // Replaced 'else' with 'unj'
        else if (word == "while") tokens.push_back(Token{T_WHILE, word});  
        else if (word == "for") tokens.push_back(Token{T_FOR, word});      
        else if (word == "break") tokens.push_back(Token{T_BREAK, word});  
        else if (word == "continue") tokens.push_back(Token{T_CONTINUE, word});  
        else if (word == "return") tokens.push_back(Token{T_RETURN, word});
        else if (word == "if") {
            cout << "Keyword error: 'if' is not valid, did you mean 'anj'?" << endl;
            exit(1);
        }
        else if (word == "else") {
            cout << "Keyword error: 'else' is not valid, did you mean 'unj'?" << endl;
            exit(1);
        }
        else {
            tokens.push_back(Token{T_ID, word});
        }
        continue;
    }

            
            switch (current) {
                case '=': tokens.push_back(Token{T_ASSIGN, "=", line}); break;
                case '+': tokens.push_back(Token{T_PLUS, "+", line}); break;
                case '-': tokens.push_back(Token{T_MINUS, "-", line}); break;
                case '*': tokens.push_back(Token{T_MUL, "*", line}); break;
                case '/': tokens.push_back(Token{T_DIV, "/", line}); break;
                case '(': tokens.push_back(Token{T_LPAREN, "(", line}); break;
                case ')': tokens.push_back(Token{T_RPAREN, ")", line}); break;
                case '{': tokens.push_back(Token{T_LBRACE, "{", line}); break;  
                case '}': tokens.push_back(Token{T_RBRACE, "}", line}); break;  
                case ';': tokens.push_back(Token{T_SEMICOLON, ";", line}); break;
                case '>': tokens.push_back(Token{T_GT, ">", line}); break;
                default: 
                    cout << "Error on line " << line << ": Unexpected character '" << current << "'" << endl;
                    exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", line});
        return tokens;
    }

    string consumeNumber() {
        size_t start = pos;
        while (pos < src.size() && isdigit(src[pos])) pos++;
        return src.substr(start, pos - start);
    }

    string consumeWord() {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos])) pos++;
        return src.substr(start, pos - start);
    }
};

class Parser {

public:
    Parser(const vector<Token> &tokens) {
        this->tokens = tokens;
        this->pos = 0;
    }

    void parseProgram() {
        if (tokens[pos].type == T_INT && tokens[pos + 1].type == T_ID && tokens[pos + 2].type == T_LPAREN) {
            parseFunction();
        } else {
            while (tokens[pos].type != T_EOF) {
                parseStatement();
            }
        }
        cout << "Parsing completed successfully! No Syntax Error" << endl;
    }


private:
    vector<Token> tokens;
    size_t pos;

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
            reportError("unexpected token", tokens[pos]);
        }
    }

    void parseBlock() {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF) {
            parseStatement();
        }
        expect(T_RBRACE);
    }

    void parseDeclaration() {
    if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || 
        tokens[pos].type == T_DOUBLE || tokens[pos].type == T_STRING || 
        tokens[pos].type == T_BOOL || tokens[pos].type == T_CHAR) {
        pos++; 
        expect(T_ID);
        expect(T_SEMICOLON);
    } else {
        cout << "Syntax error: expected a valid data type" << endl;
        exit(1);
    }
}


    void parseAssignment() {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseIfStatement() {
        expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (tokens[pos].type == T_ELSE) {
            expect(T_ELSE);
            parseStatement();
        }
    }

    void parseReturnStatement() {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression() {
        parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS) {
            pos++;
            parseTerm();
        }
        if (tokens[pos].type == T_GT) {
            pos++;
            parseExpression();
        }
    }
    void parseFunction() {
        expect(T_INT);      // Expect return type (for now only 'int')
        expect(T_ID);       // Expect function name (like 'main')
        expect(T_LPAREN);   // Expect opening parenthesis '('
        expect(T_RPAREN);   // Expect closing parenthesis ')'
        parseBlock();       // Expect the function body in curly braces
    }


    void parseTerm() {
        parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV) {
            pos++;
            parseFactor();
        }
    }

    void parseFactor() {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID) {
            pos++;
        } else if (tokens[pos].type == T_LPAREN) {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
        } else {
            cout << "Syntax error: unexpected token " << tokens[pos].value << " at line: " << pos << endl;
            exit(1);
        }
    }


    void expect(TokenType type) {
        if (tokens[pos].type == type) {
            pos++;
        } else {
            reportError("expected different token", tokens[pos]);
        }
    }

    void reportError(const string &message, const Token &token) {
        cout << "Syntax error on line " << token.line << ": " << message << " '" << token.value << "'" << endl;
        exit(1);
    }
};


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }
    string filename = argv[1];
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file: " << filename << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string input = buffer.str();
    cout << "Input string:\n" << input << endl;
    Lexer lexer(input);
    vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    parser.parseProgram();
    return 0;
}
