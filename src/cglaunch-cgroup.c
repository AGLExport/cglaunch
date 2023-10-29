/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file	cglaunch-cgroup.c
 * @brief	Source file for cgroup operation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <libcgroup.h>

#include "cglaunch.h"

/**
 * @brief Function for cgroup subgroup create and setting myself.
 *
 * @param [in]	cgconf	Must set configured cglaunch_config_t.
 * @return Description for return value
 * @retval 0	Success to read config file.
 * @retval -1	Argument error.
 * @retval -2	Error from libcgroup.
 *
 */
int cglaunch_setup_cgroup(const cglaunch_config_t *cgconf)
{
	int ret = -1;
	int result = 0;
	struct cgroup *newgroup = NULL;

	if (cgconf == NULL) {
		result = -1;
		goto do_return;
	}

	ret = cgroup_init();
	if (ret != 0) {
		// Initialize error for libcgroup.
		result = -2;
		goto do_return;
	}

	newgroup = cgroup_new_cgroup(cgconf->group_name);
	if (newgroup == NULL) {
		#ifdef _PRINTF_DEBUG_
		fprintf(stderr, "Fail to create new group. name = %s\n", cgconf->group_name);
		#endif
		// Fail to create new group.
		result = -2;
		goto do_return;
	}

	if (cgconf->enable_memory_limit == 1) {
		struct cgroup_controller *memory_controller = NULL;

		memory_controller = cgroup_add_controller(newgroup, "memory");
		if (memory_controller == NULL) {
			// Fail to create new controller group.
			result = -2;
			goto do_return;
		}

		ret = cgroup_add_value_int64(memory_controller, "memory.limit_in_bytes", cgconf->memory_limit_byte);
		if (ret != 0) {
			// Fail to set value to memory.limit_in_bytes.
			result = -2;
			goto do_return;
		}

		ret = cgroup_add_value_int64 (memory_controller, "tasks", (int64_t)getpid());
		if (ret != 0) {
			// Fail to set pid to controller.
			result = -2;
			goto do_return;
		}
	}

	if (cgconf->enable_cpu_limit == 1) {
		struct cgroup_controller *cpuset_controller = NULL;

		cpuset_controller = cgroup_add_controller(newgroup, "cpuset");
		if (cpuset_controller == NULL) {
			// Fail to create new controller group.
			result = -2;
			goto do_return;
		}

		ret = cgroup_add_value_string(cpuset_controller, "cpuset.cpus", cgconf->cpuset_cpus);
		if (ret != 0) {
			// Fail to set value to cpuset.cpus.
			result = -2;
			goto do_return;
		}

		ret = cgroup_add_value_int64(cpuset_controller, "tasks", (int64_t)getpid());
		if (ret != 0) {
			// Fail to set pid to controller.
			result = -2;
			goto do_return;
		}
	}

	ret = cgroup_create_cgroup(newgroup, 0);
	if (ret != 0) {
		//#ifdef _PRINTF_DEBUG_
		fprintf(stderr, "cgroup error : %s\n", cgroup_strerror(cgroup_get_last_errno()));
		//#endif
		result = -2;
		goto do_return;
	}

do_return:
	if (newgroup != NULL) {
		(void) cgroup_free(&newgroup);
	}

	return result;
}