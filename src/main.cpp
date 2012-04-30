#include <string>
#include <csignal>
#include <cstdlib>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include <Wt/WServer>
#include <Magick++.h>
#include "base.hpp"
#include "cgiroot.hpp"
#include "crypto.hpp"
#include "db.hpp"
#include "dbtables.hpp"
#include "exception.hpp"
#include "log.hpp"


void Terminate(int signo);
void InitTables();


int main(int argc, char **argv)
{
    try {
        /*! Gracefully handling SIGTERM */
        void (*prev_fn)(int);
        prev_fn = signal(SIGTERM, Terminate);
        if (prev_fn == SIG_IGN)
            signal(SIGTERM, SIG_IGN);

        /*! Changing CurDir to executable path */
        if (argc > 0) {
            if (argv[0] != NULL) {
                std::string path(argv[0]);
                path = path.substr(0, path.rfind("/") + 1);
                if (chdir(path.c_str()) == -1) {
                    throw SamsungDForumIr::Exception(
                                (boost::format("Cannot change current working directory to %1%")
                                 % path).str());
                }
            } else {
                throw SamsungDForumIr::Exception("The program name is not obvious (argv[0] is NULL)");
            }
        } else {
            throw SamsungDForumIr::Exception("The program name is not obvious (argc is ZERO)");
        }

        /*! Initializing Magick++ */
        Magick::InitializeMagick(*argv);

        /*! Initializing Tables */
        InitTables();

        /*! Starting web server, otherwise going down */
        LOG("Starting web server...");
        Wt::WServer server(argv[0]);
        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
        server.addEntryPoint(Wt::Application, SamsungDForumIr::CgiRoot::CreateApplication, "", "favicon.ico");
        if (server.start()) {
            Wt::WServer::waitForShutdown();
            server.stop();
        }
    }

    catch (SamsungDForumIr::Exception &ex) {
        ERR(ex.what());
    }

    catch (Wt::WServer::Exception &ex) {
        ERR(ex.what());
    }

    catch (boost::exception &ex) {
        ERR(boost::diagnostic_information(ex));
    }
    
    catch (std::exception &ex) {
        ERR(ex.what());
    }

    catch (...) {
        ERR(SamsungDForumIr::Base::ERR_UNKNOWN);
    }

    return 0;
}

void Terminate(int signo)
{
    LOG((boost::format("Terminating by signal %1%...") % signo).str());
    exit(1);
}

