#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H


/*
 * Module datatypes
 */
 
/*
 * Function prototypes
 */

/**
 * @param[in] argc
 * @param[in] argv
 * @return 0 for success or -1 for error
 */
int parse_args(int argc, char *argv[]);

/**
 * @return 1 or 0 if verbose or not verbose
 */
int parse_args_is_verbose();

#endif /* PARSE_ARGS_H */