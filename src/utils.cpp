#include <string>
#include <vector>
#include <array>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "termio.h"
#include "file.h"
#include "token.h"
#include "parse.h"
#include "runner.h"

enum class xcode{
	ok = 0,
	usage_err = 1,
	compile_err = 2,
	runtime_err = 3,
	fatal = 255
};

[[noreturn]]
void terminate(xcode code, const char* fmt, ...){
    char buffer[4096];

    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    switch (code) {
        case xcode::ok:
        case xcode::usage_err:
            termio::warn << buffer << '\n';
            break;

        case xcode::compile_err:
        case xcode::runtime_err:
            termio::err << buffer << '\n';
            break;

        case xcode::fatal:
        default:
            termio::cerr << buffer << '\n';
            break;
    }

	termio::info << "\nInterpreter exited with " << num_errors << " errors\n";

    std::exit(static_cast<int>(code));
}

std::string strip_extension(const std::string& filename){
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos == std::string::npos) {
        return filename; // no dot found, return as-is
    }
    return filename.substr(0, dot_pos);
}

// returns the compiled file
std::string compile_file(std::string filename, std::string o_ext, std::string c_ext){
	std::string ofile = strip_extension(filename) + o_ext;
	std::string cfile = strip_extension(filename) + c_ext;
	std::string content;
	get_file_str(ofile, content);
	if(content != ""){
		termio::info << "Compiling file '" << ofile << "'...\n";
	   
		// Tokenize the file into tokens
		std::vector<Token> tokens;
		tokenize(content, tokens);
		
		// parse the tokens into instructions (bytecode)
		std::string bytecode = parse_tokens(tokens);
		std::string compiled_file = cfile;
		
		write_file(compiled_file, bytecode);
		termio::info << "Succesfully compiled into '" << cfile << "'\n";
		return compiled_file;
	}
	else{
		termio::warn << "WARN: File '" << ofile << "' is empty.\n";
	}
	
	return "";
}

// executes a file (the bytecode)
void run_file(std::string filename, std::string c_ext){
	std::string content;
	std::string file_to_run = strip_extension(filename) + c_ext;
	get_file_str(file_to_run, content);
	termio::info << "Running '" << file_to_run << "'...\n\n";

	// Now execute the file
	execute_bytecode(content);
}
