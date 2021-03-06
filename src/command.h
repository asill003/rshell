#ifndef __command_h__
#define __command_h__

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <sys/wait.h>
#include <sstream>
#include <fcntl.h>
using namespace std;

class command
{
public:
	command(vector<string>);
	bool execute();
private:
	vector<string> args;
};

command::command(vector<string> strs)
{
	args = strs;
}

class node
{
public:
    node() {};
    node * left;
    node * right;
    virtual bool execute() = 0;
};

class pipenode : public node
{
public:
    pipenode() {};
    bool execute();
};

class commandnode : public node
{
public:
    commandnode(vector<string> &cmdargs) : args(cmdargs) {left = 0; right = 0;};
    vector<string> args;
    bool execute();
};

char * gen_random(const int len) {
    char * s = (char *)malloc(len * sizeof(char));
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
    return s;
}

bool pipenode::execute() {
    int back, nfd;
    fflush(stdout);
    back = dup(1);
    char * str = gen_random(40);
    str[0] = '/';
    str[1] = 't';
    str[2] = 'm';
    str[3] = 'p';
    str[4] = '/';
    FILE * file = fopen(str, "wb");
    nfd = fileno(file);

    dup2(nfd, 1);
    close(nfd);
    
    left->execute();

    fflush(stdout);
    dup2(back, 1);
    close(back);

    commandnode * cmdnode = (commandnode *)right;
    if (cmdnode->args[0] == "grep" || cmdnode->args[0] == "cat")
        cmdnode->args.push_back(string(str));

    bool val = right->execute();
    remove(str);

    return val;
}

bool commandnode::execute() {
    command * cmd = new command(args);
    return cmd->execute();
}

bool command::execute() 
{
	if (args.size() != 0 && args[0] == "exit")
    {
		exit(0);
	}                                                                                                      //exit
	else if (args.size() != 0 && (args[0] == "test" || (args[0] == "[" && args[args.size()-1] == "]")))
    {
		if (args.size() != 3 && args[0] == "test")                                                     //test function, first "test" then the other option ]
		{
			cout << "test: " << args[2] << ": unary operator expected" << endl;
			return false;
		}
		else if (args.size() != 4 && args[0] == "[" && args[args.size()-1] == "]")
		{
			cout << "[: " << args[2] << ": unary operator expected" << endl;
			return false;
		}

		string flag = args[1];
		string path = args[2];                                              //the flag and path expected after test command

		struct stat sb;
		stat(path.c_str(), &sb);
		bool test;

		test = 	(flag == "-e" && (S_ISDIR(sb.st_mode) || S_ISREG(sb.st_mode))) ||
				(flag == "-f" && S_ISREG(sb.st_mode)) ||                                                //the cases for each flag
				(flag == "-d" && S_ISDIR(sb.st_mode));

		if (flag != "-e" && flag != "-f" && flag != "-d")
        {
			perror("test: error unresolved flags");
			return false;                                                       //if it is not one of the required flags
		}

		string output = (test) ? "(True)" : "(False)";
		cout << output << endl;
		return test;	
	}

    node * root = 0;
    node * parent = 0;

    vector<string> cmdargs;
    for (int i = args.size() - 1; i != -1; i--)
    {
        if (args[i] != "|")
        {
            cmdargs.insert(cmdargs.begin(), args[i]);
        }
        else if (args[i] == "|")
        {
            if (root == 0)
            {
                root = parent = new pipenode();
                root->right = new commandnode(cmdargs);
                cmdargs.clear();
            }
            else
            {
                parent->left = new pipenode();
                parent = parent->left;
                parent->right = new commandnode(cmdargs);
                cmdargs.clear();
            }
        }
    }
    if (root != 0) {
        parent->left = new commandnode(cmdargs);
        return root->execute();
    }

    int i;
    int type = 0;
    for (i = 0; i != args.size(); i++)
    {
        if (args[i] == ">")
        {
            type = 1;
            break;
        }
        else if (args[i] == ">>")
        {
            type = 2;
            break;
        }
        else if (args[i] == "<")
        {
            type = 3;
            break;
        }
    }
    if (type == 1)
    {
        if (args.size() == i+1) {
            perror("syntax error near unexpected token 'newline'");
            return 0;
        }

        int back, nfd;
        fflush(stdout);
        back = dup(1);
        FILE * file = fopen(args[i+1].c_str(), "wb");
        nfd = fileno(file);

        dup2(nfd, 1);
        close(nfd);
        
        vector<string> cmdargs;
        for (int j = 0; j != i; j++) {
            cmdargs.push_back(args[j]);
        }

        command * cmd = new command(cmdargs);
        bool val = cmd->execute();

        fflush(stdout);
        dup2(back, 1);
        close(back);
        return val;
    }
    else if (type == 2) {
        if (args.size() == i+1) {
            perror("syntax error near unexpected token 'newline'");
            return 0;
        }

        int back, nfd;
        fflush(stdout);
        back = dup(1);
        FILE * file = fopen(args[i+1].c_str(), "a+");
        nfd = fileno(file);

        dup2(nfd, 1);
        close(nfd);
        
        vector<string> cmdargs;
        for (int j = 0; j != i; j++) {
            cmdargs.push_back(args[j]);
        }

        command * cmd = new command(cmdargs);
        bool val = cmd->execute();

        fflush(stdout);
        dup2(back, 1);
        close(back);
        return val;
    }
    else if (type == 3) {
        if (args.size() == i+1) {
            perror("syntax error near unexpected token 'newline'");
            return 0;
        }

        if (args[0] == "grep" || args[0] == "cat") {
            vector<string>::iterator iter = args.begin();
            advance(iter, i);
            args.erase(iter);
        }
        else {
            vector<string>::iterator iter = args.begin();
            advance(iter, i);
            args.erase(iter);
            args.erase(iter);
        }
    }

    int status;
    char * cmd = (char *)args[0].c_str();                                       //c string and array of strings
    char * argv[args.size() + 1];
    for (unsigned i = 0; i != args.size(); i++)
    {
        argv[i] = (char *)args[i].c_str();                                                              
    }
    argv[args.size()] = 0;

    pid_t pid = fork();                                                 //forking the process: parent and child
    if (pid == -1)
    {
        perror("error: failed to fork");
        exit(EXIT_FAILURE);                                         // if the fork fails
        return 0;
    }
    else if (pid == 0)
    {
        execvp(cmd, argv);
        string str = cmd;                                                   // if it works, it will exec, then the rest of this will not happen
        for (unsigned i = 1; i != args.size(); i++)                             //if there is an error it will print the userinput and exit 
        {
            str += " " + args[i];
        }
        perror(str.c_str());
        exit(EXIT_FAILURE);
        return 0;
    }
    else
    {
        while(waitpid(0, &status, 0) == -1)
        {
            perror("error: unable to wait");
            return 0;                                               //error if it does not wait for the fork
        }
        return status == 0;
    }
}

