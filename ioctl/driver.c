#include<stdlib.h>
#include<stdio.h>
#include<sys/ioctl.h>
#include<fcntl.h>

#define NACO_RD _IOR('a','a',int32_t*)
#define NACO_WRT _IOW('a','b', int32_t*)

int32_t sent,recieved;
int choice;

int main(){
	int fd;
	fd = open("/dev/mawanda", O_RDWR);
	if(fd < 0 ){
		printf("Can not locate specified file\n");
		return -1;
	}
	while(1){
		printf("*******choose from the following menue***********\n");
		printf("....1 stands for writing........\n");
		printf("....2 stands for reading........\n");
		printf("otherwise nothing will be returned\n");
		scanf("%d",&choice);
		switch(choice){
			case 1:
				printf("you have selected writing\n");
				printf("write any random value to the device\n");
				scanf("%d", &sent);
				ioctl(fd, NACO_WRT,(int32_t *)&sent);
				break;
			case 2:
				printf("you have selected reading from device mawanda\n");
				printf("********************************************\n");
				ioctl(fd, NACO_RD,(int32_t *)&recieved);
				printf("value read from the file is %d\n",recieved);
				break;
			default:
				printf("Wrong option. please try again \n");
				break;
			}
		}
	return 0;
}
		 
