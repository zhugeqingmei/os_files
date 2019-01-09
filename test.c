#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*0-133,every number has it's own meaning.*/
void my_errorno()
{
	perror(strerror(0));
	perror(strerror(10));
}

/*
using fork()
*/
void test_fork()
{
	pid_t pid;
	char *message;
	int n;
	pid = fork();
	if(pid<0)
	{
		perror("fork failed");
		exit(1);
	}
	if(pid == 0)
	{
		message = "This is the child\n";
		n = 6;
	}
	else{
		message = "This is the parent\n";
		n = 3;
	}
	for(;n > 0; n--)
	{
		printf(message);
		sleep(1);
	}
}
/*

*/
void test_base_fIO()
{
	
	char s[100];
    FILE* fp = fopen("/home/meng/code/linux_c/hello1","r+");
    printf("address of FILE:%p\n", fp);
    
    printf("a char in file:%c\n",fgetc(fp));
    fgets(s,100,fp);
    printf("rest of :%s\n",s);
    perror(strerror(fclose(fp)));
    
    printf("%x\n",EOF);
    
    //just write a char to stdout
    //this is a sys call
    write(0,s,10);

}
/*
if using system call ,return the number.
*/
void test_sys_IO()
{
	int fd = open("hello1",O_RDWR|O_APPEND|O_CREAT,0644);
	printf("fd:%d\n",fd);
	write(fd, "add", 3);
	close(fd);

	//open file akae.txt,write,
	// fd = open("akae.txt",O_WRONLY|O_APPEND);
	// perror(strerror(fd));

	// fd = open("akae.txt",O_WRONLY|O_CREAT,0644);
	// if(fd<0)
	// 	perror(strerror(fd));

	//TRUNC TO 0 BYTE
	// fd = open("akae.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
	// if(fd<0)
	// 	perror(strerror(fd));

	//
	fd = open("akae.txt",O_WRONLY|O_CREAT|O_EXCL,0644);
	if(fd<0)
		perror(strerror(fd));
}

int  main()
{
	// my_errorno();
	// test_fork();
	// test_base_fIO();
	test_sys_IO();
	return 0;
}