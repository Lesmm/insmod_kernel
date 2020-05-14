#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(void)
{
 int fd;
  char* mem_start;

 fd = open("/dev/shm",O_RDWR);

 if((mem_start = mmap(NULL,4096, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0)) == MAP_FAILED)
 {
 		close(fd)
         printf("mmap failed.\n");
         exit(0);
 }

close(fd)
 printf("mem:%s\n", mem_start);

 return 0;
 }   