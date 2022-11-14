#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

/************************************************************
 *  If you are looking at this, you should likely stop      *
 * doing so. You do not need to understand this in order to *
 * work with the Command class.                             *
 *                                                          *
 *  This class represents a single token                    *
 *                                                          *
 ************************************************************/

#include <iostream>
#include <string>
using namespace std;

// Constants representing token types
#define UNDEF -1
#define NOTOKEN 0
#define TEXT 1
#define BACKGROUND 2
#define PIPE 3
#define REDIR_IN 4
#define REDIR_OUT 5 
#define EOL 6

class Token
{
private:
  int _type;     // token type (see above constants)
  string _value; // string value of token
  
public:

  // default constructor
  Token() : _type(UNDEF), _value("") {}

  // accessor/modifier for Token type
  int type() const {return _type;}
  int& type() {return _type;}

  // accessor/modifier for Token value
  string value() const {return _value;}
  string& value() {return _value;}

  // print a token to given stream. 
  ostream& print(ostream &os) const;

  // overload << to print ot stream. 
  friend ostream& operator<< (ostream &os, const Token &t);
};

#endif
