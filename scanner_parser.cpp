#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <ctype.h>
#include <vector>
#include <filesystem>
#include <exception>
#include "parser.hpp"
using namespace std;

static ios_base::Init iostream_initializer;

vector<string> token_types;
vector<string> token_values;

// === === === SCANNER === === ===

map<int, string> tokens_dict = {
    {13,"Newline"}, // newline
    {10,"Newline"}, // newline
    {47,"Divide"}, // "/"
    {58,"Colon"}, // ":"
    {61,"Equal"}, // "="
    {42,"Multiply"}, // "*"
    {45,"Minus"}, // "-"
    {43,"Plus"}, // "+"
    {40,"LeftParen"}, // "("
    {41,"RightParen"}, // ")"
    {59,"Semicolon"}, // ";"
    {44,"Comma"}, // ","
    {34,"String"}, // "
    {46,"Period"}, // "."
    {60,"LessThan"}, // "<"
    {62,"GreaterThan"}, // ">"
    {33,"Excl"} // "!"
};

map<string,string> key_identifiers = {
    {"IF","If"},{"SQRT","Sqrt"},{"PRINT","Print"},{"ELSE","Else"},{"ENDIF","Endif"},{"AND","And"},{"OR","Or"},{"NOT","Not"}
};

void release(string &state, string &token, ofstream &outputFile){ //Function to release state and token
    string line;
    if(state == "Identifier"){
        if(key_identifiers.find(token) != key_identifiers.end()){
            state = key_identifiers[token];
        }
    }
    if(state == "Error"){
        outputFile << token << "\n";
        outputFile << state << "\n";
        token.clear();
        state.clear();
    }
    else if (!token.empty()) {
        line = state + " " + token + "\n";
        token_types.push_back(state);
        token_values.push_back(token);
        cout << line;
        outputFile << line;
        line.clear();
        token.clear();
        state.clear();
    }
}
void gettoken(fstream &input_file, string &outputName){ //Gets the next token from the input
    string state;
    string map;
    string token;
    char in_char;
    ofstream outputFile(outputName);

    while (input_file.get(in_char)) {
        map = tokens_dict[(int)in_char];
        if (map == "Newline"){
            in_char = ' ';
        }

        if(state != "Skip"){ //checks for state
            if((map == "Period" && state != "Number") || state == "Error"){
                if(token == ""){
                    token += "Lexical Error Illegal character/character sequence";
                    state = "Error";
                }
                else if (in_char == ' ' || map =="Newline"){
                    release(state,token,outputFile);
                }
            }
            else if(map == "String" || state == "String"){ //String
                if((int)in_char == 34 && state != "String"){
                    release(state,token,outputFile);
                    state = map;
                    token += in_char;
                }
                else if(state == "String" && map == "String"){
                    token += in_char;
                    release(state,token,outputFile);
                }
                else if(map == "Newline"){
                    state = "Error";
                    token = "Lexical Error: Unterminated string";
                    release(state,token,outputFile);
                }
                else{
                    token += in_char;
                }
            }
            else if(state == "Excl"){ //!
                if(in_char != '='){
                    state = "Error";
                    token = "Lexical Error: Illegal character/character sequence";
                    release(state,token,outputFile);
                }
                else{
                    state = "NotEqual";
                    token = "!=";
                    release(state,token,outputFile);
                }
            }
            else if (map == "Divide"){ //Divide
                if (state == "Divide"){
                    state = "Skip";
                    token.clear();
                }
                else {
                    release(state,token,outputFile);
                    state = map;
                    token += in_char;
                }
            }
            else if(state == "Number" && (in_char == '.' || in_char == 'e' || in_char == 'E')){ //decimal
                if((state == "Number") && (token.back() == '+' || token.back() == '-') && !isdigit(in_char)){ //non-digit directly after + or -
                    token += in_char;
                    token.clear();
                    token += "Lexical Error Invalid number format";
                    state = "Error";    
                    release(state,token,outputFile);
                }
                else{
                    token += in_char;
                }
            }
            else if(state == "Number" && (in_char == '+' || in_char == '-')){
                if((state == "Number") && (token.back() == 'e' || token.back() == 'E') && !isdigit(in_char)){ //non-digit directly after + or -
                    token += in_char;
                }
                else{
                    release(state,token,outputFile);
                    state = map;
                    token += in_char;
                }
            }
            else if(state == "Number" && isalpha(in_char) && (token.back() == 'e' || token.back() == 'E')){ //non-digit after e / E
                token += in_char;
                token.clear();
                token += "Lexical Error Invalid number format";
                state = "Error";
                release(state,token,outputFile);
            }
            else if((isalpha(in_char) || in_char == '_')){ //Identifier
                if(state != "Identifier"){
                    release(state,token,outputFile);
                }
                state = "Identifier";
                token += in_char;
            }
            else if((isalnum(in_char) || in_char == '_') && state == "Identifier"){ //Identifier but pwede na num dahil di na start
                state = "Identifier";
                token += in_char;
            }
            else if(isdigit(in_char)){ //Number
                if(state != "Number"){
                    release(state,token,outputFile);
                    state = "Number";
                }
                token += in_char;
            }
            else if(map == "Multiply"){ //Multiply
                if(map != state){
                    release(state,token,outputFile);
                }
                if(state == map){
                    state = "Raise";
                    token += in_char;
                }
                else{
                    state = map;
                    token += in_char;
                }
            }
            else if(map == "Colon"){ //Colon
                if(state != map && state != ""){
                    release(state,token,outputFile);
                }
                state = map;
                token += in_char;
            }
            else if(map == "GreaterThan"){ //Greater Than
                if(state != map && state != ""){
                    release(state,token,outputFile);
                }
                state = map;
                token += in_char;
            }
            else if(map == "LessThan"){ //Less Than
                if(state != map && state != ""){
                    release(state,token,outputFile);
                }
                state = map;
                token += in_char;
            }
            else if(map == "Equal"){ //Equals
                if(state == "Colon"){
                    state = "Assign";
                    token += in_char;
                    release(state,token,outputFile);
                }
                else if(state == "GreaterThan"){
                    state = "GTEqual";
                    token += in_char;
                    release(state,token,outputFile);
                }
                else if(state == "LessThan"){
                    state = "LTEqual";
                    token += in_char;
                    release(state,token,outputFile);
                }
                else{
                    release(state,token,outputFile);
                    token += in_char;
                    state = map;
                }
            }
            else if(map == "Minus"){ //Minus
                if(state != map && (state != "")){
                    release(state,token,outputFile);
                    state = map;
                    token += in_char;
                }
                else if(state == "Number"){
                    token += in_char;
                }
                else if(state == "Minus"){
                    release(state,token,outputFile);
                    state = map;
                    token += in_char;
                }
                else{
                    state = map;
                    token += in_char;
                }
            }
            else if(map == "Plus"){ //Plus
                if(state != map && (state != "")){
                    release(state,token,outputFile);
                    state = map;
                    token += in_char;
                }
                else if(state == "Number"){
                    token += in_char;
                }
                else if(state == "Plus"){
                    release(state,token,outputFile);
                    state = map;
                    token += in_char;
                }
                else{
                    state = map;
                    token += in_char;
                }
            }
            else if(map == "LeftParen"){ //LeftParen
                release(state,token,outputFile);
                state = map;
                token += in_char;
            }
            else if(map == "RightParen"){ //RightParen
                release(state,token,outputFile);
                state = map;
                token += in_char;
            }
            else if(map == "Semicolon"){ //Semicolon
                release(state,token,outputFile);
                state = map;
                token += in_char;
            }
            else if(map == "Comma"){ //Semicolon
                release(state,token,outputFile);
                state = map;
                token += in_char;
            }
            else if((in_char == ' ' || map == "Newline") && state == "Number" && token.back() == '.' ){ //end immediately after decimal
                token.pop_back();
                release(state,token,outputFile);
                token += "Lexical Error Invalid number format";
                state = "Error";
            }
            else if(map != state){ //Release
                release(state,token,outputFile);
                state = map;
            }
        }

        else if(map == "Newline"){ //resets skip when newline
            state = map;
        }

        cout << "in_char: " << in_char << " ASCII: " << (int)in_char << " Map: " << map;
        cout << " State: " << state << " Token: " << token << endl;
    }

    release(state,token,outputFile);
    outputFile << "EndofFile";
    outputFile.close();
}

