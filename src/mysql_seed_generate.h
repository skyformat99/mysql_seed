#ifndef MYSQL_SEED_MYSQL_SEED_GENERATE_H_
#define MYSQL_SEED_MYSQL_SEED_GENERATE_H_

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include "generate/generator.h" /* file/exit/string/parallelization utils */

/* error messages
 *─────────────────────────────────────────────────────────────────────────── */

/* error messages
 *─────────────────────────────────────────────────────────────────────────── */
#define FAILURE_NO_DB_SPEC	FAILURE_NO_ARG("DB_SPEC")

#define FAILURE_DB_SPEC_SHORT_1	PARSE_FAILURE_INSPECT_1
#define FAILURE_DB_SPEC_SHORT_2	PARSE_FAILURE_INSPECT_2("DB_SPEC too short")

#define ERROR_DB_SPEC_SHORT_1	PARSE_ERROR_INSPECT_1
#define ERROR_DB_SPEC_SHORT_2	PARSE_ERROR_INSPECT_2("DB_SPEC too short")


/* typedefs, struct declarations
 *─────────────────────────────────────────────────────────────────────────── */
struct GenerateCounter {
	size_t databases;
	size_t tables;
	size_t columns;
};

struct GenerateScanState {
	bool db_spec;
	bool tbl_spec;
	bool col_spec;
	bool flag;
	bool complete;
};


/* GenerateCounter operations
 *─────────────────────────────────────────────────────────────────────────── */
inline void
generate_counter_reset(struct GenerateCounter *const restrict count)
{
	count->databases = 0lu;
	count->tables	 = 0lu;
	count->columns	 = 0lu;
}

/* GenerateScanState operations
 *─────────────────────────────────────────────────────────────────────────── */
inline void
generate_scan_state_reset(struct GenerateScanState *const restrict expecting)
{
	expecting->db_spec  = true;
	expecting->tbl_spec = false;
	expecting->col_spec = false;
	expecting->flag	    = false;
	expecting->complete = false;
}


/* print error messsage and return 'EXIT_FAILURE'
 *─────────────────────────────────────────────────────────────────────────── */
inline void
print_no_db_spec(void)
{
	write_muffle(STDERR_FILENO,
		     FAILURE_NO_DB_SPEC,
		     sizeof(FAILURE_NO_DB_SPEC) - 1lu);
}

inline void
print_invalid_db_spec(char *restrict *const restrict from,
		      char *restrict *const restrict until,
		      const char *const restrict reason,
		      const size_t length_reason)
{
	char buffer[ARGV_INSPECT_BUFFER_SIZE];

	const char *const restrict *const restrict until_max = from
							     + ARGC_INSPECT_MAX;
	if (until > until_max)
		until = until_max;

	char *restrict
	ptr = put_string_size(&buffer[0],
			      PARSE_ERROR_INSPECT_1,
			      sizeof(PARSE_ERROR_INSPECT_1) - 1lu);

	ptr = put_inspect_args(ptr,
			       from,
			       until);

	ptr = put_string_size(ptr,
			      reason,
			      length_reason);


	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);
}

/* Init File/DirHandles from input
 *─────────────────────────────────────────────────────────────────────────── */
/* inline void */
/* db_basic_handles_init(struct DirHandle *const restrict db_dir, */
/* 		      struct FileHandle *const restrict loader, */
/* 		      const char *const restrict db_name, */
/* 		      const struct HandlerClosure *const restrict fail_cl) */
/* { */
/* 	const char *restrict failure; */

/* 	size_t size; */

/* 	if (!validate_utf8_input_size(&size, */
/* 				      db_name, */
/* 				      DB_NAME_LENGTH_MAX, */
/* 				      &failure)) { */
/* 		fail_cl->handle(fail_cl->arg, */
/* 				failure); */
/* 		__builtin_unreachable(); */
/* 	} */

/* 	memory_copy(&db_dir->name.buffer[0], */
/* 		    db_name, */
/* 		    size); */

/* 	db_dir->name.length = size - 1lu; */

/* 	db_dir->path.length = db_dir->name.length + DB_DIRPATH_PFX_LENGTH; */

/* 	char *restrict ptr = &db_dir->path.buffer[0]; */

/* 	PUT_STRING_WIDTH(ptr, */
/* 			 DB_DIRPATH_PFX */
/* 			 DB_DIRPATH_PFX_NN_WIDTH); */

/* 	memory_copy(ptr, */
/* 		    db_name, */
/* 		    size); */
/* } */
/* inline void */
/* table_file_init(struct FileHandle *const restrict loader_file, */
/* 		 struct DirHandle *const restrict db_dir) */
/* { */
/* 	char *restrict ptr = &loader_file->name.buffer[0]; */

/* 	PUT_STRING_WIDTH(ptr, */
/* 			 LOADER_FILENAME_PFX, */
/* 			 LOADER_FILENAME_PFX_NN_WIDTH); */

/* 	ptr = put_string_size(ptr, */
/* 			      &db_dir->name.buffer[0], */
/* 			      db_dir->name.length); */

