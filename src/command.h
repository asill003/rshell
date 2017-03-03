#ifndef __command_h__
#define __command_h__

#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <sys/wait.h>
using namespace std;

class command
{
public:
	command(vector<string> &);
	bool execute();
private:
	vector<string> args;
};

command::command(vector<string> &strs)
{
	args = strs;
}

bool command::execute() 
{
	if (args.size() != 0 && args[0] == "exit") {
		exit(0);
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

#endif