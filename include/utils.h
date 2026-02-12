#ifndef UTILS_H
#define UTILS_H

extern std::string strip_extension(const std::string& filename);
extern std::string compile_file(std::string filename, std::string o_ext, std::string c_ext);
extern void run_file(std::string filename, std::string c_ext);

enum class xcode{
	ok,
	usage_err,
	compile_err,
	runtime_err,
	fatal
};

extern void terminate(xcode code, const char* fmt, ...);

#endif
