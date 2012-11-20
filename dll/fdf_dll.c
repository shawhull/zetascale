/*
 * FDF linkage; this file was generated.
 * Copyright (c) 2012, SanDisk Corporation.  All rights reserved.
 */
#include <dlfcn.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "fdf.h"


/*
 * Macros.
 */
#define nel(a)      (sizeof(a)/sizeof(*(a)))
#define unlikely(x) __builtin_expect((x), 0)


/*
 * FDF Library locations.
 */
static char *fdflibs[] ={
    "/usr/lib64/fdf/libfdf.so",
    "/usr/lib/fdf/libfdf.so",
    "/lib64/libfdf.so",
    "/lib/libfdf.so",
    "/usr/local/lib64/libfdf.so",
    "/usr/local/lib/libfdf.so",
    "libfdf.so",
};


/*
 * Function pointers.
 */
static SDF_status_t 
(*ptr_SDFInit)(struct SDF_state **sdf_state, int argc, char **argv);

static struct SDF_thread_state *
(*ptr_SDFInitPerThreadState)(struct SDF_state *sdf_state);

static SDF_status_t 
(*ptr_SDFCreateContainer)(struct SDF_thread_state *sdf_thread_state,
                          char *cname,
                          SDF_container_props_t *properties,
                          SDF_cguid_t *cguid);

static SDF_status_t 
(*ptr_SDFOpenContainer)(struct SDF_thread_state *sdf_thread_state,
                        SDF_cguid_t cguid,
                        SDF_container_mode_t mode);

static SDF_status_t 
(*ptr_SDFCloseContainer)(struct SDF_thread_state *sdf_thread_state,
                         SDF_cguid_t cguid);

static SDF_status_t 
(*ptr_SDFDeleteContainer)(struct SDF_thread_state *sdf_thread_state,
                          SDF_cguid_t cguid);

static SDF_status_t 
(*ptr_SDFStartContainer)(struct SDF_thread_state *sdf_thread_state,
                         SDF_cguid_t cguid);

static SDF_status_t 
(*ptr_SDFStopContainer)(struct SDF_thread_state *sdf_thread_state,
                        SDF_cguid_t cguid);

static SDF_status_t 
(*ptr_SDFGetContainerProps)(struct SDF_thread_state *sdf_thread_state,
                            SDF_cguid_t cguid,
                            SDF_container_props_t *pprops);

static SDF_status_t 
(*ptr_SDFSetContainerProps)(struct SDF_thread_state *sdf_thread_state,
                            SDF_cguid_t cguid,
                            SDF_container_props_t *pprops);

static SDF_status_t 
(*ptr_SDFGetContainers)(struct SDF_thread_state *sdf_thread_state,
                        SDF_cguid_t *cguids,
                        uint32_t *n_cguids);

static SDF_status_t 
(*ptr_SDFFlushContainer)(struct SDF_thread_state *sdf_thread_state,
                         SDF_cguid_t cguid,
                         SDF_time_t current_time);

static SDF_status_t 
(*ptr_SDFGetForReadBufferedObject)(struct SDF_thread_state *sdf_thread_state,
                                   SDF_cguid_t cguid,
                                   char *key,
                                   uint32_t keylen,
                                   char **data,
                                   uint64_t *datalen,
                                   SDF_time_t current_time,
                                   SDF_time_t *expiry_time);

static SDF_status_t 
(*ptr_SDFFreeBuffer)(struct SDF_thread_state *sdf_thread_state, char *data);

static SDF_status_t 
(*ptr_SDFGetBuffer)(struct SDF_thread_state *sdf_thread_state,
                    char **data,
                    uint64_t datalen);

static SDF_status_t 
(*ptr_SDFCreateBufferedObject)(struct SDF_thread_state *sdf_thread_state,
                               SDF_cguid_t cguid,
                               char *key,
                               uint32_t keylen,
                               char *data,
                               uint64_t datalen,
                               SDF_time_t current_time,
                               SDF_time_t expiry_time);

static SDF_status_t 
(*ptr_SDFSetBufferedObject)(struct SDF_thread_state *sdf_thread_state,
                            SDF_cguid_t cguid,
                            char *key,
                            uint32_t keylen,
                            char *data,
                            uint64_t datalen,
                            SDF_time_t current_time,
                            SDF_time_t expiry_time);

static SDF_status_t 
(*ptr_SDFPutBufferedObject)(struct SDF_thread_state *sdf_thread_state,
                            SDF_cguid_t cguid,
                            char *key,
                            uint32_t keylen,
                            char *data,
                            uint64_t datalen,
                            SDF_time_t current_time,
                            SDF_time_t expiry_time);

static SDF_status_t 
(*ptr_SDFRemoveObjectWithExpiry)(struct SDF_thread_state *sdf_thread_state,
                                 SDF_cguid_t cguid,
                                 char *key,
                                 uint32_t keylen,
                                 SDF_time_t current_time);

