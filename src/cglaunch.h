/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file	cglaunch.h
 * @brief	common header file for cglaunch
 */
#ifndef CGLAUNCH_H
#define CGLAUNCH_H

#include <stdint.h>
#include <limits.h>

#define CGLAUNCH_NAME_MAX_LENGTH	(128)
#define CGLAUNCH_CPUS_MAX_LENGTH	(128)

#define CGLAUNCH_ROLE_MASTER		(1)
#define CGLAUNCH_ROLE_SLAVE			(2)

struct s_cglaunch_config {
	int cgrole;
	int enable_memory_limit;
	int enable_cpu_limit;
	int64_t memory_limit_byte;
	uint64_t exec_delay_ns;
	char group_name[CGLAUNCH_NAME_MAX_LENGTH];
	char execute_cmdlile[PATH_MAX];
	char cpuset_cpus[CGLAUNCH_CPUS_MAX_LENGTH];
};
typedef struct s_cglaunch_config cglaunch_config_t;


int cglaunch_get_config(const char *config_file, cglaunch_config_t *cgconf);
int cglaunch_setup_cgroup(const cglaunch_config_t *cgconf);
#endif	//#ifndef CGLAUNCH_H