#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <ctype.h>

using namespace std;

map<int, string> tokens_dict = {
    {13,"newline"}, // newline
    {10,"newline"}, // newline
    {47,"divide"}, // "/"
    {58,"colon"}, // ":"
    {61,"equals"}, // "="
    {42,"multiply"} // "*"
};




void gettoken(fstream &input_file){ //Gets the next token from the input
    string state;
    string map;
    string token;
    char in_char;

    while (input_file.get(in_char)) {
        map = tokens_dict[(int)in_char];
        if (map == "newline"){
            in_char = ' ';
        }

        if(state != "skip"){ //checks for state
            if (state == "divide" && map == "divide"){ //Comment
                state = "skip";
            }
            else if(isalpha(in_char) || in_char == '_'){ //Identifier
                state = "Identifier";
                token += in_char;
            }
            else if((isalnum(in_char) || in_char == '_') && state == "Identifier"){ //Identifier but pwede na num dahil di na start
                state = "Identifier";
                token += in_char;
            }
            else if(isdigit(in_char)){
                state = "Num";
                token += in_char;
            }
            else if(map == "multiply"){
                if(state == map){
                    state = "Raise";
                    token += in_char;
                }
                else{
                    state = map;
                    token += in_char;
                }
            }
            else if(map == "colon"){ //Colon
                state = map;
                token += in_char;
            }
            else if(map == "equals"){ //Equals
                token += in_char;
                if(state == "colon"){
                    state = "Assign";
                }
            }
            else { //release
                if (token != ""){
                    cout << state <<" " << token << endl;
                    token = "";
                }
                state = map;
            }
        }

        else if(map == "newline"){ //resets skip when newline
            state = map;
        }

        // cout << "in_char: " << in_char << " ASCII: " << (int)in_char << " Map: " << map;
        // cout << " State: " << state << endl;
    }

    void release(){
        cout << state <<" " << token << endl;
        token = "";
    }
}

int main(){
    fstream input_file("sample_input_1.txt");
    if (input_file.is_open()){
        cout << "File opened" << endl;
    }
    gettoken(input_file);

    return 0;
}