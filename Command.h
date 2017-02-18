#ifndef COMMAND_H__
#define COMMAND_H__

#include <iostream>
using namespace std;

class Command: public Input
{
    private: 
    string com;
    bool valid;
    public:
    Command()
    {
        com = "";
    }
    Command(string cc)
    {
        com = cc;
    }
    void execute()
    {
        valid = true;
        if(com == "ls")
        {
            cout << "hi we're doing ls" << endl;
        }//execute "com"
        else if(com.substr(0, 2) == "cd")
        {
            if(com.size() > 2)
            cout << "hi we're doing cd for " << com.substr(2) << endl;
            else
            cout << "hi we did cd, welcome to workspace" << endl;
        }
        else
        {
            valid = 0;
            cout << "invalid input" << endl;
        }
    }
    bool isValid()
    {
        if(valid)
        {
            return true;
        }
        return false;
    }
};

#endif