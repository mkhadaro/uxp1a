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
    server.simplefs_mkdir("/param");
    server.simplefs_mkdir("/dom");
    server.simplefs_mkdir("/mam");
    server.simplefs_mkdir("/beke");

    server.simplefs_mkdir("/root");

    server.createFile("/pasha",TYPE_FILE, 1, 1, 1);

    server.createFile("/root/tut",TYPE_FILE, 1, 1, 1);
    server.createFile("/root/abs",TYPE_FILE, 1, 1, 1);
    server.simplefs_mkdir("/root/home");
    server.simplefs_mkdir("/r/abs/k");
}

int testUnlink(server & server,char *nazwa)
{
    server.simplefs_unlink(nazwa);
}

int testOpenFile(server & server,char *name)
{
    std::cout<<"file description "<<server.simplefs_open(name,READ)<<std::endl;
}

int main(int argc,char** argv)
{

    // server server;
    // testCreateDir(server);
    // ls(server);
    // testUnlink(server);
    // ls(server);
    // server.simplefs_mkdir("/");
    // ls(server);

    if(argc == 2)
    {
    	const char* clientStr = "c";
    	const char* serverStr = "s";
    	{
    		if(strcmp(argv[1], clientStr) == 0)
    		{
    			printf("klient\n");
    			client c;
    			interface i;
    			i.run(c);
    		}
    		if(strcmp(argv[1], serverStr) == 0)
    		{
    			printf("server\n");
    			server s;
    			s.work();
    		}
    	}
    }
    else if (argc == 1)
    {
        server s;
        s.simplefs_mkdir("/");
        s.simplefs_mkdir("/root");
        s.simplefs_mkdir("/adf");
        s.createFile("/adf/sdf1", TYPE_FILE, 1, 1, 1);
        //s.showServerState();
        ls(s);
    }
    return 0;
}
