#ifndef MYSQL_SEED_MYSQL_SEED_REMOVE
#define MYSQL_SEED_MYSQL_SEED_REMOVE

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include "mysql_seed_file.h"	/* handle input strings */


/* typedefs, struct declarations
 *─────────────────────────────────────────────────────────────────────────── */
#ifdef WIN32
struct Win32DirNode {
	char path[MAX_PATH];
	HANDLE handle;
	struct Win32DirNode *parent;
};

#define MYSQL_SEED_REMOVE_MALLOC_FAILURE				\
MALLOC_FAILURE_MESSAGE("mysql_seed_remove")

#endif /* ifdef WIN32 */

/* error messages
 *─────────────────────────────────────────────────────────────────────────── */
#define REMOVE_FAILURE(REASON)						\
"\n" FAILURE_HEADER_WRAP("remove", " - " REASON)

#define FAILURE_NO_RM_SPEC						\
REMOVE_FAILURE("no RM_SPEC provided") MORE_INFO_MESSAGE


/* irrecoverable failures */
inline void
remove_failure_no_rm_spec(void)
{
	write_muffle(STDERR_FILENO,
		     FAILURE_NO_RM_SPEC,
		     sizeof(FAILURE_NO_RM_SPEC) - 1lu);
}

#ifdef WIN32
inline void
mysql_seed_remove_malloc_failure(void)
{
	write_muffle(STDERR_FILENO,
		     MYSQL_SEED_REMOVE_MALLOC_FAILURE,
		     sizeof(MYSQL_SEED_REMOVE_MALLOC_FAILURE) - 1lu);
}

inline void
free_win32_dir_nodes(struct Win32DirNode *restrict dir_node)
{
	struct Win32DirNode *restrict closed_node;

	do {

		(void) FindClose(dir_node->handle);

		closed_node = dir_node;

		dir_node = dir_node->parent;

		free(closed_node);
	} while (dir_node != NULL);
}
#endif /* ifdef WIN32 */

/* remove all database directories in 'database' */
inline int
mysql_seed_remove_all(void)
{
	int exit_status;
	const char *restrict failure;

#ifdef WIN32
	WIN32_FIND_DATA file_info;
	HANDLE file_handle;
	DWORD error_code;
	struct Win32DirNode *dir_node;
	struct Win32DirNode *parent;

	/* ensure cwd at database root */
	if (!mysql_seed_chdir_db_root())
		return EXIT_FAILURE;

	exit_status = EXIT_SUCCESS;

	parent = NULL;

	while (1) {
		file_handle = FindFirstFile("*",
					    &file_info);

		if (file_handle == INVALID_HANDLE_VALUE) {
			PRINT_WIN32_FAILURE("FindFirstFile",
					    GetLastError());

			exit_status = EXIT_FAILURE;

RETURN_TO_PARENT_DIR:
			if (parent == NULL)
				return exit_status;

			if (!chdir_report("..",
					  &failure)) {
				print_failure(failure);
				free_win32_dir_nodes(parent);
				return EXIT_FAILURE;
			}

			file_handle = parent->handle;
			dir_node    = parent;
			parent      = dir_node->parent;

			if (!FindNextFile(file_handle,
					  &file_info)) {
				(void) FindClose(file_handle);

				error_code = GetLastError();

				if (error_code != ERROR_NO_MORE_FILES) {
					PRINT_WIN32_FAILURE("FindNextFile",
							    error_code);
					exit_status = EXIT_FAILURE;
				}

				if (!rmdir_report(&dir_node.path[0],
						  &failure)) {
					print_failure(failure);
					exit_status = EXIT_FAILURE;
				}

				free(dir_node);

				goto RETURN_TO_PARENT_DIR;
			}
		}

		while (is_dot_dir(&file_info.cFileName[0])) {

			if (!FindNextFile(file_handle,
					  &file_info)) {


			}


		}


		while (1) {



			if (  file_info.dwFileAttributes
			    & FILE_ATTRIBUTE_DIRECTORY) {
				dir_node = malloc(sizeof(struct Win32DirNode));

				if (dir_node == NULL) {
					mysql_seed_remove_malloc_failure();

					if (parent != NULL)
						free_win32_dir_nodes(parent);

					return EXIT_FAILURE;
				}

				dir_node->parent = parent;

			}

		}







	}




	if (file_handle == INVALID_HANDLE_VALUE)




#else
	FTS *restrict tree;
	FTSENT *restrict entry;

	/* ensure cwd at project root */
	if (!mysql_seed_chdir_root())
		return EXIT_FAILURE;

	exit_status = EXIT_SUCCESS;

	char *const path_argv[] = {
		DB_ROOT_DIRNAME, NULL
	};

	if (!fts_open_report(&tree,
			     &path_argv[0],
			     FTS_PHYSICAL | FTS_NOSTAT,
			     NULL,
			     &failure)) {
		print_failure(failure);
		return EXIT_FAILURE;
	}

	while (1) {
		if (!fts_read_report(&entry,
				     tree,
				     &failure)) {

			print_failure(failure);
			exit_status = EXIT_FAILURE;

			if (entry == NULL)
				break;
			else
				continue;
		}

		switch (entry->fts_info) {
		case FTS_D:	/* possibly populated directory, skip */
			continue;

		case FTS_DP:
			if (entry->fts_level == 0) /* returning to root */
				goto FTS_CLOSE_AND_RETURN;

			if (!rmdir_report(entry->fts_accpath,
					  &failure)) {
				print_failure(failure);
				exit_status = EXIT_FAILURE;
			}
			continue;

		default:	/* terminal file/link */
			if (!unlink_report(entry->fts_accpath,
					   &failure)) {
				print_failure(failure);
				exit_status = EXIT_FAILURE;
			}
		}
	}

FTS_CLOSE_AND_RETURN:
	if (!fts_close_report(tree,
			      &failure)) {
		print_failure(failure);
		return EXIT_FAILURE;
	}
#endif /* ifdef WIN32 */

	return exit_status;
}

