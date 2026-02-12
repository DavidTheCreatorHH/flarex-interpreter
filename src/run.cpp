#include <vector>
#include <string>
#include <iostream>
#include <stack>
#include <any>
#include <unordered_map>
#include "termio.h"
#include "utils.h"

struct vmem {
    std::stack<unsigned int> stack;
    std::unordered_map<unsigned int, std::any> heap;
    unsigned int next_handle = 1;
    std::string current_state;

    int alloc(std::any obj) {
        int handle = next_handle++;
        heap.emplace(handle, std::move(obj));
        return handle;
    }

    int pop() {
        if (stack.empty())
            termio::err << "ERR: Stack underflow";

        int h = stack.top();
        stack.pop();
        return h;
    }

    void push_string(const std::string& str) {
        stack.push(alloc(str));
    }

    void push_int(int num) {
        stack.push(alloc(num));
    }

    void print_top() {
        int h = pop();

        auto it = heap.find(h);
        if (it == heap.end())
			termio::err << "ERR: Invalid handle (" << h << ")";

        auto& obj = it->second;

        if (auto p = std::any_cast<std::string>(&obj))
            std::cout << *p << '\n';
        else if (auto p = std::any_cast<int>(&obj))
            std::cout << *p << '\n';
    }

    void set_state(const std::string& state) {
        current_state = state;
    }
};

std::vector<std::string> parse_instructions(const std::string& bytecode) {
    std::vector<std::string> instructions;
    size_t start = 0;
    size_t end = 0;

    while ((end = bytecode.find("\\EOI", start)) != std::string::npos) {
        instructions.push_back(bytecode.substr(start, end - start));
        start = end + 4; // skip past \END
    }

    return instructions;
}

struct var{
	enum Type {INT, STR} type;
	int int_val;
	std::string str_val;
};

void execute_bytecode(const std::string& bytecode){
	vmem vm;
	unsigned int exit_code = 0;

	auto instructions = parse_instructions(bytecode);

	bool can_exec = true;

	if(!instructions.empty() && instructions.back() != "terminate"){termio::cerr << "CERR: Program did not include a terminate instruction\n"; can_exec = false;}

	for(const auto& instr : instructions){
		if(!can_exec) break;
		
		// Set the state
		if (instr.rfind("setstate", 0) == 0){
			vm.set_state(instr.substr(9));
		}
		// Push a string onto the stack
		else if (instr.rfind("pushs", 0) == 0){
			size_t start = 7, end = instr.rfind('\'');
			vm.push_string(instr.substr(start, end - start));
		}
		// Push a number onto the stack
		else if(instr.rfind("pushn", 0) == 0){
			std::string val = instr.substr(6);
			vm.push_int(std::stoi(val));
		}
		// Print whatever is on top of the stack
		else if (instr == "print"){
			if (!vm.stack.empty()) {
				vm.print_top();
			}
		}
		else if (instr == "pop"){
			if (!vm.stack.empty()) vm.stack.pop();
		}
		// Variables
		else if(instr.rfind("store", 0) == 0){
			int slot = std::stoi(instr.substr(6));
			if(vm.stack.empty()){
				termio::cerr << "CERR: Program did not include a terminate instruction\n";
				break;
			}

			unsigned int handle = vm.stack.top();
			vm.stack.pop();

			// Copy value into variable slot
			auto& obj = vm.heap[handle];

			if(obj.type() == typeid(int))
				vm.heap[slot] = std::any(std::any_cast<int>(obj));
			else if(obj.type() == typeid(std::string))
				vm.heap[slot] = std::any(std::any_cast<std::string>(obj));
			else{
				termio::err << "ERR: Unknown type in store\n";
				break;
			}
		}
		else if(instr.rfind("load", 0) == 0){
			int slot = std::stoi(instr.substr(5));
			if(vm.heap.count(slot) == 0){
				termio::err << "ERR: Invalid variable ID ( " << slot << " ) in load instruction\n";
				break;
			}

			auto& obj = vm.heap[slot];

			// allocate a fresh heap handle for the stack
			unsigned int new_handle = vm.next_handle++;
			if(obj.type() == typeid(int))
				vm.heap[new_handle] = std::any(std::any_cast<int>(obj));
			else if(obj.type() == typeid(std::string))
				vm.heap[new_handle] = std::any(std::any_cast<std::string>(obj));
			else{
				termio::err << "ERR: Unknown type (" << typeid(obj.type()).name() << ") in load\n";
				break;
			}

			vm.stack.push(new_handle);
		}

		else if(instr == "terminate"){
			if(vm.stack.empty()) {
				termio::err << "ERR: terminate called on empty stack\n";
				exit_code = 1; // default error code
			} else {
				int handle = vm.stack.top();
				vm.stack.pop(); // optional: remove the value
				// Look up the actual value in the heap
				if(vm.heap[handle].type() == typeid(int)) {
					exit_code = std::any_cast<int>(vm.heap[handle]);
				} else {
					termio::err << "ERR: terminate expects an int on top of stack\n";
					exit_code = 1; // error code
				}
			}
			break;
		}
		else {
			termio::err << "ERR: Unknown instruction: " << instr << '\n';
		}
	}
	
	vm.heap.clear();
	
	termio::info << "Program exited with exit code " << exit_code << "\n";
}
