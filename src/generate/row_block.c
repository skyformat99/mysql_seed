#include "generate/row_block.h"

extern inline char *
put_row_block_row(char *restrict ptr,
		  struct Rowspan *restrict from,
		  const struct Rowspan *const restrict until);

extern inline void
set_row_block_row(char *restrict ptr,
		  struct Rowspan *restrict from,
		  const struct Rowspan *const restrict until);

void
build_row_block(void *arg)
{
	struct RowBlock *const restrict row_block
	= (struct RowBlock *const restrict) arg;

	char *restrict ptr = row_block->contents;

	struct Rowspan *restrict row_span;

	for (size_t rem_rows = row_block->row_count; rem_rows > 1lu; --rem_rows)
		ptr = put_row_block_row(ptr,
					row_block->rowspans.from,
					row_block->rowspans.until);

	set_row_block_row(ptr,
			  row_block->row_spans.from,
			  row_block->row_spans.until);
}
