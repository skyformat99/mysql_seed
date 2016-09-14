#include "mysql_seed_execute.h"

/* global constants
 *─────────────────────────────────────────────────────────────────────────── */
ExecuteDispatchNode *const
EXECUTE_DISPATCH_MAP[EXECUTE_DISPATCH_MAP_LENGTH] = {
	[0u] = &execute_failure_no_exec_spec,
	[1u] = &execute_failure_short_exec_spec,
	[2u] = &execute_dispatch2,
	[3u] = &execute_dispatch3,
	[4u] = &execute_dispatch4,
	[5u] = &execute_dispatch5,
	[6u] = &execute_dispatch6
};


/* print errors
 *─────────────────────────────────────────────────────────────────────────── */
extern inline void
execute_expected_db_flag(const char *const restrict invalid);
extern inline void
execute_invalid_db_name_empty(void);
extern inline void
execute_invalid_db_name_invalid(const char *const restrict db_name);
extern inline void
execute_invalid_db_name_long(const char *const restrict db_name);


/* parsing DB_NAME
 *─────────────────────────────────────────────────────────────────────────── */
extern inline bool
execute_parse_db_name(struct String *const restrict db_name,
		      char *const restrict arg);

extern inline bool
execute_db_flag_match(char *const restrict arg);


/* read MySQL password
 *─────────────────────────────────────────────────────────────────────────── */
extern inline bool
read_mysql_password(char *const restrict buffer,
		    const size_t size_max,
		    const char *restrict *const restrict failure);


/* if EXEC_SPEC is correct, at least 2 databases need to be loaded
 *─────────────────────────────────────────────────────────────────────────── */
extern inline int
execute_dispatch_large(char *const restrict *restrict from,
		       char *const restrict *const restrict until);


/* ExecuteDispatchNodes
 *─────────────────────────────────────────────────────────────────────────── */
/* at least 1 database */
int
execute_dispatch6(char *const restrict *restrict from)
{
	return EXIT_FAILURE;
}

int
execute_dispatch5(char *const restrict *restrict from)
{
	return EXIT_FAILURE;
}

int
execute_dispatch4(char *const restrict *restrict from)
{
	return EXIT_FAILURE;
}

/* at most 2 database */
int
execute_dispatch3(char *const restrict *restrict from)
{
	struct ExecSpec exec_spec;
	struct String db_names[2];


	const char *restrict arg = *from;



	return EXIT_FAILURE;

}

/* at most 1 database */
int
execute_dispatch2(char *const restrict *restrict from)
{
	struct String db_name;

	if (   execute_db_flag_match(*from)
	    && execute_parse_db_name(&db_name,
				     from[1])) {
		int exit_status = EXIT_SUCCESS;

		mysql_seed_execute(&db_name,
				   MYSQL_DEFAULT_USER,
				   MYSQL_DEFAULT_PASSWORD,
				   &exit_status);

		return exit_status;
	}

	return EXIT_FAILURE;
}

/* irrecoverable failures */
int
execute_failure_short_exec_spec(char *const restrict *restrict from)
{
	write_muffle(STDERR_FILENO,
		     FAILURE_EXEC_SPEC_SHORT,
		     sizeof(FAILURE_EXEC_SPEC_SHORT) - 1lu);

	return EXIT_FAILURE;
}

int
execute_failure_no_exec_spec(char *const restrict *restrict from)
{
	write_muffle(STDERR_FILENO,
		     FAILURE_NO_EXEC_SPEC,
		     sizeof(FAILURE_NO_EXEC_SPEC) - 1lu);

	return EXIT_FAILURE;
}

/* dispatch load mode according to 'arg_ptr'
 *─────────────────────────────────────────────────────────────────────────── */
extern inline int
execute_dispatch(char *const restrict *restrict from,
		 const unsigned int rem_argc);
