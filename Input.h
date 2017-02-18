#ifndef INPUT_H__
#define INPUT_H__

#include <iostream>
#include <vector>
using namespace std;

class Input
{
    private:
    string inp;
    vector <string> inpV;
    
    public:
    Input()
    {
        
    }
    Input(string s)
    {
        inp = s;
    }
    //virtual void execute() = 0;
    
    //virtual void filter() = 0;
    //we need to compare the command strings with fork or something
    void parse()
    {
        string t;
        for(int i = 0; i < inp.length(); i++)
        {
            if(inp.at(i) == ';')
            {
                inpV.push_back(t);
                t = ";";
                inpV.push_back(t);
                t = "";
            }
            else if(inp.at(i) == '|' && inp.at(i + 1) == '|') 
            {
                inpV.push_back(t);
                t = "|";
                continue;
            }
            else if(inp.at(i) == '&' && inp.at(i + 1) == '&')
            {
                inpV.push_back(t);
                t = "&";
                continue;
            }
            else if(inp.at(i) == ' ') 
            {
                if(t == "exit")
                {
                    inpV.push_back(t);
                    t = "";
                }
                continue;
            }
            else if(inp.at(i) == '#')
            {
                break;
            }
            else
            {
                t.push_back(inp.at(i));
                if(t == "&&" || t == "||") 
                {
                    inpV.push_back(t);
                    t = "";
                }
            }
        }
        
        inpV.push_back(t);
        cout << t << endl;
        //cout << inpV.size() << endl;
        for(unsigned x = 0; x < inpV.size(); x++) 
        {
                //cout << inpV.at(x) << " ";
        }
    }
    
    void getVector(vector <string> &ayyy)
    {
        ayyy = inpV;
    }
};

#endif