/* remove all contents in a directory, then the directory itself */
inline int
mysql_seed_remove(char *const *db_names)
{
	const char *restrict failure;
	int exit_status;

	if (!mysql_seed_chdir_db_root())
		return EXIT_FAILURE;

	exit_status = EXIT_SUCCESS;

#ifdef WIN32

#else
	FTS *restrict tree;
	FTSENT *restrict entry;

	if (!fts_open_report(&tree,
			     db_names,
			     FTS_PHYSICAL | FTS_NOSTAT,
			     NULL,
			     &failure)) {
		print_failure(failure);
		return EXIT_FAILURE;
	}

	while (1) {
		if (!fts_read_report(&entry,
				     tree,
				     &failure)) {

			print_failure(failure);
			exit_status = EXIT_FAILURE;

			if (entry == NULL)
				break;
			else
				continue;
		}

		if (entry == NULL)
			break;	/* traversal complete */


		switch (entry->fts_info) {
		case FTS_D:	/* possibly populated directory, skip */
			continue;

		case FTS_DP:	/* empty directory, remove */
			if (!rmdir_report(entry->fts_accpath,
					  &failure)) {
				print_failure(failure);
				exit_status = EXIT_FAILURE;
			}
			continue;

		default:	/* terminal file/link */
			if (!unlink_report(entry->fts_accpath,
					   &failure)) {
				print_failure(failure);
				exit_status = EXIT_FAILURE;
			}
		}
	}

	if (!fts_close_report(tree,
			      &failure)) {
		print_failure(failure);
		return EXIT_FAILURE;
	}

	return exit_status;
#endif /* ifdef WIN32 */
}

inline int
remove_dispatch(char *const *const restrict arg)
{
	const char *restrict rm_spec = *arg;

	if (rm_spec == NULL) {
		remove_failure_no_rm_spec();
		return EXIT_FAILURE;
	}

	if (*rm_spec != '-')
		return mysql_seed_remove(arg);

	++rm_spec;

	const char *const restrict rem = rm_spec + 1l;

	switch (*rm_spec) {
	case '-':	/* parse long rm_spec */
		return strings_equal("all", rem)
		     ? mysql_seed_remove_all()
		     : mysql_seed_remove(arg);

	case 'a':
		return (*rem == '\0')
		     ? mysql_seed_remove_all()
		     : mysql_seed_remove(arg);

	default:
		return mysql_seed_remove(arg);
	}
}

#endif /* ifndef MYSQL_SEED_MYSQL_SEED_REMOVE */
