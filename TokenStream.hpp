#ifndef _TOKEN_STREAM_HPP_
#define _TOKEN_STREAM_HPP_

#include "Token.hpp"

/************************************************************
 *  If you are looking at this, you should likely stop      *
 * doing so. You do not need to understand this in order to *
 * work with the Command class.                             *
 *                                                          *
 *  This class breaks up an input stream into its tokens    *
 *                                                          *
 * Here, I utilize a discrete structure known as a          *
 *   deterministic finite automata to "parse" input and     *
 *   figure out what it represents.                         * 
 *                                                          *
 ************************************************************/


class TokenStream
{
private:
  int **_dfa;  // two dimensional array "go to" representation of DFA for tokens
public:
  TokenStream(); // default constructor

  // copy one token stream into another at costruction
  TokenStream(const TokenStream &ts) {_dfa=ts._dfa;}
  
  Token nextToken(); // get the next token from this stream. 
};

#endif
