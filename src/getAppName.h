#ifndef GET_APPNAME_H
#define GET_APPNAME_H

#ifndef APPNAME
#define APPNAME "INVALID APPNAME"
#endif  // APPNAME

/**
 * Length of APPNAME variable in the Makefile.
 */
#define APPNAME_LEN (sizeof(APPNAME) - 1)

/**
 * Maximum length of application name.
 */
#define MAX_APPNAME_LEN 64

/**
 * Handles the GET_APP_NAME instruction, which returns the application name
 *
 * @param[in,out] flags
 *   Set to IO_RETURN_AFTER_TX if successful
 *
 * @return zero or positive integer if success, negative integer otherwise.
 *
 */
int handleGetAppName();

#endif  // GET_APPNAME_H