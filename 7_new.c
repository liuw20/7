#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 #include<sys/mman.h> 
#include <fcntl.h> 
#include <linux/fb.h> 
#include <sys/ioctl.h> 


//  void csv2Bin(FILE *fp,FILE *fpb);
struct data
{
char data[20];
}sheet[3][4];
typedef struct  data df;
// void csv_bin(df a);
void Bincat();
int main()
{
        FILE *fp = NULL;
    	char *line,*record;
    	char buffer[1024];
		if ((fp = fopen("sheet.csv","r")) == NULL)
		{
			printf("打开文件错误！");
		}
		FILE *sheet_bin;//父进程拼接文件
		sheet_bin=fopen("sheet_bin","ab+");
		for (int i = 0; i <3; i++)//每次循环读取一行，分发进程
		{
			
			line=fgets(buffer,sizeof(buffer),fp);
			record = strtok(line, ",");
			strcpy(sheet[i][0].data, record);//将数据存入结构体数组中
			int j=1;
			while (record != NULL)//读取每一行的数据
			{
				
				record = strtok(NULL, ",");
				if (j==4)
				{
					break;
				}
				strcpy(sheet[i][j].data,record);
				j++;
			}
			
			int fd[2];
			df tem[1][4];
			pipe(fd);//创建管道
			pid_t fpid;
			fpid=fork();
			if (fpid<0)
			{
				printf("创建子进程失败！");
			}
			//利用进程间通信机制进行转码
			if (fpid>0)//父进程进行读取
			{
				close(fd[0]);//写之前关闭读
				write(fd[1],sheet[i][0].data,80);//写入数据
			}
			else if (fpid==0)
			{
				close(fd[1]);//读之前先关闭写
				read(fd[0],tem[0][0].data,80);//读取至tem
				fwrite(tem[0][0].data,20,4,sheet_bin);
				close(fd[0]);
				exit(0);
			}
		}
		fclose(sheet_bin);
		Bincat();
// 		df sheet_father[3][4];
// fread(sheet_father[0][0].data,20,12,sheet_bin);
//  for (int i = 0; i < 3; i++)
// 	{
// 		for (int j = 0; j < 4; j++)
// 		{
// 			printf("%s ",sheet_father[i][j].data);//将二进制文件打印出来
// 		}
	
}

void Bincat()
{
		int fd;
       char *mapped;
        if((fd = open("sheet_bin",O_RDWR))<0)
		{
		printf("open error\n");
	}
    if((mapped = mmap(NULL,240,PROT_READ,MAP_SHARED,fd,0))==MAP_FAILED){
		printf("mmap error\n");
	}
    char data[240];
    for (int i = 0; i < 240; i++)
    {
        data[i]=mapped[i];
        if ((i%20)==19)
        {
            data[i]=' ';
        }
    }
    for(int i =0;i<240;i++)
    {
        printf("%c",data[i]);
    }
    close(fd);
}




        