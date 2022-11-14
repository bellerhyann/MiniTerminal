#include "Token.hpp"


// internal array to convert from Token type (array index) to string
// representing token type (array values)
static string types[] = {"NOTOK", "TEXT",
			 "BACKGROUND",
			 "PIPE",
			 "REDIR_IN", "REDIR_OUT",
			 "EOL"};

// print token to specified input stream
ostream&
Token::print(ostream &os) const
{ 
  os << types[_type] << " ->" << _value << "<-";
  return os;
}

// print token using <<
ostream& operator<< (ostream &os, const Token &t)
{
  return t.print(os);
}
