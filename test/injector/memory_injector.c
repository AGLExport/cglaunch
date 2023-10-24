/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file	memory_injector.c
 * @brief	main source file for memory over injector
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int max_mbytes = 1000;
const size_t injection_size = 1*1024*1024;

void memory_over_injection()
{
	void * p = NULL;

	for(int i=0; i < max_mbytes;i++) {
		p = malloc(injection_size);
		memset(p,0xff,injection_size);
		fprintf(stderr,"do injection %d\n",i);
		sleep(1);
	}
}

/**
 * The main function for cglauncher.
 */
int main(int argc, char *argv[])
{
	memory_over_injection();

	return 0;
}
