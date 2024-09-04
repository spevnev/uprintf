A list of unimplemented features

### Alignment options
```c
// Unaligned
{
    int a = 1
    int number = 2
    void *p = 0x123
}

// Align values/equals
{
    int a      = 1
    int number = 2
    void *p    = 0x123
}

// Align names
{
    int   a = 1
    int   number = 2
    void *p = 0x123
}

// Align names and values
{
    int   a      = 1
    int   number = 2
    void *p      = 0x123
}
```

### Show indentation levels
```c
// Without
{
    S1 s1 = {
        int a = 1
        S2 s = {
            int b = 2
        }
    }
    int c = 3
    S3 s2 = {
        int d = 4
    }
}

// With
{
    S1 s1 = {
    |   int a = 1
    |   S2 s = {
    |   |   int b = 2
    |   }
    }
    int c = 3
    S3 s2 = {
    |   int d = 4
    }
}
```

### Print small structs on a single line
```c
// Currently
{
    int a = 3
}
[
    {
        int a = 1
    },
    {
        int a = 2
    }
]

// On single line
{ int a = 1 }
[
    { int a = 1 },
    { int a = 2 }
]
```
