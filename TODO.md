## A list of unimplemented features

### Support Windows and MacOS

Both are able to produce DWARF although not in ELF.

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

### Support rare data types
```c
_Complex double x;

_Decimal32 a;
_Decimal64 b;
_Decimal128 c;
```

### Use hash table
There are functions which linearly search for a string, e.g. `_upf_get_variable_type`, `_upf_get_function_return_type`.
Hash table should improve performance.