// === === === PARSER === === ===

int token_index = 0;
int curr_token_index = 0;
string curr_token_type = "";
bool epsilon = false;
bool print_p = false;
bool print_t = print_p;
bool print_e = print_p;
ofstream parser_output_file;
int open_ifs = 0;
bool nested_ifs = false;

// Handles token access

void get_next_token() {
    curr_token_index++;
    curr_token_type = token_types[curr_token_index];
    if (print_t) cout << "next! curr token: " << curr_token_type << endl;
}
void get_prev_token() {
    curr_token_index--;
    curr_token_type = token_types[curr_token_index];
    if (print_t) cout << "prev! curr token: " << curr_token_type << endl;
}

// Handles errors and stops the parsing the current file, if needed

struct ParseException : exception {
    string message;
    ParseException(string m) : message(m) {}
    const char* what() const noexcept override { return message.c_str(); }
};

void parse_error(string token_type, string token_value) {
    // cout << "check " << curr_token_type << " vs " << token_type << endl;
    if (curr_token_type != token_type) {
        parser_output_file << "Parse Error: " << token_type << " expected.";
        throw ParseException("Parse Error: " + token_type + " expected.");
    } else {
        cout << "terminal: " << token_value << endl;
    }
}

// The following functions simulate the given grammar of SimpCalc