static SDF_status_t 
(*ptr_SDFFlushObject)(struct SDF_thread_state *sdf_thread_state,
                      SDF_cguid_t cguid,
                      char *key,
                      uint32_t keylen,
                      SDF_time_t current_time);

static SDF_cguid_t 
(*ptr_SDFGenerateCguid)(struct SDF_thread_state *sdf_thread_state,
                        int64_t cntr_id64);

static SDF_status_t 
(*ptr_SDFFlushCache)(struct SDF_thread_state *sdf_thread_state,
                     SDF_time_t current_time);

static SDF_status_t 
(*ptr_SDFEnumerateContainerObjects)(struct SDF_thread_state *sdf_thread_state,
                                    SDF_cguid_t cguid,
                                    struct SDF_iterator **iterator);

static SDF_status_t 
(*ptr_SDFNextEnumeratedObject)(struct SDF_thread_state *sdf_thread_state,
                               struct SDF_iterator *iterator,
                               char **key,
                               uint32_t *keylen,
                               char **data,
                               uint64_t *datalen);

static SDF_status_t 
(*ptr_SDFFinishEnumeration)(struct SDF_thread_state *sdf_thread_state,
                            struct SDF_iterator *iterator);

static SDF_status_t 
(*ptr_SDFGetStats)(struct SDF_thread_state *sdf_thread_state,
                   SDF_stats_t *stats);

static SDF_status_t 
(*ptr_SDFGetContainerStats)(struct SDF_thread_state *sdf_thread_state,
                            SDF_cguid_t cguid,
                            SDF_container_stats_t *stats);

static SDF_status_t 
(*ptr_SDFBackupContainer)(struct SDF_thread_state *sdf_thread_state,
                          SDF_cguid_t cguid,
                          char *backup_directory);

static SDF_status_t 
(*ptr_SDFRestoreContainer)(struct SDF_thread_state *sdf_thread_state,
                           SDF_cguid_t cguid,
                           char *backup_directory);

static FDF_status_t 
(*ptr_FDFInit)(struct FDF_state **fdf_state);

static FDF_status_t 
(*ptr_FDFInitPerThreadState)(struct FDF_state *fdf_state,
                             struct FDF_thread_state **thd_state);

static FDF_status_t 
(*ptr_FDFReleasePerThreadState)(struct FDF_thread_state **thd_state);

static FDF_status_t 
(*ptr_FDFShutdown)(struct FDF_state *fdf_state);

static FDF_status_t 
(*ptr_FDFLoadCntrPropDefaults)(FDF_container_props_t *props);

static FDF_status_t 
(*ptr_FDFOpenContainer)(struct FDF_thread_state *fdf_thread_state,
                        char *cname,
                        FDF_container_props_t *properties,
                        uint32_t flags,
                        FDF_cguid_t *cguid);

static FDF_status_t 
(*ptr_FDFCloseContainer)(struct FDF_thread_state *fdf_thread_state,
                         FDF_cguid_t cguid);

static FDF_status_t 
(*ptr_FDFDeleteContainer)(struct FDF_thread_state *fdf_thread_state,
                          FDF_cguid_t cguid);

static FDF_status_t 
(*ptr_FDFGetContainers)(struct FDF_thread_state *fdf_thread_state,
                        FDF_cguid_t *cguids,
                        uint32_t *n_cguids);

static FDF_status_t 
(*ptr_FDFGetContainerProps)(struct FDF_thread_state *fdf_thread_state,
                            FDF_cguid_t cguid,
                            FDF_container_props_t *pprops);

static FDF_status_t 
(*ptr_FDFSetContainerProps)(struct FDF_thread_state *fdf_thread_state,
                            FDF_cguid_t cguid,
                            FDF_container_props_t *pprops);

static FDF_status_t 
(*ptr_FDFReadObject)(struct FDF_thread_state *fdf_thread_state,
                     FDF_cguid_t cguid,
                     char *key,
                     uint32_t keylen,
                     char **data,
                     uint64_t *datalen);

static FDF_status_t 
(*ptr_FDFFreeBuffer)(char *buf);

static FDF_status_t 
(*ptr_FDFWriteObject)(struct FDF_thread_state *sdf_thread_state,
                      FDF_cguid_t cguid,
                      char *key,
                      uint32_t keylen,
                      char *data,
                      uint64_t datalen,
                      uint32_t flags);

static FDF_status_t 
(*ptr_FDFDeleteObject)(struct FDF_thread_state *fdf_thread_state,
                       FDF_cguid_t cguid,
                       char *key,
                       uint32_t keylen);

static FDF_status_t 
(*ptr_FDFEnumerateContainerObjects)(struct FDF_thread_state *fdf_thread_state,
                                    FDF_cguid_t cguid,
                                    struct FDF_iterator **iterator);

static FDF_status_t 
(*ptr_FDFNextEnumeratedObject)(struct FDF_thread_state *fdf_thread_state,
                               struct FDF_iterator *iterator,
                               char **key,
                               uint32_t *keylen,
                               char **data,
                               uint64_t *datalen);

