#include "TokenStream.hpp"

// a few variables that were useful as global only to this file. 
static char charBuffer;     //last char read;might not be part of current token.
static bool charBufferFull; //is there a char stored in the last char read?

// default constructor
TokenStream::TokenStream()
{
  charBufferFull = false; //have not yet read a character. So nothing in buffer
  _dfa = new int*[7];     // 7 dfa states (see below comments)
  for (int state=0; state<7; state++) // each could "go to" based on any char
    _dfa[state] = new int[256];

  // STATE 0 --> (start)
  // STATE 1 --> TEXT
  // STATE 2 --> BG
  // STATE 3 --> PIPE
  // STATE 4 --> REDIR_IN
  // STATE 5 --> REDIR_OUT
  // STATE 6 --> EOL

  // set up "go to" transitions. Assume anything is text from start state
  //    or existing text state ... next if's patch this as need be
  for (int ch=0; ch<256; ch++)
    {
      _dfa[0][ch]=TEXT;
      _dfa[TEXT][ch]=TEXT;
    }

  // when we see an &, |, <, >, or EOL, it has to be its own token 
  _dfa[0][(int)'&'] = 2;
  _dfa[TEXT][(int)'&'] = -1;
  
  _dfa[0][(int)'|'] = 3;
  _dfa[TEXT][(int)'|'] = -1;

  _dfa[0][(int)'<'] = 4;
  _dfa[TEXT][(int)'<'] = -1;

  _dfa[0][(int)'>'] = 5;
  _dfa[TEXT][(int)'>'] = -1;

  _dfa[0][(int)'\n'] = 6;
  _dfa[TEXT][(int)'\n'] = -1;

  // all transitins out of states 2 or higher are invalid
  for (int state=2; state<7; state++)
    for (int ch=0; ch<256; ch++)
      _dfa[state][ch] = -1;

  // no whitespace is part of a token. 
  for (int state=0; state<7;state++)
    {
      _dfa[state][(int)' '] = -1;
      _dfa[state][(int)'\t'] = -1;
    }
}

// get the next token, based on the dfa
Token
TokenStream::nextToken()
{
  char ch; // next character in input stream. 

  // if the buffer is filled, then it is our next character ...
  if (charBufferFull)
    {
      ch = charBuffer;
      charBufferFull=false;
    }
  else // ... otherwise get the next character from the keyboard. 
    ch = cin.get(); // does not skip whitespace!

  // skip all whitespace
  while(ch==' ' || ch=='\t')
    {
      ch = cin.get();
    }


  // start building up token based on DFA
  string tokVal;
  
  int state = 0;    // active state in DFA; at first , it is "start" state
  int old_state=-1; // previous state in DFA (none at first)

  // as long as our state s valid
  while(state!=-1)
    {
      old_state=state; // update the old state
      
      state=_dfa[state][(int)ch]; // follow transition to new state

      // so long as new state is valid
      if (state!=-1) //live state
      {
	tokVal.push_back(ch); // add character to end of token string
	if (state==0 || state==1) // more chars possible
	  ch=cin.get();
	else // dead end state
	  break;
      }
    }

  // if we got out of above loop  due to reading an extra character ...
  if (state==-1)
    {
      // put the character in our buffer
      charBufferFull=true; 
      charBuffer=ch;

      // side note: could likley have used cin.unget() instead, but
      //  behavior of such can be a bit unreliable on differeng systems. 
    }

  // now we're ready to fill in our information into a token.
  Token t;
  t.value()=tokVal;
  if (state!=-1) // if we cut out without seeing an invalid character ...
    t.type()=state;  // state tells us the type. 
  else
    t.type()=old_state; // otehrwise old_state tells us the type. 

  return t;
}
