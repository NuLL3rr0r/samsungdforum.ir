#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <vector>
#include <csignal>
#include <cstdlib>
#include <err.h>
#include <fcntl.h>
#include <kvm.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using namespace boost;

const string DATABASE_FILE_NAME = "spawn.db";
const string LOCK_FILE_NAME = "spawn.lock";
const mode_t LOCK_FILE_PERMISSION = 0666;

int fdLock;
property_tree::ptree appsTree;

bool GetLock();
void Terminate(int signo);
bool Exec(const string &cmd, string &out_output);
bool ReadApps();
string GetProcessNameFromPath(const string &fullPath);
int	GetPidsOfProcess(const string &processName, vector<int> &pids);

void ReSpawn();

int main(int argc, char **argv)
{
    if (argc > 0) {
        if (argv[0] != NULL) {
            string path(argv[0]);
            path = path.substr(0, path.rfind("/") + 1);
            if (chdir(path.c_str()) == -1) {
                cerr << (format("Cannot change current working directory to %1% !!")
                         % path).str() << endl;
                return EXIT_FAILURE;
            }
        } else {
            cerr << "The program name is not obvious (argv[0] is NULL) !!" << endl;
            return EXIT_FAILURE;
        }
    } else {
        cerr << "The program name is not obvious (argc is ZERO) !!" << endl;
        return EXIT_FAILURE;
    }

    if(!GetLock()) {
        cerr << "Could not get lock !!" << endl;
        cerr << "Probably process is already running !!" << endl;
        return EXIT_FAILURE;
    }


    void (*prev_fn)(int);
    prev_fn = signal(SIGTERM, Terminate);
    if (prev_fn == SIG_IGN) {
        signal(SIGTERM, SIG_IGN);
    }


    if(!ReadApps()) {
        return EXIT_FAILURE;
    }

    if (appsTree.empty()
            || appsTree.count("apps") == 0
            || appsTree.get_child("apps").count("") == 0) {
        cerr << "There is no app to spawn !!" << endl;
        return EXIT_FAILURE;
    }

    while (true) {
        ReSpawn();
        sleep(1);
    }

    return EXIT_SUCCESS;
}

bool GetLock()
{
    struct flock fl;

    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 1;

    if((fdLock = open(LOCK_FILE_NAME.c_str(), O_WRONLY | O_CREAT, LOCK_FILE_PERMISSION)) == -1)
        return false;

    if(fcntl(fdLock, F_SETLK, &fl) == -1)
        return false;

    return true;
}

void Terminate(int signo)
{
    clog << "Terminating...." << endl;
    exit(signo);
}

bool Exec(const string &cmd, string &out_output)
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

bool ReadApps()
{
    ifstream dbFile(DATABASE_FILE_NAME);

    if (!dbFile.is_open()) {
        cerr << "Unable to open database file !!" << endl;
        return false;
    }

    try {
        property_tree::read_json(dbFile, appsTree);
    }

    catch (std::exception const &ex) {
        cerr << ex.what() << endl;
        return false;
    }

    return true;
}

string GetProcessNameFromPath(const string &fullPath)
{
    return fullPath.substr(fullPath.rfind("/") + 1);
}

int	GetPidsOfProcess(const string &processName, vector<int> &pids)
{
    // Based on code from pidof.c

    static kvm_t *kd = NULL;
    struct kinfo_proc *p;
    int i;
    int nProcesses;
    int processesFound = 0;

    if ((kd = kvm_open("/dev/null", "/dev/null", "/dev/null", O_RDONLY, "kvm_open")) == NULL) {
        (void)errx(1, "%s", kvm_geterr(kd));
    } else {
        p = kvm_getprocs(kd, KERN_PROC_PROC, 0, &nProcesses);
        for (i = 0; i < nProcesses; ++i)
            if (strncmp(processName.c_str(), p[i].ki_comm, COMMLEN + 1) == 0) {
                pids.push_back((int)p[i].ki_pid);
                cout << (int)p[i].ki_pid << "  : size: " << pids.size() << endl;
                processesFound++;
            }

        kvm_close(kd);
    }

    return processesFound;
}