static FDF_status_t 
(*ptr_FDFFinishEnumeration)(struct FDF_thread_state *fdf_thread_state,
                            struct FDF_iterator *iterator);

static FDF_status_t 
(*ptr_FDFFlushObject)(struct FDF_thread_state *fdf_thread_state,
                      FDF_cguid_t cguid,
                      char *key,
                      uint32_t keylen);

static FDF_status_t 
(*ptr_FDFFlushContainer)(struct FDF_thread_state *fdf_thread_state,
                         FDF_cguid_t cguid);

static FDF_status_t 
(*ptr_FDFFlushCache)(struct FDF_thread_state *fdf_thread_state);

static FDF_status_t 
(*ptr_FDFGetStats)(struct FDF_thread_state *fdf_thread_state,
                   FDF_stats_t *stats);

static FDF_status_t 
(*ptr_FDFGetContainerStats)(struct FDF_thread_state *fdf_thread_state,
                            FDF_cguid_t cguid,
                            FDF_stats_t *stats);


/*
 * Linkage table.
 */
static struct {
    const char *name;
    void       *func;
} table[] ={
    { "SDFInit",                       &ptr_SDFInit                      },
    { "SDFInitPerThreadState",         &ptr_SDFInitPerThreadState        },
    { "SDFCreateContainer",            &ptr_SDFCreateContainer           },
    { "SDFOpenContainer",              &ptr_SDFOpenContainer             },
    { "SDFCloseContainer",             &ptr_SDFCloseContainer            },
    { "SDFDeleteContainer",            &ptr_SDFDeleteContainer           },
    { "SDFStartContainer",             &ptr_SDFStartContainer            },
    { "SDFStopContainer",              &ptr_SDFStopContainer             },
    { "SDFGetContainerProps",          &ptr_SDFGetContainerProps         },
    { "SDFSetContainerProps",          &ptr_SDFSetContainerProps         },
    { "SDFGetContainers",              &ptr_SDFGetContainers             },
    { "SDFFlushContainer",             &ptr_SDFFlushContainer            },
    { "SDFGetForReadBufferedObject",   &ptr_SDFGetForReadBufferedObject  },
    { "SDFFreeBuffer",                 &ptr_SDFFreeBuffer                },
    { "SDFGetBuffer",                  &ptr_SDFGetBuffer                 },
    { "SDFCreateBufferedObject",       &ptr_SDFCreateBufferedObject      },
    { "SDFSetBufferedObject",          &ptr_SDFSetBufferedObject         },
    { "SDFPutBufferedObject",          &ptr_SDFPutBufferedObject         },
    { "SDFRemoveObjectWithExpiry",     &ptr_SDFRemoveObjectWithExpiry    },
    { "SDFFlushObject",                &ptr_SDFFlushObject               },
    { "SDFGenerateCguid",              &ptr_SDFGenerateCguid             },
    { "SDFFlushCache",                 &ptr_SDFFlushCache                },
    { "SDFEnumerateContainerObjects",  &ptr_SDFEnumerateContainerObjects },
    { "SDFNextEnumeratedObject",       &ptr_SDFNextEnumeratedObject      },
    { "SDFFinishEnumeration",          &ptr_SDFFinishEnumeration         },
    { "SDFGetStats",                   &ptr_SDFGetStats                  },
    { "SDFGetContainerStats",          &ptr_SDFGetContainerStats         },
    { "SDFBackupContainer",            &ptr_SDFBackupContainer           },
    { "SDFRestoreContainer",           &ptr_SDFRestoreContainer          },
    { "FDFInit",                       &ptr_FDFInit                      },
    { "FDFInitPerThreadState",         &ptr_FDFInitPerThreadState        },
    { "FDFReleasePerThreadState",      &ptr_FDFReleasePerThreadState     },
    { "FDFShutdown",                   &ptr_FDFShutdown                  },
    { "FDFLoadCntrPropDefaults",       &ptr_FDFLoadCntrPropDefaults      },
    { "FDFOpenContainer",              &ptr_FDFOpenContainer             },
    { "FDFCloseContainer",             &ptr_FDFCloseContainer            },
    { "FDFDeleteContainer",            &ptr_FDFDeleteContainer           },
    { "FDFGetContainers",              &ptr_FDFGetContainers             },
    { "FDFGetContainerProps",          &ptr_FDFGetContainerProps         },
    { "FDFSetContainerProps",          &ptr_FDFSetContainerProps         },
    { "FDFReadObject",                 &ptr_FDFReadObject                },
    { "FDFFreeBuffer",                 &ptr_FDFFreeBuffer                },
    { "FDFWriteObject",                &ptr_FDFWriteObject               },
    { "FDFDeleteObject",               &ptr_FDFDeleteObject              },
    { "FDFEnumerateContainerObjects",  &ptr_FDFEnumerateContainerObjects },
    { "FDFNextEnumeratedObject",       &ptr_FDFNextEnumeratedObject      },
    { "FDFFinishEnumeration",          &ptr_FDFFinishEnumeration         },
    { "FDFFlushObject",                &ptr_FDFFlushObject               },
    { "FDFFlushContainer",             &ptr_FDFFlushContainer            },
    { "FDFFlushCache",                 &ptr_FDFFlushCache                },
    { "FDFGetStats",                   &ptr_FDFGetStats                  },
    { "FDFGetContainerStats",          &ptr_FDFGetContainerStats         },
};


