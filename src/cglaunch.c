/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file	cglaunch.c
 * @brief	main source file for launcher with self cgroup setting
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include "cglaunch.h"


static const char g_test_config_file[] = "./test/config/memory-test.toml";

/**
 * The main function for cglauncher.
 */
int main(int argc, char *argv[])
{
	int ret = -1;
	int result = 1;
	const char *config_file = NULL;
	cglaunch_config_t cgconf;

	if (argc != 2) {
		#ifdef _CRITICAL_ERROR_OUT_STDERROR
		fprintf(stderr, "Must be set config file at 1st arg.\n");
		#endif
		goto do_return;
	}

	config_file = strstr(argv[1],".toml");
	if (config_file == NULL) {
		#ifdef _CRITICAL_ERROR_OUT_STDERROR
		fprintf(stderr, "Must be set toml config file at 1st arg.\n");
		#endif
		goto do_return;
	}
	config_file = argv[1];

	//config_file = g_test_config_file;
	(void) memset(&cgconf, 0, sizeof(cgconf));

	ret = cglaunch_get_config(config_file, &cgconf);
	if (ret < 0) {
		#ifdef _CRITICAL_ERROR_OUT_STDERROR
		fprintf(stderr, "Fail to read config file at %s\n", config_file);
		#endif
		goto do_return;
	}

	ret = cglaunch_setup_cgroup(&cgconf);
	if (ret < 0) {
		#ifdef _CRITICAL_ERROR_OUT_STDERROR
		fprintf(stderr, "Fail to setup cgroup\n");
		#endif
		goto do_return;
	}

	(void) execlp("./test/injector/memory_injector", "./test/injector/memory_injector", (char*)NULL);

do_return:
	return result;
}
