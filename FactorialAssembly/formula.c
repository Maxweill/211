/*
 * formula.c
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
 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include "nCr.h"
#include "formula.h"
int main(int argc, char **argv)
{
	struct timeval start, end; // declare timeval type variables start and end 

	gettimeofday(&start, NULL); //get start time
	
	if (argc<=1)
	{
		fprintf(stderr,"ERROR: Null entry\n");
		return -1;
	}
	
	if (argv[1][0]=='-' && argv[1][1]=='h' && argv[1][2]=='\0')
	{
		printf("USAGE: formula <positive integer>\n");
		return -1;
	}
	int exp = atoi(argv[1]);
	//printf("factotrial is %lu", Factorial(exp));
	
	
	if (exp<=0)
		{
			fprintf(stderr,"ERROR: Please enter a positive integer.\n");
			return -1;
		}
		
	int powerup =1;
	if (exp==1)
		{
			printf("1+x");
		}
	if (exp>1)
	{
		
		printf("1+");
		while (powerup<=exp)
		{
			printf("%dx^%d",nCr(exp,powerup),powerup);
			powerup++;
			if (powerup<=exp)
			{
					printf(" + ");
			}
		}
		
	}
	gettimeofday(&end, NULL); //get end time
	 printf("\nCalculation took %ld microseconds. \n", ((end.tv_sec * 1000000 + end.tv_usec)
		- (start.tv_sec * 1000000 + start.tv_usec)));

	return 0;
}

