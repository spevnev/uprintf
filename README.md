# Universal printf

*uprintf* is a header-only library for printing anything in C/C++.

## Examples

Examples and their outputs can be found in [examples](examples), and can be built using `make examples`.

### Example outputs

<details>
  <summary>avl (<a href="https://github.com/etherealvisage/avl">src</a>)</summary>

```c
AVL tree: {
  avl_tree_node_t *root = 0x7bc351a1ffd0 ({
    avl_tree_node_t *left = 0x7bc3519fffd0 ({
      avl_tree_node_t *left = 0x7bc3519effd0 ({
        avl_tree_node_t *left = 0x7bc3519e7fd0 ({
          avl_tree_node_t *left = 0x7bc3519e3fd0 ({...})
          avl_tree_node_t *right = 0x7bc3519ebfd0 ({...})
          int depth = 10
          void *key = 0x1ff
          void *data = 0x1ff
        })
        avl_tree_node_t *right = 0x7bc3519f7fd0 ({
          avl_tree_node_t *left = 0x7bc3519f3fd0 ({...})
          avl_tree_node_t *right = 0x7bc3519fbfd0 ({...})
          int depth = 10
          void *key = 0x5ff
          void *data = 0x5ff
        })
        int depth = 11
        void *key = 0x3ff
        void *data = 0x3ff
      })
      avl_tree_node_t *right = 0x7bc351a0ffd0 ({
        avl_tree_node_t *left = 0x7bc351a07fd0 ({
          avl_tree_node_t *left = 0x7bc351a03fd0 ({...})
          avl_tree_node_t *right = 0x7bc351a0bfd0 ({...})
          int depth = 10
          void *key = 0x9ff
          void *data = 0x9ff
        })
        avl_tree_node_t *right = 0x7bc351a17fd0 ({
          avl_tree_node_t *left = 0x7bc351a13fd0 ({...})
          avl_tree_node_t *right = 0x7bc351a1bfd0 ({...})
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
    avl_tree_node_t *right = 0x7bc351a3ffd0 ({
      avl_tree_node_t *left = 0x7bc351a2ffd0 ({
        avl_tree_node_t *left = 0x7bc351a27fd0 ({
          avl_tree_node_t *left = 0x7bc351a23fd0 ({...})
          avl_tree_node_t *right = 0x7bc351a2bfd0 ({...})
          int depth = 10
          void *key = 0x11ff
          void *data = 0x11ff
        })
        avl_tree_node_t *right = 0x7bc351a37fd0 ({
          avl_tree_node_t *left = 0x7bc351a33fd0 ({...})
          avl_tree_node_t *right = 0x7bc351a3bfd0 ({...})
          int depth = 10
          void *key = 0x15ff
          void *data = 0x15ff
        })
        int depth = 11
        void *key = 0x13ff
        void *data = 0x13ff
      })
      avl_tree_node_t *right = 0x7bc351a5ffd0 ({
        avl_tree_node_t *left = 0x7bc351a4ffd0 ({
          avl_tree_node_t *left = 0x7bc351a47fd0 ({...})
          avl_tree_node_t *right = 0x7bc351a57fd0 ({...})
          int depth = 11
          void *key = 0x1bff
          void *data = 0x1bff
        })
        avl_tree_node_t *right = 0x7bc351a6ffd0 ({
          avl_tree_node_t *left = 0x7bc351a67fd0 ({...})
          avl_tree_node_t *right = 0x7bc351a77fd0 ({...})
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
  int(void *, void *) comparator = 0x557bc91077ae (int cmp(void *k1, void *k2))
  void(void *) destructor = NULL
}
```
</details>

<details>
  <summary>sqlite (<a href="https://www.sqlite.org">src</a>)</summary>

