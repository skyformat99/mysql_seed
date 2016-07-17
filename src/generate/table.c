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

	table_file_init(&table->file,
			&table->spec->name,
			&table->parent->dirpath);

	table->file.contents.length = table_size_contents(table);

	char *restrict ptr = NULL;

	thread_try_catch_open(free,
			      ptr);

	ptr = malloc(table->file.conents.length);

	if (ptr == NULL) {
		handler_closure_call(&column->fail_cl,
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
