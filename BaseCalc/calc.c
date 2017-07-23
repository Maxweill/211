#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int BinToDec(char* input) //Cheers to Professor Russell for this one
{

	int i=0;
	int value=0;
	for (i=0;i<strlen(input);i++)
{
	switch(input[i])
	{
		case '0': value=(value<<1);break;
		case '1': value=(value<<1)+1;break;
		default: fprintf(stderr, "ERROR: Incorrect Binary Number.\n"); return -1;
	}
}
//printf("%d\n",value);
return value;
}

char* DecToDecString(int input)
{
	char* output = malloc(32+1);
	int i=0;
	while(input>0)
	{
		output[i]=(input%10)+'0';
		input/=10;
		i++;
	}
	output[i]='\0';
	
   int a = 0;
   int b = strlen(output) - 1;
	char temp;
   while (a < b) {
      temp = output[a];
      output[a] = output[b];
      output[b] = temp;
      a++;
      b--;
   }
	
	return output;
}

int DecstringToDec(char* input)
{
	int value=0;
	int pos=0;
	
	for(pos=0;pos<strlen(input);pos++)
	{
		if (!isdigit(input[pos]))
		{
				fprintf(stderr, "ERROR: Incorrect Decimal Number.\n");
				return -1;
		}
	 value=value*10+ (input[pos]-'0');
	}
//printf("%d\n",value);
return value;
}

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
	
//printf("%d\n",value);
return value;
}

int OctToDec(char* input)
{
	int value=0;
	int base=1;
	int pos=strlen(input)-1;
	for(;pos>=0;pos--)
	{
		if (!isdigit(input[pos]) || input[pos]-'0'>8)
		{
				fprintf(stderr, "ERROR: Incorrect Octal Number.\n");
				return -1;
		}
		value += (input[pos]-'0')*base;
		base*=8;
	}
	
	
	
//printf("%d\n",value);
return value;
}

