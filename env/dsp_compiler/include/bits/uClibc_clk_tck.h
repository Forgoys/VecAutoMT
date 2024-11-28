/* Follow glibc's example and use 62500000 for CLK_TCK to implement sysconf and
 * clock.c instead of the normal default of 100.
 *
 * WARNING: It is assumed that this is a constant integer value usable in
 * preprocessor conditionals!!!
 */
/*freq/16/1000=500*10^6/16/1000=31250*/
#define __UCLIBC_CLK_TCK_CONST	31250l	
