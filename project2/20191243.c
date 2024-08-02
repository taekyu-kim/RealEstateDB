int main(void) {
    MYSQL* connection = NULL;
    MYSQL conn;
    FILE* fp = fopen("CRUD.txt", "rt");    // open CRUD file.
    char line[MAX_LEN];
    char query[MAX_LEN * 10] = "";         // Multi-line queries

    if (mysql_init(&conn) == NULL) {
        printf("mysql_init() error!");
        return 1;
    }

    connection = mysql_real_connect(&conn, host, user, pw, NULL, 3306, NULL, 0);
    if (connection == NULL) {
        printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
        return 1;
    }
    else {
        printf("Connection Succeed\n\n");

        while (fgets(line, sizeof(line), fp) != NULL) {
            // Trim newline character from the end of the line
            line[strcspn(line, "\r\n")] = 0;
            if (strcmp(line, "$$$") == 0) { // Check for $$$ delimiter
                break;
            }
            if (strlen(line) > 0){
                if (strncmp(line, "--", 2) != 0) {
                    strcat(query, line); // Append line to query
                    if (line[strlen(line) - 1] == ';') { // If line ends with semicolon(;)
                        printf("Query: %s\n", query);
                        if (mysql_query(connection, query)) {
                            return 1;
                        }
                        query[0] = '\0'; // query buffer intialize
                    }
                }
            }
        }

        if (mysql_query(connection, "USE project;")) {          // use databasae project
            printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
            return 1;
        }

        Choose_type(connection);

        // DELETE & DROP
        query[0] = '\0'; // query buffer initialize
        while (fgets(line, sizeof(line), fp) != NULL) {
            // Trim newline character from the end of the line
            line[strcspn(line, "\r\n")] = 0;
            if (strlen(line) > 0) {
                if (strncmp(line, "--", 2) != 0) {
                    strcat(query, line);
                    if (line[strlen(line) - 1] == ';') { // If line ends with semicolon(;)
                        printf("Query: %s\n", query);
                        if (mysql_query(connection, query)) {
                            return 1;
                        }
                        query[0] = '\0'; // query buffer initialize
                    }
                }
            }
        }

        mysql_close(connection);
    }

    fclose(fp);
    return 0;
}