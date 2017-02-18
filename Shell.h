#ifndef SHELL_H__
#define SHELL_H__

#include <iostream>
#include <vector>
#include "Input.h"
#include "Command.h"
//#include "Connector.h"
using namespace std;

class Shell 
{
    private:
        vector <Input*> lines;  //vector that contains EACH line of input that user entered
    public:
        Shell() //constructor for Shell
        {
            
        }
    void addLine()
    {
        string s;
        cout << "$ ";
        getline(cin, s);
        Input* i = new Input(s);
        lines.push_back(i);
        lines.at(0)->parse();
        vector <string> iputs;
        i->getVector(iputs);
        for(int b = 0; b < iputs.size(); b++) 
        {
            //cout << iputs.at(b) << " ";
            if(iputs.at(b) == "exit")
            {
                cout << "[Exited.]" << endl;
                break;
            }
            if(b == 0 && iputs.size() == 1) 
            {
                Command* d = new Command(iputs.at(0));
                d->execute();
                continue;
            }
            if(iputs.at(b) == "&&" || iputs.at(b) == "||" || iputs.at(b) == ";")
            {
                    string a;
                    cout << b + 1 << endl << iputs.size() << endl;
                    if(b+1 == iputs.size())
                    {
                        a = "";
                    }
                    else
                    {
                        a = iputs.at(b+1);
                    }
                    //Input* c = new Connector(iputs.at(b-1), iputs.at(b), a);
                    connect(iputs.at(b-1), iputs.at(b), a);
            }
                //cout << iputs.at(b) << " ";
        }
    }
    void connect(string before, string connector, string after) 
    {
        if(connector == "&&")
        {
            Command* fire = new Command(before);
            fire->execute();
            bool b = fire->isValid();
            if(b)
            {
                Command* water = new Command(after);
                water->execute();
            }
        }
        else if(connector == "||")
        {
            Command* fire = new Command(before);
            fire->execute();
            bool b = fire->isValid();
            if(!b)
            {
                Command* water = new Command(after);
                water->execute();
            }
        }
        else 
        { //connector == ";"
            Command* fire = new Command(before);
            fire->execute();
            Command* water = new Command(after);
            water->execute();
            
        }
    }
    
};

#endif