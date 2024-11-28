#if !defined _FEATURES_H && !defined __need_uClibc_config_h
# error Never include <bits/uClibc_config.h> directly; use <features.h> instead
#endif

#define __UCLIBC_MAJOR__ 0
#define __UCLIBC_MINOR__ 9
#define __UCLIBC_SUBLEVEL__ 33
/* Automatically generated make config: don't edit */
/* Version: 0.9.33 */
/* Sat Jan  5 10:21:07 2019 */
#undef __TARGET_alpha__
#undef __TARGET_arm__
#undef __TARGET_avr32__
#undef __TARGET_bfin__
#undef __TARGET_cris__
#undef __TARGET_e1__
#undef __TARGET_frv__
#undef __TARGET_h8300__
#undef __TARGET_hppa__
#undef __TARGET_i386__
#undef __TARGET_i960__
#undef __TARGET_ia64__
#undef __TARGET_m68k__
#undef __TARGET_microblaze__
#undef __TARGET_mips__
#undef __TARGET_nios__
#undef __TARGET_nios2__
#undef __TARGET_powerpc__
#undef __TARGET_sh__
#undef __TARGET_sh64__
#undef __TARGET_sparc__
#undef __TARGET_v850__
#undef __TARGET_vax__
#undef __TARGET_x86_64__
#undef __TARGET_xtensa__
#define __TARGET_c6x__ 1

/* Target Architecture Features and Options */
#define __TARGET_ARCH__ "c6x"
#define __FORCE_OPTIONS_FOR_ARCH__ 1
#define __CONFIG_GENERIC_C6X__ 1
#undef __CONFIG_TMS320C64X__
#undef __CONFIG_TMS320C64XPLUS__
#define __TARGET_SUBARCH__ ""
#undef __UCLIBC_FORMAT_ELF__
#undef __UCLIBC_FORMAT_FDPIC_ELF__
#undef __UCLIBC_FORMAT_DSBT_ELF__
#define __UCLIBC_FORMAT_FLAT__ 1
#undef __UCLIBC_FORMAT_FLAT_SEP_DATA__
#undef __UCLIBC_FORMAT_SHARED_FLAT__
#define __ARCH_ANY_ENDIAN__ 1
#define __ARCH_LITTLE_ENDIAN__ 1
#undef __ARCH_WANTS_BIG_ENDIAN__
#define __ARCH_WANTS_LITTLE_ENDIAN__ 1
#define __ARCH_HAS_NO_MMU__ 1

/* Target CPU lacks a memory management unit (MMU) */
#define __UCLIBC_HAS_FLOATS__ 1
#define __UCLIBC_HAS_FPU__ 1
#define __DO_C99_MATH__ 1
#undef __DO_XSI_MATH__
#undef __UCLIBC_HAS_FENV__
#define __KERNEL_HEADERS__ "include"
#define __UCLIBC_UCLINUX_BROKEN_MUNMAP__ 1
#define __HAVE_DOT_CONFIG__ 1

/* General Library Settings */
#undef __DOPIC__
#define __ARCH_HAS_NO_SHARED__ 1
#define __ARCH_HAS_NO_LDSO__ 1
#define __UCLIBC_CTOR_DTOR__ 1
#define __HAS_NO_THREADS__ 1
#undef __LINUXTHREADS_OLD__
#undef __LINUXTHREADS_NEW__
#undef __UCLIBC_HAS_THREADS_NATIVE__
#undef __UCLIBC_HAS_LFS__
#define __MALLOC__ 1
#undef __MALLOC_SIMPLE__
#undef __MALLOC_STANDARD__
#undef __MALLOC_GLIBC_COMPAT__
#undef __UCLIBC_DYNAMIC_ATEXIT__
#define __COMPAT_ATEXIT__ 1
#undef __UCLIBC_SUSV3_LEGACY__
#undef __UCLIBC_SUSV3_LEGACY_MACROS__
#define __UCLIBC_SUSV4_LEGACY__ 1
#undef __UCLIBC_STRICT_HEADERS__
#undef __UCLIBC_HAS_STUBS__
#undef __UCLIBC_HAS_SHADOW__
#undef __UCLIBC_HAS_PROGRAM_INVOCATION_NAME__
#undef __UCLIBC_HAS_PTY__
#define __UCLIBC_HAS_GETPT__ 1
#undef __UCLIBC_HAS_TM_EXTENSIONS__
#undef __UCLIBC_HAS_TZ_CACHING__
#undef __UCLIBC_HAS_TZ_FILE__

/* Advanced Library Settings */
#define __UCLIBC_PWD_BUFFER_SIZE__ 256
#define __UCLIBC_GRP_BUFFER_SIZE__ 256

