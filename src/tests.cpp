//#include "../../include/tests.h"
#include "../include/client.h"

void test1()
{
	client c;
	c.simplefs_mkdir("/kasia");
	c.simplefs_create("/kasia/file1", 1, 1, 1);
	c.simplefs_create("/kasia/file2", 1, 1, 1);
	c.simplefs_mkdir("/mirka");
	c.simplefs_create("/mirka/file1", 1, 1, 1);
	c.simplefs_create("/mirka/file2", 1, 1, 1);
	c.simplefs_create("/mirka/file3", 1, 1, 1);
	c.simplefs_create("/mirka/file4", 1, 1, 1);
	c.simplefs_create("/mirka/file5", 1, 1, 1);
	c.simplefs_create("/mirka/file6", 1, 1, 1);
	c.simplefs_create("/mirka/file7", 1, 1, 1);
	c.simplefs_create("/mirka/file8", 1, 1, 1);
	c.simplefs_create("/mirka/file9", 1, 1, 1);
	c.simplefs_mkdir("/mikolaj");
	c.simplefs_mkdir("/mikolaj/dir1");
	c.simplefs_create("/mikolaj/dir1/file1", 1, 1, 1);
	c.simplefs_create("/file6", 1, 1, 1);
	c.simplefs_mkdir("/dir4");

	c.simplefs_create("badFile1", 1, 1, 1);
	c.simplefs_mkdir("badDir1");
	c.simplefs_create("/mikolaj/dir1/file1", 1, 1, 1);
}

void test2()
{
	client c;
	c.simplefs_unlink("/mirka/file1");
	c.simplefs_unlink("/mirka/file3");
	c.simplefs_create("/mirka/file10", 1, 1, 1);
}

void test3()
{
	client c;
	char buf[6000];
	c.simplefs_open("/mirka/file5", WRITE);
	c.simplefs_write(c.fileDescription, buf, 8000);
};
