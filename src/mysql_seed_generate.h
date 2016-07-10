#ifndef MYSQL_SEED_MYSQL_SEED_GENERATE_H_
#define MYSQL_SEED_MYSQL_SEED_GENERATE_H_

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include "mysql_seed_file.h"	/* handle input strings */


/* error messages
 *─────────────────────────────────────────────────────────────────────────── */
#define INVALID_DATABASE_FLAG_HEADER INVALID_FLAG_HEADER("DATABASE")
#define NO_DATABASE_FLAG_MESSAGE     NO_FLAG_MESSAGE("DATABASE")
#define INVALID_DB_NAME_HEADER	     INVALID_SPEC_HEADER("DB_NAME")
#define NO_DB_NAME_MESSAGE	     NO_SPEC_MESSAGE("DB_NAME")

/* print error messsage and return 'EXIT_FAILURE'
 *─────────────────────────────────────────────────────────────────────────── */
inline int
print_no_database_flag(void)
{
	write_muffle(STDERR_FILENO,
		     NO_DATABASE_FLAG_MESSAGE,
		     sizeof(NO_DATABASE_FLAG_MESSAGE));

	return EXIT_FAILURE;
}

inline int
print_invalid_database_flag(char *const restrict database_flag)
{
	char buffer[ERROR_BUFFER_SIZE] = {
		INVALID_DATABASE_FLAG_HEADER
	};

	char *restrict
	ptr = put_string_inspect(&buffer[sizeof(INVALID_DATABASE_FLAG_HEADER)],
				 database_flag,
				 FLAG_LENGTH_MAX);

	ptr = put_string_size(ptr,
			      MORE_INFO_MESSAGE,
			      sizeof(MORE_INFO_MESSAGE));

	write_muffle(STDERR_FILENO,
		     &buffer[0],
		     ptr - &buffer[0]);

	return EXIT_FAILURE;
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


/* dispatch generate mode according to 'arg_ptr'
 *─────────────────────────────────────────────────────────────────────────── */
inline int
generate_dispatch(char *const restrict *const restrict arg_ptr,
		  char *const restrict *const restrict until_ptr)
{
	if (arg_ptr == until_ptr)
		return print_no_database_flag();

	char *const restrict database_flag = *arg_ptr;

	if (*database_flag != '-')
		return print_invalid_database_flag(database_flag);

	char *const restrict next = database_flag + 1l;
	char *const restrict rem  = next + 1l;

	/* parse short database flag */
	switch (*next) {
	case '-': break;	/* parse long database flag */

	case 'd': return (*rem == '\0')
		       ? puts("TODO: dispatch databse")
		       : print_invalid_database_flag(database_flag);

	default:  return print_invalid_database_flag(database_flag);
	}

	/* parse long database flag */
	switch (*rem) {
	case 'd': return strings_equal("atabase", rem + 1l)
		       ? puts("TODO: dispatch databse")
		       : print_invalid_database_flag(database_flag);

	default:  return print_invalid_database_flag(database_flag);
	}
}

#endif /* ifndef MYSQL_SEED_MYSQL_SEED_GENERATE_H_ */
