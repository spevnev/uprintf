# Universal printf

*uprintf* is a header-only library for printing anything in C.

## Examples

Examples and their outputs can be found in [examples](examples), and can be built using `make examples`.

### Example outputs

<details>
  <summary>avl (<a href="https://github.com/etherealvisage/avl">src</a>)</summary>

```c
AVL tree: {
  avl_tree_node_t *root = 0x7bde7a21ffd0 ({
    avl_tree_node_t *left = 0x7bde7a1fffd0 ({
      avl_tree_node_t *left = 0x7bde7a1effd0 ({
        avl_tree_node_t *left = 0x7bde7a1e7fd0 ({
          avl_tree_node_t *left = 0x7bde7a1e3fd0 ({...})
          avl_tree_node_t *right = 0x7bde7a1ebfd0 ({...})
          int depth = 10
          void *key = 0x1ff
          void *data = 0x1ff
        })
        avl_tree_node_t *right = 0x7bde7a1f7fd0 ({
          avl_tree_node_t *left = 0x7bde7a1f3fd0 ({...})
          avl_tree_node_t *right = 0x7bde7a1fbfd0 ({...})
          int depth = 10
          void *key = 0x5ff
          void *data = 0x5ff
        })
        int depth = 11
        void *key = 0x3ff
        void *data = 0x3ff
      })
      avl_tree_node_t *right = 0x7bde7a20ffd0 ({
        avl_tree_node_t *left = 0x7bde7a207fd0 ({
          avl_tree_node_t *left = 0x7bde7a203fd0 ({...})
          avl_tree_node_t *right = 0x7bde7a20bfd0 ({...})
          int depth = 10
          void *key = 0x9ff
          void *data = 0x9ff
        })
        avl_tree_node_t *right = 0x7bde7a217fd0 ({
          avl_tree_node_t *left = 0x7bde7a213fd0 ({...})
          avl_tree_node_t *right = 0x7bde7a21bfd0 ({...})
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
    avl_tree_node_t *right = 0x7bde7a23ffd0 ({
      avl_tree_node_t *left = 0x7bde7a22ffd0 ({
        avl_tree_node_t *left = 0x7bde7a227fd0 ({
          avl_tree_node_t *left = 0x7bde7a223fd0 ({...})
          avl_tree_node_t *right = 0x7bde7a22bfd0 ({...})
          int depth = 10
          void *key = 0x11ff
          void *data = 0x11ff
        })
        avl_tree_node_t *right = 0x7bde7a237fd0 ({
          avl_tree_node_t *left = 0x7bde7a233fd0 ({...})
          avl_tree_node_t *right = 0x7bde7a23bfd0 ({...})
          int depth = 10
          void *key = 0x15ff
          void *data = 0x15ff
        })
        int depth = 11
        void *key = 0x13ff
        void *data = 0x13ff
      })
      avl_tree_node_t *right = 0x7bde7a25ffd0 ({
        avl_tree_node_t *left = 0x7bde7a24ffd0 ({
          avl_tree_node_t *left = 0x7bde7a247fd0 ({...})
          avl_tree_node_t *right = 0x7bde7a257fd0 ({...})
          int depth = 11
          void *key = 0x1bff
          void *data = 0x1bff
        })
        avl_tree_node_t *right = 0x7bde7a26ffd0 ({
          avl_tree_node_t *left = 0x7bde7a267fd0 ({...})
          avl_tree_node_t *right = 0x7bde7a277fd0 ({...})
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
  int(void *, void *) comparator = 0x557b024a4650 (int cmp(void *k1, void *k2))
  void(void *) destructor = NULL
}
```
</details>

<details>
  <summary>sqlite (<a href="https://www.sqlite.org">src</a>)</summary>

```c
Sqlite's database: <#0> {
    sqlite3_vfs *pVfs = 0x558f46be8be0 (<#1> {
        int iVersion = 3
        int szOsFile = 120
        int mxPathname = 512
        sqlite3_vfs *pNext = 0x558f46beae60 ({
            int iVersion = 2
            int szOsFile = 120
            int mxPathname = 1024
            sqlite3_vfs *pNext = 0x558f46be8dd8 ({
                int iVersion = 3
                int szOsFile = 120
                int mxPathname = 512
                sqlite3_vfs *pNext = 0x558f46be8d30 ({
                    int iVersion = 3
                    int szOsFile = 120
                    int mxPathname = 512
                    sqlite3_vfs *pNext = 0x558f46be8c88 ({
                        int iVersion = 3
                        int szOsFile = 120
                        int mxPathname = 512
                        sqlite3_vfs *pNext = NULL
                        const char *zName = 0x558f46848680 ("unix-none")
                        void *pAppData = 0x558f4687aa40
                        int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x558f46535780 (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
                        int(sqlite3_vfs *, const char *, int) xDelete = 0x558f46415330 (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
                        int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x558f462caf30 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
                        int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x558f46416d10 (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
                        void *(sqlite3_vfs *, const char *) xDlOpen = 0x558f46257b80 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
                        void(sqlite3_vfs *, int, char *) xDlError = 0x558f463f1870 (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
                        (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x558f46257b70 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
                        void(sqlite3_vfs *, void *) xDlClose = 0x558f462b58b0 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
                        int(sqlite3_vfs *, int, char *) xRandomness = 0x558f464135d0 (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
                        int(sqlite3_vfs *, int) xSleep = 0x558f46257a20 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
                        int(sqlite3_vfs *, double *) xCurrentTime = 0x558f462ed2f0 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
                        int(sqlite3_vfs *, int, char *) xGetLastError = 0x558f46299ff0 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
                        int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x558f462abf30 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
                        int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x558f462b10e0 (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
                        (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x558f462b0f20 ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
                        const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x558f462b0c90 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
                    })
                    const char *zName = 0x558f468486c0 ("unix-dotfile")
                    void *pAppData = 0x558f4687a940
                    int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x558f46535780 (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
                    int(sqlite3_vfs *, const char *, int) xDelete = 0x558f46415330 (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
                    int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x558f462caf30 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
                    int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x558f46416d10 (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
                    void *(sqlite3_vfs *, const char *) xDlOpen = 0x558f46257b80 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
                    void(sqlite3_vfs *, int, char *) xDlError = 0x558f463f1870 (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
                    (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x558f46257b70 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
                    void(sqlite3_vfs *, void *) xDlClose = 0x558f462b58b0 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
                    int(sqlite3_vfs *, int, char *) xRandomness = 0x558f464135d0 (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
                    int(sqlite3_vfs *, int) xSleep = 0x558f46257a20 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
                    int(sqlite3_vfs *, double *) xCurrentTime = 0x558f462ed2f0 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
                    int(sqlite3_vfs *, int, char *) xGetLastError = 0x558f46299ff0 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
                    int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x558f462abf30 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
                    int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x558f462b10e0 (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
                    (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x558f462b0f20 ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
                    const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x558f462b0c90 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
                })
                const char *zName = 0x558f46837ea0 ("unix-excl")
                void *pAppData = 0x558f4687ab40
                int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x558f46535780 (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
                int(sqlite3_vfs *, const char *, int) xDelete = 0x558f46415330 (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
                int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x558f462caf30 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
                int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x558f46416d10 (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
                void *(sqlite3_vfs *, const char *) xDlOpen = 0x558f46257b80 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
                void(sqlite3_vfs *, int, char *) xDlError = 0x558f463f1870 (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
                (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x558f46257b70 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
                void(sqlite3_vfs *, void *) xDlClose = 0x558f462b58b0 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
                int(sqlite3_vfs *, int, char *) xRandomness = 0x558f464135d0 (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
                int(sqlite3_vfs *, int) xSleep = 0x558f46257a20 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
                int(sqlite3_vfs *, double *) xCurrentTime = 0x558f462ed2f0 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
                int(sqlite3_vfs *, int, char *) xGetLastError = 0x558f46299ff0 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
                int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x558f462abf30 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
                int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x558f462b10e0 (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
                (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x558f462b0f20 ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
                const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x558f462b0c90 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
            })
            const char *zName = 0x558f4683fa80 ("memdb")
            void *pAppData = 0x558f46be8be0
            int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x558f46538cd0 (int memdbOpen(sqlite3_vfs *pVfs, const char *zName, sqlite3_file *pFd, int flags, int *pOutFlags))
            int(sqlite3_vfs *, const char *, int) xDelete = NULL
            int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x558f46263d70 (int memdbAccess(sqlite3_vfs *pVfs, const char *zPath, int flags, int *pResOut))
            int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x558f463f0c30 (int memdbFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
            void *(sqlite3_vfs *, const char *) xDlOpen = 0x558f46263dd0 (void *memdbDlOpen(sqlite3_vfs *pVfs, const char *zPath))
            void(sqlite3_vfs *, int, char *) xDlError = 0x558f46263f60 (void memdbDlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg))
            (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x558f46264130 ((void()) memdbDlSym(sqlite3_vfs *pVfs, void *p, const char *zSym))
            void(sqlite3_vfs *, void *) xDlClose = 0x558f46264300 (void memdbDlClose(sqlite3_vfs *pVfs, void *pHandle))
            int(sqlite3_vfs *, int, char *) xRandomness = 0x558f46264490 (int memdbRandomness(sqlite3_vfs *pVfs, int nByte, char *zBufOut))
            int(sqlite3_vfs *, int) xSleep = 0x558f46264660 (int memdbSleep(sqlite3_vfs *pVfs, int nMicro))
            int(sqlite3_vfs *, double *) xCurrentTime = NULL
            int(sqlite3_vfs *, int, char *) xGetLastError = 0x558f462647f0 (int memdbGetLastError(sqlite3_vfs *pVfs, int a, char *b))
            int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x558f462649d0 (int memdbCurrentTimeInt64(sqlite3_vfs *pVfs, sqlite3_int64 *p))
            int(sqlite3_vfs *, const char *, void()) xSetSystemCall = NULL
            (void())(sqlite3_vfs *, const char *) xGetSystemCall = NULL
            const char *(sqlite3_vfs *, const char *) xNextSystemCall = NULL
        })
        const char *zName = 0x558f46848640 ("unix")
        void *pAppData = 0x558f4687ab40
        int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x558f46535780 (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
        int(sqlite3_vfs *, const char *, int) xDelete = 0x558f46415330 (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
        int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x558f462caf30 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
        int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x558f46416d10 (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
        void *(sqlite3_vfs *, const char *) xDlOpen = 0x558f46257b80 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
        void(sqlite3_vfs *, int, char *) xDlError = 0x558f463f1870 (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
        (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x558f46257b70 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
        void(sqlite3_vfs *, void *) xDlClose = 0x558f462b58b0 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
        int(sqlite3_vfs *, int, char *) xRandomness = 0x558f464135d0 (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
        int(sqlite3_vfs *, int) xSleep = 0x558f46257a20 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
        int(sqlite3_vfs *, double *) xCurrentTime = 0x558f462ed2f0 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
        int(sqlite3_vfs *, int, char *) xGetLastError = 0x558f46299ff0 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
        int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x558f462abf30 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
        int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x558f462b10e0 (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
        (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x558f462b0f20 ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
        const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x558f462b0c90 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
    })
    Vdbe *pVdbe = NULL
    CollSeq *pDfltColl = 0x7cb7991e0048 ({
        char *zName = 0x7cb7991e00c0 ("BINARY")
        u8 enc = 1
        void *pUser = NULL
        int(void *, int, const void *, int, const void *) xCmp = 0x558f462b58d0 (int binCollFunc(void *NotUsed, int nKey1, const void *pKey1, int nKey2, const void *pKey2))
        void(void *) xDel = NULL
    })
    sqlite3_mutex *mutex = 0x7c27991e0058 ({
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
    Db *aDb = 0x7d67991e0320 ({
        char *zDbSName = 0x558f4682c100 ("main")
        Btree *pBt = 0x7c57991e0028 (<#2> {
            sqlite3 *db = 0x7d67991e0088 (<points to #0>)
            BtShared *pBt = 0x7cc7991e0048 ({
                Pager *pPager = 0x7d67991e0488 ({
                    sqlite3_vfs *pVfs = 0x558f46be8be0 (<points to #1>)
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
                    sqlite3_file *fd = 0x7d67991e0610 ({
                        const sqlite3_io_methods *pMethods = 0x558f4687ab80 ({
                            int iVersion = 3
                            int(sqlite3_file *) xClose = 0x558f464125b0 (int unixClose(sqlite3_file *id))
                            int(sqlite3_file *, void *, int, sqlite3_int64) xRead = 0x558f462b3250 (int unixRead(sqlite3_file *id, void *pBuf, int amt, sqlite3_int64 offset))
                            int(sqlite3_file *, const void *, int, sqlite3_int64) xWrite = 0x558f462ea400 (int unixWrite(sqlite3_file *id, const void *pBuf, int amt, sqlite3_int64 offset))
                            int(sqlite3_file *, sqlite3_int64) xTruncate = 0x558f46413980 (int unixTruncate(sqlite3_file *id, i64 nByte))
                            int(sqlite3_file *, int) xSync = 0x558f46413e20 (int unixSync(sqlite3_file *id, int flags))
                            int(sqlite3_file *, sqlite3_int64 *) xFileSize = 0x558f46299c60 (int unixFileSize(sqlite3_file *id, i64 *pSize))
                            int(sqlite3_file *, int) xLock = 0x558f4632b4c0 (int unixLock(sqlite3_file *id, int eFileLock))
                            int(sqlite3_file *, int) xUnlock = 0x558f46411480 (int unixUnlock(sqlite3_file *id, int eFileLock))
                            int(sqlite3_file *, int *) xCheckReservedLock = 0x558f4629a050 (int unixCheckReservedLock(sqlite3_file *id, int *pResOut))
                            int(sqlite3_file *, int, void *) xFileControl = 0x558f4652e780 (int unixFileControl(sqlite3_file *id, int op, void *pArg))
                            int(sqlite3_file *) xSectorSize = 0x558f46263210 (int unixSectorSize(sqlite3_file *id))
                            int(sqlite3_file *) xDeviceCharacteristics = 0x558f46263290 (int unixDeviceCharacteristics(sqlite3_file *id))
                            int(sqlite3_file *, int, int, int, volatile void **) xShmMap = 0x558f46539810 (int unixShmMap(sqlite3_file *fd, int iRegion, int szRegion, int bExtend, volatile void **pp))
                            int(sqlite3_file *, int, int, int) xShmLock = 0x558f462b3ae0 (int unixShmLock(sqlite3_file *fd, int ofst, int n, int flags))
                            void(sqlite3_file *) xShmBarrier = 0x558f462f3c90 (void unixShmBarrier(sqlite3_file *fd))
                            int(sqlite3_file *, int) xShmUnmap = 0x558f46411de0 (int unixShmUnmap(sqlite3_file *fd, int deleteFlag))
                            int(sqlite3_file *, sqlite3_int64, int, void **) xFetch = 0x558f46414df0 (int unixFetch(sqlite3_file *fd, i64 iOff, int nAmt, void **pp))
                            int(sqlite3_file *, sqlite3_int64, void *) xUnfetch = 0x558f46263a00 (int unixUnfetch(sqlite3_file *fd, i64 iOff, void *p))
                        })
                    })
                    sqlite3_file *jfd = 0x7d67991e0700 ({
                        const sqlite3_io_methods *pMethods = NULL
                    })
                    sqlite3_file *sjfd = 0x7d67991e0688 ({
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
                    char *zFilename = 0x7d67991e0784 ("/projects/uprintf/test.db")
                    char *zJournal = 0x7d67991e07a7 ("/projects/uprintf/test.db-journal")
                    int(void *) xBusyHandler = 0x558f4628e8a0 (int btreeInvokeBusyHandler(void *pArg))
                    void *pBusyHandlerArg = 0x7cc7991e0048
                    u32[] aStat = [0 <repeats 4 times>]
                    void(DbPage *) xReiniter = 0x558f46406680 (void pageReinit(DbPage *pData))
                    int(Pager *, Pgno, DbPage **, int) xGet = 0x558f464cfd90 (int getPageNormal(Pager *pPager, Pgno pgno, DbPage **ppPage, int flags))
                    char *pTmpSpace = 0x7df7991e0108 ("ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½"...)
                    PCache *pPCache = 0x7d67991e05c0 ({
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
                        int(void *, PgHdr *) xStress = 0x55eab364a590 (int pagerStress(void *p, PgHdr *pPg))
                        void *pStress = 0x7d46b89e0488
                        sqlite3_pcache *pCache = 0x7cb6b89e0048 ({} <opaque>)
                    })
                    Wal *pWal = NULL
                    char *zWal = 0x7d67991e07d1 ("/projects/uprintf/test.db-wal")
                })
                sqlite3 *db = 0x7d67991e0088 (<points to #0>)
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
                void *pSchema = 0x7ca7991e0048
                void(void *) xFreeSchema = 0x558f46307070 (void sqlite3SchemaClear(void *p))
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
                Btree *pBtree = 0x7c57991e0028 (<points to #2>)
                Pgno iTable = 1
                u8 eLock = 0
                BtLock *pNext = NULL
            }
        })
        u8 safety_level = 3
        u8 bSyncSet = 0
        Schema *pSchema = 0x7ca7991e0048 ({
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
        const char **azInit = 0x558f46beb260
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
    int(void *, sqlite3 *, const char *, int) xWalCallback = 0x558f46723900 (int sqlite3WalDefaultHook(void *pClientData, sqlite3 *db, const char *zDb, int nFrame))
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
        LookasideSlot *pInit = 0x7ec7991e8bf8 ({
            LookasideSlot *pNext = 0x7ec7991e8748 ({
                LookasideSlot *pNext = 0x7ec7991e8298 ({
                    LookasideSlot *pNext = 0x7ec7991e7de8 ({
                        LookasideSlot *pNext = 0x7ec7991e7938 ({
                            LookasideSlot *pNext = 0x7ec7991e7488 ({
                                LookasideSlot *pNext = 0x7ec7991e6fd8 ({
                                    LookasideSlot *pNext = 0x7ec7991e6b28 ({
                                        LookasideSlot *pNext = 0x7ec7991e6678 ({...})
                                    })
                                })
                            })
                        })
                    })
                })
            })
        })
        LookasideSlot *pFree = NULL
        LookasideSlot *pSmallInit = 0x7ec7991ebea8 ({
            LookasideSlot *pNext = 0x7ec7991ebe28 ({
                LookasideSlot *pNext = 0x7ec7991ebda8 ({
                    LookasideSlot *pNext = 0x7ec7991ebd28 ({
                        LookasideSlot *pNext = 0x7ec7991ebca8 ({
                            LookasideSlot *pNext = 0x7ec7991ebc28 ({
                                LookasideSlot *pNext = 0x7ec7991ebba8 ({
                                    LookasideSlot *pNext = 0x7ec7991ebb28 ({
                                        LookasideSlot *pNext = 0x7ec7991ebaa8 ({...})
                                    })
                                })
                            })
                        })
                    })
                })
            })
        })
        LookasideSlot *pSmallFree = NULL
        void *pMiddle = 0x7ec7991e90a8
        void *pStart = 0x7ec7991e0408
        void *pEnd = 0x7ec7991ebf28
        void *pTrueEnd = 0x7ec7991ebf28
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
        HashElem *first = 0x7c27991e0218 (<#3> {
            HashElem *next = 0x7c27991e01d8 ({
                HashElem *next = NULL
                HashElem *prev = 0x7c27991e0218 (<points to #3>)
                void *data = 0x7c57991e0098
                const char *pKey = 0x7c57991e00c8 ("json_each")
            })
            HashElem *prev = NULL
            void *data = 0x7c57991e0108
            const char *pKey = 0x7c57991e0138 ("json_tree")
        })
        _ht *ht = NULL
    }
    VtabCtx *pVtabCtx = NULL
    VTable **aVTrans = NULL
    VTable *pDisconnect = NULL
    Hash aFunc = {
        unsigned int htsize = 0
        unsigned int count = 1
        HashElem *first = 0x7c27991e0198 ({
            HashElem *next = NULL
            HashElem *prev = NULL
            void *data = 0x7c67991e00a8
            const char *pKey = 0x7c67991e00f0 ("match")
        })
        _ht *ht = NULL
    }
    Hash aCollSeq = {
        unsigned int htsize = 0
        unsigned int count = 3
        HashElem *first = 0x7c27991e0118 (<#4> {
            HashElem *next = 0x7c27991e00d8 (<#5> {
                HashElem *next = 0x7c27991e0098 ({
                    HashElem *next = NULL
                    HashElem *prev = 0x7c27991e00d8 (<points to #5>)
                    void *data = 0x7cb7991e0048
                    const char *pKey = 0x7cb7991e00c0 ("BINARY")
                })
                HashElem *prev = 0x7c27991e0118 (<points to #4>)
                void *data = 0x7cb7991e0118
                const char *pKey = 0x7cb7991e0190 ("NOCASE")
            })
            HashElem *prev = NULL
            void *data = 0x7cb7991e01e8
            const char *pKey = 0x7cb7991e0260 ("RTRIM")
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
            char *zDbSName = 0x558f4682c100 ("main")
            Btree *pBt = 0x7c57991e0028 (<points to #2>)
            u8 safety_level = 3
            u8 bSyncSet = 0
            Schema *pSchema = 0x7ca7991e0048 ({
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
            char *zDbSName = 0x558f4682d7a0 ("temp")
            Btree *pBt = NULL
            u8 safety_level = 1
            u8 bSyncSet = 0
            Schema *pSchema = 0x7ca7991e0108 ({
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
num 1
num 2
num 3
num 4
num 5
```
</details>

<details>
  <summary>vorbis (<a href="https://github.com/nothings/stb/blob/master/stb_vorbis.c">src</a>)</summary>

