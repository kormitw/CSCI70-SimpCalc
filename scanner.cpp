#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

map<char, string> tokens_dict;

void gettoken(fstream &input_file){ //Gets the next token from the input
    string state = "";
    char in_char;
    int i = 0; //temp for reading until certain point

    while (i < 10) { //read till end of file input_file.get(in_char)
        input_file.get(in_char);
        cout << "in_char: " << in_char << " ASCII: " << (int)in_char << endl;
        i++;
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