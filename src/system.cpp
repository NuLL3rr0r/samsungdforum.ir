#include <fstream>
#include <cstdlib>
#include <sys/time.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include "system.hpp"
#include "log.hpp"

using namespace std;
using namespace SamsungDForumIr;

namespace fs = boost::filesystem;

size_t System::RandSeed()
{
    timespec ts;

#if defined (__FreeBSD__)
    clock_gettime(CLOCK_MONOTONIC, &ts);
#elif defined (__linux)
    clock_gettime(CLOCK_REALTIME, &ts);
#endif

    return ts.tv_nsec;
}

bool System::Exec(const string &cmd)
{
    int r = system(cmd.c_str());
    if (r == 0)
        return true;
    else
        return false;
}

bool System::Exec(const string &cmd, string &out_output)
{
    out_output.clear();

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        return false;

    char buffer[256];
    while (!feof(pipe)) {
        if (fgets(buffer, 256, pipe) != NULL) {
            out_output += buffer;
        }
    }

    pclose(pipe);
    return true;
}

bool System::FileExists(const string &fileName)
{
    ifstream file(fileName.c_str());
    if (file.good())
        return true;
    else
        return false;
}

size_t System::FileSize(const string &fileName)
{
    if (FileExists(fileName.c_str())) {
        ifstream::pos_type begin;
        ifstream::pos_type end;
        ifstream file(fileName.c_str());
        begin = file.tellg();
        file.seekg(0, ios::end);
        end = file.tellg();
        file.close();
        return static_cast<size_t>(end - begin);
    } else {
        return 0;
    }
}

bool System::EraseFile(const string &fileName)
{
    /*if (FileExists(fileName.c_str())) {
        remove(fileName.c_str());
    }*/
    try {
        fs::remove(fileName);

        return true;
    }
    catch(const fs::filesystem_error &ex) {
        ERR(ex.what());
    }
    catch(...) {
        ERR("Unknown file erase error.", fileName);
    }

    return false;
}

bool System::MoveFile(const std::string &from, const std::string &to)
{
    try {
        fs::rename(from, to);

        return true;
    }
    catch(const fs::filesystem_error &ex) {
        ERR(ex.what());
    }
    catch(...) {
        ERR("Unknown file move error.", from, to);
    }

    return false;
}

bool System::CopyFile(const std::string &from, const std::string &to)
{
    /*try {
        fs::copy_file(from, to, fs::copy_option::overwrite_if_exists);

        return true;
    }
    catch(const fs::filesystem_error &ex) {
        ERR(ex.what());
    }*/
    try {
        ifstream is(from, ios::in | ios::binary);
        if (!is.is_open())
            return false;

        ofstream os(to, ios::out | ios::binary);
        if (!os.is_open())
            return false;

        os << is.rdbuf();
        os.close();
        is.close();
        return true;
    }
    catch(...) {
        ERR("Unknown file copy error.", from, to);
    }

    return false;
}

void System::ReadFile(const string &fileName, size_t &out_size, char **out_data)
{
    char *buffer = NULL;
    out_size = 0;

    ifstream file(fileName.c_str(), ios::in | ios::binary | ios::ate);
    if (file.is_open()) {
        ifstream::pos_type size;

        size = file.tellg();
        buffer = new char[size];
        file.seekg(0, ios::beg);
        file.read(buffer, size);
        file.close();

        out_size = static_cast<size_t>(size);
        *out_data = buffer;
    }
}

void System::WriteFile(const string &fileName, const char *data)
{
    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if (file.is_open()) {
        file << data;
        file.close();
    }
}