/*
 * Print out an error message and exit.
 */
static void
panic(char *fmt, ...)
{
    va_list alist;

    va_start(alist, fmt);
    vfprintf(stderr, fmt, alist);
    va_end(alist);
    fprintf(stderr, "\n");
    exit(1);
}


/*
 * An undefined symbol was found.
 */
static void
undefined(char *sym)
{
    panic("FDF: undefined symbol: %s", sym);
}


/*
 * Determine if the string ends with "No such file or directory".
 */
static int
nsfod(char *str)
{
    char *err = "No such file or directory";
    int  elen = strlen(err);
    int  slen = strlen(str);

    if (slen < elen)
        return 0;
    return !strcmp(str+slen-elen, err);
}


/*
 * Load the FDF library.
 */
static int
load(char *path)
{
    int i;
    void  *dl = dlopen(path, RTLD_LAZY);
    char *err = dlerror();

    if (!dl) {
        if (nsfod(err))
            return 0;
        panic("%s", err);
    }
    
    int n = nel(table);
    for (i = 0; i < n; i++) {
        const char *name = table[i].name;
        void *func = dlsym(dl, name);
        if (func)
            *(void **)table[i].func = func;
        else
            fprintf(stderr, "warning: FDF: undefined symbol: %s\n", name);
    }
    return 1;
}


/*
 * Load the FDF library.
 */
static void
parse(void)
{
    int i;
    char *lib = getenv("FDF_LIB");

    if (lib) {
        if (load(lib))
            return;
        panic("cannot find FDF_LIB=%s", lib);
    }

    if (load("/usr/lib64/fdf/libfdf.so"))
        return;

    for (i = 0; i < nel(fdflibs); i++)
        if (load(fdflibs[i]))
            return;
    panic("cannot find libfdf.so");
}


/*
 * SDFInit
 */
SDF_status_t 
SDFInit(struct SDF_state **sdf_state, int argc, char **argv)
{
    parse();
    if (unlikely(!ptr_SDFInit))
        undefined("SDFInit");

    return (*ptr_SDFInit)(sdf_state, argc, argv);
}


/*
 * SDFInitPerThreadState
 */
struct SDF_thread_state *
SDFInitPerThreadState(struct SDF_state *sdf_state)
{
    if (unlikely(!ptr_SDFInitPerThreadState))
        undefined("SDFInitPerThreadState");

    return (*ptr_SDFInitPerThreadState)(sdf_state);
}


/*
 * SDFCreateContainer
 */
SDF_status_t 
SDFCreateContainer(struct SDF_thread_state *sdf_thread_state,
                   char *cname,
                   SDF_container_props_t *properties,
                   SDF_cguid_t *cguid)
{
    if (unlikely(!ptr_SDFCreateContainer))
        undefined("SDFCreateContainer");

    return (*ptr_SDFCreateContainer)(sdf_thread_state,
                                     cname,
                                     properties,
                                     cguid);
}


/*
 * SDFOpenContainer
 */
SDF_status_t 
SDFOpenContainer(struct SDF_thread_state *sdf_thread_state,
                 SDF_cguid_t cguid,
                 SDF_container_mode_t mode)
{
    if (unlikely(!ptr_SDFOpenContainer))
        undefined("SDFOpenContainer");

    return (*ptr_SDFOpenContainer)(sdf_thread_state, cguid, mode);
}


/*
 * SDFCloseContainer
 */
SDF_status_t 
SDFCloseContainer(struct SDF_thread_state *sdf_thread_state,
                  SDF_cguid_t cguid)
{
    if (unlikely(!ptr_SDFCloseContainer))
        undefined("SDFCloseContainer");

    return (*ptr_SDFCloseContainer)(sdf_thread_state, cguid);
}


/*
 * SDFDeleteContainer
 */
SDF_status_t 
SDFDeleteContainer(struct SDF_thread_state *sdf_thread_state,
                   SDF_cguid_t cguid)
{
    if (unlikely(!ptr_SDFDeleteContainer))
        undefined("SDFDeleteContainer");

    return (*ptr_SDFDeleteContainer)(sdf_thread_state, cguid);
}


/*
 * SDFStartContainer
 */
SDF_status_t 
SDFStartContainer(struct SDF_thread_state *sdf_thread_state,
                  SDF_cguid_t cguid)
{
    if (unlikely(!ptr_SDFStartContainer))
        undefined("SDFStartContainer");

    return (*ptr_SDFStartContainer)(sdf_thread_state, cguid);
}


/*
 * SDFStopContainer
 */
SDF_status_t 
SDFStopContainer(struct SDF_thread_state *sdf_thread_state, SDF_cguid_t cguid)
{
    if (unlikely(!ptr_SDFStopContainer))
        undefined("SDFStopContainer");

    return (*ptr_SDFStopContainer)(sdf_thread_state, cguid);
}


