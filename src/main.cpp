#include <iostream>
#include <string>
#include <array>
#include <cstring>
#include "termio.h"
#include "utils.h"
#include "file.h"

// Version strings
#define version "Udev-0.1"

bool flagt(const std::string& flag, unsigned int func){
	if((flag == "-h" || flag == "-help") && func == 0)		return true;
	if((flag == "-g" || flag == "-guide") && func == 1)		return true;
	if((flag == "-c" || flag == "-compile") && func == 2)	return true;
	if((flag == "-r" || flag == "-run") && func == 3)		return true;
	if((flag == "-a" || flag == "-all") && func == 4)		return true;
	if((flag == "-d" || flag == "-delete") && func == 5)	return true;
	if((flag == "-v" || flag == "-version") && func == 6)	return true;
	
	// If nothing is true, return false
	return false;
}

// the main function
int main(int argc, char* argv[]){
	std::array<std::string, 2> extensions = {".flx", ".fxb"};
	// Set the log level
	if (argc >= 3) {
		init_path(argv[2]);
	}
	
	termio::info << "		-- / [[ FlareX ]] \\ --\n";
	
	termio::set_log_level(log_level::info);
	
	if(argc < 2) {
		termio::info << "Usage: " << argv[0] << " <flags> <file>\n";
		termio::info << "type '-h' for more information.\n";
		return 1;
	}
	
	std::string flag = argv[1];
	
	// Help
	if(flagt(flag, 0)){
		termio::info << "flags (first argument): \n\n" <<
		" -c / -compile	-	Compiles the specified file\n" <<
		" -r / -run		- 	Executes the specified file\n" <<
		" -a / -all		- 	Both Compiles and Executes the specified file\n" <<
		" -d / -delete	-	Cleans any compiled of built files\n" <<
		" -h / -help	-	Prints a help document\n" <<
		" -g / -guide	-	Prints a guide to the language\n" <<
		" -v / -version	-	Prints the version of the interpreter\n\n" <<
		"The second argument is the relative/full file path.\n" <<
		"NOTE: You don't have to include the file extension, it is handled automatically\n" <<
		"The source file extension is " << extensions[0] << ", anything else will not be compiled\n";
	}
	// Guide
	else if(flagt(flag, 1)){
		termio::info << "Guide to the FlareX programming language\n\n" <<
		" Input/Output:\n" <<
		"	\\> out	-	the argument is either a string, a number or a variable\n" <<
		R"(		Example: out "Hello, World!", out 1, out .x)" << '\n' <<
		"		NOTE: when printing variables, prefix them with a dot .\n" <<
		"\n Variables:\n\n"
		"	\\> var	-	is a prefix that tells the compiler it is a variable\n" <<
		"	   \\> int	-	comes after the var prefix, tells the compiler the value is an interger\n" <<
		"	   \\> str	-	comes after the var prefix, tells the compiler the value is a string literal\n" <<
		"	after the var prefix and the type comes the name of the variable. It can be anything, really\n" <<
		R"(	Example: var int count = 1, var str hello = "Hello, World!"\n)" <<
		"\n Flow:\n\n" <<
		"	\\> ext	-	Sets the exit code and terminates the program\n" <<
		"		The only argument this instruction has is the exit code,\nWhich is always an unsigned interger\n" <<
		"End.\n";
	}
	// Compile
	else if(flagt(flag, 2)){
		if(argc <= 2) terminate(xcode::usage_err, "Please specify the name of the file as the second argument");
		compile_file(argv[2], extensions[0], extensions[1]);
	}
	// Run
	else if(flagt(flag, 3)){
		if(argc <= 2) terminate(xcode::usage_err, "Please specify the name of the file as the second argument");
		run_file(argv[2], extensions[1]);
	}
	// Compile & Run
	else if(flagt(flag, 4)){
		if(argc <= 2) terminate(xcode::usage_err, "Please specify the name of the file as the second argument");
		std::string cfile = compile_file(argv[2], extensions[0], extensions[1]);
		run_file(cfile, extensions[1]);
	}
	// Remove
	else if(flagt(flag, 5)){
		if(argc <= 2) terminate(xcode::usage_err, "Please specify the name of the file as the second argument");
		remove_file(argv[2], extensions[1]);
	}
	// Version
	else if(flagt(flag, 6)){
		termio::info << "FlareX Interpreter version " << version << '\n';
	}
	// If nothing matches
	else{
		terminate(xcode::usage_err, "Undefined flag '%s'", argv[1]);
	}

	// Shows error number
	termio::info << "\nInterpreter exited with " << num_errors << " errors\n";
	return 0;
}
