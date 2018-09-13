#include <sqlite3.h>
#include <stdio.h>

int main(void) {

    sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("data.db", &db);

    if (rc != SQLITE_OK) {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return 1;
    }

    char const *sql = //"CREATE TABLE IF NOT EXISTS datetime;"
                "CREATE TABLE IF NOT EXISTS datetime(time TEXT, unixepoch INT, date TEXT, flag INT);"
                "INSERT INTO datetime VALUES('07:20:31 PM', 1536434431000, '09-08-2018', 1 );"
                "INSERT INTO datetime VALUES('07:20:32 PM', 1536434431060, '09-08-2018', 1 );"
                "INSERT INTO datetime VALUES('07:20:33 PM', 1536434431120, '09-08-2018', 1 );"
                "INSERT INTO datetime VALUES('07:20:34 PM', 1536434431180, '09-08-2018', 1 );"
                "INSERT INTO datetime VALUES('07:20:35 PM', 1536434431240, '09-08-2018', 1 );"
                "INSERT INTO datetime VALUES('07:20:36 PM', 1536434431360, '09-08-2018', 1 );";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK ) {

        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

        return 1;
    }

    sqlite3_close(db);

    return 0;
}
