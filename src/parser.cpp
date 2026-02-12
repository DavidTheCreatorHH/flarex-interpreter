#include <vector>
#include <string>
#include <unordered_map>
#include "token.h"
#include "termio.h"
#include "parse.h"
#include "utils.h"

void convert_to_bytecode(const std::vector<Token>& itt_vector, std::string& itt_map){
	auto emit = [&](const std::string& instr){
		itt_map += instr + "\\EOI";
	};

	// For variables
	static std::unordered_map<std::string, unsigned int> symbol_table;
	static std::unordered_map<std::string, std::string> var_types;
	static unsigned int num_vars = 0;

	for (size_t i = 0; i < itt_vector.size(); i++){
		const std::string& str = itt_vector[i].token;
		Token_T type = itt_vector[i].type;
		static size_t line_num = itt_vector[i].line_num;

		if(type != TOKEN_NORMAL) continue;

		std::string next_str;
		Token_T next_type = TOKEN_NEXIST;

		if (i + 1 < itt_vector.size()) {
			next_str = itt_vector[i + 1].token;
			next_type = itt_vector[i + 1].type;
		}

		if(str == "out"){
			if(next_type == TOKEN_STRING){
				emit("pushs '" + next_str + "'");
				emit("print");
				emit("pop");
				i++;
			}
			else if(next_type == TOKEN_NUMBER){
				emit("pushn " + next_str);
				emit("print");
				emit("pop");
				i++;
			}
			else if(next_type == TOKEN_VARIABLE){
				std::string var_name = next_str;

				// Check properly for undefined variables
				if(symbol_table.count(var_name) == 0)
					terminate(xcode::compile_err, "Undefined variable '%s' at line %zu\n", var_name.c_str(), line_num);

				size_t index = symbol_table.at(var_name);  // safe lookup
				emit("load " + std::to_string(index));
				emit("print");
				emit("pop");
				i += 2; // skip variable token
			}
			else{
				terminate(xcode::compile_err, "Expected number or string after 'out', got '%s'", next_str.c_str());
			}
		}
		else if(str == "var"){
			if(i + 3 >= itt_vector.size())
				terminate(xcode::compile_err, "Incomplete variable definition!");

			std::string type_str = itt_vector[i + 1].token;
			std::string var_name = itt_vector[i + 2].token;
			std::string value;
			
			size_t j = i + 3;
			if(j < itt_vector.size() && itt_vector[j].token == "=") j++;
			
			while(j < itt_vector.size()) {
				if(itt_vector[j].type == TOKEN_STRING || itt_vector[j].type == TOKEN_NUMBER) {
					value = itt_vector[j].token;
					break;
				}
				j++;
			}
    
			if(value.empty())
				terminate(xcode::compile_err, "Missing value for variable '%s'", var_name.c_str());

			unsigned int slot;
			if(symbol_table.count(var_name)) {
				if(var_types[var_name] != type_str)
					terminate(xcode::compile_err,
							  "Cannot redefine variable '%s' with different type (%s -> %s)\n",
							  var_name.c_str(), var_types[var_name].c_str(), type_str.c_str());
				
				// Redefinition: use existing slot
				slot = symbol_table[var_name];
			} else {
				// First-time definition
				slot = num_vars;
				symbol_table[var_name] = slot;
				var_types[var_name] = type_str;
				num_vars++;
			}

			//symbol_table[var_name] = num_vars;

			if(type_str == "int"){
				emit("pushn " + value);
			}
			else if(type_str == "str"){
				emit("pushs '" + value + "'");
			}
			else{
				terminate(xcode::compile_err, "Undefined variable type '%s'", type_str.c_str());
			}

			emit("store " + std::to_string(slot));
			emit("pop");

			num_vars += 1;
			i = j;
		}
		else if(str == "ext"){
			if(next_type == TOKEN_NUMBER){
				emit("setstate exec");
				emit("pushn " + next_str);
				emit("terminate");
				i++;
			}
			else{
				terminate(xcode::compile_err, "Expected number after 'ext', got '%s'", next_str.c_str());
			}
			}
		else{
			terminate(xcode::compile_err, "Invalid instruction '%s'", next_str.c_str());
		}
	}
}

std::string parse_tokens(std::vector<Token>& token_vector){
	std::vector<Token> temp_token_vector;
	std::string bytecode_map;
	
	for (size_t i = 0; i < token_vector.size(); i++) {
		temp_token_vector.push_back(token_vector[i]);

		if (token_vector[i].end) {
			convert_to_bytecode(temp_token_vector, bytecode_map);
			temp_token_vector.clear();
		}
	}

	return bytecode_map;
}
