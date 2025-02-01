#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#include<syslog.h>

int main(int argc, char *argv[])
{

    printf("File read write\n");

    // Open a log
    openlog("writer", LOG_PID, LOG_USER);
    // Check if 3 arguments are given
    if(argc != 3)
    {
        printf("Wrong input\n");
    }
    printf("You have entered %d argumnent and %s\n", argc, argv[1]);
    printf("Content is %s\n", argv[2]);

    const char *file_path = argv[1];
    const char *file_content = argv[2];
    // Opening the file with Read only permission
    int fd;

    fd = open(argv[1], O_RDWR | S_IRWXG);
    if(fd == -1)
    {
        printf("File open process failed\n");
    }

    // Reading the file
    char word[5];
    ssize_t nr;
    nr =  read(fd, &word, sizeof(char)*5);
    if(nr == -1)
    {
        printf("File read process failed\n");
        syslog(LOG_ERR, "File Read Error\n");
    }
    printf("Content of the file %s\n", word);

    //writing the file
    ssize_t wr;

    wr = write(fd, argv[2], strlen(argv[2]));
    if(wr == -1)
    {
        printf("Write file operation failed\n");
    }
    printf("Write sucssesful\n");

    syslog(LOG_DEBUG, "Writing content %s to a file\n", file_content);

    // Closing log
    closelog();
}