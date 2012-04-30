#ifndef SYSTEM_HPP
#define SYSTEM_HPP


#include <string>

namespace SamsungDForumIr {
    class System;
}

class SamsungDForumIr::System
{
public:
    static std::size_t RandSeed();

    static bool Exec(const std::string &cmd);
    static bool Exec(const std::string &cmd, std::string &out_output);

    static bool FileExists(const std::string &fileName);
    static std::size_t FileSize(const std::string &fileName);

    static bool EraseFile(const std::string &fileName);

    static bool MoveFile(const std::string &from, const std::string &to);
    static bool CopyFile(const std::string &from, const std::string &to);

    static void ReadFile(const std::string &fileName, std::size_t &out_size, char **out_data);
    static void WriteFile(const std::string &fileName, const char *data);
};


#endif /* SYSTEM_HPP */


