#include "Command.hpp"

// default constructor. 
Command::Command()
{
    _backgrounded=_pipeOut=false;
    _numArgs=0;
    _in=_out="";
    _inputStream=TokenStream(); // uses keyboard as its input. 
}  

// build a command out of an existing command. 
Command::Command(const Command &c)
{
    // copy over everything but pipeOut. Most situations want that to be false.
    _numArgs=c._numArgs;
    for (int i=0; i<_numArgs; i++) _args[i]=c._args[i];
    _pipeOut=false;                   //  <<<<--------  !!!!
    _backgrounded=c._backgrounded;
    _in = c._in;
    _out = c._out;
    _inputStream=c._inputStream;
}


// read in next command from the TokenStream (based on keyboard)
void
Command::read()
{
     // clear out any existing settings for command
    _numArgs=0;
    _pipeOut=false;
    _backgrounded=false;
    _in="";
    _out="";

    // get next "token" from input stream
    Token tok = _inputStream.nextToken();
    Token fileName;

    // As long as we have not reached the end of the current command ...
    while(tok.type()!=EOL && !_pipeOut)
    {
      // figure out what we're looking at and set this command accordingly.
      switch(tok.type())
      {
      case REDIR_IN: // "<"
              fileName = _inputStream.nextToken();
              if (fileName.type()!=TEXT)
              {
                  cerr << "ERROR: expected filename after <" << endl;
                  while(fileName.type()!=EOL)
                      fileName = _inputStream.nextToken();
                  return;
              }
              _in = fileName.value();
              break;
      case REDIR_OUT: // ">"
              fileName = _inputStream.nextToken();
              if (fileName.type()!=TEXT)
                  cerr << "ERROR: expected filename after >" << endl;
              _out = fileName.value();
              break;
      case PIPE: // "|"
              _pipeOut=true;
              break;
      case BACKGROUND: // "&"
              _backgrounded=true;
              break;
      case TEXT: // a string that needs to be added to the array of arguments
              _args[_numArgs++] = tok.value();
              break;
      default: // anything else is an error (string should catch everything)
              cerr << "unknown/unexpected token: " << tok << endl;
      }

      // get the next token to process ... unless this was a pipe out.
      //   if it was a pipe out, the next token would be part of the next
      //   command, so we don't want to process it here. 
      if (!_pipeOut)
          tok = _inputStream.nextToken();
    }
}

// prints a command (nicely)
ostream&
Command::print(ostream &os) const
{
  // print out the arguments
  for (int i=0; i<_numArgs; i++)
    {
      os << _args[i] << " ";
    }

  // print file redirction as needed. 
  if (redirIn())
    os << "< " << _in << " ";

  if (redirOut())
    os << "> " << _out << " ";

  // dump backgrounding if needed
  if (_backgrounded)
    os << "& ";

  // dump pipeOut if needed
  if (_pipeOut)
    os << "| ";
  
  return os;
}

// use the above print() to overoald the << operator. 
ostream& operator<<(ostream &os, const Command &c)
{
  return c.print(os);
}
