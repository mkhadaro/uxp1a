#ifndef CLIENT_H
#define CLIENT_H


class client
{
    public:
        client();

        void simplefs_open(char* name,int mode);
        void simplefs_unlink(char* name);
        void simplefs_mkdir(char* name);
        void simplefs_create(char* name,int mode);
        void simplefs_read(int fd,char* buf,int len);
        void simplefs_write(int fd,char* buf,int len);
        void simplefs_lseek(int fd,int whence,int len);

    protected:
        void send_msg_to_server();
    private:
};

#endif // CLIENT_H