```c
Vorbis data: {
    unsigned int sample_rate = 44100
    int channels = 2
    unsigned int setup_memory_required = 178512
    unsigned int temp_memory_required = 4096
    unsigned int setup_temp_memory_required = 7209
    char *vendor = 0x7b5c561e0010 ("Lavf58.20.100")
    int comment_list_length = 1
    char **comment_list = 0x7b5c561e0030
    FILE *f = 0x7c8c561e0300 (<ignored>)
    uint32 f_start = 0
    int close_on_free = 1
    uint8 *stream = NULL
    uint8 *stream_start = NULL
    uint8 *stream_end = NULL
    uint32 stream_len = 4115
    uint8 push_mode = 0
    uint32 first_audio_page_offset = 3998
    ProbedPage p_first = {
        uint32 page_start = 3998
        uint32 page_end = 4115
        uint32 last_decoded_sample = 44100
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
    int codebook_count = 38
    Codebook *codebooks = 0x7e5c561e0800 ({
        int dimensions = 1
        int entries = 64
        uint8 *codeword_lengths = 0x7b9c561e0080 ("\n\a\b\r\t\a\v\n\b\b\f\a\t\b\b\b\r\a\a\b\a\f\f\n\a\b\t\a\t\f\t\nï¿½ï¿½nÌ¹ï¿½nï¿½ï¿½")
        float minimum_value = 0.000000
        float delta_value = 0.000000
        uint8 value_bits = 0
        uint8 lookup_type = 0
        uint8 sequence_p = 0
        uint8 sparse = 0
        uint32 lookup_values = 0
        codetype *multiplicands = NULL
        uint32 *codewords = 0x7c4c561e0040 (0)
        int16[] fast_huffman = [0, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 24, 38, 34, 54, 17, 37, 26, 50, 21, 38, 39, 58, 10, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 8, 37, 22, 42, 25, 38, 40, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 2, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 27, 38, 34, 54, 17, 37, 26, 56, 21, 38, 39, 58, 23, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 9, 37, 22, 42, 25, 38, 48, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 4, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 24, 38, 34, 54, 17, 37, 26, 50, 21, 38, 39, 58, 10, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 19, 37, 22, 42, 25, 38, 40, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 2, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 27, 38, 34, 54, 17, 37, 26, 62, 21, 38, 39, 58, 23, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 9, 37, 22, 42, 25, 38, 51, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, -1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 24, 38, 34, 54, 17, 37, 26, 50, 21, 38, 39, 58, 10, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, -1, 37, 22, 42, 25, 38, 40, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 2, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 27, 38, 34, 54, 17, 37, 26, 56, 21, 38, 39, 58, 23, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 9, 37, 22, 42, 25, 38, 63, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 4, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 24, 38, 34, 54, 17, 37, 26, 50, 21, 38, 39, 58, 10, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 19, 37, 22, 42, 25, 38, 40, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 2, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 27, 38, 34, 54, 17, 37, 26, 62, 21, 38, 39, 58, 23, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 9, 37, 22, 42, 25, 38, 51, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59]
        uint32 *sorted_codewords = 0x7bac561e0020 (4194304)
        int *sorted_values = 0x7bac561e0094 (3)
        int sorted_entries = 17
    })
    int floor_count = 2
    uint16[] floor_types = [1, 1, 0 <repeats 62 times>]
    Floor *floor_config = 0x7d2c561e0080 (<union> {
        Floor0 floor0 = {
            uint8 order = 2
            uint16 rate = 48641
            uint16 bark_map_size = 48830
            uint8 amplitude_bits = 190
            uint8 amplitude_offset = 190
            uint8 number_of_books = 190
            uint8[] book_list = [190 <repeats 16 times>]
        }
        Floor1 floor1 = {
            uint8 partitions = 2
            uint8[] partition_class_list = [0, 1, 190 <repeats 30 times>]
            uint8[] class_dimensions = [3, 4, 190 <repeats 14 times>]
            uint8[] class_subclasses = [2, 2, 190 <repeats 14 times>]
            uint8[] class_masterbooks = [0, 1, 190 <repeats 14 times>]
            int16[][] subclass_books = [
                [-1, 2, 3, 4, -16706 <repeats 4 times>],
                [-1, 5, 6, 7, -16706 <repeats 4 times>],
                [-16706 <repeats 8 times>] <repeats 14 times>
            ]
            uint16[] Xlist = [0, 128, 14, 4, 58, 2, 8, 28, 90, 48830 <repeats 241 times>]
            uint8[] sorted_order = [0, 5, 3, 6, 2, 7, 4, 8, 1, 190 <repeats 241 times>]
            uint8[][] neighbors = [
                [190, 190],
                [190, 190],
                [0, 1],
                [0, 2],
                [2, 1],
                [0, 3],
                [3, 2],
                [2, 4],
                [4, 1],
                [190, 190] <repeats 241 times>
            ]
            uint8 floor1_multiplier = 4
            uint8 rangebits = 7
            int values = 9
        }
    })
    int residue_count = 2
    uint16[] residue_types = [2, 2, 0 <repeats 62 times>]
    Residue *residue_config = 0x7b9c561e0560 ({
        uint32 begin = 0
        uint32 end = 208
        uint32 part_size = 16
        uint8 classifications = 10
        uint8 classbook = 23
        uint8 **classdata = 0x7cbc561e1080
        int16[] *residue_books = 0x7c1c561e03c0 ([-1 <repeats 8 times>])
    })
    int mapping_count = 2
    Mapping *mapping = 0x7bbc561e04a0 ({
        uint16 coupling_steps = 1
        MappingChannel *chan = 0x7b5c561e1b30 ({
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
    float[] channel_buffers = [0x7d8c561e0100 (0.000000), 0x7d8c561e2900 (0.000000), NULL <repeats 14 times>]
    float[] outputs = [NULL <repeats 16 times>]
    float[] previous_window = [0x7d4c561e1500 (0.000000), 0x7d4c561e2900 (0.000000), NULL <repeats 14 times>]
    int previous_length = 128
    int16[] finalY = [0x7b9c561e05c0 (-16706), 0x7b9c561e0620 (-16706), NULL <repeats 14 times>]
    uint32 current_loc = 0
    int current_loc_valid = 1
    float[] A = [0x7c8c561e1480 (1.000000), 0x7d4c561e3d00 (1.000000)]
    float[] B = [0x7c8c561e1700 (0.499991), 0x7d4c561e5100 (0.500000)]
    float[] C = [0x7c4c561e1bc0 (0.999699), 0x7d0c561e1480 (0.999995)]
    float[] window = [0x7c8c561e1980 (0.000059), 0x7d4c561e6500 (0.000001)]
    uint16[] bit_reverse = [0x7b9c561e0680 (0), 0x7c8c561e1c00 (0)]
    uint32 serial = 0
    int last_page = 2
    int segment_count = 45
    uint8[] segments = [1 <repeats 45 times>, 0 <repeats 210 times>]
    uint8 page_flag = 4
    uint8 bytes_in_seg = 0
    uint8 first_decode = 0
    int next_seg = 1
    int last_seg = 1
    int last_seg_which = 0
    uint32 acc = 0
    int valid_bits = 4
    int packet_bytes = 1
    int end_seg_with_known_loc = 44
    uint32 known_loc_for_packet = 44100
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
  <summary>miniaudio (<a href="https://github.com/mackron/miniaudio">src</a>)</summary>

```c
Miniaudio engine: <#0> {
    ma_node_graph nodeGraph = <#1> {
        ma_node_base base = {
            ma_node_graph *pNodeGraph = 0x7b1cfb000020 (<points to #1>)
            const ma_node_vtable *vtable = 0x5616b4ca7580 ({
                void(ma_node *, const float **, ma_uint32 *, float **, ma_uint32 *) onProcess = 0x5616b4972750 (void ma_node_graph_node_process_pcm_frames(ma_node *pNode, const float **ppFramesIn, ma_uint32 *pFrameCountIn, float **ppFramesOut, ma_uint32 *pFrameCountOut))
                ma_result(ma_node *, ma_uint32, ma_uint32 *) onGetRequiredInputFrameCount = NULL
                ma_uint8 inputBusCount = 0
                ma_uint8 outputBusCount = 1
                ma_uint32 flags = 0
            })
            ma_uint32 inputBusCount = 0
            ma_uint32 outputBusCount = 1
            ma_node_input_bus *pInputBuses = 0x7b1cfb000078 ({
                ma_node_output_bus head = {
                    ma_node *pNode = NULL
                    ma_uint8 outputBusIndex = 0
                    ma_uint8 channels = 0
                    ma_uint8 inputNodeInputBusIndex = 0
                    ma_uint32 flags = 0
                    ma_uint32 refCount = 0
                    ma_bool32 isAttached = 0
                    ma_spinlock lock = 0
                    float volume = 0.000000
                    ma_node_output_bus *pNext = NULL
                    ma_node_output_bus *pPrev = NULL
                    ma_node *pInputNode = NULL
                }
                ma_uint32 nextCounter = 0
                ma_spinlock lock = 0
                ma_uint8 channels = 0
            })
            ma_node_output_bus *pOutputBuses = 0x7b1cfb000108 ({
                ma_node *pNode = 0x7b1cfb000020
                ma_uint8 outputBusIndex = 0
                ma_uint8 channels = 2
                ma_uint8 inputNodeInputBusIndex = 0
                ma_uint32 flags = 1
                ma_uint32 refCount = 0
                ma_bool32 isAttached = 0
                ma_spinlock lock = 0
                float volume = 1.000000
                ma_node_output_bus *pNext = NULL
                ma_node_output_bus *pPrev = NULL
                ma_node *pInputNode = NULL
            })
            float *pCachedData = NULL
            ma_uint16 cachedDataCapInFramesPerBus = 0
            ma_uint16 cachedFrameCountOut = 0
            ma_uint16 cachedFrameCountIn = 0
            ma_uint16 consumedFrameCountIn = 0
            ma_node_state state = ma_node_state_started (0)
            ma_uint64[] stateTimes = [0, 18446744073709551615]
            ma_uint64 localTime = 0
            ma_node_input_bus[] _inputBuses = [
                {
                    ma_node_output_bus head = {
                        ma_node *pNode = NULL
                        ma_uint8 outputBusIndex = 0
                        ma_uint8 channels = 0
                        ma_uint8 inputNodeInputBusIndex = 0
                        ma_uint32 flags = 0
                        ma_uint32 refCount = 0
                        ma_bool32 isAttached = 0
                        ma_spinlock lock = 0
                        float volume = 0.000000
                        ma_node_output_bus *pNext = NULL
                        ma_node_output_bus *pPrev = NULL
                        ma_node *pInputNode = NULL
                    }
                    ma_uint32 nextCounter = 0
                    ma_spinlock lock = 0
                    ma_uint8 channels = 0
                },
                {
                    ma_node_output_bus head = {
                        ma_node *pNode = NULL
                        ma_uint8 outputBusIndex = 0
                        ma_uint8 channels = 0
                        ma_uint8 inputNodeInputBusIndex = 0
                        ma_uint32 flags = 0
                        ma_uint32 refCount = 0
                        ma_bool32 isAttached = 0
                        ma_spinlock lock = 0
                        float volume = 0.000000
                        ma_node_output_bus *pNext = NULL
                        ma_node_output_bus *pPrev = NULL
                        ma_node *pInputNode = NULL
                    }
                    ma_uint32 nextCounter = 0
                    ma_spinlock lock = 0
                    ma_uint8 channels = 0
                }
            ]
            ma_node_output_bus[] _outputBuses = [
                {
                    ma_node *pNode = 0x7b1cfb000020
                    ma_uint8 outputBusIndex = 0
                    ma_uint8 channels = 2
                    ma_uint8 inputNodeInputBusIndex = 0
                    ma_uint32 flags = 1
                    ma_uint32 refCount = 0
                    ma_bool32 isAttached = 0
                    ma_spinlock lock = 0
                    float volume = 1.000000
                    ma_node_output_bus *pNext = NULL
                    ma_node_output_bus *pPrev = NULL
                    ma_node *pInputNode = NULL
                },
                {
                    ma_node *pNode = NULL
                    ma_uint8 outputBusIndex = 0
                    ma_uint8 channels = 0
                    ma_uint8 inputNodeInputBusIndex = 0
                    ma_uint32 flags = 0
                    ma_uint32 refCount = 0
                    ma_bool32 isAttached = 0
                    ma_spinlock lock = 0
                    float volume = 0.000000
                    ma_node_output_bus *pNext = NULL
                    ma_node_output_bus *pPrev = NULL
                    ma_node *pInputNode = NULL
                }
            ]
            void *_pHeap = NULL
            ma_bool32 _ownsHeap = 1
        }
        ma_node_base endpoint = {
            ma_node_graph *pNodeGraph = 0x7b1cfb000020 (<points to #1>)
            const ma_node_vtable *vtable = 0x5616b4ca7540 ({
                void(ma_node *, const float **, ma_uint32 *, float **, ma_uint32 *) onProcess = 0x5616b49687c0 (void ma_node_graph_endpoint_process_pcm_frames(ma_node *pNode, const float **ppFramesIn, ma_uint32 *pFrameCountIn, float **ppFramesOut, ma_uint32 *pFrameCountOut))
                ma_result(ma_node *, ma_uint32, ma_uint32 *) onGetRequiredInputFrameCount = NULL
                ma_uint8 inputBusCount = 1
                ma_uint8 outputBusCount = 1
                ma_uint32 flags = 1
            })
            ma_uint32 inputBusCount = 1
            ma_uint32 outputBusCount = 1
            ma_node_input_bus *pInputBuses = 0x7b1cfb0001e0 ({
                ma_node_output_bus head = <#2> {
                    ma_node *pNode = NULL
                    ma_uint8 outputBusIndex = 0
                    ma_uint8 channels = 0
                    ma_uint8 inputNodeInputBusIndex = 0
                    ma_uint32 flags = 0
                    ma_uint32 refCount = 4294967294
                    ma_bool32 isAttached = 0
                    ma_spinlock lock = 0
                    float volume = 0.000000
                    ma_node_output_bus *pNext = 0x7cacfbfe7968 (<#3> {
                        ma_node *pNode = 0x7cacfbfe7880
                        ma_uint8 outputBusIndex = 0
                        ma_uint8 channels = 2
                        ma_uint8 inputNodeInputBusIndex = 0
                        ma_uint32 flags = 1
                        ma_uint32 refCount = 0
                        ma_bool32 isAttached = 1
                        ma_spinlock lock = 0
                        float volume = 1.000000
                        ma_node_output_bus *pNext = NULL
                        ma_node_output_bus *pPrev = 0x7b1cfb0001e0 (<points to #2>)
                        ma_node *pInputNode = 0x7b1cfb000188
                    })
                    ma_node_output_bus *pPrev = NULL
                    ma_node *pInputNode = NULL
                }
                ma_uint32 nextCounter = 0
                ma_spinlock lock = 0
                ma_uint8 channels = 2
            })
            ma_node_output_bus *pOutputBuses = 0x7b1cfb000270 ({
                ma_node *pNode = 0x7b1cfb000188
                ma_uint8 outputBusIndex = 0
                ma_uint8 channels = 2
                ma_uint8 inputNodeInputBusIndex = 0
                ma_uint32 flags = 1
                ma_uint32 refCount = 0
                ma_bool32 isAttached = 0
                ma_spinlock lock = 0
                float volume = 1.000000
                ma_node_output_bus *pNext = NULL
                ma_node_output_bus *pPrev = NULL
                ma_node *pInputNode = NULL
            })
            float *pCachedData = 0x7d9cfbfe0100 (0.000000)
            ma_uint16 cachedDataCapInFramesPerBus = 900
            ma_uint16 cachedFrameCountOut = 0
            ma_uint16 cachedFrameCountIn = 0
            ma_uint16 consumedFrameCountIn = 0
            ma_node_state state = ma_node_state_started (0)
            ma_uint64[] stateTimes = [0, 18446744073709551615]
            ma_uint64 localTime = 0
            ma_node_input_bus[] _inputBuses = [
                {
                    ma_node_output_bus head = <points to #2>
                    ma_uint32 nextCounter = 0
                    ma_spinlock lock = 0
                    ma_uint8 channels = 2
                },
                {
                    ma_node_output_bus head = {
                        ma_node *pNode = NULL
                        ma_uint8 outputBusIndex = 0
                        ma_uint8 channels = 0
                        ma_uint8 inputNodeInputBusIndex = 0
                        ma_uint32 flags = 0
                        ma_uint32 refCount = 0
                        ma_bool32 isAttached = 0
                        ma_spinlock lock = 0
                        float volume = 0.000000
                        ma_node_output_bus *pNext = NULL
                        ma_node_output_bus *pPrev = NULL
                        ma_node *pInputNode = NULL
                    }
                    ma_uint32 nextCounter = 0
                    ma_spinlock lock = 0
                    ma_uint8 channels = 0
                }
            ]
            ma_node_output_bus[] _outputBuses = [
                {
                    ma_node *pNode = 0x7b1cfb000188
                    ma_uint8 outputBusIndex = 0
                    ma_uint8 channels = 2
                    ma_uint8 inputNodeInputBusIndex = 0
                    ma_uint32 flags = 1
                    ma_uint32 refCount = 0
                    ma_bool32 isAttached = 0
                    ma_spinlock lock = 0
                    float volume = 1.000000
                    ma_node_output_bus *pNext = NULL
                    ma_node_output_bus *pPrev = NULL
                    ma_node *pInputNode = NULL
                },
                {
                    ma_node *pNode = NULL
                    ma_uint8 outputBusIndex = 0
                    ma_uint8 channels = 0
                    ma_uint8 inputNodeInputBusIndex = 0
                    ma_uint32 flags = 0
                    ma_uint32 refCount = 0
                    ma_bool32 isAttached = 0
                    ma_spinlock lock = 0
                    float volume = 0.000000
                    ma_node_output_bus *pNext = NULL
                    ma_node_output_bus *pPrev = NULL
                    ma_node *pInputNode = NULL
                }
            ]
            void *_pHeap = 0x7d9cfbfe0100
            ma_bool32 _ownsHeap = 1
        }
        float *pProcessingCache = 0x7d5cfbfe6100 (-0.372549)
        ma_uint32 processingCacheFramesRemaining = 0
        ma_uint32 processingSizeInFrames = 900
        ma_bool32 isReading = 0
        ma_stack *pPreMixStack = 0x7b1cfa9f5800 ({
            size_t offset = 0
            size_t sizeInBytes = 1048576
            unsigned char[] _data = [190]
        })
    }
    ma_resource_manager *pResourceManager = 0x7cacfbfe7380 (<#4> {
        ma_resource_manager_config config = {
            ma_allocation_callbacks allocationCallbacks = {
                void *pUserData = NULL
                void *(size_t, void *) onMalloc = 0x5616b47026d0 (void *ma__malloc_default(size_t sz, void *pUserData))
                void *(void *, size_t, void *) onRealloc = 0x5616b47026c0 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
                void(void *, void *) onFree = 0x5616b47026b0 (void ma__free_default(void *p, void *pUserData))
            }
            ma_log *pLog = 0x7cacfbfe00f8 (<#5> {
                ma_log_callback[] callbacks = [
                    {
                        void(void *, ma_uint32, const char *) onLog = NULL
                        void *pUserData = NULL
                    } <repeats 4 times>
                ]
                ma_uint32 callbackCount = 0
                ma_allocation_callbacks allocationCallbacks = {
                    void *pUserData = NULL
                    void *(size_t, void *) onMalloc = 0x5616b47026d0 (void *ma__malloc_default(size_t sz, void *pUserData))
                    void *(void *, size_t, void *) onRealloc = 0x5616b47026c0 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
                    void(void *, void *) onFree = 0x5616b47026b0 (void ma__free_default(void *p, void *pUserData))
                }
                ma_mutex lock = <union> {
                    __pthread_mutex_s __data = {
                        int __lock = 0
                        unsigned int __count = 0
                        int __owner = 0
                        unsigned int __nusers = 0
                        int __kind = 0
                        short int __spins = 0
                        short int __elision = 0
                        __pthread_list_t __list = {
                            __pthread_internal_list *__prev = NULL
                            __pthread_internal_list *__next = NULL
                        }
                    }
                    char[] __size = [0 <repeats 40 times>]
                    long int __align = 0
                }
            })
            ma_format decodedFormat = ma_format_f32 (5)
            ma_uint32 decodedChannels = 0
            ma_uint32 decodedSampleRate = 48000
            ma_uint32 jobThreadCount = 1
            size_t jobThreadStackSize = 0
            ma_uint32 jobQueueCapacity = 1024
            ma_uint32 flags = 0
            ma_vfs *pVFS = 0x7cacfbfe76d8
            ma_decoding_backend_vtable **ppCustomDecodingBackendVTables = NULL
            ma_uint32 customDecodingBackendCount = 0
            void *pCustomDecodingBackendUserData = NULL
        }
        ma_resource_manager_data_buffer_node *pRootDataBufferNode = 0x7bccfbfe10c0 (<#6> {
            ma_uint32 hashedName32 = 4190531244
            ma_uint32 refCount = 1
            ma_result result = MA_SUCCESS (0)
            ma_uint32 executionCounter = 1
            ma_uint32 executionPointer = 1
            ma_bool32 isDataOwnedByResourceManager = 1
            ma_resource_manager_data_supply data = {
                ma_resource_manager_data_supply_type type = ma_resource_manager_data_supply_type_encoded (1)
                union backend = <union> {
                    struct encoded = {
                        const void *pData = 0x7b1ced834800
                        size_t sizeInBytes = 1798051
                    }
                    struct decoded = {
                        const void *pData = 0x7b1ced834800
                        ma_uint64 totalFrameCount = 1798051
                        ma_uint64 decodedFrameCount = 0
                        ma_format format = ma_format_unknown (0)
                        ma_uint32 channels = 0
                        ma_uint32 sampleRate = 0
                    }
                    struct decodedPaged = {
                        ma_paged_audio_buffer_data data = {
                            ma_format format = <unknown> (3984803840)
                            ma_uint32 channels = 31516
                            ma_paged_audio_buffer_page head = {
                                ma_paged_audio_buffer_page *pNext = 0x1b6fa3 (<out-of-bounds>)
                                ma_uint64 sizeInFrames = 0
                                ma_uint8[] pAudioData = [0]
                            }
                            ma_paged_audio_buffer_page *pTail = NULL
                        }
                        ma_uint64 decodedFrameCount = 0
                        ma_uint32 sampleRate = 0
                    }
                }
            }
            ma_resource_manager_data_buffer_node *pParent = NULL
            ma_resource_manager_data_buffer_node *pChildLo = NULL
            ma_resource_manager_data_buffer_node *pChildHi = NULL
        })
        ma_mutex dataBufferBSTLock = <union> {
            __pthread_mutex_s __data = {
                int __lock = 0
                unsigned int __count = 0
                int __owner = 0
                unsigned int __nusers = 0
                int __kind = 0
                short int __spins = 0
                short int __elision = 0
                __pthread_list_t __list = {
                    __pthread_internal_list *__prev = NULL
                    __pthread_internal_list *__next = NULL
                }
            }
            char[] __size = [0 <repeats 40 times>]
            long int __align = 0
        }
        ma_thread[] jobThreads = [135364195866304, 0 <repeats 63 times>]
        ma_job_queue jobQueue = {
            ma_uint32 flags = 0
            ma_uint32 capacity = 1024
            ma_uint64 head = 4294967296
            ma_uint64 tail = 4294967296
            ma_semaphore sem = {
                int value = 0
                ma_pthread_mutex_t lock = <union> {
                    __pthread_mutex_s __data = {
                        int __lock = 0
                        unsigned int __count = 0
                        int __owner = 0
                        unsigned int __nusers = 1
                        int __kind = 0
                        short int __spins = 0
                        short int __elision = 0
                        __pthread_list_t __list = {
                            __pthread_internal_list *__prev = NULL
                            __pthread_internal_list *__next = NULL
                        }
                    }
                    char[] __size = [0 <repeats 12 times>, 1, 0 <repeats 27 times>]
                    long int __align = 0
                }
                ma_pthread_cond_t cond = <union> {
                    __pthread_cond_s __data = {
                        __atomic_wide_counter __wseq = <union> {
                            long long unsigned int __value64 = 2
                            struct __value32 = {
                                unsigned int __low = 2
                                unsigned int __high = 0
                            }
                        }
                        __atomic_wide_counter __g1_start = <union> {
                            long long unsigned int __value64 = 0
                            struct __value32 = {
                                unsigned int __low = 0
                                unsigned int __high = 0
                            }
                        }
                        unsigned int[] __g_size = [0, 0]
                        unsigned int __g1_orig_size = 0
                        unsigned int __wrefs = 8
                        unsigned int[] __g_signals = [0, 0]
                        unsigned int __unused_initialized_1 = 0
                        unsigned int __unused_initialized_2 = 0
                    }
                    char[] __size = [2, 0 <repeats 27 times>, 8, 0 <repeats 19 times>]
                    long long int __align = 2
                }
            }
            ma_slot_allocator allocator = {
                ma_slot_allocator_group *pGroups = 0x7e4cfbfe0800 ({
                    ma_uint32 bitfield = 1
                })
                ma_uint32 *pSlots = 0x7e4cfbfe0880 (1)
                ma_uint32 count = 1
                ma_uint32 capacity = 1024
                ma_bool32 _ownsHeap = 0
                void *_pHeap = 0x7e4cfbfe0800
            }
            ma_job *pJobs = 0x7e4cfbfe1880 ({
                union toc = <union> {
                    struct breakup = {
                        ma_uint16 code = 0
                        ma_uint16 slot = 0
                        ma_uint32 refcount = 0
                    }
                    ma_uint64 allocation = 0
                }
                ma_uint64 next = 18446744073709551615
                ma_uint32 order = 0
                union data = <union> {
                    struct custom = {
                        ma_result(ma_job *) proc = NULL
                        ma_uintptr data0 = 0
                        ma_uintptr data1 = 0
                    }
                    union resourceManager = <union> {
                        struct loadDataBufferNode = {
                            void *pResourceManager = NULL
                            void *pDataBufferNode = NULL
                            char *pFilePath = NULL
                            wchar_t *pFilePathW = NULL
                            ma_uint32 flags = 0
                            ma_async_notification *pInitNotification = NULL
                            ma_async_notification *pDoneNotification = NULL
                            ma_fence *pInitFence = NULL
                            ma_fence *pDoneFence = NULL
                        }
                        struct freeDataBufferNode = {
                            void *pResourceManager = NULL
                            void *pDataBufferNode = NULL
                            ma_async_notification *pDoneNotification = NULL
                            ma_fence *pDoneFence = NULL
                        }
                        struct pageDataBufferNode = {
                            void *pResourceManager = NULL
                            void *pDataBufferNode = NULL
                            void *pDecoder = NULL
                            ma_async_notification *pDoneNotification = NULL
                            ma_fence *pDoneFence = NULL
                        }
                        struct loadDataBuffer = {
                            void *pDataBuffer = NULL
                            ma_async_notification *pInitNotification = NULL
                            ma_async_notification *pDoneNotification = NULL
                            ma_fence *pInitFence = NULL
                            ma_fence *pDoneFence = NULL
                            ma_uint64 rangeBegInPCMFrames = 0
                            ma_uint64 rangeEndInPCMFrames = 0
                            ma_uint64 loopPointBegInPCMFrames = 0
                            ma_uint64 loopPointEndInPCMFrames = 0
                            ma_uint32 isLooping = 0
                        }
                        struct freeDataBuffer = {
                            void *pDataBuffer = NULL
                            ma_async_notification *pDoneNotification = NULL
                            ma_fence *pDoneFence = NULL
                        }
                        struct loadDataStream = {
                            void *pDataStream = NULL
                            char *pFilePath = NULL
                            wchar_t *pFilePathW = NULL
                            ma_uint64 initialSeekPoint = 0
                            ma_async_notification *pInitNotification = NULL
                            ma_fence *pInitFence = NULL
                        }
                        struct freeDataStream = {
                            void *pDataStream = NULL
                            ma_async_notification *pDoneNotification = NULL
                            ma_fence *pDoneFence = NULL
                        }
                        struct pageDataStream = {
                            void *pDataStream = NULL
                            ma_uint32 pageIndex = 0
                        }
                        struct seekDataStream = {
                            void *pDataStream = NULL
                            ma_uint64 frameIndex = 0
                        }
                    }
                    union device = <union> {
                        union aaudio = <union> {
                            struct reroute = {
                                void *pDevice = NULL
                                ma_uint32 deviceType = 0
                            }
                        }
                    }
                }
            })
            ma_spinlock lock = 0
            void *_pHeap = 0x7e4cfbfe0800
            ma_bool32 _ownsHeap = 1
        }
        ma_default_vfs defaultVFS = {
            ma_vfs_callbacks cb = {
                ma_result(ma_vfs *, const char *, ma_uint32, void **) onOpen = 0x5616b4806540 (ma_result ma_default_vfs_open(ma_vfs *pVFS, const char *pFilePath, ma_uint32 openMode, void **pFile))
                ma_result(ma_vfs *, const wchar_t *, ma_uint32, void **) onOpenW = 0x5616b48b6a10 (ma_result ma_default_vfs_open_w(ma_vfs *pVFS, const wchar_t *pFilePath, ma_uint32 openMode, void **pFile))
                ma_result(ma_vfs *, void *) onClose = 0x5616b47b4b40 (ma_result ma_default_vfs_close(ma_vfs *pVFS, void *file))
                ma_result(ma_vfs *, void *, void *, size_t, size_t *) onRead = 0x5616b47b4980 (ma_result ma_default_vfs_read(ma_vfs *pVFS, void *file, void *pDst, size_t sizeInBytes, size_t *pBytesRead))
                ma_result(ma_vfs *, void *, const void *, size_t, size_t *) onWrite = 0x5616b47b47e0 (ma_result ma_default_vfs_write(ma_vfs *pVFS, void *file, const void *pSrc, size_t sizeInBytes, size_t *pBytesWritten))
                ma_result(ma_vfs *, void *, ma_int64, ma_seek_origin) onSeek = 0x5616b47b53e0 (ma_result ma_default_vfs_seek(ma_vfs *pVFS, void *file, ma_int64 offset, ma_seek_origin origin))
                ma_result(ma_vfs *, void *, ma_int64 *) onTell = 0x5616b477fd60 (ma_result ma_default_vfs_tell(ma_vfs *pVFS, void *file, ma_int64 *pCursor))
                ma_result(ma_vfs *, void *, ma_file_info *) onInfo = 0x5616b47b4560 (ma_result ma_default_vfs_info(ma_vfs *pVFS, void *file, ma_file_info *pInfo))
            }
            ma_allocation_callbacks allocationCallbacks = {
                void *pUserData = NULL
                void *(size_t, void *) onMalloc = 0x5616b47026d0 (void *ma__malloc_default(size_t sz, void *pUserData))
                void *(void *, size_t, void *) onRealloc = 0x5616b47026c0 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
                void(void *, void *) onFree = 0x5616b47026b0 (void ma__free_default(void *p, void *pUserData))
            }
        }
        ma_log log = {
            ma_log_callback[] callbacks = [
                {
                    void(void *, ma_uint32, const char *) onLog = NULL
                    void *pUserData = NULL
                } <repeats 4 times>
            ]
            ma_uint32 callbackCount = 0
            ma_allocation_callbacks allocationCallbacks = {
                void *pUserData = NULL
                void *(size_t, void *) onMalloc = NULL
                void *(void *, size_t, void *) onRealloc = NULL
                void(void *, void *) onFree = NULL
            }
            ma_mutex lock = <union> {
                __pthread_mutex_s __data = {
                    int __lock = 0
                    unsigned int __count = 0
                    int __owner = 0
                    unsigned int __nusers = 0
                    int __kind = 0
                    short int __spins = 0
                    short int __elision = 0
                    __pthread_list_t __list = {
                        __pthread_internal_list *__prev = NULL
                        __pthread_internal_list *__next = NULL
                    }
                }
                char[] __size = [0 <repeats 40 times>]
                long int __align = 0
            }
        }
    })
    ma_device *pDevice = 0x7d1cfbfe0080 ({
        ma_context *pContext = 0x7cacfbfe0080 ({
            ma_backend_callbacks callbacks = {
                ma_result(ma_context *, const ma_context_config *, ma_backend_callbacks *) onContextInit = 0x5616b48c5dd0 (ma_result ma_context_init__pulse(ma_context *pContext, const ma_context_config *pConfig, ma_backend_callbacks *pCallbacks))
                ma_result(ma_context *) onContextUninit = 0x5616b47ce780 (ma_result ma_context_uninit__pulse(ma_context *pContext))
                ma_result(ma_context *, ma_bool32(ma_context *, ma_device_type, const ma_device_info *, void *), void *) onContextEnumerateDevices = 0x5616b47c5290 (ma_result ma_context_enumerate_devices__pulse(ma_context *pContext, ma_bool32(ma_context *, ma_device_type, const ma_device_info *, void *) callback, void *pUserData))
                ma_result(ma_context *, ma_device_type, const ma_device_id *, ma_device_info *) onContextGetDeviceInfo = 0x5616b47c6290 (ma_result ma_context_get_device_info__pulse(ma_context *pContext, ma_device_type deviceType, const ma_device_id *pDeviceID, ma_device_info *pDeviceInfo))
                ma_result(ma_device *, const ma_device_config *, ma_device_descriptor *, ma_device_descriptor *) onDeviceInit = 0x5616b4901630 (ma_result ma_device_init__pulse(ma_device *pDevice, const ma_device_config *pConfig, ma_device_descriptor *pDescriptorPlayback, ma_device_descriptor *pDescriptorCapture))
                ma_result(ma_device *) onDeviceUninit = 0x5616b48a6de0 (ma_result ma_device_uninit__pulse(ma_device *pDevice))
                ma_result(ma_device *) onDeviceStart = 0x5616b48bef90 (ma_result ma_device_start__pulse(ma_device *pDevice))
                ma_result(ma_device *) onDeviceStop = 0x5616b48bee80 (ma_result ma_device_stop__pulse(ma_device *pDevice))
                ma_result(ma_device *, void *, ma_uint32, ma_uint32 *) onDeviceRead = NULL
                ma_result(ma_device *, const void *, ma_uint32, ma_uint32 *) onDeviceWrite = NULL
                ma_result(ma_device *) onDeviceDataLoop = 0x5616b47c0f70 (ma_result ma_device_data_loop__pulse(ma_device *pDevice))
                ma_result(ma_device *) onDeviceDataLoopWakeup = 0x5616b47349a0 (ma_result ma_device_data_loop_wakeup__pulse(ma_device *pDevice))
                ma_result(ma_device *, ma_device_type, ma_device_info *) onDeviceGetInfo = NULL
            }
            ma_backend backend = ma_backend_pulseaudio (7)
            ma_log *pLog = 0x7cacfbfe00f8 (<points to #5>)
            ma_log log = <points to #5>
            ma_thread_priority threadPriority = ma_thread_priority_highest (0)
            size_t threadStackSize = 0
            void *pUserData = NULL
            ma_allocation_callbacks allocationCallbacks = {
                void *pUserData = NULL
                void *(size_t, void *) onMalloc = 0x5616b47026d0 (void *ma__malloc_default(size_t sz, void *pUserData))
                void *(void *, size_t, void *) onRealloc = 0x5616b47026c0 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
                void(void *, void *) onFree = 0x5616b47026b0 (void ma__free_default(void *p, void *pUserData))
            }
            ma_mutex deviceEnumLock = <union> {
                __pthread_mutex_s __data = {
                    int __lock = 0
                    unsigned int __count = 0
                    int __owner = 0
                    unsigned int __nusers = 0
                    int __kind = 0
                    short int __spins = 0
                    short int __elision = 0
                    __pthread_list_t __list = {
                        __pthread_internal_list *__prev = NULL
                        __pthread_internal_list *__next = NULL
                    }
                }
                char[] __size = [0 <repeats 40 times>]
                long int __align = 0
            }
            ma_mutex deviceInfoLock = <union> {
                __pthread_mutex_s __data = {
                    int __lock = 0
                    unsigned int __count = 0
                    int __owner = 0
                    unsigned int __nusers = 0
                    int __kind = 0
                    short int __spins = 0
                    short int __elision = 0
                    __pthread_list_t __list = {
                        __pthread_internal_list *__prev = NULL
                        __pthread_internal_list *__next = NULL
                    }
                }
                char[] __size = [0 <repeats 40 times>]
                long int __align = 0
            }
            ma_uint32 deviceInfoCapacity = 0
            ma_uint32 playbackDeviceInfoCount = 0
            ma_uint32 captureDeviceInfoCount = 0
            ma_device_info *pDeviceInfos = NULL
            union <anonymous> = <union> {
                struct alsa = {
                    void *asoundSO = 0x7cbcfbfe0080
                    void() snd_pcm_open = 0x7f1cfd108820
                    void() snd_pcm_close = 0x7f1cfd1060a0
                    void() snd_pcm_hw_params_sizeof = 0x7f1cfd106870
                    void() snd_pcm_hw_params_any = 0x7f1cfd1069e0
                    void() snd_pcm_hw_params_set_format = 0x7f1cfd110790
                    void() snd_pcm_hw_params_set_format_first = 0x7f1cfd106150
                    void() snd_pcm_hw_params_get_format_mask = 0x7f1cfd121c60
                    void() snd_pcm_hw_params_set_channels = 0x7f1cfd121df0
                    void() snd_pcm_hw_params_set_channels_near = 0x7f1cfd11dea0
                    void() snd_pcm_hw_params_set_channels_minmax = 0x7f1cfd121cf0
                    void() snd_pcm_hw_params_set_rate_resample = 0x7f1cfd11df90
                    void() snd_pcm_hw_params_set_rate = 0x7f1cfd11e060
                    void() snd_pcm_hw_params_set_rate_near = 0x7f1cfd11e1e0
                    void() snd_pcm_hw_params_set_buffer_size_near = 0x7f1cfd11e130
                    void() snd_pcm_hw_params_set_periods_near = 0x7f1cfd11e310
                    void() snd_pcm_hw_params_set_access = 0x7f1cfd121f10
                    void() snd_pcm_hw_params_get_format = 0x7f1cfd121f80
                    void() snd_pcm_hw_params_get_channels = 0x7f1cfd11e430
                    void() snd_pcm_hw_params_get_channels_min = 0x7f1cfd120260
                    void() snd_pcm_hw_params_get_channels_max = 0x7f1cfd0fac10
                    void() snd_pcm_hw_params_get_rate = 0x7f1cfd0fac20
                    void() snd_pcm_hw_params_get_rate_min = 0x7f1cfd0fe9b0
                    void() snd_pcm_hw_params_get_rate_max = 0x7f1cfd0fc400
                    void() snd_pcm_hw_params_get_buffer_size = 0x7f1cfd0f6e30
                    void() snd_pcm_hw_params_get_periods = 0x7f1cfd0f6d00
                    void() snd_pcm_hw_params_get_access = 0x7f1cfd105640
                    void() snd_pcm_hw_params_test_format = 0x7f1cfd105660
                    void() snd_pcm_hw_params_test_channels = 0x7f1cfd109890
                    void() snd_pcm_hw_params_test_rate = 0x7f1cfd10a0b0
                    void() snd_pcm_hw_params = 0x7f1cfd1051a0
                    void() snd_pcm_sw_params_sizeof = 0x7f1cfd107070
                    void() snd_pcm_sw_params_current = 0x7f1cfd0f8220
                    void() snd_pcm_sw_params_get_boundary = 0x7f1cfd0f83a0
                    void() snd_pcm_sw_params_set_avail_min = 0x7f1cfd0f9890
                    void() snd_pcm_sw_params_set_start_threshold = 0x7f1cfd116930
                    void() snd_pcm_sw_params_set_stop_threshold = 0x7f1cfd116c30
                    void() snd_pcm_sw_params = 0x7f1cfd118a50
                    void() snd_pcm_format_mask_sizeof = 0x7f1cfd118b20
                    void() snd_pcm_format_mask_test = 0x7f1cfd116fc0
                    void() snd_pcm_get_chmap = 0x7f1cfd1121c0
                    void() snd_pcm_state = 0x7f1cfd11d250
                    void() snd_pcm_prepare = 0x7f1cfd11d330
                    void() snd_pcm_start = 0x7f1cfd11d520
                    void() snd_pcm_drop = 0x7f1cfd120460
                    void() snd_pcm_drain = 0x7f1cfd11d780
                    void() snd_pcm_reset = 0x7f1cfd113360
                    void() snd_device_name_hint = 0x7f1cfd113270
                    void() snd_device_name_get_hint = 0x7f1cfd113900
                    void() snd_card_get_index = 0x7f1cfd113810
                    void() snd_device_name_free_hint = 0x7f1cfd11d8e0
                    void() snd_pcm_mmap_begin = 0x7f1cfd11bd20
                    void() snd_pcm_mmap_commit = 0x7f1cfd11b900
                    void() snd_pcm_recover = 0x7f1cfd11da20
                    void() snd_pcm_readi = 0x7f1cfd11bae0
                    void() snd_pcm_writei = 0x7f1cfd11c090
                    void() snd_pcm_avail = 0x7f1cfd1124a0
                    void() snd_pcm_avail_update = 0x7f1cfd11b8e0
                    void() snd_pcm_wait = 0x7f1cfd112870
                    void() snd_pcm_nonblock = 0x7f1cfd112b10
                    void() snd_pcm_info = 0x7f1cfd112d10
                    void() snd_pcm_info_sizeof = 0x7f1cfd112e30
                    void() snd_pcm_info_get_name = 0x7c2cfbfe0540
                    void() snd_pcm_poll_descriptors = 0x7c4cfbfe03c0
                    void() snd_pcm_poll_descriptors_count = NULL
                    void() snd_pcm_poll_descriptors_revents = NULL
                    void() snd_config_update_free_global = NULL
                    ma_mutex internalDeviceEnumLock = <union> {
                        __pthread_mutex_s __data = {
                            int __lock = 0
                            unsigned int __count = 0
                            int __owner = 0
                            unsigned int __nusers = 0
                            int __kind = 0
                            short int __spins = 0
                            short int __elision = 0
                            __pthread_list_t __list = {
                                __pthread_internal_list *__prev = NULL
                                __pthread_internal_list *__next = NULL
                            }
                        }
                        char[] __size = [0 <repeats 40 times>]
                        long int __align = 0
                    }
                    ma_bool32 useVerboseDeviceEnumeration = 0
                }
                struct pulse = {
                    void *pulseSO = 0x7cbcfbfe0080
                    void() pa_mainloop_new = 0x7f1cfd108820
                    void() pa_mainloop_free = 0x7f1cfd1060a0
                    void() pa_mainloop_quit = 0x7f1cfd106870
                    void() pa_mainloop_get_api = 0x7f1cfd1069e0
                    void() pa_mainloop_iterate = 0x7f1cfd110790
                    void() pa_mainloop_wakeup = 0x7f1cfd106150
                    void() pa_threaded_mainloop_new = 0x7f1cfd121c60
                    void() pa_threaded_mainloop_free = 0x7f1cfd121df0
                    void() pa_threaded_mainloop_start = 0x7f1cfd11dea0
                    void() pa_threaded_mainloop_stop = 0x7f1cfd121cf0
                    void() pa_threaded_mainloop_lock = 0x7f1cfd11df90
                    void() pa_threaded_mainloop_unlock = 0x7f1cfd11e060
                    void() pa_threaded_mainloop_wait = 0x7f1cfd11e1e0
                    void() pa_threaded_mainloop_signal = 0x7f1cfd11e130
                    void() pa_threaded_mainloop_accept = 0x7f1cfd11e310
                    void() pa_threaded_mainloop_get_retval = 0x7f1cfd121f10
                    void() pa_threaded_mainloop_get_api = 0x7f1cfd121f80
                    void() pa_threaded_mainloop_in_thread = 0x7f1cfd11e430
                    void() pa_threaded_mainloop_set_name = 0x7f1cfd120260
                    void() pa_context_new = 0x7f1cfd0fac10
                    void() pa_context_unref = 0x7f1cfd0fac20
                    void() pa_context_connect = 0x7f1cfd0fe9b0
                    void() pa_context_disconnect = 0x7f1cfd0fc400
                    void() pa_context_set_state_callback = 0x7f1cfd0f6e30
                    void() pa_context_get_state = 0x7f1cfd0f6d00
                    void() pa_context_get_sink_info_list = 0x7f1cfd105640
                    void() pa_context_get_source_info_list = 0x7f1cfd105660
                    void() pa_context_get_sink_info_by_name = 0x7f1cfd109890
                    void() pa_context_get_source_info_by_name = 0x7f1cfd10a0b0
                    void() pa_operation_unref = 0x7f1cfd1051a0
                    void() pa_operation_get_state = 0x7f1cfd107070
                    void() pa_channel_map_init_extend = 0x7f1cfd0f8220
                    void() pa_channel_map_valid = 0x7f1cfd0f83a0
                    void() pa_channel_map_compatible = 0x7f1cfd0f9890
                    void() pa_stream_new = 0x7f1cfd116930
                    void() pa_stream_unref = 0x7f1cfd116c30
                    void() pa_stream_connect_playback = 0x7f1cfd118a50
                    void() pa_stream_connect_record = 0x7f1cfd118b20
                    void() pa_stream_disconnect = 0x7f1cfd116fc0
                    void() pa_stream_get_state = 0x7f1cfd1121c0
                    void() pa_stream_get_sample_spec = 0x7f1cfd11d250
                    void() pa_stream_get_channel_map = 0x7f1cfd11d330
                    void() pa_stream_get_buffer_attr = 0x7f1cfd11d520
                    void() pa_stream_set_buffer_attr = 0x7f1cfd120460
                    void() pa_stream_get_device_name = 0x7f1cfd11d780
                    void() pa_stream_set_write_callback = 0x7f1cfd113360
                    void() pa_stream_set_read_callback = 0x7f1cfd113270
                    void() pa_stream_set_suspended_callback = 0x7f1cfd113900
                    void() pa_stream_set_moved_callback = 0x7f1cfd113810
                    void() pa_stream_is_suspended = 0x7f1cfd11d8e0
                    void() pa_stream_flush = 0x7f1cfd11bd20
                    void() pa_stream_drain = 0x7f1cfd11b900
                    void() pa_stream_is_corked = 0x7f1cfd11da20
                    void() pa_stream_cork = 0x7f1cfd11bae0
                    void() pa_stream_trigger = 0x7f1cfd11c090
                    void() pa_stream_begin_write = 0x7f1cfd1124a0
                    void() pa_stream_write = 0x7f1cfd11b8e0
                    void() pa_stream_peek = 0x7f1cfd112870
                    void() pa_stream_drop = 0x7f1cfd112b10
                    void() pa_stream_writable_size = 0x7f1cfd112d10
                    void() pa_stream_readable_size = 0x7f1cfd112e30
                    void *pMainLoop = 0x7c2cfbfe0540
                    void *pPulseContext = 0x7c4cfbfe03c0
                    char *pApplicationName = NULL
                    char *pServerName = NULL
                }
                struct jack = {
                    void *jackSO = 0x7cbcfbfe0080
                    void() jack_client_open = 0x7f1cfd108820
                    void() jack_client_close = 0x7f1cfd1060a0
                    void() jack_client_name_size = 0x7f1cfd106870
                    void() jack_set_process_callback = 0x7f1cfd1069e0
                    void() jack_set_buffer_size_callback = 0x7f1cfd110790
                    void() jack_on_shutdown = 0x7f1cfd106150
                    void() jack_get_sample_rate = 0x7f1cfd121c60
                    void() jack_get_buffer_size = 0x7f1cfd121df0
                    void() jack_get_ports = 0x7f1cfd11dea0
                    void() jack_activate = 0x7f1cfd121cf0
                    void() jack_deactivate = 0x7f1cfd11df90
                    void() jack_connect = 0x7f1cfd11e060
                    void() jack_port_register = 0x7f1cfd11e1e0
                    void() jack_port_name = 0x7f1cfd11e130
                    void() jack_port_get_buffer = 0x7f1cfd11e310
                    void() jack_free = 0x7f1cfd121f10
                    char *pClientName = 0x7f1cfd121f80 ("óúH…ÿtH‹?ÿ%¶l")
                    ma_bool32 tryStartServer = 4245808176
                }
                struct null_backend = {
                    int _unused = -67239808
                }
            }
            union <anonymous> = <union> {
                struct posix = {
                    int _unused = 0
                }
                int _unused = 0
            }
        })
        ma_device_type type = ma_device_type_playback (1)
        ma_uint32 sampleRate = 48000
        ma_atomic_device_state state = {
            ma_device_state value = ma_device_state_started (2)
        }
        void(ma_device *, void *, const void *, ma_uint32) onData = 0x5616b4981dc0 (void ma_engine_data_callback_internal(ma_device *pDevice, void *pFramesOut, const void *pFramesIn, ma_uint32 frameCount))
        void(const ma_device_notification *) onNotification = NULL
        void(ma_device *) onStop = NULL
        void *pUserData = 0x7b1cfb000020
        ma_mutex startStopLock = <union> {
            __pthread_mutex_s __data = {
                int __lock = 0
                unsigned int __count = 0
                int __owner = 0
                unsigned int __nusers = 0
                int __kind = 0
                short int __spins = 0
                short int __elision = 0
                __pthread_list_t __list = {
                    __pthread_internal_list *__prev = NULL
                    __pthread_internal_list *__next = NULL
                }
            }
            char[] __size = [0 <repeats 40 times>]
            long int __align = 0
        }
        ma_event wakeupEvent = {
            ma_uint32 value = 0
            ma_pthread_mutex_t lock = <union> {
                __pthread_mutex_s __data = {
                    int __lock = 0
                    unsigned int __count = 0
                    int __owner = 0
                    unsigned int __nusers = 0
                    int __kind = 0
                    short int __spins = 0
                    short int __elision = 0
                    __pthread_list_t __list = {
                        __pthread_internal_list *__prev = NULL
                        __pthread_internal_list *__next = NULL
                    }
                }
                char[] __size = [0 <repeats 40 times>]
                long int __align = 0
            }
            ma_pthread_cond_t cond = <union> {
                __pthread_cond_s __data = {
                    __atomic_wide_counter __wseq = <union> {
                        long long unsigned int __value64 = 3
                        struct __value32 = {
                            unsigned int __low = 3
                            unsigned int __high = 0
                        }
                    }
                    __atomic_wide_counter __g1_start = <union> {
                        long long unsigned int __value64 = 0
                        struct __value32 = {
                            unsigned int __low = 0
                            unsigned int __high = 0
                        }
                    }
                    unsigned int[] __g_size = [0, 0]
                    unsigned int __g1_orig_size = 4
                    unsigned int __wrefs = 0
                    unsigned int[] __g_signals = [0, 0]
                    unsigned int __unused_initialized_1 = 0
                    unsigned int __unused_initialized_2 = 0
                }
                char[] __size = [3, 0 <repeats 23 times>, 4, 0 <repeats 23 times>]
                long long int __align = 3
            }
        }
        ma_event startEvent = {
            ma_uint32 value = 0
            ma_pthread_mutex_t lock = <union> {
                __pthread_mutex_s __data = {
                    int __lock = 0
                    unsigned int __count = 0
                    int __owner = 0
                    unsigned int __nusers = 0
                    int __kind = 0
                    short int __spins = 0
                    short int __elision = 0
                    __pthread_list_t __list = {
                        __pthread_internal_list *__prev = NULL
                        __pthread_internal_list *__next = NULL
                    }
                }
                char[] __size = [0 <repeats 40 times>]
                long int __align = 0
            }
            ma_pthread_cond_t cond = <union> {
                __pthread_cond_s __data = {
                    __atomic_wide_counter __wseq = <union> {
                        long long unsigned int __value64 = 3
                        struct __value32 = {
                            unsigned int __low = 3
                            unsigned int __high = 0
                        }
                    }
                    __atomic_wide_counter __g1_start = <union> {
                        long long unsigned int __value64 = 0
                        struct __value32 = {
                            unsigned int __low = 0
                            unsigned int __high = 0
                        }
                    }
                    unsigned int[] __g_size = [0, 0]
                    unsigned int __g1_orig_size = 4
                    unsigned int __wrefs = 0
                    unsigned int[] __g_signals = [0, 0]
                    unsigned int __unused_initialized_1 = 0
                    unsigned int __unused_initialized_2 = 0
                }
                char[] __size = [3, 0 <repeats 23 times>, 4, 0 <repeats 23 times>]
                long long int __align = 3
            }
        }
        ma_event stopEvent = {
            ma_uint32 value = 0
            ma_pthread_mutex_t lock = <union> {
                __pthread_mutex_s __data = {
                    int __lock = 0
                    unsigned int __count = 0
                    int __owner = 0
                    unsigned int __nusers = 0
                    int __kind = 0
                    short int __spins = 0
                    short int __elision = 0
                    __pthread_list_t __list = {
                        __pthread_internal_list *__prev = NULL
                        __pthread_internal_list *__next = NULL
                    }
                }
                char[] __size = [0 <repeats 40 times>]
                long int __align = 0
            }
            ma_pthread_cond_t cond = <union> {
                __pthread_cond_s __data = {
                    __atomic_wide_counter __wseq = <union> {
                        long long unsigned int __value64 = 3
                        struct __value32 = {
                            unsigned int __low = 3
                            unsigned int __high = 0
                        }
                    }
                    __atomic_wide_counter __g1_start = <union> {
                        long long unsigned int __value64 = 0
                        struct __value32 = {
                            unsigned int __low = 0
                            unsigned int __high = 0
                        }
                    }
                    unsigned int[] __g_size = [0, 0]
                    unsigned int __g1_orig_size = 4
                    unsigned int __wrefs = 0
                    unsigned int[] __g_signals = [0, 0]
                    unsigned int __unused_initialized_1 = 0
                    unsigned int __unused_initialized_2 = 0
                }
                char[] __size = [3, 0 <repeats 23 times>, 4, 0 <repeats 23 times>]
                long long int __align = 3
            }
        }
        ma_thread thread = 135364215830208
        ma_result workResult = MA_SUCCESS (0)
        ma_bool8 isOwnerOfContext = 1
        ma_bool8 noPreSilencedOutputBuffer = 1
        ma_bool8 noClip = 1
        ma_bool8 noDisableDenormals = 0
        ma_bool8 noFixedSizedCallback = 0
        ma_atomic_float masterVolumeFactor = {
            ma_float value = 1.000000
        }
        ma_duplex_rb duplexRB = {
            ma_pcm_rb rb = {
                ma_data_source_base ds = {
                    const ma_data_source_vtable *vtable = NULL
                    ma_uint64 rangeBegInFrames = 0
                    ma_uint64 rangeEndInFrames = 0
                    ma_uint64 loopBegInFrames = 0
                    ma_uint64 loopEndInFrames = 0
                    ma_data_source *pCurrent = NULL
                    ma_data_source *pNext = NULL
                    ma_data_source *(ma_data_source *) onGetNext = NULL
                    ma_bool32 isLooping = 0
                }
                ma_rb rb = {
                    void *pBuffer = NULL
                    ma_uint32 subbufferSizeInBytes = 0
                    ma_uint32 subbufferCount = 0
                    ma_uint32 subbufferStrideInBytes = 0
                    ma_uint32 encodedReadOffset = 0
                    ma_uint32 encodedWriteOffset = 0
                    ma_bool8 ownsBuffer = 0
                    ma_bool8 clearOnWriteAcquire = 0
                    ma_allocation_callbacks allocationCallbacks = {
                        void *pUserData = NULL
                        void *(size_t, void *) onMalloc = NULL
                        void *(void *, size_t, void *) onRealloc = NULL
                        void(void *, void *) onFree = NULL
                    }
                }
                ma_format format = ma_format_unknown (0)
                ma_uint32 channels = 0
                ma_uint32 sampleRate = 0
            }
        }
        struct resampling = {
            ma_resample_algorithm algorithm = ma_resample_algorithm_linear (0)
            ma_resampling_backend_vtable *pBackendVTable = NULL
            void *pBackendUserData = NULL
            struct linear = {
                ma_uint32 lpfOrder = 4
            }
        }
        struct playback = {
            ma_device_id *pID = NULL
            ma_device_id id = <union> {
                ma_wchar_win32[] wasapi = [0 <repeats 64 times>]
                ma_uint8[] dsound = [0 <repeats 16 times>]
                ma_uint32 winmm = 0
                char[] alsa = [0 <repeats 256 times>]
                char[] pulse = [0 <repeats 256 times>]
                int jack = 0
                char[] coreaudio = [0 <repeats 256 times>]
                char[] sndio = [0 <repeats 256 times>]
                char[] audio4 = [0 <repeats 256 times>]
                char[] oss = [0 <repeats 64 times>]
                ma_int32 aaudio = 0
                ma_uint32 opensl = 0
                char[] webaudio = [0 <repeats 32 times>]
                union custom = <union> {
                    int i = 0
                    char[] s = [0 <repeats 256 times>]
                    void *p = NULL
                }
                int nullbackend = 0
            }
            char[] name = [70 ('F'), 97 ('a'), 109 ('m'), 105 ('i'), 108 ('l'), 121 ('y'), 32 (' '), 49 ('1'), 55 ('7'), 104 ('h'), 47 ('/'), 49 ('1'), 57 ('9'), 104 ('h'), 47 ('/'), 49 ('1'), 97 ('a'), 104 ('h'), 32 (' '), 72 ('H'), 68 ('D'), 32 (' '), 65 ('A'), 117 ('u'), 100 ('d'), 105 ('i'), 111 ('o'), 32 (' '), 67 ('C'), 111 ('o'), 110 ('n'), 116 ('t'), 114 ('r'), 111 ('o'), 108 ('l'), 108 ('l'), 101 ('e'), 114 ('r'), 32 (' '), 72 ('H'), 101 ('e'), 97 ('a'), 100 ('d'), 112 ('p'), 104 ('h'), 111 ('o'), 110 ('n'), 101 ('e'), 115 ('s'), 0 <repeats 207 times>]
            ma_share_mode shareMode = ma_share_mode_shared (0)
            ma_format format = ma_format_f32 (5)
            ma_uint32 channels = 2
            ma_channel[] channelMap = [2, 3, 0 <repeats 252 times>]
            ma_format internalFormat = ma_format_s32 (4)
            ma_uint32 internalChannels = 2
            ma_uint32 internalSampleRate = 48000
            ma_channel[] internalChannelMap = [2, 3, 0 <repeats 252 times>]
            ma_uint32 internalPeriodSizeInFrames = 900
            ma_uint32 internalPeriods = 4
            ma_channel_mix_mode channelMixMode = ma_channel_mix_mode_rectangular (0)
            ma_bool32 calculateLFEFromSpatialChannels = 0
            ma_data_converter converter = {
                ma_format formatIn = ma_format_f32 (5)
                ma_format formatOut = ma_format_s32 (4)
                ma_uint32 channelsIn = 2
                ma_uint32 channelsOut = 2
                ma_uint32 sampleRateIn = 48000
                ma_uint32 sampleRateOut = 48000
                ma_dither_mode ditherMode = ma_dither_mode_none (0)
                ma_data_converter_execution_path executionPath = ma_data_converter_execution_path_format_only (1)
                ma_channel_converter channelConverter = {
                    ma_format format = ma_format_f32 (5)
                    ma_uint32 channelsIn = 2
                    ma_uint32 channelsOut = 2
                    ma_channel_mix_mode mixingMode = ma_channel_mix_mode_rectangular (0)
                    ma_channel_conversion_path conversionPath = ma_channel_conversion_path_passthrough (1)
                    ma_channel *pChannelMapIn = 0x7b3cfbfe2910 ("")
                    ma_channel *pChannelMapOut = 0x7b3cfbfe2912 ("")
                    ma_uint8 *pShuffleTable = NULL
                    union weights = <union> {
                        float **f32 = NULL
                        ma_int32 **s16 = NULL
                    }
                    void *_pHeap = 0x7b3cfbfe2910
                    ma_bool32 _ownsHeap = 0
                }
                ma_resampler resampler = {
                    ma_resampling_backend *pBackend = NULL
                    ma_resampling_backend_vtable *pBackendVTable = NULL
                    void *pBackendUserData = NULL
                    ma_format format = ma_format_unknown (0)
                    ma_uint32 channels = 0
                    ma_uint32 sampleRateIn = 0
                    ma_uint32 sampleRateOut = 0
                    union state = <union> {
                        ma_linear_resampler linear = {
                            ma_linear_resampler_config config = {
                                ma_format format = ma_format_unknown (0)
                                ma_uint32 channels = 0
                                ma_uint32 sampleRateIn = 0
                                ma_uint32 sampleRateOut = 0
                                ma_uint32 lpfOrder = 0
                                double lpfNyquistFactor = 0.000000
                            }
                            ma_uint32 inAdvanceInt = 0
                            ma_uint32 inAdvanceFrac = 0
                            ma_uint32 inTimeInt = 0
                            ma_uint32 inTimeFrac = 0
                            union x0 = <union> {
                                float *f32 = NULL
                                ma_int16 *s16 = NULL
                            }
                            union x1 = <union> {
                                float *f32 = NULL
                                ma_int16 *s16 = NULL
                            }
                            ma_lpf lpf = {
                                ma_format format = ma_format_unknown (0)
                                ma_uint32 channels = 0
                                ma_uint32 sampleRate = 0
                                ma_uint32 lpf1Count = 0
                                ma_uint32 lpf2Count = 0
                                ma_lpf1 *pLPF1 = NULL
                                ma_lpf2 *pLPF2 = NULL
                                void *_pHeap = NULL
                                ma_bool32 _ownsHeap = 0
                            }
                            void *_pHeap = NULL
                            ma_bool32 _ownsHeap = 0
                        }
                    }
                    void *_pHeap = NULL
                    ma_bool32 _ownsHeap = 0
                }
                ma_bool8 hasPreFormatConversion = 0
                ma_bool8 hasPostFormatConversion = 1
                ma_bool8 hasChannelConverter = 0
                ma_bool8 hasResampler = 0
                ma_bool8 isPassthrough = 0
                ma_bool8 _ownsHeap = 1
                void *_pHeap = 0x7b3cfbfe2910
            }
            void *pIntermediaryBuffer = 0x7d5cfbfe2100
            ma_uint32 intermediaryBufferCap = 900
            ma_uint32 intermediaryBufferLen = 900
            void *pInputCache = NULL
            ma_uint64 inputCacheCap = 0
            ma_uint64 inputCacheConsumed = 0
            ma_uint64 inputCacheRemaining = 0
        }
        struct capture = {
            ma_device_id *pID = NULL
            ma_device_id id = <union> {
                ma_wchar_win32[] wasapi = [0 <repeats 64 times>]
                ma_uint8[] dsound = [0 <repeats 16 times>]
                ma_uint32 winmm = 0
                char[] alsa = [0 <repeats 256 times>]
                char[] pulse = [0 <repeats 256 times>]
                int jack = 0
                char[] coreaudio = [0 <repeats 256 times>]
                char[] sndio = [0 <repeats 256 times>]
                char[] audio4 = [0 <repeats 256 times>]
                char[] oss = [0 <repeats 64 times>]
                ma_int32 aaudio = 0
                ma_uint32 opensl = 0
                char[] webaudio = [0 <repeats 32 times>]
                union custom = <union> {
                    int i = 0
                    char[] s = [0 <repeats 256 times>]
                    void *p = NULL
                }
                int nullbackend = 0
            }
            char[] name = [0 <repeats 256 times>]
            ma_share_mode shareMode = ma_share_mode_shared (0)
            ma_format format = ma_format_unknown (0)
            ma_uint32 channels = 0
            ma_channel[] channelMap = [0 <repeats 254 times>]
            ma_format internalFormat = ma_format_unknown (0)
            ma_uint32 internalChannels = 0
            ma_uint32 internalSampleRate = 0
            ma_channel[] internalChannelMap = [0 <repeats 254 times>]
            ma_uint32 internalPeriodSizeInFrames = 0
            ma_uint32 internalPeriods = 0
            ma_channel_mix_mode channelMixMode = ma_channel_mix_mode_rectangular (0)
            ma_bool32 calculateLFEFromSpatialChannels = 0
            ma_data_converter converter = {
                ma_format formatIn = ma_format_unknown (0)
                ma_format formatOut = ma_format_unknown (0)
                ma_uint32 channelsIn = 0
                ma_uint32 channelsOut = 0
                ma_uint32 sampleRateIn = 0
                ma_uint32 sampleRateOut = 0
                ma_dither_mode ditherMode = ma_dither_mode_none (0)
                ma_data_converter_execution_path executionPath = ma_data_converter_execution_path_passthrough (0)
                ma_channel_converter channelConverter = {
                    ma_format format = ma_format_unknown (0)
                    ma_uint32 channelsIn = 0
                    ma_uint32 channelsOut = 0
                    ma_channel_mix_mode mixingMode = ma_channel_mix_mode_rectangular (0)
                    ma_channel_conversion_path conversionPath = ma_channel_conversion_path_unknown (0)
                    ma_channel *pChannelMapIn = NULL
                    ma_channel *pChannelMapOut = NULL
                    ma_uint8 *pShuffleTable = NULL
                    union weights = <union> {
                        float **f32 = NULL
                        ma_int32 **s16 = NULL
                    }
                    void *_pHeap = NULL
                    ma_bool32 _ownsHeap = 0
                }
                ma_resampler resampler = {
                    ma_resampling_backend *pBackend = NULL
                    ma_resampling_backend_vtable *pBackendVTable = NULL
                    void *pBackendUserData = NULL
                    ma_format format = ma_format_unknown (0)
                    ma_uint32 channels = 0
                    ma_uint32 sampleRateIn = 0
                    ma_uint32 sampleRateOut = 0
                    union state = <union> {
                        ma_linear_resampler linear = {
                            ma_linear_resampler_config config = {
                                ma_format format = ma_format_unknown (0)
                                ma_uint32 channels = 0
                                ma_uint32 sampleRateIn = 0
                                ma_uint32 sampleRateOut = 0
                                ma_uint32 lpfOrder = 0
                                double lpfNyquistFactor = 0.000000
                            }
                            ma_uint32 inAdvanceInt = 0
                            ma_uint32 inAdvanceFrac = 0
                            ma_uint32 inTimeInt = 0
                            ma_uint32 inTimeFrac = 0
                            union x0 = <union> {
                                float *f32 = NULL
                                ma_int16 *s16 = NULL
                            }
                            union x1 = <union> {
                                float *f32 = NULL
                                ma_int16 *s16 = NULL
                            }
                            ma_lpf lpf = {
                                ma_format format = ma_format_unknown (0)
                                ma_uint32 channels = 0
                                ma_uint32 sampleRate = 0
                                ma_uint32 lpf1Count = 0
                                ma_uint32 lpf2Count = 0
                                ma_lpf1 *pLPF1 = NULL
                                ma_lpf2 *pLPF2 = NULL
                                void *_pHeap = NULL
                                ma_bool32 _ownsHeap = 0
                            }
                            void *_pHeap = NULL
                            ma_bool32 _ownsHeap = 0
                        }
                    }
                    void *_pHeap = NULL
                    ma_bool32 _ownsHeap = 0
                }
                ma_bool8 hasPreFormatConversion = 0
                ma_bool8 hasPostFormatConversion = 0
                ma_bool8 hasChannelConverter = 0
                ma_bool8 hasResampler = 0
                ma_bool8 isPassthrough = 0
                ma_bool8 _ownsHeap = 0
                void *_pHeap = NULL
            }
            void *pIntermediaryBuffer = NULL
            ma_uint32 intermediaryBufferCap = 0
            ma_uint32 intermediaryBufferLen = 0
        }
        union <anonymous> = <union> {
            struct alsa = {
                void *pPCMPlayback = 0x7c2cfbfe0a40
                void *pPCMCapture = 0x7c4cfbfe0ac0
                void *pPollDescriptorsPlayback = 0x7cccfbfe0780
                void *pPollDescriptorsCapture = NULL
                int pollDescriptorCountPlayback = 0
                int pollDescriptorCountCapture = 0
                int wakeupfdPlayback = 0
                int wakeupfdCapture = 0
                ma_bool8 isUsingMMapPlayback = 0
                ma_bool8 isUsingMMapCapture = 0
            }
            struct pulse = {
                void *pMainLoop = 0x7c2cfbfe0a40
                void *pPulseContext = 0x7c4cfbfe0ac0
                void *pStreamPlayback = 0x7cccfbfe0780
                void *pStreamCapture = NULL
            }
            struct jack = {
                void *pClient = 0x7c2cfbfe0a40
                void **ppPortsPlayback = 0x7c4cfbfe0ac0
                void **ppPortsCapture = 0x7cccfbfe0780
                float *pIntermediaryBufferPlayback = NULL
                float *pIntermediaryBufferCapture = NULL
            }
            struct null_device = {
                ma_thread deviceThread = 136532648135232
                ma_event operationEvent = {
                    ma_uint32 value = 4227730112
                    ma_pthread_mutex_t lock = <union> {
                        __pthread_mutex_s __data = {
                            int __lock = -67238016
                            unsigned int __count = 31948
                            int __owner = 0
                            unsigned int __nusers = 0
                            int __kind = 0
                            short int __spins = 0
                            short int __elision = 0
                            __pthread_list_t __list = {
                                __pthread_internal_list *__prev = NULL
                                __pthread_internal_list *__next = NULL
                            }
                        }
                        char[] __size = [-128, 7, -2, -5, -52, 124 ('|'), 0 <repeats 34 times>]
                        long int __align = 137219842901888
                    }
                    ma_pthread_cond_t cond = <union> {
                        __pthread_cond_s __data = {
                            __atomic_wide_counter __wseq = <union> {
                                long long unsigned int __value64 = 0
                                struct __value32 = {
                                    unsigned int __low = 0
                                    unsigned int __high = 0
                                }
                            }
                            __atomic_wide_counter __g1_start = <union> {
                                long long unsigned int __value64 = 0
                                struct __value32 = {
                                    unsigned int __low = 0
                                    unsigned int __high = 0
                                }
                            }
                            unsigned int[] __g_size = [0, 0]
                            unsigned int __g1_orig_size = 0
                            unsigned int __wrefs = 0
                            unsigned int[] __g_signals = [0, 0]
                            unsigned int __unused_initialized_1 = 0
                            unsigned int __unused_initialized_2 = 0
                        }
                        char[] __size = [0 <repeats 48 times>]
                        long long int __align = 0
                    }
                }
                ma_event operationCompletionEvent = {
                    ma_uint32 value = 0
                    ma_pthread_mutex_t lock = <union> {
                        __pthread_mutex_s __data = {
                            int __lock = 0
                            unsigned int __count = 0
                            int __owner = 0
                            unsigned int __nusers = 0
                            int __kind = 0
                            short int __spins = 0
                            short int __elision = 0
                            __pthread_list_t __list = {
                                __pthread_internal_list *__prev = NULL
                                __pthread_internal_list *__next = NULL
                            }
                        }
                        char[] __size = [0 <repeats 40 times>]
                        long int __align = 0
                    }
                    ma_pthread_cond_t cond = <union> {
                        __pthread_cond_s __data = {
                            __atomic_wide_counter __wseq = <union> {
                                long long unsigned int __value64 = 0
                                struct __value32 = {
                                    unsigned int __low = 0
                                    unsigned int __high = 0
                                }
                            }
                            __atomic_wide_counter __g1_start = <union> {
                                long long unsigned int __value64 = 0
                                struct __value32 = {
                                    unsigned int __low = 0
                                    unsigned int __high = 0
                                }
                            }
                            unsigned int[] __g_size = [0, 0]
                            unsigned int __g1_orig_size = 0
                            unsigned int __wrefs = 0
                            unsigned int[] __g_signals = [0, 0]
                            unsigned int __unused_initialized_1 = 0
                            unsigned int __unused_initialized_2 = 0
                        }
                        char[] __size = [0 <repeats 48 times>]
                        long long int __align = 0
                    }
                }
                ma_semaphore operationSemaphore = {
                    int value = 0
                    ma_pthread_mutex_t lock = <union> {
                        __pthread_mutex_s __data = {
                            int __lock = 0
                            unsigned int __count = 0
                            int __owner = 0
                            unsigned int __nusers = 0
                            int __kind = 0
                            short int __spins = 0
                            short int __elision = 0
                            __pthread_list_t __list = {
                                __pthread_internal_list *__prev = NULL
                                __pthread_internal_list *__next = NULL
                            }
                        }
                        char[] __size = [0 <repeats 40 times>]
                        long int __align = 0
                    }
                    ma_pthread_cond_t cond = <union> {
                        __pthread_cond_s __data = {
                            __atomic_wide_counter __wseq = <union> {
                                long long unsigned int __value64 = 0
                                struct __value32 = {
                                    unsigned int __low = 0
                                    unsigned int __high = 0
                                }
                            }
                            __atomic_wide_counter __g1_start = <union> {
                                long long unsigned int __value64 = 0
                                struct __value32 = {
                                    unsigned int __low = 0
                                    unsigned int __high = 0
                                }
                            }
                            unsigned int[] __g_size = [0, 0]
                            unsigned int __g1_orig_size = 0
                            unsigned int __wrefs = 0
                            unsigned int[] __g_signals = [0, 0]
                            unsigned int __unused_initialized_1 = 0
                            unsigned int __unused_initialized_2 = 0
                        }
                        char[] __size = [0 <repeats 48 times>]
                        long long int __align = 0
                    }
                }
                ma_uint32 operation = 0
                ma_result operationResult = MA_SUCCESS (0)
                ma_timer timer = <union> {
                    ma_int64 counter = 0
                    double counterD = 0.000000
                }
                double priorRunTime = 0.000000
                ma_uint32 currentPeriodFramesRemainingPlayback = 0
                ma_uint32 currentPeriodFramesRemainingCapture = 0
                ma_uint64 lastProcessedFramePlayback = 0
                ma_uint64 lastProcessedFrameCapture = 0
                ma_atomic_bool32 isStarted = {
                    ma_bool32 value = 0
                }
            }
        }
    })
    ma_log *pLog = 0x7cacfbfe00f8 (<points to #5>)
    ma_uint32 sampleRate = 48000
    ma_uint32 listenerCount = 1
    ma_spatializer_listener[] listeners = [
        {
            ma_spatializer_listener_config config = {
                ma_uint32 channelsOut = 2
                ma_channel *pChannelMapOut = 0x7b3cfbfe40f0 ("\v\f")
                ma_handedness handedness = ma_handedness_right (0)
                float coneInnerAngleInRadians = 6.283185
                float coneOuterAngleInRadians = 6.283185
                float coneOuterGain = 0.000000
                float speedOfSound = 343.299988
                ma_vec3f worldUp = {
                    float x = 0.000000
                    float y = 1.000000
                    float z = 0.000000
                }
            }
            ma_atomic_vec3f position = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
                ma_spinlock lock = 0
            }
            ma_atomic_vec3f direction = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = -1.000000
                }
                ma_spinlock lock = 0
            }
            ma_atomic_vec3f velocity = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
                ma_spinlock lock = 0
            }
            ma_bool32 isEnabled = 1
            ma_bool32 _ownsHeap = 1
            void *_pHeap = 0x7b3cfbfe40f0
        },
        {
            ma_spatializer_listener_config config = {
                ma_uint32 channelsOut = 0
                ma_channel *pChannelMapOut = NULL
                ma_handedness handedness = ma_handedness_right (0)
                float coneInnerAngleInRadians = 0.000000
                float coneOuterAngleInRadians = 0.000000
                float coneOuterGain = 0.000000
                float speedOfSound = 0.000000
                ma_vec3f worldUp = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
            }
            ma_atomic_vec3f position = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
                ma_spinlock lock = 0
            }
            ma_atomic_vec3f direction = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
                ma_spinlock lock = 0
            }
            ma_atomic_vec3f velocity = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
                ma_spinlock lock = 0
            }
            ma_bool32 isEnabled = 0
            ma_bool32 _ownsHeap = 0
            void *_pHeap = NULL
        },
        {
            ma_spatializer_listener_config config = {
                ma_uint32 channelsOut = 0
                ma_channel *pChannelMapOut = NULL
                ma_handedness handedness = ma_handedness_right (0)
                float coneInnerAngleInRadians = 0.000000
                float coneOuterAngleInRadians = 0.000000
                float coneOuterGain = 0.000000
                float speedOfSound = 0.000000
                ma_vec3f worldUp = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
            }
            ma_atomic_vec3f position = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
                ma_spinlock lock = 0
            }
            ma_atomic_vec3f direction = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
                ma_spinlock lock = 0
            }
            ma_atomic_vec3f velocity = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
                ma_spinlock lock = 0
            }
            ma_bool32 isEnabled = 0
            ma_bool32 _ownsHeap = 0
            void *_pHeap = NULL
        },
        {
            ma_spatializer_listener_config config = {
                ma_uint32 channelsOut = 0
                ma_channel *pChannelMapOut = NULL
                ma_handedness handedness = ma_handedness_right (0)
                float coneInnerAngleInRadians = 0.000000
                float coneOuterAngleInRadians = 0.000000
                float coneOuterGain = 0.000000
                float speedOfSound = 0.000000
                ma_vec3f worldUp = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
            }
            ma_atomic_vec3f position = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
                ma_spinlock lock = 0
            }
            ma_atomic_vec3f direction = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
                ma_spinlock lock = 0
            }
            ma_atomic_vec3f velocity = {
                ma_vec3f v = {
                    float x = 0.000000
                    float y = 0.000000
                    float z = 0.000000
                }
                ma_spinlock lock = 0
            }
            ma_bool32 isEnabled = 0
            ma_bool32 _ownsHeap = 0
            void *_pHeap = NULL
        }
    ]
    ma_allocation_callbacks allocationCallbacks = {
        void *pUserData = NULL
        void *(size_t, void *) onMalloc = 0x5616b47026d0 (void *ma__malloc_default(size_t sz, void *pUserData))
        void *(void *, size_t, void *) onRealloc = 0x5616b47026c0 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
        void(void *, void *) onFree = 0x5616b47026b0 (void ma__free_default(void *p, void *pUserData))
    }
    ma_bool8 ownsResourceManager = 1
    ma_bool8 ownsDevice = 1
    ma_spinlock inlinedSoundLock = 0
    ma_sound_inlined *pInlinedSoundHead = 0x7cacfbfe7880 ({
        ma_sound sound = {
            ma_engine_node engineNode = {
                ma_node_base baseNode = {
                    ma_node_graph *pNodeGraph = 0x7b1cfb000020 (<points to #1>)
                    const ma_node_vtable *vtable = 0x5616b4ca71c0 ({
                        void(ma_node *, const float **, ma_uint32 *, float **, ma_uint32 *) onProcess = 0x5616b4989bf0 (void ma_engine_node_process_pcm_frames__sound(ma_node *pNode, const float **ppFramesIn, ma_uint32 *pFrameCountIn, float **ppFramesOut, ma_uint32 *pFrameCountOut))
                        ma_result(ma_node *, ma_uint32, ma_uint32 *) onGetRequiredInputFrameCount = NULL
                        ma_uint8 inputBusCount = 0
                        ma_uint8 outputBusCount = 1
                        ma_uint32 flags = 0
                    })
                    ma_uint32 inputBusCount = 0
                    ma_uint32 outputBusCount = 1
                    ma_node_input_bus *pInputBuses = 0x7cacfbfe78d8 ({
                        ma_node_output_bus head = {
                            ma_node *pNode = NULL
                            ma_uint8 outputBusIndex = 0
                            ma_uint8 channels = 0
                            ma_uint8 inputNodeInputBusIndex = 0
                            ma_uint32 flags = 0
                            ma_uint32 refCount = 0
                            ma_bool32 isAttached = 0
                            ma_spinlock lock = 0
                            float volume = 0.000000
                            ma_node_output_bus *pNext = NULL
                            ma_node_output_bus *pPrev = NULL
                            ma_node *pInputNode = NULL
                        }
                        ma_uint32 nextCounter = 0
                        ma_spinlock lock = 0
                        ma_uint8 channels = 0
                    })
                    ma_node_output_bus *pOutputBuses = 0x7cacfbfe7968 (<points to #3>)
                    float *pCachedData = NULL
                    ma_uint16 cachedDataCapInFramesPerBus = 0
                    ma_uint16 cachedFrameCountOut = 0
                    ma_uint16 cachedFrameCountIn = 0
                    ma_uint16 consumedFrameCountIn = 0
                    ma_node_state state = ma_node_state_started (0)
                    ma_uint64[] stateTimes = [0, 18446744073709551615]
                    ma_uint64 localTime = 0
                    ma_node_input_bus[] _inputBuses = [
                        {
                            ma_node_output_bus head = {
                                ma_node *pNode = NULL
                                ma_uint8 outputBusIndex = 0
                                ma_uint8 channels = 0
                                ma_uint8 inputNodeInputBusIndex = 0
                                ma_uint32 flags = 0
                                ma_uint32 refCount = 0
                                ma_bool32 isAttached = 0
                                ma_spinlock lock = 0
                                float volume = 0.000000
                                ma_node_output_bus *pNext = NULL
                                ma_node_output_bus *pPrev = NULL
                                ma_node *pInputNode = NULL
                            }
                            ma_uint32 nextCounter = 0
                            ma_spinlock lock = 0
                            ma_uint8 channels = 0
                        },
                        {
                            ma_node_output_bus head = {
                                ma_node *pNode = NULL
                                ma_uint8 outputBusIndex = 0
                                ma_uint8 channels = 0
                                ma_uint8 inputNodeInputBusIndex = 0
                                ma_uint32 flags = 0
                                ma_uint32 refCount = 0
                                ma_bool32 isAttached = 0
                                ma_spinlock lock = 0
                                float volume = 0.000000
                                ma_node_output_bus *pNext = NULL
                                ma_node_output_bus *pPrev = NULL
                                ma_node *pInputNode = NULL
                            }
                            ma_uint32 nextCounter = 0
                            ma_spinlock lock = 0
                            ma_uint8 channels = 0
                        }
                    ]
                    ma_node_output_bus[] _outputBuses = [
                        <points to #3>,
                        {
                            ma_node *pNode = NULL
                            ma_uint8 outputBusIndex = 0
                            ma_uint8 channels = 0
                            ma_uint8 inputNodeInputBusIndex = 0
                            ma_uint32 flags = 0
                            ma_uint32 refCount = 0
                            ma_bool32 isAttached = 0
                            ma_spinlock lock = 0
                            float volume = 0.000000
                            ma_node_output_bus *pNext = NULL
                            ma_node_output_bus *pPrev = NULL
                            ma_node *pInputNode = NULL
                        }
                    ]
                    void *_pHeap = 0x7b5cfbfe2b50
                    ma_bool32 _ownsHeap = 0
                }
                ma_engine *pEngine = 0x7b1cfb000020 (<points to #0>)
                ma_uint32 sampleRate = 48000
                ma_uint32 volumeSmoothTimeInPCMFrames = 0
                ma_mono_expansion_mode monoExpansionMode = ma_mono_expansion_mode_duplicate (0)
                ma_fader fader = {
                    ma_fader_config config = {
                        ma_format format = ma_format_f32 (5)
                        ma_uint32 channels = 2
                        ma_uint32 sampleRate = 48000
                    }
                    float volumeBeg = 1.000000
                    float volumeEnd = 1.000000
                    ma_uint64 lengthInFrames = 0
                    ma_int64 cursorInFrames = 0
                }
                ma_linear_resampler resampler = {
                    ma_linear_resampler_config config = {
                        ma_format format = ma_format_f32 (5)
                        ma_uint32 channels = 2
                        ma_uint32 sampleRateIn = 1
                        ma_uint32 sampleRateOut = 1
                        ma_uint32 lpfOrder = 0
                        double lpfNyquistFactor = 1.000000
                    }
                    ma_uint32 inAdvanceInt = 1
                    ma_uint32 inAdvanceFrac = 0
                    ma_uint32 inTimeInt = 1
                    ma_uint32 inTimeFrac = 0
                    union x0 = <union> {
                        float *f32 = 0x7b5cfbfe2b50 (0.000000)
                        ma_int16 *s16 = 0x7b5cfbfe2b50 (0)
                    }
                    union x1 = <union> {
                        float *f32 = 0x7b5cfbfe2b58 (0.000000)
                        ma_int16 *s16 = 0x7b5cfbfe2b58 (0)
                    }
                    ma_lpf lpf = {
                        ma_format format = ma_format_f32 (5)
                        ma_uint32 channels = 2
                        ma_uint32 sampleRate = 1
                        ma_uint32 lpf1Count = 0
                        ma_uint32 lpf2Count = 0
                        ma_lpf1 *pLPF1 = 0x7b5cfbfe2b60 ({
                            ma_format format = <unknown> (3083)
                            ma_uint32 channels = 0
                            ma_biquad_coefficient a = <union> <#7> {
                                float f32 = 0.000000
                                ma_int32 s32 = 0
                            }
                            ma_biquad_coefficient *pR1 = 0x3f8000003f800000 (<out-of-bounds>)
                            void *_pHeap = 0x3f8000003f800000
                            ma_bool32 _ownsHeap = 0
                        })
                        ma_lpf2 *pLPF2 = 0x7b5cfbfe2b60 ({
                            ma_biquad bq = {
                                ma_format format = <unknown> (3083)
                                ma_uint32 channels = 0
                                ma_biquad_coefficient b0 = <union> <points to #7>
                                ma_biquad_coefficient b1 = <union> {
                                    float f32 = 0.000000
                                    ma_int32 s32 = 0
                                }
                                ma_biquad_coefficient b2 = <union> {
                                    float f32 = 1.000000
                                    ma_int32 s32 = 1065353216
                                }
                                ma_biquad_coefficient a1 = <union> {
                                    float f32 = 1.000000
                                    ma_int32 s32 = 1065353216
                                }
                                ma_biquad_coefficient a2 = <union> {
                                    float f32 = 1.000000
                                    ma_int32 s32 = 1065353216
                                }
                                ma_biquad_coefficient *pR1 = NULL
                                ma_biquad_coefficient *pR2 = NULL
                                void *_pHeap = NULL
                                ma_bool32 _ownsHeap = 0
                            }
                        })
                        void *_pHeap = 0x7b5cfbfe2b60
                        ma_bool32 _ownsHeap = 0
                    }
                    void *_pHeap = 0x7b5cfbfe2b50
                    ma_bool32 _ownsHeap = 0
                }
                ma_spatializer spatializer = {
                    ma_uint32 channelsIn = 2
                    ma_uint32 channelsOut = 2
                    ma_channel *pChannelMapIn = 0x7b5cfbfe2b60 ("\v\f")
                    ma_attenuation_model attenuationModel = ma_attenuation_model_inverse (1)
                    ma_positioning positioning = ma_positioning_absolute (0)
                    ma_handedness handedness = ma_handedness_right (0)
                    float minGain = 0.000000
                    float maxGain = 1.000000
                    float minDistance = 1.000000
                    float maxDistance = 340282346638528859811704183484516925440.000000
                    float rolloff = 1.000000
                    float coneInnerAngleInRadians = 6.283185
                    float coneOuterAngleInRadians = 6.283185
                    float coneOuterGain = 0.000000
                    float dopplerFactor = 1.000000
                    float directionalAttenuationFactor = 1.000000
                    ma_uint32 gainSmoothTimeInFrames = 384
                    ma_atomic_vec3f position = {
                        ma_vec3f v = {
                            float x = 0.000000
                            float y = 0.000000
                            float z = 0.000000
                        }
                        ma_spinlock lock = 0
                    }
                    ma_atomic_vec3f direction = {
                        ma_vec3f v = {
                            float x = 0.000000
                            float y = 0.000000
                            float z = -1.000000
                        }
                        ma_spinlock lock = 0
                    }
                    ma_atomic_vec3f velocity = {
                        ma_vec3f v = {
                            float x = 0.000000
                            float y = 0.000000
                            float z = 0.000000
                        }
                        ma_spinlock lock = 0
                    }
                    float dopplerPitch = 1.000000
                    float minSpatializationChannelGain = 0.200000
                    ma_gainer gainer = {
                        ma_gainer_config config = {
                            ma_uint32 channels = 2
                            ma_uint32 smoothTimeInFrames = 384
                        }
                        ma_uint32 t = 4294967295
                        float masterVolume = 1.000000
                        float *pOldGains = 0x7b5cfbfe2b70 (1.000000)
                        float *pNewGains = 0x7b5cfbfe2b78 (1.000000)
                        void *_pHeap = 0x7b5cfbfe2b70
                        ma_bool32 _ownsHeap = 0
                    }
                    float *pNewChannelGainsOut = 0x7b5cfbfe2b68 (0.000000)
                    void *_pHeap = 0x7b5cfbfe2b60
                    ma_bool32 _ownsHeap = 0
                }
                ma_panner panner = {
                    ma_format format = ma_format_f32 (5)
                    ma_uint32 channels = 2
                    ma_pan_mode mode = ma_pan_mode_balance (0)
                    float pan = 0.000000
                }
                ma_gainer volumeGainer = {
                    ma_gainer_config config = {
                        ma_uint32 channels = 0
                        ma_uint32 smoothTimeInFrames = 0
                    }
                    ma_uint32 t = 0
                    float masterVolume = 0.000000
                    float *pOldGains = NULL
                    float *pNewGains = NULL
                    void *_pHeap = NULL
                    ma_bool32 _ownsHeap = 0
                }
                ma_atomic_float volume = {
                    ma_float value = 1.000000
                }
                float pitch = 1.000000
                float oldPitch = 1.000000
                float oldDopplerPitch = 1.000000
                ma_bool32 isPitchDisabled = 1
                ma_bool32 isSpatializationDisabled = 1
                ma_uint32 pinnedListenerIndex = 0
                struct fadeSettings = {
                    ma_atomic_float volumeBeg = {
                        ma_float value = 1.000000
                    }
                    ma_atomic_float volumeEnd = {
                        ma_float value = 1.000000
                    }
                    ma_atomic_uint64 fadeLengthInFrames = {
                        ma_uint64 value = 18446744073709551615
                    }
                    ma_atomic_uint64 absoluteGlobalTimeInFrames = {
                        ma_uint64 value = 18446744073709551615
                    }
                }
                ma_bool8 _ownsHeap = 1
                void *_pHeap = 0x7b5cfbfe2b50
            }
            ma_data_source *pDataSource = 0x7c8cfbfe0780
            ma_uint64 seekTarget = 18446744073709551615
            ma_bool32 atEnd = 0
            void *endCallback = NULL
            void *pEndCallbackUserData = NULL
            ma_bool8 ownsDataSource = 1
            ma_resource_manager_data_source *pResourceManagerDataSource = 0x7c8cfbfe0780 ({
                union backend = <union> {
                    ma_resource_manager_data_buffer buffer = {
                        ma_data_source_base ds = <#8> {
                            const ma_data_source_vtable *vtable = 0x5616b4ca7620 ({
                                ma_result(ma_data_source *, void *, ma_uint64, ma_uint64 *) onRead = 0x5616b495e020 (ma_result ma_resource_manager_data_buffer_cb__read_pcm_frames(ma_data_source *pDataSource, void *pFramesOut, ma_uint64 frameCount, ma_uint64 *pFramesRead))
                                ma_result(ma_data_source *, ma_uint64) onSeek = 0x5616b4958140 (ma_result ma_resource_manager_data_buffer_cb__seek_to_pcm_frame(ma_data_source *pDataSource, ma_uint64 frameIndex))
                                ma_result(ma_data_source *, ma_format *, ma_uint32 *, ma_uint32 *, ma_channel *, size_t) onGetDataFormat = 0x5616b495bc60 (ma_result ma_resource_manager_data_buffer_cb__get_data_format(ma_data_source *pDataSource, ma_format *pFormat, ma_uint32 *pChannels, ma_uint32 *pSampleRate, ma_channel *pChannelMap, size_t channelMapCap))
                                ma_result(ma_data_source *, ma_uint64 *) onGetCursor = 0x5616b495c470 (ma_result ma_resource_manager_data_buffer_cb__get_cursor_in_pcm_frames(ma_data_source *pDataSource, ma_uint64 *pCursor))
                                ma_result(ma_data_source *, ma_uint64 *) onGetLength = 0x5616b495c6e0 (ma_result ma_resource_manager_data_buffer_cb__get_length_in_pcm_frames(ma_data_source *pDataSource, ma_uint64 *pLength))
                                ma_result(ma_data_source *, ma_bool32) onSetLooping = 0x5616b491a6e0 (ma_result ma_resource_manager_data_buffer_cb__set_looping(ma_data_source *pDataSource, ma_bool32 isLooping))
                                ma_uint32 flags = 0
                            })
                            ma_uint64 rangeBegInFrames = 0
                            ma_uint64 rangeEndInFrames = 18446744073709551615
                            ma_uint64 loopBegInFrames = 0
                            ma_uint64 loopEndInFrames = 18446744073709551615
                            ma_data_source *pCurrent = 0x7c8cfbfe0780
                            ma_data_source *pNext = NULL
                            ma_data_source *(ma_data_source *) onGetNext = NULL
                            ma_bool32 isLooping = 0
                        }
                        ma_resource_manager *pResourceManager = 0x7cacfbfe7380 (<points to #4>)
                        ma_resource_manager_data_buffer_node *pNode = 0x7bccfbfe10c0 (<points to #6>)
                        ma_uint32 flags = 28684
                        ma_uint32 executionCounter = 0
                        ma_uint32 executionPointer = 0
                        ma_uint64 seekTargetInPCMFrames = 0
                        ma_bool32 seekToCursorOnNextRead = 0
                        ma_result result = MA_SUCCESS (0)
                        ma_bool32 isLooping = 0
                        ma_atomic_bool32 isConnectorInitialized = {
                            ma_bool32 value = 1
                        }
                        union connector = <union> {
                            ma_decoder decoder = {
                                ma_data_source_base ds = <#9> {
                                    const ma_data_source_vtable *vtable = 0x5616b4ca7840 ({
                                        ma_result(ma_data_source *, void *, ma_uint64, ma_uint64 *) onRead = 0x5616b49347a0 (ma_result ma_decoder__data_source_on_read(ma_data_source *pDataSource, void *pFramesOut, ma_uint64 frameCount, ma_uint64 *pFramesRead))
                                        ma_result(ma_data_source *, ma_uint64) onSeek = 0x5616b4937810 (ma_result ma_decoder__data_source_on_seek(ma_data_source *pDataSource, ma_uint64 frameIndex))
                                        ma_result(ma_data_source *, ma_format *, ma_uint32 *, ma_uint32 *, ma_channel *, size_t) onGetDataFormat = 0x5616b49383e0 (ma_result ma_decoder__data_source_on_get_data_format(ma_data_source *pDataSource, ma_format *pFormat, ma_uint32 *pChannels, ma_uint32 *pSampleRate, ma_channel *pChannelMap, size_t channelMapCap))
                                        ma_result(ma_data_source *, ma_uint64 *) onGetCursor = 0x5616b47c49f0 (ma_result ma_decoder__data_source_on_get_cursor(ma_data_source *pDataSource, ma_uint64 *pCursor))
                                        ma_result(ma_data_source *, ma_uint64 *) onGetLength = 0x5616b4938a00 (ma_result ma_decoder__data_source_on_get_length(ma_data_source *pDataSource, ma_uint64 *pLength))
                                        ma_result(ma_data_source *, ma_bool32) onSetLooping = NULL
                                        ma_uint32 flags = 0
                                    })
                                    ma_uint64 rangeBegInFrames = 0
                                    ma_uint64 rangeEndInFrames = 18446744073709551615
                                    ma_uint64 loopBegInFrames = 0
                                    ma_uint64 loopEndInFrames = 18446744073709551615
                                    ma_data_source *pCurrent = 0x7c8cfbfe0800
                                    ma_data_source *pNext = NULL
                                    ma_data_source *(ma_data_source *) onGetNext = NULL
                                    ma_bool32 isLooping = 0
                                }
                                ma_data_source *pBackend = 0x7bdcfbfe0400
                                const ma_decoding_backend_vtable *pBackendVTable = 0x5616b4ca7960 ({
                                    ma_result(void *, ma_result(void *, void *, size_t, size_t *), ma_result(void *, ma_int64, ma_seek_origin), ma_result(void *, ma_int64 *), void *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInit = 0x5616b4928ed0 (ma_result ma_decoding_backend_init__flac(void *pUserData, ma_result(void *, void *, size_t, size_t *) onRead, ma_result(void *, ma_int64, ma_seek_origin) onSeek, ma_result(void *, ma_int64 *) onTell, void *pReadSeekTellUserData, const ma_decoding_backend_config *pConfig, const ma_allocation_callbacks *pAllocationCallbacks, ma_data_source **ppBackend))
                                    ma_result(void *, const char *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitFile = 0x5616b49c7610 (ma_result ma_decoding_backend_init_file__flac(void *pUserData, const char *pFilePath, const ma_decoding_backend_config *pConfig, const ma_allocation_callbacks *pAllocationCallbacks, ma_data_source **ppBackend))
                                    ma_result(void *, const wchar_t *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitFileW = 0x5616b49c7960 (ma_result ma_decoding_backend_init_file_w__flac(void *pUserData, const wchar_t *pFilePath, const ma_decoding_backend_config *pConfig, const ma_allocation_callbacks *pAllocationCallbacks, ma_data_source **ppBackend))
                                    ma_result(void *, const void *, size_t, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitMemory = 0x5616b49c8400 (ma_result ma_decoding_backend_init_memory__flac(void *pUserData, const void *pData, size_t dataSize, const ma_decoding_backend_config *pConfig, const ma_allocation_callbacks *pAllocationCallbacks, ma_data_source **ppBackend))
                                    void(void *, ma_data_source *, const ma_allocation_callbacks *) onUninit = 0x5616b49c8f80 (void ma_decoding_backend_uninit__flac(void *pUserData, ma_data_source *pBackend, const ma_allocation_callbacks *pAllocationCallbacks))
                                })
                                void *pBackendUserData = NULL
                                void *onRead = NULL
                                ma_result(ma_decoder *, ma_int64, ma_seek_origin) onSeek = NULL
                                ma_result(ma_decoder *, ma_int64 *) onTell = NULL
                                void *pUserData = NULL
                                ma_uint64 readPointerInPCMFrames = 0
                                ma_format outputFormat = ma_format_f32 (5)
                                ma_uint32 outputChannels = 2
                                ma_uint32 outputSampleRate = 48000
                                ma_data_converter converter = {
                                    ma_format formatIn = ma_format_f32 (5)
                                    ma_format formatOut = ma_format_f32 (5)
                                    ma_uint32 channelsIn = 2
                                    ma_uint32 channelsOut = 2
                                    ma_uint32 sampleRateIn = 44100
                                    ma_uint32 sampleRateOut = 48000
                                    ma_dither_mode ditherMode = ma_dither_mode_none (0)
                                    ma_data_converter_execution_path executionPath = ma_data_converter_execution_path_resample_only (3)
                                    ma_channel_converter channelConverter = {
                                        ma_format format = ma_format_f32 (5)
                                        ma_uint32 channelsIn = 2
                                        ma_uint32 channelsOut = 2
                                        ma_channel_mix_mode mixingMode = ma_channel_mix_mode_rectangular (0)
                                        ma_channel_conversion_path conversionPath = ma_channel_conversion_path_passthrough (1)
                                        ma_channel *pChannelMapIn = 0x7c1cfbfe0140 ("")
                                        ma_channel *pChannelMapOut = NULL
                                        ma_uint8 *pShuffleTable = NULL
                                        union weights = {...}
                                        void *_pHeap = 0x7c1cfbfe0140
                                        ma_bool32 _ownsHeap = 0
                                    }
                                    ma_resampler resampler = {
                                        ma_resampling_backend *pBackend = 0x7c8cfbfe0928
                                        ma_resampling_backend_vtable *pBackendVTable = 0x5616b4ca8280 ({...})
                                        void *pBackendUserData = 0x7c8cfbfe0900
                                        ma_format format = ma_format_f32 (5)
                                        ma_uint32 channels = 2
                                        ma_uint32 sampleRateIn = 44100
                                        ma_uint32 sampleRateOut = 48000
                                        union state = {...}
                                        void *_pHeap = 0x7c1cfbfe0148
                                        ma_bool32 _ownsHeap = 0
                                    }
                                    ma_bool8 hasPreFormatConversion = 0
                                    ma_bool8 hasPostFormatConversion = 0
                                    ma_bool8 hasChannelConverter = 0
                                    ma_bool8 hasResampler = 1
                                    ma_bool8 isPassthrough = 0
                                    ma_bool8 _ownsHeap = 1
                                    void *_pHeap = 0x7c1cfbfe0140
                                }
                                void *pInputCache = NULL
                                ma_uint64 inputCacheCap = 0
                                ma_uint64 inputCacheConsumed = 0
                                ma_uint64 inputCacheRemaining = 0
                                ma_allocation_callbacks allocationCallbacks = {
                                    void *pUserData = NULL
                                    void *(size_t, void *) onMalloc = 0x5616b47026d0 (void *ma__malloc_default(size_t sz, void *pUserData))
                                    void *(void *, size_t, void *) onRealloc = 0x5616b47026c0 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
                                    void(void *, void *) onFree = 0x5616b47026b0 (void ma__free_default(void *p, void *pUserData))
                                }
                                union data = <union> {
                                    struct vfs = {
                                        ma_vfs *pVFS = NULL
                                        void *file = NULL
                                    }
                                    struct memory = {
                                        const ma_uint8 *pData = NULL
                                        size_t dataSize = 0
                                        size_t currentReadPos = 0
                                    }
                                }
                            }
                            ma_audio_buffer buffer = {
                                ma_audio_buffer_ref ref = {
                                    ma_data_source_base ds = <points to #9>
                                    ma_format format = <unknown> (4227728384)
                                    ma_uint32 channels = 31708
                                    ma_uint32 sampleRate = 3033168224
                                    ma_uint64 cursor = 0
                                    ma_uint64 sizeInFrames = 0
                                    const void *pData = NULL
                                }
                                ma_allocation_callbacks allocationCallbacks = {
                                    void *pUserData = NULL
                                    void *(size_t, void *) onMalloc = NULL
                                    void *(void *, size_t, void *) onRealloc = NULL
                                    void(void *, void *) onFree = <out-of-bounds>
                                }
                                ma_bool32 ownsData = 48000
                                ma_uint8[] _pExtraData = [0]
                            }
                            ma_paged_audio_buffer pagedBuffer = {
                                ma_data_source_base ds = <points to #9>
                                ma_paged_audio_buffer_data *pData = 0x7bdcfbfe0400 ({
                                    ma_format format = <unknown> (3033168320)
                                    ma_uint32 channels = 22038
                                    ma_paged_audio_buffer_page head = {
                                        ma_paged_audio_buffer_page *pNext = NULL
                                        ma_uint64 sizeInFrames = 18446744073709551615
                                        ma_uint8[] pAudioData = [0]
                                    }
                                    ma_paged_audio_buffer_page *pTail = 0xffffffffffffffff (<out-of-bounds>)
                                })
                                ma_paged_audio_buffer_page *pCurrent = 0x5616b4ca7960 ({
                                    ma_paged_audio_buffer_page *pNext = 0x5616b4928ed0 ({
                                        ma_paged_audio_buffer_page *pNext = 0x4d00000078bf5741 ({...})
                                        ma_uint64 sizeInFrames = 4739626623106666377
                                        ma_uint8[] pAudioData = [85 ('U')]
                                    })
                                    ma_uint64 sizeInFrames = 94655519421968
                                    ma_uint8[] pAudioData = [96 ('`')]
                                })
                                ma_uint64 relativeCursor = 0
                                ma_uint64 absoluteCursor = 0
                            }
                        }
                    }
                    ma_resource_manager_data_stream stream = {
                        ma_data_source_base ds = <points to #8>
                        ma_resource_manager *pResourceManager = 0x7cacfbfe7380 (<points to #4>)
                        ma_uint32 flags = 4227731648
                        ma_decoder decoder = {
                            ma_data_source_base ds = {
                                const ma_data_source_vtable *vtable = 0x700c (<out-of-bounds>)
                                ma_uint64 rangeBegInFrames = 0
                                ma_uint64 rangeEndInFrames = 0
                                ma_uint64 loopBegInFrames = 0
                                ma_uint64 loopEndInFrames = 4294967296
                                ma_data_source *pCurrent = 0x5616b4ca7840
                                ma_data_source *pNext = NULL
                                ma_data_source *(ma_data_source *) onGetNext = <out-of-bounds>
                                ma_bool32 isLooping = 0
                            }
                            ma_data_source *pBackend = 0xffffffffffffffff
                            const ma_decoding_backend_vtable *pBackendVTable = 0x7c8cfbfe0800 ({
                                ma_result(void *, ma_result(void *, void *, size_t, size_t *), ma_result(void *, ma_int64, ma_seek_origin), ma_result(void *, ma_int64 *), void *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInit = 0x5616b4ca7840
                                ma_result(void *, const char *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitFile = NULL
                                ma_result(void *, const wchar_t *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitFileW = <out-of-bounds>
                                ma_result(void *, const void *, size_t, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitMemory = NULL
                                void(void *, ma_data_source *, const ma_allocation_callbacks *) onUninit = <out-of-bounds>
                            })
                            void *pBackendUserData = NULL
                            void *onRead = NULL
                            ma_result(ma_decoder *, ma_int64, ma_seek_origin) onSeek = NULL
                            ma_result(ma_decoder *, ma_int64 *) onTell = 0x7bdcfbfe0400
                            void *pUserData = 0x5616b4ca7960
                            ma_uint64 readPointerInPCMFrames = 0
                            ma_format outputFormat = ma_format_unknown (0)
                            ma_uint32 outputChannels = 0
                            ma_uint32 outputSampleRate = 0
                            ma_data_converter converter = {
                                ma_format formatIn = ma_format_unknown (0)
                                ma_format formatOut = ma_format_unknown (0)
                                ma_uint32 channelsIn = 0
                                ma_uint32 channelsOut = 0
                                ma_uint32 sampleRateIn = 0
                                ma_uint32 sampleRateOut = 0
                                ma_dither_mode ditherMode = <unknown> (5)
                                ma_data_converter_execution_path executionPath = ma_data_converter_execution_path_channels_only (2)
                                ma_channel_converter channelConverter = {
                                    ma_format format = <unknown> (48000)
                                    ma_uint32 channelsIn = 0
                                    ma_uint32 channelsOut = 5
                                    ma_channel_mix_mode mixingMode = <unknown> (5)
                                    ma_channel_conversion_path conversionPath = ma_channel_conversion_path_mono_out (2)
                                    ma_channel *pChannelMapIn = 0xbb800000ac44 (<out-of-bounds>)
                                    ma_channel *pChannelMapOut = 0x300000000 (<out-of-bounds>)
                                    ma_uint8 *pShuffleTable = 0x200000005 (<out-of-bounds>)
                                    union weights = <union> {
                                        float **f32 = 0x2
                                        ma_int32 **s16 = 0x2
                                    }
                                    void *_pHeap = 0x1
                                    ma_bool32 _ownsHeap = 4227727680
                                }
                                ma_resampler resampler = {
                                    ma_resampling_backend *pBackend = NULL
                                    ma_resampling_backend_vtable *pBackendVTable = NULL
                                    void *pBackendUserData = NULL
                                    ma_format format = <unknown> (4227727680)
                                    ma_uint32 channels = 31772
                                    ma_uint32 sampleRateIn = 0
                                    ma_uint32 sampleRateOut = 0
                                    union state = <union> {
                                        ma_linear_resampler linear = {...}
                                    }
                                    void *_pHeap = 0x7c1cfbfe0158
                                    ma_bool32 _ownsHeap = 4227727704
                                }
                                ma_bool8 hasPreFormatConversion = 0
                                ma_bool8 hasPostFormatConversion = 0
                                ma_bool8 hasChannelConverter = 0
                                ma_bool8 hasResampler = 0
                                ma_bool8 isPassthrough = 0
                                ma_bool8 _ownsHeap = 0
                                void *_pHeap = 0x7c1cfbfe0148
                            }
                            void *pInputCache = NULL
                            ma_uint64 inputCacheCap = 136463928656200
                            ma_uint64 inputCacheConsumed = 0
                            ma_uint64 inputCacheRemaining = 1099528404992
                            ma_allocation_callbacks allocationCallbacks = {
                                void *pUserData = 0x7c1cfbfe0140
                                void *(size_t, void *) onMalloc = NULL
                                void *(void *, size_t, void *) onRealloc = NULL
                                void(void *, void *) onFree = NULL
                            }
                            union data = <union> {
                                struct vfs = {
                                    ma_vfs *pVFS = NULL
                                    void *file = NULL
                                }
                                struct memory = {
                                    const ma_uint8 *pData = NULL
                                    size_t dataSize = 0
                                    size_t currentReadPos = 94655516518096
                                }
                            }
                        }
                        ma_bool32 isDecoderInitialized = 3027248832
                        ma_uint64 totalLengthInPCMFrames = 94655516518064
                        ma_uint32 relativeCursor = 0
                        ma_uint64 absoluteCursor = 0
                        ma_uint32 currentPageIndex = 0
                        ma_uint32 executionCounter = 0
                        ma_uint32 executionPointer = 0
                        ma_bool32 isLooping = 0
                        void *pPageData = NULL
                        ma_uint32[] pageFrameCount = [0, 0]
                        ma_result result = MA_SUCCESS (0)
                        ma_bool32 isDecoderAtEnd = 0
                        ma_bool32[] isPageValid = [0, 0]
                        ma_bool32 seekCounter = 0
                    }
                }
                ma_uint32 flags = 28684
                ma_uint32 executionCounter = 0
                ma_uint32 executionPointer = 0
            })
        }
        ma_sound_inlined *pNext = NULL
        ma_sound_inlined *pPrev = NULL
    })
    ma_uint32 inlinedSoundCount = 1
    ma_uint32 gainSmoothTimeInFrames = 384
    ma_uint32 defaultVolumeSmoothTimeInPCMFrames = 0
    ma_mono_expansion_mode monoExpansionMode = ma_mono_expansion_mode_duplicate (0)
    void(void *, float *, ma_uint64) onProcess = NULL
    void *pProcessUserData = NULL
}
```
</details>

<details>
  <summary>uprintf</summary>

```c
uprintf's state: {
    _upf_memory_region *allocator = 0x7f6da74b0800 ({
        _upf_memory_region *prev = 0x7f6da7d13800 ({
            _upf_memory_region *prev = 0x7e7da57f4800 ({
                _upf_memory_region *prev = NULL
                size_t capacity = 65535
                size_t length = 65440
                uint8_t *data = 0x7e7da57f4820 (6)
            })
            size_t capacity = 131070
            size_t length = 125632
            uint8_t *data = 0x7f6da7d13820 (232)
        })
        size_t capacity = 262140
        size_t length = 174152
        uint8_t *data = 0x7f6da74b0820 (104)
    })
    _Bool is_init = true
    const uint8_t *base = 0x558c51ed3000 (127)
    uint8_t *file = 0x7f6da6127000 (127)
    off_t file_size = 1355032
    _Bool is64bit = false
    uint8_t offset_size = 4
    uint8_t address_size = 8
    const uint8_t *die = 0x7f6da6208cab (244)
    size_t die_size = 210168
    const uint8_t *abbrev = 0x7f6da623c1a3 (1)
    const char *str = 0x7f6da62523dd ("__glibc_reserved")
    const char *line_str = 0x7f6da6254259 ("/projects/uprintf")
    const uint8_t *str_offsets = NULL
    const uint8_t *addr = NULL
    const uint8_t *rnglists = 0x7f6da62688fb (255)
    _upf_type_map_vec type_map = {
        uint32_t capacity = 128
        uint32_t length = 120
        _upf_type_map_entry *data = 0x7f6da74d67f0 ({
            const uint8_t *die = 0x7f6da620a524 (15)
            _upf_type *type_ptr = 0x7f6da74d3840 ({
                const char *name = 0x7f6da6252f2e ("_upf_memory_region")
                _upf_type_kind kind = _UPF_TK_POINTER (4)
                int modifiers = 0
                size_t size = 8
                union as = <union> {
                    struct cstruct = {
                        _Bool is_declaration = true
                        _upf_member_vec members = {
                            uint32_t capacity = 0
                            uint32_t length = 0
                            _upf_member *data = NULL
                        }
                    }
                    struct cenum = {
                        _upf_type *underlying_type = 0x7f6da74d3cb0 (<#0> {
                            const char *name = 0x7f6da6252f2e ("_upf_memory_region")
                            _upf_type_kind kind = _UPF_TK_STRUCT (0)
                            int modifiers = 0
                            size_t size = 32
                            union as = <union> {
                                struct cstruct = {
                                    _Bool is_declaration = false
                                    _upf_member_vec members = {
                                        uint32_t capacity = 4
                                        uint32_t length = 4
                                        _upf_member *data = 0x7f6da74d38e0 ({...})
                                    }
                                }
                                struct cenum = {
                                    _upf_type *underlying_type = NULL
                                    _upf_enum_vec enums = {
                                        uint32_t capacity = 4
                                        uint32_t length = 4
                                        _upf_enum *data = 0x7f6da74d38e0 ({...})
                                    }
                                }
                                struct array = {
                                    _upf_type *element_type = NULL
                                    _upf_size_t_vec lengths = {
                                        uint32_t capacity = 4
                                        uint32_t length = 4
                                        size_t *data = 0x7f6da74d38e0 (140108915622397)
                                    }
                                }
                                struct pointer = {
                                    _upf_type *type = NULL
                                    _Bool is_reference = true
                                }
                                struct function = {
                                    _upf_type *return_type = NULL
                                    _upf_type_ptr_vec arg_types = {
                                        uint32_t capacity = 4
                                        uint32_t length = 4
                                        _upf_type **data = 0x7f6da74d38e0
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
                        _upf_type *element_type = 0x7f6da74d3cb0 (<points to #0>)
                        _upf_size_t_vec lengths = {
                            uint32_t capacity = 0
                            uint32_t length = 0
                            size_t *data = NULL
                        }
                    }
                    struct pointer = {
                        _upf_type *type = 0x7f6da74d3cb0 (<points to #0>)
                        _Bool is_reference = false
                    }
                    struct function = {
                        _upf_type *return_type = 0x7f6da74d3cb0 (<points to #0>)
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
    _upf_type_vec types = {
        uint32_t capacity = 4
        uint32_t length = 1
        _upf_type **data = 0x7f6da74d9848
    }
    _upf_cu_vec cus = {
        uint32_t capacity = 4
        uint32_t length = 1
        _upf_cu *data = 0x7f6da74d25e0 (<#1> {
            const uint8_t *base = 0x7f6da6208cab (244)
            _upf_scope scope = {
                _upf_range_vec ranges = {
                    uint32_t capacity = 16
                    uint32_t length = 13
                    _upf_range *data = 0x7e7da57fca80 ({
                        uint64_t start = 340864
                        uint64_t end = 494070
                    })
                }
                _upf_named_type_vec vars = {
                    uint32_t capacity = 256
                    uint32_t length = 209
                    _upf_named_type *data = 0x7f6da74bb9e0 ({
                        const uint8_t *die = 0x7f6da62091cd (37)
                        const char *name = 0x7f6da6253bfd ("_DYNAMIC")
                    })
                }
                _upf_scope_vec scopes = {
                    uint32_t capacity = 128
                    uint32_t length = 68
                    _upf_scope *data = 0x7f6da74d0160 ({
                        _upf_range_vec ranges = {
                            uint32_t capacity = 4
                            uint32_t length = 1
                            _upf_range *data = 0x7e7da57fef00 ({
                                uint64_t start = 340496
                                uint64_t end = 340548
                            })
                        }
                        _upf_named_type_vec vars = {
                            uint32_t capacity = 0
                            uint32_t length = 0
                            _upf_named_type *data = NULL
                        }
                        _upf_scope_vec scopes = {
                            uint32_t capacity = 0
                            uint32_t length = 0
                            _upf_scope *data = NULL
                        }
                    })
                }
            }
            uint64_t base_address = 0
            uint64_t addr_base = 0
            uint64_t str_offsets_base = 18446744073709551615
            uint64_t rnglists_base = 18446744073709551615
            _upf_abbrev_vec abbrevs = {
                uint32_t capacity = 128
                uint32_t length = 97
                _upf_abbrev *data = 0x7e7da57f95c0 ({
                    uint64_t tag = 73
                    _Bool has_children = false
                    _upf_attr_vec attrs = {
                        uint32_t capacity = 4
                        uint32_t length = 2
                        _upf_attr *data = 0x7e7da57f49e0 ({
                            uint64_t name = 2
                            uint64_t form = 24
                            int64_t implicit_const = 0
                        })
                    }
                })
            }
            _upf_named_type_vec types = {
                uint32_t capacity = 128
                uint32_t length = 120
                _upf_named_type *data = 0x7e7da57fd3c0 ({
                    const uint8_t *die = 0x7f6da6208cd5 (36)
                    const char *name = 0x7f6da6253047 ("long unsigned int")
                })
            }
            _upf_function_vec functions = {
                uint32_t capacity = 256
                uint32_t length = 172
                _upf_function *data = 0x7f6da74cc5a0 ({
                    const char *name = 0x7f6da6253768 ("strncmp")
                    const uint8_t *return_type_die = 0x7f6da6208cef (77)
                    _upf_named_type_vec args = {
                        uint32_t capacity = 4
                        uint32_t length = 3
                        _upf_named_type *data = 0x7e7da57fdc40 ({
                            const uint8_t *die = 0x7f6da6209649 (15)
                            const char *name = NULL
                        })
                    }
                    _Bool is_variadic = false
                    uint64_t pc = 18446744073709551615
                })
            }
        })
    }
    _upf_extern_function_vec extern_functions = {
        uint32_t capacity = 16
        uint32_t length = 9
        _upf_extern_function *data = 0x7e7da57f48e0 ({
            const char *name = 0x558c51ed3b06 ("__asan_init")
            uint64_t pc = 140108937495472
        })
    }
    int circular_id = 2
    _upf_range_vec addresses = {
        uint32_t capacity = 128
        uint32_t length = 86
        _upf_range *data = 0x7f6da74d2fc0 ({
            uint64_t start = 2147446784
            uint64_t end = 2415882240
        })
    }
    char *buffer = 0x7dfda57e0200 (<out-of-bounds>)
    char *ptr = 0x7dfda57e2b43 (<out-of-bounds>)
    uint32_t size = 16384
    uint32_t free = 5742
    jmp_buf error_jmp_buf = [
        {
            __jmp_buf __jmpbuf = [0, 323818633243883578, 140733000285336, 1, 7, 1, 323818633457793082, 5808276030726305850]
            int __mask_was_saved = 0
            __sigset_t __saved_mask = {
                long unsigned int[] __val = [0 <repeats 16 times>]
            }
        }
    ]
    const char *file_path = 0x558c51f50f00 ("examples/uprintf.c")
    int line = 7
    _upf_token_vec tokens = {
        uint32_t capacity = 4
        uint32_t length = 2
        _upf_token *data = 0x7f6da74d37f0 ({
            _upf_token_type type = _UPF_TT_AMPERSAND (28)
            const char *string = 0x558c51f51500 ("&")
        })
    }
    size_t tokens_idx = 2
    uint64_t current_pc = 340540
    _upf_cu *current_cu = 0x7f6da74d25e0 (<points to #1>)
```
</details>

<details>
  <summary>cxxopts (<a href="https://github.com/jarro2783/cxxopts">src</a>)</summary>

```c
Options: {
    string m_program = {
        _Alloc_hider _M_dataplus = {
            char *_M_p = 0x7b4357300810 ("test")
        }
        size_type _M_string_length = 4
        union <anonymous> = <union> {
            char[] _M_local_buf = [116 ('t'), 101 ('e'), 115 ('s'), 116 ('t'), 0 <repeats 12 times>]
            size_type _M_allocated_capacity = 1953719668
        }
    }
    String m_help_string = {
        _Alloc_hider _M_dataplus = {
            char *_M_p = 0x7b73581e0040 ("A brief description")
        }
        size_type _M_string_length = 19
        union <anonymous> = <union> {
            char[] _M_local_buf = [19, 0 <repeats 15 times>]
            size_type _M_allocated_capacity = 19
        }
    }
    string m_custom_help = {
        _Alloc_hider _M_dataplus = {
            char *_M_p = 0x7b4357300850 ("[OPTION...]")
        }
        size_type _M_string_length = 11
        union <anonymous> = <union> {
            char[] _M_local_buf = [91 ('['), 79 ('O'), 80 ('P'), 84 ('T'), 73 ('I'), 79 ('O'), 78 ('N'), 46 ('.'), 46 ('.'), 46 ('.'), 93 (']'), 0 <repeats 5 times>]
            size_type _M_allocated_capacity = 3336691550293872475
        }
    }
    string m_positional_help = {
        _Alloc_hider _M_dataplus = {
            char *_M_p = 0x7b73581e0070 ("positional parameters")
        }
        size_type _M_string_length = 21
        union <anonymous> = <union> {
            char[] _M_local_buf = [21, 0 <repeats 15 times>]
            size_type _M_allocated_capacity = 21
        }
    }
    bool m_show_positional = false
    bool m_allow_unrecognised = false
    size_t m_width = 76
    bool m_tab_expansion = false
    shared_ptr<std::unordered_map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::shared_ptr<cxxopts::OptionDetails>, std::hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::shared_ptr<cxxopts::OptionDetails> > > > > m_options = {
        element_type *_M_ptr = 0x7bb3581e0030 ({
            _Hashtable _M_h = {
                _Hashtable_ebo_helper<std::hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, true> _M_hash = {
                    hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > _M_obj = {}
                }
                _Hashtable_ebo_helper<std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, true> _M_equal = {
                    equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > _M_obj = {}
                }
                _Hashtable_ebo_helper<std::allocator<std::__detail::_Hash_node<std::pair<const std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::shared_ptr<cxxopts::OptionDetails> >, true> >, true> _M_alloc = {
                    allocator<std::__detail::_Hash_node<std::pair<const std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::shared_ptr<cxxopts::OptionDetails> >, true> > _M_obj = {}
                }
                __node_base **_M_buckets = 0x7bf3581e0040
                size_type _M_bucket_count = 13
                __node_base _M_before_begin = {
                    _Hash_node_base *_M_nxt = 0x7ba3581e04a0 ({
                        _Hash_node_base *_M_nxt = 0x7ba3581e0320 ({
                            _Hash_node_base *_M_nxt = 0x7ba3581e0260 ({
                                _Hash_node_base *_M_nxt = 0x7ba3581e0380 ({
                                    _Hash_node_base *_M_nxt = 0x7ba3581e0200 ({
                                        _Hash_node_base *_M_nxt = 0x7ba3581e0140 ({...})
                                    })
                                })
                            })
                        })
                    })
                }
                size_type _M_element_count = 8
                _Prime_rehash_policy _M_rehash_policy = {
                    float _M_max_load_factor = 1.000000
                    size_t _M_next_resize = 13
                }
                __node_base *_M_single_bucket = NULL
            }
        })
        __shared_count<(__gnu_cxx::_Lock_policy)2> _M_refcount = {
            _Sp_counted_base<(__gnu_cxx::_Lock_policy)2> *_M_pi = 0x7bb3581e0020 ({
                int(...) *_vptr._Sp_counted_base = 0x55f73452e440
                _Atomic_word _M_use_count = 1
                _Atomic_word _M_weak_count = 1
            })
        }
    }
    vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > m_positional = {
        _Vector_impl _M_impl = {
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_start = NULL
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_finish = NULL
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_end_of_storage = NULL
        }
    }
    unordered_set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > m_positional_set = {
        _Hashtable _M_h = {
            _Hashtable_ebo_helper<std::hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, true> _M_hash = {
                hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > _M_obj = {}
            }
            _Hashtable_ebo_helper<std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, true> _M_equal = {
                equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > _M_obj = {}
            }
            _Hashtable_ebo_helper<std::allocator<std::__detail::_Hash_node<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, true> >, true> _M_alloc = {
                allocator<std::__detail::_Hash_node<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, true> > _M_obj = {}
            }
            __node_base **_M_buckets = 0x7b43573008f0
            size_type _M_bucket_count = 1
            __node_base _M_before_begin = {
                _Hash_node_base *_M_nxt = NULL
            }
            size_type _M_element_count = 0
            _Prime_rehash_policy _M_rehash_policy = {
                float _M_max_load_factor = 1.000000
                size_t _M_next_resize = 0
            }
            __node_base *_M_single_bucket = NULL
        }
    }
    vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > m_group = {
        _Vector_impl _M_impl = {
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_start = 0x7b73581e0100 ({
                _Alloc_hider _M_dataplus = {
                    char *_M_p = 0x7b73581e0110 ("")
                }
                size_type _M_string_length = 0
                union <anonymous> = <union> {
                    char[] _M_local_buf = [0, -66 <repeats 15 times>]
                    size_type _M_allocated_capacity = 13744632839234567680
                }
            })
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_finish = 0x7b73581e0120 (<#0> {
                _Alloc_hider _M_dataplus = {
                    char *_M_p = 0x2000000202 (<out-of-bounds>)
                }
                size_type _M_string_length = 26
                union <anonymous> = <union> {
                    char[] _M_local_buf = [64 ('@'), 1, 30, 88 ('X'), 115 ('s'), 123 ('{'), 0, 0, 3, 0 <repeats 7 times>]
                    size_type _M_allocated_capacity = 135735329816896
                }
            })
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_end_of_storage = 0x7b73581e0120 (<points to #0>)
        }
    }
    map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, cxxopts::HelpGroupDetails, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, cxxopts::HelpGroupDetails> > > m_help = {
        _Rep_type _M_t = {
            _Rb_tree_impl<std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, true> _M_impl = {
                less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > _M_key_compare = {}
                _Rb_tree_node_base _M_header = {
                    _Rb_tree_color _M_color = _S_red (0)
                    void *_M_parent = 0x7c23581e0040
                    void *_M_left = 0x7c23581e0040
                    void *_M_right = 0x7c23581e0040
                }
                size_t _M_node_count = 1
            }
        }
    }
}
Result: {
    NameHashMap m_keys = {
        _Hashtable _M_h = {
            _Hashtable_ebo_helper<std::hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, true> _M_hash = {
                hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > _M_obj = {}
            }
            _Hashtable_ebo_helper<std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, true> _M_equal = {
                equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > _M_obj = {}
            }
            _Hashtable_ebo_helper<std::allocator<std::__detail::_Hash_node<std::pair<const std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, long unsigned int>, true> >, true> _M_alloc = {
                allocator<std::__detail::_Hash_node<std::pair<const std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, long unsigned int>, true> > _M_obj = {}
            }
            __node_base **_M_buckets = 0x7bf3581e01a0
            size_type _M_bucket_count = 13
            __node_base _M_before_begin = {
                _Hash_node_base *_M_nxt = 0x7ba3581e0980 ({
                    _Hash_node_base *_M_nxt = 0x7ba3581e08c0 ({
                        _Hash_node_base *_M_nxt = 0x7ba3581e0860 ({
                            _Hash_node_base *_M_nxt = 0x7ba3581e0800 ({
                                _Hash_node_base *_M_nxt = 0x7ba3581e07a0 ({
                                    _Hash_node_base *_M_nxt = 0x7ba3581e0740 ({
                                        _Hash_node_base *_M_nxt = 0x7ba3581e0920 ({...})
                                    })
                                })
                            })
                        })
                    })
                })
            }
            size_type _M_element_count = 8
            _Prime_rehash_policy _M_rehash_policy = {
                float _M_max_load_factor = 1.000000
                size_t _M_next_resize = 13
            }
            __node_base *_M_single_bucket = NULL
        }
    }
    ParsedHashMap m_values = {
        _Hashtable _M_h = {
            _Hashtable_ebo_helper<std::hash<long unsigned int>, true> _M_hash = {
                hash<long unsigned int> _M_obj = {}
            }
            _Hashtable_ebo_helper<std::equal_to<long unsigned int>, true> _M_equal = {
                equal_to<long unsigned int> _M_obj = {}
            }
            _Hashtable_ebo_helper<std::allocator<std::__detail::_Hash_node<std::pair<long unsigned int const, cxxopts::OptionValue>, false> >, true> _M_alloc = {
                allocator<std::__detail::_Hash_node<std::pair<long unsigned int const, cxxopts::OptionValue>, false> > _M_obj = {}
            }
            __node_base **_M_buckets = 0x7bf3581e00f0
            size_type _M_bucket_count = 13
            __node_base _M_before_begin = {
                _Hash_node_base *_M_nxt = 0x7ba3581e0680 ({
                    _Hash_node_base *_M_nxt = 0x7ba3581e0620 ({
                        _Hash_node_base *_M_nxt = 0x7ba3581e05c0 (<#0> {
                            _Hash_node_base *_M_nxt = 0x7ba3581e0500 ({
                                _Hash_node_base *_M_nxt = NULL
                            })
                        })
                    })
                })
            }
            size_type _M_element_count = 4
            _Prime_rehash_policy _M_rehash_policy = {
                float _M_max_load_factor = 1.000000
                size_t _M_next_resize = 13
            }
            __node_base *_M_single_bucket = NULL
        }
    }
    vector<cxxopts::KeyValue, std::allocator<cxxopts::KeyValue> > m_sequential = {
        _Vector_impl _M_impl = {
            KeyValue *_M_start = NULL
            KeyValue *_M_finish = NULL
            KeyValue *_M_end_of_storage = NULL
        }
    }
    vector<cxxopts::KeyValue, std::allocator<cxxopts::KeyValue> > m_defaults = {
        _Vector_impl _M_impl = {
            KeyValue *_M_start = 0x7c53581e02c0 ({
                string m_key = {
                    _Alloc_hider _M_dataplus = {
                        char *_M_p = 0x7c53581e02d0 ("help")
                    }
                    size_type _M_string_length = 4
                    union <anonymous> = <union> {
                        char[] _M_local_buf = [104 ('h'), 101 ('e'), 108 ('l'), 112 ('p'), 0, -66 <repeats 11 times>]
                        size_type _M_allocated_capacity = 13744632021876761960
                    }
                }
                string m_value = {
                    _Alloc_hider _M_dataplus = {
                        char *_M_p = 0x7c53581e02f0 ("false")
                    }
                    size_type _M_string_length = 5
                    union <anonymous> = <union> {
                        char[] _M_local_buf = [102 ('f'), 97 ('a'), 108 ('l'), 115 ('s'), 101 ('e'), 0, -66 <repeats 10 times>]
                        size_type _M_allocated_capacity = 13744423548509512038
                    }
                }
            })
            KeyValue *_M_finish = 0x7c53581e0380 ({
                string m_key = {
                    _Alloc_hider _M_dataplus = {
                        char *_M_p = 0xbebebebebebebebe (<out-of-bounds>)
                    }
                    size_type _M_string_length = 13744632839234567870
                    union <anonymous> = <union> {
                        char[] _M_local_buf = [-66 <repeats 16 times>]
                        size_type _M_allocated_capacity = 13744632839234567870
                    }
                }
                string m_value = {
                    _Alloc_hider _M_dataplus = {
                        char *_M_p = 0xbebebebebebebebe (<out-of-bounds>)
                    }
                    size_type _M_string_length = 13744632839234567870
                    union <anonymous> = <union> {
                        char[] _M_local_buf = [-66 <repeats 16 times>]
                        size_type _M_allocated_capacity = 13744632839234567870
                    }
                }
            })
            KeyValue *_M_end_of_storage = 0x7c53581e03c0 ({
                string m_key = {
                    _Alloc_hider _M_dataplus = {
                        char *_M_p = 0xcc6e96b9cc6e96b9 (<out-of-bounds>)
                    }
                    size_type _M_string_length = 136697402491888
                    union <anonymous> = <union> {
                        char[] _M_local_buf = [0 <repeats 16 times>]
                        size_type _M_allocated_capacity = 0
                    }
                }
                string m_value = {
                    _Alloc_hider _M_dataplus = {
                        char *_M_p = NULL
                    }
                    size_type _M_string_length = 0
                    union <anonymous> = <union> {
                        char[] _M_local_buf = [3, 17, 0, 0, -16, 0, 0, 0, 96 ('`'), 0 <repeats 7 times>]
                        size_type _M_allocated_capacity = 1030792155395
                    }
                }
            })
        }
    }
    vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > m_unmatched = {
        _Vector_impl _M_impl = {
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_start = NULL
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_finish = NULL
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_end_of_storage = NULL
        }
    }
}
```
</details>

<details>
  <summary>taskflow (<a href="https://github.com/taskflow/taskflow">src</a>)</summary>

```c
TaskA
TaskC
TaskB
TaskD
executor: {
    mutex _taskflows_mutex = {
        __native_type _M_mutex = <union> {
            __pthread_mutex_s __data = {
                int __lock = 0
                unsigned int __count = 0
                int __owner = 0
                unsigned int __nusers = 0
                int __kind = 0
                short int __spins = 0
                short int __elision = 0
                __pthread_list_t __list = {
                    __pthread_internal_list *__prev = NULL
                    __pthread_internal_list *__next = NULL
                }
            }
            char[] __size = [0 <repeats 40 times>]
            long int __align = 0
        }
    }
    vector<tf::Worker, std::allocator<tf::Worker> > _workers = {
        _Vector_impl _M_impl = {
            void *_M_start = 0x7e3ba6be0400
            void *_M_finish = 0x7e3ba6be9c00
            void *_M_end_of_storage = 0x7e3ba6be9c00
        }
    }
    DefaultNotifier _notifier = {
        atomic<long unsigned int> _state = {
            __int_type _M_i = 4398046511119
        }
        vector<tf::NonblockingNotifierV2::Waiter, std::allocator<tf::NonblockingNotifierV2::Waiter> > _waiters = {
            _Vector_impl _M_impl = {
                Waiter *_M_start = 0x7d3ba6be0080 ({
                    atomic<long unsigned int> next = {
                        __int_type _M_i = 16383
                    }
                    uint64_t epoch = 4398046511104
                    mutex mu = {
                        __native_type _M_mutex = <union> {
                            __pthread_mutex_s __data = {
                                int __lock = 0
                                unsigned int __count = 0
                                int __owner = 0
                                unsigned int __nusers = 1
                                int __kind = 0
                                short int __spins = 0
                                short int __elision = 0
                                __pthread_list_t __list = {
                                    __pthread_internal_list *__prev = NULL
                                    __pthread_internal_list *__next = NULL
                                }
                            }
                            char[] __size = [0 <repeats 12 times>, 1, 0 <repeats 27 times>]
                            long int __align = 0
                        }
                    }
                    condition_variable cv = {
                        __condvar _M_cond = {
                            __gthread_cond_t _M_cond = <union> {
                                __pthread_cond_s __data = {
                                    __atomic_wide_counter __wseq = <union> {
                                        long long unsigned int __value64 = 2
                                        struct __value32 = {...}
                                    }
                                    __atomic_wide_counter __g1_start = <union> {
                                        long long unsigned int __value64 = 0
                                        struct __value32 = {...}
                                    }
                                    unsigned int[] __g_size = [0, 0]
                                    unsigned int __g1_orig_size = 0
                                    unsigned int __wrefs = 8
                                    unsigned int[] __g_signals = [0, 0]
                                    unsigned int __unused_initialized_1 = 0
                                    unsigned int __unused_initialized_2 = 0
                                }
                                char[] __size = [2, 0 <repeats 27 times>, 8, 0 <repeats 19 times>]
                                long long int __align = 2
                            }
                        }
                    }
                    unsigned int state = 1
                })
                Waiter *_M_finish = 0x7d3ba6be0880 (<#0> {
                    atomic<long unsigned int> next = {
                        __int_type _M_i = 0
                    }
                    uint64_t epoch = 0
                    mutex mu = {
                        __native_type _M_mutex = <union> {
                            __pthread_mutex_s __data = {
                                int __lock = 0
                                unsigned int __count = 0
                                int __owner = 0
                                unsigned int __nusers = 0
                                int __kind = 0
                                short int __spins = 0
                                short int __elision = 0
                                __pthread_list_t __list = {
                                    __pthread_internal_list *__prev = NULL
                                    __pthread_internal_list *__next = NULL
                                }
                            }
                            char[] __size = [0 <repeats 40 times>]
                            long int __align = 0
                        }
                    }
                    condition_variable cv = {
                        __condvar _M_cond = {
                            __gthread_cond_t _M_cond = <union> {
                                __pthread_cond_s __data = {
                                    __atomic_wide_counter __wseq = <union> {
                                        long long unsigned int __value64 = 0
                                        struct __value32 = {...}
                                    }
                                    __atomic_wide_counter __g1_start = <union> {
                                        long long unsigned int __value64 = 0
                                        struct __value32 = {...}
                                    }
                                    unsigned int[] __g_size = [0, 0]
                                    unsigned int __g1_orig_size = 0
                                    unsigned int __wrefs = 0
                                    unsigned int[] __g_signals = [0, 0]
                                    unsigned int __unused_initialized_1 = 0
                                    unsigned int __unused_initialized_2 = 0
                                }
                                char[] __size = [0 <repeats 48 times>]
                                long long int __align = 0
                            }
                        }
                    }
                    unsigned int state = 0
                })
                Waiter *_M_end_of_storage = 0x7d3ba6be0880 (<points to #0>)
            }
        }
    }
    condition_variable _topology_cv = {
        __condvar _M_cond = {
            __gthread_cond_t _M_cond = <union> {
                __pthread_cond_s __data = {
                    __atomic_wide_counter __wseq = <union> {
                        long long unsigned int __value64 = 0
                        struct __value32 = {
                            unsigned int __low = 0
                            unsigned int __high = 0
                        }
                    }
                    __atomic_wide_counter __g1_start = <union> {
                        long long unsigned int __value64 = 0
                        struct __value32 = {
                            unsigned int __low = 0
                            unsigned int __high = 0
                        }
                    }
                    unsigned int[] __g_size = [0, 0]
                    unsigned int __g1_orig_size = 0
                    unsigned int __wrefs = 0
                    unsigned int[] __g_signals = [0, 0]
                    unsigned int __unused_initialized_1 = 0
                    unsigned int __unused_initialized_2 = 0
                }
                char[] __size = [0 <repeats 48 times>]
                long long int __align = 0
            }
        }
    }
    mutex _topology_mutex = {
        __native_type _M_mutex = <union> {
            __pthread_mutex_s __data = {
                int __lock = 0
                unsigned int __count = 0
                int __owner = 0
                unsigned int __nusers = 0
                int __kind = 0
                short int __spins = 0
                short int __elision = 0
                __pthread_list_t __list = {
                    __pthread_internal_list *__prev = NULL
                    __pthread_internal_list *__next = NULL
                }
            }
            char[] __size = [0 <repeats 40 times>]
            long int __align = 0
        }
    }
    size_t _num_topologies = 0
    list<tf::Taskflow, std::allocator<tf::Taskflow> > _taskflows = {
        _List_impl _M_impl = {
            _Node_header _M_node = {
                _List_node_base *_M_next = 0x7b6ba5b006e0 (<#1> {
                    _List_node_base *_M_next = 0x7b6ba5b006e0 (<points to #1>)
                    _List_node_base *_M_prev = 0x7b6ba5b006e0 (<points to #1>)
                })
                _List_node_base *_M_prev = 0x7b6ba5b006e0 (<points to #1>)
                size_t _M_size = 0
            }
        }
    }
    Freelist<tf::Node*> _buffers = {
        vector<tf::Freelist<tf::Node*>::Bucket, std::allocator<tf::Freelist<tf::Node*>::Bucket> > _buckets = {
            _Vector_impl _M_impl = {
                Bucket *_M_start = 0x7d1ba6be0080 ({
                    mutex mutex = {
                        __native_type _M_mutex = <union> {
                            __pthread_mutex_s __data = {
                                int __lock = 0
                                unsigned int __count = 0
                                int __owner = 0
                                unsigned int __nusers = 0
                                int __kind = 0
                                short int __spins = 0
                                short int __elision = 0
                                __pthread_list_t __list = {
                                    __pthread_internal_list *__prev = NULL
                                    __pthread_internal_list *__next = NULL
                                }
                            }
                            char[] __size = [0 <repeats 40 times>]
                            long int __align = 0
                        }
                    }
                    UnboundedTaskQueue<tf::Node*> queue = {
                        atomic<long int> _top = {
                            __int_type _M_i = 0
                        }
                        atomic<long int> _bottom = {
                            __int_type _M_i = 0
                        }
                        atomic<tf::UnboundedTaskQueue<tf::Node*>::Array*> _array = {
                            __base_type _M_b = {
                                Array *_M_p = 0x7b9ba6be0040 ({
                                    int64_t C = 1024
                                    int64_t M = 1023
                                    atomic<tf::Node*> *S = 0x7dbba6be0100 ({
                                        __base_type _M_b = {...}
                                    })
                                })
                            }
                        }
                        vector<tf::UnboundedTaskQueue<tf::Node*>::Array*, std::allocator<tf::UnboundedTaskQueue<tf::Node*>::Array*> > _garbage = {
                            _Vector_impl _M_impl = {
                                Array **_M_start = 0x7c7ba6be0040
                                Array **_M_finish = 0x7c7ba6be0040
                                Array **_M_end_of_storage = 0x7c7ba6be0140
                            }
                        }
                    }
                })
                Bucket *_M_finish = 0x7d1ba6be0680 (<#2> {
                    mutex mutex = {
                        __native_type _M_mutex = <union> {
                            __pthread_mutex_s __data = {
                                int __lock = 0
                                unsigned int __count = 0
                                int __owner = 0
                                unsigned int __nusers = 0
                                int __kind = 0
                                short int __spins = 0
                                short int __elision = 0
                                __pthread_list_t __list = {
                                    __pthread_internal_list *__prev = NULL
                                    __pthread_internal_list *__next = NULL
                                }
                            }
                            char[] __size = [0 <repeats 40 times>]
                            long int __align = 0
                        }
                    }
                    UnboundedTaskQueue<tf::Node*> queue = {
                        atomic<long int> _top = {
                            __int_type _M_i = 0
                        }
                        atomic<long int> _bottom = {
                            __int_type _M_i = 0
                        }
                        atomic<tf::UnboundedTaskQueue<tf::Node*>::Array*> _array = {
                            __base_type _M_b = {
                                Array *_M_p = NULL
                            }
                        }
                        vector<tf::UnboundedTaskQueue<tf::Node*>::Array*, std::allocator<tf::UnboundedTaskQueue<tf::Node*>::Array*> > _garbage = {
                            _Vector_impl _M_impl = {
                                Array **_M_start = NULL
                                Array **_M_finish = NULL
                                Array **_M_end_of_storage = NULL
                            }
                        }
                    }
                })
                Bucket *_M_end_of_storage = 0x7d1ba6be0680 (<points to #2>)
            }
        }
    }
    shared_ptr<tf::WorkerInterface> _worker_interface = {
        element_type *_M_ptr = NULL
        __shared_count<(__gnu_cxx::_Lock_policy)2> _M_refcount = {
            _Sp_counted_base<(__gnu_cxx::_Lock_policy)2> *_M_pi = NULL
        }
    }
    unordered_set<std::shared_ptr<tf::ObserverInterface>, std::hash<std::shared_ptr<tf::ObserverInterface> >, std::equal_to<std::shared_ptr<tf::ObserverInterface> >, std::allocator<std::shared_ptr<tf::ObserverInterface> > > _observers = {
        _Hashtable _M_h = {
            _Hashtable_ebo_helper<std::hash<std::shared_ptr<tf::ObserverInterface> >, true> _M_hash = {
                hash<std::shared_ptr<tf::ObserverInterface> > _M_obj = {}
            }
            _Hashtable_ebo_helper<std::equal_to<std::shared_ptr<tf::ObserverInterface> >, true> _M_equal = {
                equal_to<std::shared_ptr<tf::ObserverInterface> > _M_obj = {}
            }
            _Hashtable_ebo_helper<std::allocator<std::__detail::_Hash_node<std::shared_ptr<tf::ObserverInterface>, false> >, true> _M_alloc = {
                allocator<std::__detail::_Hash_node<std::shared_ptr<tf::ObserverInterface>, false> > _M_obj = {}
            }
            __node_base **_M_buckets = 0x7b6ba5b00750
            size_type _M_bucket_count = 1
            __node_base _M_before_begin = {
                _Hash_node_base *_M_nxt = NULL
            }
            size_type _M_element_count = 0
            _Prime_rehash_policy _M_rehash_policy = {
                float _M_max_load_factor = 1.000000
                size_t _M_next_resize = 0
            }
            __node_base *_M_single_bucket = NULL
        }
    }
}
taskflow: {
    Graph &_graph = {
        _Vector_impl _M_impl = {
            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_start = 0x7b9ba6be0430 ({
                __uniq_ptr_data<tf::Node, std::default_delete<tf::Node>, true, true> _M_t = {
                    tuple<tf::Node*, std::default_delete<tf::Node> > _M_t = {
                        default_delete<tf::Node> _M_head_impl = {}
                        Node *_M_head_impl = 0x7c7ba6be0900 ({
                            nstate_t _nstate = 0
                            atomic<int> _estate = {
                                __int_type _M_i = 0
                            }
                            string _name = {
                                _Alloc_hider _M_dataplus = {
                                    char *_M_p = 0x7c7ba6be0918 ("")
                                }
                                size_type _M_string_length = 0
                                union <anonymous> = <union> {
                                    char[] _M_local_buf = [0, -66 <repeats 15 times>]
                                    size_type _M_allocated_capacity = 13744632839234567680
                                }
                            }
                            void *_data = NULL
                            Topology *_topology = 0x7c3ba6be0050 ({
                                Taskflow &_taskflow = {
                                    Graph &_graph = {
                                        _Vector_impl _M_impl = {...}
                                    }
                                    mutex _mutex = {
                                        __native_type _M_mutex = {...}
                                    }
                                    string _name = {
                                        _Alloc_hider _M_dataplus = {...}
                                        size_type _M_string_length = 0
                                        union <anonymous> = {...}
                                    }
                                    Graph _graph = {
                                        _Vector_impl _M_impl = {...}
                                    }
                                    queue<std::shared_ptr<tf::Topology>, std::deque<std::shared_ptr<tf::Topology>, std::allocator<std::shared_ptr<tf::Topology> > > > _topologies = {
                                        deque<std::shared_ptr<tf::Topology>, std::allocator<std::shared_ptr<tf::Topology> > > c = {...}
                                    }
                                    optional<std::_List_iterator<tf::Taskflow> > _satellite = {
                                        _Optional_payload<std::_List_iterator<tf::Taskflow>, true, true, true> _M_payload = {...}
                                    }
                                }
                                promise<void> _promise = {
                                    shared_ptr<std::__future_base::_State_baseV2> _M_future = {
                                        element_type *_M_ptr = 0x7baba6be0020 ({...})
                                        __shared_count<(__gnu_cxx::_Lock_policy)2> _M_refcount = {...}
                                    }
                                    _Ptr_type _M_storage = {
                                        __uniq_ptr_data<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter, true, true> _M_t = {...}
                                    }
                                }
                                function<bool()> _pred = {
                                    _Any_data _M_functor = <union> {
                                        _Nocopy_types _M_unused = {...}
                                        char[] _M_pod_data = [-1 <repeats 8 times>, 0 <repeats 8 times>]
                                    }
                                    bool(_Any_data &, const _Any_data &, _Manager_operation) _M_manager = 0x56314f8b12b9
                                    bool(const _Any_data &) _M_invoker = 0x56314f8b126a
                                }
                                function<void()> _call = {
                                    _Any_data _M_functor = <union> {
                                        _Nocopy_types _M_unused = {...}
                                        char[] _M_pod_data = [0 <repeats 16 times>]
                                    }
                                    bool(_Any_data &, const _Any_data &, _Manager_operation) _M_manager = 0x56314f8b14da
                                    void(const _Any_data &) _M_invoker = 0x56314f8b1496
                                }
                                atomic<long unsigned int> _join_counter = {
                                    __int_type _M_i = 0
                                }
                                atomic<int> _estate = {
                                    __int_type _M_i = 0
                                }
                                exception_ptr _exception_ptr = {
                                    void *_M_exception_object = NULL
                                }
                            })
                            Node *_parent = NULL
                            size_t _num_successors = 2
                            SmallVector<tf::Node*, 4> _edges = {
                                void *BeginX = 0x7c7ba6be0960
                                void *EndX = 0x7c7ba6be0970
                                void *CapacityX = 0x7c7ba6be0980
                                U FirstEl = {
                                    byte[] buff = [<unknown> (192), <unknown> (7), <unknown> (190), <unknown> (166), <unknown> (123 ('{')), <unknown> (124 ('|')), <unknown> (0), <unknown> (0)]
                                }
                                SmallVectorStorage<tf::Node*, 4> Storage = {
                                    U[] InlineElts = [
                                        {...},
                                        {...},
                                        {...}
                                    ]
                                }
                            }
                            atomic<long unsigned int> _join_counter = {
                                __int_type _M_i = 0
                            }
                            handle_t _handle = {
                                _Variadic_union<false, std::monostate, tf::Node::Static, tf::Node::Runtime, tf::Node::Subflow, tf::Node::Condition, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_u = <union> {
                                    _Uninitialized<std::monostate, true> _M_first = {
                                        monostate _M_storage = {...}
                                    }
                                    _Variadic_union<false, tf::Node::Static, tf::Node::Runtime, tf::Node::Subflow, tf::Node::Condition, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_rest = <union> {
                                        _Uninitialized<tf::Node::Static, false> _M_first = {...}
                                        _Variadic_union<false, tf::Node::Runtime, tf::Node::Subflow, tf::Node::Condition, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_rest = {...}
                                    }
                                }
                                __index_type _M_index = 1
                            }
                            unique_ptr<tf::Node::Semaphores, std::default_delete<tf::Node::Semaphores> > _semaphores = {
                                __uniq_ptr_data<tf::Node::Semaphores, std::default_delete<tf::Node::Semaphores>, true, true> _M_t = {
                                    tuple<tf::Node::Semaphores*, std::default_delete<tf::Node::Semaphores> > _M_t = {
                                        default_delete<tf::Node::Semaphores> _M_head_impl = {...}
                                        Semaphores *_M_head_impl = NULL
                                    }
                                }
                            }
                            exception_ptr _exception_ptr = {
                                void *_M_exception_object = NULL
                            }
                        })
                    }
                }
            })
            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_finish = 0x7b9ba6be0450 (<#0> {
                __uniq_ptr_data<tf::Node, std::default_delete<tf::Node>, true, true> _M_t = {
                    tuple<tf::Node*, std::default_delete<tf::Node> > _M_t = {
                        default_delete<tf::Node> _M_head_impl = {}
                        Node *_M_head_impl = NULL
                    }
                }
            })
            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_end_of_storage = 0x7b9ba6be0450 (<points to #0>)
        }
    }
    mutex _mutex = {
        __native_type _M_mutex = <union> {
            __pthread_mutex_s __data = {
                int __lock = 0
                unsigned int __count = 0
                int __owner = 0
                unsigned int __nusers = 0
                int __kind = 0
                short int __spins = 0
                short int __elision = 0
                __pthread_list_t __list = {
                    __pthread_internal_list *__prev = NULL
                    __pthread_internal_list *__next = NULL
                }
            }
            char[] __size = [0 <repeats 40 times>]
            long int __align = 0
        }
    }
    string _name = {
        _Alloc_hider _M_dataplus = {
            char *_M_p = 0x7b6ba5b00550 ("")
        }
        size_type _M_string_length = 0
        union <anonymous> = <union> {
            char[] _M_local_buf = [0 <repeats 16 times>]
            size_type _M_allocated_capacity = 0
        }
    }
    Graph _graph = {
        _Vector_impl _M_impl = {
            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_start = 0x7b9ba6be0430 ({
                __uniq_ptr_data<tf::Node, std::default_delete<tf::Node>, true, true> _M_t = {
                    tuple<tf::Node*, std::default_delete<tf::Node> > _M_t = {
                        default_delete<tf::Node> _M_head_impl = {}
                        Node *_M_head_impl = 0x7c7ba6be0900 ({
                            nstate_t _nstate = 0
                            atomic<int> _estate = {
                                __int_type _M_i = 0
                            }
                            string _name = {
                                _Alloc_hider _M_dataplus = {
                                    char *_M_p = 0x7c7ba6be0918 ("")
                                }
                                size_type _M_string_length = 0
                                union <anonymous> = <union> {
                                    char[] _M_local_buf = [0, -66 <repeats 15 times>]
                                    size_type _M_allocated_capacity = 13744632839234567680
                                }
                            }
                            void *_data = NULL
                            Topology *_topology = 0x7c3ba6be0050 ({
                                Taskflow &_taskflow = {
                                    Graph &_graph = {
                                        _Vector_impl _M_impl = {...}
                                    }
                                    mutex _mutex = {
                                        __native_type _M_mutex = {...}
                                    }
                                    string _name = {
                                        _Alloc_hider _M_dataplus = {...}
                                        size_type _M_string_length = 0
                                        union <anonymous> = {...}
                                    }
                                    Graph _graph = {
                                        _Vector_impl _M_impl = {...}
                                    }
                                    queue<std::shared_ptr<tf::Topology>, std::deque<std::shared_ptr<tf::Topology>, std::allocator<std::shared_ptr<tf::Topology> > > > _topologies = {
                                        deque<std::shared_ptr<tf::Topology>, std::allocator<std::shared_ptr<tf::Topology> > > c = {...}
                                    }
                                    optional<std::_List_iterator<tf::Taskflow> > _satellite = {
                                        _Optional_payload<std::_List_iterator<tf::Taskflow>, true, true, true> _M_payload = {...}
                                    }
                                }
                                promise<void> _promise = {
                                    shared_ptr<std::__future_base::_State_baseV2> _M_future = {
                                        element_type *_M_ptr = 0x7baba6be0020 ({...})
                                        __shared_count<(__gnu_cxx::_Lock_policy)2> _M_refcount = {...}
                                    }
                                    _Ptr_type _M_storage = {
                                        __uniq_ptr_data<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter, true, true> _M_t = {...}
                                    }
                                }
                                function<bool()> _pred = {
                                    _Any_data _M_functor = <union> {
                                        _Nocopy_types _M_unused = {...}
                                        char[] _M_pod_data = [-1 <repeats 8 times>, 0 <repeats 8 times>]
                                    }
                                    bool(_Any_data &, const _Any_data &, _Manager_operation) _M_manager = 0x56314f8b12b9
                                    bool(const _Any_data &) _M_invoker = 0x56314f8b126a
                                }
                                function<void()> _call = {
                                    _Any_data _M_functor = <union> {
                                        _Nocopy_types _M_unused = {...}
                                        char[] _M_pod_data = [0 <repeats 16 times>]
                                    }
                                    bool(_Any_data &, const _Any_data &, _Manager_operation) _M_manager = 0x56314f8b14da
                                    void(const _Any_data &) _M_invoker = 0x56314f8b1496
                                }
                                atomic<long unsigned int> _join_counter = {
                                    __int_type _M_i = 0
                                }
                                atomic<int> _estate = {
                                    __int_type _M_i = 0
                                }
                                exception_ptr _exception_ptr = {
                                    void *_M_exception_object = NULL
                                }
                            })
                            Node *_parent = NULL
                            size_t _num_successors = 2
                            SmallVector<tf::Node*, 4> _edges = {
                                void *BeginX = 0x7c7ba6be0960
                                void *EndX = 0x7c7ba6be0970
                                void *CapacityX = 0x7c7ba6be0980
                                U FirstEl = {
                                    byte[] buff = [<unknown> (192), <unknown> (7), <unknown> (190), <unknown> (166), <unknown> (123 ('{')), <unknown> (124 ('|')), <unknown> (0), <unknown> (0)]
                                }
                                SmallVectorStorage<tf::Node*, 4> Storage = {
                                    U[] InlineElts = [
                                        {...},
                                        {...},
                                        {...}
                                    ]
                                }
                            }
                            atomic<long unsigned int> _join_counter = {
                                __int_type _M_i = 0
                            }
                            handle_t _handle = {
                                _Variadic_union<false, std::monostate, tf::Node::Static, tf::Node::Runtime, tf::Node::Subflow, tf::Node::Condition, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_u = <union> {
                                    _Uninitialized<std::monostate, true> _M_first = {
                                        monostate _M_storage = {...}
                                    }
                                    _Variadic_union<false, tf::Node::Static, tf::Node::Runtime, tf::Node::Subflow, tf::Node::Condition, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_rest = <union> {
                                        _Uninitialized<tf::Node::Static, false> _M_first = {...}
                                        _Variadic_union<false, tf::Node::Runtime, tf::Node::Subflow, tf::Node::Condition, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_rest = {...}
                                    }
                                }
                                __index_type _M_index = 1
                            }
                            unique_ptr<tf::Node::Semaphores, std::default_delete<tf::Node::Semaphores> > _semaphores = {
                                __uniq_ptr_data<tf::Node::Semaphores, std::default_delete<tf::Node::Semaphores>, true, true> _M_t = {
                                    tuple<tf::Node::Semaphores*, std::default_delete<tf::Node::Semaphores> > _M_t = {
                                        default_delete<tf::Node::Semaphores> _M_head_impl = {...}
                                        Semaphores *_M_head_impl = NULL
                                    }
                                }
                            }
                            exception_ptr _exception_ptr = {
                                void *_M_exception_object = NULL
                            }
                        })
                    }
                }
            })
            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_finish = 0x7b9ba6be0450 (<points to #0>)
            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_end_of_storage = 0x7b9ba6be0450 (<points to #0>)
        }
    }
    queue<std::shared_ptr<tf::Topology>, std::deque<std::shared_ptr<tf::Topology>, std::allocator<std::shared_ptr<tf::Topology> > > > _topologies = {
        deque<std::shared_ptr<tf::Topology>, std::allocator<std::shared_ptr<tf::Topology> > > c = {
            _Deque_impl _M_impl = {
                void *_M_map = 0x7bcba6be0080
                size_t _M_map_size = 8
                iterator _M_start = {
                    void *_M_cur = 0x7cbba6be0310
                    void *_M_first = 0x7cbba6be0300
                    void *_M_last = 0x7cbba6be0500
                    void *_M_node = 0x7bcba6be0098
                }
                iterator _M_finish = {
                    void *_M_cur = 0x7cbba6be0310
                    void *_M_first = 0x7cbba6be0300
                    void *_M_last = 0x7cbba6be0500
                    void *_M_node = 0x7bcba6be0098
                }
            }
        }
    }
    optional<std::_List_iterator<tf::Taskflow> > _satellite = {
        _Optional_payload<std::_List_iterator<tf::Taskflow>, true, true, true> _M_payload = {
            _Storage<std::_List_iterator<tf::Taskflow>, true> _M_payload = <union> {
                _Empty_byte _M_empty = {}
                _List_iterator<tf::Taskflow> _M_value = {
                    _List_node_base *_M_node = NULL
                }
            }
            bool _M_engaged = false
        }
    }
}
```
</details>

## Requirements

- Works only on Linux
- The oldest supported C version is c99 (tests require c11)
- Executable must have debugging information (`-g2` or higher)
- `elf.h` must be in the include path

## Usage

1. [Download](https://raw.githubusercontent.com/spevnev/uprintf/refs/heads/main/uprintf.h) the library, or clone the repo

2. Define `UPRINTF_IMPLEMENTATION` in one of the files:
    ```c
    #define UPRINTF_IMPLEMENTATION
    #include "uprintf.h"
    ```
    The implementation must be included in **only one** of the files. \
    A rarely modified file should be used because it increases the build time. \
    [Options](#options) can be defined before the implementation.

3.
    ```c
    uprintf(fmt, ...);
    ```
    `fmt` - a format string with placeholders (`%` followed by any letter). Use `%%` to print `%`. \
    For every placeholder, there must be an argument which is a **pointer** to the data that should be printed.

### Options

Options must be defined **before** the implementation:

```c
#define OPTION_NAME VALUE
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"
```

option | description | default
-|-|-
`UPRINTF_INDENTATION_WIDTH` | The number of spaces to use for indentation | 4
`UPRINTF_MAX_DEPTH` | Limits the nesting depth of structures. Negative values remove the limit | 10
`UPRINTF_IGNORE_STRUCTS` | Comma-separated list of struct names whose values should not be printed | FILE,pthread_mutex_t,pthread_cond_t
`UPRINTF_ARRAY_COMPRESSION_THRESHOLD` | The number of consecutive identical array elements before they get compressed. Non-positive values disable compression | 4
`UPRINTF_MAX_STRING_LENGTH` | The length before the string gets truncated. Non-positive values remove the limit | 200

## Limitations

- Incorrect type deduction when printing variables that get shadowed *later* in the same scope.
    ```c
    char var = 'c';
    {
        uprintf("%S\n", &var); // interprets char as an int
        int var = 1;
        uprintf("%S\n", &var); // prints int
    }
    ```

- Printing information about function from a shared library only works with gcc:
    ```c
    uprintf("%S\n", printf);
    // gcc  : 0x12345678 <void printf(const char *, ...)>
    // clang: 0x12345678
    ```

## How does it work?

The library uses debugging information to learn about the types.

On initialization:
1. Read the current binary.
2. Parse ELF and DWARF.
3. Store information about scopes and types.

On each invokation:
1. Find scopes based on the PC.
2. Parse the arguments string.
3. Find types of arguments in the current scope.
4. Print data using type definition.

## Testing

### Tested on

- x86_64
- gcc: 11, 12, 13, 14
- clang: 14, 15, 16, 17, 18

### Running tests

```console
$ make tests
```
