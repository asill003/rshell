#ifndef __shell_h__
#define __shell_h__

#include <string>
#include <unistd.h>
#include <queue>
#include <vector>
#include <sstream>
#include <iostream>
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
    getline(cin, userinput);

    statement * st = new statement(userinput);
    st->execute();
}

#endif