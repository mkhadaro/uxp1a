#include <iostream>
#include "include/server.h"
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int arc,char** argv)
{
    server server;
    
    server.setBlockBit(0, 7, true);
    server.setBlockBit(12, 7, true);
    bool inode = server.canAddToFile(7, 5);
    printf(inode ? "true\n" : "false\n");


    return 0;
}
