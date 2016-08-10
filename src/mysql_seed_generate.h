#ifndef MYSQL_SEED_MYSQL_SEED_GENERATE_H_
#define MYSQL_SEED_MYSQL_SEED_GENERATE_H_

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include "generate/generate.h"		/* mysql_seed_generate */

/* macro constants
 *─────────────────────────────────────────────────────────────────────────── */
#define TINYINT_SIGNED_MIN	-128ll
#define TINYINT_SIGNED_MAX	127ll
#define TINYINT_UNSIGNED_MAX	255llu

#define SMALLINT_SIGNED_MIN	-32768ll
#define SMALLINT_SIGNED_MAX	32767ll
#define SMALLINT_UNSIGNED_MAX	65535llu

#define MEDIUMINT_SIGNED_MIN	-8388608ll
#define MEDIUMINT_SIGNED_MAX	8388607ll
#define MEDIUMINT_UNSIGNED_MAX	16777215llu

#define INT_SIGNED_MIN		-2147483648ll
#define INT_SIGNED_MAX		2147483647ll
#define INT_UNSIGNED_MAX	4294967295llu

#define BIGINT_SIGNED_MIN	-9223372036854775808ll
#define BIGINT_SIGNED_MAX	9223372036854775807ll
#define BIGINT_UNSIGNED_MAX	18446744073709551615llu


/* error messages
 *─────────────────────────────────────────────────────────────────────────── */
#define FAILURE_NO_DB_SPEC						\
GENERATE_FAILURE("no DB_SPEC provided") MORE_INFO_MESSAGE

#define FAILURE_DB_SPEC_SHORT						\
GENERATE_FAILURE("DB_SPEC too short - need at least "			\
		 DB_SPEC_LENGTH_MIN_STRING " arguments to describe "	\
		 "a database in generate mode, ignoring DB_SPEC "	\
		 "starting with:")

#define GENERATE_FAILURE_NO_VALID_DB_SPEC				\
GENERATE_FAILURE("no valid DB_SPEC")

#define IGNORING_DB_SPEC						\
"\n" ERROR_WRAP("ignoring DB_SPEC") "\n"

#define IGNORING_DB_SPEC_STARTING_WITH					\
"\n" ERROR_WRAP("ignoring DB_SPEC starting with:") "\n"

/* parsing DB_SPEC */
#define ERROR_DB_SPEC_SHORT						\
PARSE_ERROR_MESSAGE("DB_SPEC too short - need at least "		\
		    DB_SPEC_LENGTH_MIN_STRING " arguments to describe "	\
		    "a database in generate mode, ignoring final "	\
		    "DB_SPEC starting with:")

#define ERROR_EXPECTED_DB_FLAG_HEADER					\
PARSE_ERROR_HEADER("expected DATABASE flag instead of")

#define ERROR_INCOMPLETE_DB_SPEC_HEADER					\
PARSE_ERROR_HEADER("incomplete DB_SPEC")

#define ERROR_INCOMPLETE_DB_SPEC_COL_NAME				\
"\n" ERROR_WRAP("reason: expected COL_NAME to follow COLUMN flag, "	\
		"ignoring above DB_SPEC") "\n"

#define ERROR_INCOMPLETE_DB_SPEC_TBL_NAME				\
"\n" ERROR_WRAP("reason: expected TBL_NAME to follow TABLE flag, "	\
		"ignoring above DB_SPEC") "\n"

#define ERROR_INCOMPLETE_DB_SPEC_ROW_COUNT				\
"\n" ERROR_WRAP("reason: expected ROW_COUNT to follow TBL_NAME, "	\
		"ignoring above DB_SPEC") "\n"

#define ERROR_INCOMPLETE_DB_SPEC_COL_FLAG				\
"\n" ERROR_WRAP("reason: expected COLUMN flag to follow ROW_COUNT, "	\
		"ignoring above DB_SPEC") "\n"

/* parsing DB_NAME */
#define ERROR_INVALID_DB_NAME_HEADER					\
PARSE_ERROR_HEADER("invalid DB_NAME")

#define ERROR_INVALID_DB_NAME_EMPTY					\
PARSE_ERROR_HEADER("invalid DB_NAME (empty), ignoring DB_SPEC starting"	\
		   " with")

#define ERROR_INVALID_DB_NAME_REASON_LONG				\
"\n" ERROR_WRAP("reason: exceeded MySql maximum of "			\
		DB_NAME_LENGTH_MAX_STRING " non-null UTF-8 codepoints,"	\
		" ignoring DB_SPEC starting with:") "\n"

#define ERROR_INVALID_DB_NAME_REASON_INVALID				\
"\n" ERROR_WRAP("reason: includes one or more invalid UTF-8 codepoints"	\
		", ignoring DB_SPEC starting with:") "\n"

/* parsing TBL_SPEC */
#define ERROR_EXPECTED_TBL_FLAG_HEADER					\
PARSE_ERROR_HEADER("expected TABLE flag instead of")

/* parsing TBL_NAME */
#define ERROR_INVALID_TBL_NAME_HEADER					\
PARSE_ERROR_HEADER("invalid TBL_NAME")

#define ERROR_INVALID_TBL_NAME_EMPTY					\
PARSE_ERROR_HEADER("invalid TBL_NAME (empty), ignoring DB_SPEC "	\
		   "starting with")

#define ERROR_INVALID_TBL_NAME_REASON_LONG				\
"\n" ERROR_WRAP("reason: exceeded MySql maximum of "			\
		TBL_NAME_LENGTH_MAX_STRING " non-null UTF-8 codepoints"	\
		", ignoring DB_SPEC starting with:") "\n"

#define ERROR_INVALID_TBL_NAME_REASON_INVALID				\
"\n" ERROR_WRAP("reason: includes one or more invalid UTF-8 codepoints"	\
		", ignoring DB_SPEC starting with:") "\n"

/* parsing COL_SPEC */
#define ERROR_EXPECTED_COL_FLAG_HEADER					\
PARSE_ERROR_HEADER("expected COLUMN flag instead of")

/* parsing COL_NAME */
#define ERROR_INVALID_COL_NAME_HEADER					\
PARSE_ERROR_HEADER("invalid COL_NAME")

#define ERROR_INVALID_COL_NAME_EMPTY					\
PARSE_ERROR_HEADER("invalid COL_NAME (empty), ignoring DB_SPEC "	\
		   "starting with")

#define ERROR_INVALID_COL_NAME_REASON_LONG				\
"\n" ERROR_WRAP("reason: exceeded MySql maximum of "			\
		COL_NAME_LENGTH_MAX_STRING " non-null UTF-8 codepoints"	\
		", ignoring DB_SPEC starting with:") "\n"

#define ERROR_INVALID_COL_NAME_REASON_INVALID				\
"\n" ERROR_WRAP("reason: includes one or more invalid UTF-8 codepoints"	\
		", ignoring DB_SPEC starting with:") "\n"

/* parsing ROW_COUNT */
#define ERROR_INVALID_ROW_COUNT_HEADER					\
PARSE_ERROR_HEADER("invalid ROW_COUNT")

#define ERROR_INVALID_ROW_COUNT_REASON_INVALID				\
"\n" ERROR_WRAP("reason: not a number or overflows implementation-"	\
		"defined uintmax_t, ignoring DB_SPEC starting with:") "\n"

#define ERROR_INVALID_ROW_COUNT_REASON_ZERO				\
"\n" ERROR_WRAP("reason: ROW_COUNT must be ≥ 1, ignoring DB_SPEC "	\
		"starting with:") "\n"

#define ERROR_INVALID_ROW_COUNT_REASON_LARGE				\
"\n" ERROR_WRAP("reason: ROW_COUNT exceeds the system-Dependent "	\
		"constant UPTO_MAX (" UPTO_MAX_STRING "), ignoring "	\
		"DB_SPEC starting with:") "\n"

/* parsing COL_TYPE */
#define ERROR_NO_COL_TYPE						\
PARSE_ERROR_HEADER("no COL_TYPE provided, ignoring DB_SPEC starting "	\
		   "with")

#define ERROR_INVALID_COL_TYPE_HEADER					\
PARSE_ERROR_HEADER("invalid COL_TYPE")

#define ERROR_INVALID_COL_TYPE_REASON_NOTSUP				\
"\n" ERROR_WRAP("reason: not supported, ignoring DB_SPEC starting "	\
		"with:") "\n"

