/* src/include/ngspice/config.h.  Generated from config.h.in by configure.  */
/* src/include/ngspice/config.h.in.  Generated from configure.ac by autoheader.  */

/* Support for Verilog-A models */
/* #undef ADMS */

/* Define if we want debug sensitivity analysis */
/* #undef ASDEBUG */

/* Define if we want to bypass cbd/cbs calculation for non varying vbs/vbd */
/* #undef CAPBYPASS */

/* The CIDER enhancements */
/* #undef CIDER */

/* Define to 1 if the `closedir' function returns void instead of `int'. */
/* #undef CLOSEDIR_VOID */

/* Spice cluster support */
/* #undef CLUSTER */

/* Define if you want to debug ngspice shell */
/* #undef CPDEBUG */

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */

/* Define if we want debug distortion analysis (BLOCKTIMES) */
/* #undef D_DBG_BLOCKTIMES */

/* Define if we want debug distortion analysis (SMALLTIMES) */
/* #undef D_DBG_SMALLTIMES */

/* Define if we want some experimental code */
/* #undef EXPERIMENTAL_CODE */

/* Define if we want to enable experimental devices */
/* #undef EXP_DEV */

/* Define if you want to debug frontend */
/* #undef FTEDEBUG */

/* ngspice.dll Windows */
/* #undef HAS_SHWIN */

/* Tcl Windows */
/* #undef HAS_TCLWIN */

/* define to specify MS Windows executable with simple GUI */
/* #undef HAS_WINGUI */

/* Define to 1 if you have the `access' function. */
#define HAVE_ACCESS 1

/* Define to 1 if you have the `acosh' function. */
#define HAVE_ACOSH 1

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the `asinh' function. */
#define HAVE_ASINH 1

/* Define to 1 if you have the `atanh' function. */
#define HAVE_ATANH 1

/* Define to 1 if you have the `bcopy' function. */
#define HAVE_BCOPY 1

/* Define to 1 if you have the <blt.h> header file. */
/* #undef HAVE_BLT_H */

/* Define to enable BSD editline */
/* #undef HAVE_BSDEDITLINE */

/* Define to 1 if you have the `bzero' function. */
#define HAVE_BZERO 1

/* Define to 1 if you have the <ctype.h> header file. */
#define HAVE_CTYPE_H 1

/* Define to 1 if you have the declaration of `isinf', and to 0 if you don't.
   */
#define HAVE_DECL_ISINF 1

/* Define to 1 if you have the declaration of `isnan', and to 0 if you don't.
   */
#define HAVE_DECL_ISNAN 1

/* Define to 1 if you have the declaration of `tzname', and to 0 if you don't.
   */
/* #undef HAVE_DECL_TZNAME */

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#define HAVE_DIRENT_H 1

/* Have dirname in libgen */
#define HAVE_DIRNAME 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* Define to 1 if you have the `dup2' function. */
#define HAVE_DUP2 1

/* Define to 1 if you have the <editline/readline.h> header file. */
/* #undef HAVE_EDITLINE_READLINE_H */

/* Define to 1 if you have the `endpwent' function. */
#define HAVE_ENDPWENT 1

/* Define to 1 if you have the `erfc' function. */
#define HAVE_ERFC 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the <fftw3.h> header file. */
/* #undef HAVE_FFTW3_H */

/* Define to 1 if you have the `finite' function. */
#define HAVE_FINITE 1

/* Define to 1 if you have the <float.h> header file. */
#define HAVE_FLOAT_H 1

/* Define to 1 if you have the `fork' function. */
#define HAVE_FORK 1

/* Define to 1 if you have the `ftime' function. */
#define HAVE_FTIME 1

/* Define to 1 if you have the `getcwd' function. */
#define HAVE_GETCWD 1

/* Define to 1 if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME 1

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Have fcn getopt_long() */
#define HAVE_GETOPT_LONG 1

/* Define to 1 if you have the `getrlimit' function. */
#define HAVE_GETRLIMIT 1

/* Define to 1 if you have the `getrusage' function. */
#define HAVE_GETRUSAGE 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `getwd' function. */
/* #undef HAVE_GETWD */

