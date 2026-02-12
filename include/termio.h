#ifndef DEBUG_OUTPUT_H
#define DEBUG_OUTPUT_H

#include <ostream>

extern int num_errors;

enum class log_level { info=0, warn=1, err=2, crit=3 };

namespace termio {
	struct log_stream {
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
    extern log_stream info;
    extern log_stream warn;
    extern log_stream err;
    extern log_stream cerr;

    void set_log_level(log_level lvl);
}

#endif