class connector
{
public:
    connector();
    virtual bool execen(bool, command *) = 0;               //connector class
};

connector::connector() {};

class logicalor : public connector
{
public:
    bool execen(bool, command *);                               //whether to execute next or not, this function exists in different forms for each class
};

class logicaland : public connector
{
public:
    bool execen(bool, command *);
};

class semicolon : public connector
{
public:
    bool execen(bool, command *);
};

bool logicalor::execen(bool left, command * right)
{
    return left || right->execute();                                    //executing next based on the logic
}

bool logicaland::execen(bool left, command * right)
{
    return left && right->execute();
}

bool semicolon::execen(bool left, command * right)
{
    return right->execute();
}



class statement : public command
{
public:
    statement(string &);
    bool execute();
private:
    queue<connector *> conns;
    queue<command *> cmds;
};

statement::statement(string &userinput) : command(vector<string>())
{
    string word;
    string para;
    stringstream sstr(userinput);
    vector<string> arguments;                                           //parsing
    connector * curr;
    int end, start;

    while (sstr >> word)
    {
        if (word[0] == '#')
        {
            break;                                                          //breaking up strings and pushing them back into vector based on whether the command ends
        }
        else if (word[0] == '(')
        {
            int npars = 0;
            for (unsigned i = 0; word[i] == '('; i++) {
                npars++;
            }
            for (unsigned i = word.size() - 1; word[i] == ')'; i--)
            {
                npars--;
            }
            if (npars == 0)
            {
                para = word.substr(1, word.size() - 2);
                cmds.push(new statement(para));
                continue;
            }
            start = sstr.tellg();
            start -= word.size() - 1;
            bool found = false;
            while (sstr >> word)
            {
                for (unsigned i = 0; word[i] == '('; i++)
                {
                    npars++;
                }
                for (unsigned i = word.size() - 1; word[i] == ')'; i--)
                {
                    npars--;
                }
                
                if (npars == 0) {
                    end = sstr.tellg();
                    if (end == -1)
                    {
                        end = userinput.size() - 1;
                    }
                    else
                    {
                        end -= 1;
                    }
                    para = userinput.substr(start, end - start);
                    cmds.push(new statement(para));
                    found = true;
                    break;
                }
            }
            
            if (!found)
            {
                cout << "error missing ')'" << endl;
            }
        }
        else if (word == "&&")
        {
            if (arguments.size() != 0) cmds.push(new command(arguments));
            curr = new logicaland();
            conns.push(curr);
            arguments.clear();
        }
        else if (word[word.size() - 1] == ';')
        {
            if (arguments.size() != 0)
            {
                string sub = word.substr(0, word.size() - 1);
                arguments.push_back(sub);
                cmds.push(new command(arguments));
            }
            curr = new semicolon();
            conns.push(curr);
            arguments.clear();
        }
        else if (word == "||")
        {
            if (arguments.size() != 0) cmds.push(new command(arguments));
            curr = new logicalor();
            conns.push(curr);
            arguments.clear();
        }
        else
        {
            arguments.push_back(word);
        }
    }
    if (arguments.size() != 0) cmds.push(new command(arguments));
}

bool statement::execute()
{
    if (cmds.size() == 0)
    {
        return 0;
    }
    command * currcmd = cmds.front();
    connector * currcon = 0;
    cmds.pop();                                                         //executing the commands one by one based on connectors and the logic behind the connectors
    bool overall = currcmd->execute();
    while (!cmds.empty())
    {
        currcon = conns.front();
        conns.pop();
        command * currcmd = cmds.front();
        cmds.pop();
        currcon->execen(overall, currcmd);
    }

    return overall;
}

#endif