void InitTables()
{
    using namespace SamsungDForumIr;

    namespace fs = boost::filesystem;

    try {
        if (fs::exists(Base::FSDB_PATH )) {
            if (!fs::is_directory(Base::FSDB_PATH)) {
                fs::remove(Base::FSDB_PATH);
            }
        }
        if (fs::exists(Base::FSDB_USERS_PATH)) {
            if (!fs::is_directory(Base::FSDB_USERS_PATH)) {
                fs::remove(Base::FSDB_USERS_PATH);
            }
        }
        if (!fs::exists(Base::FSDB_USERS_PATH)) {
            fs::create_directories(Base::FSDB_USERS_PATH);
        }
    }
    catch(const fs::filesystem_error &ex) {
    }
    catch(...) {
    }

    SamsungDForumIr::DB *db = new SamsungDForumIr::DB(SamsungDForumIr::Base::DATABASE_NAME);
    SamsungDForumIr::DBTables *dbTables = new SamsungDForumIr::DBTables();

    db->CreateTable(dbTables->Table("ROOT"), dbTables->Fields("ROOT"));
    db->Insert(dbTables->Table("ROOT"), "user, pwd, email", 3,
               Base::ROOT_USER_NAME_EN.c_str(),
               Base::ROOT_USER_PWD_EN.c_str(),
               Base::ROOT_USER_EMAIL_EN.c_str());
    db->Insert(dbTables->Table("ROOT"), "user, pwd, email", 3,
               Base::ROOT_USER_NAME_FA.c_str(),
               Base::ROOT_USER_PWD_FA.c_str(),
               Base::ROOT_USER_EMAIL_FA.c_str());

    db->CreateTable(dbTables->Table("CONTACTS_EN"), dbTables->Fields("CONTACTS"));
    db->CreateTable(dbTables->Table("CONTACTS_FA"), dbTables->Fields("CONTACTS"));
    db->CreateTable(dbTables->Table("CONTACTS_EXTRA_INFO"), dbTables->Fields("CONTACTS_EXTRA_INFO"));

    db->Insert(dbTables->Table("CONTACTS_EXTRA_INFO"), "lang, info",
               2, "en",
               Crypto::Encrypt("").c_str());
    db->Insert(dbTables->Table("CONTACTS_EXTRA_INFO"), "lang, info",
               2, "fa",
               Crypto::Encrypt("").c_str());

    db->CreateTable(dbTables->Table("NEWS_EN"), dbTables->Fields("NEWS"));
    db->CreateTable(dbTables->Table("NEWS_FA"), dbTables->Fields("NEWS"));

    db->CreateTable(dbTables->Table("PAGES_EN"), dbTables->Fields("PAGES"));
    db->CreateTable(dbTables->Table("PAGES_FA"), dbTables->Fields("PAGES"));

    db->Insert(dbTables->Table("PAGES_EN"), "pg, parent, fullpath, zindex, body, viewcount, url",
               7, Base::DB_PAGE_ID_HOME.c_str(),
               "/",
               Base::DB_PAGE_FULLPATH_HOME.c_str(),
               "0",
               Crypto::Encrypt("").c_str(),
               "0",
               Base::URL_HOME.c_str());
    db->Insert(dbTables->Table("PAGES_FA"), "pg, parent, fullpath, zindex, body, viewcount, url",
               7, Base::DB_PAGE_ID_HOME.c_str(),
               "/",
               Base::DB_PAGE_FULLPATH_HOME.c_str(),
               "0",
               Crypto::Encrypt(""
                               "<div class=\"slideshow\"><img class=\"active\" src=\"pub/home/slideshow/fa01.jpg\" alt=\"Slideshow Image 1\" /> <img src=\"pub/home/slideshow/fa02.jpg\" alt=\"Slideshow Image 2\" /> <img src=\"pub/home/slideshow/fa03.jpg\" alt=\"Slideshow Image 3\" /> <img src=\"pub/home/slideshow/fa04.jpg\" alt=\"Slideshow Image 4\" /></div>"
                               "<div class=\"dvQuickLinks\" style=\"background: url('pub/home/quicklinks/fabase.png');\"><!--Foreces TinyMCE to keep the div -->"
                               "<ul>"
                               "<li id=\"menu1\" onmouseover=\"onMenu(id)\" onmouseout=\"outMenu(id)\" onclick=\"OnMenuClicked(1)\"><a id=\"menuLink1\" style=\"margin-left: -84px; background: url('pub/home/quicklinks/fa01.png');\"></a></li>"
                               "<li id=\"menu2\" onmouseover=\"onMenu(id)\" onmouseout=\"outMenu(id)\" onclick=\"OnMenuClicked(2)\"><a id=\"menuLink2\" style=\"margin-left: -80px; background: url('pub/home/quicklinks/fa02.png');\"></a></li>"
                               "<li id=\"menu3\" onmouseover=\"onMenu(id)\" onmouseout=\"outMenu(id)\" onclick=\"OnMenuClicked(3)\"><a id=\"menuLink3\" style=\"margin-left: -75px; background: url('pub/home/quicklinks/fa03.png');\"></a></li>"
                               "<li id=\"menu4\" onmouseover=\"onMenu(id)\" onmouseout=\"outMenu(id)\" onclick=\"OnMenuClicked(4)\"><a id=\"menuLink4\" style=\"margin-left: -69px; background: url('pub/home/quicklinks/fa04.png');\"></a></li>"
                               "<li id=\"menu5\"><a id=\"menuLink5\" style=\"margin-left: -50px; border: 1px solid blue; background: blue;\"></a></li>"
                               "</ul>"
                               "</div>"
                               "").c_str(),
               "0",
               Base::URL_HOME.c_str());

    /// TEMPORARY LINKS ON HOME
    db->Insert(dbTables->Table("PAGES_FA"), "pg, parent, fullpath, zindex, body, viewcount, url",
               7, "مسابقات",
               Base::DB_PAGE_FULLPATH_HOME.c_str(),
               (Base::DB_PAGE_FULLPATH_HOME + "/" + "مسابقات").c_str(),
               "0",
               Crypto::Encrypt("").c_str(),
               "0",
               Crypto::GenHash(Base::DB_PAGE_FULLPATH_HOME + "/" + "مسابقات").c_str());

    db->Insert(dbTables->Table("PAGES_FA"), "pg, parent, fullpath, zindex, body, viewcount, url",
               7, "راهنما",
               Base::DB_PAGE_FULLPATH_HOME.c_str(),
               (Base::DB_PAGE_FULLPATH_HOME + "/" + "راهنما").c_str(),
               "0",
               Crypto::Encrypt("").c_str(),
               "0",
               Crypto::GenHash(Base::DB_PAGE_FULLPATH_HOME + "/" + "راهنما").c_str());

    db->Insert(dbTables->Table("PAGES_FA"), "pg, parent, fullpath, zindex, body, viewcount, url",
               7, "SDK",
               Base::DB_PAGE_FULLPATH_HOME.c_str(),
               (Base::DB_PAGE_FULLPATH_HOME + "/" + "SDK").c_str(),
               "0",
               Crypto::Encrypt("").c_str(),
               "0",
               Crypto::GenHash(Base::DB_PAGE_FULLPATH_HOME + "/" + "SDK").c_str());

    db->Insert(dbTables->Table("PAGES_FA"), "pg, parent, fullpath, zindex, body, viewcount, url",
               7, "جوایز",
               Base::DB_PAGE_FULLPATH_HOME.c_str(),
               (Base::DB_PAGE_FULLPATH_HOME + "/" + "جوایز").c_str(),
               "0",
               Crypto::Encrypt("").c_str(),
               "0",
               Crypto::GenHash(Base::DB_PAGE_FULLPATH_HOME + "/" + "جوایز").c_str());

    db->Insert(dbTables->Table("PAGES_FA"), "pg, parent, fullpath, zindex, body, viewcount, url",
               7, "زمان بندی",
               Base::DB_PAGE_FULLPATH_HOME.c_str(),
               (Base::DB_PAGE_FULLPATH_HOME + "/" + "زمان بندی").c_str(),
               "0",
               Crypto::Encrypt("").c_str(),
               "0",
               Crypto::GenHash(Base::DB_PAGE_FULLPATH_HOME + "/" + "زمان بندی").c_str());
    /// END OF TEMPORARY LINKS ON HOME


    db->CreateTable(dbTables->Table("APPOINTMENTS"), dbTables->Fields("APPOINTMENTS"));
    db->CreateTable(dbTables->Table("APPS"), dbTables->Fields("APPS"));
    db->CreateTable(dbTables->Table("IDEAS"), dbTables->Fields("IDEAS"));
    db->CreateTable(dbTables->Table("USERS"), dbTables->Fields("USERS"));
    db->CreateTable(dbTables->Table("USERS_COMPANY"), dbTables->Fields("USERS_COMPANY"));
    db->CreateTable(dbTables->Table("USERS_COMPANY_EXTRA"), dbTables->Fields("USERS_COMPANY_EXTRA"));
    db->CreateTable(dbTables->Table("USERS_INDIVIDUAL"), dbTables->Fields("USERS_INDIVIDUAL"));
    db->CreateTable(dbTables->Table("USERS_INDIVIDUAL_EXTRA"), dbTables->Fields("USERS_INDIVIDUAL_EXTRA"));

    delete dbTables;
    delete db;
    SamsungDForumIr::DB::Vacuum(SamsungDForumIr::Base::DATABASE_NAME.c_str());
}