/* 	SET_STRING_WIDTH(ptr, */
/* 			 LOADER_FILENAME_SFX, */
/* 			 LOADER_FILENAME_SFX_WIDTH); */

/* 	loader_file->name.length = LOADER_FILENAME_PFX_LENGTH */
/* 				 + db_dir->name.length */
/* 				 + LOADER_FILENAME_SFX_LENGTH; */

/* 	const name_size = loader_file->name.length + 1lu; */

/* 	loader_file->path.length = name_size */
/* 				 + db_dir->path.length; */

/* 	ptr = put_string_size(&loader_file->path.buffer[0], */
/* 			      &db_dir->path.buffer[0], */
/* 			      db_dir->path.length); */

/* 	*ptr = PATH_DELIM; */
/* 	++ptr; */

/* 	memory_copy(ptr, */
/* 		    &loader_file->name.buffer[0], */
/* 		    name_size); */
/* } */


/* dispatch generate mode according to 'arg'
 *─────────────────────────────────────────────────────────────────────────── */
inline bool
generate_scan_specs(struct GenerateCounter *const restrict total_count,
		    char *restrict *const restrict from,
		    const char *const restrict *const restrict until)
{
	struct GenerateScanState expecting = {
		.db_spec  = true,
		.tbl_spec = false,
		.col_spec = false,
		.flag	  = false,
		.complete = false
	};

	struct GenerateCounter running_count = {
		.databases = 0lu,
		.tables	   = 0lu,
		.columns   = 0lu
	};

	bool valid_parse = false; /* at least 1 completely valid db_spec */

	char *restrict arg;
	char *restrict rem;

	while (from < until) {
		arg = *from;

		if (expecting.flag) {

			if (*arg != '-')

			switch (*arg) {
			case '-': break;	/* parse long mode flag */

			case 'd': return (*rem == '\0')
				       ? generate_dispatch(from + 1l, until)
				       : print_invalid_mode_flag(flag);

			case 't': return (*rem == '\0')
				       ? help_dispatch(from + 1l, until)
				       : print_invalid_mode_flag(flag);

			case 'c': return (*rem == '\0')
				       ? load_dispatch(from + 1l, until)
				       : print_invalid_mode_flag(flag);

			default:  return print_invalid_mode_flag(flag);
			}

		} else {
		}



	}
}


inline int
generate_dispatch(char *restrict *const restrict from,
		  const char *const restrict *const restrict until)
{
	const size_t length_interval = until - from;

	if (length_interval < DB_SPEC_LENGTH_MIN) {

		if (length_interval == 0lu)
			print_no_db_spec()
		else
			print_invalid_db_spec(from,
					      until,
					      DB_SPEC_TOO_SHORT,
					      sizeof(DB_SPEC_TOO_SHORT) - 1lu);

		return EXIT_FAILURE;
	}


	struct GenerateCounter count = {
		.databases = 0lu;
		.tables	   = 0lu;
		.columns   = 0lu;
	};


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
	 * -g -d DB_NAME -t TBL_NAME ROW_COUNT -c COL_NAME COL_TYPE
	 *
	 * generates 1 database with 1 table with 1 column
	 *
	 *
	 * extreme valid cases (mac = argc - 1 for leading mode flag)
	 *
	 * DB_SPEC count maximized, need memory for:
	 *                                                   always 1 or 0
	 *	Database and DbSpec  X DB_CNT  = mac / 8    ┌─────────────┐
	 *	Table	 and TblSpec X TBL_CNT = DB_CNT  +  ((mac % 8) / 6)
	 *	Column	 and ColSpec X COL_CNT = TBL_CNT + (((mac % 8) % 6) / 3)
	 *	                                           └───────────────────┘
	 *	with                                             always 0
	 *
	 *	((mac % 8) % 6) % 3
	 *
	 *	args leftover for storage-free COL_TYPE_Qs
	 *
	 *
	 * TBL_SPEC count maximized, need memory for:
	 *
	 *	Database and DbSpec  X DB_CNT  = 1
	 *	Table	 and TblSpec X TBL_CNT = (mac - 2) / 6
	 *	Column	 and ColSpec X COL_CNT = TBL_CNT + (((mac - 2) % 6) / 3)
	 *	                                           └───────────────────┘
	 *	with                                           always 1 or 0
	 *
	 *	((mac - 2) % 6) % 3
	 *
	 *	args leftover for COL_TYPE_Qs
	 *
	 *
	 * COL_SPEC count maximized, need memory for:
	 *
	 *	Database and DbSpec	X DB_CNT =  1
	 *	Table	 and TblSpec	X TBL_CNT = 1
	 *	Column	 and ColSpec	X COL_CNT = (margc - 5) / 3
	 *
	 *	with
	 *
	 *	(margc - 5) % 3
	 *
	 *	args leftover for COL_TYPE_Qs
	 *
	 *
	 * a malicious user maximizing storage/argc
	 * */

	const size_t spec_alloc = k

	generate_scan_specs(&count,
			    from,
			    until);


	return EXIT_SUCCESS;
}

#endif /* ifndef MYSQL_SEED_MYSQL_SEED_GENERATE_H_ */
