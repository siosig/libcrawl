#include "crawl/db.hpp"
#include "crawl/util.hpp"
#include <boost/format.hpp>

namespace crawl {
  class DB::DBPrivate {
  public:
    DBPrivate()
      : driver(NULL)
      , con(NULL)

    {
    }

    ~DBPrivate() {
      close();
    }

    void connect(std::string s) {
      schema = s;
      if (!driver) {
        driver = sql::mysql::get_mysql_driver_instance();
      }
      if (!con) {
        con = driver->connect("tcp://127.0.0.1:3306", "user", "password");
        con->setSchema(schema);
      // } else if(!con->isValid() || con->isClosed()) {
      } else if(con->isClosed()) {
        delete con;
        con = driver->connect("tcp://127.0.0.1:3306", "user", "password");
        con->setSchema(schema);
      }
    }

    void setupConnection() {
      connect(schema);
    }


    void close() {
      if (con && ! con->isClosed()) {
        con->close();
      }
      delete con;
      con = NULL;
    }

    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    std::string schema;
    std::map<sql::PreparedStatement*, std::string> stmt;
  };

  DB::DB()
    : d(new DBPrivate)
  {
  }

  DB::DB(const std::string &schema)
    : d(new DBPrivate)
  {
    d->schema = schema;
  }

  DB::~DB() {
    delete d;
  }


  void DB::begin() {
    d->con->setAutoCommit(0);
  }

  void DB::commit() {
    d->con->commit();
  }

  void DB::close() {
    d->close();
  }

  void DB::setSchema(const std::string &schema) {
    d->schema = schema;
    if (!d->driver) {
      d->driver = sql::mysql::get_mysql_driver_instance();
    }
  }

  uint32_t DB::bulkInsert(const std::string &q, const std::vector<std::vector<std::string>> &rows,bool outputlog) const {
    uint32_t ret;
    d->setupConnection();
    if (rows.size() == 0){
      return 0;
    }

    std::string params = "?";
    for (uint32_t i = 0; i < q.length(); i++) {
      char ch = (q.c_str())[i];
      if (ch == ',') {
        params = params + ",?";
      }
    }
    std::string query = q + " VALUES ";
    std::string values_block = "(" + params + ")";

#if 1
    for(uint32_t i = 1; i < rows.size(); i++) {
      values_block = values_block + ",(" + params + ")";
    }
    query = query + values_block;
    std::unique_ptr<sql::PreparedStatement> pstmt(d->con->prepareStatement(query));
    uint32_t i = 1;
    for(std::vector<std::vector<std::string>>::const_iterator row = rows.begin();
        row != rows.end(); row++){
      for(std::vector<std::string>::const_iterator val = (*row).begin();
          val != (*row).end(); val++) {
        pstmt->setString(i++, *val);
      }
    }

    if (outputlog) {
      INFO(boost::format("query: %s") % query.substr(0, 200));
    }
    return pstmt->executeUpdate();
#else
    uint32_t step = 1000;
    for(uint32_t i  = 0; i < rows.size(); i += step) {
      if (i + step >= rows.size()) {
        step = rows.size() - 1;
      }
      std::cout << step << std::endl;
      const std::vector<std::vector<std::string>> sub(&rows[i], &rows[i + step]);

      for(uint32_t i = 1; i < sub.size(); i++) {
        values_block = values_block + ",(" + params + ")";
      }
      query = query + values_block;
      std::unique_ptr<sql::PreparedStatement> pstmt(d->con->prepareStatement(query));
      uint32_t i = 1;
      std::for_each(std::begin(rows), std::end(rows), const std::vector<std::string> &row) {
        std::for_each(std::begin(row), std::end(row), const std::string &val) {
          pstmt->setString(i++, val);
        }
      }

      if (outputlog) {
        INFO(boost::format("query: %s") % query.substr(0, 200));
      }
      ret += pstmt->executeUpdate();
    }
#endif
    return ret;
  }

  sql::PreparedStatement* DB::prepare(const std::string &query, bool outputlog) {
    d->setupConnection();
    if (outputlog) {
      DEBUG(boost::format("query: %s") % query);
    }
    sql::PreparedStatement *prep_stmt = d->con->prepareStatement(query.c_str());
    d->stmt[prep_stmt] = query;
    return prep_stmt;
  }

  sql::ResultSet* DB::execute(sql::PreparedStatement* stmt, std::vector<std::string> param, bool outputlog) {
    d->setupConnection();
    for(uint32_t i = 0; i < param.size(); i++){
      stmt->setString(i + 1,  param.at(i));
    }
    if (outputlog) {
      INFO(boost::format("query: %s") % d->stmt[stmt]);
    }
    return stmt->executeQuery();
  }

  sql::ResultSet* DB::execute(sql::PreparedStatement* stmt, bool outputlog) {
    d->setupConnection();
    if (outputlog) {
      INFO(boost::format("query: %s") % d->stmt[stmt]);
    }
    return stmt->executeQuery();
  }

  uint32_t DB::update(sql::PreparedStatement* stmt, std::vector<std::string> param, bool outputlog) {
    d->setupConnection();
    for(uint32_t i = 0; i < param.size(); i++){
      stmt->setString(i + 1,  param.at(i));
    }
    if (outputlog) {
      INFO(boost::format("query: %s") % d->stmt[stmt]);
    }
    return stmt->execute();
  }

};