/*
 * SDFGetContainerProps
 */
SDF_status_t 
SDFGetContainerProps(struct SDF_thread_state *sdf_thread_state,
                     SDF_cguid_t cguid,
                     SDF_container_props_t *pprops)
{
    if (unlikely(!ptr_SDFGetContainerProps))
        undefined("SDFGetContainerProps");

    return (*ptr_SDFGetContainerProps)(sdf_thread_state, cguid, pprops);
}


/*
 * SDFSetContainerProps
 */
SDF_status_t 
SDFSetContainerProps(struct SDF_thread_state *sdf_thread_state,
                     SDF_cguid_t cguid,
                     SDF_container_props_t *pprops)
{
    if (unlikely(!ptr_SDFSetContainerProps))
        undefined("SDFSetContainerProps");

    return (*ptr_SDFSetContainerProps)(sdf_thread_state, cguid, pprops);
}


/*
 * SDFGetContainers
 */
SDF_status_t 
SDFGetContainers(struct SDF_thread_state *sdf_thread_state,
                 SDF_cguid_t *cguids,
                 uint32_t *n_cguids)
{
    if (unlikely(!ptr_SDFGetContainers))
        undefined("SDFGetContainers");

    return (*ptr_SDFGetContainers)(sdf_thread_state, cguids, n_cguids);
}


/*
 * SDFFlushContainer
 */
SDF_status_t 
SDFFlushContainer(struct SDF_thread_state *sdf_thread_state,
                  SDF_cguid_t cguid,
                  SDF_time_t current_time)
{
    if (unlikely(!ptr_SDFFlushContainer))
        undefined("SDFFlushContainer");

    return (*ptr_SDFFlushContainer)(sdf_thread_state, cguid, current_time);
}


/*
 * SDFGetForReadBufferedObject
 */
SDF_status_t 
SDFGetForReadBufferedObject(struct SDF_thread_state *sdf_thread_state,
                            SDF_cguid_t cguid,
                            char *key,
                            uint32_t keylen,
                            char **data,
                            uint64_t *datalen,
                            SDF_time_t current_time,
                            SDF_time_t *expiry_time)
{
    if (unlikely(!ptr_SDFGetForReadBufferedObject))
        undefined("SDFGetForReadBufferedObject");

    return (*ptr_SDFGetForReadBufferedObject)(sdf_thread_state,
                                              cguid,
                                              key,
                                              keylen,
                                              data,
                                              datalen,
                                              current_time,
                                              expiry_time);
}


/*
 * SDFFreeBuffer
 */
SDF_status_t 
SDFFreeBuffer(struct SDF_thread_state *sdf_thread_state, char *data)
{
    if (unlikely(!ptr_SDFFreeBuffer))
        undefined("SDFFreeBuffer");

    return (*ptr_SDFFreeBuffer)(sdf_thread_state, data);
}


/*
 * SDFGetBuffer
 */
SDF_status_t 
SDFGetBuffer(struct SDF_thread_state *sdf_thread_state,
             char **data,
             uint64_t datalen)
{
    if (unlikely(!ptr_SDFGetBuffer))
        undefined("SDFGetBuffer");

    return (*ptr_SDFGetBuffer)(sdf_thread_state, data, datalen);
}


/*
 * SDFCreateBufferedObject
 */
SDF_status_t 
SDFCreateBufferedObject(struct SDF_thread_state *sdf_thread_state,
                        SDF_cguid_t cguid,
                        char *key,
                        uint32_t keylen,
                        char *data,
                        uint64_t datalen,
                        SDF_time_t current_time,
                        SDF_time_t expiry_time)
{
    if (unlikely(!ptr_SDFCreateBufferedObject))
        undefined("SDFCreateBufferedObject");

    return (*ptr_SDFCreateBufferedObject)(sdf_thread_state,
                                          cguid,
                                          key,
                                          keylen,
                                          data,
                                          datalen,
                                          current_time,
                                          expiry_time);
}


/*
 * SDFSetBufferedObject
 */
SDF_status_t 
SDFSetBufferedObject(struct SDF_thread_state *sdf_thread_state,
                     SDF_cguid_t cguid,
                     char *key,
                     uint32_t keylen,
                     char *data,
                     uint64_t datalen,
                     SDF_time_t current_time,
                     SDF_time_t expiry_time)
{
    if (unlikely(!ptr_SDFSetBufferedObject))
        undefined("SDFSetBufferedObject");

    return (*ptr_SDFSetBufferedObject)(sdf_thread_state,
                                       cguid,
                                       key,
                                       keylen,
                                       data,
                                       datalen,
                                       current_time,
                                       expiry_time);
}


/*
 * SDFPutBufferedObject
 */
