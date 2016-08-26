#include "generate/generate.h"

/* global variables
 *─────────────────────────────────────────────────────────────────────────── */
GenerateConstructor *const GEN_CTOR_MAP[GEN_CTOR_MAP_LENGTH] = {
	[RAND_32_CTOR_FLAG]	    = &random_32_constructor,
	[RAND_64_CTOR_FLAG]	    = &random_64_constructor,
	[RAND_32_64_CTOR_FLAG]	    = &random_32_64_constructor,
	[RAND_32_UUID_CTOR_FLAG]    = &random_32_uuid_constructor,
	[RAND_32_64_UUID_CTOR_FLAG] = &random_32_64_uuid_constructor
};

/* Generator/DatbaseCounter operations
 *─────────────────────────────────────────────────────────────────────────── */
extern inline void
generator_counter_update(struct GeneratorCounter *const restrict generator,
			 struct DatabaseCounter *const restrict database);
extern inline void
database_counter_init(struct DatabaseCounter *const restrict database,
		      const size_t row_count);

/* print error messsage
 *─────────────────────────────────────────────────────────────────────────── */
extern inline void
generate_failure_constructor(const char *const restrict failure);
extern inline void
generate_failure_destructor(const char *const restrict failure);
extern inline void
generate_failure_malloc(void);
extern inline void
generate_failure_thread_pool(const char *const restrict failure);

/* destructors
 *─────────────────────────────────────────────────────────────────────────── */
extern inline void
free_columns(struct Column *restrict from,
	     const struct Column *const restrict until);
extern inline void
check_remove_loaders_dirs(struct Database *restrict from,
		    const struct Database *const restrict until);
extern inline void
remove_loaders_dirs(struct Database *restrict from,
		    const struct Database *const restrict until);
extern inline void
free_table_files(struct Table *restrict from,
		 const struct Table *const restrict until);
extern inline void
remove_free_table_files(struct Table *restrict from,
			const struct Table *const restrict until);

/* generate mode constructors, destructors
 *─────────────────────────────────────────────────────────────────────────── */
extern inline bool
mysql_seed_generate_constructors(const unsigned int ctor_flags);
extern inline void
mysql_seed_generate_destructors(int *const restrict exit_status);
extern inline void
mysql_seed_generate_destructors_muffle(void);

/* generate mode
 *─────────────────────────────────────────────────────────────────────────── */
extern inline void
mysql_seed_generate(const struct GeneratorCounter *const restrict count,
		    const struct DbSpec *restrict db_spec,
		    int *const restrict exit_status);
