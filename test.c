#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#define MSG_TRY "try again\n"

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

void test_block()
{
	char buf[10];
	int n;
	n = read(STDIN_FILENO, buf, 10);
	if(n < 0)
	{
		perror("read STDIN_FILENO");
		exit(1);
	}
	write(STDOUT_FILENO, buf, n);
}
/*
非阻塞地读STDIN，如果发生了阻塞那么会产生一个E
*/
int test_nonblock()
{
	char buf[10];
	int fd, n=-1;
	fd = open("/dev/tty", O_RDONLY|O_NONBLOCK);
	if(fd<0)
	{
		perror("open /dev/tty");
		exit(1);
	}
	while(n<0)
	{
		n = read(fd, buf, 10);
		if(n<0)
		{
			if(errno == EAGAIN)
			{
				sleep(1);
				write(STDOUT_FILENO,MSG_TRY,strlen(MSG_TRY));
			}else{
				perror("read /dev/tty");
				exit(1);
			}
		}
	}
	write(STDOUT_FILENO, buf, n);
	close(fd);
	return 0;
}

void test_book()
{
	char buf[10];
	int fd, n;
	fd = open("/dev/tty", O_RDONLY|O_NONBLOCK);
	if(fd<0) {
		perror("open /dev/tty");
		exit(1);
	}
tryagain:
	n = read(fd, buf, 10);
	if (n < 0) 
	{
		if (errno == EAGAIN) 
		{
			sleep(1);
			write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
			goto tryagain;
		}       
		perror("read /dev/tty");
		exit(1);
	}
	write(STDOUT_FILENO, buf, n);
	close(fd);
}

void test_lseek()
{
	int fd = open("hello1", O_RDWR);
	printf("set the position at begining:%ld\n",lseek(fd, 0, SEEK_SET));
	printf("file length:%ld\n",lseek(fd, 0, SEEK_END));
	printf("get current position:%ld\n",lseek(fd, 0, SEEK_CUR));
	close(fd);

	FILE* fp = fopen("hello1","r+");
	fseek(fp, 0, SEEK_END);
	printf("length of file:%ld\n",ftell(fp));
	fclose(fp);
}
/*
reopen the file and set attr to it.
*/
void test_fcntl()
{
	char buf[10];
    int n;
    int flags;
    flags = fcntl(STDIN_FILENO, F_GETFL);
    flags |= O_NONBLOCK;
    if (fcntl(STDIN_FILENO, F_SETFL, flags) == -1) 
	{
        perror("fcntl");
        exit(1);
    }
tryagain:
    n = read(STDIN_FILENO, buf, 10);
    if (n < 0) {
            if (errno == EAGAIN) {
                sleep(1);	
                write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
                goto tryagain;
    	    }
        perror("read stdin");
        exit(1);
    }
    write(STDOUT_FILENO, buf, n);
}

// int main(int argc, char** argv)
// {
// 	int val;
//     if (argc != 2) {
//         fputs("usage: a.out <descriptor#>\n", stderr);
//         exit(1);
//     }
//     printf("main[0]:%s\nmain[1]%s\n",argv[0], argv[1]);

//     if ((val = fcntl(atoi(argv[1]), F_GETFL)) < 0) {
//         printf("fcntl error for fd %d\n", atoi(argv[1]));
//         exit(1);
//     }
//     switch(val & O_ACCMODE) {
// 	    case O_RDONLY:
//             printf("read only");
//             break;
// 	    case O_WRONLY:
//             printf("write only");
//             break;
// 	    case O_RDWR:            
//             printf("read write");
//             break;
// 	    default:
//             fputs("invalid access mode\n", stderr);
//         	exit(1);
//     }
// 	if(val & O_APPEND)  
//     	printf(", append");
// 	if(val & O_NONBLOCK)           
//         printf(", nonblocking");
//     putchar('\n');
//     return 0;
// }


void test_ioctl()
{
    struct winsize size;
    if (isatty(STDOUT_FILENO) == 0)
            exit(1);
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size)<0) {
            perror("ioctl TIOCGWINSZ error");
            exit(1);
    }
    printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
}

void test_mmap()
{
	int *p;
	int fd = open("hello", O_RDWR);
	if(fd<0)
	{
		perror("open hello");
		exit(1);
	}
	p = mmap(NULL, 6, PROT_WRITE, MAP_SHARED, fd, 0);
	if(p == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}
	close(fd);
	p[0] = 0x30313233;
	munmap(p, 6);
	
}


int  main()
{
	// my_errorno();
	// test_fork();
	// test_base_fIO();
	// test_sys_IO();
	// test_block();
	// test_nonblock();
	// test_lseek();
	// test_fcntl();
	// test_fcntl2();
	// test_ioctl();
	test_mmap();
	return 0;
}