/* parsing COL_TYPE_Q */
#define ERROR_INVALID_COL_TYPE_Q_HEADER					\
PARSE_ERROR_HEADER("invalid COL_TYPE_Q")

#define ERROR_INVALID_COL_TYPE_Q_REASON_NOTSUP				\
"\n" ERROR_WRAP("reason: not supported, ignoring DB_SPEC starting "	\
		"with:") "\n"

#define ERROR_NO_BASE_STRING						\
PARSE_ERROR_HEADER("no column base string provided, ignoring DB_SPEC "	\
		   "starting with")

#define ERROR_INVALID_BASE_STRING_HEADER				\
PARSE_ERROR_HEADER("invalid column base string")

#define ERROR_INVALID_BASE_STRING_REASON_INVALID			\
"\n" ERROR_WRAP("reason: includes one or more invalid UTF-8 codepoints"	\
		", ignoring DB_SPEC starting with:") "\n"

#define ERROR_INVALID_BASE_STRING_REASON_LONG				\
"\n" ERROR_WRAP("reason: exceeded maximum of "				\
		BASE_STRING_LENGTH_MAX_STRING " non-null UTF-8 "	\
		"codepoints, ignoring DB_SPEC starting with:") "\n"

#define ERROR_NO_HASH_LENGTH						\
PARSE_ERROR_HEADER("no HASH_LENGTH provided, ignoring DB_SPEC starting"	\
		   " with")

#define ERROR_INVALID_HASH_LENGTH_HEADER				\
PARSE_ERROR_HEADER("invalid HASH_LENGTH")

#define ERROR_INVALID_HASH_LENGTH_REASON_INVALID			\
"\n" ERROR_WRAP("reason: not a number or overflows implementation-"	\
		"defined uintmax_t, ignoring DB_SPEC starting with:") "\n"

#define ERROR_INVALID_HASH_LENGTH_REASON_ZERO				\
"\n" ERROR_WRAP("reason: HASH_LENGTH must be ≥ 1, ignoring DB_SPEC "	\
		"starting with:") "\n"

#define ERROR_INVALID_HASH_LENGTH_REASON_LARGE				\
"\n" ERROR_WRAP("reason: HASH_LENGTH exceeds upper limit "		\
		"HASH_LENGTH_MAX (" HASH_LENGTH_MAX_STRING "), "	\
		"ignoring DB_SPEC starting with:") "\n"

/* parsing next SPEC */
#define ERROR_EXPECTED_COL_TBL_DB_FLAG_HEADER				\
PARSE_ERROR_HEADER("expected COLUMN flag, TABLE flag, DATABASE flag, "	\
		   "or end of arguments instead of")


/* typedefs, struct declarations
 *─────────────────────────────────────────────────────────────────────────── */
struct ValidDbSpecQueue {
	struct DbSpec **last;
	struct DbSpec *head;
};

struct GenerateSpecState {
	struct DbSpec  *db;
	struct TblSpec *tbl;
	struct ColSpec *col;
};

struct GenerateArgvState {
	struct ArgvInterval arg;
	struct ArgvInterval db_spec;
};

struct GenerateParseState {
	struct GenerateArgvState argv;
	struct GenerateSpecState specs;
	struct GeneratorCounter generator;
	struct DatabaseCounter database;
	struct ValidDbSpecQueue valid;
	int exit_status;
};

/* print error messsage and return 'EXIT_FAILURE'
 *─────────────────────────────────────────────────────────────────────────── */
/* irrecoverable failures */
inline void
generate_failure_no_db_spec(void)
{
	write_muffle(STDERR_FILENO,
		     FAILURE_NO_DB_SPEC,
		     sizeof(FAILURE_NO_DB_SPEC) - 1lu);
}

inline void
generate_failure_short_db_spec(char *const restrict *const restrict from,
			       char *const restrict *const restrict until)
{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  FAILURE_DB_SPEC_SHORT,
			  sizeof(FAILURE_DB_SPEC_SHORT) - 1);


	ptr = put_inspect_args(ptr,
			       from,
			       until - 1l);

	*ptr = '\n';

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr + 1l - &buffer[0]);
}

inline void
generate_failure_no_valid_db_spec(void)
{
	write_muffle(STDERR_FILENO,
		     GENERATE_FAILURE_NO_VALID_DB_SPEC,
		     sizeof(GENERATE_FAILURE_NO_VALID_DB_SPEC) - 1lu);

}

/* parsing errors (recoverable) */
/* DB_SPEC */
inline void
short_db_spec(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_DB_SPEC_SHORT,
			  sizeof(ERROR_DB_SPEC_SHORT) - 1);

	ptr = put_inspect_args(ptr,
			       argv->arg.from,
			       argv->arg.until - 1lu);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
