#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
char* DecToDecString(int input, char* output)
{
	int i=0;
	//printf("%i the value of input begin of input", input);
	int negcheck=input;
	if (input<0)
	{
			input*=-1;
	}
	while(input>0)
	{
		output[i]=(input%10)+'0';
		input/=10;
		i++;
	}
	if (negcheck<0)
	{
		output[i]='-';
		i++;
	}
	output[i]='\0';
	//printf("%s value of output midway thru dectodecstr \n",output);
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
int BinAsciiToDec(char* input)
{
		char *str=input;
		int value=0;
		int i;
		for (i=0;i<32;i++)
		{
			switch(str[i])
			{
			case '0': value=(value<<1);break;
			case '1': value=(value<<1)+1;break;
			}
		}	
		//printf("%d value in binascii to dec \n",value);
		return value;
}
static void
get_float_digit( float x, char * digit, int * pow10, float * remainder )
{
	int			p10;

	if ( x == 0.0 )
	{
		*digit = '0';			// digit is always '0'
		*pow10 = 0;
		*remainder = 0.0;
	}
	else
	{
		*pow10 = 0;			// start with 10^0
		while ( x >= 10.0 )		// reduce
		{
			x /= 10.0;		// large values get smaller
			*pow10 += 1;
		}
		while ( x < 1.0 )		// increase
		{
			x *= 10.0;		// small values get larger
			*pow10 -= 1;
		}
		*digit = '0';
		do {				// 10.0 > x >= 1.0
			x -= 1.0;
			*digit += 1;		// digit is always non-'0'zero
		} while ( x >= 1.0 );
		p10 = 0;
		while ( p10 > *pow10 )		// leading digit is now removed from x
		{
			x /= 10;
			p10 -= 1;
		}
		while ( p10 < *pow10 )
		{
			x *= 10;
			p10 += 1;
		}
		*remainder = x;
	}
}

// Improve efficiency by adding more while loops to handle larger powers of 10, e.g. :
// while ( x >= 1e1024 ) { x /= 1e0124; pow10 += 1024; }
// while ( x >= 1e512 ) { x /= 1e512; pow10 += 512; }
// ...
// while ( x >= 10.0 ) { x /= 10.0 ; pow10 += 1; }
// And
// while ( x < 1.0 ) { x *= 10; pow10 -= 1; }
// ...
// while ( x < 1e-512 ) { x *= 1e512; pow10 -= 512; }
// while ( x < 1e-1024 ) { x *= 1e1024; pow10 -= 1024; }

static void
append( char * s, char c )
{
	char		buf[2];

	buf[0] = c;
	buf[1] = '\0';
	strcat( s, buf );
}

union Number {
	int	i;
	float	f;
};

void
floatToASCII( float x, char * output )
{
	char		c;
	int		pow10, p10, plast;
	int		i;
	float		remainder;
	char		exponent[10];
	union Number	a;
	unsigned int	biasedExp;
	unsigned int	mantissa;
	int		sign;

	a.f = x;
	biasedExp = a.i >> 23 & 0x000000ff;
	mantissa = a.i & 0x007fffff;
	sign = a.i >> 31;
	//printf( "BKR x is %g.  biasedExp is %x  mantissa is %08x  sign is %d\n", x,
		//biasedExp, mantissa, sign );
	if ( biasedExp == 0xff )
	{
		if ( mantissa == 0 )
		{
			if ( sign != 0 )
			{
				strcpy( output, "-inf" );
		printf( "BKR returning from file %s line %d\n", __FILE__, __LINE__ );
				return;
			}
			else
			{
				strcpy( output, "+inf" );
		printf( "BKR returning from file %s line %d\n", __FILE__, __LINE__ );
				return;
			}
		}
		else
		{
			if ( sign != 0 )
			{
				strcpy( output, "-NaN" );
		printf( "BKR returning from file %s line %d\n", __FILE__, __LINE__ );
				return;
			}
			else
			{
				strcpy( output, "+NaN" );
		printf( "BKR returning from file %s line %d\n", __FILE__, __LINE__ );
				return;
			}
		}
	}
	output[0] ='\0';
	if ( x < 0.0 )
	{
		append( output, '-' );
		x = -x;					// make x positive
	}
	get_float_digit( x, &c, &pow10, &remainder );
	append( output, c );
	append( output, '.' );
	x = remainder;
	plast = p10 = pow10;			// pow10 set by get_float_digit()
	for ( i = 1 ; i < 7 ; i++ )		// 7 significant digits in 32-bit float
	{
		get_float_digit( x, &c, &p10, &remainder );
		if ( (plast - p10) > 1 )
		{
			append( output, '0' );	// fill in zero to next nonzero digit
			plast -= 1;
		}
		else
		{
			append( output, c );
			x = remainder;
			plast = p10;
		}
	}
	if ( pow10 < 0 )		// negative exponent
	{
		exponent[0] = 'e';
		DecToDecString( pow10, exponent+1 );
	}
	else if ( pow10 < 10 )		// positive single-digit exponent
	{
		exponent[0] = 'e';
		exponent[1] = '+';
		exponent[2] = '0';
		DecToDecString( pow10, exponent+3 );
	}
	else				// positive multi-digit exponent
	{
		exponent[0] = 'e';
		exponent[1] = '+';
		DecToDecString( pow10, exponent+2 );
	}
	strcat( output, exponent );
}
int main(int argc, char **argv) 
{
	if(argv[1]== '\0' || argv[2]=='\0')
	{
		fprintf(stderr, "ERROR: One or more require arguments are null!");
		return -1;
	}
		int len = strlen(argv[1]);
	if (len!=32)
	{
		fprintf(stderr, "ERROR: Bit sequence must be 32 bits long.");
		return -1;
	}
	
	if(strcasecmp(argv[2],"FLOAT")==0) //if its a float
	{
		int value = BinAsciiToDec(argv[1]);
		float floatval;
		memcpy(&floatval,&value,sizeof(value));
		char* output = malloc(32+1);
		floatToASCII(floatval,output);
		printf("%s\n",output);
		free(output);
		
	}
	else if(strcasecmp(argv[2],"INT")==0) //if its an int
	{	
		int value = BinAsciiToDec(argv[1]);
		char*  output = malloc(32+1);
		printf("%s\n",DecToDecString(value,output));
		free(output);	
	}
	else
	{
		fprintf(stderr, "ERROR: Incorrect argument. Must be 'float' or 'int'.");
		return -1;
	}
	
	
return 0;
}
