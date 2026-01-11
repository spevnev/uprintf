typedef struct {
    int a;
} Common;

struct BinPrivate;

void print_common(Common *s);
void print_lib_private();
void print_bin_private(struct BinPrivate *s);