char* DecToBin(int input)
{
	int i;
	int j=0; //tracks the opposite value of i so that we can flip the proper bit
	char* output = malloc(32+1);//max size of 32
	 j=0;
	 for (i=31;i>=0;i--)
	 {
		if(input &(1<<i))
		{
			*(output+j)=1+'0';
		}
		else
		{
			*(output+j)=0+'0';
		}
		j=31-i;
	 }
		*(output+j)='\0';
	return output;//might delete leading 0s, not sur yet

}
char* DecToHex(int input)
{
	//printf("Entering dec to hex");
	char* output = malloc(32+1);
	char* cstr = malloc(32+1);
	int i;
	int n;
	unsigned int input2 = input;
	char * hexchar="0123456789ABCDEF";
	output[0]='\0';


	for(i=0;i<8;i++)
	{
		n = input2>>28;
		cstr[0]=hexchar[n];
		cstr[1]='\0';
		strcat(output,cstr);
		input2=input2<<4;
	}
	free(cstr);
return output;
}
char* DecToOct(int input)
{
	char* output = malloc(32+1);
	unsigned int shiftnum = 30; //30 because 32 isnt evenly divisible by 3 :(
	int i=0;
    for(; i < 11; ++i)
    {
        output[i]=(input >> shiftnum & 7)+'0';//since we fill out output left to right, we can just decrement shift
        shiftnum -= 3;
    }
    output[i]='\0';
	return output;
}
int main(int argc, char **argv) 
{
	if (argv[1] == '\0' ||argv[2] == '\0' ||argv[3] == '\0' || argv[4] == '\0')
	{
		fprintf(stderr,"ERROR:Invalid input, one or more arguments are null.\n");
		return -1;
	} 
	char operator; 
	if ((argv[1][0]=='+' ||argv[1][0]=='-'||argv[1][0]=='*') && argv[1][1]=='\0') //makes sure operator is +- or * otherwise err
	{
		//printf("valid: %s\n",argv[1]);
		operator = argv[1][0];
	}
	else
	{
		fprintf(stderr,"ERROR: Invalid operation input. Must be +,- or *.\n");
		return -1;
	}
	
	char* num1digits;
	char* num2digits; //everything after the original input 
	
	int num1dec;
	int num2dec;
	
	int num1neg =1;// end result will be multiplied by this, so if the num is positive, gets val of 1, otherwise is -1
	int num2neg =1;
	
	num1neg =argv[2][0]=='-' ? -1:1;
	num2neg =argv[3][0]=='-' ? -1:1;
	
	char num1base= num1neg==-1 ? argv[2][1]: argv[2][0];
	char num2base= num2neg==-1 ? argv[3][1]: argv[3][0];
	
	if ((num1base=='o' || num1base=='d' || num1base=='b' || num1base=='x' ) &&  //properly separating digits from the other arguments
	    (num2base=='o' || num2base=='d' || num2base=='b' || num2base=='x' ))
	{
			num1digits = num1neg==-1 ? argv[2]+2: argv[2]+1;
			num2digits = num2neg==-1 ? argv[3]+2: argv[3]+1;
	}
	else
	{
		fprintf(stderr,"ERROR: One or more invalid base inputs. Must be d (decimal), o (octal), b (binary), or x (hexadecimal).\n");
		return -1;
	}
	
	//printf("%s\n",num1digits);
	//printf("%s\n",num2digits);
	switch(num1base)
	{
		case 'b':
		{
			num1dec = BinToDec(num1digits);
			break;
		}
	
		case 'd':
		{
			num1dec = DecstringToDec(num1digits);
			break;
		}
		
		case 'o':
		{
			num1dec = OctToDec(num1digits);
			break;
		}
		
		case 'x':
		{
			num1dec = HexToDec(num1digits);
			break;
		}
	}
	if (num1dec==-1)
	{
		fprintf(stderr,"ERROR: Improper Number or Value Overflow.");
		return -1;
	}
	
	
	
	switch(num2base)
	{
		case 'b':
		{
			num2dec = BinToDec(num2digits);
			break;
		}
	
		case 'd':
		{
			num2dec = DecstringToDec(num2digits);
			break;
		}
		
		case 'o':
		{
			num2dec = OctToDec(num2digits);
			break;
		}
		
		case 'x':
		{
			num2dec = HexToDec(num2digits);
			break;
		}
	}
	if (num2dec==-1)
	{
		return -1;
		fprintf(stderr,"ERROR: Improper Number or Value Overflow.");
		
	}
	
	int finaldec=0;
	switch(operator)
	{
		case '+':
		{
		 finaldec=(num1dec*num1neg) + (num2dec*num2neg);
		 break;
		}
		case '-':
		{
		 finaldec=(num1dec*num1neg) - (num2dec*num2neg);
		 break;
		}
		case '*':
		{
		 finaldec=(num1dec*num1neg) * (num2dec*num2neg);
		 break;
		}
	}
	//printf("Decimal Form:%d\n",finaldec);
	char* finalprint;
	
	if ((argv[4][0] == 'o' || argv[4][0] == 'b' || argv[4][0] == 'x' || argv[4][0] == 'd') && argv[4][1]=='\0')
	{
		switch(argv[4][0])
		{
			printf("entering case o");
			case 'o':
			{
				
				if(finaldec>=0)
				{
				finalprint=DecToOct(finaldec);
				printf("o%s\n",finalprint);
				}
				else
				{
				finalprint=DecToOct(-1*finaldec);
				printf("-o%s\n",finalprint);
				}
				free(finalprint);
				break;
			}
			case 'b':
			{
				if(finaldec>=0)
				{
				finalprint=DecToBin(finaldec);
				printf("b%s\n",finalprint);
				}
				else
				{
				finalprint=DecToBin(-1*finaldec);
				printf("-b%s\n",finalprint);
				}
				free(finalprint);
				break;
				
			}
			case 'x':
			{
				//printf("Entering case x");
				if(finaldec>=0)
				{
				finalprint=DecToHex(finaldec);
				printf("x%s\n",finalprint);
				}
				else
				{
				finalprint=DecToHex(-1*finaldec);
				printf("-x%s\n",finalprint);
				}
				free(finalprint);
				break;
			}
			case 'd':
			{
				if(finaldec>=0)
				{
				finalprint=DecToDecString(finaldec);
				printf("d%s\n",finalprint);
				}
				else
				{
				finalprint=DecToDecString(-1*finaldec);
				printf("-d%s\n",finalprint);
				}
				free(finalprint);
				break;
			}
		}
	}
	else
	{
	fprintf(stderr,"ERROR: incorrect entry for base. Must be b (binary), d (decimal), x (hexadecimal), or o (octal).\n");	
	}
	return 0;
}
