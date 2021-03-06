#ifndef DB_HPP
#define DB_HPP


#include <string>
#include <cppdb/frontend.h>

namespace SamsungDForumIr {
    class DB;
}

class SamsungDForumIr::DB
{
private:
	static const std::string DB_PATH;

    cppdb::session m_sql;

public:
    DB(const std::string &dbName);
    ~DB();

	static void Vacuum(const char *dbFile);

	cppdb::session &Sql();
    void CreateTable(const std::string &table, const std::string &fields);
    void DropTable(const std::string &table);
    void Insert(const std::string &table, const std::string &fields, const int count, ...);
    void Update(const std::string &table, const std::string &where, const std::string &value,
                const std::string &set, const int count, ...);
    void Delete(const std::string &table, const std::string &where, const std::string &value);
};


#endif /* DB_HPP */