expected_db_flag(char *const restrict invalid)
{
	char buffer[ARG_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_EXPECTED_DB_FLAG_HEADER,
			  sizeof(ERROR_EXPECTED_DB_FLAG_HEADER) - 1);


	ptr = put_string_inspect(ptr,
				 invalid,
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      IGNORING_DB_SPEC,
			      sizeof(IGNORING_DB_SPEC) - 1lu);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_db_name_empty(const struct GenerateArgvState *const restrict argv)

{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_DB_NAME_EMPTY,
			  sizeof(ERROR_INVALID_DB_NAME_EMPTY) - 1);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);


	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_db_name_invalid(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_DB_NAME_HEADER,
			  sizeof(ERROR_INVALID_DB_NAME_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_DB_NAME_REASON_INVALID,
			      sizeof(ERROR_INVALID_DB_NAME_REASON_INVALID)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_db_name_long(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_DB_NAME_HEADER,
			  sizeof(ERROR_INVALID_DB_NAME_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_DB_NAME_REASON_LONG,
			      sizeof(ERROR_INVALID_DB_NAME_REASON_LONG)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}


/* TBL_SPEC */
inline void
expected_tbl_flag(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_EXPECTED_TBL_FLAG_HEADER,
			  sizeof(ERROR_EXPECTED_TBL_FLAG_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      IGNORING_DB_SPEC_STARTING_WITH,
			      sizeof(IGNORING_DB_SPEC_STARTING_WITH) - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_tbl_name_empty(const struct GenerateArgvState *const restrict argv)

{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_TBL_NAME_EMPTY,
			  sizeof(ERROR_INVALID_TBL_NAME_EMPTY) - 1);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_tbl_name_invalid(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_TBL_NAME_HEADER,
			  sizeof(ERROR_INVALID_TBL_NAME_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_TBL_NAME_REASON_INVALID,
			      sizeof(ERROR_INVALID_TBL_NAME_REASON_INVALID)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_tbl_name_long(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_TBL_NAME_HEADER,
			  sizeof(ERROR_INVALID_TBL_NAME_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_TBL_NAME_REASON_LONG,
			      sizeof(ERROR_INVALID_TBL_NAME_REASON_LONG)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

/* COL_SPEC */
inline void
expected_col_flag(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_EXPECTED_COL_FLAG_HEADER,
			  sizeof(ERROR_EXPECTED_COL_FLAG_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      IGNORING_DB_SPEC_STARTING_WITH,
			      sizeof(IGNORING_DB_SPEC_STARTING_WITH) - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_col_name_empty(const struct GenerateArgvState *const restrict argv)

{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_COL_NAME_EMPTY,
			  sizeof(ERROR_INVALID_COL_NAME_EMPTY) - 1);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_col_name_invalid(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_COL_NAME_HEADER,
			  sizeof(ERROR_INVALID_COL_NAME_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_COL_NAME_REASON_INVALID,
			      sizeof(ERROR_INVALID_COL_NAME_REASON_INVALID)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_col_name_long(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_COL_NAME_HEADER,
			  sizeof(ERROR_INVALID_COL_NAME_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_COL_NAME_REASON_LONG,
			      sizeof(ERROR_INVALID_COL_NAME_REASON_LONG)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}


inline void
invalid_row_count_invalid(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_ROW_COUNT_HEADER,
			  sizeof(ERROR_INVALID_ROW_COUNT_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_ROW_COUNT_REASON_INVALID,
			      sizeof(ERROR_INVALID_ROW_COUNT_REASON_INVALID)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}


inline void
invalid_row_count_zero(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_ROW_COUNT_HEADER,
			  sizeof(ERROR_INVALID_ROW_COUNT_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_ROW_COUNT_REASON_ZERO,
			      sizeof(ERROR_INVALID_ROW_COUNT_REASON_ZERO)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}


inline void
invalid_row_count_large(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_ROW_COUNT_HEADER,
			  sizeof(ERROR_INVALID_ROW_COUNT_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_ROW_COUNT_REASON_LARGE,
			      sizeof(ERROR_INVALID_ROW_COUNT_REASON_LARGE)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}


inline void
no_col_type(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_NO_COL_TYPE,
			  sizeof(ERROR_NO_COL_TYPE) - 1);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from - 1l);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}


inline void
invalid_col_type_notsup(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_COL_TYPE_HEADER,
			  sizeof(ERROR_INVALID_COL_TYPE_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_COL_TYPE_REASON_NOTSUP,
			      sizeof(ERROR_INVALID_COL_TYPE_REASON_NOTSUP)
			      - 1lu);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}


inline void
invalid_col_type_q_notsup(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_COL_TYPE_Q_HEADER,
			  sizeof(ERROR_INVALID_COL_TYPE_Q_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_COL_TYPE_Q_REASON_NOTSUP,
			      sizeof(ERROR_INVALID_COL_TYPE_Q_REASON_NOTSUP)
			      - 1lu);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}


inline void
no_base_string(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_NO_BASE_STRING,
			  sizeof(ERROR_NO_BASE_STRING) - 1);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from - 1l);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_base_string_invalid(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_BASE_STRING_HEADER,
			  sizeof(ERROR_INVALID_BASE_STRING_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_BASE_STRING_REASON_INVALID,
			      sizeof(ERROR_INVALID_BASE_STRING_REASON_INVALID)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_base_string_long(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_BASE_STRING_HEADER,
			  sizeof(ERROR_INVALID_BASE_STRING_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_BASE_STRING_REASON_LONG,
			      sizeof(ERROR_INVALID_BASE_STRING_REASON_LONG)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
no_hash_length(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_NO_HASH_LENGTH,
			  sizeof(ERROR_NO_HASH_LENGTH) - 1);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from - 1l);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_hash_length_invalid(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_HASH_LENGTH_HEADER,
			  sizeof(ERROR_INVALID_HASH_LENGTH_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_HASH_LENGTH_REASON_INVALID,
			      sizeof(ERROR_INVALID_HASH_LENGTH_REASON_INVALID)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_hash_length_zero(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_HASH_LENGTH_HEADER,
			  sizeof(ERROR_INVALID_HASH_LENGTH_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_HASH_LENGTH_REASON_ZERO,
			      sizeof(ERROR_INVALID_HASH_LENGTH_REASON_ZERO)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
invalid_hash_length_large(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INVALID_HASH_LENGTH_HEADER,
			  sizeof(ERROR_INVALID_HASH_LENGTH_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      ERROR_INVALID_HASH_LENGTH_REASON_LARGE,
			      sizeof(ERROR_INVALID_HASH_LENGTH_REASON_LARGE)
			      - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
expected_col_tbl_db_flag(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARG_ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_EXPECTED_COL_TBL_DB_FLAG_HEADER,
			  sizeof(ERROR_EXPECTED_COL_TBL_DB_FLAG_HEADER) - 1);

	ptr = put_string_inspect(ptr,
				 *(argv->arg.from),
				 LENGTH_INSPECT_MAX);

	ptr = put_string_size(ptr,
			      IGNORING_DB_SPEC_STARTING_WITH,
			      sizeof(IGNORING_DB_SPEC_STARTING_WITH) - 1lu);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

/* incomplete DB_SPEC */
inline void
incomplete_db_spec_col_name(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INCOMPLETE_DB_SPEC_HEADER,
			  sizeof(ERROR_INCOMPLETE_DB_SPEC_HEADER) - 1);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from - 1l);

	ptr = put_string_size(ptr,
			      ERROR_INCOMPLETE_DB_SPEC_COL_NAME,
			      sizeof(ERROR_INCOMPLETE_DB_SPEC_COL_NAME) - 1lu);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
incomplete_db_spec_tbl_name(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INCOMPLETE_DB_SPEC_HEADER,
			  sizeof(ERROR_INCOMPLETE_DB_SPEC_HEADER) - 1);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from - 1l);

	ptr = put_string_size(ptr,
			      ERROR_INCOMPLETE_DB_SPEC_TBL_NAME,
			      sizeof(ERROR_INCOMPLETE_DB_SPEC_TBL_NAME) - 1lu);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

inline void
incomplete_db_spec_row_count(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INCOMPLETE_DB_SPEC_HEADER,
			  sizeof(ERROR_INCOMPLETE_DB_SPEC_HEADER) - 1);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from - 1l);

	ptr = put_string_size(ptr,
			      ERROR_INCOMPLETE_DB_SPEC_ROW_COUNT,
			      sizeof(ERROR_INCOMPLETE_DB_SPEC_ROW_COUNT) - 1lu);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}


inline void
incomplete_db_spec_col_flag(const struct GenerateArgvState *const restrict argv)
{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	char *restrict ptr
	= put_string_size(&buffer[0],
			  ERROR_INCOMPLETE_DB_SPEC_HEADER,
			  sizeof(ERROR_INCOMPLETE_DB_SPEC_HEADER) - 1);

	ptr = put_inspect_args(ptr,
			       argv->db_spec.from,
			       argv->arg.from - 1l);

	ptr = put_string_size(ptr,
			      ERROR_INCOMPLETE_DB_SPEC_COL_FLAG,
			      sizeof(ERROR_INCOMPLETE_DB_SPEC_COL_FLAG) - 1lu);

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}


/* confirm flag match
 *─────────────────────────────────────────────────────────────────────────── */
inline bool
db_flag_match(struct GenerateArgvState *const restrict argv)
{
	const bool matched_db_flag = flag_match(*(argv->arg.from),
						'd',
						"database");

	if (matched_db_flag)
		argv->db_spec.from = argv->arg.from;
	else
		expected_db_flag(*(argv->arg.from));

	return matched_db_flag;
}

inline bool
tbl_flag_match(struct GenerateArgvState *const restrict argv)
{
	const bool matched_tbl_flag = flag_match(*(argv->arg.from),
					       't',
					       "table");

	if (!matched_tbl_flag)
		expected_tbl_flag(argv);

	return matched_tbl_flag;
}

inline bool
col_flag_match(struct GenerateArgvState *const restrict argv)
{
	const bool matched_col_flag = flag_match(*(argv->arg.from),
					       'c',
					       "column");

	if (!matched_col_flag)
		expected_col_flag(argv);

	return matched_col_flag;
}


/* parse UTF-8 identifiers from input
 *─────────────────────────────────────────────────────────────────────────── */
inline bool
parse_db_name(struct String *const restrict db_name,
	      struct GenerateArgvState *const restrict argv)
{
	const octet_t *restrict octets
	= (const octet_t *restrict) *(argv->arg.from);

	if (*octets == '\0') {
		invalid_db_name_empty(argv);
		return false;
	}

	unsigned int width;

	size_t rem_code_points = DB_NAME_LENGTH_MAX;

	while (1) {
		width = utf8_width(octets);

		if (width == 0u) {
			invalid_db_name_invalid(argv);
			return false;
		}

		octets += width;

		if (*octets == '\0') {
			db_name->bytes  = *(argv->arg.from);
			db_name->length = octets
					- ((const octet_t *const restrict)
					   db_name->bytes);
			return true;
		}

		--rem_code_points;

		if (rem_code_points == 0lu) {
			invalid_db_name_long(argv);
			return false;
		}
	}
}


inline bool
parse_tbl_name(struct String *const restrict tbl_name,
	      struct GenerateArgvState *const restrict argv)
{
	const octet_t *restrict octets
	= (const octet_t *restrict) *(argv->arg.from);

	if (*octets == '\0') {
		invalid_tbl_name_empty(argv);
		return false;
	}

	unsigned int width;

	size_t rem_code_points = TBL_NAME_LENGTH_MAX;

	while (1) {
		width = utf8_width(octets);

		if (width == 0u) {
			invalid_tbl_name_invalid(argv);
			return false;
		}

		octets += width;

		if (*octets == '\0') {
			tbl_name->bytes  = *(argv->arg.from);
			tbl_name->length = octets
					- ((const octet_t *const restrict)
					   tbl_name->bytes);
			return true;
		}

		--rem_code_points;

		if (rem_code_points == 0lu) {
			invalid_tbl_name_long(argv);
			return false;
		}
	}
}


inline bool
parse_col_name(struct String *const restrict col_name,
	      struct GenerateArgvState *const restrict argv)
{
	const octet_t *restrict octets
	= (const octet_t *restrict) *(argv->arg.from);

	if (*octets == '\0') {
		invalid_col_name_empty(argv);
		return false;
	}

	unsigned int width;

	size_t rem_code_points = COL_NAME_LENGTH_MAX;

	while (1) {
		width = utf8_width(octets);

		if (width == 0u) {
			invalid_col_name_invalid(argv);
			return false;
		}

		octets += width;

		if (*octets == '\0') {
			col_name->bytes  = *(argv->arg.from);
			col_name->length = octets
					- ((const octet_t *const restrict)
					   col_name->bytes);
			return true;
		}

		--rem_code_points;

		if (rem_code_points == 0lu) {
			invalid_col_name_long(argv);
			return false;
		}
	}
}


inline bool
parse_base_string(struct String *const restrict base,
		  struct GenerateArgvState *const restrict argv)
{
	const octet_t *restrict octets
	= (const octet_t *restrict) *(argv->arg.from);

	unsigned int width;

	size_t rem_code_points = BASE_STRING_LENGTH_MAX;

	while (1) {
		if (*octets == '\0') {
			base->bytes  = *(argv->arg.from);
			base->length = octets
					- ((const octet_t *const restrict)
					   base->bytes);
			return true;
		}

		width = utf8_width(octets);

		if (width == 0u) {
			invalid_base_string_invalid(argv);
			return false;
		}

		octets += width;

		--rem_code_points;

		if (rem_code_points == 0lu) {
			invalid_base_string_long(argv);
			return false;
		}
	}
}


/* parse ASCII numbers from input
 *─────────────────────────────────────────────────────────────────────────── */
inline bool
parse_row_count(size_t *const restrict row_count,
		struct GenerateArgvState *const restrict argv)
{
	uintmax_t parsed;

	if (!parse_uint(&parsed,
			*(argv->arg.from))) {
		invalid_row_count_invalid(argv);
		return false;
	}

	if (parsed == 0llu) {
		invalid_row_count_zero(argv);
		return false;
	}

	if (parsed > UPTO_MAX) {
		invalid_row_count_large(argv);
		return false;
	}

	*row_count = (size_t) parsed;
	return true;
}

inline bool
parse_hash_length(size_t *const restrict hash_length,
		  struct GenerateArgvState *const restrict argv)
{
	uintmax_t parsed;

	if (!parse_uint(&parsed,
			*(argv->arg.from))) {
		invalid_hash_length_invalid(argv);
		return false;
	}

	if (parsed == 0llu) {
		invalid_hash_length_zero(argv);
		return false;
	}

	if (parsed > HASH_LENGTH_MAX) {
		invalid_hash_length_large(argv);
		return false;
	}

	*hash_length = (size_t) parsed;
	return true;
}


/* assign type according to MySQL data types, limits
 *─────────────────────────────────────────────────────────────────────────── */
inline void
type_set_char(struct Label *const restrict type,
	      const uintmax_t length)
{
	char *restrict ptr = &type->buffer[0];

	PUT_STRING_WIDTH(ptr, "CHAR(", 5);

	ptr = put_uint(ptr,
		       length);

	SET_STRING_WIDTH(ptr, ")", 2);

	type->width = ptr + 1l - &type->buffer[0];
}

inline void
type_set_char_parsed_length(struct Label *const restrict type,
			    const char *restrict length)
{
	char *restrict ptr = &type->buffer[0];

	PUT_STRING_WIDTH(ptr, "CHAR(", 5);

	while (*length == '0')	/* ignore leading zeros */
		++length;

	ptr = put_string(ptr,
			 length);

	SET_STRING_WIDTH(ptr, ")", 2);

	type->width = ptr + 1l - &type->buffer[0];
}

inline void
type_set_varchar(struct Label *const restrict type,
		 const uintmax_t length)
{
	char *restrict ptr = &type->buffer[0];

	PUT_STRING_WIDTH(ptr, "VARCHAR(", 8);

	ptr = put_uint(ptr,
		       length);

	SET_STRING_WIDTH(ptr, ")", 2);

	type->width = ptr + 1l - &type->buffer[0];

}

inline void
type_set_timestamp(struct Label *const restrict type)
{
	SET_STRING_WIDTH(&type->buffer[0],
			 "TIMESTAMP",
			 10);
	type->width = 9u;
}


inline void
type_set_tinyint(struct Label *const restrict type)
{
	SET_STRING_WIDTH(&type->buffer[0],
			 "TINYINT",
			 8);
	type->width = 7u;
}

inline void
type_set_smallint(struct Label *const restrict type)
{
	SET_STRING_WIDTH(&type->buffer[0],
			 "SMALLINT",
			 9);
	type->width = 8u;
}

inline void
type_set_mediumint(struct Label *const restrict type)
{
	SET_STRING_WIDTH(&type->buffer[0],
			 "MEDIUMINT",
			 10);
	type->width = 9u;
}

inline void
type_set_int(struct Label *const restrict type)
{
	SET_STRING_WIDTH(&type->buffer[0],
			 "INT",
			 4);
	type->width = 3u;
}

inline void
type_set_bigint(struct Label *const restrict type)
{
	SET_STRING_WIDTH(&type->buffer[0],
			 "BIGINT",
			 7);
	type->width = 6u;
}

inline void
type_assign_integer_unsigned(struct Label *const restrict type,
			     const uintmax_t upto)
{
	if (upto > SMALLINT_UNSIGNED_MAX) {
		if (upto > MEDIUMINT_UNSIGNED_MAX) {
			if (upto > INT_UNSIGNED_MAX) {
				type_set_bigint(type);
			} else {
				type_set_int(type);
			}
		} else {
			type_set_mediumint(type);
		}
	} else {
		if (upto > TINYINT_UNSIGNED_MAX) {
			type_set_smallint(type);
		} else {
			type_set_tinyint(type);
		}
	}
}

inline void
type_assign_upto(struct Label *const restrict type,
		 const size_t upto)
{
#if LARGE_UPTO_MAX
	if (upto > SMALLINT_UNSIGNED_MAX) {
		type_set_mediumint(type);
	} else {
#endif /* if LARGE_UPTO_MAX */

		if (upto > TINYINT_UNSIGNED_MAX) {
			type_set_smallint(type);
		} else {
			type_set_tinyint(type);
		}

#if LARGE_UPTO_MAX
	}
#endif /* if LARGE_UPTO_MAX */
}


/* make compiler happy
 *─────────────────────────────────────────────────────────────────────────── */
inline void
parse_next_col_spec(struct GenerateParseState *const restrict state);
inline void
parse_next_db_spec(struct GenerateParseState *const restrict state);
inline void
parse_next_tbl_spec(struct GenerateParseState *const restrict state);
inline void
generate_parse_error(struct GenerateParseState *const restrict state);


/* finished parsing
 *─────────────────────────────────────────────────────────────────────────── */
inline void
parse_table_complete(struct GenerateParseState *const restrict state)
{
	state->specs.tbl->col_specs.until = state->specs.col + 1l;
}

inline void
parse_valid_complete(struct GenerateParseState *const restrict state)
{
	parse_table_complete(state);
	state->specs.tbl->next = NULL;
	*(state->valid.last) = state->specs.db;
	state->valid.last = &state->specs.db->next;
	generator_counter_update(&state->generator,
				 &state->database);
}

inline void
parse_database_complete(struct GenerateParseState *const restrict state)
{
	parse_valid_complete(state);
	state->specs.db = (struct DbSpec *) state->specs.tbl->col_specs.until;
}

inline void
generate_parse_complete(struct GenerateParseState *const restrict state)
{
	parse_valid_complete(state);
	*(state->valid.last) = NULL;
}

inline void
parse_column_complete(struct GenerateParseState *const restrict state)
{
	if (state->argv.arg.from == state->argv.arg.until) {
		generate_parse_complete(state);
		return;
	}

	char *restrict arg = *(state->argv.arg.from);

	if (*arg != '-') {
EXPECTED_COL_TBL_DB_FLAG:
		expected_col_tbl_db_flag(&state->argv);
		++(state->argv.arg.from);
		generate_parse_error(state);
		return;
	}

	++arg;
	char *const restrict rem = arg + 1l;

	switch (*arg) {
	case '-':
		break; /* parse long SPEC */

	case 'c':
		if (*rem == '\0') {
			parse_next_col_spec(state);
			return;
		}
		goto EXPECTED_COL_TBL_DB_FLAG;

	case 'd':
		if (*rem == '\0') {
			parse_database_complete(state);
			parse_next_db_spec(state);
			return;
		}
		goto EXPECTED_COL_TBL_DB_FLAG;

	case 't':
		if (*rem == '\0') {
			parse_table_complete(state);
			parse_next_tbl_spec(state);
			return;
		}
		goto EXPECTED_COL_TBL_DB_FLAG;

	default:
		goto EXPECTED_COL_TBL_DB_FLAG;
	}

	switch (*rem) {
	case 'c':
		if (strings_equal("olumn", rem + 1l)) {
			parse_next_col_spec(state);
			return;
		}
		goto EXPECTED_COL_TBL_DB_FLAG;

	case 'd':
		if (strings_equal("atabase", rem + 1l)) {
			parse_database_complete(state);
			parse_next_db_spec(state);
			return;
		}
		goto EXPECTED_COL_TBL_DB_FLAG;

	case 't':
		if (strings_equal("able", rem + 1l)) {
			parse_table_complete(state);
			parse_next_tbl_spec(state);
			return;
		}
		goto EXPECTED_COL_TBL_DB_FLAG;

	default:
		goto EXPECTED_COL_TBL_DB_FLAG;
	}
}


/* set COL_SPEC
 *─────────────────────────────────────────────────────────────────────────── */
/* -c COL_NAME -i */
inline void
column_integer_default(struct ColSpec *const restrict col_spec,
		       const size_t row_count,
		       size_t *const restrict counter_upto)
{
	type_assign_upto(&col_spec->type,
			 row_count);

	col_spec->build = &build_column_integer_unique;

	if (row_count > *counter_upto)
		*counter_upto = row_count;
}

/* -c COL_NAME -s -u BASE_STRING */
inline void
column_string_unique(struct ColSpec *const restrict col_spec,
		     const size_t row_count,
		     size_t *const restrict counter_upto)
{
	type_set_varchar(&col_spec->type,
			 (uintmax_t)
			 (col_spec->type_qualifier.string.base.length
			  + uint_digit_count(row_count)));

	col_spec->build = &build_column_string_unique;

	if (row_count > *counter_upto)
		*counter_upto = row_count;
}

/* -c COL_NAME -s -f BASE_STRING */
inline void
column_string_fixed(struct ColSpec *const restrict col_spec)
{
	type_set_char(&col_spec->type,
		      (uintmax_t)
		      col_spec->type_qualifier.string.base.length);

	col_spec->build = &build_column_string_fixed;
}

/* -c COL_NAME -s -uu */
inline void
column_string_uuid(struct ColSpec *const restrict col_spec,
		   unsigned int *const restrict ctor_flags)
{
	SET_UUID_TYPE(&col_spec->type.buffer[0]);

	col_spec->type.width = UUID_TYPE_NN_WIDTH;

	col_spec->build = &build_column_string_uuid;

	*ctor_flags |= UUID_CTOR_FLAG;
}

/* -c COL_NAME -s -h HASH_LENGTH */
inline void
column_string_hash(struct ColSpec *const restrict col_spec,
		   const char *const restrict length)
{
	type_set_char_parsed_length(&col_spec->type,
				    length);

	col_spec->build = &build_column_string_hash;
}


/* -c COL_NAME -n1 */
inline void
column_string_names_first(struct ColSpec *const restrict col_spec,
			  unsigned int *const restrict ctor_flags)
{
	SET_NAMES_FIRST_TYPE(&col_spec->type.buffer[0]);

	col_spec->type.width = NAMES_FIRST_TYPE_NN_WIDTH;

	col_spec->build = &build_column_string_names_first;

	*ctor_flags |= RAND_CTOR_FLAG;
}

/* -c COL_NAME -nl */
inline void
column_string_names_last(struct ColSpec *const restrict col_spec,
			 unsigned int *const restrict ctor_flags)
{
	SET_NAMES_LAST_TYPE(&col_spec->type.buffer[0]);

	col_spec->type.width = NAMES_LAST_TYPE_NN_WIDTH;

	col_spec->build = &build_column_string_names_last;

	*ctor_flags |= RAND_CTOR_FLAG;
}

/* -c COL_NAME -nf */
inline void
column_string_names_full(struct ColSpec *const restrict col_spec,
			 unsigned int *const restrict ctor_flags)
{
	SET_NAMES_FULL_TYPE(&col_spec->type.buffer[0]);

	col_spec->type.width = NAMES_FULL_TYPE_NN_WIDTH;

	col_spec->build = &build_column_string_names_full;

	*ctor_flags |= RAND_CTOR_FLAG;
}

/* -c COL_NAME -s */
inline void
column_string_default(struct ColSpec *const restrict col_spec,
		      const size_t row_count,
		      size_t *const restrict counter_upto)
{
	col_spec->type_qualifier.string.base = col_spec->name;
	column_string_unique(col_spec,
			     row_count,
			     counter_upto);
}

/* -c COL_NAME -ts -u */
inline void
column_timestamp_unique(struct ColSpec *const restrict col_spec)
{
	type_set_timestamp(&col_spec->type);
	col_spec->build = &build_column_timestamp_unique;
}

/* -c COL_NAME -ts -f */
inline void
column_timestamp_fixed(struct ColSpec *const restrict col_spec)
{
	type_set_timestamp(&col_spec->type);
	col_spec->build = &build_column_timestamp_fixed;
}

/* -c COL_NAME -ts */
inline void
column_timestamp_default(struct ColSpec *const restrict col_spec)
{
	type_set_timestamp(&col_spec->type);
	col_spec->build = &build_column_timestamp_unique;
}

/* parse spec groups
 *─────────────────────────────────────────────────────────────────────────── */
inline void
parse_integer_qualifier(struct GenerateParseState *const restrict state)
{
	++(state->argv.arg.from);

	if (state->argv.arg.from == state->argv.arg.until) {
		column_integer_default(state->specs.col,
				       state->specs.tbl->row_count,
				       &state->database.counter_upto);
		generate_parse_complete(state); /* done parsing */
		return;
	}
}


inline void
parse_string_qualifier(struct GenerateParseState *const restrict state)
{
	++(state->argv.arg.from);

	if (state->argv.arg.from == state->argv.arg.until) {
STRING_DEFAULT_PARSE_COMPLETE:
		column_string_default(state->specs.col,
				      state->specs.tbl->row_count,
				      &state->database.counter_upto);
		generate_parse_complete(state); /* done parsing */
		return;
	}

	const char *restrict arg = *(state->argv.arg.from);

	if (*arg != '-') {
INVALID_STRING_QUALIFIER_NOTSUP:
		invalid_col_type_q_notsup(&state->argv);
		++(state->argv.arg.from);
		generate_parse_error(state);
		return;
	}

	++arg;
	const char *const restrict rem = arg + 1l;

	switch (*arg) {
	case '-':
		break;	/* parse long string qualifier */

	case 'u':
		switch (*rem) {
		case '\0':
STRING_UNIQUE:
			++(state->argv.arg.from);

			if (state->argv.arg.from == state->argv.arg.until)
				goto STRING_DEFAULT_PARSE_COMPLETE;

			const bool valid_base_string
			= parse_base_string(&state->specs.col->type_qualifier.string.base,
					    &state->argv);

			++(state->argv.arg.from);

			if (valid_base_string) {
				column_string_unique(state->specs.col,
						     state->specs.tbl->row_count,
						     &state->database.counter_upto);
				parse_column_complete(state);
			} else {
				generate_parse_error(state);
			}
			return;

		case 'u':
			if (rem[1] == '\0') {
STRING_UUID:
				++(state->argv.arg.from);

				column_string_uuid(state->specs.col,
						   &state->database.ctor_flags);
				parse_column_complete(state);
				return;
			}
			goto INVALID_STRING_QUALIFIER_NOTSUP;

		default:
			goto INVALID_STRING_QUALIFIER_NOTSUP;
		}

	case 'f':
		if (*rem == '\0') {
STRING_FIXED:
			++(state->argv.arg.from);

			if (state->argv.arg.from == state->argv.arg.until) {
				no_base_string(&state->argv);
TERMINATE_VALID_EXIT_FAILURE:
				*(state->valid.last) = NULL;
				state->exit_status = EXIT_FAILURE;
				return;
			}

			const bool valid_base_string
			= parse_base_string(&state->specs.col->type_qualifier.string.base,
					    &state->argv);

			++(state->argv.arg.from);

			if (valid_base_string) {
				column_string_fixed(state->specs.col);
				parse_column_complete(state);
			} else {
				generate_parse_error(state);
			}
			return;
		}
		goto INVALID_STRING_QUALIFIER_NOTSUP;

	case 'h':
		if (*rem == '\0') {
STRING_HASH:
			++(state->argv.arg.from);

			if (state->argv.arg.from == state->argv.arg.until) {
				no_hash_length(&state->argv);
				goto TERMINATE_VALID_EXIT_FAILURE;
			}

			if (parse_hash_length(&state->specs.col->type_qualifier.string.length_scale.fixed,
					      &state->argv)) {
				column_string_hash(state->specs.col,
						   *(state->argv.arg.from));
				++(state->argv.arg.from);
				parse_column_complete(state);
			} else {
				++(state->argv.arg.from);
				generate_parse_error(state);
			}
			return;
		}
		goto INVALID_STRING_QUALIFIER_NOTSUP;

	case 'n':
		switch (*rem) {
		case '1':
			if (rem[1] == '\0') {
STRING_NAMES_FIRST:
				column_string_names_first(state->specs.col,
							  &state->database.ctor_flags);
				++(state->argv.arg.from);
				parse_column_complete(state);
				return;
			}
			goto INVALID_STRING_QUALIFIER_NOTSUP;

		case 'l':
			if (rem[1] == '\0') {
STRING_NAMES_LAST:
				column_string_names_last(state->specs.col,
							 &state->database.ctor_flags);
				++(state->argv.arg.from);
				parse_column_complete(state);
				return;
			}
			goto INVALID_STRING_QUALIFIER_NOTSUP;

		case 'f':
			if (rem[1] == '\0') {
STRING_NAMES_FULL:
				column_string_names_full(state->specs.col,
							 &state->database.ctor_flags);
				++(state->argv.arg.from);
				parse_column_complete(state);
				return;
			}
			goto INVALID_STRING_QUALIFIER_NOTSUP;

		default:
			goto INVALID_STRING_QUALIFIER_NOTSUP;
		}

	case 'c':
		if (*rem == '\0') {
STRING_DEFAULT_NEXT_COL_SPEC:
			column_string_default(state->specs.col,
					      state->specs.tbl->row_count,
					      &state->database.counter_upto);
			parse_next_col_spec(state);
			return;
		}
		goto INVALID_STRING_QUALIFIER_NOTSUP;

	case 'd':
		if (*rem == '\0') {
STRING_DEFAULT_NEXT_DB_SPEC:
			column_string_default(state->specs.col,
					      state->specs.tbl->row_count,
					      &state->database.counter_upto);
			parse_database_complete(state);
			parse_next_db_spec(state);
			return;
		}
		goto INVALID_STRING_QUALIFIER_NOTSUP;

	case 't':
		if (*rem == '\0') {
STRING_DEFAULT_NEXT_TBL_SPEC:
			column_string_default(state->specs.col,
					      state->specs.tbl->row_count,
					      &state->database.counter_upto);
			parse_table_complete(state);
			parse_next_tbl_spec(state);
			return;
		}
		goto INVALID_STRING_QUALIFIER_NOTSUP;

	default:
		goto INVALID_STRING_QUALIFIER_NOTSUP;
	}


	switch (*rem) {
	case 'u':
		switch (rem[1]) {
		case 'n':
			if (strings_equal("ique", rem + 2l))
				goto STRING_UNIQUE;

			goto INVALID_STRING_QUALIFIER_NOTSUP;

		case 'u':
			if (strings_equal("id", rem + 2l)) {
				goto STRING_UUID;
			}

			goto INVALID_STRING_QUALIFIER_NOTSUP;

		default:
			goto INVALID_STRING_QUALIFIER_NOTSUP;
		}

	case 'f':
		if (strings_equal("ixed", rem + 1l))
			goto STRING_FIXED;

		goto INVALID_STRING_QUALIFIER_NOTSUP;

	case 'h':
		if (strings_equal("ash", rem + 1l))
			goto STRING_HASH;

		goto INVALID_STRING_QUALIFIER_NOTSUP;

	case 'n':
		arg = string_starts_with(rem + 1l,
					 "ames-");

		if (arg == NULL)
			goto INVALID_STRING_QUALIFIER_NOTSUP;

		switch (*arg) {
		case 'f':
			switch (arg[1]) {
			case 'i':
				if (strings_equal("rst", arg + 2l))
					goto STRING_NAMES_FIRST;

				goto INVALID_STRING_QUALIFIER_NOTSUP;

			case 'u':
				if (strings_equal("ll", arg + 2l))
					goto STRING_NAMES_FULL;

				goto INVALID_STRING_QUALIFIER_NOTSUP;

			default:
				goto INVALID_STRING_QUALIFIER_NOTSUP;
			}

		case 'l':
			if (strings_equal("ast", arg + 1l))
					goto STRING_NAMES_LAST;

			goto INVALID_STRING_QUALIFIER_NOTSUP;

		default:
			goto INVALID_STRING_QUALIFIER_NOTSUP;
		}

	case 'c':
		if (strings_equal("olumn", rem + 1l))
			goto STRING_DEFAULT_NEXT_COL_SPEC;

		goto INVALID_STRING_QUALIFIER_NOTSUP;

	case 'd':
		if (strings_equal("atabase", rem + 1l))
			goto STRING_DEFAULT_NEXT_DB_SPEC;

		goto INVALID_STRING_QUALIFIER_NOTSUP;

	case 't':
		if (strings_equal("able", rem + 1l))
			goto STRING_DEFAULT_NEXT_TBL_SPEC;

		goto INVALID_STRING_QUALIFIER_NOTSUP;

	default:
		goto INVALID_STRING_QUALIFIER_NOTSUP;
	}
}

inline void
parse_timestamp_qualifier(struct GenerateParseState *const restrict state)
{
	++(state->argv.arg.from);

	if (state->argv.arg.from == state->argv.arg.until) {
		column_timestamp_default(state->specs.col);
		generate_parse_complete(state); /* done parsing */
		return;
	}

	const char *restrict arg = *(state->argv.arg.from);

	if (*arg != '-') {
INVALID_TIMESTAMP_QUALIFIER_NOTSUP:
		invalid_col_type_q_notsup(&state->argv);
		++(state->argv.arg.from);
		generate_parse_error(state);
		return;
	}

	++arg;
	const char *const restrict rem = arg + 1l;

	switch (*arg) {
	case '-':
		break;	/* parse long string qualifier */
	case 'f':
		if (*rem == '\0') {
TIMESTAMP_FIXED:
			column_timestamp_fixed(state->specs.col);
			++(state->argv.arg.from);
			parse_column_complete(state);
			return;
		}
		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;

	case 'u':
		if (*rem == '\0') {
TIMESTAMP_UNIQUE:
			column_timestamp_unique(state->specs.col);
			++(state->argv.arg.from);
			parse_column_complete(state);
			return;
		}
		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;

	case 'c':
		if (*rem == '\0') {
TIMESTAMP_DEFAULT_NEXT_COL_SPEC:
			column_timestamp_default(state->specs.col);
			parse_next_col_spec(state);
			return;
		}
		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;

	case 'd':
		if (*rem == '\0') {
TIMESTAMP_DEFAULT_NEXT_DB_SPEC:
			column_timestamp_default(state->specs.col);
			parse_database_complete(state);
			parse_next_db_spec(state);
			return;
		}
		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;

	case 't':
		if (*rem == '\0') {
TIMESTAMP_DEFAULT_NEXT_TBL_SPEC:
			column_timestamp_default(state->specs.col);
			parse_table_complete(state);
			parse_next_tbl_spec(state);
			return;
		}
		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;

	default:
		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;
	}


	switch (*rem) {
	case 'f':
		if (strings_equal("ixed", rem + 1l))
			goto TIMESTAMP_FIXED;

		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;

	case 'u':
		if (strings_equal("nique", rem + 1l))
			goto TIMESTAMP_UNIQUE;

		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;

	case 'c':
		if (strings_equal("olumn", rem + 1l))
			goto TIMESTAMP_DEFAULT_NEXT_COL_SPEC;

		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;

	case 'd':
		if (strings_equal("atabase", rem + 1l))
			goto TIMESTAMP_DEFAULT_NEXT_DB_SPEC;

		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;

	case 't':
		if (strings_equal("able", rem + 1l))
			goto TIMESTAMP_DEFAULT_NEXT_TBL_SPEC;

		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;

	default:
		goto INVALID_TIMESTAMP_QUALIFIER_NOTSUP;
	}
}

inline void
parse_col_type(struct GenerateParseState *const restrict state)
{
	const char *restrict arg = *(state->argv.arg.from);

	if (*arg != '-') {
INVALID_COL_TYPE_NOTSUP:
		invalid_col_type_notsup(&state->argv);
		++(state->argv.arg.from);
		generate_parse_error(state);
		return;
	}

	++arg;
	const char *const restrict rem = arg + 1l;

	switch (*arg) {
	case '-':
		break;	/* parse long COL_TYPE */

	case 'i':
		if (*rem == '\0') {
			parse_integer_qualifier(state);
			return;
		}
		goto INVALID_COL_TYPE_NOTSUP;

	case 's':
		if (*rem == '\0') {
			parse_string_qualifier(state);
			return;
		}
		goto INVALID_COL_TYPE_NOTSUP;

	case 't':
		if (*rem == 's' && rem[1] == '\0') {
			parse_timestamp_qualifier(state);
			return;
		}
		goto INVALID_COL_TYPE_NOTSUP;

	default:
		goto INVALID_COL_TYPE_NOTSUP;
	}

	/* long COL_TYPE */
	switch (*rem) {
	case 'i':
		if (strings_equal("nteger", rem + 1l)) {
			parse_integer_qualifier(state);
			return;
		}
		goto INVALID_COL_TYPE_NOTSUP;

	case 's':
		if (strings_equal("tring", rem + 1l)) {
			parse_string_qualifier(state);
			return;
		}
		goto INVALID_COL_TYPE_NOTSUP;

	case 't':
		if (strings_equal("imestamp", rem + 1l)) {
			parse_timestamp_qualifier(state);
			return;
		}
		goto INVALID_COL_TYPE_NOTSUP;

	default:
		goto INVALID_COL_TYPE_NOTSUP;
	}
}

inline void
parse_first_col_spec_safe(struct GenerateParseState *const restrict state)
{
	const bool matched_col_flag = col_flag_match(&state->argv);

	++(state->argv.arg.from);

	if (matched_col_flag) {
		struct ColSpec *const restrict col_spec
		= (struct ColSpec *const restrict) (state->specs.tbl + 1l);

		const bool valid_col_name = parse_col_name(&col_spec->name,
							   &state->argv);

		++(state->argv.arg.from);

		if (valid_col_name) {
			state->specs.tbl->col_specs.from = col_spec;

			state->specs.col = col_spec;

			parse_col_type(state);

		} else {
			generate_parse_error(state);
		}
	} else {
		generate_parse_error(state);
	}
}


inline void
parse_first_col_spec(struct GenerateParseState *const restrict state)
{
	const bool matched_col_flag = col_flag_match(&state->argv);

	++(state->argv.arg.from);

	if (!matched_col_flag) {
		generate_parse_error(state);
		return;
	}

	if (state->argv.arg.from == state->argv.arg.until) {
		incomplete_db_spec_col_name(&state->argv);
TERMINATE_VALID_EXIT_FAILURE:
		*(state->valid.last) = NULL;
		state->exit_status = EXIT_FAILURE;
		return;
	}

	struct ColSpec *const restrict col_spec
	= (struct ColSpec *const restrict) (state->specs.tbl + 1l);


	const bool valid_col_name = parse_col_name(&col_spec->name,
						   &state->argv);

	++(state->argv.arg.from);

	if (!valid_col_name) {
		generate_parse_error(state);
		return;
	}

	if (state->argv.arg.from == state->argv.arg.until) {
		no_col_type(&state->argv);
		goto TERMINATE_VALID_EXIT_FAILURE;
	}

	state->specs.col = col_spec;

	++(state->database.columns); /* set column counter */

	parse_col_type(state);
}


inline void
parse_next_col_spec(struct GenerateParseState *const restrict state)
{
	++(state->argv.arg.from);

	if (state->argv.arg.from == state->argv.arg.until) {
		incomplete_db_spec_col_name(&state->argv);
TERMINATE_VALID_EXIT_FAILURE:
		*(state->valid.last) = NULL;
		state->exit_status = EXIT_FAILURE;
		return;
	}

	++(state->specs.col);

	const bool valid_col_name
	= parse_col_name(&state->specs.col->name,
			 &state->argv);

	++(state->argv.arg.from);

	if (!valid_col_name) {
		generate_parse_error(state);
		return;
	}

	if (state->argv.arg.from == state->argv.arg.until) {
		no_col_type(&state->argv);
		goto TERMINATE_VALID_EXIT_FAILURE;
	}

	++(state->database.columns);

	parse_col_type(state);
}


/* TBL_SPEC */
inline void
parse_first_tbl_spec(struct GenerateParseState *const restrict state)
{
	const bool matched_tbl_flag = tbl_flag_match(&state->argv);

	++(state->argv.arg.from);

	if (matched_tbl_flag) {

		struct TblSpec *const restrict tbl_spec
		= (struct TblSpec *const restrict) (state->specs.db + 1l);

		const bool valid_tbl_name = parse_tbl_name(&tbl_spec->name,
							   &state->argv);

		++(state->argv.arg.from);

		if (valid_tbl_name) {

			const bool valid_row_count
			= parse_row_count(&tbl_spec->row_count,
					  &state->argv);

			++(state->argv.arg.from);

			if (valid_row_count) {
				state->specs.db->tbl_specs = tbl_spec;
				state->specs.tbl = tbl_spec;

				/* set initial counter values */
				state->database.ctor_flags = 0u;
				state->database.rows = tbl_spec->row_count;
				state->database.row_count_max
				= tbl_spec->row_count;
				state->database.tables = 1u;
				state->database.columns = 1u;
				state->database.counter_upto = 0lu;

				parse_first_col_spec_safe(state);
				return;
			}
		}
	}
	generate_parse_error(state);
}


inline void
parse_next_tbl_spec(struct GenerateParseState *const restrict state)
{
	++(state->argv.arg.from);

	if (state->argv.arg.from == state->argv.arg.until) {
		incomplete_db_spec_tbl_name(&state->argv);
TERMINATE_VALID_EXIT_FAILURE:
		*(state->valid.last) = NULL;
		state->exit_status = EXIT_FAILURE;
		return;
	}

	state->specs.tbl->next
	= (struct TblSpec *) state->specs.tbl->col_specs.until;

	state->specs.tbl = state->specs.tbl->next;

	const bool valid_tbl_name
	= parse_tbl_name(&state->specs.tbl->name,
			 &state->argv);

	++(state->argv.arg.from);

	if (!valid_tbl_name) {
		generate_parse_error(state);
		return;
	}

	if (state->argv.arg.from == state->argv.arg.until) {
		incomplete_db_spec_row_count(&state->argv);
		goto TERMINATE_VALID_EXIT_FAILURE;
	}

	const bool valid_row_count
	= parse_row_count(&state->specs.tbl->row_count,
			  &state->argv);

	++(state->argv.arg.from);

	if (!valid_row_count) {
		generate_parse_error(state);
		return;
	}

	if (state->argv.arg.from == state->argv.arg.until) {
		incomplete_db_spec_col_flag(&state->argv);
		goto TERMINATE_VALID_EXIT_FAILURE;
	}

	/* set counter values */
	state->database.rows += state->specs.tbl->row_count;

	if (state->specs.tbl->row_count > state->database.row_count_max)
		state->database.row_count_max = state->specs.tbl->row_count;

	state->database.tables += 1u;

	parse_first_col_spec(state);
}




/* DB_SPEC */
inline void
parse_first_db_spec(struct GenerateParseState *const restrict state)
{
	const bool valid_db_name = parse_db_name(&state->specs.db->name,
						 &state->argv);

	++(state->argv.arg.from);

	if (valid_db_name)
		parse_first_tbl_spec(state);
	else
		generate_parse_error(state);
}


inline void
parse_next_db_spec(struct GenerateParseState *const restrict state)
{
	if (state->argv.arg.from < state->argv.db_spec.until) {
		state->argv.db_spec.from = state->argv.arg.from;

		++(state->argv.arg.from);

		const bool valid_db_name
		= parse_db_name(&state->specs.db->name,
				&state->argv);

		++(state->argv.arg.from);

		if (valid_db_name)
			parse_first_tbl_spec(state);
		else
			generate_parse_error(state);

	} else {
		if (state->argv.arg.from < state->argv.arg.until)
			short_db_spec(&state->argv); /* error spec too short */

		*(state->valid.last) = NULL; /* terminate valid db_specs */

		state->exit_status = EXIT_FAILURE;
	}
}


inline void
generate_parse_error(struct GenerateParseState *const restrict state)
{
	state->exit_status = EXIT_FAILURE;

	/* find next DB_SPEC */
	state->argv.arg.from = flag_match_next(state->argv.arg.from,
					       state->argv.arg.until,
					       'd',
					       "database");

	parse_next_db_spec(state);
}


inline void
parse_db_specs(struct GenerateParseState *const restrict state)
{
	const bool matched_db_flag = db_flag_match(&state->argv);

	++(state->argv.arg.from);

	if (matched_db_flag)
		parse_first_db_spec(state);
	else
		generate_parse_error(state);
}


inline int
generate_dispatch(char *const restrict *const restrict arg,
		  const int rem_argc)
{
	if (rem_argc == 0lu) {
		generate_failure_no_db_spec();
		return EXIT_FAILURE;
	}
	/* calculate max required size for all components + their specs
	 *
	 *
	 * minimal valid (i.e. most memory dense) input for GENERATE MODE:
	 *
	 *                      ARGV - 9 args
	 * ┌──────────────────────────────────────────────────────┐
	 * │                     DB_SPEC - 8 args                 │
	 * │  ┌───────────────────────────────────────────────────┐
	 * │  │                        TBL_SPEC - 6 args          │
	 * │  │          ┌────────────────────────────────────────┐
	 * │  │          │                      COL_SPEC - 3 args │
	 * │  │          │                     ┌──────────────────┐
	 * │  │          │                     │                  │
	 * -g -D DB_NAME -T TBL_NAME ROW_COUNT -c COL_NAME COL_TYPE
	 *
	 * generates 1 database with 1 table with 1 column
	 *
	 *
	 * extreme valid cases (mac = argc - 1 for leading mode flag)
	 *
	 * (a) DB_SPEC count maximized, need memory for:
	 *                                      always 1 or 0
	 *	DbSpec  X DB_CNT  = mac / 8    ┌─────────────┐
	 *	TblSpec X TBL_CNT = DB_CNT  +  ((mac % 8) / 6)
	 *	ColSpec X COL_CNT = TBL_CNT + (((mac % 8) % 6) / 3)
	 *	                              └───────────────────┘
	 *	with                                always 0
	 *
	 *	((mac % 8) % 6) % 3
	 *
	 *	args leftover for storage-free COL_TYPE_Qs
	 *
	 *
	 * (b) TBL_SPEC count maximized, need memory for:
	 *
	 *	DbSpec  X DB_CNT  = 1
	 *	TblSpec X TBL_CNT = (mac - 2) / 6
	 *	ColSpec X COL_CNT = TBL_CNT + (((mac - 2) % 6) / 3)
	 *	                              └───────────────────┘
	 *	with                              always 1 or 0
	 *
	 *	((mac - 2) % 6) % 3
	 *
	 *	args leftover for COL_TYPE_Qs
	 *
	 *
	 * (c) COL_SPEC count maximized, need memory for:
	 *
	 *	DbSpec	X DB_CNT  = 1
	 *	TblSpec	X TBL_CNT = 1
	 *	ColSpec	X COL_CNT = (margc - 5) / 3
	 *
	 *	with
	 *
	 *	(margc - 5) % 3
	 *
	 *	args leftover for COL_TYPE_Qs
	 *
	 *
	 *						overhead
	 * struct ColSpec {
	 *	struct String name;			1 pointer + 1 size_t
	 *	union TypeQualifier			2 ldbls + 1 uint (max)
	 *	Procedure *build;			1 pointer
	 * };						────────────────
	 *						2 pointer + 1 size_t
	 *						+ 2 ldbls + 1 uint
	 *
	 * struct TblSpec {
	 *	struct String name;			1 pointer + 1 size_t
	 *	size_t row_count			1 size_t
	 *	struct ColSpecInterval col_specs;	2 pointers
	 *	struct TblSpec *next;			1 pointer
	 * };						────────────────
	 *						4 pointers + 2 size_t
	 *
	 * struct DbSpec {
	 *	struct String name;			1 pointer + 1 size_t
	 *	struct TblSpec *tbl_specs;		1 pointer
	 *	struct DbSpec *next;			1 pointer
	 * };						────────────────
	 *						3 pointers + 1 size_t
	 *
	 * worst case for allocing specs appears to be (c) */

	char *const restrict *const restrict arg_until = arg + rem_argc;
	char *const restrict *const restrict db_spec_until = arg_until
							   + 1l
							   - DB_SPEC_LENGTH_MIN;

	if (arg >= db_spec_until) {
		generate_failure_short_db_spec(arg,
					       arg_until);
		return EXIT_FAILURE;
	}

	struct DbSpec *const restrict
	spec_alloc = malloc(sizeof(struct DbSpec)
			    + sizeof(struct TblSpec)
			    + (sizeof(struct ColSpec)
			       * ((rem_argc - 5) / 3)));

	if (spec_alloc == NULL) {
		generate_failure_malloc();
		return EXIT_FAILURE;
	}

	/* initialize parsing state */
	struct GenerateParseState state;

	state.argv.arg.from	 = arg;
	state.argv.arg.until	 = arg_until;
	state.argv.db_spec.until = db_spec_until;

	state.specs.db = spec_alloc;

	state.generator.ctor_flags    = 0u;
	state.generator.rows	      = 0llu;
	state.generator.row_count_max = 0lu;
	state.generator.columns	      = 0u;
	state.generator.tables	      = 0u;
	state.generator.databases     = 0u;
	state.generator.counter_upto  = 0lu;

	state.valid.last  = &state.valid.head;

	state.exit_status = EXIT_SUCCESS;


	/* populate specs according to argv */
	parse_db_specs(&state);

	if (state.generator.databases == 0u) {
		generate_failure_no_valid_db_spec();
		free(spec_alloc);
		return EXIT_FAILURE;
	}

/* 	for (struct DbSpec *db_spec = state.valid.head; */
/* 	     db_spec != NULL; */
/* 	     db_spec = db_spec->next) { */

/* 		printf("db_name: %s\n", db_spec->name.bytes); */

/* 		for (struct TblSpec *tbl_spec = db_spec->tbl_specs; */
/* 		     tbl_spec != NULL; */
/* 		     tbl_spec = tbl_spec->next) { */
/* 			printf("\ttbl_name:  %s\n",  tbl_spec->name.bytes); */
/* 			printf("\trow_count: %zu\n", tbl_spec->row_count); */

/* 			for (struct ColSpec *col_spec = tbl_spec->col_specs.from; */
/* 			     col_spec < tbl_spec->col_specs.until; */
/* 			     ++col_spec) { */
/* 				printf("\t\tcol_name:  %s\n", col_spec->name.bytes); */
/* 			} */
/* 		} */
/* 	} */

/* 	printf("ctor_flags:    %u\n" */
/* 	       "rows:          %lu\n" */
/* 	       "row_count_max: %zu\n" */
/* 	       "columns:       %u\n" */
/* 	       "tables:        %u\n" */
/* 	       "databases:     %u\n" */
/* 	       "counter_upto:  %zu\n" */
/* 	       "exit_status:   EXIT_%s\n", */
/* 	       state.generator.ctor_flags, */
/* 	       state.generator.rows, */
/* 	       state.generator.row_count_max, */
/* 	       state.generator.columns, */
/* 	       state.generator.tables, */
/* 	       state.generator.databases, */
/* 	       state.generator.counter_upto, */
/* 	       state.exit_status == EXIT_SUCCESS ? "SUCCESS" : "FAILURE"); */

	mysql_seed_generate(&state.generator,
			    state.valid.head,
			    &state.exit_status);

	free(spec_alloc);

	return state.exit_status;
}

#endif /* ifndef MYSQL_SEED_MYSQL_SEED_GENERATE_H_ */