/* Support various families of functions */
#undef __UCLIBC_LINUX_MODULE_26__
#undef __UCLIBC_LINUX_SPECIFIC__
#undef __UCLIBC_HAS_GNU_ERROR__
#undef __UCLIBC_BSD_SPECIFIC__
#undef __UCLIBC_HAS_BSD_ERR__
#undef __UCLIBC_HAS_OBSOLETE_BSD_SIGNAL__
#undef __UCLIBC_HAS_OBSOLETE_SYSV_SIGNAL__
#undef __UCLIBC_NTP_LEGACY__
#undef __UCLIBC_SV4_DEPRECATED__
#define __UCLIBC_HAS_REALTIME__ 1
#define __UCLIBC_HAS_ADVANCED_REALTIME__ 1
#undef __UCLIBC_HAS_EPOLL__
#undef __UCLIBC_HAS_XATTR__
#undef __UCLIBC_HAS_PROFILING__
#undef __UCLIBC_HAS_CRYPT_IMPL__
#undef __UCLIBC_HAS_CRYPT_STUB__
#undef __UCLIBC_HAS_NETWORK_SUPPORT__

/* String and Stdio Support */
#undef __UCLIBC_HAS_STRING_GENERIC_OPT__
#undef __UCLIBC_HAS_STRING_ARCH_OPT__
#undef __UCLIBC_HAS_CTYPE_TABLES__
#define __UCLIBC_HAS_WCHAR__ 1
#undef __UCLIBC_HAS_LOCALE__
#undef __UCLIBC_HAS_GLIBC_CUSTOM_PRINTF__
#define __UCLIBC_PRINTF_SCANF_POSITIONAL_ARGS__ 9
#undef __UCLIBC_HAS_SCANF_GLIBC_A_FLAG__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_NONE__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_256__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_512__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_1024__
#undef __UCLIBC_HAS_STDIO_BUFSIZ_2048__
#define __UCLIBC_HAS_STDIO_BUFSIZ_4096__ 1
#undef __UCLIBC_HAS_STDIO_BUFSIZ_8192__
#define __UCLIBC_HAS_STDIO_BUILTIN_BUFFER_NONE__ 1
#undef __UCLIBC_HAS_STDIO_BUILTIN_BUFFER_4__
#undef __UCLIBC_HAS_STDIO_BUILTIN_BUFFER_8__
#undef __UCLIBC_HAS_STDIO_SHUTDOWN_ON_ABORT__
#undef __UCLIBC_HAS_STDIO_GETC_MACRO__
#undef __UCLIBC_HAS_STDIO_PUTC_MACRO__
#undef __UCLIBC_HAS_STDIO_AUTO_RW_TRANSITION__
#undef __UCLIBC_HAS_FOPEN_EXCLUSIVE_MODE__
#undef __UCLIBC_HAS_FOPEN_CLOSEEXEC_MODE__
#undef __UCLIBC_HAS_GLIBC_CUSTOM_STREAMS__
#undef __UCLIBC_HAS_PRINTF_M_SPEC__
#define __UCLIBC_HAS_ERRNO_MESSAGES__ 1
#undef __UCLIBC_HAS_SYS_ERRLIST__
#undef __UCLIBC_HAS_SIGNUM_MESSAGES__
#undef __UCLIBC_HAS_GNU_GETOPT__
#undef __UCLIBC_HAS_GETOPT_LONG__
#undef __UCLIBC_HAS_GNU_GETSUBOPT__

/* Big and Tall */
#undef __UCLIBC_HAS_REGEX__
#undef __UCLIBC_HAS_FNMATCH__
#undef __UCLIBC_HAS_NFTW__
#undef __UCLIBC_HAS_FTW__
#undef __UCLIBC_HAS_FTS__
#undef __UCLIBC_HAS_UTMPX__

/* Library Installation Options */
#define __RUNTIME_PREFIX__ "/home/work/M3000/mgcc/"
#define __DEVEL_PREFIX__ "/home/work/M3000/mgcc/"
#define __MULTILIB_DIR__ "lib"
#define __HARDWIRED_ABSPATH__ 1

/* Security options */
#undef __UCLIBC_HAS_ARC4RANDOM__
#undef __UCLIBC_HAS_SSP__
#define __UCLIBC_BUILD_NOEXECSTACK__ 1

/* Development/debugging options */
#define __CROSS_COMPILER_PREFIX__ "/home/work/M3000/mgcc/bin/tic6x-elf-"
#define __UCLIBC_EXTRA_CFLAGS__ "-O2"
#undef __DODEBUG__
#undef __DOSTRIP__
#undef __DOASSERTS__
#undef __UCLIBC_MALLOC_DEBUGGING__
#define __WARNINGS__ "-w"
#define __EXTRA_WARNINGS__ 1
#undef __DOMULTI__
#undef __UCLIBC_MJN3_ONLY__
