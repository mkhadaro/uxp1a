/**
<--Opis pliku -->
Plik zródłowy zawiera implementacje
API clienta,slużące do wykonywania określonych funkcjonalności
na systemie plikow
**/
#include "../../include/client.h"

void client::simplefs_open(char* path,int mode)
{
    serverResponse response = sendRequest(OPEN_ACT, path, 0, 0, mode);
    if(response.result < 0)
        fileDescription = -1;
    else
        fileDescription = response.fd;
}

void client::simplefs_unlink(char* name)
{
    serverResponse response = sendRequest(UNLINK_ACT, name, 0, 0, 0);
    if(response.result < 0)
        printf("Blad pzry usuwaniu pliku");
}

void client::simplefs_mkdir(char* path)
{
	serverResponse response = sendRequest(MKDIR_ACT, path, 0, 0, 0);
    if(response.result == -1)
        printf("Blad przytworzeniu katalogu");
}

void client::simplefs_create(char* name,int r, int w, int x)
{
    serverResponse response = sendRequest(CREATE_ACT, name, r, w, x);
    if(response.result < 0)
        printf("Blad przytworzeniu pliku");
}

void client::simplefs_read(int fd,char* buf,int len)
{
    serverResponse response = sendRequest(READ_ACT, "", fd, len, READ);
    if(response.result < 0)
        return;
    for(int i = 0;i < len; ++i)
        buf[i] = fs->dataBlocks[response.result + i ];

    sendRequest(CLOSE, "", fd, 0, 0);
}

void client::simplefs_write(int fd,char* buf,int len)
{
    serverResponse response = sendRequest(WRITE_ACT, "", fd, len, WRITE);
    if(response.result < 0)
        return;
    for(int i = 0;i < strlen(buf); ++i)
        fs->dataBlocks[response.result + i ] = buf[i];

    sendRequest(CLOSE, "", fd, 0, 0);
}

void client::simplefs_lseek(int fd,int whence,int offset)
{
    serverResponse response = sendRequest(LSEEK_ACT, "", fd, offset, whence);
}

