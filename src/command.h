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
using namespace std;

class command
{
public:
	command(vector<string>);
	virtual bool execute();
private:
	vector<string> args;
};

command::command(vector<string> strs)
{
	args = strs;
}

bool command::execute() 
{
	if (args.size() != 0 && args[0] == "exit")
    {
		exit(0);
	}
	else if (args.size() != 0 && (args[0] == "test" || (args[0] == "[" && args[args.size()-1] == "]")))
    {
		if (args.size() != 3 && args[0] == "test")
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
		string path = args[2];

		struct stat sb;
		stat(path.c_str(), &sb);
		bool test;

		test = 	(flag == "-e" && (S_ISDIR(sb.st_mode) || S_ISREG(sb.st_mode))) ||
				(flag == "-f" && S_ISREG(sb.st_mode)) ||
				(flag == "-d" && S_ISDIR(sb.st_mode));

		if (flag != "-e" && flag != "-f" && flag != "-d")
        {
			perror("test: error unresolved flags");
			return false;
		}

		string output = (test) ? "(True)" : "(False)";
		cout << output << endl;
		return test;	
	}

	int status;
	char * cmd = (char *)args[0].c_str();
	char * argv[args.size() + 1];
	for (unsigned i = 0; i != args.size(); i++)
	{
		argv[i] = (char *)args[i].c_str();
	}
	argv[args.size()] = 0;

	pid_t pid = fork();
	if (pid == -1)
	{
		perror("error: failed to fork");
		exit(EXIT_FAILURE);
		return 0;
	}
	else if (pid == 0)
	{
		execvp(cmd, argv);
		string str = cmd;
		for (unsigned i = 1; i != args.size(); i++)
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
			return 0;
		}
		return status == 0;
	}
}

class connector
{
public:
    connector();
    virtual bool execen(bool, command *) = 0;
};

connector::connector() {};

class logicalor : public connector
{
public:
    bool execen(bool, command *);
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
    return left || right->execute();
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
    vector<string> arguments;
    connector * curr;
    int end, start;

    while (sstr >> word)
    {
        if (word[0] == '#')
        {
            break;
        }
        else if (word[0] == '(')
        {
            int npars = 1;
            for (unsigned i = 1; i != word.size(); i++)
            {
                if (word[i] == '(') npars++;
                else if (word[i] == ')') npars--;
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
                if (word[word.size() - 1] == ')')
                {
                    if (npars == 1)
                    {
                        end = sstr.tellg();
                        if (end == -1)
                        {
                            end = userinput.size() - 1;
                        }
                        else
                        {
                            end--;
                        }
                        para = userinput.substr(start, end - start);
                        cmds.push(new statement(para));
                        found = true;
                        break;
                    }
                    else
                    {
                        npars--;
                    }
                }
                else
                {
                    if (word[0] == '(')
                    {
                        npars++;
                    }
                }
            }
            
            if (!found)
            {
                cout << "error missing ')'" << endl;
                exit(0);
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
        cout << "no input into statement or a messed up parentheses" << endl;
        exit(0);
    }
    command * currcmd = cmds.front();
    connector * currcon = 0;
    cmds.pop();
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