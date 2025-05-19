#ifndef MOCKMYSQL_H
#define MOCKMYSQL_H
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <mysql/mysql.h>

class MockMYSQL {
public:
    MOCK_METHOD(int, mysql_query, (MYSQL *conn, const char *query), ());
    MOCK_METHOD(MYSQL_RES*, mysql_store_result, (MYSQL *conn), ());
    MOCK_METHOD(MYSQL_ROW, mysql_fetch_row, (MYSQL_RES *res), ());
    MOCK_METHOD(void, mysql_free_result, (MYSQL_RES *res), ());
};
#endif