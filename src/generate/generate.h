#ifndef MYSQL_SEED_GENERATE_GENERATE_H_
#define MYSQL_SEED_GENERATE_GENERATE_H_

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include "generate/loader.h"		/* build_loader */
#include "generate/column_id.h"		/* build_column_id */
#include "generate/column_string.h"	/* build_column_string_base */
#include "generate/table.h"		/* build_table_header */

/* error messages
 *─────────────────────────────────────────────────────────────────────────── */
#define GENERATE_FAILURE(REASON)					\
"\n" FAILURE_HEADER_WRAP("generate", " - " REASON)

#define GENERATE_FAILURE_MALLOC						\
GENERATE_FAILURE(MALLOC_FAILURE_REASON)


/* typedefs, struct declarations
 *─────────────────────────────────────────────────────────────────────────── */
struct GeneratorCounter {
	uintmax_t rows;
	size_t row_count_max;
	unsigned int columns;
	unsigned int tables;
	unsigned int databases;
};

struct DatabaseCounter {
	uintmax_t rows;
	size_t row_count_max;
	unsigned int columns;
	unsigned int tables;
};


/* Generator/DatbaseCounter operations
 *─────────────────────────────────────────────────────────────────────────── */
inline void
generator_counter_update(struct GeneratorCounter *const restrict generator,
			 struct DatabaseCounter *const restrict database)
{
	generator->rows      += database->rows;

	if (database->row_count_max > generator->row_count_max)
		generator->row_count_max = database->row_count_max;

	generator->columns   += database->columns;
	generator->tables    += database->tables;
	generator->databases += 1u;
}


/* print error messsage
 *─────────────────────────────────────────────────────────────────────────── */
inline void
generate_failure_malloc(void)
{
	write_muffle(STDERR_FILENO,
		     GENERATE_FAILURE_MALLOC,
		     sizeof(GENERATE_FAILURE_MALLOC) - 1lu);
}


/* generate mode
 *─────────────────────────────────────────────────────────────────────────── */
inline void
mysql_seed_generate(const struct GeneratorCounter *const restrict count,
		    const struct DbSpec *restrict db_spec,
		    int *const restrict exit_status)
{
	const size_t row_block_row_count_max
	= (count->row_count_max < COUNT_WORKERS)
	? count->row_count_max
	: (count->row_count_max / COUNT_WORKERS);

	const size_t count_row_blocks_max = (count->rows
					     / row_block_row_count_max)
					  + count->columns;

	const size_t count_tasks_max
	= 1lu			/* build_counter */
	+ count->columns	/* build_column_X */
	+ count->databases	/* build_loader */
	+ (count->tables * 2lu) /* build_table_header + write_file */
	+ count_row_blocks_max; /* build_table_contents */

	const size_t count_rowspans_max = count_row_blocks_max
					* count->columns;

	struct Database *const restrict generator_alloc
	= malloc(  (sizeof(struct Database) * count->databases)
		 + (sizeof(struct Table)    * count->tables)
		 + (sizeof(struct Column)   * count->columns)
		 + (sizeof(struct Rowspan)  * count_rowspans_max)
		 + (sizeof(struct RowBlock) * count_row_blocks_max)
		 + (sizeof(struct TaskNode) * count_tasks_max));

	if (generator_alloc == NULL) {
		generate_failure_malloc();
		*exit_status = EXIT_FAILURE;
		return;
	}

	struct Generator generator;

	/* database_interval_init(&generator.databases, */
	/* 		       databases, */
	/* 		       databases + count->databases); */

	struct Table *const restrict tables
	= (struct Table *const restrict) (generator_alloc + count->databases);

	struct Column *const restrict columns
	= (struct Column *const restrict) (tables + count->tables);

	struct Rowspan *const restrict rowspans
	= (struct Rowspan *const restrict) (columns + count->columns);

	struct RowBlock *const restrict row_blocks
	= (struct RowBlock *const restrict) (rowspans + count_rowspans_max);

	struct TaskNode *const restrict task_nodes
	= (struct TaskNode *const restrict) (row_blocks + count_row_blocks_max);




	free(generator_alloc);
}

#endif /* ifndef MYSQL_SEED_GENERATE_GENERATE_H_ */