```c
Sqlite's database: <#0> {
    sqlite3_vfs *pVfs = 0x558311446500 (<#1> {
        int iVersion = 3
        int szOsFile = 120
        int mxPathname = 512
        sqlite3_vfs *pNext = 0x558311097720 ({
            int iVersion = 2
            int szOsFile = 120
            int mxPathname = 1024
            sqlite3_vfs *pNext = 0x5583114466f8 ({
                int iVersion = 3
                int szOsFile = 120
                int mxPathname = 512
                sqlite3_vfs *pNext = 0x558311446650 ({
                    int iVersion = 3
                    int szOsFile = 120
                    int mxPathname = 512
                    sqlite3_vfs *pNext = 0x5583114465a8 ({
                        int iVersion = 3
                        int szOsFile = 120
                        int mxPathname = 512
                        sqlite3_vfs *pNext = NULL
                        const char *zName = 0x558310eeb580 ("unix-none")
                        void *pAppData = 0x5583110491c0
                        int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x5583109d654f (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
                        int(sqlite3_vfs *, const char *, int) xDelete = 0x5583109d738e (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
                        int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x5583109d7655 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
                        int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x5583109d92dc (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
                        void *(sqlite3_vfs *, const char *) xDlOpen = 0x5583109d9879 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
                        void(sqlite3_vfs *, int, char *) xDlError = 0x5583109d989c (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
                        (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x5583109d98f0 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
                        void(sqlite3_vfs *, void *) xDlClose = 0x5583109d9925 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
                        int(sqlite3_vfs *, int, char *) xRandomness = 0x5583109d995a (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
                        int(sqlite3_vfs *, int) xSleep = 0x5583109d9d20 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
                        int(sqlite3_vfs *, double *) xCurrentTime = 0x5583109da128 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
                        int(sqlite3_vfs *, int, char *) xGetLastError = 0x5583109da305 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
                        int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x5583109d9ee4 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
                        int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x5583109c0bed (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
                        (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x5583109c17ba ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
                        const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x5583109c1a92 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
                    })
                    const char *zName = 0x558310eeb5c0 ("unix-dotfile")
                    void *pAppData = 0x5583110492c0
                    int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x5583109d654f (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
                    int(sqlite3_vfs *, const char *, int) xDelete = 0x5583109d738e (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
                    int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x5583109d7655 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
                    int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x5583109d92dc (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
                    void *(sqlite3_vfs *, const char *) xDlOpen = 0x5583109d9879 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
                    void(sqlite3_vfs *, int, char *) xDlError = 0x5583109d989c (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
                    (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x5583109d98f0 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
                    void(sqlite3_vfs *, void *) xDlClose = 0x5583109d9925 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
                    int(sqlite3_vfs *, int, char *) xRandomness = 0x5583109d995a (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
                    int(sqlite3_vfs *, int) xSleep = 0x5583109d9d20 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
                    int(sqlite3_vfs *, double *) xCurrentTime = 0x5583109da128 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
                    int(sqlite3_vfs *, int, char *) xGetLastError = 0x5583109da305 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
                    int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x5583109d9ee4 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
                    int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x5583109c0bed (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
                    (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x5583109c17ba ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
                    const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x5583109c1a92 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
                })
                const char *zName = 0x558310ec9660 ("unix-excl")
                void *pAppData = 0x5583110490c0
                int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x5583109d654f (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
                int(sqlite3_vfs *, const char *, int) xDelete = 0x5583109d738e (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
                int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x5583109d7655 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
                int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x5583109d92dc (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
                void *(sqlite3_vfs *, const char *) xDlOpen = 0x5583109d9879 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
                void(sqlite3_vfs *, int, char *) xDlError = 0x5583109d989c (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
                (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x5583109d98f0 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
                void(sqlite3_vfs *, void *) xDlClose = 0x5583109d9925 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
                int(sqlite3_vfs *, int, char *) xRandomness = 0x5583109d995a (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
                int(sqlite3_vfs *, int) xSleep = 0x5583109d9d20 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
                int(sqlite3_vfs *, double *) xCurrentTime = 0x5583109da128 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
                int(sqlite3_vfs *, int, char *) xGetLastError = 0x5583109da305 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
                int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x5583109d9ee4 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
                int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x5583109c0bed (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
                (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x5583109c17ba ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
                const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x5583109c1a92 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
            })
            const char *zName = 0x558310ec9b20 ("memdb")
            void *pAppData = 0x558311446500
            int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x5583109dd63d (int memdbOpen(sqlite3_vfs *pVfs, const char *zName, sqlite3_file *pFd, int flags, int *pOutFlags))
            int(sqlite3_vfs *, const char *, int) xDelete = NULL
            int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x5583109de31a (int memdbAccess(sqlite3_vfs *pVfs, const char *zPath, int flags, int *pResOut))
            int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x5583109de3a1 (int memdbFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
            void *(sqlite3_vfs *, const char *) xDlOpen = 0x5583109de3e3 (void *memdbDlOpen(sqlite3_vfs *pVfs, const char *zPath))
            void(sqlite3_vfs *, int, char *) xDlError = 0x5583109de53f (void memdbDlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg))
            (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x5583109de6a1 ((void()) memdbDlSym(sqlite3_vfs *pVfs, void *p, const char *zSym))
            void(sqlite3_vfs *, void *) xDlClose = 0x5583109de805 (void memdbDlClose(sqlite3_vfs *pVfs, void *pHandle))
            int(sqlite3_vfs *, int, char *) xRandomness = 0x5583109de962 (int memdbRandomness(sqlite3_vfs *pVfs, int nByte, char *zBufOut))
            int(sqlite3_vfs *, int) xSleep = 0x5583109deac3 (int memdbSleep(sqlite3_vfs *pVfs, int nMicro))
            int(sqlite3_vfs *, double *) xCurrentTime = NULL
            int(sqlite3_vfs *, int, char *) xGetLastError = 0x5583109dec1c (int memdbGetLastError(sqlite3_vfs *pVfs, int a, char *b))
            int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x5583109ded86 (int memdbCurrentTimeInt64(sqlite3_vfs *pVfs, sqlite3_int64 *p))
            int(sqlite3_vfs *, const char *, void()) xSetSystemCall = NULL
            (void())(sqlite3_vfs *, const char *) xGetSystemCall = NULL
            const char *(sqlite3_vfs *, const char *) xNextSystemCall = NULL
        })
        const char *zName = 0x558310eeb540 ("unix")
        void *pAppData = 0x5583110490c0
        int(sqlite3_vfs *, const char *, sqlite3_file *, int, int *) xOpen = 0x5583109d654f (int unixOpen(sqlite3_vfs *pVfs, const char *zPath, sqlite3_file *pFile, int flags, int *pOutFlags))
        int(sqlite3_vfs *, const char *, int) xDelete = 0x5583109d738e (int unixDelete(sqlite3_vfs *NotUsed, const char *zPath, int dirSync))
        int(sqlite3_vfs *, const char *, int, int *) xAccess = 0x5583109d7655 (int unixAccess(sqlite3_vfs *NotUsed, const char *zPath, int flags, int *pResOut))
        int(sqlite3_vfs *, const char *, int, char *) xFullPathname = 0x5583109d92dc (int unixFullPathname(sqlite3_vfs *pVfs, const char *zPath, int nOut, char *zOut))
        void *(sqlite3_vfs *, const char *) xDlOpen = 0x5583109d9879 (void *unixDlOpen(sqlite3_vfs *NotUsed, const char *zFilename))
        void(sqlite3_vfs *, int, char *) xDlError = 0x5583109d989c (void unixDlError(sqlite3_vfs *NotUsed, int nBuf, char *zBufOut))
        (void())(sqlite3_vfs *, void *, const char *) xDlSym = 0x5583109d98f0 ((void()) unixDlSym(sqlite3_vfs *NotUsed, void *p, const char *zSym))
        void(sqlite3_vfs *, void *) xDlClose = 0x5583109d9925 (void unixDlClose(sqlite3_vfs *NotUsed, void *pHandle))
        int(sqlite3_vfs *, int, char *) xRandomness = 0x5583109d995a (int unixRandomness(sqlite3_vfs *NotUsed, int nBuf, char *zBuf))
        int(sqlite3_vfs *, int) xSleep = 0x5583109d9d20 (int unixSleep(sqlite3_vfs *NotUsed, int microseconds))
        int(sqlite3_vfs *, double *) xCurrentTime = 0x5583109da128 (int unixCurrentTime(sqlite3_vfs *NotUsed, double *prNow))
        int(sqlite3_vfs *, int, char *) xGetLastError = 0x5583109da305 (int unixGetLastError(sqlite3_vfs *NotUsed, int NotUsed2, char *NotUsed3))
        int(sqlite3_vfs *, sqlite3_int64 *) xCurrentTimeInt64 = 0x5583109d9ee4 (int unixCurrentTimeInt64(sqlite3_vfs *NotUsed, sqlite3_int64 *piNow))
        int(sqlite3_vfs *, const char *, void()) xSetSystemCall = 0x5583109c0bed (int unixSetSystemCall(sqlite3_vfs *pNotUsed, const char *zName, void() pNewFunc))
        (void())(sqlite3_vfs *, const char *) xGetSystemCall = 0x5583109c17ba ((void()) unixGetSystemCall(sqlite3_vfs *pNotUsed, const char *zName))
        const char *(sqlite3_vfs *, const char *) xNextSystemCall = 0x5583109c1a92 (const char *unixNextSystemCall(sqlite3_vfs *p, const char *zName))
    })
    Vdbe *pVdbe = NULL
    CollSeq *pDfltColl = 0x7c5327be0048 ({
        char *zName = 0x7c5327be00c0 ("BINARY")
        u8 enc = 1
        void *pUser = NULL
        int(void *, int, const void *, int, const void *) xCmp = 0x558310e1fcc2 (int binCollFunc(void *NotUsed, int nKey1, const void *pKey1, int nKey2, const void *pKey2))
        void(void *) xDel = NULL
    })
    sqlite3_mutex *mutex = 0x7bc327be0058 ({
        pthread_mutex_t mutex = <union> <ignored>
    })
    Db *aDb = 0x7d0327be0320 ({
        char *zDbSName = 0x558310ece3a0 ("main")
        Btree *pBt = 0x7bf327be0028 (<#2> {
            sqlite3 *db = 0x7d0327be0088 (<points to #0>)
            BtShared *pBt = 0x7c6327be0048 ({
                Pager *pPager = 0x7d0327be0488 ({
                    sqlite3_vfs *pVfs = 0x558311446500 (<points to #1>)
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
                    sqlite3_file *fd = 0x7d0327be0610 ({
                        const sqlite3_io_methods *pMethods = 0x558311049000 ({
                            int iVersion = 3
                            int(sqlite3_file *) xClose = 0x5583109c7f87 (int unixClose(sqlite3_file *id))
                            int(sqlite3_file *, void *, int, sqlite3_int64) xRead = 0x5583109c8dbf (int unixRead(sqlite3_file *id, void *pBuf, int amt, sqlite3_int64 offset))
                            int(sqlite3_file *, const void *, int, sqlite3_int64) xWrite = 0x5583109c9643 (int unixWrite(sqlite3_file *id, const void *pBuf, int amt, sqlite3_int64 offset))
                            int(sqlite3_file *, sqlite3_int64) xTruncate = 0x5583109ca3ac (int unixTruncate(sqlite3_file *id, i64 nByte))
                            int(sqlite3_file *, int) xSync = 0x5583109c9d95 (int unixSync(sqlite3_file *id, int flags))
                            int(sqlite3_file *, sqlite3_int64 *) xFileSize = 0x5583109ca8c7 (int unixFileSize(sqlite3_file *id, i64 *pSize))
                            int(sqlite3_file *, int) xLock = 0x5583109c5494 (int unixLock(sqlite3_file *id, int eFileLock))
                            int(sqlite3_file *, int) xUnlock = 0x5583109c7d0e (int unixUnlock(sqlite3_file *id, int eFileLock))
                            int(sqlite3_file *, int *) xCheckReservedLock = 0x5583109c44aa (int unixCheckReservedLock(sqlite3_file *id, int *pResOut))
                            int(sqlite3_file *, int, void *) xFileControl = 0x5583109cbb60 (int unixFileControl(sqlite3_file *id, int op, void *pArg))
                            int(sqlite3_file *) xSectorSize = 0x5583109cca1b (int unixSectorSize(sqlite3_file *id))
                            int(sqlite3_file *) xDeviceCharacteristics = 0x5583109ccac3 (int unixDeviceCharacteristics(sqlite3_file *id))
                            int(sqlite3_file *, int, int, int, volatile void **) xShmMap = 0x5583109cfb40 (int unixShmMap(sqlite3_file *fd, int iRegion, int szRegion, int bExtend, volatile void **pp))
                            int(sqlite3_file *, int, int, int) xShmLock = 0x5583109d15a1 (int unixShmLock(sqlite3_file *fd, int ofst, int n, int flags))
                            void(sqlite3_file *) xShmBarrier = 0x5583109d2712 (void unixShmBarrier(sqlite3_file *fd))
                            int(sqlite3_file *, int) xShmUnmap = 0x5583109d2730 (int unixShmUnmap(sqlite3_file *fd, int deleteFlag))
                            int(sqlite3_file *, sqlite3_int64, int, void **) xFetch = 0x5583109d3c54 (int unixFetch(sqlite3_file *fd, i64 iOff, int nAmt, void **pp))
                            int(sqlite3_file *, sqlite3_int64, void *) xUnfetch = 0x5583109d40d1 (int unixUnfetch(sqlite3_file *fd, i64 iOff, void *p))
                        })
                    })
                    sqlite3_file *jfd = 0x7d0327be0700 ({
                        const sqlite3_io_methods *pMethods = NULL
                    })
                    sqlite3_file *sjfd = 0x7d0327be0688 ({
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
                    char *zFilename = 0x7d0327be0784 ("projects/uprintf/test.db")
                    char *zJournal = 0x7d0327be07a7 ("projects/uprintf/test.db-journal")
                    int(void *) xBusyHandler = 0x558310a535e6 (int btreeInvokeBusyHandler(void *pArg))
                    void *pBusyHandlerArg = 0x7c6327be0048
                    u32[] aStat = [0 <repeats 4 times>]
                    void(DbPage *) xReiniter = 0x558310a53519 (void pageReinit(DbPage *pData))
                    int(Pager *, Pgno, DbPage **, int) xGet = 0x558310a17b8a (int getPageNormal(Pager *pPager, Pgno pgno, DbPage **ppPage, int flags))
                    char *pTmpSpace = 0x7d9327be0108 ("��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������"...)
                    PCache *pPCache = 0x7d0327be05c0 ({
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
                        int(void *, PgHdr *) xStress = 0x558310a11f8e (int pagerStress(void *p, PgHdr *pPg))
                        void *pStress = 0x7d0327be0488
                        sqlite3_pcache *pCache = 0x7c7327be0048 ({} <opaque>)
                    })
                    Wal *pWal = NULL
                    char *zWal = 0x7d0327be07d1 ("projects/uprintf/test.db-wal")
                })
                sqlite3 *db = 0x7d0327be0088 (<points to #0>)
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
                void *pSchema = 0x7c4327be0048
                void(void *) xFreeSchema = 0x558310c3fd07 (void sqlite3SchemaClear(void *p))
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
                Btree *pBtree = 0x7bf327be0028 (<points to #2>)
                Pgno iTable = 1
                u8 eLock = 0
                BtLock *pNext = NULL
            }
        })
        u8 safety_level = 3
        u8 bSyncSet = 0
        Schema *pSchema = 0x7c4327be0048 ({
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
        const char **azInit = 0x558311055360
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
    int(void *, sqlite3 *, const char *, int) xWalCallback = 0x558310e2799f (int sqlite3WalDefaultHook(void *pClientData, sqlite3 *db, const char *zDb, int nFrame))
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
        LookasideSlot *pInit = 0x7e6327be8bf8 ({
            LookasideSlot *pNext = 0x7e6327be8748 ({
                LookasideSlot *pNext = 0x7e6327be8298 ({
                    LookasideSlot *pNext = 0x7e6327be7de8 ({
                        LookasideSlot *pNext = 0x7e6327be7938 ({
                            LookasideSlot *pNext = 0x7e6327be7488 ({
                                LookasideSlot *pNext = 0x7e6327be6fd8 ({
                                    LookasideSlot *pNext = 0x7e6327be6b28 ({
                                        LookasideSlot *pNext = 0x7e6327be6678 ({...})
                                    })
                                })
                            })
                        })
                    })
                })
            })
        })
        LookasideSlot *pFree = NULL
        LookasideSlot *pSmallInit = 0x7e6327bebea8 ({
            LookasideSlot *pNext = 0x7e6327bebe28 ({
                LookasideSlot *pNext = 0x7e6327bebda8 ({
                    LookasideSlot *pNext = 0x7e6327bebd28 ({
                        LookasideSlot *pNext = 0x7e6327bebca8 ({
                            LookasideSlot *pNext = 0x7e6327bebc28 ({
                                LookasideSlot *pNext = 0x7e6327bebba8 ({
                                    LookasideSlot *pNext = 0x7e6327bebb28 ({
                                        LookasideSlot *pNext = 0x7e6327bebaa8 ({...})
                                    })
                                })
                            })
                        })
                    })
                })
            })
        })
        LookasideSlot *pSmallFree = NULL
        void *pMiddle = 0x7e6327be90a8
        void *pStart = 0x7e6327be0408
        void *pEnd = 0x7e6327bebf28
        void *pTrueEnd = 0x7e6327bebf28
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
        HashElem *first = 0x7bc327be0218 (<#3> {
            HashElem *next = 0x7bc327be01d8 ({
                HashElem *next = NULL
                HashElem *prev = 0x7bc327be0218 (<points to #3>)
                void *data = 0x7bf327be0098
                const char *pKey = 0x7bf327be00c8 ("json_each")
            })
            HashElem *prev = NULL
            void *data = 0x7bf327be0108
            const char *pKey = 0x7bf327be0138 ("json_tree")
        })
        _ht *ht = NULL
    }
    VtabCtx *pVtabCtx = NULL
    VTable **aVTrans = NULL
    VTable *pDisconnect = NULL
    Hash aFunc = {
        unsigned int htsize = 0
        unsigned int count = 1
        HashElem *first = 0x7bc327be0198 ({
            HashElem *next = NULL
            HashElem *prev = NULL
            void *data = 0x7c0327be00a8
            const char *pKey = 0x7c0327be00f0 ("match")
        })
        _ht *ht = NULL
    }
    Hash aCollSeq = {
        unsigned int htsize = 0
        unsigned int count = 3
        HashElem *first = 0x7bc327be0118 (<#4> {
            HashElem *next = 0x7bc327be00d8 (<#5> {
                HashElem *next = 0x7bc327be0098 ({
                    HashElem *next = NULL
                    HashElem *prev = 0x7bc327be00d8 (<points to #5>)
                    void *data = 0x7c5327be0048
                    const char *pKey = 0x7c5327be00c0 ("BINARY")
                })
                HashElem *prev = 0x7bc327be0118 (<points to #4>)
                void *data = 0x7c5327be0118
                const char *pKey = 0x7c5327be0190 ("NOCASE")
            })
            HashElem *prev = NULL
            void *data = 0x7c5327be01e8
            const char *pKey = 0x7c5327be0260 ("RTRIM")
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
            char *zDbSName = 0x558310ece3a0 ("main")
            Btree *pBt = 0x7bf327be0028 (<points to #2>)
            u8 safety_level = 3
            u8 bSyncSet = 0
            Schema *pSchema = 0x7c4327be0048 ({
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
            char *zDbSName = 0x558310ee05c0 ("temp")
            Btree *pBt = NULL
            u8 safety_level = 1
            u8 bSyncSet = 0
            Schema *pSchema = 0x7c4327be0108 ({
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
    char *vendor = 0x7bcf7b9e0010 ("Lavf58.20.100")
    int comment_list_length = 1
    char **comment_list = 0x7bcf7b9e0030
    FILE *f = 0x7cff7b9e0300 (<ignored>)
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
    Codebook *codebooks = 0x7ecf7b9e0800 ({
        int dimensions = 1
        int entries = 64
        uint8 *codeword_lengths = 0x7c0f7b9e0080 ("\n\a\b\r\t\a\v\n\b\b\f\a\t\b\b\b\r\a\a\b\a\f\f\n\a\b\t\a\t\f\t\n��n̹�n��")
        float minimum_value = 0.000000
        float delta_value = 0.000000
        uint8 value_bits = 0
        uint8 lookup_type = 0
        uint8 sequence_p = 0
        uint8 sparse = 0
        uint32 lookup_values = 0
        codetype *multiplicands = NULL
        uint32 *codewords = 0x7cbf7b9e0040 (0)
        int16[] fast_huffman = [0, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 24, 38, 34, 54, 17, 37, 26, 50, 21, 38, 39, 58, 10, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 8, 37, 22, 42, 25, 38, 40, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 2, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 27, 38, 34, 54, 17, 37, 26, 56, 21, 38, 39, 58, 23, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 9, 37, 22, 42, 25, 38, 48, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 4, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 24, 38, 34, 54, 17, 37, 26, 50, 21, 38, 39, 58, 10, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 19, 37, 22, 42, 25, 38, 40, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 2, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 27, 38, 34, 54, 17, 37, 26, 62, 21, 38, 39, 58, 23, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 9, 37, 22, 42, 25, 38, 51, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, -1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 24, 38, 34, 54, 17, 37, 26, 50, 21, 38, 39, 58, 10, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, -1, 37, 22, 42, 25, 38, 40, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 2, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 27, 38, 34, 54, 17, 37, 26, 56, 21, 38, 39, 58, 23, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 9, 37, 22, 42, 25, 38, 63, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 4, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 24, 38, 34, 54, 17, 37, 26, 50, 21, 38, 39, 58, 10, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 19, 37, 22, 42, 25, 38, 40, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 2, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 43, 21, 38, 39, 58, 6, 37, 22, 42, 25, 38, 35, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 27, 38, 34, 54, 17, 37, 26, 62, 21, 38, 39, 58, 23, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59, 1, 37, 22, 42, 20, 38, 34, 54, 17, 37, 26, 49, 21, 38, 39, 58, 9, 37, 22, 42, 25, 38, 51, 55, 18, 37, 33, 53, 21, 38, 41, 59, 5, 37, 22, 42, 32, 38, 34, 54, 17, 37, 26, 52, 21, 38, 39, 58, 16, 37, 22, 42, 25, 38, 36, 57, 18, 37, 33, 53, 21, 38, 41, 59]
        uint32 *sorted_codewords = 0x7c1f7b9e0020 (4194304)
        int *sorted_values = 0x7c1f7b9e0094 (3)
        int sorted_entries = 17
    })
    int floor_count = 2
    uint16[] floor_types = [1, 1, 0 <repeats 62 times>]
    Floor *floor_config = 0x7d9f7b9e0080 (<union> {
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
    Residue *residue_config = 0x7c0f7b9e0560 ({
        uint32 begin = 0
        uint32 end = 208
        uint32 part_size = 16
        uint8 classifications = 10
        uint8 classbook = 23
        uint8 **classdata = 0x7d2f7b9e1080
        int16[] *residue_books = 0x7c8f7b9e03c0 ([-1 <repeats 8 times>])
    })
    int mapping_count = 2
    Mapping *mapping = 0x7c2f7b9e04a0 ({
        uint16 coupling_steps = 1
        MappingChannel *chan = 0x7bcf7b9e1b30 ({
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
    float[] channel_buffers = [0x7dff7b9e0100 (0.000000), 0x7dff7b9e2900 (0.000000), NULL <repeats 14 times>]
    float[] outputs = [NULL <repeats 16 times>]
    float[] previous_window = [0x7dbf7b9e1500 (0.000000), 0x7dbf7b9e2900 (0.000000), NULL <repeats 14 times>]
    int previous_length = 128
    int16[] finalY = [0x7c0f7b9e05c0 (-16706), 0x7c0f7b9e0620 (-16706), NULL <repeats 14 times>]
    uint32 current_loc = 0
    int current_loc_valid = 1
    float[] A = [0x7cff7b9e1480 (1.000000), 0x7dbf7b9e3d00 (1.000000)]
    float[] B = [0x7cff7b9e1700 (0.499991), 0x7dbf7b9e5100 (0.500000)]
    float[] C = [0x7cbf7b9e1bc0 (0.999699), 0x7d7f7b9e1480 (0.999995)]
    float[] window = [0x7cff7b9e1980 (0.000059), 0x7dbf7b9e6500 (0.000001)]
    uint16[] bit_reverse = [0x7c0f7b9e0680 (0), 0x7cff7b9e1c00 (0)]
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
            ma_node_graph *pNodeGraph = 0x7bb2cc600020 (<points to #1>)
            const ma_node_vtable *vtable = 0x55955d6e8220 ({
                void(ma_node *, const float **, ma_uint32 *, float **, ma_uint32 *) onProcess = 0x55955d31af0a (void ma_node_graph_node_process_pcm_frames(ma_node *pNode, const float **ppFramesIn, ma_uint32 *pFrameCountIn, float **ppFramesOut, ma_uint32 *pFrameCountOut))
                ma_result(ma_node *, ma_uint32, ma_uint32 *) onGetRequiredInputFrameCount = NULL
                ma_uint8 inputBusCount = 0
                ma_uint8 outputBusCount = 1
                ma_uint32 flags = 0
            })
            ma_uint32 inputBusCount = 0
            ma_uint32 outputBusCount = 1
            ma_node_input_bus *pInputBuses = 0x7bb2cc600078 ({
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
            ma_node_output_bus *pOutputBuses = 0x7bb2cc600108 ({
                ma_node *pNode = 0x7bb2cc600020
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
                    ma_node *pNode = 0x7bb2cc600020
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
            ma_node_graph *pNodeGraph = 0x7bb2cc600020 (<points to #1>)
            const ma_node_vtable *vtable = 0x55955d6e8260 ({
                void(ma_node *, const float **, ma_uint32 *, float **, ma_uint32 *) onProcess = 0x55955d31b1a3 (void ma_node_graph_endpoint_process_pcm_frames(ma_node *pNode, const float **ppFramesIn, ma_uint32 *pFrameCountIn, float **ppFramesOut, ma_uint32 *pFrameCountOut))
                ma_result(ma_node *, ma_uint32, ma_uint32 *) onGetRequiredInputFrameCount = NULL
                ma_uint8 inputBusCount = 1
                ma_uint8 outputBusCount = 1
                ma_uint32 flags = 1
            })
            ma_uint32 inputBusCount = 1
            ma_uint32 outputBusCount = 1
            ma_node_input_bus *pInputBuses = 0x7bb2cc6001e0 ({
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
                    ma_node_output_bus *pNext = 0x7d42cd5e7968 (<#3> {
                        ma_node *pNode = 0x7d42cd5e7880
                        ma_uint8 outputBusIndex = 0
                        ma_uint8 channels = 2
                        ma_uint8 inputNodeInputBusIndex = 0
                        ma_uint32 flags = 1
                        ma_uint32 refCount = 0
                        ma_bool32 isAttached = 1
                        ma_spinlock lock = 0
                        float volume = 1.000000
                        ma_node_output_bus *pNext = NULL
                        ma_node_output_bus *pPrev = 0x7bb2cc6001e0 (<points to #2>)
                        ma_node *pInputNode = 0x7bb2cc600188
                    })
                    ma_node_output_bus *pPrev = NULL
                    ma_node *pInputNode = NULL
                }
                ma_uint32 nextCounter = 0
                ma_spinlock lock = 0
                ma_uint8 channels = 2
            })
            ma_node_output_bus *pOutputBuses = 0x7bb2cc600270 ({
                ma_node *pNode = 0x7bb2cc600188
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
            float *pCachedData = 0x7e32cd5e0100 (0.000000)
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
                    ma_node *pNode = 0x7bb2cc600188
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
            void *_pHeap = 0x7e32cd5e0100
            ma_bool32 _ownsHeap = 1
        }
        float *pProcessingCache = 0x7df2cd5e6100 (-0.372549)
        ma_uint32 processingCacheFramesRemaining = 0
        ma_uint32 processingSizeInFrames = 900
        ma_bool32 isReading = 0
        ma_stack *pPreMixStack = 0x7bb2cb2fe800 ({
            size_t offset = 0
            size_t sizeInBytes = 1048576
            unsigned char[] _data = [190]
        })
    }
    ma_resource_manager *pResourceManager = 0x7d42cd5e7380 (<#4> {
        ma_resource_manager_config config = {
            ma_allocation_callbacks allocationCallbacks = {
                void *pUserData = NULL
                void *(size_t, void *) onMalloc = 0x55955d190f98 (void *ma__malloc_default(size_t sz, void *pUserData))
                void *(void *, size_t, void *) onRealloc = 0x55955d190fb6 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
                void(void *, void *) onFree = 0x55955d190fdf (void ma__free_default(void *p, void *pUserData))
            }
            ma_log *pLog = 0x7d42cd5e00f8 (<#5> {
                ma_log_callback[] callbacks = [
                    {
                        void(void *, ma_uint32, const char *) onLog = NULL
                        void *pUserData = NULL
                    } <repeats 4 times>
                ]
                ma_uint32 callbackCount = 0
                ma_allocation_callbacks allocationCallbacks = {
                    void *pUserData = NULL
                    void *(size_t, void *) onMalloc = 0x55955d190f98 (void *ma__malloc_default(size_t sz, void *pUserData))
                    void *(void *, size_t, void *) onRealloc = 0x55955d190fb6 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
                    void(void *, void *) onFree = 0x55955d190fdf (void ma__free_default(void *p, void *pUserData))
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
            ma_vfs *pVFS = 0x7d42cd5e76d8
            ma_decoding_backend_vtable **ppCustomDecodingBackendVTables = NULL
            ma_uint32 customDecodingBackendCount = 0
            void *pCustomDecodingBackendUserData = NULL
        }
        ma_resource_manager_data_buffer_node *pRootDataBufferNode = 0x7c62cd5e10c0 (<#6> {
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
                        const void *pData = 0x7bb2bf034800
                        size_t sizeInBytes = 1798051
                    }
                    struct decoded = {
                        const void *pData = 0x7bb2bf034800
                        ma_uint64 totalFrameCount = 1798051
                        ma_uint64 decodedFrameCount = 0
                        ma_format format = ma_format_unknown (0)
                        ma_uint32 channels = 0
                        ma_uint32 sampleRate = 0
                    }
                    struct decodedPaged = {
                        ma_paged_audio_buffer_data data = {
                            ma_format format = <unknown> (3204663296)
                            ma_uint32 channels = 31666
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
        ma_thread[] jobThreads = [136007660820160, 0 <repeats 63 times>]
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
                ma_slot_allocator_group *pGroups = 0x7ee2cd5e0800 ({
                    ma_uint32 bitfield = 1
                })
                ma_uint32 *pSlots = 0x7ee2cd5e0880 (1)
                ma_uint32 count = 1
                ma_uint32 capacity = 1024
                ma_bool32 _ownsHeap = 0
                void *_pHeap = 0x7ee2cd5e0800
            }
            ma_job *pJobs = 0x7ee2cd5e1880 ({
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
            void *_pHeap = 0x7ee2cd5e0800
            ma_bool32 _ownsHeap = 1
        }
        ma_default_vfs defaultVFS = {
            ma_vfs_callbacks cb = {
                ma_result(ma_vfs *, const char *, ma_uint32, void **) onOpen = 0x55955d2c465d (ma_result ma_default_vfs_open(ma_vfs *pVFS, const char *pFilePath, ma_uint32 openMode, void **pFile))
                ma_result(ma_vfs *, const wchar_t *, ma_uint32, void **) onOpenW = 0x55955d2c4703 (ma_result ma_default_vfs_open_w(ma_vfs *pVFS, const wchar_t *pFilePath, ma_uint32 openMode, void **pFile))
                ma_result(ma_vfs *, void *) onClose = 0x55955d2c47a9 (ma_result ma_default_vfs_close(ma_vfs *pVFS, void *file))
                ma_result(ma_vfs *, void *, void *, size_t, size_t *) onRead = 0x55955d2c47dc (ma_result ma_default_vfs_read(ma_vfs *pVFS, void *file, void *pDst, size_t sizeInBytes, size_t *pBytesRead))
                ma_result(ma_vfs *, void *, const void *, size_t, size_t *) onWrite = 0x55955d2c4889 (ma_result ma_default_vfs_write(ma_vfs *pVFS, void *file, const void *pSrc, size_t sizeInBytes, size_t *pBytesWritten))
                ma_result(ma_vfs *, void *, ma_int64, ma_seek_origin) onSeek = 0x55955d2c4936 (ma_result ma_default_vfs_seek(ma_vfs *pVFS, void *file, ma_int64 offset, ma_seek_origin origin))
                ma_result(ma_vfs *, void *, ma_int64 *) onTell = 0x55955d2c4974 (ma_result ma_default_vfs_tell(ma_vfs *pVFS, void *file, ma_int64 *pCursor))
                ma_result(ma_vfs *, void *, ma_file_info *) onInfo = 0x55955d2c4a11 (ma_result ma_default_vfs_info(ma_vfs *pVFS, void *file, ma_file_info *pInfo))
            }
            ma_allocation_callbacks allocationCallbacks = {
                void *pUserData = NULL
                void *(size_t, void *) onMalloc = 0x55955d190f98 (void *ma__malloc_default(size_t sz, void *pUserData))
                void *(void *, size_t, void *) onRealloc = 0x55955d190fb6 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
                void(void *, void *) onFree = 0x55955d190fdf (void ma__free_default(void *p, void *pUserData))
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
    ma_device *pDevice = 0x7db2cd5e0080 ({
        ma_context *pContext = 0x7d42cd5e0080 ({
            ma_backend_callbacks callbacks = {
                ma_result(ma_context *, const ma_context_config *, ma_backend_callbacks *) onContextInit = 0x55955d1d70dd (ma_result ma_context_init__pulse(ma_context *pContext, const ma_context_config *pConfig, ma_backend_callbacks *pCallbacks))
                ma_result(ma_context *) onContextUninit = 0x55955d1d6a50 (ma_result ma_context_uninit__pulse(ma_context *pContext))
                ma_result(ma_context *, ma_bool32(ma_context *, ma_device_type, const ma_device_info *, void *), void *) onContextEnumerateDevices = 0x55955d1c7e0b (ma_result ma_context_enumerate_devices__pulse(ma_context *pContext, ma_bool32(ma_context *, ma_device_type, const ma_device_info *, void *) callback, void *pUserData))
                ma_result(ma_context *, ma_device_type, const ma_device_id *, ma_device_info *) onContextGetDeviceInfo = 0x55955d1c9d2c (ma_result ma_context_get_device_info__pulse(ma_context *pContext, ma_device_type deviceType, const ma_device_id *pDeviceID, ma_device_info *pDeviceInfo))
                ma_result(ma_device *, const ma_device_config *, ma_device_descriptor *, ma_device_descriptor *) onDeviceInit = 0x55955d1ccf94 (ma_result ma_device_init__pulse(ma_device *pDevice, const ma_device_config *pConfig, ma_device_descriptor *pDescriptorPlayback, ma_device_descriptor *pDescriptorCapture))
                ma_result(ma_device *) onDeviceUninit = 0x55955d1ca33f (ma_result ma_device_uninit__pulse(ma_device *pDevice))
                ma_result(ma_device *) onDeviceStart = 0x55955d1d61aa (ma_result ma_device_start__pulse(ma_device *pDevice))
                ma_result(ma_device *) onDeviceStop = 0x55955d1d649a (ma_result ma_device_stop__pulse(ma_device *pDevice))
                ma_result(ma_device *, void *, ma_uint32, ma_uint32 *) onDeviceRead = NULL
                ma_result(ma_device *, const void *, ma_uint32, ma_uint32 *) onDeviceWrite = NULL
                ma_result(ma_device *) onDeviceDataLoop = 0x55955d1d66ef (ma_result ma_device_data_loop__pulse(ma_device *pDevice))
                ma_result(ma_device *) onDeviceDataLoopWakeup = 0x55955d1d68b7 (ma_result ma_device_data_loop_wakeup__pulse(ma_device *pDevice))
                ma_result(ma_device *, ma_device_type, ma_device_info *) onDeviceGetInfo = NULL
            }
            ma_backend backend = ma_backend_pulseaudio (7)
            ma_log *pLog = 0x7d42cd5e00f8 (<points to #5>)
            ma_log log = <points to #5>
            ma_thread_priority threadPriority = ma_thread_priority_highest (0)
            size_t threadStackSize = 0
            void *pUserData = NULL
            ma_allocation_callbacks allocationCallbacks = {
                void *pUserData = NULL
                void *(size_t, void *) onMalloc = 0x55955d190f98 (void *ma__malloc_default(size_t sz, void *pUserData))
                void *(void *, size_t, void *) onRealloc = 0x55955d190fb6 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
                void(void *, void *) onFree = 0x55955d190fdf (void ma__free_default(void *p, void *pUserData))
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
                    void *asoundSO = 0x7d52cd5e0080
                    void() snd_pcm_open = 0x7fb2cf24a820
                    void() snd_pcm_close = 0x7fb2cf2480a0
                    void() snd_pcm_hw_params_sizeof = 0x7fb2cf248870
                    void() snd_pcm_hw_params_any = 0x7fb2cf2489e0
                    void() snd_pcm_hw_params_set_format = 0x7fb2cf252790
                    void() snd_pcm_hw_params_set_format_first = 0x7fb2cf248150
                    void() snd_pcm_hw_params_get_format_mask = 0x7fb2cf263c60
                    void() snd_pcm_hw_params_set_channels = 0x7fb2cf263df0
                    void() snd_pcm_hw_params_set_channels_near = 0x7fb2cf25fea0
                    void() snd_pcm_hw_params_set_channels_minmax = 0x7fb2cf263cf0
                    void() snd_pcm_hw_params_set_rate_resample = 0x7fb2cf25ff90
                    void() snd_pcm_hw_params_set_rate = 0x7fb2cf260060
                    void() snd_pcm_hw_params_set_rate_near = 0x7fb2cf2601e0
                    void() snd_pcm_hw_params_set_rate_minmax = 0x7fb2cf260130
                    void() snd_pcm_hw_params_set_buffer_size_near = 0x7fb2cf260310
                    void() snd_pcm_hw_params_set_periods_near = 0x7fb2cf263f10
                    void() snd_pcm_hw_params_set_access = 0x7fb2cf263f80
                    void() snd_pcm_hw_params_get_format = 0x7fb2cf260430
                    void() snd_pcm_hw_params_get_channels = 0x7fb2cf262260
                    void() snd_pcm_hw_params_get_channels_min = 0x7fb2cf23cc10
                    void() snd_pcm_hw_params_get_channels_max = 0x7fb2cf23cc20
                    void() snd_pcm_hw_params_get_rate = 0x7fb2cf2409b0
                    void() snd_pcm_hw_params_get_rate_min = 0x7fb2cf23e400
                    void() snd_pcm_hw_params_get_rate_max = 0x7fb2cf238e30
                    void() snd_pcm_hw_params_get_buffer_size = 0x7fb2cf238d00
                    void() snd_pcm_hw_params_get_periods = 0x7fb2cf247640
                    void() snd_pcm_hw_params_get_access = 0x7fb2cf247660
                    void() snd_pcm_hw_params_test_format = 0x7fb2cf24b890
                    void() snd_pcm_hw_params_test_channels = 0x7fb2cf24c0b0
                    void() snd_pcm_hw_params_test_rate = 0x7fb2cf2471a0
                    void() snd_pcm_hw_params = 0x7fb2cf249070
                    void() snd_pcm_sw_params_sizeof = 0x7fb2cf23a220
                    void() snd_pcm_sw_params_current = 0x7fb2cf23a3a0
                    void() snd_pcm_sw_params_get_boundary = 0x7fb2cf23b890
                    void() snd_pcm_sw_params_set_avail_min = 0x7fb2cf258930
                    void() snd_pcm_sw_params_set_start_threshold = 0x7fb2cf258c30
                    void() snd_pcm_sw_params_set_stop_threshold = 0x7fb2cf25aa50
                    void() snd_pcm_sw_params = 0x7fb2cf25ab20
                    void() snd_pcm_format_mask_sizeof = 0x7fb2cf258fc0
                    void() snd_pcm_format_mask_test = 0x7fb2cf2541c0
                    void() snd_pcm_get_chmap = 0x7fb2cf25f250
                    void() snd_pcm_state = 0x7fb2cf25f330
                    void() snd_pcm_prepare = 0x7fb2cf25f520
                    void() snd_pcm_start = 0x7fb2cf262460
                    void() snd_pcm_drop = 0x7fb2cf25f780
                    void() snd_pcm_drain = 0x7fb2cf255360
                    void() snd_pcm_reset = 0x7fb2cf255270
                    void() snd_device_name_hint = 0x7fb2cf255900
                    void() snd_device_name_get_hint = 0x7fb2cf255810
                    void() snd_card_get_index = 0x7fb2cf25f8e0
                    void() snd_device_name_free_hint = 0x7fb2cf25dd20
                    void() snd_pcm_mmap_begin = 0x7fb2cf25d900
                    void() snd_pcm_mmap_commit = 0x7fb2cf25fa20
                    void() snd_pcm_recover = 0x7fb2cf25dae0
                    void() snd_pcm_readi = 0x7fb2cf25e090
                    void() snd_pcm_writei = 0x7fb2cf2544a0
                    void() snd_pcm_avail = 0x7fb2cf25d8e0
                    void() snd_pcm_avail_update = 0x7fb2cf254870
                    void() snd_pcm_wait = 0x7fb2cf254b10
                    void() snd_pcm_nonblock = 0x7fb2cf254d10
                    void() snd_pcm_info = 0x7fb2cf254e30
                    void() snd_pcm_info_sizeof = 0x7cc2cd5e0540
                    void() snd_pcm_info_get_name = 0x7ce2cd5e03c0
                    void() snd_pcm_poll_descriptors = NULL
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
                    void *pulseSO = 0x7d52cd5e0080
                    void() pa_mainloop_new = 0x7fb2cf24a820
                    void() pa_mainloop_free = 0x7fb2cf2480a0
                    void() pa_mainloop_quit = 0x7fb2cf248870
                    void() pa_mainloop_get_api = 0x7fb2cf2489e0
                    void() pa_mainloop_iterate = 0x7fb2cf252790
                    void() pa_mainloop_wakeup = 0x7fb2cf248150
                    void() pa_threaded_mainloop_new = 0x7fb2cf263c60
                    void() pa_threaded_mainloop_free = 0x7fb2cf263df0
                    void() pa_threaded_mainloop_start = 0x7fb2cf25fea0
                    void() pa_threaded_mainloop_stop = 0x7fb2cf263cf0
                    void() pa_threaded_mainloop_lock = 0x7fb2cf25ff90
                    void() pa_threaded_mainloop_unlock = 0x7fb2cf260060
                    void() pa_threaded_mainloop_wait = 0x7fb2cf2601e0
                    void() pa_threaded_mainloop_signal = 0x7fb2cf260130
                    void() pa_threaded_mainloop_accept = 0x7fb2cf260310
                    void() pa_threaded_mainloop_get_retval = 0x7fb2cf263f10
                    void() pa_threaded_mainloop_get_api = 0x7fb2cf263f80
                    void() pa_threaded_mainloop_in_thread = 0x7fb2cf260430
                    void() pa_threaded_mainloop_set_name = 0x7fb2cf262260
                    void() pa_context_new = 0x7fb2cf23cc10
                    void() pa_context_unref = 0x7fb2cf23cc20
                    void() pa_context_connect = 0x7fb2cf2409b0
                    void() pa_context_disconnect = 0x7fb2cf23e400
                    void() pa_context_set_state_callback = 0x7fb2cf238e30
                    void() pa_context_get_state = 0x7fb2cf238d00
                    void() pa_context_get_sink_info_list = 0x7fb2cf247640
                    void() pa_context_get_source_info_list = 0x7fb2cf247660
                    void() pa_context_get_sink_info_by_name = 0x7fb2cf24b890
                    void() pa_context_get_source_info_by_name = 0x7fb2cf24c0b0
                    void() pa_operation_unref = 0x7fb2cf2471a0
                    void() pa_operation_get_state = 0x7fb2cf249070
                    void() pa_channel_map_init_extend = 0x7fb2cf23a220
                    void() pa_channel_map_valid = 0x7fb2cf23a3a0
                    void() pa_channel_map_compatible = 0x7fb2cf23b890
                    void() pa_stream_new = 0x7fb2cf258930
                    void() pa_stream_unref = 0x7fb2cf258c30
                    void() pa_stream_connect_playback = 0x7fb2cf25aa50
                    void() pa_stream_connect_record = 0x7fb2cf25ab20
                    void() pa_stream_disconnect = 0x7fb2cf258fc0
                    void() pa_stream_get_state = 0x7fb2cf2541c0
                    void() pa_stream_get_sample_spec = 0x7fb2cf25f250
                    void() pa_stream_get_channel_map = 0x7fb2cf25f330
                    void() pa_stream_get_buffer_attr = 0x7fb2cf25f520
                    void() pa_stream_set_buffer_attr = 0x7fb2cf262460
                    void() pa_stream_get_device_name = 0x7fb2cf25f780
                    void() pa_stream_set_write_callback = 0x7fb2cf255360
                    void() pa_stream_set_read_callback = 0x7fb2cf255270
                    void() pa_stream_set_suspended_callback = 0x7fb2cf255900
                    void() pa_stream_set_moved_callback = 0x7fb2cf255810
                    void() pa_stream_is_suspended = 0x7fb2cf25f8e0
                    void() pa_stream_flush = 0x7fb2cf25dd20
                    void() pa_stream_drain = 0x7fb2cf25d900
                    void() pa_stream_is_corked = 0x7fb2cf25fa20
                    void() pa_stream_cork = 0x7fb2cf25dae0
                    void() pa_stream_trigger = 0x7fb2cf25e090
                    void() pa_stream_begin_write = 0x7fb2cf2544a0
                    void() pa_stream_write = 0x7fb2cf25d8e0
                    void() pa_stream_peek = 0x7fb2cf254870
                    void() pa_stream_drop = 0x7fb2cf254b10
                    void() pa_stream_writable_size = 0x7fb2cf254d10
                    void() pa_stream_readable_size = 0x7fb2cf254e30
                    void *pMainLoop = 0x7cc2cd5e0540
                    void *pPulseContext = 0x7ce2cd5e03c0
                    char *pApplicationName = NULL
                    char *pServerName = NULL
                }
                struct jack = {
                    void *jackSO = 0x7d52cd5e0080
                    void() jack_client_open = 0x7fb2cf24a820
                    void() jack_client_close = 0x7fb2cf2480a0
                    void() jack_client_name_size = 0x7fb2cf248870
                    void() jack_set_process_callback = 0x7fb2cf2489e0
                    void() jack_set_buffer_size_callback = 0x7fb2cf252790
                    void() jack_on_shutdown = 0x7fb2cf248150
                    void() jack_get_sample_rate = 0x7fb2cf263c60
                    void() jack_get_buffer_size = 0x7fb2cf263df0
                    void() jack_get_ports = 0x7fb2cf25fea0
                    void() jack_activate = 0x7fb2cf263cf0
                    void() jack_deactivate = 0x7fb2cf25ff90
                    void() jack_connect = 0x7fb2cf260060
                    void() jack_port_register = 0x7fb2cf2601e0
                    void() jack_port_name = 0x7fb2cf260130
                    void() jack_port_get_buffer = 0x7fb2cf260310
                    void() jack_free = 0x7fb2cf263f10
                    char *pClientName = 0x7fb2cf263f80 ("��H��tH�?�%�l")
                    ma_bool32 tryStartServer = 3475375152
                }
                struct null_backend = {
                    int _unused = -849477504
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
        void(ma_device *, void *, const void *, ma_uint32) onData = 0x55955d33c425 (void ma_engine_data_callback_internal(ma_device *pDevice, void *pFramesOut, const void *pFramesIn, ma_uint32 frameCount))
        void(const ma_device_notification *) onNotification = NULL
        void(ma_device *) onStop = NULL
        void *pUserData = 0x7bb2cc600020
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
        ma_thread thread = 136007680784064
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
                    ma_channel *pChannelMapIn = 0x7bd2cd5e2910 ("")
                    ma_channel *pChannelMapOut = 0x7bd2cd5e2912 ("")
                    ma_uint8 *pShuffleTable = NULL
                    union weights = <union> {
                        float **f32 = NULL
                        ma_int32 **s16 = NULL
                    }
                    void *_pHeap = 0x7bd2cd5e2910
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
                void *_pHeap = 0x7bd2cd5e2910
            }
            void *pIntermediaryBuffer = 0x7df2cd5e2100
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
                void *pPCMPlayback = 0x7cc2cd5e0a40
                void *pPCMCapture = 0x7ce2cd5e0ac0
                void *pPollDescriptorsPlayback = 0x7d62cd5e0080
                void *pPollDescriptorsCapture = NULL
                int pollDescriptorCountPlayback = 0
                int pollDescriptorCountCapture = 0
                int wakeupfdPlayback = 0
                int wakeupfdCapture = 0
                ma_bool8 isUsingMMapPlayback = 0
                ma_bool8 isUsingMMapCapture = 0
            }
            struct pulse = {
                void *pMainLoop = 0x7cc2cd5e0a40
                void *pPulseContext = 0x7ce2cd5e0ac0
                void *pStreamPlayback = 0x7d62cd5e0080
                void *pStreamCapture = NULL
            }
            struct jack = {
                void *pClient = 0x7cc2cd5e0a40
                void **ppPortsPlayback = 0x7ce2cd5e0ac0
                void **ppPortsCapture = 0x7d62cd5e0080
                float *pIntermediaryBufferPlayback = NULL
                float *pIntermediaryBufferCapture = NULL
            }
            struct null_device = {
                ma_thread deviceThread = 137176110991936
                ma_event operationEvent = {
                    ma_uint32 value = 3445492416
                    ma_pthread_mutex_t lock = <union> {
                        __pthread_mutex_s __data = {
                            int __lock = -849477504
                            unsigned int __count = 32098
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
                        char[] __size = [-128, 0, 94 ('^'), -51, 98 ('b'), 125 ('}'), 0 <repeats 34 times>]
                        long int __align = 137863305756800
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
    ma_log *pLog = 0x7d42cd5e00f8 (<points to #5>)
    ma_uint32 sampleRate = 48000
    ma_uint32 listenerCount = 1
    ma_spatializer_listener[] listeners = [
        {
            ma_spatializer_listener_config config = {
                ma_uint32 channelsOut = 2
                ma_channel *pChannelMapOut = 0x7bd2cd5e40f0 ("\v\f")
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
            void *_pHeap = 0x7bd2cd5e40f0
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
        void *(size_t, void *) onMalloc = 0x55955d190f98 (void *ma__malloc_default(size_t sz, void *pUserData))
        void *(void *, size_t, void *) onRealloc = 0x55955d190fb6 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
        void(void *, void *) onFree = 0x55955d190fdf (void ma__free_default(void *p, void *pUserData))
    }
    ma_bool8 ownsResourceManager = 1
    ma_bool8 ownsDevice = 1
    ma_spinlock inlinedSoundLock = 0
    ma_sound_inlined *pInlinedSoundHead = 0x7d42cd5e7880 ({
        ma_sound sound = {
            ma_engine_node engineNode = {
                ma_node_base baseNode = {
                    ma_node_graph *pNodeGraph = 0x7bb2cc600020 (<points to #1>)
                    const ma_node_vtable *vtable = 0x55955d6f77e0 ({
                        void(ma_node *, const float **, ma_uint32 *, float **, ma_uint32 *) onProcess = 0x55955d335ac5 (void ma_engine_node_process_pcm_frames__sound(ma_node *pNode, const float **ppFramesIn, ma_uint32 *pFrameCountIn, float **ppFramesOut, ma_uint32 *pFrameCountOut))
                        ma_result(ma_node *, ma_uint32, ma_uint32 *) onGetRequiredInputFrameCount = NULL
                        ma_uint8 inputBusCount = 0
                        ma_uint8 outputBusCount = 1
                        ma_uint32 flags = 0
                    })
                    ma_uint32 inputBusCount = 0
                    ma_uint32 outputBusCount = 1
                    ma_node_input_bus *pInputBuses = 0x7d42cd5e78d8 ({
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
                    ma_node_output_bus *pOutputBuses = 0x7d42cd5e7968 (<points to #3>)
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
                    void *_pHeap = 0x7bf2cd5e2b50
                    ma_bool32 _ownsHeap = 0
                }
                ma_engine *pEngine = 0x7bb2cc600020 (<points to #0>)
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
                        float *f32 = 0x7bf2cd5e2b50 (0.000000)
                        ma_int16 *s16 = 0x7bf2cd5e2b50 (0)
                    }
                    union x1 = <union> {
                        float *f32 = 0x7bf2cd5e2b58 (0.000000)
                        ma_int16 *s16 = 0x7bf2cd5e2b58 (0)
                    }
                    ma_lpf lpf = {
                        ma_format format = ma_format_f32 (5)
                        ma_uint32 channels = 2
                        ma_uint32 sampleRate = 1
                        ma_uint32 lpf1Count = 0
                        ma_uint32 lpf2Count = 0
                        ma_lpf1 *pLPF1 = 0x7bf2cd5e2b60 ({
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
                        ma_lpf2 *pLPF2 = 0x7bf2cd5e2b60 ({
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
                        void *_pHeap = 0x7bf2cd5e2b60
                        ma_bool32 _ownsHeap = 0
                    }
                    void *_pHeap = 0x7bf2cd5e2b50
                    ma_bool32 _ownsHeap = 0
                }
                ma_spatializer spatializer = {
                    ma_uint32 channelsIn = 2
                    ma_uint32 channelsOut = 2
                    ma_channel *pChannelMapIn = 0x7bf2cd5e2b60 ("\v\f")
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
                        float *pOldGains = 0x7bf2cd5e2b70 (1.000000)
                        float *pNewGains = 0x7bf2cd5e2b78 (1.000000)
                        void *_pHeap = 0x7bf2cd5e2b70
                        ma_bool32 _ownsHeap = 0
                    }
                    float *pNewChannelGainsOut = 0x7bf2cd5e2b68 (0.000000)
                    void *_pHeap = 0x7bf2cd5e2b60
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
                void *_pHeap = 0x7bf2cd5e2b50
            }
            ma_data_source *pDataSource = 0x7d22cd5e0780
            ma_uint64 seekTarget = 18446744073709551615
            ma_bool32 atEnd = 0
            void *endCallback = NULL
            void *pEndCallbackUserData = NULL
            ma_bool8 ownsDataSource = 1
            ma_resource_manager_data_source *pResourceManagerDataSource = 0x7d22cd5e0780 ({
                union backend = <union> {
                    ma_resource_manager_data_buffer buffer = {
                        ma_data_source_base ds = <#8> {
                            const ma_data_source_vtable *vtable = 0x55955d6dc0a0 ({
                                ma_result(ma_data_source *, void *, ma_uint64, ma_uint64 *) onRead = 0x55955d3063b5 (ma_result ma_resource_manager_data_buffer_cb__read_pcm_frames(ma_data_source *pDataSource, void *pFramesOut, ma_uint64 frameCount, ma_uint64 *pFramesRead))
                                ma_result(ma_data_source *, ma_uint64) onSeek = 0x55955d3063e7 (ma_result ma_resource_manager_data_buffer_cb__seek_to_pcm_frame(ma_data_source *pDataSource, ma_uint64 frameIndex))
                                ma_result(ma_data_source *, ma_format *, ma_uint32 *, ma_uint32 *, ma_channel *, size_t) onGetDataFormat = 0x55955d30640c (ma_result ma_resource_manager_data_buffer_cb__get_data_format(ma_data_source *pDataSource, ma_format *pFormat, ma_uint32 *pChannels, ma_uint32 *pSampleRate, ma_channel *pChannelMap, size_t channelMapCap))
                                ma_result(ma_data_source *, ma_uint64 *) onGetCursor = 0x55955d306454 (ma_result ma_resource_manager_data_buffer_cb__get_cursor_in_pcm_frames(ma_data_source *pDataSource, ma_uint64 *pCursor))
                                ma_result(ma_data_source *, ma_uint64 *) onGetLength = 0x55955d306479 (ma_result ma_resource_manager_data_buffer_cb__get_length_in_pcm_frames(ma_data_source *pDataSource, ma_uint64 *pLength))
                                ma_result(ma_data_source *, ma_bool32) onSetLooping = 0x55955d30649e (ma_result ma_resource_manager_data_buffer_cb__set_looping(ma_data_source *pDataSource, ma_bool32 isLooping))
                                ma_uint32 flags = 0
                            })
                            ma_uint64 rangeBegInFrames = 0
                            ma_uint64 rangeEndInFrames = 18446744073709551615
                            ma_uint64 loopBegInFrames = 0
                            ma_uint64 loopEndInFrames = 18446744073709551615
                            ma_data_source *pCurrent = 0x7d22cd5e0780
                            ma_data_source *pNext = NULL
                            ma_data_source *(ma_data_source *) onGetNext = NULL
                            ma_bool32 isLooping = 0
                        }
                        ma_resource_manager *pResourceManager = 0x7d42cd5e7380 (<points to #4>)
                        ma_resource_manager_data_buffer_node *pNode = 0x7c62cd5e10c0 (<points to #6>)
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
                                    const ma_data_source_vtable *vtable = 0x55955d6bb8c0 ({
                                        ma_result(ma_data_source *, void *, ma_uint64, ma_uint64 *) onRead = 0x55955d2d3233 (ma_result ma_decoder__data_source_on_read(ma_data_source *pDataSource, void *pFramesOut, ma_uint64 frameCount, ma_uint64 *pFramesRead))
                                        ma_result(ma_data_source *, ma_uint64) onSeek = 0x55955d2d3265 (ma_result ma_decoder__data_source_on_seek(ma_data_source *pDataSource, ma_uint64 frameIndex))
                                        ma_result(ma_data_source *, ma_format *, ma_uint32 *, ma_uint32 *, ma_channel *, size_t) onGetDataFormat = 0x55955d2d328a (ma_result ma_decoder__data_source_on_get_data_format(ma_data_source *pDataSource, ma_format *pFormat, ma_uint32 *pChannels, ma_uint32 *pSampleRate, ma_channel *pChannelMap, size_t channelMapCap))
                                        ma_result(ma_data_source *, ma_uint64 *) onGetCursor = 0x55955d2d32d2 (ma_result ma_decoder__data_source_on_get_cursor(ma_data_source *pDataSource, ma_uint64 *pCursor))
                                        ma_result(ma_data_source *, ma_uint64 *) onGetLength = 0x55955d2d32f7 (ma_result ma_decoder__data_source_on_get_length(ma_data_source *pDataSource, ma_uint64 *pLength))
                                        ma_result(ma_data_source *, ma_bool32) onSetLooping = NULL
                                        ma_uint32 flags = 0
                                    })
                                    ma_uint64 rangeBegInFrames = 0
                                    ma_uint64 rangeEndInFrames = 18446744073709551615
                                    ma_uint64 loopBegInFrames = 0
                                    ma_uint64 loopEndInFrames = 18446744073709551615
                                    ma_data_source *pCurrent = 0x7d22cd5e0800
                                    ma_data_source *pNext = NULL
                                    ma_data_source *(ma_data_source *) onGetNext = NULL
                                    ma_bool32 isLooping = 0
                                }
                                ma_data_source *pBackend = 0x7c72cd5e0400
                                const ma_decoding_backend_vtable *pBackendVTable = 0x55955d6ba0e0 ({
                                    ma_result(void *, ma_result(void *, void *, size_t, size_t *), ma_result(void *, ma_int64, ma_seek_origin), ma_result(void *, ma_int64 *), void *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInit = 0x55955d2d024c (ma_result ma_decoding_backend_init__flac(void *pUserData, ma_result(void *, void *, size_t, size_t *) onRead, ma_result(void *, ma_int64, ma_seek_origin) onSeek, ma_result(void *, ma_int64 *) onTell, void *pReadSeekTellUserData, const ma_decoding_backend_config *pConfig, const ma_allocation_callbacks *pAllocationCallbacks, ma_data_source **ppBackend))
                                    ma_result(void *, const char *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitFile = 0x55955d2d033f (ma_result ma_decoding_backend_init_file__flac(void *pUserData, const char *pFilePath, const ma_decoding_backend_config *pConfig, const ma_allocation_callbacks *pAllocationCallbacks, ma_data_source **ppBackend))
                                    ma_result(void *, const wchar_t *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitFileW = 0x55955d2d0415 (ma_result ma_decoding_backend_init_file_w__flac(void *pUserData, const wchar_t *pFilePath, const ma_decoding_backend_config *pConfig, const ma_allocation_callbacks *pAllocationCallbacks, ma_data_source **ppBackend))
                                    ma_result(void *, const void *, size_t, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitMemory = 0x55955d2d04eb (ma_result ma_decoding_backend_init_memory__flac(void *pUserData, const void *pData, size_t dataSize, const ma_decoding_backend_config *pConfig, const ma_allocation_callbacks *pAllocationCallbacks, ma_data_source **ppBackend))
                                    void(void *, ma_data_source *, const ma_allocation_callbacks *) onUninit = 0x55955d2d05cc (void ma_decoding_backend_uninit__flac(void *pUserData, ma_data_source *pBackend, const ma_allocation_callbacks *pAllocationCallbacks))
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
                                        ma_channel *pChannelMapIn = 0x7cb2cd5e0140 ("")
                                        ma_channel *pChannelMapOut = NULL
                                        ma_uint8 *pShuffleTable = NULL
                                        union weights = <union> {
                                            float **f32 = NULL
                                            ma_int32 **s16 = NULL
                                        }
                                        void *_pHeap = 0x7cb2cd5e0140
                                        ma_bool32 _ownsHeap = 0
                                    }
                                    ma_resampler resampler = {
                                        ma_resampling_backend *pBackend = 0x7d22cd5e0928
                                        ma_resampling_backend_vtable *pBackendVTable = 0x55955d688d20 ({
                                            ma_result(void *, const ma_resampler_config *, size_t *) onGetHeapSize = 0x55955d27cf37 (ma_result ma_resampling_backend_get_heap_size__linear(void *pUserData, const ma_resampler_config *pConfig, size_t *pHeapSizeInBytes))
                                            ma_result(void *, const ma_resampler_config *, void *, ma_resampling_backend **) onInit = 0x55955d27d0a6 (ma_result ma_resampling_backend_init__linear(void *pUserData, const ma_resampler_config *pConfig, void *pHeap, ma_resampling_backend **ppBackend))
                                            void(void *, ma_resampling_backend *, const ma_allocation_callbacks *) onUninit = 0x55955d27d36b (void ma_resampling_backend_uninit__linear(void *pUserData, ma_resampling_backend *pBackend, const ma_allocation_callbacks *pAllocationCallbacks))
                                            ma_result(void *, ma_resampling_backend *, const void *, ma_uint64 *, void *, ma_uint64 *) onProcess = 0x55955d27d395 (ma_result ma_resampling_backend_process__linear(void *pUserData, ma_resampling_backend *pBackend, const void *pFramesIn, ma_uint64 *pFrameCountIn, void *pFramesOut, ma_uint64 *pFrameCountOut))
                                            ma_result(void *, ma_resampling_backend *, ma_uint32, ma_uint32) onSetRate = 0x55955d27d3d6 (ma_result ma_resampling_backend_set_rate__linear(void *pUserData, ma_resampling_backend *pBackend, ma_uint32 sampleRateIn, ma_uint32 sampleRateOut))
                                            ma_uint64(void *, const ma_resampling_backend *) onGetInputLatency = 0x55955d27d402 (ma_uint64 ma_resampling_backend_get_input_latency__linear(void *pUserData, const ma_resampling_backend *pBackend))
                                            ma_uint64(void *, const ma_resampling_backend *) onGetOutputLatency = 0x55955d27d420 (ma_uint64 ma_resampling_backend_get_output_latency__linear(void *pUserData, const ma_resampling_backend *pBackend))
                                            ma_result(void *, const ma_resampling_backend *, ma_uint64, ma_uint64 *) onGetRequiredInputFrameCount = 0x55955d27d43e (ma_result ma_resampling_backend_get_required_input_frame_count__linear(void *pUserData, const ma_resampling_backend *pBackend, ma_uint64 outputFrameCount, ma_uint64 *pInputFrameCount))
                                            ma_result(void *, const ma_resampling_backend *, ma_uint64, ma_uint64 *) onGetExpectedOutputFrameCount = 0x55955d27d46f (ma_result ma_resampling_backend_get_expected_output_frame_count__linear(void *pUserData, const ma_resampling_backend *pBackend, ma_uint64 inputFrameCount, ma_uint64 *pOutputFrameCount))
                                            ma_result(void *, ma_resampling_backend *) onReset = 0x55955d27d4a0 (ma_result ma_resampling_backend_reset__linear(void *pUserData, ma_resampling_backend *pBackend))
                                        })
                                        void *pBackendUserData = 0x7d22cd5e0900
                                        ma_format format = ma_format_f32 (5)
                                        ma_uint32 channels = 2
                                        ma_uint32 sampleRateIn = 44100
                                        ma_uint32 sampleRateOut = 48000
                                        union state = <union> {
                                            ma_linear_resampler linear = {
                                                ma_linear_resampler_config config = {
                                                    ma_format format = ma_format_f32 (5)
                                                    ma_uint32 channels = 2
                                                    ma_uint32 sampleRateIn = 147
                                                    ma_uint32 sampleRateOut = 160
                                                    ma_uint32 lpfOrder = 4
                                                    double lpfNyquistFactor = 1.000000
                                                }
                                                ma_uint32 inAdvanceInt = 0
                                                ma_uint32 inAdvanceFrac = 147
                                                ma_uint32 inTimeInt = 1
                                                ma_uint32 inTimeFrac = 0
                                                union x0 = <union> {
                                                    float *f32 = 0x7cb2cd5e0148 (0.000000)
                                                    ma_int16 *s16 = 0x7cb2cd5e0148 (0)
                                                }
                                                union x1 = <union> {
                                                    float *f32 = 0x7cb2cd5e0150 (0.000000)
                                                    ma_int16 *s16 = 0x7cb2cd5e0150 (0)
                                                }
                                                ma_lpf lpf = {
                                                    ma_format format = ma_format_f32 (5)
                                                    ma_uint32 channels = 2
                                                    ma_uint32 sampleRate = 160
                                                    ma_uint32 lpf1Count = 0
                                                    ma_uint32 lpf2Count = 2
                                                    ma_lpf1 *pLPF1 = 0x7cb2cd5e0158 ({
                                                        ma_format format = ma_format_f32 (5)
                                                        ma_uint32 channels = 2
                                                        ma_biquad_coefficient a = <union> <#10> {
                                                            float f32 = 0.797715
                                                            ma_int32 s32 = 1061959439
                                                        }
                                                        ma_biquad_coefficient *pR1 = 0x3fc8da2a3f4c370f (<out-of-bounds>)
                                                        void *_pHeap = 0x3f1f27e6
                                                        ma_bool32 _ownsHeap = 3445490136
                                                    })
                                                    ma_lpf2 *pLPF2 = 0x7cb2cd5e0158 ({
                                                        ma_biquad bq = {
                                                            ma_format format = ma_format_f32 (5)
                                                            ma_uint32 channels = 2
                                                            ma_biquad_coefficient b0 = <union> <points to #10>
                                                            ma_biquad_coefficient b1 = <union> <#11> {
                                                                float f32 = 1.595430
                                                                ma_int32 s32 = 1070348047
                                                            }
                                                            ma_biquad_coefficient b2 = <union> <#12> {
                                                                float f32 = 0.797715
                                                                ma_int32 s32 = 1061959439
                                                            }
                                                            ma_biquad_coefficient a1 = <union> {
                                                                float f32 = 1.569158
                                                                ma_int32 s32 = 1070127658
                                                            }
                                                            ma_biquad_coefficient a2 = <union> {
                                                                float f32 = 0.621703
                                                                ma_int32 s32 = 1059006438
                                                            }
                                                            ma_biquad_coefficient *pR1 = 0x7cb2cd5e01d8 (<union> <#13> {
                                                                float f32 = 0.000000
                                                                ma_int32 s32 = 0
                                                            })
                                                            ma_biquad_coefficient *pR2 = 0x7cb2cd5e01e0 (<union> {
                                                                float f32 = 0.000000
                                                                ma_int32 s32 = 0
                                                            })
                                                            void *_pHeap = 0x7cb2cd5e01d8
                                                            ma_bool32 _ownsHeap = 0
                                                        }
                                                    })
                                                    void *_pHeap = 0x7cb2cd5e0158
                                                    ma_bool32 _ownsHeap = 0
                                                }
                                                void *_pHeap = 0x7cb2cd5e0148
                                                ma_bool32 _ownsHeap = 0
                                            }
                                        }
                                        void *_pHeap = 0x7cb2cd5e0148
                                        ma_bool32 _ownsHeap = 0
                                    }
                                    ma_bool8 hasPreFormatConversion = 0
                                    ma_bool8 hasPostFormatConversion = 0
                                    ma_bool8 hasChannelConverter = 0
                                    ma_bool8 hasResampler = 1
                                    ma_bool8 isPassthrough = 0
                                    ma_bool8 _ownsHeap = 1
                                    void *_pHeap = 0x7cb2cd5e0140
                                }
                                void *pInputCache = NULL
                                ma_uint64 inputCacheCap = 0
                                ma_uint64 inputCacheConsumed = 0
                                ma_uint64 inputCacheRemaining = 0
                                ma_allocation_callbacks allocationCallbacks = {
                                    void *pUserData = NULL
                                    void *(size_t, void *) onMalloc = 0x55955d190f98 (void *ma__malloc_default(size_t sz, void *pUserData))
                                    void *(void *, size_t, void *) onRealloc = 0x55955d190fb6 (void *ma__realloc_default(void *p, size_t sz, void *pUserData))
                                    void(void *, void *) onFree = 0x55955d190fdf (void ma__free_default(void *p, void *pUserData))
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
                                    ma_format format = <unknown> (3445490688)
                                    ma_uint32 channels = 31858
                                    ma_uint32 sampleRate = 1567334624
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
                                ma_paged_audio_buffer_data *pData = 0x7c72cd5e0400 ({
                                    ma_format format = <unknown> (1567328384)
                                    ma_uint32 channels = 21909
                                    ma_paged_audio_buffer_page head = {
                                        ma_paged_audio_buffer_page *pNext = NULL
                                        ma_uint64 sizeInFrames = 18446744073709551615
                                        ma_uint8[] pAudioData = [0]
                                    }
                                    ma_paged_audio_buffer_page *pTail = 0xffffffffffffffff (<out-of-bounds>)
                                })
                                ma_paged_audio_buffer_page *pCurrent = 0x55955d6ba0e0 ({
                                    ma_paged_audio_buffer_page *pNext = 0x55955d2d024c ({
                                        ma_paged_audio_buffer_page *pNext = 0x40ec8348e5894855 (<out-of-bounds>)
                                        ma_uint64 sizeInFrames = 16173984583002196296
                                        ma_uint8[] pAudioData = [72 ('H')]
                                    })
                                    ma_uint64 sizeInFrames = 94100001719103
                                    ma_uint8[] pAudioData = [21]
                                })
                                ma_uint64 relativeCursor = 0
                                ma_uint64 absoluteCursor = 0
                            }
                        }
                    }
                    ma_resource_manager_data_stream stream = {
                        ma_data_source_base ds = <points to #8>
                        ma_resource_manager *pResourceManager = 0x7d42cd5e7380 (<points to #4>)
                        ma_uint32 flags = 3445493952
                        ma_decoder decoder = {
                            ma_data_source_base ds = {
                                const ma_data_source_vtable *vtable = 0x700c (<out-of-bounds>)
                                ma_uint64 rangeBegInFrames = 0
                                ma_uint64 rangeEndInFrames = 0
                                ma_uint64 loopBegInFrames = 0
                                ma_uint64 loopEndInFrames = 4294967296
                                ma_data_source *pCurrent = 0x55955d6bb8c0
                                ma_data_source *pNext = NULL
                                ma_data_source *(ma_data_source *) onGetNext = <out-of-bounds>
                                ma_bool32 isLooping = 0
                            }
                            ma_data_source *pBackend = 0xffffffffffffffff
                            const ma_decoding_backend_vtable *pBackendVTable = 0x7d22cd5e0800 ({
                                ma_result(void *, ma_result(void *, void *, size_t, size_t *), ma_result(void *, ma_int64, ma_seek_origin), ma_result(void *, ma_int64 *), void *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInit = 0x55955d6bb8c0
                                ma_result(void *, const char *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitFile = NULL
                                ma_result(void *, const wchar_t *, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitFileW = <out-of-bounds>
                                ma_result(void *, const void *, size_t, const ma_decoding_backend_config *, const ma_allocation_callbacks *, ma_data_source **) onInitMemory = NULL
                                void(void *, ma_data_source *, const ma_allocation_callbacks *) onUninit = <out-of-bounds>
                            })
                            void *pBackendUserData = NULL
                            void *onRead = NULL
                            ma_result(ma_decoder *, ma_int64, ma_seek_origin) onSeek = NULL
                            ma_result(ma_decoder *, ma_int64 *) onTell = 0x7c72cd5e0400
                            void *pUserData = 0x55955d6ba0e0
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
                                    ma_bool32 _ownsHeap = 3445489984
                                }
                                ma_resampler resampler = {
                                    ma_resampling_backend *pBackend = NULL
                                    ma_resampling_backend_vtable *pBackendVTable = NULL
                                    void *pBackendUserData = NULL
                                    ma_format format = <unknown> (3445489984)
                                    ma_uint32 channels = 31922
                                    ma_uint32 sampleRateIn = 0
                                    ma_uint32 sampleRateOut = 0
                                    union state = <union> {
                                        ma_linear_resampler linear = {
                                            ma_linear_resampler_config config = {
                                                ma_format format = <unknown> (3445492008)
                                                ma_uint32 channels = 32034
                                                ma_uint32 sampleRateIn = 1567132960
                                                ma_uint32 sampleRateOut = 21909
                                                ma_uint32 lpfOrder = 3445491968
                                                double lpfNyquistFactor = 0.000000
                                            }
                                            ma_uint32 inAdvanceInt = 44100
                                            ma_uint32 inAdvanceFrac = 48000
                                            ma_uint32 inTimeInt = 5
                                            ma_uint32 inTimeFrac = 2
                                            union x0 = <union> {
                                                float *f32 = 0xa000000093 (<out-of-bounds>)
                                                ma_int16 *s16 = 0xa000000093 (<out-of-bounds>)
                                            }
                                            union x1 = <union> {
                                                float *f32 = 0x4 (<out-of-bounds>)
                                                ma_int16 *s16 = 0x4 (<out-of-bounds>)
                                            }
                                            ma_lpf lpf = {
                                                ma_format format = ma_format_unknown (0)
                                                ma_uint32 channels = 1072693248
                                                ma_uint32 sampleRate = 0
                                                ma_uint32 lpf1Count = 147
                                                ma_uint32 lpf2Count = 1
                                                ma_lpf1 *pLPF1 = 0x7cb2cd5e0148 ({
                                                    ma_format format = ma_format_unknown (0)
                                                    ma_uint32 channels = 0
                                                    ma_biquad_coefficient a = <union> {
                                                        float f32 = 0.000000
                                                        ma_int32 s32 = 0
                                                    }
                                                    ma_biquad_coefficient *pR1 = 0x200000005 (<out-of-bounds>)
                                                    void *_pHeap = 0x3fcc370f3f4c370f
                                                    ma_bool32 _ownsHeap = 1061959439
                                                })
                                                ma_lpf2 *pLPF2 = 0x7cb2cd5e0150 ({
                                                    ma_biquad bq = {
                                                        ma_format format = ma_format_unknown (0)
                                                        ma_uint32 channels = 0
                                                        ma_biquad_coefficient b0 = <union> {
                                                            float f32 = 0.000000
                                                            ma_int32 s32 = 5
                                                        }
                                                        ma_biquad_coefficient b1 = <union> {
                                                            float f32 = 0.000000
                                                            ma_int32 s32 = 2
                                                        }
                                                        ma_biquad_coefficient b2 = <union> <points to #10>
                                                        ma_biquad_coefficient a1 = <union> <points to #11>
                                                        ma_biquad_coefficient a2 = <union> <points to #12>
                                                        ma_biquad_coefficient *pR1 = 0x3f1f27e6 (<out-of-bounds>)
                                                        ma_biquad_coefficient *pR2 = 0x7cb2cd5e01d8 (<union> <points to #13>)
                                                        void *_pHeap = 0x7cb2cd5e01e0
                                                        ma_bool32 _ownsHeap = 3445490136
                                                    }
                                                })
                                                void *_pHeap = 0x200000005
                                                ma_bool32 _ownsHeap = 160
                                            }
                                            void *_pHeap = 0x2
                                            ma_bool32 _ownsHeap = 3445490008
                                        }
                                    }
                                    void *_pHeap = 0x7cb2cd5e0158
                                    ma_bool32 _ownsHeap = 3445490008
                                }
                                ma_bool8 hasPreFormatConversion = 0
                                ma_bool8 hasPostFormatConversion = 0
                                ma_bool8 hasChannelConverter = 0
                                ma_bool8 hasResampler = 0
                                ma_bool8 isPassthrough = 0
                                ma_bool8 _ownsHeap = 0
                                void *_pHeap = 0x7cb2cd5e0148
                            }
                            void *pInputCache = NULL
                            ma_uint64 inputCacheCap = 137107391512904
                            ma_uint64 inputCacheConsumed = 0
                            ma_uint64 inputCacheRemaining = 1099528404992
                            ma_allocation_callbacks allocationCallbacks = {
                                void *pUserData = 0x7cb2cd5e0140
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
                                    size_t currentReadPos = 94100000411544
                                }
                            }
                        }
                        ma_bool32 isDecoderInitialized = 1561923510
                        ma_uint64 totalLengthInPCMFrames = 94100000411615
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
    _upf_memory_region *allocator = 0x7efd4b8be800 ({
        _upf_memory_region *prev = 0x7efd4c03e800 ({
            _upf_memory_region *prev = 0x7e0d4a3f4800 ({
                _upf_memory_region *prev = NULL
                size_t capacity = 65535
                size_t length = 65456
                uint8_t *data = 0x7e0d4a3f4820 (70)
            })
            size_t capacity = 131070
            size_t length = 131040
            uint8_t *data = 0x7efd4c03e820 (4)
        })
        size_t capacity = 262140
        size_t length = 169064
        uint8_t *data = 0x7efd4b8be820 (67)
    })
    _upf_cstr_vec ignored_structs = {
        uint32_t capacity = 4
        uint32_t length = 3
        const char **data = 0x7e0d4a3f4848
    }
    _Bool is_init = true
    uint32_t map_index = 176
    const uint8_t *base = 0x5649f6da5000 (127)
    uint8_t *elf_file = 0x7afd4a260000 (127)
    off_t elf_file_size = 1561848
    uint8_t offset_size = 4
    uint8_t address_size = 8
    const uint8_t *die = 0x7afd4a3c58ab (254)
    size_t die_size = 37122
    const uint8_t *abbrev = 0x7afd4a3ce9ad (1)
    const char *str = 0x7afd4a3d7ce7 ("__glibc_reserved")
    const char *line_str = 0x7afd4a3d9e71 ("/projects/uprintf")
    const uint8_t *str_offsets = NULL
    const uint8_t *addr = NULL
    const uint8_t *rnglists = 0x7afd4a3d9ffd (254)
    _upf_die_type_map type_map = {
        uint32_t capacity = 192
        uint32_t size = 141
        struct *data = 0x7efd4b8e0530 ({
            uint32_t hash = 1906327296
            const uint8_t *key = 0x7afd4a3c6830 (13)
            _upf_type *value = 0x7efd4b8de4b0 ({
                const char *name = 0x5649f6e66160 ("struct")
                _upf_type_kind kind = _UPF_TK_STRUCT (0)
                int modifiers = 0
                size_t size = 16
                union as = <union> {
                    struct cstruct = {
                        _Bool is_declaration = false
                        _upf_member_vec members = {
                            uint32_t capacity = 4
                            uint32_t length = 3
                            _upf_member *data = 0x7efd4b8de1f0 ({
                                const char *name = 0x7afd4a3d8f87 ("capacity")
                                _upf_type *type = 0x7efd4b8dcbb0 ({
                                    const char *name = 0x7afd4a3d8d10 ("uint32_t")
                                    _upf_type_kind kind = _UPF_TK_U4 (10)
                                    int modifiers = 0
                                    size_t size = 4
                                    union as = <union> {
                                        struct cstruct = {...}
                                        struct cenum = {...}
                                        struct array = {...}
                                        struct pointer = {...}
                                        struct reference = {...}
                                        struct function = {...}
                                    }
                                })
                                size_t offset = 0
                                int bit_size = 0
                                int inheritance = 0
                            })
                        }
                        _upf_method_vec methods = {
                            uint32_t capacity = 0
                            uint32_t length = 0
                            _upf_method *data = NULL
                        }
                    }
                    struct cenum = {
                        _upf_type *underlying_type = NULL
                        _upf_enum_vec enums = {
                            uint32_t capacity = 4
                            uint32_t length = 3
                            _upf_enum *data = 0x7efd4b8de1f0 ({
                                const char *name = 0x7afd4a3d8f87 ("capacity")
                                int64_t value = 139626359409584
                            })
                        }
                    }
                    struct array = {
                        _upf_type *element_type = NULL
                        _upf_size_t_vec lengths = {
                            uint32_t capacity = 4
                            uint32_t length = 3
                            size_t *data = 0x7efd4b8de1f0 (135228290862983)
                        }
                    }
                    struct pointer = {
                        _upf_type *type = NULL
                    }
                    struct reference = {
                        _upf_type *type = NULL
                        _Bool is_rvalue = true
                    }
                    struct function = {
                        _upf_type *return_type = NULL
                        _upf_type_ptr_vec arg_types = {
                            uint32_t capacity = 4
                            uint32_t length = 3
                            _upf_type **data = 0x7efd4b8de1f0
                        }
                        _Bool is_variadic = false
                        const _upf_function *function_ptr = NULL
                    }
                }
            })
        })
    }
    _upf_cu_vec cus = {
        uint32_t capacity = 4
        uint32_t length = 1
        _upf_cu *data = 0x7efd4b8db040 (<#0> {
            _Bool is_cxx = false
            const uint8_t *base = 0x7afd4a3c58ab (254)
            uint64_t base_address = 435529
            uint64_t addr_base = 0
            uint64_t str_offsets_base = 18446744073709551615
            uint64_t rnglists_base = 18446744073709551615
            _upf_abbrev_vec abbrevs = {
                uint32_t capacity = 64
                uint32_t length = 63
                _upf_abbrev *data = 0x7e0d4a3f7598 ({
                    uint64_t tag = 52
                    _Bool has_children = false
                    _upf_attr_vec attrs = {
                        uint32_t capacity = 8
                        uint32_t length = 6
                        _upf_attr *data = 0x7e0d4a3f4a78 ({
                            uint64_t name = 3
                            uint64_t form = 14
                            int64_t implicit_const = 0
                        })
                    }
                })
            }
            _upf_scope scope = {
                _upf_range_vec ranges = {
                    uint32_t capacity = 4
                    uint32_t length = 1
                    _upf_range *data = 0x7e0d4a3fa3d8 ({
                        uint64_t start = 435529
                        uint64_t end = 778665
                    })
                }
                _upf_scope_vec scopes = {
                    uint32_t capacity = 256
                    uint32_t length = 137
                    _upf_scope **data = 0x7efd4b8d9640
                }
                _upf_name_type_map vars = {
                    uint32_t capacity = 0
                    uint32_t size = 0
                    struct *data = NULL
                }
                _upf_name_type_map types = {
                    uint32_t capacity = 0
                    uint32_t size = 0
                    struct *data = NULL
                }
                _upf_ns_vec nss = {
                    uint32_t capacity = 4
                    uint32_t length = 1
                    _upf_ns **data = 0x7efd4b8db020
                }
            }
            _upf_name_function_map extern_functions = {
                uint32_t capacity = 48
                uint32_t size = 30
                struct *data = 0x7e0d4a4029a0 ({
                    uint32_t hash = 2323452720
                    const char *key = 0x7afd4a3d9cae ("_upf_uprintf")
                    _upf_function value = {
                        const char *name = 0x7afd4a3d9cae ("_upf_uprintf")
                        const char *linkage_name = 0x7afd4a3d9cae ("_upf_uprintf")
                        const uint8_t *return_type_die = NULL
                        const uint8_t *specification_die = NULL
                        _upf_named_type_vec args = {
                            uint32_t capacity = 4
                            uint32_t length = 4
                            _upf_named_type *data = 0x7e0d4a403cd0 ({
                                const uint8_t *die = 0x7afd4a3c624d (7)
                                const char *name = 0x7afd4a3d8d28 ("file_path")
                            })
                        }
                        _Bool is_variadic = true
                        _Bool is_external = true
                        uint64_t pc = 771546
                    }
                })
            }
        })
    }
    _upf_pc_cstr_map extern_functions = {
        uint32_t capacity = 12
        uint32_t size = 7
        struct *data = 0x7e0d4a3f48f8 ({
            uint32_t hash = 2903970276
            uint64_t key = 139626370237360
            const char *value = 0x5649f6da5b4e ("__asan_init")
        })
    }
    int struct_id = 1
    _upf_range_vec addresses = {
        uint32_t capacity = 128
        uint32_t length = 85
        _upf_range *data = 0x7efd4b8dba80 ({
            uint64_t start = 2147446784
            uint64_t end = 2415882240
        })
    }
    char *buffer = 0x7d8d4a3e0200 (<out-of-bounds>)
    char *ptr = 0x7d8d4a3e267f (<out-of-bounds>)
    uint32_t size = 16384
    uint32_t free = 6962
    jmp_buf error_jmp_buf = [
        {
            __jmp_buf __jmpbuf = [1, 3969433231574925655, 140732349255256, 1, 139626378383360, 94875674998824, 3969433231375696215, 7239594593855045975]
            int __mask_was_saved = 0
            __sigset_t __saved_mask = {
                long unsigned int[] __val = [0 <repeats 16 times>]
            }
        }
    ]
    const char *file_path = 0x5649f6e6a380 ("examples/uprintf.c")
    int line = 7
    _upf_token_vec tokens = {
        uint32_t capacity = 4
        uint32_t length = 2
        _upf_token *data = 0x7efd4b8dc2d0 ({
            _upf_token_type type = _UPF_TT_AMPERSAND (32)
            const char *string = 0x5649f6e68e40 ("&")
        })
    }
    uint32_t tokens_idx = 2
    _upf_cu *current_cu = 0x7efd4b8db040 (<points to #0>)
    _upf_scope_vec current_scopes = {
        uint32_t capacity = 4
        uint32_t length = 1
        _upf_scope **data = 0x7efd4b8dc280
    }
}
```
</details>