SDF_status_t 
SDFPutBufferedObject(struct SDF_thread_state *sdf_thread_state,
                     SDF_cguid_t cguid,
                     char *key,
                     uint32_t keylen,
                     char *data,
                     uint64_t datalen,
                     SDF_time_t current_time,
                     SDF_time_t expiry_time)
{
    if (unlikely(!ptr_SDFPutBufferedObject))
        undefined("SDFPutBufferedObject");

    return (*ptr_SDFPutBufferedObject)(sdf_thread_state,
                                       cguid,
                                       key,
                                       keylen,
                                       data,
                                       datalen,
                                       current_time,
                                       expiry_time);
}


/*
 * SDFRemoveObjectWithExpiry
 */
SDF_status_t 
SDFRemoveObjectWithExpiry(struct SDF_thread_state *sdf_thread_state,
                          SDF_cguid_t cguid,
                          char *key,
                          uint32_t keylen,
                          SDF_time_t current_time)
{
    if (unlikely(!ptr_SDFRemoveObjectWithExpiry))
        undefined("SDFRemoveObjectWithExpiry");

    return (*ptr_SDFRemoveObjectWithExpiry)(sdf_thread_state,
                                            cguid,
                                            key,
                                            keylen,
                                            current_time);
}


/*
 * SDFFlushObject
 */
SDF_status_t 
SDFFlushObject(struct SDF_thread_state *sdf_thread_state,
               SDF_cguid_t cguid,
               char *key,
               uint32_t keylen,
               SDF_time_t current_time)
{
    if (unlikely(!ptr_SDFFlushObject))
        undefined("SDFFlushObject");

    return (*ptr_SDFFlushObject)(sdf_thread_state,
                                 cguid,
                                 key,
                                 keylen,
                                 current_time);
}


/*
 * SDFGenerateCguid
 */
SDF_cguid_t 
SDFGenerateCguid(struct SDF_thread_state *sdf_thread_state, int64_t cntr_id64)
{
    if (unlikely(!ptr_SDFGenerateCguid))
        undefined("SDFGenerateCguid");

    return (*ptr_SDFGenerateCguid)(sdf_thread_state, cntr_id64);
}


/*
 * SDFFlushCache
 */
SDF_status_t 
SDFFlushCache(struct SDF_thread_state *sdf_thread_state,
              SDF_time_t current_time)
{
    if (unlikely(!ptr_SDFFlushCache))
        undefined("SDFFlushCache");

    return (*ptr_SDFFlushCache)(sdf_thread_state, current_time);
}


/*
 * SDFEnumerateContainerObjects
 */
SDF_status_t 
SDFEnumerateContainerObjects(struct SDF_thread_state *sdf_thread_state,
                             SDF_cguid_t cguid,
                             struct SDF_iterator **iterator)
{
    if (unlikely(!ptr_SDFEnumerateContainerObjects))
        undefined("SDFEnumerateContainerObjects");

    return (*ptr_SDFEnumerateContainerObjects)(sdf_thread_state,
                                               cguid,
                                               iterator);
}


/*
 * SDFNextEnumeratedObject
 */
SDF_status_t 
SDFNextEnumeratedObject(struct SDF_thread_state *sdf_thread_state,
                        struct SDF_iterator *iterator,
                        char **key,
                        uint32_t *keylen,
                        char **data,
                        uint64_t *datalen)
{
    if (unlikely(!ptr_SDFNextEnumeratedObject))
        undefined("SDFNextEnumeratedObject");

    return (*ptr_SDFNextEnumeratedObject)(sdf_thread_state,
                                          iterator,
                                          key,
                                          keylen,
                                          data,
                                          datalen);
}


/*
 * SDFFinishEnumeration
 */
SDF_status_t 
SDFFinishEnumeration(struct SDF_thread_state *sdf_thread_state,
                     struct SDF_iterator *iterator)
{
    if (unlikely(!ptr_SDFFinishEnumeration))
        undefined("SDFFinishEnumeration");

    return (*ptr_SDFFinishEnumeration)(sdf_thread_state, iterator);
}


/*
 * SDFGetStats
 */
SDF_status_t 
SDFGetStats(struct SDF_thread_state *sdf_thread_state, SDF_stats_t *stats)
{
    if (unlikely(!ptr_SDFGetStats))
        undefined("SDFGetStats");

    return (*ptr_SDFGetStats)(sdf_thread_state, stats);
}


/*
 * SDFGetContainerStats
 */
SDF_status_t 
SDFGetContainerStats(struct SDF_thread_state *sdf_thread_state,
                     SDF_cguid_t cguid,
                     SDF_container_stats_t *stats)
{
    if (unlikely(!ptr_SDFGetContainerStats))
        undefined("SDFGetContainerStats");

    return (*ptr_SDFGetContainerStats)(sdf_thread_state, cguid, stats);
}


/*
 * SDFBackupContainer
 */
SDF_status_t 
SDFBackupContainer(struct SDF_thread_state *sdf_thread_state,
                   SDF_cguid_t cguid,
                   char *backup_directory)
{
    if (unlikely(!ptr_SDFBackupContainer))
        undefined("SDFBackupContainer");

    return (*ptr_SDFBackupContainer)(sdf_thread_state,
                                     cguid,
                                     backup_directory);
}


