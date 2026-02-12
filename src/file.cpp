#include <cstdlib>
#include <filesystem>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "termio.h"
#include "utils.h"

namespace fs = std::filesystem;

fs::path expand_user(const std::string &p) {
    if (!p.empty() && p[0] == '~') {
        const char* home = nullptr;

#ifdef _WIN32
        // On Windows, check USERPROFILE first
        home = std::getenv("USERPROFILE");
        if (!home) {
            const char* drive = std::getenv("HOMEDRIVE");
            const char* path  = std::getenv("HOMEPATH");
            static std::string hp;
            if (drive && path) {
                hp = std::string(drive) + std::string(path);
                home = hp.c_str();
            }
        }
#else
        // On Linux/macOS
        home = std::getenv("HOME");
#endif

        if (!home) {
            // fallback: return unmodified path
            return fs::path(p);
        }

        if (p.size() == 1) {
            return fs::path(home);
        }

        if (p[1] == '/' || p[1] == '\\') {
            return fs::path(home) / p.substr(2);
        }
    }

    return fs::path(p);
}


fs::path path;

void init_path(const std::string &file_path){
	path = expand_user(file_path);
}

int get_file_str(std::string file_path, std::string &content){
	fs::path p = expand_user(file_path);
	
	// Error handlers
	if (!fs::exists(p)) {
		terminate(xcode::fatal, "ERR: File '%s' doesn't exist!\n", file_path.c_str());
		return 1;
	}
	if (fs::is_directory(p)) {
		termio::err << "ERR: '" << file_path << "' is a directory!\n";
		return 2;
	}
	if (!fs::is_regular_file(p)) {
		termio::err << "ERR: '" << file_path << "' Is not a regular file!\n";
		return 3;
	}

	std::ifstream file(p);
	if (!file) {
		termio::cerr << "CERR: Failed to open file!\n";
		return 4;
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	content = buffer.str();

	return 0;
}

int remove_file(const std::string &file_name, std::string o_ext){
	std::string file_to_remove = strip_extension(file_name) + o_ext;
	bool removed = std::filesystem::remove(file_to_remove);
	if (removed) {
		termio::info << "Successfully removed file '" << file_to_remove << "'\n";
	}
	return removed ? 0 : 1;
}

int write_file(const std::string &file_name, const std::string &content, std::string file_path = "") {
    if (file_path.empty()) file_path = "."; // default to current directory

    fs::path dir_path = expand_user(file_path);      // directory where file will go
    fs::path file_dir = dir_path / file_name;        // full path to file

    try {
        fs::create_directories(dir_path); // only create the directory, not the file
    } catch (const fs::filesystem_error &e) {
        termio::err << "ERR: Failed to create directories: " << e.what() << "\n";
        return 1;
    }

    std::ofstream file(file_dir, std::ios::trunc);
    if (!file) {
        termio::err << "ERR: Failed to create file '" << file_dir << "'\n";
        return 1;
    }

    file << content;
    return 0;
}

int create_file(std::string file_name, std::string file_path){
	if(file_path == ""){
		file_path = path;
	}
	
	fs::path file_dir = fs::path(file_path) / file_name;
	
	fs::create_directories(file_dir.parent_path());
	
	std::ofstream file(file_dir);
	if(!file){
		termio::err << "ERR: Failed to create file!\n";
		return 1;
	}
	
	file.close();
	
	return 0;
}