void Prg() {
    if (print_p) cout << "Prg" << endl;
    Blk();
    EndOfFile();
}

void EndOfFile() {
    if (print_p) cout << "EndOfFile" << endl;
}

void Blk() {
    if (print_p) cout << "Blk" << endl;
    if (
        curr_token_type == "Identifier" ||
        curr_token_type == "Print" ||
        curr_token_type == "If"
    ) {
        Stm();
        Blk();
    } else {
        if (print_e) cout << "Blk epsilon" << endl;
        epsilon = false;
    }
}

void Stm() {
    if (print_p) cout << "Stm" << endl;
    if (
        curr_token_type == "Identifier"
    ) {
        cout << "terminal: " << token_values[curr_token_index] << endl;
        get_next_token();
        parse_error("Assign", ":=");
        Exp();
        // get_next_token();
        parse_error("Semicolon", ";");
        parser_output_file << "Assignment Statement Recognized" << endl;
        cout << "Assignment Statement Recognized" << endl;
        get_next_token();
    } else if (
        curr_token_type == "Print"
    ) {
        cout << "terminal: PRINT" << endl;
        get_next_token();
        parse_error("LeftParen", "(");
        Arg();
        Argfollow();
        parse_error("RightParen", "PR)");
        get_next_token();
        parse_error("Semicolon", ";");
        parser_output_file << "Print Statement Recognized" << endl;
        cout << "Print Statement Recognized" << endl;
        // get_next_token();
    } else if (
        curr_token_type == "If"
    ) {
        open_ifs++;
        cout << "open ifs: " << open_ifs << endl;
        if (open_ifs > 1) {
            nested_ifs = true;
            cout << "ON NEST" << endl;
        }
        parser_output_file << "If Statement Begins" << endl;
        cout << "If Statement Begins" << endl;
        cout << "terminal: IF" << endl;
        Cnd();
        parse_error("Colon", ":");
        get_next_token();
        Blk();
        Iffollow();
        get_next_token();
    } else {
        parser_output_file << "Parse error: Invalid Statement";
    }
}

void Exp() {
    if (print_p) cout << "Exp" << endl;
    Trm();
    Trmfollow();
}

void Trm() {
    if (print_p) cout << "Trm" << endl;
    Fac();
    Facfollow();
}

void Trmfollow() {
    if (print_p) cout << "Trmfollow" << endl;
    if (
        curr_token_type == "Plus"
    ) {
        cout << "terminal: +" << endl;
        Trm();
        Trmfollow();
    } else if (
        curr_token_type == "Minus"
    ) {
        cout << "terminal: -" << endl;
        Trm();
        Trmfollow();
    } else {
        if (print_e) cout << "TF epsilon" << endl;
        epsilon = true;
    }
}

void Fac() {
    if (print_p) cout << "Fac" << endl;
    Lit();
    Litfollow();
}

void Facfollow() {
    if (print_p) cout << "Facfollow" << endl;
    if (
        curr_token_type == "Multiply"
    ) {
        cout << "terminal: *" << endl;
        Fac();
        Facfollow();
    } else if (
        curr_token_type == "Divide"
    ) {
        cout << "terminal: /" << endl;
        Fac();
        Facfollow();
    } else {
        if (print_e) cout << "FF epsilon" << endl;
        epsilon = true;
    }

}

void Lit() {
    if (print_p) cout << "Lit" << endl;
    get_next_token();
    if (
        curr_token_type == "Minus"
    ) {
        cout << "terminal: -" << endl;
        get_next_token();
        Val();
    } else {
        Val();
    }
}

void Litfollow() {
    if (print_p) cout << "Litfollow" << endl;
    if (
        curr_token_type == "Raise"
    ) {
        cout << "terminal: **" << endl;
        Lit();
        Litfollow();
    } else {
        if (print_e) cout << "LF epsilon" << endl;
        epsilon = true;
    }
}

void Val() {
    if (print_p) cout << "Val" << endl;
    if (
        curr_token_type == "Identifier"
    ) {
        cout << "terminal: " << token_values[curr_token_index] << endl;
        get_next_token();
    } else if (
        curr_token_type == "Number"
    ) {
        cout << "terminal: " << token_values[curr_token_index] << endl;
        get_next_token();
    } else if (
        curr_token_type == "Sqrt"
    ) {
        cout << "terminal: SQRT" << endl;
        get_next_token();
        parse_error("LeftParen", "(");
        Exp();
        parse_error("RightParen", "SQ)");
        get_next_token();
    } else if (
        curr_token_type == "LeftParen"
    ) {
        cout << "terminal: (" << endl;
        Exp();
        parse_error("RightParen", "LP)");
        get_next_token();
    }
}

