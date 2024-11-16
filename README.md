# Universal printf

uprintf is a single-header library that allows to print anything in C, intended for debugging and prototyping.

### What is a single header library?

The idea behind single-header libraries is that they're easy to distribute since it's trivial to add them to any project.

By default the header file acts as any header would, i.e. contains **declarations** (without definitions). \
However, by defining a macro the header start to act as a `.c` file, i.e. contains **definitions**.

## Examples

Source files as well as their outputs can be found in [examples](examples). \
You can also build them yourself using `make examples`, but note that `sqlite` takes long to build.

In addition, there are also [tests](tests) with their outputs in the [baselines](tests/baselines).

### Example outputs

<details>
  <summary>avl</summary>

  ```c
AVL tree: {
    avl_tree_node_t *root = 0x50400003ffd0 ({
        avl_tree_node_t *left = 0x50400001ffd0 ({
            avl_tree_node_t *left = 0x50400000ffd0 ({
                avl_tree_node_t *left = 0x504000007fd0 ({
                    avl_tree_node_t *left = 0x504000003fd0 ({...})
                    avl_tree_node_t *right = 0x50400000bfd0 ({...})
                    int depth = 10
                    void *key = 0x1ff
                    void *data = 0x1ff
                })
                avl_tree_node_t *right = 0x504000017fd0 ({
                    avl_tree_node_t *left = 0x504000013fd0 ({...})
                    avl_tree_node_t *right = 0x50400001bfd0 ({...})
                    int depth = 10
                    void *key = 0x5ff
                    void *data = 0x5ff
                })
                int depth = 11
                void *key = 0x3ff
                void *data = 0x3ff
            })
            avl_tree_node_t *right = 0x50400002ffd0 ({
                avl_tree_node_t *left = 0x504000027fd0 ({
                    avl_tree_node_t *left = 0x504000023fd0 ({...})
                    avl_tree_node_t *right = 0x50400002bfd0 ({...})
                    int depth = 10
                    void *key = 0x9ff
                    void *data = 0x9ff
                })
                avl_tree_node_t *right = 0x504000037fd0 ({
                    avl_tree_node_t *left = 0x504000033fd0 ({...})
                    avl_tree_node_t *right = 0x50400003bfd0 ({...})
                    int depth = 10
                    void *key = 0xdff
                    void *data = 0xdff
                })
                int depth = 11
                void *key = 0xbff
                void *data = 0xbff
            })
            int depth = 12
            void *key = 0x7ff
            void *data = 0x7ff
        })
        avl_tree_node_t *right = 0x50400005ffd0 ({
            avl_tree_node_t *left = 0x50400004ffd0 ({
                avl_tree_node_t *left = 0x504000047fd0 ({
                    avl_tree_node_t *left = 0x504000043fd0 ({...})
                    avl_tree_node_t *right = 0x50400004bfd0 ({...})
                    int depth = 10
                    void *key = 0x11ff
                    void *data = 0x11ff
                })
                avl_tree_node_t *right = 0x504000057fd0 ({
                    avl_tree_node_t *left = 0x504000053fd0 ({...})
                    avl_tree_node_t *right = 0x50400005bfd0 ({...})
                    int depth = 10
                    void *key = 0x15ff
                    void *data = 0x15ff
                })
                int depth = 11
                void *key = 0x13ff
                void *data = 0x13ff
            })
            avl_tree_node_t *right = 0x50400007ffd0 ({
                avl_tree_node_t *left = 0x50400006ffd0 ({
                    avl_tree_node_t *left = 0x504000067fd0 ({...})
                    avl_tree_node_t *right = 0x504000077fd0 ({...})
                    int depth = 11
                    void *key = 0x1bff
                    void *data = 0x1bff
                })
                avl_tree_node_t *right = 0x50400008ffd0 ({
                    avl_tree_node_t *left = 0x504000087fd0 ({...})
                    avl_tree_node_t *right = 0x504000097fd0 ({...})
                    int depth = 11
                    void *key = 0x23ff
                    void *data = 0x23ff
                })
                int depth = 12
                void *key = 0x1fff
                void *data = 0x1fff
            })
            int depth = 13
            void *key = 0x17ff
            void *data = 0x17ff
        })
        int depth = 14
        void *key = 0xfff
        void *data = 0xfff
    })
    int(void *, void *) comparator = 0x5f69682a31c0 (int cmp(void *k1, void *k2))
    void(void *) destructor = NULL
}
```
</details>

