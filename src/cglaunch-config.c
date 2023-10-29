/**
 * SPDX-License-Identifier: Apache-2.0
 *
 * @file	cglaunch-configu.c
 * @brief	Source file for config parser.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "toml.h"
#include "cglaunch.h"


/**
 * @brief Internal function for string assessment.
 *
 * @param [in]		cpus_string		String for cpus setting.
 * @return Description for return value
 * @retval 0	Valid string.
 * @retval -1	Invalid string.
 *
 */
static int cglaunch_test_cpus_string(const char *cpus_string, const size_t length)
{
	int result = 0;

	for(size_t count = 0; count < length; count++) {
		char c = cpus_string[count];

		if (!(('0' <= c) && (c <= '9'))) {
			// Not a valid number
			if (!((c == ',') || (c =='-'))){
				// Not a valid character
				result = -1;
				break;
			}
		}
	}

	return result;
}

/**
 * @brief Internal function for get execution setting.
 *
 * @param [in]		toml_conf		The toml table.
 * @param [inout]	cgconf			Must set pre allocated and zero filled cglaunch_config_t. Will set config date from file.
 * @return Description for return value
 * @retval 0	Success to read config file.
 * @retval -1	Argument error.
 * @retval -2	Can't get config element.
 *
 */
static int cglaunch_get_config_exec(const toml_table_t *toml_conf, cglaunch_config_t *cgconf)
{
	int result = 0;
	size_t length = 0;
	toml_table_t *exec = NULL;
	toml_datum_t cmdline;

	(void) memset(&cmdline, 0, sizeof(cmdline));

	exec = toml_table_in(toml_conf, "exec");
	if (exec == NULL) {
		#ifdef _PRINTF_DEBUG_
		fprintf(stderr, "Can't get exec section from toml file.\n");
		#endif
		result = -2;
		goto do_return;
	}

	cmdline = toml_string_in(exec, "command-line");
	if (cmdline.ok == 0) {
		#ifdef _PRINTF_DEBUG_
		fprintf(stderr, "Can't get command-line from toml file.\n");
		#endif
		result = -2;
		goto do_return;
	}

	length = strlen(cmdline.u.s);
	if (length >= sizeof(cgconf->execute_cmdlile)){
		// A command line is too long.
		result = -2;
		goto do_return;
	}

	(void) strncpy(cgconf->execute_cmdlile, cmdline.u.s, (sizeof(cgconf->execute_cmdlile) - 1UL));

do_return:
	(void) free(cmdline.u.s);

	return result;
}
/**
 * @brief Internal function for get cgroup setting.
 *
 * @param [in]		toml_conf		The toml table.
 * @param [inout]	cgconf			Must set pre allocated and zero filled cglaunch_config_t. Will set config date from file.
 * @return Description for return value
 * @retval 0	Success to read config file.
 * @retval -1	Argument error.
 * @retval -2	Can't get config element.
 *
 */