void Cnd() {
    if (print_p) cout << "Cnd" << endl;
    Exp();
    Rel();
    Exp();
}

void Rel() {
    if (print_p) cout << "Rel" << endl;
    // get_next_token();
    if (
        curr_token_type == "LessThan"
    ) {
        cout << "terminal: <" << endl;
    } else if (
        curr_token_type == "Equal"
    ) {
        cout << "terminal: =" << endl;
    } else if (
        curr_token_type == "GreaterThan"
    ) {
        cout << "terminal: >" << endl;
    } else if (
        curr_token_type == "GTEqual"
    ) {
        cout << "terminal: >=" << endl;
    } else if (
        curr_token_type == "NotEqual"
    ) {
        cout << "terminal: !=" << endl;
    } else if (
        curr_token_type == "LTEqual"
    ) {
        cout << "terminal: <=" << endl;
    } else {
        parser_output_file << "Parse Error: Missing relational operator.";
        throw ParseException("Parse Error: Missing relational operator.");
    }
}

void Iffollow() {
    if (print_p) cout << "Iffollow " << curr_token_type << endl;
    if ((open_ifs > 1 && nested_ifs) || !nested_ifs) {
        cout << "nested_ifs" << endl;
        get_next_token();
    }
    if (
        curr_token_type == "Endif"
    ) {
        cout << "terminal: ENDIF" << endl;
        get_next_token();
        parse_error("Semicolon", ";");
        parser_output_file << "If Statement Ends" << endl;
        cout << "If Statement Ends" << endl;
    } else if (
        curr_token_type == "Else"
    ) {
        cout << "terminal: ELSE" << endl;
        get_next_token();
        Blk();
        get_next_token();
        parse_error("Endif", "ENDIF");
        get_next_token();
        parse_error("Semicolon", ";");
        parser_output_file << "If Statement Ends" << endl;
        cout << "If Statement Ends" << endl;
    } else {
        parser_output_file << "Parse Error: Incomplete if Statement.";
        throw ParseException("Parse Error: Incomplete if Statement.");
    }
    open_ifs--;
    if (open_ifs == 0) {
        nested_ifs = false;
        cout << "OFF NEST" << endl;
    }
}

void Arg() {
    if (print_p) cout << "Arg" << endl;
    get_next_token();
    if (
        curr_token_type == "String"
    ) {
        cout << "terminal: " << token_values[curr_token_index] << endl;
        get_next_token();
    } else {
        get_prev_token();
        Exp();
    }
}

void Argfollow() {
    if (print_p) cout << "Argffollow" << endl;
    if (
        curr_token_type == "Comma"
    ) {
        cout << "terminal: ," << endl;
        Arg();
        Argfollow();
    } else {
        if (print_e) cout << "AF epsilon" << endl;
        epsilon = true;
    }
}

int main() {

    // Gathers valid file names for scanning and parsing

    vector<string> input_file_names;
    vector<string> input_file_extras;
    string input_file_name_format = "sample_input";
    string directory = "./";

    for (const auto & entry : filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".txt" && entry.path().filename().string().rfind(input_file_name_format, 0) == 0) {
            string valid_input_file_name = entry.path().filename().string();
            input_file_names.push_back(valid_input_file_name);
            input_file_extras.push_back(valid_input_file_name.substr(input_file_name_format.length(), valid_input_file_name.length()-input_file_name_format.length()-3-1));
        }
    }

    for (int fn = 0; fn < size(input_file_names); fn++) {

        // Initializes the current file and its outputs

        string input_file_name = directory + input_file_names[fn];
        string scanner_output_file_name = "sample_output_scan" + input_file_extras[fn] + ".txt";
        string parser_output_file_name = "sample_output_parse" + input_file_extras[fn] + ".txt";
    
        fstream input_file(input_file_name);
        parser_output_file.open(parser_output_file_name);
    
        // Reset required variables for parsing

        token_types.clear();
        token_values.clear();

        token_index = 0;
        curr_token_index = 0;
        curr_token_type = "";
        open_ifs = 0;
        nested_ifs = false;

        // Runs the scanner, and generates tokens from the file

        if (input_file.is_open()){
            gettoken(input_file, scanner_output_file_name);
            token_types.push_back("EndofFile");
            token_values.push_back("");
        }
    
        // Runs the parser

        curr_token_type = token_types[curr_token_index];
    
        try {
            Prg();
            parser_output_file << input_file_name << " is a valid SimpCalc program";
        } catch (const ParseException& e) {
            printf("Caught an exception: %s\n", e.what());
        }

        parser_output_file.close();

    }
    return 0;
}