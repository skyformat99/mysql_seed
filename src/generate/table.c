#include "generate/table.h"

#define BTH_MALLOC_FAILURE MALLOC_FAILURE_MESSAGE("build_table_header")

extern inline size_t
table_size_contents(const struct Table *const restrict table);

extern inline char *
table_put_header(char *restrict ptr,
		 const struct Table *const restrict table);

void
build_table_header(void *arg)
{
	struct Table *const restrict table
	= (struct Table *const restrict) arg;

	table->file.contents.length = table_size_contents(table);

	char *restrict ptr = NULL;

	thread_try_catch_open(free,
			      ptr);

	ptr = malloc(table->file.contents.length);

	if (ptr == NULL) {
		handler_closure_call(&table->fail_cl,
				     BTH_MALLOC_FAILURE);
		__builtin_unreachable();
	}

	table->file.contents.bytes = ptr;

	ptr = table_put_header(ptr,
			       table);

	const struct RowBlock *const restrict until = table->row_blocks.until;
	struct RowBlock *restrict from		    = table->row_blocks.from;

	while (1) {
		from->contents = ptr;

		++from;

		if (from == until)
			break;

		ptr += from->total.length;
	}

	thread_try_catch_close();
}


void
build_table_file(void *arg)
{
	struct Table *const restrict table
	= (struct Table *const restrict) arg;

	thread_try_ensure_open(free,
			       table->file.contents.bytes);

	file_handle_process(&table->file,
			    &table->fail_cl);

	thread_try_ensure_close();

	table->file.contents.bytes = NULL; /* no-op if freed again */
}
