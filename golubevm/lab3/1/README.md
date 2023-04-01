# Lab3-1: Reverse directory and files
---
To build program use make
---
**NAME**\
		rvs - create reverse directory and files

**SYNOPSIS**\
		rvs SOURCE OUTPUT

**DESCRIPTION**\
		Creates directory at OUTPUT location, if it does not already exists. Name of directory is reverse name of SOURCE directory. Created directory contains copies of all regular files from SOURCE directory with reversed names and data.

**ERRORS**\
		NOT_ENOUGH_ARGUMENTS
 		CANNOT_OPEN_SPECIFIED_DIR
 		CANNOT_OPEN_OUTPUT_DIR
 		CANNOT_OPEN_REVERSE_DIR
 		CANNOT_CREATE_REVERSE_DIR
 		CANNOT_OPEN_FILE
 		CANNOT_CREATE_REVERSE_FILE
 		READ_ERROR
 		WRITE_ERROR
