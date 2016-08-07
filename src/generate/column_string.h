#ifndef MYSQL_SEED_GENERATE_COLUMN_STRING_H_
#define MYSQL_SEED_GENERATE_COLUMN_STRING_H_


/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "generate/counter.h"		  /* string/thread utils, Counter */
#include "generate/column_string_names.h" /* build_column_string_names_X */
#include "generate/column_string_uuid.h"  /* build_column_string_uuid */

/* worker thread entry point */
void
build_column_string_unique(void *arg);

/* worker thread entry point */
void
build_column_string_fixed(void *arg);

#endif /* ifndef MYSQL_SEED_GENERATE_COLUMN_STRING_H_ */
