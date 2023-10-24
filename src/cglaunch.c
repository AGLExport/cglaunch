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

#include <libcgroup.h>

void memory_over_injection()
{
	void * p = NULL;
	const size_t injection_size = 1*1024*1024;

	for(int i=0; i < 100;i++) {
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
	int ret = -1;
	int result = 0;
	struct cgroup *newgroup = NULL;
	struct cgroup_controller *memory_controller = NULL;

	ret = cgroup_init();
	//#ifdef _PRINTF_DEBUG_
	fprintf(stderr, "cgroup_init return = %d\n",ret);
	//#endif

	newgroup = cgroup_new_cgroup("test-cglaunch");
	//#ifdef _PRINTF_DEBUG_
	fprintf(stderr, "cgroup_new_cgroup return = %p\n",newgroup);
	//#endif

	memory_controller = cgroup_add_controller(newgroup, "memory");
	//#ifdef _PRINTF_DEBUG_
	fprintf(stderr, "cgroup_add_controller return = %p\n",memory_controller);
	//#endif
	ret = cgroup_add_value_int64 (memory_controller, "memory.limit_in_bytes", (10LL * 1024LL * 1024LL));
	//#ifdef _PRINTF_DEBUG_
	fprintf(stderr, "cgroup_add_value_int64 return = %d\n",ret);
	//#endif

	ret = cgroup_add_value_int64 (memory_controller, "tasks", getpid());
	//#ifdef _PRINTF_DEBUG_
	fprintf(stderr, "cgroup_add_value_int64 return = %d\n",ret);
	//#endif

	ret = cgroup_create_cgroup(newgroup, 0);
	//#ifdef _PRINTF_DEBUG_
	fprintf(stderr, "cgroup_create_cgroup return = %d\n",ret);
	if (ret != 0) {
		fprintf(stderr, "cgroup error : %s\n", cgroup_strerror(cgroup_get_last_errno()));
	}
	//#endif

	cgroup_free(&newgroup);

	memory_over_injection();

	return result;
}
