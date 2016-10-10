/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
	char* string;
	void* ptr;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) {
   
	
	TokenizerT *stream = malloc(sizeof(TokenizerT));
	//strcpy(stream->string,ts);
	stream->ptr =stream;
	//printf("original adress adress:%p\n",stream->ptr);
	//if (*ts=='\0')
	//{return NULL;}

	stream->string=strdup(ts);
	//printf("address after strdup:%p\n",stream->ptr);
  
  return stream;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) { 
	free(tk);
	tk=NULL;
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk ) {	
	char* nxttoken=strdup(tk->string);

	int i=0;
	int hasdeci=0;
	//char hold;
	while(nxttoken[i]!='\0') //loop to find next token
	{
		if (nxttoken[i]=='.')
		hasdeci=1;
		
		if (isspace(nxttoken[i])) //stop when you find a space
		{
			nxttoken[i]='\0';
		}
		else if (nxttoken[i]=='\\' && i!=0) //stop when you find a backslash
		{
			nxttoken[i]='\0';
		}
		else if (nxttoken[i]=='\\' && i==0) //return the \ and the next char
		{
			nxttoken[2]='\0';
			
			tk->string=tk->string+2;
			return nxttoken;
		}
		else if (hasdeci==1 && nxttoken[i]=='e')
		{
				i++;
		}
		
		else if(nxttoken[0]=='0' && (nxttoken[1]=='x'||nxttoken[1]=='X') && isxdigit(nxttoken[i]) && !isdigit(nxttoken[i])) //allowing hex digits only if in a hex token
		{
			i++;
		}
		else if(isxdigit(nxttoken[i]) && !isdigit(nxttoken[i]) && i==0) //allowing hex digits only if in a hex token
		{
			nxttoken[1]='\0';
			tk->string=tk->string+1;
			return nxttoken;
		}
		else if (isxdigit(nxttoken[i]) && !isdigit(nxttoken[i]) && i!=0) //allowing hex digits only if in a hex token
		{
			nxttoken[i]='\0';
		}
		else if((isascii(nxttoken[i]) && !isdigit(nxttoken[i]))&&(nxttoken[i]!='e') && 
		(nxttoken[i]!='E') && (nxttoken[i]!='x') && (nxttoken[i]!='X') && (i!=0) && 
		(nxttoken[i]!='.') && (nxttoken[i]!='+') && (nxttoken[i]!='-') && !isxdigit(nxttoken[i]))
		
		//stop when you find an ascii character that isnt x or e or i or . or + or - or a hex chr
		{
			//printf("Is alpha %c\n", nxttoken[i]);
			//hold=nxttoken[i];
			nxttoken[i]='\0';
		}
		else if((isascii(nxttoken[i]) && !isdigit(nxttoken[i]))&&(nxttoken[i]!='e' && 
		nxttoken[i]!='E') && (nxttoken[i]!='x') && (nxttoken[i]!='X') && (i==0) &&
		(nxttoken[i]!='.') && (nxttoken[i]!='+') && (nxttoken[i]!='-') && !isxdigit(nxttoken[i]))
		//return the individual next letter that isnt x or e or i or . or + or - or a hex chr
		{
			//printf("Is alpha %c\n", nxttoken[i]);
			nxttoken[1]='\0';
			tk->string=tk->string+1;
			return nxttoken;
		}
		else // continue through
		{
			i++;
		}
	}
	tk->string=tk->string+i; //update the string to start after the token just found 

return nxttoken;
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) 
{
	if (argv[1][0]=='\0')
	{
		printf("The initial array is empty, nothing to tokenize");
		return 0;
	}
	
	TokenizerT stream=*TKCreate(argv[1]);
	//printf("stream adress adress:%p\n",stream.ptr);
	char* tsshort=stream.string;	
	printf("%s\n",tsshort); 
	while(stream.string[0]!='\0') //begin loop through stream
	{
		if (isspace(stream.string[0]))
			{
				//printf("%s\n","Space Removed");
				stream.string=stream.string+1; //loop through spaces
				
			}
		else
			{
				//printf("%s\n","Token encountered, gathering...");
				char* token = TKGetNextToken(&stream);
				
				//getchar();
				
					//begin token check
					//As of now we know we have a segment of anything other than white space
					//Need to make sure it is a proper numerical token
					//follow FSM to do this
						
						int i=0;
						int state=-1; //default case
						int isOctal=0;
						int isHex=0;
						int isFloat=0;
						int isDec=0;
						//int isZero=0;
					while(token[i]!='\0')
						{
							switch(state) //begin finite state machine
							{
								case -3: //if is character
								{
									printf("MAL: [0x%x]\n",token[0]);
									
									i=strlen(token);//end loop
									isOctal=0;
									isHex=0;
									isFloat=0;
									isDec=0;
									break;
								}
								case -2: //if is escape char or MAL
								{
									if (token[0]=='\\')
									{
										switch(token[1]) //escape sequences
										{
											case 'a':
											{
												printf("MAL: [0x07]\n");
												break;
											}
											case 'b':
											{
												printf("MAL: [0x08]\n");
												break;
											}
											case 'f':
											{
												printf("MAL: [0x0C]\n");
												break;
											}
											case 'n':
											{
												printf("MAL: [0x0A]\n");
												break;
											}
											case 'r':
											{
												printf("MAL: [0x0D]\n");
												break;
											}
											case 't':
											{
												printf("MAL: [0x09]\n");
												break;
											}
											case 'v':
											{
												printf("MAL: [0x0B]\n");
												break;
											}
											case '\\':
											{
												printf("MAL: [0x5C]\n");
												break;
											}
											case '\'':
											{
												printf("MAL: [0x27]\n");
												break;
											}
											case '\"':
											{
												printf("MAL: [0x22]\n");
												break;
											}
											case '\?':
											{
												printf("MAL: [0x3F]\n");
												break;
											}
											default:
											{
												printf("MAL: [0x%x]",token[0]);
												printf("[0x%x]\n",token[1]);
											}
										}
									}
									else
									{
										printf("MAL: %s\n",token);	
									}
									i=strlen(token);
									isOctal=0;
									isHex=0;
									isFloat=0;
									isDec=0;
									break;
								}
								case -1: //will decide which path to take or if is MAL
								{
										if (isdigit(token[0]))
										{
											if(token[0]=='0') //I DONT KNOW WHY THIS DOESNT WORK
												{
													//printf("NullCheck %c\n",token[1]);
													if (token[1]=='\0'); //THIS DOESNT WORK EITHER (apparently 7=='0'...)
													{
														isDec=1;
													}
													i++;
													state=0;
												}
											else //1-9
												{
													//printf("NullCheck %c\n",token[1]);
													if (token[1]=='\0'); //Same broken stuff
													{
														isDec=1;
													}
													i++;
													state=1;
												}
										}
										else if(token[1]=='\0') //is malformed
										{	
											state=-3;
										}
										else
										{
											state=-2;
										}							
										break;
								}
								case 0: //if its octal
								{
										if (isdigit(token[i]) && ((token[i]!='9') && (token[i]!='8')))//keep looping through unless last pass
											{
												//keep looping through unless last pass
												if (token[i+1]=='\0')
													{
														isOctal=1;
														isHex=0;
														isFloat=0;
														isDec=0;
													}
												i++;
											}
										else if((token[1]=='X') || (token[1]=='x'))//send it to hex, Hex X must be 2nd letter
										{
												if (strlen(token)==2)
													{state=-2;}
												else
												{
												i++;
												//printf("Sending to HEX %s\n",token);
												state=2;
												}
										}
										else if(token[i]=='.')// send it to float
										{
												i++;
											//	printf("Sending to float %s\n",token);
												state=3;
										}
										else if((token[i]=='9' ||token[i]=='8'))// is mal
										{
												state=-2;
										}
										else//is malformed if its not another digit, a hex, or a float
										{
												state=-2;
										}
										break;
								}
								case 1: //if its decimal
								{
										if (isdigit(token[i]))//keep looping through unless last pass
											{
												if (token[i+1]=='\0')
													{
														//printf("Setting isDec to true %s\n",token);
														isDec=1;
													}
												i++;
											}
										else if(token[i]=='.')//send it to float
										{
											i++;
											state=3;
										}
										else//is MAL
										{
											state=-2;
										}
										break;
								}
								case 2: //if its hex
								{
									//printf("Inside Hex %s\n",token);
									if (isxdigit(token[i]))//keep looping through unless last pass
											{
												
												//keep looping through unless last pass
												if (token[i+1]=='\0')
													{
														
														isHex=1;
														isDec=0;
													}
												i++;
											}
									else//is malformed
									{
										state= -2;
									}
									break;
								}
								case 3: //if it has a .
								{
									//printf("Inside Float 1 %s\n",token);
									//printf("Float Check (1) %c\n",token[i]);
									if (isdigit(token[i])) //keep looping through unless last pass
									{
											//printf("Float Check (1) success %c\n",token[i]);
											//keep looping through unless last pass
											if (token[i+1]=='\0')
												{	
												
												isFloat=1;
												isDec=0;
												}
										i++;
									}
									else if (((token[i]=='E') ||(token[i]=='e')) && isdigit((token[i-1])))//there can be an E, but only if digits are in between it and the .
									{
										if (token[i+1]=='\0')
										{
											state=-2;
											break;
										}
										i++;
										state=4;
									}
									else//is MAL
									{
										state =-2;
									}
									break;
								}
								case 4: //if it has a . followed by digits, then an E
								{
									//printf("Inside Float 2 %s\n",token);
									//printf("Float Check (2) %c\n",token[i]);
									if (token[i]=='\0')
									{
											state=-2; 
											break;
									}
									if (isdigit(token[i]))//keep looping through unless last pass
											{
												//printf("Float Check Success %c\n",token[i]);
												if (token[i+1]=='\0')
													{
														isFloat=1;
														isDec=0;
													}
												i++;
											}
									else if ((token[i]=='+'||token[i]=='-')&&(token[i-1]=='E' || token[i-1]=='e'))// can have an +- but only if immediately following an Ee an a number must follow
									{
										if (token[i+1]=='\0')//is mal
										{
											state=-2;
											break;
										}
										i++;
										state=5;
									}
									else //is MAL
									{
										state=-2;
									}
									break;
								}
								case 5: //if it has a . followed by digits, then an Ee, then a +-
								{
									//printf("Inside Float 3 %s\n",token);
									if (isdigit(token[i]))//keep looping through unless last pass
											{
												//keep looping through unless last pass
												if (token[i+1]=='\0')
													{
														isFloat=1;
														isDec=0;
													}
												i++;
											}
									else//is mal
									{
										state=-2;
									}
									break;
								}
							}			
						}
					if(isOctal)
						printf("Octal %s\n",token);
					if(isDec)
						printf("Decimal %s\n",token);
					if(isHex)
						printf("Hexadecimal %s\n",token);
					if(isFloat)	
						printf("Float %s\n",token);
					
					
					free(token);// freememory used in getnexttoken
			}//else
	}//while
	//printf("free adress:%p",&stream);
	TKDestroy(stream.ptr);

	return 0;
}

