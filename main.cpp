#include <iostream>
#include "include/shared_memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <set>

#include "include/server.h"
#include "include/client.h"
#include "include/interface.h"

using namespace std;

void showFiles(server & server,INode *inode)
{
    for(int j =0; j < sizeof(inode->pointers)/sizeof(int); ++j)
    {
        std::cout<<inode->pointers[j]<<" ";
        //if(inode->pointers[j] != 0)
            //std::cout<<"name "<<server.fs->inodes[inode->pointers[j]].name<<" ";
        if(server.fs->inodes[inode->pointers[j]].type == TYPE_HELPER)
        {
                std::cout<<"\n\t"<<"\t";
                showFiles(server,&server.fs->inodes[inode->pointers[j]]);
        }
    }
}

void ls(server & server)
{
    for(int i = 0; i < INODE_COUNT ;++i)
    {
        if(server.fs->inodes[i].type != -1)
        {
            std::cout<<server.fs->inodes[i].name <<"\t"<<i<<"\t";
            showFiles(server,&server.fs->inodes[i]);
            std::cout<<endl;
        }
    }
    std::cout<<endl;
}

int testCreateDir(server & server)
{
    server.simplefs_mkdir("/");

    server.simplefs_mkdir("/root");
    server.simplefs_mkdir("/opt");
    server.simplefs_mkdir("/run");
    server.simplefs_mkdir("/lusia");
    server.simplefs_mkdir("/ania");
    server.simplefs_mkdir("/blin");
    server.simplefs_mkdir("/cabaj");
    server.simplefs_mkdir("/mam");
    server.simplefs_mkdir("/beke");

    server.simplefs_mkdir("/root");

    server.createFile("/pasha",TYPE_FILE, 1, 1, 1);

    server.createFile("/root/tut",TYPE_FILE, 1, 1, 1);
    server.createFile("/root/abs",TYPE_FILE, 1, 1, 1);
    server.simplefs_mkdir("/root/abs");
    server.simplefs_mkdir("/r/abs/k");
}

int testUnlink(server & server)
{
    char nazwa[] = "/root/abs";
    char *wskaznik = nazwa;
    server.simplefs_unlink(wskaznik);
}

int main(int argc,char** argv)
{
 //    server server;
 //    testCreateDir(server);
 //    ls(server);
 //    testUnlink(server);
 //    ls(server);
 //    server.simplefs_mkdir("/root/home");
 //    ls(server);
    
	const char* clientStr = "c";
	const char* serverStr = "s";
	{
		if(strcmp(argv[1], clientStr) == 0) 
		{
			printf("klient\n");
			client c;
			char tmp[128];
			strcpy(tmp, "ahoj");
			// c.sendRequest(0,tmp, 0, 0, 0);
			interface i;
			i.run();
		}
		if(strcmp(argv[1], serverStr) == 0)
		{
			printf("server\n");
			server s;
			s.work();
		}
	}

    // testCreateDir(server);
    //server.setBlockBit(0, 7, true);
    //server.setBlockBit(12, 7, true);
    //bool inode = server.canAddToFile(7, 5);
    //printf(inode ? "true\n" : "false\n");
    return 0;
}
