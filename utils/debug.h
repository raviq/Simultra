#ifndef _DEBUG_
#define _DEBUG_

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "%s[%s:%d]%s " M "\n", KBLU, __FILE__, __LINE__, KNRM, ##__VA_ARGS__)
#endif

#define alert(M, ...) fprintf(stderr, "%s[%s:%d]%s " M "\n", KMAG, __FILE__, __LINE__, KNRM, ##__VA_ARGS__)

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif


//    printf("%sred\n", KRED);
//    printf("%sgreen\n", KGRN);
//    printf("%syellow\n", KYEL);
//    printf("%sblue\n", KBLU);
//    printf("%smagenta\n", KMAG);
//    printf("%scyan\n", KCYN);
//    printf("%swhite\n", KWHT);
//    printf("%snormal\n", KNRM);
 
