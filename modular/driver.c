#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<assert.h>

int main(){
	int fd;
	char my_array[1024];
	char kbuff[1024];
	ssize_t bytes_read;
	printf("write some message to the device:\n");
	scanf("%[^\n]s",my_array);
	fd = open("/dev/dev_naco", O_RDWR);
	write(fd,my_array,strlen(my_array));
	bytes_read = read(fd,kbuff,1024);
	printf("***************************************************\n");
	printf("%s\n", kbuff);
	printf("read %d bytes of data\n", strlen(kbuff));
	close(fd);
	return 0;
}