<details>
  <summary>sqlite</summary>

  ```c
Sqlite's database: <#0> {
    sqlite3_vfs *pVfs = 0x609a01405d40 (<#1> {
        int iVersion = 3
        int szOsFile = 120
        int mxPathname = 512
        sqlite3_vfs *pNext = 0x609a01408040 ({
            int iVersion = 2
            int szOsFile = 120
            int mxPathname = 1024
            sqlite3_vfs *pNext = 0x609a01405f38 ({
                int iVersion = 3
                int szOsFile = 120
                int mxPathname = 512
                sqlite3_vfs *pNext = 0x609a01405e90 ({
                    int iVersion = 3
                    int szOsFile = 120
                    int mxPathname = 512
                    sqlite3_vfs *pNext = 0x609a01405de8 ({
                        int iVersion = 3
                        int szOsFile = 120
                        int mxPathname = 512
                        sqlite3_vfs *pNext = NULL
                        const char *zName = 0x609a0103fce0 ("unix-none")
                        void *pAppData = 0x609a01077a40
                        int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x609a00d15c20 (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
                        int(sqlite3_vfs *, const char *, int) xDelete = 0x609a00bf3a40 (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
                        int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x609a00ab3f20 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
                        int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x609a00bf58c0 (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
                        void *(sqlite3_vfs *, const char *) xDlOpen = 0x609a00a4c930 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
                        void(sqlite3_vfs *, int, char *) xDlError = 0x609a00bcf730 (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
                        (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x609a00a4c920 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
                        void(sqlite3_vfs *, void *) xDlClose = 0x609a00a9f630 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
                        int(sqlite3_vfs *, int, char *) xRandomness = 0x609a00bf1ee0 (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
                        int(sqlite3_vfs *, int) xSleep = 0x609a00a4c7d0 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
                        int(sqlite3_vfs *, double *) xCurrentTime = 0x609a00ad6bc0 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
                        int(sqlite3_vfs *, int, char *) xGetLastError = 0x609a00a85aa0 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
                        int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x609a00a95a40 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
                        int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x609a00a9b2c0 (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
                        (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x609a00a9b100 ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
                        const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x609a00a9ae60 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
                    })
                    const char *zName = 0x609a0103fd20 ("unix-dotfile")
                    void *pAppData = 0x609a01077940
                    int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x609a00d15c20 (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
                    int(sqlite3_vfs *, const char *, int) xDelete = 0x609a00bf3a40 (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
                    int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x609a00ab3f20 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
                    int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x609a00bf58c0 (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
                    void *(sqlite3_vfs *, const char *) xDlOpen = 0x609a00a4c930 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
                    void(sqlite3_vfs *, int, char *) xDlError = 0x609a00bcf730 (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
                    (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x609a00a4c920 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
                    void(sqlite3_vfs *, void *) xDlClose = 0x609a00a9f630 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
                    int(sqlite3_vfs *, int, char *) xRandomness = 0x609a00bf1ee0 (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
                    int(sqlite3_vfs *, int) xSleep = 0x609a00a4c7d0 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
                    int(sqlite3_vfs *, double *) xCurrentTime = 0x609a00ad6bc0 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
                    int(sqlite3_vfs *, int, char *) xGetLastError = 0x609a00a85aa0 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
                    int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x609a00a95a40 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
                    int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x609a00a9b2c0 (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
                    (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x609a00a9b100 ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
                    const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x609a00a9ae60 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
                })
                const char *zName = 0x609a0102f180 ("unix-excl")
                void *pAppData = 0x609a01077b40
                int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x609a00d15c20 (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
                int(sqlite3_vfs *, const char *, int) xDelete = 0x609a00bf3a40 (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
                int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x609a00ab3f20 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
                int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x609a00bf58c0 (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
                void *(sqlite3_vfs *, const char *) xDlOpen = 0x609a00a4c930 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
                void(sqlite3_vfs *, int, char *) xDlError = 0x609a00bcf730 (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
                (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x609a00a4c920 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
                void(sqlite3_vfs *, void *) xDlClose = 0x609a00a9f630 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
                int(sqlite3_vfs *, int, char *) xRandomness = 0x609a00bf1ee0 (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
                int(sqlite3_vfs *, int) xSleep = 0x609a00a4c7d0 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
                int(sqlite3_vfs *, double *) xCurrentTime = 0x609a00ad6bc0 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
                int(sqlite3_vfs *, int, char *) xGetLastError = 0x609a00a85aa0 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
                int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x609a00a95a40 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
                int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x609a00a9b2c0 (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
                (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x609a00a9b100 ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
                const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x609a00a9ae60 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
            })
            const char *zName = 0x609a01036d60 ("memdb")
            void *pAppData = 0x609a01405d40
            int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x609a00d19200 (int memdbOpen(sqlite3_vfs *pVfs, const char *zName, sqlite3_file *pFd, int flags, int *pOutFlags))
            int(sqlite3_vfs *, const char *, int) xDelete = NULL
            int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x609a00a57c60 (int memdbAccess(sqlite3_vfs *pVfs, const char *zPath, int flags, int *pResOut))
            int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x609a00bcec80 (int memdbFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
            void *(sqlite3_vfs *, const char *) xDlOpen = 0x609a00a57cc0 (void *memdbDlOpen(sqlite3_vfs *pVfs, const char *zPath))
            void(sqlite3_vfs *, int, char *) xDlError = 0x609a00a57dd0 (void memdbDlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg))
            (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x609a00a57ef0 ((void()) memdbDlSym(sqlite3_vfs *pVfs, void *p, const char *zSym))
            void(sqlite3_vfs *, void *) xDlClose = 0x609a00a58010 (void memdbDlClose(sqlite3_vfs *pVfs, void *pHandle))
            int(sqlite3_vfs *, int, char *) xRandomness = 0x609a00a58120 (int memdbRandomness(sqlite3_vfs *pVfs, int nByte, char *zBufOut))
            int(sqlite3_vfs *, int) xSleep = 0x609a00a58240 (int memdbSleep(sqlite3_vfs *pVfs, int nMicro))
            int(sqlite3_vfs *, double *) xCurrentTime = NULL
            int(sqlite3_vfs *, int, char *) xGetLastError = 0x609a00a58350 (int memdbGetLastError(sqlite3_vfs *pVfs, int a, char *b))
            int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x609a00a58480 (int memdbCurrentTimeInt64(sqlite3_vfs *pVfs, sqlite3_int64 *p))
            int(sqlite3_vfs *, const char *, void()) xSetSystemCall = NULL
            (void())(sqlite3_vfs *, const char *) xGetSystemCall = NULL
            const char *(sqlite3_vfs *, const char *) xNextSystemCall = NULL
        })
        const char *zName = 0x609a0103fca0 ("unix")
        void *pAppData = 0x609a01077b40
        int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x609a00d15c20 (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
        int(sqlite3_vfs *, const char *, int) xDelete = 0x609a00bf3a40 (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
        int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x609a00ab3f20 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
        int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x609a00bf58c0 (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
        void *(sqlite3_vfs *, const char *) xDlOpen = 0x609a00a4c930 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
        void(sqlite3_vfs *, int, char *) xDlError = 0x609a00bcf730 (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
        (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x609a00a4c920 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
        void(sqlite3_vfs *, void *) xDlClose = 0x609a00a9f630 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
        int(sqlite3_vfs *, int, char *) xRandomness = 0x609a00bf1ee0 (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
        int(sqlite3_vfs *, int) xSleep = 0x609a00a4c7d0 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
        int(sqlite3_vfs *, double *) xCurrentTime = 0x609a00ad6bc0 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
        int(sqlite3_vfs *, int, char *) xGetLastError = 0x609a00a85aa0 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
        int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x609a00a95a40 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
        int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x609a00a9b2c0 (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
        (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x609a00a9b100 ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
        const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x609a00a9ae60 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
    })
    Vdbe *pVdbe = NULL
    CollSeq *pDfltColl = 0x50d000000048 ({
        char *zName = 0x50d0000000c0 ("BINARY")
        u8 enc = 1
        void *pUser = NULL
        int(void *, int, const void *, int, const void *) xCmp = 0x609a00a9f650 (int binCollFunc(void *NotUsed, int nKey1, const void *pKey1, int nKey2, const void *pKey2))
        void(void *) xDel = NULL
    })
    sqlite3_mutex *mutex = 0x504000000058 ({
        pthread_mutex_t mutex = <union> {
            __pthread_mutex_s __data = {
                int __lock = 0
                unsigned int __count = 0
                int __owner = 0
                unsigned int __nusers = 0
                int __kind = 1
                short int __spins = 0
                short int __elision = 0
                __pthread_list_t __list = {
                    __pthread_internal_list *__prev = NULL
                    __pthread_internal_list *__next = NULL
                }
            }
            char[] __size = [0 <repeats 16 times>, 1, 0 <repeats 23 times>]
            long int __align = 0
        }
    })
    Db *aDb = 0x518000000320 ({
        char *zDbSName = 0x609a01023700 ("main")
        Btree *pBt = 0x507000000028 (<#2> {
            sqlite3 *db = 0x518000000088 (<points to #0>)
            BtShared *pBt = 0x50e000000048 ({
                Pager *pPager = 0x518000000488 ({
                    sqlite3_vfs *pVfs = 0x609a01405d40 (<points to #1>)
                    u8 exclusiveMode = 0
                    u8 journalMode = 0
                    u8 useJournal = 1
                    u8 noSync = 0
                    u8 fullSync = 1
                    u8 extraSync = 0
                    u8 syncFlags = 2
                    u8 walSyncFlags = 10
                    u8 tempFile = 0
                    u8 noLock = 0
                    u8 readOnly = 0
                    u8 memDb = 0
                    u8 memVfs = 0
                    u8 eState = 0
                    u8 eLock = 0
                    u8 changeCountDone = 0
                    u8 setSuper = 0
                    u8 doNotSpill = 0
                    u8 subjInMemory = 0
                    u8 bUseFetch = 0
                    u8 hasHeldSharedLock = 0
                    Pgno dbSize = 0
                    Pgno dbOrigSize = 0
                    Pgno dbFileSize = 0
                    Pgno dbHintSize = 0
                    int errCode = 0
                    int nRec = 0
                    u32 cksumInit = 0
                    u32 nSubRec = 0
                    Bitvec *pInJournal = NULL
                    sqlite3_file *fd = 0x518000000610 ({
                        const sqlite3_io_methods *pMethods = 0x609a01077b80 ({
                            int iVersion = 3
                            int(sqlite3_file *) xClose = 0x609a00bf0f50 (int unixClose(sqlite3_file *id))
                            int(sqlite3_file *, void *, int, sqlite3_int64) xRead = 0x609a00a9d220 (int unixRead(sqlite3_file *id, void *pBuf, int amt, sqlite3_int64 offset))
                            int(sqlite3_file *, const void *, int, sqlite3_int64) xWrite = 0x609a00ad3c50 (int unixWrite(sqlite3_file *id, const void *pBuf, int amt, sqlite3_int64 offset))
                            int(sqlite3_file *, sqlite3_int64) xTruncate = 0x609a00bf2290 (int unixTruncate(sqlite3_file *id, i64 nByte))
                            int(sqlite3_file *, int) xSync = 0x609a00bf2710 (int unixSync(sqlite3_file *id, int flags))
                            int(sqlite3_file *, sqlite3_int64 *) xFileSize = 0x609a00a85710 (int unixFileSize(sqlite3_file *id, i64 *pSize))
                            int(sqlite3_file *, int) xLock = 0x609a00b0f550 (int unixLock(sqlite3_file *id, int eFileLock))
                            int(sqlite3_file *, int) xUnlock = 0x609a00befd70 (int unixUnlock(sqlite3_file *id, int eFileLock))
                            int(sqlite3_file *, int *) xCheckReservedLock = 0x609a00a85b00 (int unixCheckReservedLock(sqlite3_file *id, int *pResOut))
                            int(sqlite3_file *, int, void *) xFileControl = 0x609a00d0f4d0 (int unixFileControl(sqlite3_file *id, int op, void *pArg))
                            int(sqlite3_file *) xSectorSize = 0x609a00a571a0 (int unixSectorSize(sqlite3_file *id))
                            int(sqlite3_file *) xDeviceCharacteristics = 0x609a00a57210 (int unixDeviceCharacteristics(sqlite3_file *id))
                            int(sqlite3_file *, int, int, int, volatile void **) xShmMap = 0x609a00d19d70 (int unixShmMap(sqlite3_file *fd, int iRegion, int szRegion, int bExtend, volatile void **pp))
                            int(sqlite3_file *, int, int, int) xShmLock = 0x609a00a9daf0 (int unixShmLock(sqlite3_file *fd, int ofst, int n, int flags))
                            void(sqlite3_file *) xShmBarrier = 0x609a00adcc80 (void unixShmBarrier(sqlite3_file *fd))
                            int(sqlite3_file *, int) xShmUnmap = 0x609a00bf0770 (int unixShmUnmap(sqlite3_file *fd, int deleteFlag))
                            int(sqlite3_file *, sqlite3_int64, int, void **) xFetch = 0x609a00bf3680 (int unixFetch(sqlite3_file *fd, i64 iOff, int nAmt, void **pp))
                            int(sqlite3_file *, sqlite3_int64, void *) xUnfetch = 0x609a00a578b0 (int unixUnfetch(sqlite3_file *fd, i64 iOff, void *p))
                        })
                    })
                    sqlite3_file *jfd = 0x518000000700 ({
                        const sqlite3_io_methods *pMethods = NULL
                    })
                    sqlite3_file *sjfd = 0x518000000688 ({
                        const sqlite3_io_methods *pMethods = NULL
                    })
                    i64 journalOff = 0
                    i64 journalHdr = 0
                    sqlite3_backup *pBackup = NULL
                    PagerSavepoint *aSavepoint = NULL
                    int nSavepoint = 0
                    u32 iDataVersion = 1
                    char[] dbFileVers = [0 <repeats 16 times>]
                    int nMmapOut = 0
                    sqlite3_int64 szMmap = 0
                    PgHdr *pMmapFreelist = NULL
                    u16 nExtra = 136
                    i16 nReserve = 0
                    u32 vfsFlags = 262
                    u32 sectorSize = 512
                    Pgno mxPgno = 4294967294
                    Pgno lckPgno = 262145
                    i64 pageSize = 4096
                    i64 journalSizeLimit = -1
                    char *zFilename = 0x518000000784 ("/home/tx/projects/uprintf/test.db")
                    char *zJournal = 0x5180000007a7 ("/home/tx/projects/uprintf/test.db-journal")
                    int(void *) xBusyHandler = 0x609a00a7bc50 (int btreeInvokeBusyHandler(void *pArg))
                    void *pBusyHandlerArg = 0x50e000000048
                    u32[] aStat = [0 <repeats 4 times>]
                    void(DbPage *) xReiniter = 0x609a00be5050 (void pageReinit(DbPage *pData))
                    int(Pager *, Pgno, DbPage **, int) xGet = 0x609a00cb0190 (int getPageNormal(Pager *pPager, Pgno pgno, DbPage **ppPage, int flags))
                    char *pTmpSpace = 0x521000000108 ("��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������"...)
                    PCache *pPCache = 0x5180000005c0 ({
                        PgHdr *pDirty = NULL
                        PgHdr *pDirtyTail = NULL
                        PgHdr *pSynced = NULL
                        i64 nRefSum = 0
                        int szCache = -2000
                        int szSpill = 1
                        int szPage = 4096
                        int szExtra = 136
                        u8 bPurgeable = 1
                        u8 eCreate = 2
                        int(void *, PgHdr *) xStress = 0x609a00d303e0 (int pagerStress(void *p, PgHdr *pPg))
                        void *pStress = 0x518000000488
                        sqlite3_pcache *pCache = 0x50f000000048 ({})
                    })
                    Wal *pWal = NULL
                    char *zWal = 0x5180000007d1 ("/home/tx/projects/uprintf/test.db-wal")
                })
                sqlite3 *db = 0x518000000088 (<points to #0>)
                BtCursor *pCursor = NULL
                MemPage *pPage1 = NULL
                u8 openFlags = 0
                u8 autoVacuum = 0
                u8 incrVacuum = 0
                u8 bDoTruncate = 0
                u8 inTransaction = 0
                u8 max1bytePayload = 0
                u8 nReserveWanted = 0
                u16 btsFlags = 0
                u16 maxLocal = 0
                u16 minLocal = 0
                u16 maxLeaf = 0
                u16 minLeaf = 0
                u32 pageSize = 4096
                u32 usableSize = 4096
                int nTransaction = 0
                u32 nPage = 0
                void *pSchema = 0x50c000000048
                void(void *) xFreeSchema = 0x609a00aee880 (void sqlite3SchemaClear(void *p))
                sqlite3_mutex *mutex = NULL
                Bitvec *pHasContent = NULL
                int nRef = 1
                BtShared *pNext = NULL
                BtLock *pLock = NULL
                Btree *pWriter = NULL
                u8 *pTmpSpace = NULL
                int nPreformatSize = 0
            })
            u8 inTrans = 0
            u8 sharable = 0
            u8 locked = 0
            u8 hasIncrblobCur = 0
            int wantToLock = 0
            int nBackup = 0
            u32 iBDataVersion = 0
            Btree *pNext = NULL
            Btree *pPrev = NULL
            BtLock lock = {
                Btree *pBtree = 0x507000000028 (<points to #2>)
                Pgno iTable = 1
                u8 eLock = 0
                BtLock *pNext = NULL
            }
        })
        u8 safety_level = 3
        u8 bSyncSet = 0
        Schema *pSchema = 0x50c000000048 ({
            int schema_cookie = 0
            int iGeneration = 0
            Hash tblHash = {
                unsigned int htsize = 0
                unsigned int count = 0
                HashElem *first = NULL
                _ht *ht = NULL
            }
            Hash idxHash = {
                unsigned int htsize = 0
                unsigned int count = 0
                HashElem *first = NULL
                _ht *ht = NULL
            }
            Hash trigHash = {
                unsigned int htsize = 0
                unsigned int count = 0
                HashElem *first = NULL
                _ht *ht = NULL
            }
            Hash fkeyHash = {
                unsigned int htsize = 0
                unsigned int count = 0
                HashElem *first = NULL
                _ht *ht = NULL
            }
            Table *pSeqTab = NULL
            u8 file_format = 0
            u8 enc = 1
            u16 schemaFlags = 0
            int cache_size = 0
        })
    })
    int nDb = 2
    u32 mDbFlags = 0
    u64 flags = 3758391520
    i64 lastRowid = 0
    i64 szMmap = 0
    u32 nSchemaLock = 0
    unsigned int openFlags = 6
    int errCode = 0
    int errByteOffset = -1
    int errMask = 255
    int iSysErrno = 0
    u32 dbOptFlags = 0
    u8 enc = 1
    u8 autoCommit = 1
    u8 temp_store = 0
    u8 mallocFailed = 0
    u8 bBenignMalloc = 0
    u8 dfltLockMode = 0
    signed char nextAutovac = -1
    u8 suppressErr = 0
    u8 vtabOnConflict = 0
    u8 isTransactionSavepoint = 0
    u8 mTrace = 0
    u8 noSharedCache = 0
    u8 nSqlExec = 0
    u8 eOpenState = 118 ('v')
    int nextPagesize = 0
    i64 nChange = 0
    i64 nTotalChange = 0
    int[] aLimit = [1000000000, 1000000000, 2000, 1000, 500, 250000000, 127, 10, 50000, 32766, 1000, 0]
    int nMaxSorterMmap = 0
    sqlite3InitInfo init = {
        Pgno newTnum = 0
        u8 iDb = 0
        u8 busy = 0
        unsigned int orphanTrigger = 0 <1 bit>
        unsigned int imposterTable = 0 <1 bit>
        unsigned int reopenMemdb = 0 <1 bit>
        const char **azInit = 0x609a01408440
    }
    int nVdbeActive = 0
    int nVdbeRead = 0
    int nVdbeWrite = 0
    int nVdbeExec = 0
    int nVDestroy = 0
    int nExtension = 0
    void **aExtension = NULL
    union trace = <union> {
        void(void *, const char *) xLegacy = NULL
        int(u32, void *, void *, void *) xV2 = NULL
    }
    void *pTraceArg = NULL
    void(void *, const char *, u64) xProfile = NULL
    void *pProfileArg = NULL
    void *pCommitArg = NULL
    int(void *) xCommitCallback = NULL
    void *pRollbackArg = NULL
    void(void *) xRollbackCallback = NULL
    void *pUpdateArg = NULL
    void(void *, int, const char *, const char *, sqlite_int64) xUpdateCallback = NULL
    void *pAutovacPagesArg = NULL
    void(void *) xAutovacDestr = NULL
    unsigned int(void *, const char *, u32, u32, u32) xAutovacPages = NULL
    Parse *pParse = NULL
    int(void *, sqlite3 *, const char *, int) xWalCallback = 0x609a00f0def0 (int sqlite3WalDefaultHook(void *pClientData, sqlite3 *db, const char *zDb, int nFrame))
    void *pWalArg = 0x3e8
    void(void *, sqlite3 *, int, const char *) xCollNeeded = NULL
    void(void *, sqlite3 *, int, const void *) xCollNeeded16 = NULL
    void *pCollNeededArg = NULL
    sqlite3_value *pErr = NULL
    union u1 = <union> {
        volatile int isInterrupted = 0
        double notUsed1 = 0.000000
    }
    Lookaside lookaside = {
        u32 bDisable = 0
        u16 sz = 1200
        u16 szTrue = 1200
        u8 bMalloced = 1
        u32 nSlot = 123
        u32[] anStat = [0, 0, 0]
        LookasideSlot *pInit = 0x52e000008bf8 ({
            LookasideSlot *pNext = 0x52e000008748 ({
                LookasideSlot *pNext = 0x52e000008298 ({
                    LookasideSlot *pNext = 0x52e000007de8 ({
                        LookasideSlot *pNext = 0x52e000007938 ({
                            LookasideSlot *pNext = 0x52e000007488 ({
                                LookasideSlot *pNext = 0x52e000006fd8 ({
                                    LookasideSlot *pNext = 0x52e000006b28 ({
                                        LookasideSlot *pNext = 0x52e000006678 ({...})
                                    })
                                })
                            })
                        })
                    })
                })
            })
        })
        LookasideSlot *pFree = NULL
        LookasideSlot *pSmallInit = 0x52e00000bea8 ({
            LookasideSlot *pNext = 0x52e00000be28 ({
                LookasideSlot *pNext = 0x52e00000bda8 ({
                    LookasideSlot *pNext = 0x52e00000bd28 ({
                        LookasideSlot *pNext = 0x52e00000bca8 ({
                            LookasideSlot *pNext = 0x52e00000bc28 ({
                                LookasideSlot *pNext = 0x52e00000bba8 ({
                                    LookasideSlot *pNext = 0x52e00000bb28 ({
                                        LookasideSlot *pNext = 0x52e00000baa8 ({...})
                                    })
                                })
                            })
                        })
                    })
                })
            })
        })
        LookasideSlot *pSmallFree = NULL
        void *pMiddle = 0x52e0000090a8
        void *pStart = 0x52e000000408
        void *pEnd = 0x52e00000bf28
        void *pTrueEnd = 0x52e00000bf28
    }
    int(void *, int, const char *, const char *, const char *, const char *) xAuth = NULL
    void *pAuthArg = NULL
    int(void *) xProgress = NULL
    void *pProgressArg = NULL
    unsigned int nProgressOps = 0
    int nVTrans = 0
    Hash aModule = {
        unsigned int htsize = 0
        unsigned int count = 2
        HashElem *first = 0x504000000218 (<#3> {
            HashElem *next = 0x5040000001d8 ({
                HashElem *next = NULL
                HashElem *prev = 0x504000000218 (<points to #3>)
                void *data = 0x507000000098
                const char *pKey = 0x5070000000c8 ("json_each")
            })
            HashElem *prev = NULL
            void *data = 0x507000000108
            const char *pKey = 0x507000000138 ("json_tree")
        })
        _ht *ht = NULL
    }
    VtabCtx *pVtabCtx = NULL
    VTable **aVTrans = NULL
    VTable *pDisconnect = NULL
    Hash aFunc = {
        unsigned int htsize = 0
        unsigned int count = 1
        HashElem *first = 0x504000000198 ({
            HashElem *next = NULL
            HashElem *prev = NULL
            void *data = 0x5080000000a8
            const char *pKey = 0x5080000000f0 ("match")
        })
        _ht *ht = NULL
    }
    Hash aCollSeq = {
        unsigned int htsize = 0
        unsigned int count = 3
        HashElem *first = 0x504000000118 (<#4> {
            HashElem *next = 0x5040000000d8 (<#5> {
                HashElem *next = 0x504000000098 ({
                    HashElem *next = NULL
                    HashElem *prev = 0x5040000000d8 (<points to #5>)
                    void *data = 0x50d000000048
                    const char *pKey = 0x50d0000000c0 ("BINARY")
                })
                HashElem *prev = 0x504000000118 (<points to #4>)
                void *data = 0x50d000000118
                const char *pKey = 0x50d000000190 ("NOCASE")
            })
            HashElem *prev = NULL
            void *data = 0x50d0000001e8
            const char *pKey = 0x50d000000260 ("RTRIM")
        })
        _ht *ht = NULL
    }
    BusyHandler busyHandler = {
        int(void *, int) xBusyHandler = NULL
        void *pBusyArg = NULL
        int nBusy = 0
    }
    Db[] aDbStatic = [
        {
            char *zDbSName = 0x609a01023700 ("main")
            Btree *pBt = 0x507000000028 (<points to #2>)
            u8 safety_level = 3
            u8 bSyncSet = 0
            Schema *pSchema = 0x50c000000048 ({
                int schema_cookie = 0
                int iGeneration = 0
                Hash tblHash = {
                    unsigned int htsize = 0
                    unsigned int count = 0
                    HashElem *first = NULL
                    _ht *ht = NULL
                }
                Hash idxHash = {
                    unsigned int htsize = 0
                    unsigned int count = 0
                    HashElem *first = NULL
                    _ht *ht = NULL
                }
                Hash trigHash = {
                    unsigned int htsize = 0
                    unsigned int count = 0
                    HashElem *first = NULL
                    _ht *ht = NULL
                }
                Hash fkeyHash = {
                    unsigned int htsize = 0
                    unsigned int count = 0
                    HashElem *first = NULL
                    _ht *ht = NULL
                }
                Table *pSeqTab = NULL
                u8 file_format = 0
                u8 enc = 1
                u16 schemaFlags = 0
                int cache_size = 0
            })
        },
        {
            char *zDbSName = 0x609a01024d40 ("temp")
            Btree *pBt = NULL
            u8 safety_level = 1
            u8 bSyncSet = 0
            Schema *pSchema = 0x50c000000108 ({
                int schema_cookie = 0
                int iGeneration = 0
                Hash tblHash = {
                    unsigned int htsize = 0
                    unsigned int count = 0
                    HashElem *first = NULL
                    _ht *ht = NULL
                }
                Hash idxHash = {
                    unsigned int htsize = 0
                    unsigned int count = 0
                    HashElem *first = NULL
                    _ht *ht = NULL
                }
                Hash trigHash = {
                    unsigned int htsize = 0
                    unsigned int count = 0
                    HashElem *first = NULL
                    _ht *ht = NULL
                }
                Hash fkeyHash = {
                    unsigned int htsize = 0
                    unsigned int count = 0
                    HashElem *first = NULL
                    _ht *ht = NULL
                }
                Table *pSeqTab = NULL
                u8 file_format = 0
                u8 enc = 1
                u16 schemaFlags = 0
                int cache_size = 0
            })
        }
    ]
    Savepoint *pSavepoint = NULL
    int nAnalysisLimit = 0
    int busyTimeout = 0
    int nSavepoint = 0
    int nStatement = 0
    i64 nDeferredCons = 0
    i64 nDeferredImmCons = 0
    int *pnBytesFreed = NULL
    DbClientData *pDbData = NULL
}
  ```
