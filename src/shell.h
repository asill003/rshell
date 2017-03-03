#ifndef __shell_h__
#define __shell_h__

#include <string>
#include <unistd.h>
#include <queue>
#include <vector>
#include <sstream>
#include <iostream>
#include "connector.h"
#include "command.h"
using namespace std;

class shell
{
private:
    char username[128];
    char hostname[128];

public:
    shell();
    void add_line();

};

shell::shell()
{
    getlogin_r(username, 128);
    gethostname(hostname, 128);
}

void shell::add_line()
{
    cout << username << "@" << hostname << "$ ";
    string userinput;
    string word;
    getline(cin, userinput);
    stringstream sstr(userinput);

    queue<connector *> conns;
    queue<command *> cmds;
    vector<string> arguments;
    connector * curr;

    while(sstr >> word)
    {
        if (word[0] == "#")
        {
            break;
        }
        else if (word == "&&")
        {
            cmds.push(new command(arguments));
            curr = new logicaland();
            conns.push(curr);
            arguments.clear();
        }
        else if (word[word.size() - 1] == ';')
        {
            string sub = word.substr(0, word.size() - 1);
            arguments.push_back(sub);
            cmds.push(new command(arguments));
            curr = new semicolon();
            conns.push(curr);
            arguments.clear();
        }
        else if (word == "||")
        {
            cmds.push(new command(arguments));
            curr = new logicalor();
            conns.push(curr);
            arguments.clear();
        }
        else
        {
            arguments.push_back(word);
        }
    }
    if (!arguments.empty())
    {
        cmds.push(new command(arguments));
    }
        
    command * currcmd = cmds.front();
    connector * currcon = 0;
    cmds.pop();
    bool overall = currcmd->execute();
    while (!cmds.empty()) {
        currcon = conns.front();
        conns.pop();
        command * currcmd = cmds.front();
        cmds.pop();
        currcon->execen(overall, currcmd);
    }

}

#endif
