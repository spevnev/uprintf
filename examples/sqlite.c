// Build sqlite as a part of this file because sqlite3 struct in the header is opaque.
#include "sqlite3.c"  // https://www.sqlite.org

#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int select_callback(void *user_data, int columns, char **values, char **names) {
    (void) user_data;

    for (int i = 0; i < columns; i++) {
        printf("%s %s\n", names[i], values[i]);
    }

    return 0;
}

int main(void) {
    sqlite3 *db;
    if (sqlite3_open("test.db", &db) != SQLITE_OK || db == NULL) {
        fprintf(stderr, "Unable to open sqlite database.\n");
        return EXIT_FAILURE;
    }

    uprintf("Sqlite's database: %S\n", db);

    char *error;
    if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (num INT);", NULL, NULL, &error) != SQLITE_OK
        || sqlite3_exec(db, "INSERT INTO test VALUES (1), (2), (3), (4), (5);", NULL, NULL, &error) != SQLITE_OK
        || sqlite3_exec(db, "SELECT * FROM test;", select_callback, NULL, &error) != SQLITE_OK) {
        fprintf(stderr, "Unable to exec SQL query: %s.\n", error);
        sqlite3_free(error);
        return EXIT_FAILURE;
    }

    sqlite3_close(db);
    return EXIT_SUCCESS;
}
