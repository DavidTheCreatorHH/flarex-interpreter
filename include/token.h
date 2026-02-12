#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

enum Token_T{
    TOKEN_NORMAL,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_VARIABLE,
	TOKEN_NEXIST
};

struct Token{
   std::string token;
   Token_T type;
   bool end;
   std::size_t line_num;
};

extern int tokenize(const std::string& code, std::vector<Token> &token_vector);

#endif
