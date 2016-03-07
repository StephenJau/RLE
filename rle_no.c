#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rle.h"

int main(int argc,char * argv[])
{
	ArgCheck(argc);
	int trigger=Trigger(argv[1]);
	if (trigger==0)
		ExtCheck_rle(argv[2]);
	if(trigger==1)
		ExtCheck(argv[2]);
	FILE * fp=fopen(argv[2],"rb");
	if (fp==NULL){
		perror("Error");
		exit(0);
	}
	int fileLen=FileLen(fp);
	unsigned char * data=(unsigned char*)calloc(fileLen,sizeof(unsigned char));
	if(data==NULL){
		perror("Data Error");
		exit(0);
	}
	int readNum=fread(data,1,fileLen,fp);fclose(fp);
	printf("%d elements read into data.\n",readNum);
	unsigned char * dataiterator=data;
	unsigned int BYTEUSAGE=0,buffSize=0;
	switch(trigger)
	{
		case 0:{//comression
			buffSize=fileLen;
			unsigned char * buffer=(unsigned char*)calloc(buffSize,sizeof(unsigned char));
			if(buffer==NULL){
				perror("Buffer Error");
				exit(0);
			}
			printf("%d bytes allocated for buffer.\n",buffSize);
			unsigned char * buffiterator=buffer;
			unsigned char preChar,curChar,wrtChar,len1,len2,size1,size2,size3,size4;
			unsigned short runlen=1;
			unsigned int cunt=1,MAXREACH=0;
			size4=(unsigned char)(fileLen);
			size3=(unsigned char)(fileLen>>8);
			size2=(unsigned char)(fileLen>>16);
			size1=(unsigned char)(fileLen>>24);
			*(buffiterator++)=size1;
			*(buffiterator++)=size2;
			*(buffiterator++)=size3;
			*(buffiterator++)=size4;
			BYTEUSAGE+=4;
			while(cunt<fileLen){
				preChar=*(dataiterator++);
				curChar=*dataiterator;
				cunt++;
				if(NotEqual(preChar,curChar)){
					len2=(unsigned char)(runlen);
					len1=(unsigned char)(runlen>>8);
					wrtChar=preChar;
					*(buffiterator++)=len1;
					*(buffiterator++)=len2;
					*(buffiterator++)=wrtChar;
					BYTEUSAGE+=3;
					runlen=1;
					if(cunt==fileLen-1){//tacle with the end of data.
						len2=(unsigned char)(runlen);
						len1=(unsigned char)(runlen>>8);
						wrtChar=curChar;
						*(buffiterator++)=len1;
						*(buffiterator++)=len2;
						*(buffiterator++)=wrtChar;
						BYTEUSAGE+=3;
					}
				}
				else{
					runlen++;
					if(runlen>MAXREACH)
						MAXREACH=runlen;//get the max repteat times for data type:runlen.
					if(cunt==fileLen){//tacle with the end of data.
						len2=(unsigned char)(runlen);
						len1=(unsigned char)(runlen>>8);
						wrtChar=preChar;
						*(buffiterator++)=len1;
						*(buffiterator++)=len2;
						*(buffiterator++)=wrtChar;
						BYTEUSAGE+=3;
					}
				}
			}
			printf("Maximum repteat is: %d\n", MAXREACH);
			printf("Bytes used in buffer:%d\n",BYTEUSAGE);
			if((BYTEUSAGE-1)%3!=0)
				printf("Bytes used should be 3 multiple.\n");
			char filename[30];
			strcpy(filename,argv[2]);
			strcat(filename,".rle");
			FILE * fpw=fopen(filename,"wb");
			int WrtNum=fwrite(buffer, 1, BYTEUSAGE, fpw);
			printf("Written bytes: %d\n",WrtNum);
			unsigned int compressRate=(BYTEUSAGE*100)/fileLen;
			printf("Compression Rate:%d%%\n",compressRate);
			free(data);
			free(buffer);
			fclose(fpw);
			break;
		}
		case 1:{//decompression
			unsigned char size1,size2,size3,size4;
			size1=*(dataiterator++);
			size2=*(dataiterator++);
			size3=*(dataiterator++);
			size4=*(dataiterator++);
			buffSize=((((((buffSize+size1)<<8)+size2)<<8)+size3)<<8)+size4;
			unsigned char * buffer=(unsigned char*)calloc(buffSize,sizeof(unsigned char));
			if(buffer==NULL){
				perror("Buffer Error");
				exit(0);
			}
			printf("%d bytes allocated for buffer.\n",buffSize);
			unsigned char * buffiterator=buffer;
			unsigned char wrtChar,len1,len2;
			unsigned short runlen=0;
			unsigned int cunt=0,i=0;
			while(cunt<fileLen){
				runlen=0;
				len1=*(dataiterator++);
				len2=*(dataiterator++);
				wrtChar=*(dataiterator++);
				cunt+=3;
				runlen=((runlen+len1)<<8)+len2;
				for(i=0;i<runlen;i++)
					*(buffiterator++)=wrtChar;
				BYTEUSAGE+=runlen;
			}
			printf("cunt:%d,  BYTEUSAGE:%d\n",cunt,BYTEUSAGE);
			char filename[30];
			strcpy(filename,argv[2]);
			DeExtension(filename);
			AddCopy(filename);
			FILE * fpw=fopen(filename,"wb");
			int WrtNum=fwrite(buffer, 1, BYTEUSAGE, fpw);
			printf("Written bytes: %d\n",WrtNum);
			free(data);
			free(buffer);
			fclose(fpw);
			break;
		}
		default:{
			printf("-c for compression, -u for decompression\n");
			exit(0);
			break;
		}
	}



	return 0;
}