<details>
  <summary>cxxopts (<a href="https://github.com/jarro2783/cxxopts">src</a>)</summary>

```c
Options: {
    string m_program = {
        _Alloc_hider _M_dataplus = {
            char *_M_p = 0x7b3a0ed00810 ("test")
        }
        size_type _M_string_length = 4
        union <anonymous> = <union> {
            char[] _M_local_buf = [116 ('t'), 101 ('e'), 115 ('s'), 116 ('t'), 0 <repeats 12 times>]
            size_type _M_allocated_capacity = 1953719668
        }
    }
    String m_help_string = {
        _Alloc_hider _M_dataplus = {
            char *_M_p = 0x7b6a0fbe0040 ("A brief description")
        }
        size_type _M_string_length = 19
        union <anonymous> = <union> {
            char[] _M_local_buf = [19, 0 <repeats 15 times>]
            size_type _M_allocated_capacity = 19
        }
    }
    string m_custom_help = {
        _Alloc_hider _M_dataplus = {
            char *_M_p = 0x7b3a0ed00850 ("[OPTION...]")
        }
        size_type _M_string_length = 11
        union <anonymous> = <union> {
            char[] _M_local_buf = [91 ('['), 79 ('O'), 80 ('P'), 84 ('T'), 73 ('I'), 79 ('O'), 78 ('N'), 46 ('.'), 46 ('.'), 46 ('.'), 93 (']'), 0 <repeats 5 times>]
            size_type _M_allocated_capacity = 3336691550293872475
        }
    }
    string m_positional_help = {
        _Alloc_hider _M_dataplus = {
            char *_M_p = 0x7b6a0fbe0070 ("positional parameters")
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
        element_type *_M_ptr = 0x7baa0fbe0030 ({
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
                __node_base **_M_buckets = 0x7bea0fbe0040
                size_type _M_bucket_count = 13
                __node_base _M_before_begin = {
                    _Hash_node_base *_M_nxt = 0x7b9a0fbe04a0 ({
                        _Hash_node_base *_M_nxt = 0x7b9a0fbe0320 ({
                            _Hash_node_base *_M_nxt = 0x7b9a0fbe0260 ({
                                _Hash_node_base *_M_nxt = 0x7b9a0fbe0380 ({
                                    _Hash_node_base *_M_nxt = 0x7b9a0fbe0200 ({
                                        _Hash_node_base *_M_nxt = 0x7b9a0fbe0140 ({...})
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
            _Sp_counted_base<(__gnu_cxx::_Lock_policy)2> *_M_pi = 0x7baa0fbe0020 ({
                int(...) *_vptr._Sp_counted_base = 0x557ec5439440
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
            __node_base **_M_buckets = 0x7b3a0ed008f0
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
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_start = 0x7b6a0fbe0100 ({
                _Alloc_hider _M_dataplus = {
                    char *_M_p = 0x7b6a0fbe0110 ("")
                }
                size_type _M_string_length = 0
                union <anonymous> = <union> {
                    char[] _M_local_buf = [0, -66 <repeats 15 times>]
                    size_type _M_allocated_capacity = 13744632839234567680
                }
            })
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_finish = 0x7b6a0fbe0120 (<#0> {
                _Alloc_hider _M_dataplus = {
                    char *_M_p = 0x2000000202 (<out-of-bounds>)
                }
                size_type _M_string_length = 26
                union <anonymous> = <union> {
                    char[] _M_local_buf = [64 ('@'), 1, -66, 15, 106 ('j'), 123 ('{'), 0, 0, 3, 0 <repeats 7 times>]
                    size_type _M_allocated_capacity = 135695460860224
                }
            })
            basic_string<char, std::char_traits<char>, std::allocator<char> > *_M_end_of_storage = 0x7b6a0fbe0120 (<points to #0>)
        }
    }
    map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, cxxopts::HelpGroupDetails, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, cxxopts::HelpGroupDetails> > > m_help = {
        _Rep_type _M_t = {
            _Rb_tree_impl<std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, true> _M_impl = {
                less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > _M_key_compare = {}
                _Rb_tree_node_base _M_header = {
                    _Rb_tree_color _M_color = _S_red (0)
                    void *_M_parent = 0x7c1a0fbe0040
                    void *_M_left = 0x7c1a0fbe0040
                    void *_M_right = 0x7c1a0fbe0040
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
            __node_base **_M_buckets = 0x7bea0fbe01a0
            size_type _M_bucket_count = 13
            __node_base _M_before_begin = {
                _Hash_node_base *_M_nxt = 0x7b9a0fbe0980 ({
                    _Hash_node_base *_M_nxt = 0x7b9a0fbe08c0 ({
                        _Hash_node_base *_M_nxt = 0x7b9a0fbe0860 ({
                            _Hash_node_base *_M_nxt = 0x7b9a0fbe0800 ({
                                _Hash_node_base *_M_nxt = 0x7b9a0fbe07a0 ({
                                    _Hash_node_base *_M_nxt = 0x7b9a0fbe0740 ({
                                        _Hash_node_base *_M_nxt = 0x7b9a0fbe0920 ({...})
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
            __node_base **_M_buckets = 0x7bea0fbe00f0
            size_type _M_bucket_count = 13
            __node_base _M_before_begin = {
                _Hash_node_base *_M_nxt = 0x7b9a0fbe0680 ({
                    _Hash_node_base *_M_nxt = 0x7b9a0fbe0620 ({
                        _Hash_node_base *_M_nxt = 0x7b9a0fbe05c0 (<#0> {
                            _Hash_node_base *_M_nxt = 0x7b9a0fbe0500 ({
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
            KeyValue *_M_start = 0x7c4a0fbe02c0 ({
                string m_key = {
                    _Alloc_hider _M_dataplus = {
                        char *_M_p = 0x7c4a0fbe02d0 ("help")
                    }
                    size_type _M_string_length = 4
                    union <anonymous> = <union> {
                        char[] _M_local_buf = [104 ('h'), 101 ('e'), 108 ('l'), 112 ('p'), 0, -66 <repeats 11 times>]
                        size_type _M_allocated_capacity = 13744632021876761960
                    }
                }
                string m_value = {
                    _Alloc_hider _M_dataplus = {
                        char *_M_p = 0x7c4a0fbe02f0 ("false")
                    }
                    size_type _M_string_length = 5
                    union <anonymous> = <union> {
                        char[] _M_local_buf = [102 ('f'), 97 ('a'), 108 ('l'), 115 ('s'), 101 ('e'), 0, -66 <repeats 10 times>]
                        size_type _M_allocated_capacity = 13744423548509512038
                    }
                }
            })
            KeyValue *_M_finish = 0x7c4a0fbe0380 ({
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
            KeyValue *_M_end_of_storage = 0x7c4a0fbe03c0 ({
                string m_key = {
                    _Alloc_hider _M_dataplus = {
                        char *_M_p = 0xcc6e96b9cc6e96b9 (<out-of-bounds>)
                    }
                    size_type _M_string_length = 136657533535216
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
                        char[] _M_local_buf = [3, 17, 0, 0, -16, 0, 0, 0, 97 ('a'), 0 <repeats 7 times>]
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
            void *_M_start = 0x7e25b99e0400
            void *_M_finish = 0x7e25b99e9c00
            void *_M_end_of_storage = 0x7e25b99e9c00
        }
    }
    DefaultNotifier _notifier = {
        atomic<long unsigned int> _state = {
            __int_type _M_i = 4398046511116
        }
        vector<tf::NonblockingNotifierV2::Waiter, std::allocator<tf::NonblockingNotifierV2::Waiter> > _waiters = {
            _Vector_impl _M_impl = {
                Waiter *_M_start = 0x7d25b99e0080 ({
                    atomic<long unsigned int> next = {
                        __int_type _M_i = 4
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
                    }
                    unsigned int state = 1
                })
                Waiter *_M_finish = 0x7d25b99e0880 (<#0> {
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
                    unsigned int state = 0
                })
                Waiter *_M_end_of_storage = 0x7d25b99e0880 (<points to #0>)
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
                _List_node_base *_M_next = 0x7b55b89006e0 (<#1> {
                    _List_node_base *_M_next = 0x7b55b89006e0 (<points to #1>)
                    _List_node_base *_M_prev = 0x7b55b89006e0 (<points to #1>)
                })
                _List_node_base *_M_prev = 0x7b55b89006e0 (<points to #1>)
                size_t _M_size = 0
            }
        }
    }
    Freelist<tf::Node*> _buffers = {
        vector<tf::Freelist<tf::Node*>::Bucket, std::allocator<tf::Freelist<tf::Node*>::Bucket> > _buckets = {
            _Vector_impl _M_impl = {
                Bucket *_M_start = 0x7d05b99e0080 ({
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
                                Array *_M_p = 0x7b85b99e0040 ({
                                    int64_t C = 1024
                                    int64_t M = 1023
                                    atomic<tf::Node*> *S = 0x7da5b99e0100 ({
                                        __base_type _M_b = {
                                            Node *_M_p = 0xbebebebebebebebe (<out-of-bounds>)
                                        }
                                    })
                                })
                            }
                        }
                        vector<tf::UnboundedTaskQueue<tf::Node*>::Array*, std::allocator<tf::UnboundedTaskQueue<tf::Node*>::Array*> > _garbage = {
                            _Vector_impl _M_impl = {
                                Array **_M_start = 0x7c65b99e0040
                                Array **_M_finish = 0x7c65b99e0040
                                Array **_M_end_of_storage = 0x7c65b99e0140
                            }
                        }
                    }
                })
                Bucket *_M_finish = 0x7d05b99e0680 (<#2> {
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
                Bucket *_M_end_of_storage = 0x7d05b99e0680 (<points to #2>)
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
            __node_base **_M_buckets = 0x7b55b8900750
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
taskflow: <#0> {
    Graph &_graph = <#1> {
        _Vector_impl _M_impl = {
            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_start = 0x7b85b99e0430 ({
                __uniq_ptr_data<tf::Node, std::default_delete<tf::Node>, true, true> _M_t = {
                    tuple<tf::Node*, std::default_delete<tf::Node> > _M_t = {
                        default_delete<tf::Node> _M_head_impl = {}
                        Node *_M_head_impl = 0x7c65b99e0900 ({
                            nstate_t _nstate = 0
                            atomic<int> _estate = {
                                __int_type _M_i = 0
                            }
                            string _name = {
                                _Alloc_hider _M_dataplus = {
                                    char *_M_p = 0x7c65b99e0918 ("")
                                }
                                size_type _M_string_length = 0
                                union <anonymous> = <union> {
                                    char[] _M_local_buf = [0, -66 <repeats 15 times>]
                                    size_type _M_allocated_capacity = 13744632839234567680
                                }
                            }
                            void *_data = NULL
                            Topology *_topology = 0x7c25b99e0050 ({
                                Taskflow &_taskflow = <points to #0>
                                promise<void> _promise = {
                                    shared_ptr<std::__future_base::_State_baseV2> _M_future = {
                                        element_type *_M_ptr = 0x7b95b99e0020 ({
                                            int(...) *_vptr._State_baseV2 = 0x564c5aa489c0
                                            _Ptr_type _M_result = {
                                                __uniq_ptr_data<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter, true, true> _M_t = {
                                                    tuple<std::__future_base::_Result_base*, std::__future_base::_Result_base::_Deleter> _M_t = {
                                                        _Deleter _M_head_impl = {}
                                                        _Result_base *_M_head_impl = NULL
                                                    }
                                                }
                                            }
                                            __atomic_futex_unsigned<2147483648> _M_status = {
                                                atomic<unsigned int> _M_data = {
                                                    __int_type _M_i = 1
                                                }
                                            }
                                            atomic_flag _M_retrieved = {
                                                __atomic_flag_data_type _M_i = true
                                            }
                                            once_flag _M_once = {
                                                __gthread_once_t _M_once = 2
                                            }
                                        })
                                        __shared_count<(__gnu_cxx::_Lock_policy)2> _M_refcount = {
                                            _Sp_counted_base<(__gnu_cxx::_Lock_policy)2> *_M_pi = 0x7b95b99e0010 ({
                                                int(...) *_vptr._Sp_counted_base = 0xd0000002b
                                                _Atomic_word _M_use_count = 0
                                                _Atomic_word _M_weak_count = 0
                                            })
                                        }
                                    }
                                    _Ptr_type _M_storage = {
                                        __uniq_ptr_data<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter, true, true> _M_t = {
                                            tuple<std::__future_base::_Result<void>*, std::__future_base::_Result_base::_Deleter> _M_t = {
                                                _Deleter _M_head_impl = {}
                                                _Result<void> *_M_head_impl = NULL
                                            }
                                        }
                                    }
                                }
                                function<bool()> _pred = {
                                    _Any_data _M_functor = <union> {
                                        _Nocopy_types _M_unused = <union> {
                                            void *_M_object = 0xffffffffffffffff
                                            const void *_M_const_object = 0xffffffffffffffff
                                            void() _M_function_pointer = <out-of-bounds>
                                            <unnamed> _M_member_pointer = <member pointer>
                                        }
                                        char[] _M_pod_data = [-1 <repeats 8 times>, 0 <repeats 8 times>]
                                    }
                                    bool(_Any_data &, const _Any_data &, _Manager_operation) _M_manager = 0x564c5a9cf73f
                                    bool(const _Any_data &) _M_invoker = 0x564c5a9cf6f0
                                }
                                function<void()> _call = {
                                    _Any_data _M_functor = <union> {
                                        _Nocopy_types _M_unused = <union> {
                                            void *_M_object = NULL
                                            const void *_M_const_object = NULL
                                            void() _M_function_pointer = NULL
                                            <unnamed> _M_member_pointer = <member pointer>
                                        }
                                        char[] _M_pod_data = [0 <repeats 16 times>]
                                    }
                                    bool(_Any_data &, const _Any_data &, _Manager_operation) _M_manager = 0x564c5a9cf960
                                    void(const _Any_data &) _M_invoker = 0x564c5a9cf91c
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
                                void *BeginX = 0x7c65b99e0960
                                void *EndX = 0x7c65b99e0970
                                void *CapacityX = 0x7c65b99e0980
                                U FirstEl = {
                                    byte[] buff = [<unknown> (192), <unknown> (7), <unknown> (158), <unknown> (185), <unknown> (101 ('e')), <unknown> (124 ('|')), <unknown> (0), <unknown> (0)]
                                }
                                SmallVectorStorage<tf::Node*, 4> Storage = {
                                    U[] InlineElts = [
                                        {
                                            byte[] buff = [<unknown> (128), <unknown> (6), <unknown> (158), <unknown> (185), <unknown> (101 ('e')), <unknown> (124 ('|')), <unknown> (0), <unknown> (0)]
                                        },
                                        {
                                            byte[] buff = [<unknown> (190) <repeats 8 times>]
                                        },
                                        {
                                            byte[] buff = [<unknown> (190) <repeats 8 times>]
                                        }
                                    ]
                                }
                            }
                            atomic<long unsigned int> _join_counter = {
                                __int_type _M_i = 0
                            }
                            handle_t _handle = {
                                _Variadic_union<false, std::monostate, tf::Node::Static, tf::Node::Runtime, tf::Node::Subflow, tf::Node::Condition, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_u = <union> {
                                    _Uninitialized<std::monostate, true> _M_first = {
                                        monostate _M_storage = {}
                                    }
                                    _Variadic_union<false, tf::Node::Static, tf::Node::Runtime, tf::Node::Subflow, tf::Node::Condition, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_rest = <union> {
                                        _Uninitialized<tf::Node::Static, false> _M_first = {
                                            __aligned_membuf<tf::Node::Static> _M_storage = {
                                                unsigned char[] _M_storage = [0 <repeats 16 times>, 243, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0, 175, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0]
                                            }
                                        }
                                        _Variadic_union<false, tf::Node::Runtime, tf::Node::Subflow, tf::Node::Condition, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_rest = <union> {
                                            _Uninitialized<tf::Node::Runtime, false> _M_first = {
                                                __aligned_membuf<tf::Node::Runtime> _M_storage = {
                                                    unsigned char[] _M_storage = [0 <repeats 16 times>, 243, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0, 175, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0]
                                                }
                                            }
                                            _Variadic_union<false, tf::Node::Subflow, tf::Node::Condition, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_rest = <union> {
                                                _Uninitialized<tf::Node::Subflow, false> _M_first = {
                                                    __aligned_membuf<tf::Node::Subflow> _M_storage = {
                                                        unsigned char[] _M_storage = [0 <repeats 16 times>, 243, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0, 175, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0, 190 <repeats 24 times>]
                                                    }
                                                }
                                                _Variadic_union<false, tf::Node::Condition, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_rest = <union> {
                                                    _Uninitialized<tf::Node::Condition, false> _M_first = {
                                                        __aligned_membuf<tf::Node::Condition> _M_storage = {
                                                            unsigned char[] _M_storage = [0 <repeats 16 times>, 243, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0, 175, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0]
                                                        }
                                                    }
                                                    _Variadic_union<false, tf::Node::MultiCondition, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_rest = <union> {
                                                        _Uninitialized<tf::Node::MultiCondition, false> _M_first = {
                                                            __aligned_membuf<tf::Node::MultiCondition> _M_storage = {
                                                                unsigned char[] _M_storage = [0 <repeats 16 times>, 243, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0, 175, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0]
                                                            }
                                                        }
                                                        _Variadic_union<false, tf::Node::Module, tf::Node::Async, tf::Node::DependentAsync> _M_rest = <union> {
                                                            _Uninitialized<tf::Node::Module, true> _M_first = {
                                                                Module _M_storage = {
                                                                    Graph &graph = {
                                                                        _Vector_impl _M_impl = {
                                                                            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_start = NULL
                                                                            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_finish = NULL
                                                                            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_end_of_storage = 0x564c5a913af3 ({
                                                                                __uniq_ptr_data<tf::Node, std::default_delete<tf::Node>, true, true> _M_t = {
                                                                                    tuple<tf::Node*, std::default_delete<tf::Node> > _M_t = {
                                                                                        default_delete<tf::Node> _M_head_impl = {}
                                                                                        Node *_M_head_impl = 0x48535441e5894855 (<out-of-bounds>)
                                                                                    }
                                                                                }
                                                                            })
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                            _Variadic_union<false, tf::Node::Async, tf::Node::DependentAsync> _M_rest = <union> {
                                                                _Uninitialized<tf::Node::Async, false> _M_first = {
                                                                    __aligned_membuf<tf::Node::Async> _M_storage = {
                                                                        unsigned char[] _M_storage = [0 <repeats 16 times>, 243, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0, 175, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0, 190 <repeats 8 times>]
                                                                    }
                                                                }
                                                                _Variadic_union<false, tf::Node::DependentAsync> _M_rest = <union> {
                                                                    _Uninitialized<tf::Node::DependentAsync, false> _M_first = {
                                                                        __aligned_membuf<tf::Node::DependentAsync> _M_storage = {
                                                                            unsigned char[] _M_storage = [0 <repeats 16 times>, 243, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0, 175, 58 (':'), 145, 90 ('Z'), 76 ('L'), 86 ('V'), 0, 0, 190 <repeats 24 times>]
                                                                        }
                                                                    }
                                                                    _Variadic_union<false> _M_rest = <union> {}
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                __index_type _M_index = 1
                            }
                            unique_ptr<tf::Node::Semaphores, std::default_delete<tf::Node::Semaphores> > _semaphores = {
                                __uniq_ptr_data<tf::Node::Semaphores, std::default_delete<tf::Node::Semaphores>, true, true> _M_t = {
                                    tuple<tf::Node::Semaphores*, std::default_delete<tf::Node::Semaphores> > _M_t = {
                                        default_delete<tf::Node::Semaphores> _M_head_impl = {}
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
            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_finish = 0x7b85b99e0450 (<#2> {
                __uniq_ptr_data<tf::Node, std::default_delete<tf::Node>, true, true> _M_t = {
                    tuple<tf::Node*, std::default_delete<tf::Node> > _M_t = {
                        default_delete<tf::Node> _M_head_impl = {}
                        Node *_M_head_impl = NULL
                    }
                }
            })
            unique_ptr<tf::Node, std::default_delete<tf::Node> > *_M_end_of_storage = 0x7b85b99e0450 (<points to #2>)
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
            char *_M_p = 0x7b55b8900550 ("")
        }
        size_type _M_string_length = 0
        union <anonymous> = <union> {
            char[] _M_local_buf = [0 <repeats 16 times>]
            size_type _M_allocated_capacity = 0
        }
    }
    Graph _graph = <points to #1>
    queue<std::shared_ptr<tf::Topology>, std::deque<std::shared_ptr<tf::Topology>, std::allocator<std::shared_ptr<tf::Topology> > > > _topologies = {
        deque<std::shared_ptr<tf::Topology>, std::allocator<std::shared_ptr<tf::Topology> > > c = {
            _Deque_impl _M_impl = {
                shared_ptr<tf::Topology> **_M_map = 0x7bb5b99e0080
                size_t _M_map_size = 8
                iterator _M_start = {
                    shared_ptr<tf::Topology> *_M_cur = 0x7ca5b99e0310 (<#3> {
                        element_type *_M_ptr = 0xbebebebebebebebe (<out-of-bounds>)
                        __shared_count<(__gnu_cxx::_Lock_policy)2> _M_refcount = {
                            _Sp_counted_base<(__gnu_cxx::_Lock_policy)2> *_M_pi = 0xbebebebebebebebe (<out-of-bounds>)
                        }
                    })
                    shared_ptr<tf::Topology> *_M_first = 0x7ca5b99e0300 (<#4> {
                        element_type *_M_ptr = NULL
                        __shared_count<(__gnu_cxx::_Lock_policy)2> _M_refcount = {
                            _Sp_counted_base<(__gnu_cxx::_Lock_policy)2> *_M_pi = NULL
                        }
                    })
                    shared_ptr<tf::Topology> *_M_last = 0x7ca5b99e0500 (<#5> {
                        element_type *_M_ptr = 0xcc6e96b9cc6e96b9 (<out-of-bounds>)
                        __shared_count<(__gnu_cxx::_Lock_policy)2> _M_refcount = {
                            _Sp_counted_base<(__gnu_cxx::_Lock_policy)2> *_M_pi = 0x7ca5b99e0570 ({
                                int(...) *_vptr._Sp_counted_base = 0x1d800001103
                                _Atomic_word _M_use_count = 41
                                _Atomic_word _M_weak_count = 0
                            })
                        }
                    })
                    shared_ptr<tf::Topology> **_M_node = 0x7bb5b99e0098
                }
                iterator _M_finish = {
                    shared_ptr<tf::Topology> *_M_cur = 0x7ca5b99e0310 (<points to #3>)
                    shared_ptr<tf::Topology> *_M_first = 0x7ca5b99e0300 (<points to #4>)
                    shared_ptr<tf::Topology> *_M_last = 0x7ca5b99e0500 (<points to #5>)
                    shared_ptr<tf::Topology> **_M_node = 0x7bb5b99e0098
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
- Minimum C version is c99 (tests require c11)
- Minimum C++ version is c++11
- Executable must be compiled with debugging information (`-g2` or `-g3`)

## Usage

1. Download the header file

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

- Dynamic arrays can only be printed as pointers because there is no information about its length.
    ```c
    int *arr = malloc(sizeof(*arr) * 3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    uprintf("%S\n", &arr); // 0x12345678 (1)
    ```

- Printing information about function from a shared library only works with gcc:
    ```c
    uprintf("%S\n", printf);
    // gcc  : 0x12345678 (int printf(const char *, ...))
    // clang: 0x12345678
    ```

- Incorrect type deduction when printing variables that get shadowed **later in the same scope**.
    ```c
    char var = 'c';
    {
        uprintf("%S\n", &var); // interprets char as an int
        int var = 1;
        uprintf("%S\n", &var); // prints int
    }
    ```

- Casting to struct/union defined in arguments is not supported.
    ```c
    uprintf("%S\n", &(struct { int i; float f; }) { 1, 2.3F }); // unsupported
    ```

- Some functions and types may get inlined and have debugging information omitted by the compiler.

### C++

- `typeid` is not supported.
    ```cpp
    int x = 10;
    uprintf("%S\n", &typeid(x)); // unsupported
    ```

- `decltype` in arguments is not supported.
    ```cpp
    int x = 10;
    int y = 20;
    uprintf("%S\n", (decltype(x) *) &y); // unsupported
    ```

- Lambda definitions in arguments is not supported.
    ```cpp
    uprintf("%S\n", [&]() { ... }()); // unsupported
    ```

- Templated variable and functions in arguments are not supported.
    ```cpp
    template <typename T>
    T var = T{};

    template <typename T>
    T foo(T a) { return a; }

    uprintf("%S\n", &var<int>); // unsupported
    uprintf("%S\n", &foo<int>); // unsupported
    ```

- Invoking overloaded operators in arguments is not supported.
      ```cpp
      T operator+(const T &a, const T &b);

      T a, b;
      // + expects integers ignoring the overloaded version.
      uprintf("%S\n", a + b);
      ```

## How it works

The library uses debugging information to get type information required for printing.

On initialization:
1. Read the current binary.
2. Parse ELF and DWARF.
3. Store metadata about scopes and types.

On each invocation:
1. Identify current scope based on the PC.
2. Parse the arguments string.
3. Look up arguments' types based on current scopes.
4. Print the data using type information.

## Testing

### Tested compilers

- gcc & g++: 11, 12, 13, 14
- clang & clang++: 14, 15, 16, 17, 18

### Running tests

```console
$ make test
```