/*
 * SDFRestoreContainer
 */
SDF_status_t 
SDFRestoreContainer(struct SDF_thread_state *sdf_thread_state,
                    SDF_cguid_t cguid,
                    char *backup_directory)
{
    if (unlikely(!ptr_SDFRestoreContainer))
        undefined("SDFRestoreContainer");

    return (*ptr_SDFRestoreContainer)(sdf_thread_state,
                                      cguid,
                                      backup_directory);
}


/*
 * FDFInit
 */
FDF_status_t 
FDFInit(struct FDF_state **fdf_state)
{
    parse();
    if (unlikely(!ptr_FDFInit))
        undefined("FDFInit");

    return (*ptr_FDFInit)(fdf_state);
}


/*
 * FDFInitPerThreadState
 */
FDF_status_t 
FDFInitPerThreadState(struct FDF_state *fdf_state,
                      struct FDF_thread_state **thd_state)
{
    if (unlikely(!ptr_FDFInitPerThreadState))
        undefined("FDFInitPerThreadState");

    return (*ptr_FDFInitPerThreadState)(fdf_state, thd_state);
}


/*
 * FDFReleasePerThreadState
 */
FDF_status_t 
FDFReleasePerThreadState(struct FDF_thread_state **thd_state)
{
    if (unlikely(!ptr_FDFReleasePerThreadState))
        undefined("FDFReleasePerThreadState");

    return (*ptr_FDFReleasePerThreadState)(thd_state);
}


/*
 * FDFShutdown
 */
FDF_status_t 
FDFShutdown(struct FDF_state *fdf_state)
{
    if (unlikely(!ptr_FDFShutdown))
        undefined("FDFShutdown");

    return (*ptr_FDFShutdown)(fdf_state);
}


/*
 * FDFLoadCntrPropDefaults
 */
FDF_status_t 
FDFLoadCntrPropDefaults(FDF_container_props_t *props)
{
    if (unlikely(!ptr_FDFLoadCntrPropDefaults))
        undefined("FDFLoadCntrPropDefaults");

    return (*ptr_FDFLoadCntrPropDefaults)(props);
}


/*
 * FDFOpenContainer
 */
FDF_status_t 
FDFOpenContainer(struct FDF_thread_state *fdf_thread_state,
                 char *cname,
                 FDF_container_props_t *properties,
                 uint32_t flags,
                 FDF_cguid_t *cguid)
{
    if (unlikely(!ptr_FDFOpenContainer))
        undefined("FDFOpenContainer");

    return (*ptr_FDFOpenContainer)(fdf_thread_state,
                                   cname,
                                   properties,
                                   flags,
                                   cguid);
}


/*
 * FDFCloseContainer
 */
FDF_status_t 
FDFCloseContainer(struct FDF_thread_state *fdf_thread_state,
                  FDF_cguid_t cguid)
{
    if (unlikely(!ptr_FDFCloseContainer))
        undefined("FDFCloseContainer");

    return (*ptr_FDFCloseContainer)(fdf_thread_state, cguid);
}


/*
 * FDFDeleteContainer
 */
FDF_status_t 
FDFDeleteContainer(struct FDF_thread_state *fdf_thread_state,
                   FDF_cguid_t cguid)
{
    if (unlikely(!ptr_FDFDeleteContainer))
        undefined("FDFDeleteContainer");

    return (*ptr_FDFDeleteContainer)(fdf_thread_state, cguid);
}


/*
 * FDFGetContainers
 */
FDF_status_t 
FDFGetContainers(struct FDF_thread_state *fdf_thread_state,
                 FDF_cguid_t *cguids,
                 uint32_t *n_cguids)
{
    if (unlikely(!ptr_FDFGetContainers))
        undefined("FDFGetContainers");

    return (*ptr_FDFGetContainers)(fdf_thread_state, cguids, n_cguids);
}


/*
 * FDFGetContainerProps
 */
FDF_status_t 
FDFGetContainerProps(struct FDF_thread_state *fdf_thread_state,
                     FDF_cguid_t cguid,
                     FDF_container_props_t *pprops)
{
    if (unlikely(!ptr_FDFGetContainerProps))
        undefined("FDFGetContainerProps");

    return (*ptr_FDFGetContainerProps)(fdf_thread_state, cguid, pprops);
}


/*
 * FDFSetContainerProps
 */
FDF_status_t 
FDFSetContainerProps(struct FDF_thread_state *fdf_thread_state,
                     FDF_cguid_t cguid,
                     FDF_container_props_t *pprops)
{
    if (unlikely(!ptr_FDFSetContainerProps))
        undefined("FDFSetContainerProps");

    return (*ptr_FDFSetContainerProps)(fdf_thread_state, cguid, pprops);
}


/*
 * FDFReadObject
 */