</details>

<details>
  <summary>vorbis</summary>

  ```c
Vorbis data: {
    unsigned int sample_rate = 44100
    int channels = 2
    unsigned int setup_memory_required = 193864
    unsigned int temp_memory_required = 4096
    unsigned int setup_temp_memory_required = 7209
    char *vendor = 0x504000000010 ("Xiph.Org libVorbis I 20120203 (Omnipresent)")
    int comment_list_length = 3
    char **comment_list = 0x503000000100
    FILE *f = 0x515000000080 (<ignored>)
    uint32 f_start = 0
    int close_on_free = 1
    uint8 *stream = NULL
    uint8 *stream_start = NULL
    uint8 *stream_end = NULL
    uint32 stream_len = 1032948
    uint8 push_mode = 0
    uint32 first_audio_page_offset = 4465
    ProbedPage p_first = {
        uint32 page_start = 4465
        uint32 page_end = 8913
        uint32 last_decoded_sample = 22080
    }
    ProbedPage p_last = {
        uint32 page_start = 0
        uint32 page_end = 0
        uint32 last_decoded_sample = 0
    }
    stb_vorbis_alloc alloc = {
        char *alloc_buffer = NULL
        int alloc_buffer_length_in_bytes = 0
    }
    int setup_offset = 0
    int temp_offset = 0
    int eof = 0
    STBVorbisError error = VORBIS__no_error (0)
    int[] blocksize = [256, 2048]
    int blocksize_0 = 256
    int blocksize_1 = 2048
    int codebook_count = 42
    Codebook *codebooks = 0x532000000800 ({
        int dimensions = 1
        int entries = 8
        uint8 *codeword_lengths = 0x502000000030 ("")
        float minimum_value = 0.000000
        float delta_value = 0.000000
        uint8 value_bits = 0
        uint8 lookup_type = 0
        uint8 sequence_p = 0
        uint8 sparse = 0
        uint32 lookup_values = 0
        codetype *multiplicands = NULL
        uint32 *codewords = 0x503000000190 (0)
        int16[] fast_huffman = [0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 3, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 7, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 3, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 7, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 3, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 7, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 3, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 7, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 3, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 7, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 3, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 7, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 3, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 7, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 3, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 7, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 6, 0, 4, 0, 1, 0, 4, 0, 2, 0, 4, 0, 1, 0, 4, 0, 5, 0, 4]
        uint32 *sorted_codewords = NULL
        int *sorted_values = NULL
        int sorted_entries = 0
    })
    int floor_count = 2
    uint16[] floor_types = [1, 1, 0 <repeats 62 times>]
    Floor *floor_config = 0x51f000000080 (<union> {
        Floor0 floor0 = {
            uint8 order = 6
            uint16 rate = 257
            uint16 bark_map_size = 770
            uint8 amplitude_bits = 3
            uint8 amplitude_offset = 190
            uint8 number_of_books = 190
            uint8[] book_list = [190 <repeats 16 times>]
        }
        Floor1 floor1 = {
            uint8 partitions = 6
            uint8[] partition_class_list = [0, 1, 1, 2, 3, 3, 190 <repeats 26 times>]
            uint8[] class_dimensions = [2, 3, 3, 3, 190 <repeats 12 times>]
            uint8[] class_subclasses = [0, 1, 2, 2, 190 <repeats 12 times>]
            uint8[] class_masterbooks = [190, 0, 1, 2, 190 <repeats 12 times>]
            int16[][] subclass_books = [
                [3, -16706 <repeats 7 times>],
                [4, 5, -16706 <repeats 6 times>],
                [-1, 6, 7, 8, -16706 <repeats 4 times>],
                [-1, 9, 10, 11, -16706 <repeats 4 times>],
                [-16706 <repeats 8 times>] <repeats 12 times>
            ]
            uint16[] Xlist = [0, 128, 12, 46, 4, 8, 16, 23, 33, 70, 2, 6, 10, 14, 19, 28, 39, 58, 90, 48830 <repeats 231 times>]
            uint8[] sorted_order = [0, 10, 4, 11, 5, 12, 2, 13, 6, 14, 7, 15, 8, 16, 3, 17, 9, 18, 1, 190 <repeats 231 times>]
            uint8[][] neighbors = [
                [190, 190],
                [190, 190],
                [0, 1],
                [2, 1],
                [0, 2],
                [4, 2],
                [2, 3],
                [6, 3],
                [7, 3],
                [3, 1],
                [0, 4],
                [4, 5],
                [5, 2],
                [2, 6],
                [6, 7],
                [7, 8],
                [8, 3],
                [3, 9],
                [9, 1],
                [190, 190] <repeats 231 times>
            ]
            uint8 floor1_multiplier = 2
            uint8 rangebits = 7
            int values = 19
        }
    })
    int residue_count = 2
    uint16[] residue_types = [2, 2, 0 <repeats 62 times>]
    Residue *residue_config = 0x5060000005c0 ({
        uint32 begin = 0
        uint32 end = 240
        uint32 part_size = 16
        uint8 classifications = 10
        uint8 classbook = 27
        uint8 **classdata = 0x518000001080
        int16[] *residue_books = 0x50e0000003c0 ([-1 <repeats 8 times>])
    })
    int mapping_count = 2
    Mapping *mapping = 0x5080000004a0 ({
        uint16 coupling_steps = 1
        MappingChannel *chan = 0x502000001ab0 ({
            uint8 magnitude = 0
            uint8 angle = 1
            uint8 mux = 0
        })
        uint8 submaps = 1
        uint8[] submap_floor = [0 <repeats 15 times>]
        uint8[] submap_residue = [0 <repeats 15 times>]
    })
    int mode_count = 2
    Mode[] mode_config = [
        {
            uint8 blockflag = 0
            uint8 mapping = 0
            uint16 windowtype = 0
            uint16 transformtype = 0
        },
        {
            uint8 blockflag = 1
            uint8 mapping = 1
            uint16 windowtype = 0
            uint16 transformtype = 0
        },
        {
            uint8 blockflag = 0
            uint8 mapping = 0
            uint16 windowtype = 0
            uint16 transformtype = 0
        } <repeats 62 times>
    ]
    uint32 total_samples = 0
    float[] channel_buffers = [0x525000000100 (-0.000001), 0x525000002900 (0.000001), NULL <repeats 14 times>]
    float[] outputs = [NULL <repeats 16 times>]
    float[] previous_window = [0x521000001500 (0.000001), 0x521000002900 (-0.000000), NULL <repeats 14 times>]
    int previous_length = 128
    int16[] finalY = [0x506000000620 (26), 0x506000000680 (26), NULL <repeats 14 times>]
    uint32 current_loc = 0
    int current_loc_valid = 1
    float[] A = [0x515000001c00 (1.000000), 0x521000003d00 (1.000000)]
    float[] B = [0x515000001e80 (0.499991), 0x521000005100 (0.500000)]
    float[] C = [0x5110000020c0 (0.999699), 0x51d000001480 (0.999995)]
    float[] window = [0x515000002100 (0.000059), 0x521000006500 (0.000001)]
    uint16[] bit_reverse = [0x5060000006e0 (0), 0x515000002380 (0)]
    uint32 serial = 0
    int last_page = 2
    int segment_count = 35
    uint8[] segments = [22, 255, 25, 127, 72 ('H'), 93 (']'), 233, 255, 101 ('e'), 71 ('G'), 65 ('A'), 115 ('s'), 82 ('R'), 103 ('g'), 245, 255, 107 ('k'), 72 ('H'), 63 ('?'), 62 ('>'), 62 ('>'), 113 ('q'), 79 ('O'), 94 ('^'), 234, 255, 107 ('k'), 71 ('G'), 64 ('@'), 118 ('v'), 87 ('W'), 93 (']'), 218, 255, 113 ('q'), 0 <repeats 220 times>]
    uint8 page_flag = 0
    uint8 bytes_in_seg = 0
    uint8 first_decode = 0
    int next_seg = 1
    int last_seg = 1
    int last_seg_which = 0
    uint32 acc = 0
    int valid_bits = 3
    int packet_bytes = 22
    int end_seg_with_known_loc = 34
    uint32 known_loc_for_packet = 22080
    int discard_samples_deferred = 0
    uint32 samples_output = 0
    int page_crc_tests = -1
    CRCscan[] scan = [
        {
            uint32 goal_crc = 0
            int bytes_left = 0
            uint32 crc_so_far = 0
            int bytes_done = 0
            uint32 sample_loc = 0
        } <repeats 4 times>
    ]
    int channel_buffer_start = 0
    int channel_buffer_end = 0
}
  ```
