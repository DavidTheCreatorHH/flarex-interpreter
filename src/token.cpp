#include <iostream>
#include <vector>
#include <string>
#include <array>

#include "token.h"
#include "termio.h"
#include "utils.h"

int tokenize(const std::string& code, std::vector<Token> &token_vector){
	std::string cur_token;
	bool in_string = false;
	size_t line_num = 0;
	bool is_comment = false;

	for(size_t i = 0; i < code.size(); i++) {
        char c = code[i];

		if(in_string){
			if(c == '"') {
				// End string token
				Token t;
				t.token = cur_token;
				t.type = TOKEN_STRING;
				t.end = false; // handled later
				token_vector.push_back(t);
				cur_token.clear();
				in_string = false;
			}
			else {
				cur_token += c;
			}
			continue;
		}

		if(c == '"') {
			in_string = true;
			cur_token.clear();
			continue;
		}

		if(c == '/' && i + 1 < code.size() && code[i+1] == '/'){
			// Skip until end of line
			i += 2; // skip both slashes
			while(i < code.size() && code[i] != '\n') i++;
			line_num++;  // increment line counter for debugging
			continue;
		}

		// Token separator
		if(c == ' ' || c == '\n' || c == ';'){
			if(is_comment) continue;
			
			if(c == '\n'){
				is_comment = false;
				continue;
			}
			
			if(!cur_token.empty()){
				Token t;

				// Check if number
				Token_T type = TOKEN_NORMAL;
				bool is_number = true;
				for(size_t i = 0; i < cur_token.size(); i++){
					char ch = cur_token[i];
					if(i == 0 && ch == '-') continue; // allow negative sign
					if(!isdigit(ch)){
						is_number = false;
						break;
					}
				}

				if(is_number){
					type = TOKEN_NUMBER;
				}

				if (!cur_token.empty() && cur_token.front() == '.'){
					cur_token.erase(0,1);
					type = TOKEN_VARIABLE;
				}

				t.type = type;
				t.end = (c == ';');
				t.line_num = line_num;
				t.token = cur_token;
				line_num += (c == '\n');
				token_vector.push_back(t);
				cur_token.clear();
			}

			continue;
		}

        cur_token += c;
    }

    // Check last token (if code doesn't end with semicolon)
    if(!cur_token.empty()) {
        terminate(xcode::compile_err, "ERR: Missing semicolon at line %u", line_num);
        return 1;
    }

    return 0;
}
