#include <iostream>
#include <csignal>
#include <cstring>
#include <istream>
#include <fstream>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include "Command.hpp"

/*****************************************************
 *  Example usage of the Command class for project 1 *
 *****************************************************/


int main(void)
{
    Command com;
    int num=1; // keep track of number fo commands. 
    
    // prompt for and read in first command. 
    cout << ">>>> ";
    com.read();
    while(com.name() != "exit")
    {
        if(com.name() == "cd")
        {
           //retreave input array
            string* _argArray;
            _argArray = (string*) com.args();
            //store file path to varible
            string _file = _argArray[1];
            //converts string to char array
            const char *path = _file.c_str();
            //  chdir(filepath);
            chdir(path);
        }
        else
        {
            //unless otherwise stated child will run and then prompt for another com
            if(!com.backgrounded())
            {
                if(com.redirIn() and com.redirOut())
                {
                    int pid = fork();
                    //the 20 lines below are repeated often. This only this iteration will contain comments.
                    string _name;
                    _name = com.name();
                    //_name now has the first argument from command
                    //this character array will store the strings from the input
                    //after they have been altered with c_str()
                    char* array[100];
                    //pull arguments from command class
                    string* _argArray;
                    _argArray = (string*) com.args();
                    //create char array and store arguments in it
                    array[0] = new char[1]; strcpy(array[0], _name.c_str());
                    //loop through the rest
                    for(int i=0; i< com.numArgs(); i++)
                    {array[i] = new char[8]; strcpy(array[i], _argArray[i].c_str());}
                    //must store NULL at end so it can run execvp
                    int last = com.numArgs();
                    array[last] = NULL;
                    
                    if (pid!=0)
                    { waitpid(pid, NULL, 0);}
                    else
                    {
                        FILE *in = fopen(com.inputRedirectFile().c_str(), "r");
                        FILE *fp = fopen(com.outputRedirectFile().c_str(), "w+");
                        dup2( fileno(fp), fileno(stdout) );
                        dup2( fileno(in), fileno(stdin) );
                        execvp(array[0], array);
                    }
                }
                else if(com.redirIn())
                {
                    int pid = fork();
                    string _name;
                    _name = com.name();
                    char* array[100];
                    string* _argArray;
                    _argArray = (string*) com.args();
                    array[0] = new char[1]; strcpy(array[0], _name.c_str());
                    for(int i=0; i< com.numArgs(); i++)
                    {array[i] = new char[8]; strcpy(array[i], _argArray[i].c_str());}

                    int last = com.numArgs();
                    array[last] = NULL;
                    if (pid!=0)
                    {waitpid(pid, NULL, 0);}
                    else
                    {
                        FILE *fp = fopen(com.inputRedirectFile().c_str(), "r");
                        dup2( fileno(fp), fileno(stdin) );
                        execvp(array[0], array);
                    }
                }
                else if(com.redirOut())
                {
                    int pid = fork();
                    string _name;
                    _name = com.name();
                    char* array[100];
                    string* _argArray;
                    _argArray = (string*) com.args();
                    array[0] = new char[1]; strcpy(array[0], _name.c_str());
                    for(int i=0; i< com.numArgs(); i++)
                    {array[i] = new char[8]; strcpy(array[i], _argArray[i].c_str());}
                    int last = com.numArgs();
                    array[last] = NULL;
                    if (pid!=0)
                    { waitpid(pid, NULL, 0);}
                    else
                    {
                        FILE *fp = fopen(com.outputRedirectFile().c_str(), "w+");
                        dup2( fileno(fp), fileno(stdout) );
                        execvp(array[0], array);
                    }
                }
                else if(com.pipeOut())
                {
                    int pid = fork();
                    if(pid!=0)
                    {
                        waitpid(pid, NULL, 0);
                    }
                    else
                    {
                        //pipe varibles
                        int pipeInfo[2];
                        pipe(pipeInfo);
                        //char* array[100];
                        int pid = fork();
                        if (pid!=0)
                        {
                            waitpid(pid, NULL, 0);

                            close(pipeInfo[1]);
                            dup2(pipeInfo[0], fileno(stdin));
                            //reads what happens after
                            com.read();
                            // grabs whats after pipe
                            string _name;
                            _name = com.name();
                            string* _argArray;
                            _argArray = (string*) com.args();
                          
                            char* array[2];
                            array[1] = NULL;
                            array[0] = new char[1]; strcpy(array[0], _name.c_str());
                          
                            execvp(array[0], array);
                            //exit(1);
                        }
                    else /// do "ls -l"
                      {
                        close(pipeInfo[0]);
                        dup2(pipeInfo[1], fileno(stdout));
                          string _name;
                          _name = com.name();
                          char* array[100];
                          string* _argArray;
                          _argArray = (string*) com.args();
                          array[0] = new char[1]; strcpy(array[0], _name.c_str());
                          for(int i=0; i< com.numArgs(); i++)
                          {array[i] = new char[8]; strcpy(array[i], _argArray[i].c_str());}
                          int last = com.numArgs();
                          array[last] = NULL;
                          execvp(array[0], array);
                          //exit(1);
                       }
                    }
                }
                //end of pipe
                //if simple command run like regular
                else
                {
                int pid = fork();
                if (pid != 0)
                {waitpid(pid, NULL, 0);}
                else
                {
                    string _name;
                    _name = com.name();
                    char* array[100];
                    string* _argArray;
                    _argArray = (string*) com.args();
                    array[0] = new char[1]; strcpy(array[0], _name.c_str());
                    for(int i=0; i< com.numArgs(); i++)
                    { array[i] = new char[8]; strcpy(array[i], _argArray[i].c_str());}
                    int last = com.numArgs();
                    array[last] = NULL;
                    execvp(array[0], array);
                    }
                }
            }
            if(com.backgrounded())
            {
                int pid = fork();
                if (pid!=0)
                {
                    //give a space to answer
                }
                else
                {
                    int pid = fork();
                    if (pid!=0)
                    {
                        waitpid(pid, NULL, 0);
                        //this will run first & finish this run through
                        cout<< "Completed: PID = " << pid << " :: Command = "<< com<< endl;
                        exit(1);
                    }
                    else
                    {
                        //once I finish not backgrounded, the same things should be put below
                            if(com.redirIn() and com.redirOut())
                            {
                                int pid = fork();
                                //the 20 lines below are repeated often. This only this iteration will contain comments.
                                string _name;
                                _name = com.name();
                                //_name now has the first argument from command
                                //this character array will store the strings from the input
                                //after they have been altered with c_str()
                                char* array[100];
                                //pull arguments from command class
                                string* _argArray;
                                _argArray = (string*) com.args();
                                //create char array and store arguments in it
                                array[0] = new char[1]; strcpy(array[0], _name.c_str());
                                //loop through the rest
                                for(int i=0; i< com.numArgs(); i++)
                                {array[i] = new char[8]; strcpy(array[i], _argArray[i].c_str());}
                                //must store NULL at end so it can run execvp
                                int last = com.numArgs();
                                array[last] = NULL;
                                
                                if (pid!=0)
                                { waitpid(pid, NULL, 0);}
                                else
                                {
                                    FILE *in = fopen(com.inputRedirectFile().c_str(), "r");
                                    FILE *fp = fopen(com.outputRedirectFile().c_str(), "w+");
                                    dup2( fileno(fp), fileno(stdout) );
                                    dup2( fileno(in), fileno(stdin) );
                                    execvp(array[0], array);
                                }
                            }
                            else if(com.redirIn())
                            {
                                int pid = fork();
                                string _name;
                                _name = com.name();
                                char* array[100];
                                string* _argArray;
                                _argArray = (string*) com.args();
                                array[0] = new char[1]; strcpy(array[0], _name.c_str());
                                for(int i=0; i< com.numArgs(); i++)
                                {array[i] = new char[8]; strcpy(array[i], _argArray[i].c_str());}

                                int last = com.numArgs();
                                array[last] = NULL;
                                if (pid!=0)
                                {waitpid(pid, NULL, 0);}
                                else
                                {
                                    FILE *fp = fopen(com.inputRedirectFile().c_str(), "r");
                                    dup2( fileno(fp), fileno(stdin) );
                                    execvp(array[0], array);
                                }
                            }
                            else if(com.redirOut())
                            {
                                int pid = fork();
                                string _name;
                                _name = com.name();
                                char* array[100];
                                string* _argArray;
                                _argArray = (string*) com.args();
                                array[0] = new char[1]; strcpy(array[0], _name.c_str());
                                for(int i=0; i< com.numArgs(); i++)
                                {array[i] = new char[8]; strcpy(array[i], _argArray[i].c_str());}
                                int last = com.numArgs();
                                array[last] = NULL;
                                if (pid!=0)
                                { waitpid(pid, NULL, 0);}
                                else
                                {
                                    FILE *fp = fopen(com.outputRedirectFile().c_str(), "w+");
                                    dup2( fileno(fp), fileno(stdout) );
                                    execvp(array[0], array);
                                }
                            }
                            else
                            {
                                string _name;
                                _name = com.name();
                                char* array[100];
                                string* _argArray;
                                _argArray = (string*) com.args();
                                array[0] = new char[1]; strcpy(array[0], _name.c_str());
                                for(int i=0; i< com.numArgs(); i++)
                                { array[i] = new char[8]; strcpy(array[i], _argArray[i].c_str());}
                                int last = com.numArgs();
                                array[last] = NULL;
                                execvp(array[0], array);
                            
                            }

                    }
                }
            }
        }
        // print out current command
        cout << num++ << ")" << com << endl;
        // prompt for and read next command
	cout << ">>>> ";
        com.read();
    }
    cout << "Thank you for using mini-shell. We now return you to your regularly scheduled shell!" << endl;
}
