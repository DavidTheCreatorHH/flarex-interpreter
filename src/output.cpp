#include <iostream>
#include <fstream>
#include <cstring>
#include "termio.h"
#include "utils.h"

int num_errors = 0;

struct log_stream{
    std::ostream* out;

    log_stream(std::ostream* o = nullptr) : out(o) {}

    template<typename T>
    log_stream& operator<<(const T& val) {
        if (out) (*out) << val;
        return *this;
    }

    // manipulators like std::endl
    log_stream& operator<<(std::ostream& (*manip)(std::ostream&)) {
        if (out) (*out) << manip;
        return *this;
    }
};


class nullbuf : public std::streambuf {
protected:
    int overflow(int c) override { return c; }
};

class colorbuf : public std::streambuf{
    std::streambuf* dest;
    const char* color;
    bool is_error;
public:
    colorbuf(std::streambuf* d, const char* c, bool e)
        : dest(d), color(c), is_error(e) {}

protected:
    int overflow(int ch) override{
        if (ch == EOF) return EOF;
        
        if(is_error && ch == '\n') ++num_errors;
        dest->sputn(color, std::strlen(color));
        dest->sputc(ch);
        dest->sputn("\033[0m", 4);
        
        return traits_type::not_eof(ch);
    }
};

namespace termio{
    nullbuf nb;
    std::ostream null_stream(nullptr);

    colorbuf info_buf(std::cout.rdbuf(), "\033[97m", false);
    colorbuf warn_buf(std::cerr.rdbuf(), "\033[93m", false);
    colorbuf err_buf(std::cerr.rdbuf(), "\033[95m", true);
    colorbuf cerr_buf(std::cerr.rdbuf(), "\033[31m", true);

    std::ostream info_stream(&info_buf);
    std::ostream warn_stream(&warn_buf);
    std::ostream error_stream(&err_buf);
    std::ostream critical_err_stream(&cerr_buf);

    // The proxy streams
    log_stream info(&info_stream);
    log_stream warn(&warn_stream);
    log_stream err(&error_stream);
    log_stream cerr(&critical_err_stream);

    log_level current_level = log_level::info;

    void set_log_level(log_level lvl) {
        current_level = lvl;

        info.out  = (lvl <= log_level::info) ? &info_stream : &null_stream;
        warn.out  = (lvl <= log_level::warn) ? &warn_stream : &null_stream;
        err.out   = (lvl <= log_level::err ) ? &error_stream : &null_stream;
        cerr.out  = (lvl <= log_level::crit) ? &critical_err_stream : &null_stream;
    }
}
