#ifndef RUNNER_H
#define RUNNER_H

#include <vector>
#include <string>

extern std::vector<std::string> parse_instructions(const std::string& bytecode);

extern void execute_bytecode(const std::string& bytecode);

#endif