</details>

<details>
  <summary>uprintf</summary>

  ```c
uprintf's state: {
    uint32_t capacity = 128
    uint32_t length = 111
    _upf_type_map_entry *data = 0x7a8aeed108e8 ({
        const uint8_t *die = 0x7a8aeb686851 (14)
        _upf_type *type_ptr = 0x7a8aeed0d8b8 ({
            const char *name = 0x7a8aeb6d6ef4 ("_upf_arena_region")
            _upf_type_kind kind = _UPF_TK_POINTER (4)
            int modifiers = 0
            size_t size = 8
            union as = <union> {
                struct cstruct = {
                    _upf_member_vec members = {
                        uint32_t capacity = 4006665512
                        uint32_t length = 31370
                        _upf_member *data = NULL
                    }
                }
                struct cenum = {
                    _upf_type *underlying_type = 0x7a8aeed0dd28 (<#0> {
                        const char *name = 0x7a8aeb6d6ef4 ("_upf_arena_region")
                        _upf_type_kind kind = _UPF_TK_STRUCT (0)
                        int modifiers = 0
                        size_t size = 32
                        union as = <union> {
                            struct cstruct = {
                                _upf_member_vec members = {
                                    uint32_t capacity = 4
                                    uint32_t length = 4
                                    _upf_member *data = 0x7a8aeed0db78 ({
                                        const char *name = 0x7a8aeb6d6eba ("data")
                                        _upf_type *type = 0x7a8aeed0d928 ({...})
                                        size_t offset = 0
                                        int bit_size = 0
                                    })
                                }
                            }
                            struct cenum = {
                                _upf_type *underlying_type = 0x400000004 (<out-of-bounds>)
                                _upf_enum_vec enums = {
                                    uint32_t capacity = 4006665080
                                    uint32_t length = 31370
                                    _upf_enum *data = NULL
                                }
                            }
                            struct array = {
                                _upf_type *element_type = 0x400000004 (<out-of-bounds>)
                                _upf_size_t_vec lengths = {
                                    uint32_t capacity = 4006665080
                                    uint32_t length = 31370
                                    size_t *data = NULL
                                }
                            }
                            struct pointer = {
                                _upf_type *type = 0x400000004 (<out-of-bounds>)
                            }
                            struct function = {
                                _upf_type *return_type = 0x400000004 (<out-of-bounds>)
                                _upf_type_ptr_vec arg_types = {
                                    uint32_t capacity = 4006665080
                                    uint32_t length = 31370
                                    _upf_type **data = NULL
                                }
                            }
                        }
                    })
                    _upf_enum_vec enums = {
                        uint32_t capacity = 0
                        uint32_t length = 0
                        _upf_enum *data = NULL
                    }
                }
                struct array = {
                    _upf_type *element_type = 0x7a8aeed0dd28 (<points to #0>)
                    _upf_size_t_vec lengths = {
                        uint32_t capacity = 0
                        uint32_t length = 0
                        size_t *data = NULL
                    }
                }
                struct pointer = {
                    _upf_type *type = 0x7a8aeed0dd28 (<points to #0>)
                }
                struct function = {
                    _upf_type *return_type = 0x7a8aeed0dd28 (<points to #0>)
                    _upf_type_ptr_vec arg_types = {
                        uint32_t capacity = 0
                        uint32_t length = 0
                        _upf_type **data = NULL
                    }
                }
            }
        })
    })
}
  ```