void ReSpawn()
{
    try {
        BOOST_FOREACH(property_tree::ptree::value_type &appNode, appsTree.get_child("apps")) {
            if (appNode.first.empty()) {
                // 'optional<property_tree::basic_ptree<string, string> &>' works too.
                // but 'auto' is safer here.
                auto socket = appNode.second.get_child_optional("socket");
                auto port = appNode.second.get_child_optional("port");

                string cmd;

                if (socket || port) {
                    if (socket) {
                        cmd = (format("/usr/bin/netstat | /usr/bin/grep '%1%'")
                               % appNode.second.get<std::string>("socket")).str();
                    } else {
                        cmd = (format("/usr/bin/netstat -an | /usr/bin/grep '%1%'")
                               % appNode.second.get<std::string>("port")).str();
                    }
                } else {
                    throw "Please specify socket or port !!";
                }

                string output;
                bool running = false;
                if (Exec(cmd, output)) {
                    if (socket) {
                        if (algorithm::trim_copy(output) != "") {
                            running = true;
                        }
                    } else {
                        if (output.find("LISTEN") != string::npos) {
                            running = true;
                        }
                    }
                }

                if (!running) {
                    vector<int> pids;
                    if (GetPidsOfProcess(GetProcessNameFromPath(appNode.second.get<std::string>("app")), pids)) {
                        for (std::vector<int>::const_iterator it =
                             pids.begin(); it != pids.end(); ++it) {
                            cmd = (format("/bin/ps -p %1% | grep '%2%'")
                                   % *it
                                   % appNode.second.get<std::string>("app")).str();
                            Exec(cmd, output);
                            if (output.find(appNode.second.get<std::string>("app")) != string::npos) {
                                clog << endl
                                     << (format("  * KILLING ==>  %1%"
                                                "\n\t%2%")
                                         % *it
                                         % appNode.second.get<std::string>("app")).str()
                                     << endl;

                                cmd = (format("/bin/kill -SIGKILL %1%")
                                       % *it).str();
                                system(cmd.c_str());
                            }
                        }
                    }

                    if (socket) {
                        clog << endl
                             << (format("  * RESPAWNING ==>  %1%"
                                        "\n\tConfig File        :  %2%"
                                        "\n\tUnix domain socket :  %3%"
                                        "\n\tWorking directory  :  %4%")
                                 % appNode.second.get<std::string>("app")
                                 % appNode.second.get<std::string>("config")
                                 % appNode.second.get<std::string>("socket")
                                 % appNode.second.get<std::string>("workdir")
                                 ).str()
                             << endl << endl;

                        cmd = (format("cd %2% && WT_CONFIG_XML=%1% /usr/local/bin/spawn-fcgi -d %2% -s %3% -n -M 0770 -u www -g www -- %4% &")
                               % appNode.second.get<std::string>("config")
                               % appNode.second.get<std::string>("workdir")
                               % appNode.second.get<std::string>("socket")
                               % appNode.second.get<std::string>("app")
                               ).str();
                    } else {
                        clog << endl
                             << (format("  * RESPAWNING ==>  %1%"
                                        "\n\tConfig File        :  %2%"
                                        "\n\tTCP port           :  %3%"
                                        "\n\tWorking directory  :  %4%")
                                 % appNode.second.get<std::string>("app")
                                 % appNode.second.get<std::string>("config")
                                 % appNode.second.get<std::string>("port")
                                 % appNode.second.get<std::string>("workdir")
                                 ).str()
                             << endl << endl;

                        cmd = (format("cd %2% && WT_CONFIG_XML=%1% /usr/local/bin/spawn-fcgi -d %2% -a 0.0.0.0 -p %3% -n -u www -g www -- %4% &")
                               % appNode.second.get<std::string>("config")
                               % appNode.second.get<std::string>("workdir")
                               % appNode.second.get<std::string>("port")
                               % appNode.second.get<std::string>("app")
                               ).str();
                    }

                    system(cmd.c_str());
                }
            }
        }
    }

    catch (const char * const &ex) {
        cerr << ex << endl;
    }

    catch (std::exception const &ex) {
        cerr << ex.what() << endl;
    }
}


