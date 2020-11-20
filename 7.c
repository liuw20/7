#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <error.h>
#include <fcntl.h>
#define num 255

void csv2Bin(FILE *fbir,FILE *fp);
void Bincat();
int main(int argc,const char *argv[]){
	char biao1[]="2014310512,Nima Wang,E.E,22\n";
	char biao2[]="2014310513,John Smith,C.S,23\n";
	char biaotou[]="id,name,Department,Age\n";
	FILE *fp;
	FILE *fbir;
	
	fp=fopen("sheet.csv","w+");
	if (!fp)
	{
		printf("error!\n");
		exit(-1);
	}
	fwrite(biaotou,sizeof(biaotou),1,fp);
	fwrite(biao1,sizeof(biao1),1,fp);
	fwrite(biao2,sizeof(biao2),1,fp);

	fbir=fopen("sheet.bin","wb+");
	if (!fbir)
	{
		printf("error!\n");
		exit(-1);
	}

	csv2Bin(fbir,fp);//mission 1
	fclose(fbir);
	Bincat();//mission 2
	
	fclose(fbir);
	fclose(fp);
}

void csv2Bin(FILE *fbir,FILE *fp)
{
	char shuzu[num*8],temp[num*8],line[num*8],line_temp[num*8],daduan_temp[num*8];
	int count=0;
	int ch;
	int i,k,l,j,flag=0;
	int daduan[8];
	int fd1[2],fd2[2],fd3[2];
	int fd4[2],fd5[2],fd6[2];
	pipe(fd2);pipe(fd3);pipe(fd1);//创建管道
	pipe(fd4);pipe(fd5);pipe(fd6);

	fseek(fp,0,SEEK_SET);
	fread(shuzu,sizeof(shuzu),1,fp);

	pid_t pid;

	while(flag<=3){
		pid=fork();
		if(pid<0){
			printf("error in fork");	
			}
		flag++;
		if(pid==0) break;
		
	}
	//对父进程进行操作
	if(pid>0){ //对每行进行写入
		for(l=0;l<3;l++){
			if(l==0){
				for(i=0,j=0;j<num;j++,i++){
					temp[i]=shuzu[j];			
					if((int)shuzu[j]==0) {
						close(fd1[0]);
						write(fd1[1],temp,i);//遇到‘0’字符后将该行字符串写入到fd[1]
						break;
						}
					}
				}
			if(l==1){
				for(i=0,j=j+1;j<num;j++,i++){
					temp[i]=shuzu[j];			
					if((int)shuzu[j]==0) {
						close(fd2[0]);
						write(fd2[1],temp,i);
						break;
						}
					}
				}
			if(l==2){
				for(i=0,j=j+1;j<num;j++,i++){
					temp[i]=shuzu[j];			
					if((int)shuzu[j]==0) {
						close(fd3[0]);
						write(fd3[1],temp,i);
						break;
						}
					}
			    	}
		}
		waitpid(pid,NULL,0);
		close(fd4[1]);
		read(fd4[0],line,num*8);
		
	
		//传回
		for(l=0,k=0;l<=num*8;l++){
			fprintf(fbir,"%d",line[l]);
			if(line[l]==0) k=k+1;
			else k=0;
			if(k==12) break;
			}

		
		close(fd5[1]);
		read(fd5[0],line,num*8);
		for(l=0,k=0;l<=num*8;l++){
			fprintf(fbir,"%d",line[l]);
			if(line[l]==0) k=k+1;
			else k=0;
			if(k==12) break;
			}
		
		close(fd6[1]);
		read(fd6[0],line,num*8);
		for(l=0,k=0;l<=num*8;l++){
			fprintf(fbir,"%d",line[l]);
			if(line[l]==0) k=k+1;
			else k=0;
			if(k==12) break;
			}
	}

	if(flag==1){
		for(i=0;i<num;i++)
			line_temp[i]='!';

		close(fd1[1]);
		read(fd1[0],line,num*8);
		for(k=0;k<num*8;k++){
			line_temp[k]=line[k];
			if((int)line[k]==0) { break;}
		}
	
		for(k=0;k<(num*8-1) && line_temp[k]!='!';k++){
			ch=line_temp[k];

			for(l=0;l<=7;l++){//daduan
				daduan[l]=ch>>l&1;
				daduan_temp[k*8+l]=daduan[l];
				//printf("%d ",daduan[l]);
			}	
		}	
		
			close(fd4[0]);			
			write(fd4[1],daduan_temp,(k-1)*8+7);
	}

	if(flag==2){
		//printf("pid 10 \n");
		for(i=0;i<num;i++)//shuzu fu kongzhi
			line_temp[i]='!';

		close(fd2[1]);
		read(fd2[0],line,num*8);
		for(k=0;k<num*8;k++){
			line_temp[k]=line[k];
			if((int)line[k]==0) { break;}
		}
		//printf("pid 11 \n");	
		for(k=0;k<(num*8-1) && line_temp[k]!='!';k++){
			ch=line_temp[k];
			//printf("%c ",ch);
			for(l=0;l<=7;l++){//daduan
			daduan[l]=ch>>l&1;
			daduan_temp[k*8+l]=daduan[l];
			}		
		}	
			close(fd5[0]);			
			write(fd5[1],daduan_temp,(k-1)*8+7);
	}

	if(flag==3){
		//printf("pid 20 \n");
		for(i=0;i<num;i++)//shuzu fu kongzhi
			line_temp[i]='!';
		
		close(fd3[1]);
		read(fd3[0],line,num*8);
		for(k=0;k<num*8;k++){
			line_temp[k]=line[k];
			if((int)line[k]==0) { break;}
		}
		
		for(k=0;k<(num*8-1) && line_temp[k]!='!';k++){
			ch=line_temp[k];

			for(l=0;l<=7;l++){//daduan
			daduan[l]=ch>>l&1;
			daduan_temp[k*8+l]=daduan[l];
			}		
		}
		close(fd6[0]);			
		write(fd6[1],daduan_temp,(k-1)*8+7);
	}
}

void Bincat()
{
	int fd,nread;
	struct stat sb;
	char *mapped;

	int i,j,flag=0,itemp;
	char temp_binary[8];
	int int_binary[8];
	char tram;

	if((fd = open("sheet.bin",O_RDWR))<0){
		printf("open error\n");
	}

	if((fstat(fd,&sb))==-1){
		printf("fstat error\n");
	}

	if((mapped = mmap(NULL,sb.st_size,PROT_READ,MAP_SHARED,fd,0))==(void*) -1){
		//printf("mmap error\n");
	}

	while(flag<=num*8){
		if(mapped[flag]==0) break;

		j=0;
		for(i=flag+7;i>=flag && i<flag+8;i--){
			temp_binary[j%8]=mapped[i];
			j=j+1;
		}

		for(i=0;i<8;i++){
			int_binary[i]=(temp_binary[i]-'0');	
		}

		itemp=1;
		tram=0;
		for(i=7;i>=0;i--){
			tram=tram+int_binary[i]*itemp;
			itemp=itemp*2;
		}
		
		printf("%c",tram);
		flag=flag+8;

	}
}