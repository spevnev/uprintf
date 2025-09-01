# Universal printf

*uprintf* is a header-only library for printing anything in C.

## Examples

Examples and their outputs can be found in [examples](examples), and can be built using `make examples`.

### Example outputs

<details>
  <summary>avl</summary>

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
  <summary>sqlite</summary>

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
                    char *pTmpSpace = 0x7df7991e0108 ("��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������"...)
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
                u16 btsFlags = 2
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
  <summary>vorbis</summary>

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
        uint8 *codeword_lengths = 0x7b9c561e0080 ("\n\a\b\r\t\a\v\n\b\b\f\a\t\b\b\b\r\a\a\b\a\f\f\n\a\b\t\a\t\f\t\n��n̹�n��")
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
  <summary>uprintf</summary>

```c
uprintf's state: {
    _upf_arena arena = {
        _upf_arena_region *head = 0x7bc1c77e00a0 ({
            uint8_t *data = 0x7f91c9442800 (56)
            size_t capacity = 262140
            size_t length = 164760
            _upf_arena_region *prev = 0x7bc1c77e0070 ({
                uint8_t *data = 0x7f91c9484800 (111)
                size_t capacity = 131070
                size_t length = 131008
                _upf_arena_region *prev = 0x7bc1c77e0040 ({
                    uint8_t *data = 0x7ea1c77f4800 (214)
                    size_t capacity = 65535
                    size_t length = 65472
                    _upf_arena_region *prev = NULL
                })
            })
        })
    }
    _Bool is_init = true
    const uint8_t *base = 0x5573c3030000 (127)
    uint8_t *file = 0x7f91c88a3000 (127)
    off_t file_size = 1428112
    _Bool is64bit = false
    uint8_t offset_size = 4
    uint8_t address_size = 8
    const uint8_t *die = 0x7f91c899425b (67)
    size_t die_size = 226631
    const uint8_t *abbrev = 0x7f91c89cb7a2 (1)
    const char *str = 0x7f91c89e1f03 ("_UPF_TOK_ASSIGNMENT")
    const char *line_str = 0x7f91c89e3d4b ("/projects/uprintf")
    const uint8_t *str_offsets = NULL
    const uint8_t *addr = NULL
    const uint8_t *rnglists = 0x7f91c89f5eed (233)
    _upf_type_map_vec type_map = {
        uint32_t capacity = 128
        uint32_t length = 120
        _upf_type_map_entry *data = 0x7f91c9467df0 ({
            const uint8_t *die = 0x7f91c8994db1 (15)
            _upf_type *type_ptr = 0x7f91c9464dc0 ({
                const char *name = 0x7f91c89e25a6 ("_upf_arena_region")
                _upf_type_kind kind = _UPF_TK_POINTER (4)
                int modifiers = 0
                size_t size = 8
                union as = <union> {
                    struct cstruct = {
                        _upf_member_vec members = {
                            uint32_t capacity = 3376828976
                            uint32_t length = 32657
                            _upf_member *data = NULL
                        }
                    }
                    struct cenum = {
                        _upf_type *underlying_type = 0x7f91c9465230 (<#0> {
                            const char *name = 0x7f91c89e25a6 ("_upf_arena_region")
                            _upf_type_kind kind = _UPF_TK_STRUCT (0)
                            int modifiers = 0
                            size_t size = 32
                            union as = <union> {
                                struct cstruct = {
                                    _upf_member_vec members = {
                                        uint32_t capacity = 4
                                        uint32_t length = 4
                                        _upf_member *data = 0x7f91c9465080 ({...})
                                    }
                                }
                                struct cenum = {
                                    _upf_type *underlying_type = 0x400000004 (<out-of-bounds>)
                                    _upf_enum_vec enums = {
                                        uint32_t capacity = 3376828544
                                        uint32_t length = 32657
                                        _upf_enum *data = NULL
                                    }
                                }
                                struct array = {
                                    _upf_type *element_type = 0x400000004 (<out-of-bounds>)
                                    _upf_size_t_vec lengths = {
                                        uint32_t capacity = 3376828544
                                        uint32_t length = 32657
                                        size_t *data = NULL
                                    }
                                }
                                struct pointer = {
                                    _upf_type *type = 0x400000004 (<out-of-bounds>)
                                }
                                struct function = {
                                    _upf_type *return_type = 0x400000004 (<out-of-bounds>)
                                    _upf_type_ptr_vec arg_types = {
                                        uint32_t capacity = 3376828544
                                        uint32_t length = 32657
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
                        _upf_type *element_type = 0x7f34ffea18d8 (<points to #0>)
                        _upf_size_t_vec lengths = {
                            uint32_t capacity = 0
                            uint32_t length = 0
                            size_t *data = NULL
                        }
                    }
                    struct pointer = {
                        _upf_type *type = 0x7f34ffea18d8 (<points to #0>)
                    }
                    struct function = {
                        _upf_type *return_type = 0x7f34ffea18d8 (<points to #0>)
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
        _upf_type **data = 0x7f34ffea72a0
    }
    _upf_cu_vec cus = {
        uint32_t capacity = 4
        uint32_t length = 1
        _upf_cu *data = 0x7f34ffea0218 (<#1> {
            const uint8_t *base = 0x7f34fec0611b (190)
            _upf_scope scope = {
                _upf_range_vec ranges = {
                    uint32_t capacity = 16
                    uint32_t length = 12
                    _upf_range *data = 0x7e44fe1fd6d8 ({
                        uint64_t start = 333360
                        uint64_t end = 502059
                    })
                }
                _upf_named_type_vec vars = {
                    uint32_t capacity = 8
                    uint32_t length = 6
                    _upf_named_type *data = 0x7e44fe1fe818 ({
                        const uint8_t *die = 0x7f34fec06158 (77)
                        const char *name = 0x7f34fec52840 ("_upf_test_status")
                    })
                }
                _upf_scope_vec scopes = {
                    uint32_t capacity = 128
                    uint32_t length = 65
                    _upf_scope *data = 0x7f34ffe9e858 ({
                        _upf_range_vec ranges = {
                            uint32_t capacity = 4
                            uint32_t length = 1
                            _upf_range *data = 0x7e44fe1ffb18 ({
                                uint64_t start = 332992
                                uint64_t end = 333044
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
            uint64_t addr_base = 0
            uint64_t str_offsets_base = 18446744073709551615
            uint64_t rnglists_base = 18446744073709551615
            _upf_abbrev_vec abbrevs = {
                uint32_t capacity = 128
                uint32_t length = 101
                _upf_abbrev *data = 0x7e44fe1f9998 ({
                    uint64_t code = 1
                    uint64_t tag = 73
                    _Bool has_children = false
                    _upf_attr_vec attrs = {
                        uint32_t capacity = 4
                        uint32_t length = 2
                        _upf_attr *data = 0x7e44fe1f49d8 ({
                            uint64_t name = 2
                            uint64_t form = 24
                            int64_t implicit_const = 0
                        })
                    }
                })
            }
            _upf_named_type_vec types = {
                uint32_t capacity = 128
                uint32_t length = 123
                _upf_named_type *data = 0x7e44fe1fe018 ({
                    const uint8_t *die = 0x7f34fec06145 (34)
                    const char *name = 0x7f34fec531a4 ("long unsigned int")
                })
            }
            _upf_function_vec functions = {
                uint32_t capacity = 256
                uint32_t length = 169
                _upf_function *data = 0x7f34ffe9a6d8 ({
                    const char *name = 0x7f34fec53010 ("strncmp")
                    const uint8_t *return_type_die = 0x7f34fec06158 (77)
                    _upf_named_type_vec args = {
                        uint32_t capacity = 4
                        uint32_t length = 3
                        _upf_named_type *data = 0x7e44fe1fe898 ({
                            const uint8_t *die = 0x7f34fec06172 (13)
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
        _upf_extern_function *data = 0x7e44fe1f48d8 ({
            const char *name = 0x564bed9dbad6 ("__asan_init")
            uint64_t pc = 139865611240368
        })
    }
    int circular_id = 2
    _upf_range_vec addresses = {
        uint32_t capacity = 128
        uint32_t length = 85
        _upf_range *data = 0x7f34ffea0bd8 ({
            uint64_t start = 2147446784
            uint64_t end = 2415882240
        })
    }
    char *buffer = 0x7dc4fe1e0200 (<out-of-bounds>)
    char *ptr = 0x7dc4fe1e2ad5 (<out-of-bounds>)
    uint32_t size = 16384
    uint32_t free = 5852
    jmp_buf jmp_buf = [
        {
            __jmp_buf __jmpbuf = [0, -3760453226303410042, 140734485823144, 1, 5, 1, -3760453226114666362, -7441794944332025722]
            int __mask_was_saved = 0
            __sigset_t __saved_mask = {
                long unsigned int[] __val = [0 <repeats 16 times>]
            }
        }
    ]
    const char *file_path = 0x564beda5aec0 ("examples/uprintf.c")
    int line = 5
    _upf_token_vec tokens = {
        uint32_t capacity = 4
        uint32_t length = 2
        _upf_token *data = 0x7f34ffea1408 ({
            _upf_token_type type = _UPF_TT_AMPERSAND (28)
            const char *string = 0x564beda5b4c0 ("&")
        })
    }
    size_t tokens_idx = 2
    uint64_t current_pc = 333036
    _upf_cu *current_cu = 0x7f34ffea0218 (<points to #1>)
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
`UPRINTF_IGNORE_STDIO_FILE` | Should `stdio.h`'s `FILE` be ignored | true
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