static int cglaunch_get_config_cgroup(const toml_table_t *toml_conf, cglaunch_config_t *cgconf)
{
	int result = 0;
	toml_table_t *cgroup = NULL;

	cgroup = toml_table_in(toml_conf, "cgroup");
	if (cgroup == NULL) {
		#ifdef _PRINTF_DEBUG_
		fprintf(stderr, "Can't get cgroup section from toml file.\n");
		#endif
		// Disable cgroup limitation.
		cgconf->enable_memory_limit = 0;
		cgconf->enable_cpu_limit = 0;
		result = 0;
		goto do_return;
	}

	// Get a group name.
	{
		toml_datum_t group_name;
		(void) memset(&group_name, 0, sizeof(group_name));

		group_name = toml_string_in(cgroup, "group-name");
		if (group_name.ok == 0) {
			// No group name, must set name.
			result = -2;
			goto do_return;
		} else {
			size_t length = 0;

			length = strlen(group_name.u.s);
			if (length >= sizeof(cgconf->group_name)){
				// A cpu-core-limit is too long.
				result = -2;
				(void) free(group_name.u.s);
				goto do_return;
			}

			(void) strncpy(cgconf->group_name, group_name.u.s, (sizeof(cgconf->group_name) - 1UL));
			(void) free(group_name.u.s);
		}
	}

	// Get a memory limit setting.
	{
		toml_datum_t mem_limit;
		(void) memset(&mem_limit, 0, sizeof(mem_limit));

		mem_limit = toml_int_in(cgroup, "memory-limit-mb");
		if (mem_limit.ok == 0) {
			#ifdef _PRINTF_DEBUG_
			fprintf(stderr, "Can't get memory-limit-mb from toml file. Disable.\n");
			#endif
			cgconf->enable_memory_limit = 0;
		} else {
			if (mem_limit.u.i <= 0) {
				// Out of range, set -1 that mean is disable setting.
				result = -2;
				goto do_return;
			} else {
				if (mem_limit.u.i >= (INT64_MAX/1024/1024)) {
					// Size over, set -1 that mean is disable setting.
					result = -2;
					goto do_return;
				} else {
					cgconf->memory_limit_byte = mem_limit.u.i * 1024LL * 1024LL;
					cgconf->enable_memory_limit = 1;
				}
			}
		}
	}

	// Get a cpus limit setting.
	{
		toml_datum_t cpu_list;
		(void) memset(&cpu_list, 0, sizeof(cpu_list));

		cpu_list = toml_string_in(cgroup, "cpu-core-limit");
		if (cpu_list.ok == 0) {
			#ifdef _PRINTF_DEBUG_
			fprintf(stderr, "Can't get cpu-core-limit from toml file. Disable.\n");
			#endif
			cgconf->enable_cpu_limit = 0;
		} else {
			size_t length = 0;
			int ret = -1;

			length = strlen(cpu_list.u.s);
			if (length >= sizeof(cgconf->cpuset_cpus)){
				// A cpu-core-limit is too long.
				result = -2;
				(void) free(cpu_list.u.s);
				goto do_return;
			}

			ret = cglaunch_test_cpus_string(cpu_list.u.s, length);
			if (ret == 0) {
				(void) strncpy(cgconf->cpuset_cpus, cpu_list.u.s, (sizeof(cgconf->cpuset_cpus) - 1UL));
				cgconf->enable_cpu_limit = 1;
			} else {
				// Invalid string at cpu-core-limit.
				result = -2;
				(void) free(cpu_list.u.s);
				goto do_return;
			}
			(void) free(cpu_list.u.s);
		}
	}

do_return:

	return result;
}

/**
 * @brief Function for get config from file.
 *
 * @param [in]		config_file		Full path for cglaunch config file.
 * @param [inout]	cgconf			Must set pre allocated and zero filled cglaunch_config_t. Will set config date from file.
 * @return Description for return value
 * @retval 0	Success to read config file.
 * @retval -1	Argument error.
 * @retval -2	File read error. No file/Format error.
 *
 */
int cglaunch_get_config(const char *config_file, cglaunch_config_t *cgconf)
{
	int ret = -1;
	int result = 0;
	char error_buf[128];
	FILE* toml_fp = NULL;
	toml_table_t *toml_conf = NULL;

	if ((config_file == NULL) || (cgconf == NULL)) {
		result = -1;
		goto do_return;
	}

	toml_fp = fopen(config_file, "r");
	if (toml_fp == NULL) {
		result = -2;
		goto do_return;
	}

	(void) memset(error_buf, 0, sizeof(error_buf));

	// Parse for toml file
	toml_conf = toml_parse_file(toml_fp, error_buf, sizeof(error_buf));
	if (toml_conf == NULL) {
		#ifdef _PRINTF_DEBUG_
		fprintf(stderr, "%s\n",error_buf);
		#endif
		result = -2;
		goto do_return;
	}

	ret = cglaunch_get_config_exec(toml_conf, cgconf);
	if (ret < 0) {
		result = -2;
		goto do_return;
	}

	ret = cglaunch_get_config_cgroup(toml_conf, cgconf);
	if (ret < 0) {
		result = -2;
		goto do_return;
	}

do_return:
	if (toml_fp != NULL) {
		(void) fclose(toml_fp);
	}

	return result;
}