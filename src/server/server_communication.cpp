/**
<--Opis pliku -->
Plik zródłowy zawiera implementacje
wybranych funkcji servera:

funkcja work - odpowiada za komunikacje z klientami
->odczytuje z fifo komunikaty
->obsluguje
->zapisuje odpowiedzi do fifo klienta
**/
#include "../../include/server.h"

void server::work()
{
  char buf[128];
  int n = 0;
  while(true)
  {
    clientRequest req;
    int fd = open(SERVER_FIFO, O_RDONLY);
    n = read(fd, &req, sizeof(req));
    close(fd);
    if(n == 0)
      break;
    int result = -1;
    switch(req.type)
    {
      case OPEN_ACT:
                    result = simplefs_open(req.path,req.mode);
                    break;
      case UNLINK_ACT:
                    result = simplefs_unlink(req.path);
                    break;
      case MKDIR_ACT:
                    result = simplefs_mkdir(req.path);
                    break;
      case CREATE_ACT:
                    result = createFile(req.path,TYPE_FILE,req.fd,req.size,req.mode);
                    break;
      case READ_ACT:
                    result = simplefs_read(req.fd,req.size);
                    break;
      case LSEEK_ACT:
                    result = simplefs_lseek(req.fd,req.type,req.size);
                    break;
      case CLOSE:
                    result = close(req.fd);
                    break;
      case WRITE_ACT:
                    int nodeNumber = getNodeNumberByFD(req.fd);
                    if((nodeNumber < 0) || (checkMode(nodeNumber,req.mode)))//sprawdzam uprawnienia
                        result = -1;
                    else
                        result = writeToFile(nodeNumber, req.size);
                    break;
    }
    printf("%s\n", req.path);
    serverResponse res;
    res.result = result;
    strcpy(res.clientFifoId, req.clientFifoId);
    int clientfd = open(req.clientFifoId, O_WRONLY);
    write(clientfd, &res, sizeof(res));
    close(clientfd);
  }
}

