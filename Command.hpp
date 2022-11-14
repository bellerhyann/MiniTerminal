#ifndef _COMMAND_HPP_
#define _COMMAND_HPP_

/*****************************************************
 * Basic representation of a command. Also allows    *
 *  next command to be read in from keyboard         *
 *****************************************************/

#include <ostream>
#include <string>

using namespace std;

// commands are pulled froma stream of Tokens. You do NOT
//   need to unserstand this to use the Command class. 
#include "TokenStream.hpp"

// maximum number of arguments any COmmand can have (includes command itself)
#define MAX_ARGS 100

class Command
{
private:
  string _args[MAX_ARGS];    // arary of arguments
  int _numArgs;              // actual number of arguments
  string _in;                // input redirect filename, if any
  string _out;               // input redirect filename, if any
  bool _pipeOut;             // true if output is to be sent to a pipe
  bool _backgrounded;        // true if command is to be run in background

  TokenStream _inputStream;  // stream from which command is read.
  
public:
  Command();                 // Basic command (allows reads from keyboard)
  Command(const Command &c); // build (duplicated) version of existing command.
                             //      note that copy has pipeOut set to false!
  
  void read();   // read the next command
  
  const string& name() const {return _args[0];} // returns command name

  const string* args() const {return _args;} // returns array of arguments
  int numArgs() const {return _numArgs;}     // returns how many arguments

  // returns input redirection file
  const string&  inputRedirectFile() const {return _in;}
  // returns input redirection file
  const string&  outputRedirectFile() const {return _out;}

  // the following indicate if input/output redirection is happening and how
  bool pipeOut() const {return _pipeOut;} // is there an output pipe?
  bool redirIn() const {return _in.length()!=0;}  // output redirection?
  bool redirOut() const {return _out.length()!=0;}// input redirection?

  // is this command followed by a "&" for backgrounding? 
  bool backgrounded() const {return _backgrounded;} 

  // prints Command to specified stream
  ostream& print(ostream &os) const;

  // overloads << to be able to print a Command. 
  friend ostream& operator<<(ostream &os, const Command &c);
};

#endif