</details>

## Installation

Installing a single header library is as simple as downloading `uprintf.h`.

If you have cloned the repository, it can be installed to `/usr/local/include` by running `make install`. \
This way you should be able to include it without copying the header to every project (since it is a default include path).

## Requirements

- Linux
- Minimum C version is `c99`
- Debug information included, `-g2` or higher
- Have `elf.h` in include path

### Tested on (using CI/CD):

Architecture: \
`x86_64/amd64`

Compilers: \
`gcc`: 14, 13, 12, 11 \
`clang`: 18, 17, 16, 15, 14

### Limitations

1. Casting to function pointer, e.g.:
    ```c
    uprintf("%S\n", (void (*)(void)) whatever);
    ```
2. Refering to the variable from higher scope when it is also defined in the current one, e.g.:
    ```c
    char var = 'c';
    {
        uprintf("%S\n", &var); // prints char as an int
        int var = 1;
        uprintf("%S\n", &var); // prints int
    }
    ```

3. Printing information about function from shared library with clang:
    ```c
    uprintf("%S\n", printf);
    // gcc  : 0x12345678 <void printf(const char *, ...)>
    // clang: 0x12345678
    ```

## Usage

0. Copy or install the library, ensure it is in the include path

1. Pick a single file and define `UPRINTF_IMPLEMENTATION` before the include:
    ```c
    #define UPRINTF_IMPLEMENTATION
    #include "uprintf.h"
    ```
    This will copy library's implementation to be built as a part of this file, so you should choose rarely modified file in order to avoid unnecessary rebuilding. \
    You can also define options here, see below.

