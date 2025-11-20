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
    {61,"Equals"}, // "="
    {42,"Multiply"}, // "*"
    {45,"Minus"}, // "-"
    {43,"Plus"}, // "+"
    {40,"LeftParen"}, // "("
    {41,"RightParen"}, // ")"
    {59,"Semicolon"}, // ";"
    {44,"Comma"}, // ","
    {34,"String"}, // "
    {62,"GT"} // ">"
};

map<string,string> key_identifiers = {
    {"IF","If"},{"SQRT","Sqrt"},{"PRINT","Print"},{"ELSE","Else"},{"ENDIF","Endif"},{"AND","And"},{"OR","Or"},{"NOT","Not"}
};

void release(string &state, string &token){ //Function to release state and token
    if(state == "Identifier"){
        if(key_identifiers.find(token) != key_identifiers.end()){
            state = key_identifiers[token];
        }
    }
    if (!token.empty()) {
        cout << state << " " << token << endl;
        token.clear();
        state.clear();
    }
}
void gettoken(fstream &input_file){ //Gets the next token from the input
    string state;
    string map;
    string token;
    char in_char;

    while (input_file.get(in_char)) {
        map = tokens_dict[(int)in_char];
        if (map == "Newline"){
            in_char = ' ';
        }

        if(state != "Skip"){ //checks for state
            if(map == "String" || state == "String"){ //String
                if((int)in_char == 34 && state != "String"){
                    release(state,token);
                    state = map;
                    token += in_char;
                }
                else if(state == "String" && map == "String"){
                    token += in_char;
                    release(state,token);
                }
                else if(map == "Newline"){
                    state = "ERROR: ";
                    token = "Newline during string";
                    release(state, token);
                }
                else{
                    token += in_char;
                }
            }
            else if (map == "Divide"){ //Divide
                if (state == "Divide"){
                    state = "Skip";
                    token.clear();
                }
                else {
                    release(state,token);
                    state = map;
                    token += in_char;
                }
            }
            else if(isalpha(in_char) || in_char == '_'){ //Identifier
                if(state != "Identifier"){
                    release(state, token);
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
                    release(state, token);
                    state = "Number";
                }
                token += in_char;
            }
            else if(state == "Number" && in_char == '.'){ //decimal
                token += in_char;
            }
            else if(map == "Multiply"){ //Multiply
                if(map != state){
                    release(state, token);
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
                    release(state,token);
                }
                state = map;
                token += in_char;
            }
            else if(map == "GT"){ //Greater Than
                if(state != map && state != ""){
                    release(state,token);
                }
                state = map;
                token += in_char;
            }
            else if(map == "Equals"){ //Equals
                token += in_char;
                if(state == "Colon"){
                    state = "Assign";
                    release(state,token);
                }
                if(state == "GT"){
                    state = "GTEqual";
                    release(state,token);
                }
                else{
                    state = map;
                }
            }
            else if(map == "Minus"){ //Minus
                if(state != map && state != ""){
                    release(state,token);
                }
                state = map;
                token += in_char;
            }
            else if(map == "Plus"){ //Plus
                if(state != map && state != ""){
                    release(state,token);
                }
                state = map;
                token += in_char;
            }
            else if(map == "LeftParen"){ //LeftParen
                release(state,token);
                state = map;
                token += in_char;
            }
            else if(map == "RightParen"){ //RightParen
                release(state,token);
                state = map;
                token += in_char;
            }
            else if(map == "Semicolon"){ //Semicolon
                release(state,token);
                state = map;
                token += in_char;
            }
            else if(map == "Comma"){ //Semicolon
                release(state,token);
                state = map;
                token += in_char;
            }
            else if(map != state){ //Release
                release(state, token);
                state = map;
            }
        }

        else if(map == "Newline"){ //resets skip when newline
            state = map;
        }

        // cout << "in_char: " << in_char << " ASCII: " << (int)in_char << " Map: " << map;
        // cout << " State: " << state << endl;
    }

}

int main(){
    fstream input_file("sample_input_1.txt");
    if (input_file.is_open()){
        gettoken(input_file);
    }
    cout << "EndofFile" << endl;

    return 0;
}