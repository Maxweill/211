/*
 * y86emul.c
 * 
 * Copyright 2016 Maxwell Mucha <mem434@null.cs.rutgers.edu>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include  <string.h>
int HexToDec(char* input) //transforms hex to dec 
{
	int value=0;
	char temp;
	
	while (*input!='\0')
	{
		if (!isxdigit(*input))
		{
				fprintf(stderr, "ERROR: Incorrect Hexadecimal Number.\n");
				return -1;
		}
		temp = toupper(*input++);
		temp-='0';
		if (temp>9) //if value of temp is 16-10, find its relative value by forcing it into the decimal 0-9 segment
		{
		temp = temp-7;	
		}
		value = (value<<4) + temp; //shift 4 bits (this means we can only have a max of 8 hex digits ):
		
	}
	return value;
}
int main(int argc, char **argv)
{
    if (argc<1)
    {
		fprintf(stderr, "ERROR: incorrect argument");
	}
	
	if (argv[1][0]=='-' && argv[1][1]=='h')
	{
			printf("USAGE: y86emul [-h] <.y86 file>");
			return -1;
	}
	
    char const* const fileName = argv[1]; 
    FILE* file = fopen(fileName, "r");
    char line[512];
    int blocksize;
    unsigned char* membuf;
    int reg[8];
    int startloc;
    int debug=0;
    int flag[3]; //OF ZF SF (in that order)
    int hastxt=0;
	int hassize=0;
    while (fgets(line, sizeof(line), file)) 
    {
        char p1[512];
        char p2[512];
        char p3[512];
        unsigned int* yip=malloc(256);
        
        sscanf(line, "%s %[^\r\n]", p1, p2);
		if(debug)
		{
		printf("%s\n",line);
		}
        if (p1[0]=='.' && p1[1]=='s' && p1[2]=='i') //.size directive
        {
		hassize=1;
		int temp = HexToDec(p2);
		//int temp = atoi(p2);
		membuf = calloc(temp,1);
		if(debug)
		{printf("*Assigning size of memory buffer to be %i*\n", temp);} //printout
		blocksize=temp;
		}
		
		if(p1[0]=='.' && p1[1]=='s' && p1[2]=='t') //.string directive
		{	
			sscanf(p2, "%x %[^\r\n]",yip,p3);
			if(debug)
			{printf("instruction pointer at %i:\n",*yip);
			printf("adding string in hex form: %s:\n",p3);}
			
			memcpy(membuf+*yip,p3+1,strlen(p3)-2);
			
			membuf[*yip+strlen(p3)-1]='\0';
			
		}
		
		if(p1[0]=='.' && p1[1]=='l' && p1[2]=='o')//.long directive
		{
			sscanf(p2, "%x %[^\r\n]",yip,p3);
			unsigned int y = atoi(p3);
			if(debug)
			{printf("instruction pointer at %i\n",*yip);
			
			printf("value of %x\n",y);}
			memcpy(membuf+*yip,&y,sizeof(int));
		}
		
		if(p1[0]=='.' && p1[1]=='b' && p1[2]=='y')//.byte directive
		{
			sscanf(p2, "%x %[^\r\n]",yip,p3);
			unsigned int byte = HexToDec(p3);
			if(debug)
			{
			printf("instruction pointer at %i\n",*yip);
			printf("value of %x\n",byte);}
			memcpy(membuf+*yip,&byte,1);
		}
		
		if(p1[0]=='.' && p1[1]=='t' && p1[2]=='e')//.text directive
		{
			hastxt=1;
			sscanf(p2, "%x %[^\r\n]",yip,p3);
			startloc = *yip;
			//printf("instruction pointer at %i\n",*yip);
			//printf("\n%s\n",p3);
			char subbuf[2];
			int i=0;
			while(i<strlen(p3))
			{
				subbuf[0]=p3[i];
				subbuf[1]=p3[i+1];
				subbuf[2]='\0';
				//printf("%c %c \n",subbuf[0],subbuf[1]);
				i+=2;
				unsigned int byte = HexToDec(subbuf);
				//printf("value of %x\n",byte);
				memcpy(membuf+*yip,&byte,1);
				*yip+=1;
			}
		}
	}//fetch complete
	 int i=0;
	 if(!hastxt)
	 {
		fprintf(stderr,"ERROR: no .text instruction found.");
		return -1;
	 }
	 if(!hassize)
	 {
		fprintf(stderr,"ERROR: not .size instruction found.");
		return -1;
	 }
	
	if(debug){
	printf("\n");
	for(;i<blocksize;i++)
		{
		 if (membuf[i]==0)
		 printf("0");
		 
		 printf("%x",membuf[i]);
		}
	}
	//begin execute
	i=startloc;
	if(debug)
	printf("\n");
	
	for (;membuf[i]!=0x10;)
	{
		if (membuf[i]==0x00)//nop function
		{
			printf("NOP\n");
			i++;
		}
		else if (membuf[i]==0x20)//rrmovl
		{
			int a = membuf[i+1];
			a = a/16;
			int b = membuf[i+1];
			b= b%16;
			if(debug){
			printf ("Moving contents from register %i into register %i\n", a,b);}			
			reg[b]=reg[a];
			i+=2;
		}
		else if (membuf[i]==0x30)//irmovl
		{
			int b = membuf[i+1];
			b = b%16;
			int V;
			memcpy(&V,membuf+i+2,4);
			reg[b]=V;
			if(debug){
			printf("Moving value %i into register %i\n", reg[b],b);
			printf("THE CURRENT VALUE OF REGISTER %i is %i\n",b,reg[b]);}
			i+=6;
		}
		else if (membuf[i]==0x40)//rmmovl
		{
			int a = membuf[i+1];
			a = a/16;
			int b = membuf[i+1];
			b= b%16;
			int D;
	
			memcpy(&D,membuf+i+2,4);
			int L = reg[b]+D;
			
			memcpy(membuf+L,reg+a,4);
			i+=6;
			if(debug){
			printf("Moving value from register %i to memloc %i",a,reg[b]+D);}
		} 
		else if (membuf[i]==0x50)//mrmovl
		{
			int a = membuf[i+1];
			a = a/16;
			int b = membuf[i+1];
			b= b%16;
			
			int D;
			memcpy(&D,membuf+i+2,4);
			int L = reg[b]+D;
			memcpy(reg+a,membuf+L,4);
			i+=6;
			if(debug){
			printf("Moving value to register %i from memloc %i",a,reg[b]+D);}
		}
		else if (membuf[i]==0x60)//addl
		{
			int a = membuf[i+1];
			a = a/16;
			int b = membuf[i+1];
			b= b%16;
			int temp;
			temp=reg[a]+reg[b];
			if(debug){
			printf ("Adding %i and %i\n",reg[a],reg[b]);}
			if(reg[a]>0 && reg[b]>0 && reg[a]+reg[b]<0)
			{flag[0]=1;}
			else
			{flag[0]=0;}
			
			if(reg[a]<0 && reg[b]<0 && reg[a]+reg[b]>0)
			{flag[0]=1;}
			else
			{flag[0]=0;}
			
			if(temp ==0)
			{flag[1]=1;}
			else
			{flag[1]=0;}
			
			if(temp < 0)
			{flag[2]=1;}
			else
			{flag[2]=0;}
			
			reg[b]=temp;
			i+=2;
		}
		else if (membuf[i]==0x61)//subl
		{
			int a = membuf[i+1];
			a = a/16;
			int b = membuf[i+1];
			b= b%16;
			int temp;
			temp=reg[b]-reg[a];
			if(debug){
			printf ("subtracting %i from %i\n",reg[a],reg[b]);
			}
			if(reg[a]<0 && reg[b]>0 && reg[a]-reg[b]>0)
			{flag[0]=1;}
			else
			{flag[0]=0;}
			
			if(reg[a]>0 && reg[b]<0 && reg[a]-reg[b]<0)
			{flag[0]=1;}
			else
			{flag[0]=0;}
			
			if(temp ==0)
			{flag[1]=1;}
			else
			{flag[1]=0;}
			
			if(temp < 0)
			{flag[2]=1;}
			else
			{flag[2]=0;}
			
			reg[b]=temp;
			
			i+=2;
		}
		else if (membuf[i]==0x62)//andl
		{
			int a = membuf[i+1];
			a = a/16;
			int b = membuf[i+1];
			b= b%16;
			
			reg[b]=reg[a] & reg[b];
			
			flag[0]=0;
			if (reg[b]==0)
			{flag[1]=1;}
			else
			{flag[1]=0;}
			
			if (reg[b]<0)
			{flag[2]=1;}
			else
			{flag[2]=0;}
			
			i+=2;
		}
		else if (membuf[i]==0x63)//xorl
		{
			int a = membuf[i+1];
			a = a/16;
			int b = membuf[i+1];
			b= b%16;
		
			reg[b]=reg[a] ^ reg[b];
			
			flag[0]=0;
			
			if (reg[b]==0)
			{flag[1]=1;}
			else
			{flag[1]=0;}
			
			if (reg[b]==0)
			{flag[1]=1;}
			else
			{flag[1]=0;}
			
			if (reg[b]<0)
			{flag[2]=1;}
			else
			{flag[2]=0;}
			
			i+=2;
		}
		else if (membuf[i]==0x64)//mull
		{
			int a = membuf[i+1];
			a = a/16;
			int b = membuf[i+1];
			b= b%16;
			int temp;
			temp=reg[a] * reg[b];
			
			if(reg[a]>0 && reg[b]>0 && temp<0)
			{flag[0]=1;}
			else
			{flag[0]=0;}
			
			
			if(reg[a]<0 && reg[b]<0 && temp<0)
			{flag[0]=1;}
			else
			{flag[0]=0;}
			
			
			if(reg[a]>0 && reg[b]<0 && temp>0)
			{flag[0]=1;}
			else
			{flag[0]=0;}
			
			
			if(reg[a]<0 && reg[b]>0 && temp>0)
			{flag[0]=1;}
			else
			{flag[0]=0;}
			
		
			reg[b]=temp;
			
			if (reg[b]==0)
			{flag[1]=1;}
			else
			{flag[1]=0;}
			
			if (reg[b]<0)
			{flag[2]=1;}
			else
			{flag[2]=0;}
			
			
			i+=2;
		}
		else if (membuf[i]==0x65)//cmpl
		{
			int a = membuf[i+1];
			a = a/16;
			int b = membuf[i+1];
			b= b%16;
			int temp;
			temp=reg[b]-reg[a];
			if(debug){
			printf ("CMP:subtracting %i from %i\n",reg[a],reg[b]);
			}
			if(reg[a]<0 && reg[b]>0 && reg[a]-reg[b]>0)
			{flag[0]=1;}
			else
			{flag[0]=0;}
			
			if(reg[a]>0 && reg[b]<0 && reg[a]-reg[b]<0)
			{flag[0]=1;}
			else
			{flag[0]=0;}
			
			if(temp ==0)
			{flag[1]=1;}
			else
			{flag[1]=0;}
			
			if(temp < 0)
			{flag[2]=1;}
			else
			{flag[2]=0;}
			
			
			i+=2;
		}
		else if (membuf[i]==0x70)//jmp
		{
			int D;
			memcpy(&D,membuf+i+1,4);
			i=D;
		}
		else if (membuf[i]==0x71)//jle SF xor OF | ZF
		{
			int D;
			memcpy(&D,membuf+i+1,4);
			
			if((flag[2]==1) != (flag[1]==1 || flag[0]==1))
			{i=D;}
			else
			{i+=5;}
			
		}
		else if (membuf[i]==0x72)//jl SF xor OF
		{
			int D;
			memcpy(&D,membuf+i+1,4);
			
			if(flag[0]!=flag[2])
			{i=D;}
			else
			{i+=5;}
		}
		else if (membuf[i]==0x73)//je ZF
		{
			int D;
			memcpy(&D,membuf+i+1,4);
			//printf("%i",D);
			if(flag[1]==1)
			{i=D;}
			else
			{i+=5;}
		}
		else if (membuf[i]==0x74)//jne !ZF
		{
			int D;
			memcpy(&D,membuf+i+1,4);
			
			if(flag[1]==0)
			{i=D;}
			else
			{i+=5;}
		}
		else if (membuf[i]==0x75)//jge !(SF xor OF)
		{
			int D;
			memcpy(&D,membuf+i+1,4);
			
			if(flag[2]==flag[0])
			{i=D;}
			else
			{i+=5;}
		}
		else if (membuf[i]==0x76)//jg SF == OF &!zf
		{
			int D;
			memcpy(&D,membuf+i+1,4);
			
			if(flag[2]==flag[0] && flag[1]==0)
			i=D;
			else
			{i+=5;}
		}
		else if (membuf[i]==0x80)//call
		{
			reg[4]=reg[4]-4;//decrement the sp
			int tmp=i+5;
			memcpy(membuf+reg[4],&tmp,4);//push i + 5 onto the stack
			int L;
			memcpy(&L,membuf+i+1,4);//find the new destination
			if(debug){
			printf("Pushed %i onto the stack, calling value @ %i\n",i,L);}
			i=L;//set the new destination
		}
		else if (membuf[i]==0x90)//ret
		{
			int L;
			memcpy(&L, membuf+reg[4],4); //copy the old value of i
			reg[4]=reg[4]+4;//decrement the stack pointer
			if(debug){
				printf("Popped off the stack, setting i to %i",L);}
			i=L;//set i to L
		}
		else if (membuf[i]==0xa0)//push
		{
			int a = membuf[i+1];
			a = a/16;
			
			reg[4]=reg[4]-4;
			memcpy(membuf+reg[4],reg+a,4);
			if(debug){
			printf("Pushed %i onto the stack, ESP is now %i\n",reg[a],reg[4]);}
			i+=2;
		}
		else if (membuf[i]==0xb0)//popl
		{
			int a = membuf[i+1];
			a=a/16;
			
			int V;
			memcpy(&V,membuf+reg[4],4);
			
			if(debug){
			printf("ESP:%i\n",reg[4]);
			printf("Popping %i from the stack, ESP should now be %i\n",V,reg[4]+4);}
			
			reg[a]=V;
			reg[4]=reg[4]+4;
			
			i+=2;
		}
		else if (membuf[i]==0xc0)//readb
		{
			int a = membuf[i+1];
			a = a/16;
			
			int D;
			memcpy(&D,membuf+i+2,4);
			
			int L = reg[a]+D;	
			char buf[3];
			fgets(buf, 2, stdin);
			
			if (buf[0] == '\n') {
				flag[1] = 1;
			} else {
				flag[1] = 0;
				unsigned int byte = HexToDec(buf);
				memcpy(membuf+L,&byte,1);
				if(debug){
					printf("Saving value %i to memloc %i",byte,L);
				}
				
			}
			
			i+=6;
		}
		else if (membuf[i]==0xc1)//readl
		{
			int a = membuf[i+1];
			a = a/16;
			int D;
			memcpy(&D,membuf+i+2,4);
			flag[1]=0;
			int L = reg[a]+D;
			char buf[50];
			fgets(buf, 4, stdin);
			
			if (buf[0] == '\n') {
				flag[1] = 1;
			} else {
				flag[1] = 0;
				unsigned int word = atoi(buf);
				memcpy(membuf+L,&word,4);	
				if(debug){
					printf("Saving value %i to memloc %i",word,L);
				}
				
			}
			
			i+=6;
		}
		else if (membuf[i]==0xd0)//writeb
		{
			if(debug){
			printf("this is being reached\n");}
			int a = membuf[i+1];
			a = a/16;
			int D;
			memcpy(&D,membuf+i+2,4);
			
			printf("%c",membuf[reg[a]+D]);
			i+=6;
		}
		else if (membuf[i]==0xd1)//writel
		{
			int a = membuf[i+1];
			a = a/16;
			int D;
			memcpy(&D,membuf+i+2,4);
			int word;
			
			memcpy(&word,membuf+reg[a]+D,4);
			printf("%d",word);
			
			i+=6;
		}
		else if (membuf[i]==0xe0)//movsbl
		{
				int a = membuf[i+1];
				a = a/16;
				int b = membuf[i+1];
				b= b%16;
				
				int D;
				memcpy(&D,membuf+i+2,4);
				int L = reg[b]+D;

				char byte;
				memcpy(&byte,membuf+L,1);
				reg[a]=(int)byte;
				
				i+=6;
		}
		else
		{
			fprintf(stderr,"***Invalid Instrucion***");
			return -1;
		}
		//printf("\n");
		if(debug)
		printf("%x\n",membuf[i]);
	}
	 i=0;
	fprintf(stderr,"***Program Halted***");
	
	if(debug)
	{for(;i<blocksize;i++)
		{
		 if (membuf[i]==0)
		 printf("0");
		 
		 printf("%x",membuf[i]);
		}
	}
    fclose(file);
	free(membuf);
	
    return 0;
}