2. Add include in other files where you intend to use it:
    ```c
    #include "uprintf.h"
    ```

3. Call function.
    ```c
    uprintf(fmt, ...);
    ```
    `fmt` - a format string with placeholders(`%` followed by a letter). Unlike in `printf`, you can use anything, e.g. I use `%S`. Use `%%` to print `%`. \
    For each format specifier there must be a pointer to whatever should be printed in its place (except `void*`).

### Options

Behavior of the library can be changed by setting options before **implementation**:

```c
#define OPTION_NAME VALUE
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"
```

The list of options:

macro |  description | default
-|-|-
`UPRINTF_INDENTATION_WIDTH` | The number of spaces to use for indentation | 4
`UPRINTF_MAX_DEPTH` | How deep can nested structures be. Use a negative value to have no limit | 10
`UPRINTF_IGNORE_STDIO_FILE` | Should `stdio.h`'s `FILE` be ignored | true
`UPRINTF_ARRAY_COMPRESSION_THRESHOLD` | The minimum number of consecutive array values that get compressed(`VALUE <repeats X times>`). Use a non-positive value to disable it | 4
`UPRINTF_MAX_STRING_LENGTH` | The max string length after which it will be truncated. Use a non-positive value to have no limit | 200

## How does it work?

TL;DR: It works by inspecting debugging information of the executable in a debugger-like manner, which allows it to interpret and format passed pointers.

0. Read current binary.
1. Parse ELF and DWARF.
2. Get info about scopes and types.
3. Locate scopes from which current call is invoked based on the PC.
4. Parse provided arguments.
5. Infer types from the parsed arguments.
7. Print data using type definition as reference.

## Tests

Tests take long to complete, so it is highly recommended to set `-j NUMBER_OF_CORES`.

If you use compiler other than `gcc` and `clang` (e.g. `clang-18`) make sure to specify `COMPILERS` or edit [Makefile](Makefile).

```console
$ make tests
$ make tests -jNUMBER_OF_CORES
$ make tests COMPILERS=COMPILER_NAME
```
