#pragma once

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <vector>

namespace crawl {
  class DB {
  public:
    DB();
    explicit DB(const std::string &schema);
    ~DB();
    void begin();
    void commit();
    void close();
    void setSchema(const std::string &schema);
    void lockTable(const std::string &table);
    void unlockTables();
    uint32_t bulkInsert(const std::string &query, const std::vector<std::vector<std::string>> &rows, bool outputlog = false) const;
    sql::PreparedStatement* prepare(const std::string &query, bool outputlog = false);
    sql::ResultSet* execute(sql::PreparedStatement* stmt, std::vector<std::string> param, bool outputlog = false);
    sql::ResultSet* execute(sql::PreparedStatement* stmt, bool outputlog = false);
    uint32_t update(const std::string& query);
    uint32_t update(sql::PreparedStatement* stmt, std::vector<std::string> param, bool outputlog = false);
  protected:
  private:
    class DBPrivate;
    DBPrivate *d;
  };
}
