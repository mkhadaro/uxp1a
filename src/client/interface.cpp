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
		if (command.compare("mkdir") == 0)
		{
			char path[128];
			strcpy(path, subs[1].c_str());
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
            char path[128];
			int fd = atoi(subs[1].c_str());
			int length  = atoi(subs[2].c_str());
			c.simplefs_read(fd,path,length);
		}
        else if (command.compare("lseek") == 0)
		{
			int fd = atoi(subs[1].c_str());
			int whence = atoi(subs[2].c_str());
			int length = atoi(subs[3].c_str());
			c.simplefs_lseek(fd,whence,length);
		}
        else if (command.compare("close") == 0)
		{
            char path[128];
            int fd = atoi(subs[1].c_str());
            c.close_file(fd);
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
