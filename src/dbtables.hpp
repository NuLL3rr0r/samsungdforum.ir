#ifndef DBTABLES_HPP
#define DBTABLES_HPP


#include <string>
#include <unordered_map>
#include "cgienv.hpp"

namespace SamsungDForumIr {
    class DBTables;
}

class SamsungDForumIr::DBTables
{
private:
    std::unordered_map<std::string, std::string> m_tablesHash;
    std::unordered_map<std::string, std::string> m_fieldsHash;

public:
    DBTables();
    DBTables(const CgiEnv::ELang &lang);

    std::string Table(const char *id);
    std::string Fields(const char *id);

private:
    void InitHashes(const CgiEnv::ELang &lang);
};


#endif /* DBTABLES_HPP */


