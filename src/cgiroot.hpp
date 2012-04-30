#ifndef CGIROOT_HPP
#define CGIROOT_HPP


#include <memory>
#include <string>

namespace Wt {
    class WApplication;
    class WEnvironment;
    class WWidget;
}

namespace SamsungDForumIr {
    class Captcha;
    class CgiEnv;
    class CgiRoot;
    class DB;
    class DBTables;
    class Lang;
}

class SamsungDForumIr::CgiRoot : public Wt::WApplication
{
public:
    typedef std::shared_ptr<Captcha> Captcha_ptr;
    typedef std::shared_ptr<CgiEnv> CgiEnv_ptr;
    typedef std::shared_ptr<DB> DB_ptr;
    typedef std::shared_ptr<DBTables> DBTables_ptr;
    typedef std::shared_ptr<Lang> Lang_ptr;

private:
    static const std::size_t M_LANG_COOKIE_LIFETIME;

public:
    Wt::WContainerWidget *HtmlRoot;

    Captcha_ptr CaptchaInstance;
    CgiEnv_ptr CgiEnvInstance;
    DB_ptr DBInstance;
    DBTables_ptr DBTablesInstance;
    Lang_ptr LangInstance;

public:
    CgiRoot(const Wt::WEnvironment &env);

public:
    static Wt::WApplication *CreateApplication(const Wt::WEnvironment &env);

    void Redirect(const std::string &url);
    void Exit(const std::string &url);
    void Exit();

private:
    void Error(const std::string &err) const;
    Wt::WWidget *InitHome();
    Wt::WWidget *InitRootLogin();
    void HandleSiteMapReq();
    void HandleVerifyReq();
};


#endif /* CGIROOT_HPP */


