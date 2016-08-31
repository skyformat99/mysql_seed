#mysql_seed

##Overview
`mysql_seed` is a command-line utility for generating large quantities of high quality tabular data intended to provision MySQL database servers. A single invokation can create multiple databases worth of data written to `.csv` files accompanied by `.mysql` scripts that will handle schema creation and data loading.


##Prerequisites
- gcc >= 2.5
- GNU Make


##Building
Invoking `make` at the project root will create the main executable binary, `bin/mysql_seed`, along with executable unit tests for all tested modules in the `test` directory.


##Usage
###`mysql_seed <MODE> [SPECS]`



##MODES
###`help`
`mysql_seed <-h, --help> [MODE]`  
prints operation details for mode `MODE` or general usage if not specified

**examples**  
`mysql_seed --help`  
will print help concerning general usage

`mysql_seed -h generate`  
will print help concerning `generate` mode

`mysql_seed --help e`  
will print help concerning `execute` mode



###`generate`
`mysql_seed <-g, --generate> <DB_SPEC_1> [DB_SPEC_2] ... [DB_SPEC_N]`  
generates seed files for `N` databases according to their `DB_SPEC` specification

**examples**  


###`execute`
`mysql_seed <-e, --execute> <DB_NAME> [MYSQL_ARGS]`  



###`remove`
`mysql_seed <-r, --remove> <-a, --all | DB_NAME_1> [DB_NAME_2] ... [DB_NAME_N]`  




##SPECS

###`DB_SPEC`
`<-d, --database> <DB_NAME> <TBL_SPEC_1> [TBL_SPEC_2] ... [TBL_SPEC_N]`  
indicates that the tables described by `TBL_SPEC`s `1` through `N` belong to the MySQL database named `DB_NAME`


###`TBL_SPEC`
`<-t, --table> <TBL_NAME> <ROW_COUNT> <COL_SPEC_1> [COL_SPEC_2] ... [COL_SPEC_N]`  
specifies a database table with name `TBL_NAME` having `ROW_COUNT` rows and `N` columns populated according to their respective `COL_SPEC`s"


###`COL_SPEC`
`<-c, --column> <COL_NAME> <COL_TYPE> [COL_TYPE_Q] [RAND_SPEC] [GRP_SPEC]`  
specifies a database column with name `COL_NAME` and data type `COL_TYPE`. A column type qualifier, `COL_TYPE_Q`,  may be provided to fine-tune data generation for a given type. For some `COL_TYPE`-`COL_TYPE_Q` combinations a `RAND_SPEC` may be provided to define limits on random data generation. For all non-fixed data specifications a group specification, `GRP_SPEC`, may be appended to partition a column into groups.



###`RAND_SPEC`
`<-r, --random> [<-f, --from> <MIN_TYPE> | <-u, --upto> <MAX_(TYPE)> | <-r, --range> <MIN_(TYPE)> <MAX_(TYPE)>]`  




###`GRP_SPEC`
`<-g, --group> <GRP_COUNT> [<-e, --even> | <-l, --linear>]`  


###Notation
- &lt;MANDATORY&gt;
- [OPTIONAL]
- THIS AND THAT | OR THESE
- HOMOGENOUS INPUT ... SEQUENCE
- -shrtflg, --equivalent-long-flag


##Supported Data Types

###String (VARCHAR, CHAR)
`<-s, --string>`

###Integer (TINYINT, SMALLINT, MEDIUMINT, INT, BIGINT)
`<-i, --integer>`

###Unsinged Integer (TINYINT UNSIGNED, SMALLINT UNSIGNED, MEDIUMINT UNSIGNED, INT UNSIGNED, BIGINT UNSIGNED)
`<-u, --unsigned-integer>`

###Datetime (DATETIME)
`<-dt, --datetime>`

###Timestamp (TIMESTAMP)
`<-ts, --timestamp>`


##Features
- robust thread pool implementation build with POSIX threads (pthread) API
- graceful error handling
  - recoverable errors are caught and reported
  - system call failures are logged and reported after cleanup
  - any generated files are deleted, and all dynamically allocated memory is freed on fatal errors before program termination


##Goals

###Short Term
- Linux Compatibility
- Windows Compatibility

###Long Term
- support for floating point types (FLOAT, DOUBLE)
- composable string types (i.e. printf-style formatting)
