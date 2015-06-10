#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../include/interface.h"
#include "../../include/communication.h"

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
        std::cout<<"command "<<command<<std::endl;
		if (command.compare("mkdir") == 0)
		{
			char path[128];
			strcpy(path, subs[1].c_str());
			std::cout<<"mkdir "<<path<<std::endl;
			c.simplefs_mkdir(path);
		}
		else if (command.compare("list") == 0)
		{
			char path[128];
			strcpy(path, subs[1].c_str());
			c.simplefs_list(path);
		}else if (command.compare("open") == 0)
		{
            char path[128];
			strcpy(path, subs[1].c_str());
			int mode = atoi(subs[2].c_str());
			c.simplefs_open(path,mode);
		}
		else if (command.compare("unlink") == 0)
		{
            char path[128];
			strcpy(path, subs[1].c_str());
			c.simplefs_unlink(path);
		}
		else if (command.compare("create") == 0)
		{
            char path[128];
			strcpy(path, subs[1].c_str());
			int r = atoi(subs[2].c_str());
			int w = atoi(subs[3].c_str());
			int x = atoi(subs[4].c_str());
			c.simplefs_create(path,r,w,x);
		}
		else if (command.compare("read") == 0)
		{
            //simplefs_read
		}
        else if (command.compare("close") == 0)
		{
            //simplefs_read
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