FDF_status_t 
FDFReadObject(struct FDF_thread_state *fdf_thread_state,
              FDF_cguid_t cguid,
              char *key,
              uint32_t keylen,
              char **data,
              uint64_t *datalen)
{
    if (unlikely(!ptr_FDFReadObject))
        undefined("FDFReadObject");

    return (*ptr_FDFReadObject)(fdf_thread_state,
                                cguid,
                                key,
                                keylen,
                                data,
                                datalen);
}


/*
 * FDFFreeBuffer
 */
FDF_status_t 
FDFFreeBuffer(char *buf)
{
    if (unlikely(!ptr_FDFFreeBuffer))
        undefined("FDFFreeBuffer");

    return (*ptr_FDFFreeBuffer)(buf);
}


/*
 * FDFWriteObject
 */
FDF_status_t 
FDFWriteObject(struct FDF_thread_state *sdf_thread_state,
               FDF_cguid_t cguid,
               char *key,
               uint32_t keylen,
               char *data,
               uint64_t datalen,
               uint32_t flags)
{
    if (unlikely(!ptr_FDFWriteObject))
        undefined("FDFWriteObject");

    return (*ptr_FDFWriteObject)(sdf_thread_state,
                                 cguid,
                                 key,
                                 keylen,
                                 data,
                                 datalen,
                                 flags);
}


/*
 * FDFDeleteObject
 */
FDF_status_t 
FDFDeleteObject(struct FDF_thread_state *fdf_thread_state,
                FDF_cguid_t cguid,
                char *key,
                uint32_t keylen)
{
    if (unlikely(!ptr_FDFDeleteObject))
        undefined("FDFDeleteObject");

    return (*ptr_FDFDeleteObject)(fdf_thread_state, cguid, key, keylen);
}


/*
 * FDFEnumerateContainerObjects
 */
FDF_status_t 
FDFEnumerateContainerObjects(struct FDF_thread_state *fdf_thread_state,
                             FDF_cguid_t cguid,
                             struct FDF_iterator **iterator)
{
    if (unlikely(!ptr_FDFEnumerateContainerObjects))
        undefined("FDFEnumerateContainerObjects");

    return (*ptr_FDFEnumerateContainerObjects)(fdf_thread_state,
                                               cguid,
                                               iterator);
}


/*
 * FDFNextEnumeratedObject
 */
FDF_status_t 
FDFNextEnumeratedObject(struct FDF_thread_state *fdf_thread_state,
                        struct FDF_iterator *iterator,
                        char **key,
                        uint32_t *keylen,
                        char **data,
                        uint64_t *datalen)
{
    if (unlikely(!ptr_FDFNextEnumeratedObject))
        undefined("FDFNextEnumeratedObject");

    return (*ptr_FDFNextEnumeratedObject)(fdf_thread_state,
                                          iterator,
                                          key,
                                          keylen,
                                          data,
                                          datalen);
}


/*
 * FDFFinishEnumeration
 */
FDF_status_t 
FDFFinishEnumeration(struct FDF_thread_state *fdf_thread_state,
                     struct FDF_iterator *iterator)
{
    if (unlikely(!ptr_FDFFinishEnumeration))
        undefined("FDFFinishEnumeration");

    return (*ptr_FDFFinishEnumeration)(fdf_thread_state, iterator);
}


/*
 * FDFFlushObject
 */
FDF_status_t 
FDFFlushObject(struct FDF_thread_state *fdf_thread_state,
               FDF_cguid_t cguid,
               char *key,
               uint32_t keylen)
{
    if (unlikely(!ptr_FDFFlushObject))
        undefined("FDFFlushObject");

    return (*ptr_FDFFlushObject)(fdf_thread_state, cguid, key, keylen);
}


/*
 * FDFFlushContainer
 */
FDF_status_t 
FDFFlushContainer(struct FDF_thread_state *fdf_thread_state,
                  FDF_cguid_t cguid)
{
    if (unlikely(!ptr_FDFFlushContainer))
        undefined("FDFFlushContainer");

    return (*ptr_FDFFlushContainer)(fdf_thread_state, cguid);
}


/*
 * FDFFlushCache
 */
FDF_status_t 
FDFFlushCache(struct FDF_thread_state *fdf_thread_state)
{
    if (unlikely(!ptr_FDFFlushCache))
        undefined("FDFFlushCache");

    return (*ptr_FDFFlushCache)(fdf_thread_state);
}


/*
 * FDFGetStats
 */
FDF_status_t 
FDFGetStats(struct FDF_thread_state *fdf_thread_state, FDF_stats_t *stats)
{
    if (unlikely(!ptr_FDFGetStats))
        undefined("FDFGetStats");

    return (*ptr_FDFGetStats)(fdf_thread_state, stats);
}


/*
 * FDFGetContainerStats
 */
FDF_status_t 
FDFGetContainerStats(struct FDF_thread_state *fdf_thread_state,
                     FDF_cguid_t cguid,
                     FDF_stats_t *stats)
{
    if (unlikely(!ptr_FDFGetContainerStats))
        undefined("FDFGetContainerStats");

    return (*ptr_FDFGetContainerStats)(fdf_thread_state, cguid, stats);
}