/* Define if we have GNU readline */
/* #undef HAVE_GNUREADLINE */

/* Define to 1 if you have the <ieeefp.h> header file. */
/* #undef HAVE_IEEEFP_H */

/* Define to 1 if you have the `index' function. */
/* #undef HAVE_INDEX */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `isatty' function. */
#define HAVE_ISATTY 1

/* Have fft routines in libfftw3 */
/* #undef HAVE_LIBFFTW3 */

/* Define if we want garbage collection enabled */
/* #undef HAVE_LIBGC */

/* Define to 1 if you have the <libintl.h> header file. */
/* #undef HAVE_LIBINTL_H */

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `pthread' library (-lpthread). */
/* #undef HAVE_LIBPTHREAD */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the `localtime' function. */
#define HAVE_LOCALTIME 1

/* Define to 1 if you have the `logb' function. */
#define HAVE_LOGB 1

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
   to 0 otherwise. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the <malloc.h> header file. */
/* #undef HAVE_MALLOC_H */

/* Define to 1 if you have the `memmove' function. */
/* #undef HAVE_MEMMOVE */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the `modf' function. */
/* #undef HAVE_MODF */

/* Define to 1 if you have the <ncurses/termcap.h> header file. */
/* #undef HAVE_NCURSES_TERMCAP_H */

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the `popen' function. */
#define HAVE_POPEN 1

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the `qsort' function. */
#define HAVE_QSORT 1

/* Define to 1 if you have the <readline/history.h> header file. */
/* #undef HAVE_READLINE_HISTORY_H */

/* Define to 1 if you have the <readline/readline.h> header file. */
/* #undef HAVE_READLINE_READLINE_H */

/* Define to 1 if your system has a GNU libc compatible `realloc' function,
   and to 0 otherwise. */
#define HAVE_REALLOC 1

/* Define to 1 if you have the `rindex' function. */
/* #undef HAVE_RINDEX */

/* Define to 1 if you have the `scalb' function. */
#define HAVE_SCALB 1

/* Define to 1 if you have the `scalbn' function. */
#define HAVE_SCALBN 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the <sgtty.h> header file. */
/* #undef HAVE_SGTTY_H */

/* Define to 1 if the system has the type `sighandler_t'. */
/* #undef HAVE_SIGHANDLER_T */

/* Define if we have sigsetjmp(). */
#define HAVE_SIGSETJMP /**/

/* Define to 1 if the system has the type `sig_t'. */
#define HAVE_SIG_T 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define to 1 if stdbool.h conforms to C99. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strncasecmp' function. */
#define HAVE_STRNCASECMP 1

/* Define to 1 if you have the <stropts.h> header file. */
/* #undef HAVE_STROPTS_H */

/* Define to 1 if you have the `strrchr' function. */
#define HAVE_STRRCHR 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the `strtol' function. */
#define HAVE_STRTOL 1

/* Define to 1 if `tm_zone' is a member of `struct tm'. */
#define HAVE_STRUCT_TM_TM_ZONE 1

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/file.h> header file. */
#define HAVE_SYS_FILE_H 1

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/io.h> header file. */
/* #undef HAVE_SYS_IO_H */

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/timeb.h> header file. */
#define HAVE_SYS_TIMEB_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have <sys/wait.h> that is POSIX.1 compatible. */
#define HAVE_SYS_WAIT_H 1

/* Define to 1 if you have the `tcgetattr' function. */
#define HAVE_TCGETATTR 1

/* Define to 1 if you have the <tcl.h> header file. */
/* #undef HAVE_TCL_H */

/* Define to 1 if you have the `tcsetattr' function. */
#define HAVE_TCSETATTR 1

/* Define if we have ncurses or termcap */
/* #undef HAVE_TERMCAP */

/* Define to 1 if you have the <termcap.h> header file. */
#define HAVE_TERMCAP_H 1

/* Define to 1 if you have the <termios.h> header file. */
#define HAVE_TERMIOS_H 1

/* Define to 1 if you have the <termio.h> header file. */
/* #undef HAVE_TERMIO_H */

