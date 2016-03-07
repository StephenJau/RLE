#ifndef _RLE_H
#define _RLE_H
#endif

void ArgCheck(int argc){
	if (argc<3){
		printf("Please Specify the Control Parameter and filename\n");
		printf("'-c' for compress\n");
		printf("'-u' for uncompress\n");
		exit(0);
	}
}

int Trigger(char * argv)
{
	if(strcmp("-c",argv)==0)
		return 0;
	else if (strcmp("-u",argv)==0)
		return 1;
	else
		return -1;
}

void ExtCheck(char* argv){
	char* ext = strrchr(argv, '.');
	if(strcmp(ext+1,"rle")){
		perror("File extension Error");
		exit(0);
	}
}
void ExtCheck_rle(char* argv){
	char* ext=strrchr(argv,'.');
	if(!strcmp(ext,".rle")){
		perror("Cannot do rle compression double times.\n");
		exit(0);
	}
}

int FileLen(FILE * fp){
	fseek(fp,0, SEEK_END);
	int num=ftell(fp);
	int ifsuccess=fseek(fp,0,SEEK_SET);
	if(ifsuccess!=0){
		perror ("Error returning file pointer to beginning");
		exit(0);
	}
	return num;
}

void DeExtension(char * argv)
{
	char * location=strrchr(argv,'.');
	*location='\0';
}
void AddCopy(char * argv)
{
	char * location=strrchr(argv,'.');
	int extLen=strlen(location),i;
	char ext[extLen];
	for(i=0;i<extLen;i++)
		ext[i]=*(location+i);
	strcpy(location,"_copy");
	strcat(argv,ext);
	printf("New File Name: %s\n",argv );
}
bool NotEqual(char preChar,char curChar){
	if (preChar==curChar)
		return false;
	else
		return true;
}
