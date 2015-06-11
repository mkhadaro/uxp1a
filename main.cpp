#include <iostream>
#include "include/shared_memory.h"

#include "include/tests.h"
#include "include/server.h"
#include "include/client.h"
#include "include/interface.h"


using namespace std;

void showFiles(FileSystem * fs,INode *inode)
{
    for(int j =0; j < sizeof(inode->pointers)/sizeof(int); ++j)
    {
        std::cout<<inode->pointers[j]<<" ";
        if(fs->inodes[inode->pointers[j]].type == TYPE_HELPER)
        {
                std::cout<<"\n\t"<<"\t";
                showFiles(fs,&fs->inodes[inode->pointers[j]]);
        }
    }
}

void ls(FileSystem * fs)
{
    for(int i = 0; i < INODE_COUNT ;++i)
    {
        if(fs->inodes[i].type != -1)
        {
            std::cout<<fs->inodes[i].name <<"\t"<<i<<"\t";
            showFiles(fs,&fs->inodes[i]);
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
    server.simplefs_mkdir("/eres");
    server.simplefs_mkdir("/root");
    server.createFile("/root/abs",TYPE_FILE, 1, 1, 1);
    server.simplefs_mkdir("/root/home");
    server.simplefs_mkdir("/root/home/ala");

}

int testUnlink(server & server,char *nazwa)
{
    server.simplefs_unlink(nazwa);
    return 0;
}

int testOpenFile(server & server,char *name)
{
    std::cout<<"file description "<<server.simplefs_open(name,READ)<<std::endl;
}

int main(int argc,char** argv)
{

    server server;
    testCreateDir(server);
    ls(server.fs);
    server.showServerState();
    testUnlink(server,"/root/abs");
    ls(server.fs);
    server.showServerState();

    //<----------------------create file ,open,write and read ----------->
    std::cout<<"Tworze pliki do zapisu i odczytu"<<std::endl;
    int responseCreste = server.createFile("/opt/kasia",TYPE_FILE,1,1,0);
    if(responseCreste < 0)
        return -1;
    int fd = server.simplefs_open("/opt/kasia",WRITE);
    if(fd == -1)
        return -1;
    int length = 10;
    int nodeNumber = server.getNodeNumberByFD(fd);
    char bufor2[10] = "makaron";
    if(server.checkMode(nodeNumber,WRITE) < 0)
        return -1;
    int positionToWrite = server.writeToFile(nodeNumber,length);
    if(positionToWrite != -1)
    for(int i = 0;i < length; ++i)
            server.fs->dataBlocks[positionToWrite + i] = bufor2[i];

    std::cout<<"Zapisano do pliku slowo-->";
    if(positionToWrite != -1)
    for(int i = 0;i < length; ++i)
        std::cout<<server.fs->dataBlocks[positionToWrite + i ];
    std::cout<<std::endl;

    positionToWrite = server.writeToFile(nodeNumber,length);

    if(positionToWrite != -1)
    for(int i = 0; i < length; ++i)
            server.fs->dataBlocks[positionToWrite + i] = bufor2[i];

    std::cout<<"Dopisuje do pliku ";
    for(int i = 0;i < length; ++i)
        std::cout<<server.fs->dataBlocks[positionToWrite + i ];

    int address = server.fs->inodes[nodeNumber].address;
    std::cout<<"Zawartosc pliku: ";
    for(int i = 0;i < server.fs->inodes[nodeNumber].size; ++i)
        std::cout<<server.fs->dataBlocks[address + i ];
    // <-------------------------------------------------------------->
    server.close(fd);

    fd = server.simplefs_open("/opt/kasia",READ);
    if(fd == -1)
        return -1;
    std::cout<<"\nCzytam z pliku"<<std::endl;
    char bufor[60] = {0};
    int blockNumber = server.simplefs_read(fd,length);
    if(blockNumber >= 0)
    for(int i = 0;i < length; ++i)
    {
        std::cout<<server.fs->dataBlocks[blockNumber + i ];
        bufor[i] = server.fs->dataBlocks[blockNumber + i ];
    }

    std::cout<<"\nzawartosc bufora po przeczytaniu"<<bufor<<std::endl;
    server.close(fd);

/**********************************************************************/
   char* names = "/root/abs";
   std::cout<<"Tworze plik "<<names<<std::endl;
   server.createFile(names,TYPE_FILE,1,1,0);
   fd = server.simplefs_open(names,WRITE);
    if(fd == -1)
        return -1;
    length = 5;
    nodeNumber = server.getNodeNumberByFD(fd);
    char bufor3[5] = "ania";
    if(server.checkMode(nodeNumber,WRITE) < 0)
        return -1;
    std::cout<<"Pisze do pliku "<<names<<std::endl;
    positionToWrite = server.writeToFile(nodeNumber,length);

    if(positionToWrite != -1)
    for(int i = 0;i < length; ++i)
            server.fs->dataBlocks[positionToWrite + i] = bufor3[i];

    std::cout<<"\nZawartosc pliku po zapisie ";
    if(positionToWrite != -1)
    for(int i = 0;i < length; ++i)
        std::cout<<server.fs->dataBlocks[positionToWrite + i ];
    std::cout<<std::endl;
    server.close(fd);

    std::cout<<"Czytam z pliku "<<names<<std::endl;
    fd = server.simplefs_open("/root/abs",READ);
    if(fd == -1)
        return -1;
    char bufor4[5] = {0};
    blockNumber = server.simplefs_read(fd,length);
    std::cout<<"Zawartosc pliku "<<names<<" "<<std::endl;
    if(blockNumber >= 0)
    for(int i = 0;i < length; ++i)
    {
        std::cout<<server.fs->dataBlocks[blockNumber + i ];
        bufor4[i] = server.fs->dataBlocks[blockNumber + i ];
    }

    std::cout<<"\nZawartosc bufora po odczycie "<<bufor4<<std::endl;
    server.close(fd);

   server.createFile("/root/rwx",TYPE_FILE,1,1,0);
   fd = server.simplefs_open("/root/rwx",WRITE);
       if(fd == -1)
        return -1;

    length = 5;
    nodeNumber = server.getNodeNumberByFD(fd);

    if(server.checkMode(nodeNumber,WRITE) < 0)
        return -1;

    positionToWrite = server.writeToFile(nodeNumber,length);

    if(positionToWrite != -1)
    for(int i = 0;i < length; ++i)
            server.fs->dataBlocks[positionToWrite + i] = bufor3[i];

    std::cout<<"Dane zapisane do pliku ---";
    if(positionToWrite != -1)
    for(int i = 0;i < length; ++i)
        std::cout<<server.fs->dataBlocks[positionToWrite + i ];
    std::cout<<std::endl;
    server.close(fd);
    ls(server.fs);
    server.showServerState();
    return 0;
}
