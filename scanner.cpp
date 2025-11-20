#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <ctype.h>

using namespace std;

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
    if (!token.empty()) {
        line = state + " " + token + "\n";
        cout << line;
        outputFile << line;
        line.clear();
        token.clear();
        state.clear();
    }
}
void gettoken(fstream &input_file){ //Gets the next token from the input
    string state;
    string map;
    string token;
    char in_char;
    ofstream outputFile("scanner_output.txt");
    bool seenDot = false;
    bool seenExp = false;
    bool expHasDigits = false;

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
                    release(state,token,outputFile);
                }
                else if (in_char == ' ' || map =="Newline"){
                    release(state,token,outputFile);
                }
            }
            else if(!isalnum(in_char) && token.back() == '.'){
                state = "Error";
                token = "Lexical Error: Invalid number format";
                release(state,token,outputFile);
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
                    state = "ERROR: ";
                    token = "Lexical Error: Unterminated string";
                    release(state,token,outputFile);
                }
                else{
                    token += in_char;
                }
            }
            else if(state == "Excl"){ //!
                if(in_char != '='){
                    state = "ERROR";
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
                    seenDot = false;
                    seenExp = false;
                    expHasDigits = false;
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
                if(state == "Number" && (token.back() == 'e' || token.back() == 'E')){
                    state = "Error";
                    token = "Lexical Error: Invalid number format";
                }
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

    outputFile << "EndofFile";
    outputFile.close();
}

int main(){
    fstream input_file("sample_input_1.txt");
    if (input_file.is_open()){
        gettoken(input_file);
    }
    cout << "EndofFile" << endl;
    input_file.close();
    return 0;
}