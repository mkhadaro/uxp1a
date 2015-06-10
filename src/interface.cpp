#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/interface.h"
#include "../include/communication.h"

using namespace std;

void interface::run(client &c)
{
	cout<<"System plikow\n";

	while(true)
	{
		cout<<">";
		string input;
		getline(cin, input);
		istringstream iss(input);
		string subs[16];
		int i = 0;
		do
		{
			iss >> subs[i];
			i++;
		}
		while(iss && i < 16);

		string command = subs[0];

		if (command.compare("mkdir") == 0)
		{
			char path[128];
			strcpy(path, subs[1].c_str());
			c.simplefs_mkdir(path);
		}
		// else if (command == "open")
		// else if (command == "unlink")
		// else if (command == "creat")
		// else if (command == "read")
		// else if (command == "write")
		// else if (command == "lseek")
		else if (command.compare("list") == 0)
		{
			char path[128];
			strcpy(path, subs[1].c_str());
			c.simplefs_list(path);
		}
		else if (command.compare("exit") == 0)
		{
			break;
		}
		else 
		{ 
			cout<<"Nieprawidlowe polecenie\n";
		}
	}

}