#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "parser.hpp"
// #include "parser.hpp"
using namespace std;

static ios_base::Init iostream_initializer; // <-- force init

int token_index = 0;
int curr_token_index = 0;
string curr_token_type = "";
vector<string> token_types;
vector<string> token_values;
bool epsilon = false;
bool print_p = true;
bool print_t = print_p;
bool print_e = print_p;
ofstream output_file("test_output.txt");

void get_next_token() {
    curr_token_index++;
    curr_token_type = token_types[curr_token_index];
    if (print_t) cout << "next! curr token: " << curr_token_type << endl;
}

// void get_next_token() {
//     curr_token_index++;
//     if (curr_token_index >= token_types.size()) {
//         cerr << "ERROR: Unexpected end of token stream\n";
//         exit(1);
//     }
//     curr_token_type = token_types[curr_token_index];
// }

void get_prev_token() {
    curr_token_index--;
    curr_token_type = token_types[curr_token_index];
    if (print_t) cout << "prev! curr token: " << curr_token_type << endl;
}

// void get_prev_token() {
//     if (curr_token_index == 0) {
//         cerr << "ERROR: Attempted backtrack before start of tokens\n";
//         exit(1);
//     }
//     curr_token_index--;
//     curr_token_type = token_types[curr_token_index];
// }


void parse_error(string token_type, string token_value) {
    // cout << "check " << curr_token_type << " vs " << token_type << endl;
    if (curr_token_type != token_type) {
        output_file << "Parse Error: " << token_type << " expected.";
        exit(0);
    } else {
        cout << "terminal: " << token_value << endl;
    }
}

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
        output_file << "Assignment Statement Recognized" << endl;
        get_next_token();
    } else if (
        curr_token_type == "Print"
    ) {
        cout << "terminal: PRINT" << endl;
        cout << "terminal: (" << endl;
        get_next_token();
        Arg();
        Argfollow();
        cout << "terminal: PR)" << endl;
        get_next_token();
        output_file << "Print Statement Recognized" << endl;
    } else if (
        curr_token_type == "If"
    ) {
        output_file << "If Statement Begins" << endl;
        cout << "terminal: IF" << endl;
        Cnd();
        cout << "terminal: :" << endl;
        get_next_token();
        Blk();
        Iffollow();
        output_file << "If Statement Ends" << endl;
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
    // if (!epsilon) {
    //     get_next_token();
    // } else {
    //     epsilon = !epsilon;
    // }
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
    // if (!epsilon) {
    //     get_next_token();
    // } else {
    //     epsilon = !epsilon;
    // }
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
    // if (!epsilon) {
    //     get_next_token();
    // } else {
    //     epsilon = !epsilon;
    // }
    if (
        curr_token_type == "Raise"
    ) {
        cout << "terminal: **" << endl;
        Lit();
        Litfollow();
    } else {
        // get_prev_token();
        if (print_e) cout << "LF epsilon" << endl;
        epsilon = true;
    }
}

void Val() {
    if (print_p) cout << "Val" << endl;
    // cout << "Val: " << epsilon << endl;
    // get_next_token();
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
        cout << "terminal: (" << endl;
        get_next_token();
        Exp();
        cout << "terminal: SQ)" << endl;
        get_next_token();
    } else if (
        curr_token_type == "LeftParen"
    ) {
        cout << "terminal: (" << endl;
        Exp();
        cout << "terminal: LP)" << endl;
        get_next_token();
    }
    // Parentheses
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
    }
}

void Iffollow() {
    if (print_p) cout << "Iffollow" << endl;
    get_next_token();
    if (
        curr_token_type == "Endif"
    ) {
        cout << "terminal: ENDIF" << endl;
        cout << "terminal: ;" << endl;
        get_next_token();
    } else if (
        curr_token_type == "Else"
    ) {
        cout << "terminal: ELSE" << endl;
        get_next_token();
        Blk();
        cout << "terminal: ENDIF" << endl;
        get_next_token();
        cout << "terminal: ;" << endl;
        get_next_token();
        get_next_token();
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
    string input_file_name = "sample_output_scan_2.txt";
    // string input_file_name = "test_input.txt";
    fstream input_file(input_file_name);
    if (input_file.is_open()){
        cout << "File opened" << endl;
    }
    string current_line;
    getline(input_file, current_line);
    while (current_line != "EndofFile") {
        stringstream line_stream(current_line);
        string token_type;
        string token_value;
        char temp_token_char;
        while (line_stream.get(temp_token_char)) { // get token type
            if (temp_token_char == ' ') {
                break;
            }
            token_type += temp_token_char;
        }
        while (line_stream.get(temp_token_char)) { // ignore spaces
            if (temp_token_char != ' ') {
                token_value += temp_token_char;
                break;
            }
        }
        while (line_stream.get(temp_token_char)) { // get token value
            token_value += temp_token_char;
        }
        token_types.push_back(token_type);
        token_values.push_back(token_value);
        token_index++;
        // cout << "token_type: " << token_type << ", token: " << token_value << endl;
        getline(input_file, current_line);
    }
    token_types.push_back("EndofFile");
    token_values.push_back("");

    curr_token_type = token_types[curr_token_index];
    Prg();
    
    output_file << input_file_name << " is a valid SimpCalc program";
    output_file.close();

    return 0;
}