/* Define to 1 if you have the `time' function. */
#define HAVE_TIME 1

/* Define to 1 if your `struct tm' has `tm_zone'. Deprecated, use
   `HAVE_STRUCT_TM_TM_ZONE' instead. */
#define HAVE_TM_ZONE 1

/* Define to 1 if you don't have `tm_zone' but do have the external array
   `tzname'. */
/* #undef HAVE_TZNAME */

/* Define to 1 if you have the `ulimit' function. */
/* #undef HAVE_ULIMIT */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `utimes' function. */
#define HAVE_UTIMES 1

/* Define to 1 if you have the <values.h> header file. */
/* #undef HAVE_VALUES_H */

/* Define to 1 if you have the `vfork' function. */
#define HAVE_VFORK 1

/* Define to 1 if you have the <vfork.h> header file. */
/* #undef HAVE_VFORK_H */

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if `fork' works. */
#define HAVE_WORKING_FORK 1

/* Define to 1 if `vfork' works. */
#define HAVE_WORKING_VFORK 1

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* Define to 1 if you have the </proc/meminfo> header file. */
/* #undef HAVE__PROC_MEMINFO */

/* Define to 1 if the system has the type `__sighandler_t'. */
/* #undef HAVE___SIGHANDLER_T */

/* Client-Server only via stdio. */
/* #undef IPC_DEBUG_VIA_STDIO */

/* Client-Server via socket. */
/* #undef IPC_UNIX_SOCKETS */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* The NDEV interface */
/* #undef NDEV */

/* Define if you want to discover :) */
/* #undef NEWPRED */

/* Do not trigger unwanted traps by default */
/* #undef NEWTRUNC */

/* Compile with debug info */
#define NGDEBUG 1

/* Define the directory for executables */
#define NGSPICEBINDIR "/usr/local/bin"

/* Define the build date */
#define NGSPICEBUILDDATE "Sat Sep 12 04:38:33 CST 2015"

/* Define the directory for architecture independent data files */
#define NGSPICEDATADIR "/usr/local/share/ngspice"

/* Define if we want NOBYPASS */
/* #undef NOBYPASS */

/* Experimental code never implemented to damp Newton iterations */
/* #undef NODELIMITING */

/* Name of package */
#define PACKAGE "ngspice"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://ngspice.sourceforge.net/bugrep.html"

/* Define to the full name of this package. */
#define PACKAGE_NAME "ngspice"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "ngspice 26"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "ngspice"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "26"

/* Define if we want predictor algorithm */
/* #undef PREDICTOR */

/* Define if you want to debug pole-zero analysis */
/* #undef PZDEBUG */

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Define to the type of arg 1 for `select'. */
#define SELECT_TYPE_ARG1 int

/* Define to the type of args 2, 3 and 4 for `select'. */
#define SELECT_TYPE_ARG234 (fd_set *)

/* Define to the type of arg 5 for `select'. */
#define SELECT_TYPE_ARG5 (struct timeval *)

/* Define if we want debug sensititvity analysis */
/* #undef SENSDEBUG */

/* shared ngspice module */
/* #undef SHARED_MODULE */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if the `S_IS*' macros in <sys/stat.h> do not work properly. */
/* #undef STAT_MACROS_BROKEN */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if we want stepdebug */
/* #undef STEPDEBUG */

/* Tcl Module */
/* #undef TCL_MODULE */

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* OpenMP parallel processing */
/* #undef USE_OMP */

/* Version number of package */
#define VERSION "26"

/* Define if we want spice2 sensitivity analysis */
/* #undef WANT_SENSE2 */

/* Define if you want PSS analysis */
/* #undef WITH_PSS */

/* The xspice enhancements */
/* #undef XSPICE */

//#define DEVLIB
#define Cocos2dGNU //使用cocos2d-x作作GNU界面

#define SIMULATOR

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */
#define X_DISPLAY_MISSING

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#define YYTEXT_POINTER 1

/* Define to rpl_malloc if the replacement function should be used. */
/* #undef malloc */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to rpl_realloc if the replacement function should be used. */
/* #undef realloc */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define as `fork' if `vfork' does not work. */
/* #undef vfork */
