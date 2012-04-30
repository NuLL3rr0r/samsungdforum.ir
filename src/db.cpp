#include <cstdarg>
#include <sqlite3.h>
#include <cppdb/backend.h>
#include <cppdb/driver_manager.h>
#include <cppdb/frontend.h>
#include "db.hpp"

using namespace std;
using namespace cppdb;
using namespace SamsungDForumIr;

const string DB::DB_PATH = "../db/";

DB::DB(const string &dbName)
{
    m_sql.open("sqlite3:db=" + DB_PATH + dbName);
}

DB::~DB()
{
    m_sql.close();
}

void DB::Vacuum(const char *dbFile)
{
    sqlite3 *db;
    int rc = sqlite3_open((DB_PATH + dbFile).c_str(), &db);
    if (!rc)
        sqlite3_exec(db, "VACUUM;", 0, 0, 0);
}

cppdb::session &DB::Sql()
{
	return m_sql;
}

void DB::CreateTable(const std::string &table, const std::string &fields)
{
    m_sql << "CREATE TABLE IF NOT EXISTS [" + table + "] ("
		+ fields +
		");"
		<< exec;
}

void DB::DropTable(const std::string &table)
{
    m_sql << "DROP TABLE IF EXISTS [" + table + "];"
        << exec;
}

void DB::Insert(const std::string &table, const std::string &fields, const int count, ...)
{
    va_list args;
    va_start(args, count);

    string ph;
    for (int i = 0; i < count; ++i) {
        if (i != 0)
            ph += ", ";
        ph += "?";
    }

    statement stat = m_sql << "INSERT OR IGNORE INTO [" + table + "] "
							  "(" + fields + ") "
                              "VALUES (" + ph + ");";

    for(int i = 0; i < count; ++i) {
         stat.bind(va_arg(args, char*));
    }

    va_end(args);

    stat.exec();
}

void DB::Update(const std::string &table, const std::string &where, const std::string &value,  const std::string &set, const int count, ...)
{
    va_list args;
    va_start(args, count);

    statement stat = m_sql << "UPDATE [" + table + "] "
                              "SET " + set + " "
                              "WHERE " + where + "=?;";

    for(int i = 0; i < count; ++i) {
         stat.bind(va_arg(args, char*));
    }

    va_end(args);

    stat.bind(value);

    stat.exec();
}

void DB::Delete(const std::string &table, const std::string &where, const std::string &value)
{
    m_sql << "DELETE FROM [" + table + "] "
		"WHERE " + where + "=?;"
		<< value
		<< exec;
}


