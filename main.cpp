#include <iostream>
#include "include/server.h"

using namespace std;

int main(int arc,char** argv)
{
    cout << "Hello world!" << endl;
    server sever;
    sever.initDataInSharedMemory();
    return 0;
}
