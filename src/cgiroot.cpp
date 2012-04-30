/**********************************
*    = Header File Inclusion =
**********************************/

#include <sstream>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WEnvironment>
#include <Wt/WText>
#include <Wt/WWidget>
#include "cgiroot.hpp"
#include "base.hpp"
#include "captcha.hpp"
#include "cdate.hpp"
#include "cgienv.hpp"
#include "crypto.hpp"
#include "db.hpp"
#include "dbtables.hpp"
#include "exception.hpp"
#include "home.hpp"
#include "lang.hpp"
#include "log.hpp"
#include "mailer.hpp"
#include "rootcms.hpp"
#include "rootlogin.hpp"
#include "system.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace Wt;
using namespace cppdb;
using namespace SamsungDForumIr;


/**********************************
*    = Constants =
**********************************/

const size_t CgiRoot::M_LANG_COOKIE_LIFETIME = 31536000;


/**********************************
*    = Enumerations =
**********************************/

/**********************************
*    = Properties =
**********************************/

/**********************************
*    = Fields =
**********************************/

/**********************************
*    = Constructos =
**********************************/

CgiRoot::CgiRoot(const WEnvironment &env) : WApplication(env)
{
    try {
        setCssTheme("polished");

        CgiEnvInstance = CgiEnv_ptr(new CgiEnv(env));

        if (CgiEnvInstance->FoundXSS())
            throw Exception(Base::ERR_ALICE);

        srand(System::RandSeed());

        root()->clear();
        HtmlRoot = root();

        if (!CgiEnvInstance->IsRootReq()
                && !CgiEnvInstance->IsVerifyReq()) {
            switch(CgiEnvInstance->CurrentLang) {
            case CgiEnv::ELang_None:
                try {
                    Exit("?lang=" + (env.getCookie("lang")));
                }
                catch (...) {
                    Exit("?lang=fa");
                }
                return;

            case CgiEnv::ELang_Invalid:
                Exit("?lang=fa");
                return;

            case CgiEnv::ELang_Fa:
                setLayoutDirection(Wt::RightToLeft);
                // Fall through
            case CgiEnv::ELang_En:
            default:
                if (env.supportsCookies()) {
                    setCookie("lang", CgiEnvInstance->HttpGet["lang"], M_LANG_COOKIE_LIFETIME);
                }
                break;
            }
        }

        CaptchaInstance = Captcha_ptr(new Captcha());
        DBInstance = DB_ptr(new DB(Base::DATABASE_NAME));
        DBTablesInstance = DBTables_ptr(new DBTables(CgiEnvInstance->CurrentLang));
        LangInstance = Lang_ptr(new Lang(CgiEnvInstance->CurrentLang));

        if (CgiEnvInstance->IsSiteMapReq()) {
            HandleSiteMapReq();
            return;
        }

        if (CgiEnvInstance->IsVerifyReq()) {
            HandleVerifyReq();
            return;
        }

        if (!CgiEnvInstance->IsRootReq()) {
            root()->addWidget(InitHome());

            std::stringstream jsMenuInitCmd;
            jsMenuInitCmd << "ddsmoothmenu.init({"
               << "mainmenuid: 'dvSmoothMenu', "
               << "orientation: 'h', "
               << "classname: 'ddsmoothmenu', "
               << "contentsource: 'markup'"
               << "})";
            doJavaScript(jsMenuInitCmd.str(), true);

            doJavaScript("InitHome();", true);
        } else {
            switch(CgiEnvInstance->CurrentLang) {
            case CgiEnv::ELang_RootFa:
                setLayoutDirection(Wt::RightToLeft);
                break;
            default:
                break;
            }

            root()->addWidget(InitRootLogin());
        }
    }

    catch (Exception &ex) {
        root()->clear();
        root()->addWidget(new WText(ex.what()));
    }

    catch (std::exception &ex) {
        Error(ex.what());
    }

    catch (...) {
        Error(Base::ERR_UNKNOWN);
    }
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

WApplication *CgiRoot::CreateApplication(const WEnvironment &env)
{
    return new CgiRoot(env);
}

void CgiRoot::Redirect(const string &url)
{
    redirect(url);
}

void CgiRoot::Exit(const std::string &url)
{
    redirect(url);
    quit();
}

void CgiRoot::Exit()
{
    quit();
}


/**********************************
*    = Event Handlers =
**********************************/

/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

void CgiRoot::Error(const string &err) const
{
    ERR(err);
    root()->clear();
    root()->addWidget(new WText(Base::ERR_CRITICAL));
}

WWidget *CgiRoot::InitHome()
{
    /////////////// MENU & Home initialization

    switch(CgiEnvInstance->CurrentLang) {

    case CgiEnv::ELang_En:
        require("ddsmoothmenu-ltr.js");
        break;

    case CgiEnv::ELang_Fa:
        require("ddsmoothmenu-rtl.js");
        break;

    default:
        break;
    }

    require("jquery.min.js");
    useStyleSheet("ddsmoothmenu.css");

    switch(CgiEnvInstance->CurrentLang) {

    case CgiEnv::ELang_En:
        useStyleSheet("ddsmoothmenu-ltrfix.css");
        break;

    case CgiEnv::ELang_Fa:
        useStyleSheet("ddsmoothmenu-rtlfix.css");
        break;

    default:
        break;
    }

    useStyleSheet("pub/home/home.css");
    require("pub/home/home.js");
    switch(CgiEnvInstance->CurrentLang) {

    case CgiEnv::ELang_En:
        useStyleSheet("pub/home/home-en.css");
        break;

    case CgiEnv::ELang_Fa:
        useStyleSheet("pub/home/home-fa.css");
        break;

    default:
        break;
    }

    /////////////// END MENU & Home initialization


    useStyleSheet("home.css");

    switch(CgiEnvInstance->CurrentLang) {

    case CgiEnv::ELang_En:
        useStyleSheet("home-ltr.css");
        useStyleSheet("home-en.css");
        break;

    case CgiEnv::ELang_Fa:
        useStyleSheet("home-rtl.css");
        useStyleSheet("home-fa.css");
        break;

    default:
        break;
    }

    require("calendar.js");
    useStyleSheet("calendar.css");

    return new Home(const_cast<CgiRoot *>(this));
}

WWidget *CgiRoot::InitRootLogin()
{
    useStyleSheet("root.css");

    switch(CgiEnvInstance->CurrentLang) {

    case CgiEnv::ELang_RootEn:
        useStyleSheet("root-ltr.css");
        useStyleSheet("root-en.css");
        break;

    case CgiEnv::ELang_RootFa:
        useStyleSheet("root-rtl.css");
        useStyleSheet("root-fa.css");
        break;

    default:
        break;
    }

    return new RootLogin(const_cast<CgiRoot *>(this));
    //return new RootCMS(const_cast<CgiRoot *>(this));
}

void CgiRoot::HandleSiteMapReq()
{
    // ToDo
}

void CgiRoot::HandleVerifyReq()
{
    wstring msgSuccess;
    wstring errAlreadyVerified;
    wstring errInvalidRequest;

    switch(CgiEnvInstance->CurrentLang) {

    case CgiEnv::ELang_En:
        msgSuccess = L"<br /><strong><center><pre>Thank you! Your email has been successfully verified.\n"
                "Your account has been activated, and you may now login.</pre></center></strong>";
        errAlreadyVerified = L"<br /><strong><center><pre>Your e-mail account is already verified !!</pre></center></strong>";
        errInvalidRequest = L"<br /><strong><center><pre>Invalid verification request !!</pre></center></strong>";
        break;

    case CgiEnv::ELang_Fa:
        setLayoutDirection(Wt::RightToLeft);
        msgSuccess = L"<br /><strong><center><pre>با تشکر!پست الکترونیکی شما با موفقیت تائید شد.\n"
                "حساب کاربری شما هم اکنون فعال می باشد.\n"
                "با مراجعه به صفحه آغازین وب سایت می توانید به حساب کاربری تان وارد شوید.</pre></center></strong>";
        errAlreadyVerified = L"<br /><strong><center><pre>پست الکترونیکی و حساب کاربری شما قبلا تائید و فعال شده است !!</pre></center></strong>";
        errInvalidRequest = L"<br /><strong><center><pre>درخواست فعال سازی شما نامعتبر می باشد !!</pre></center></strong>";
        break;

    default:
        errInvalidRequest = L"<br /><strong><center><pre>Invalid verification request !!</pre></center></strong>";
        break;
    }

    try {
        if (CgiEnvInstance->CurrentLang == CgiEnv::ELang_None
                || CgiEnvInstance->CurrentLang == CgiEnv::ELang_Invalid) {
            throw "";
        }

        root()->clear();

        string email = Crypto::Decrypt(CgiEnvInstance->GetVerifyReq());

        if (email == "") {
            throw "";
        }

        result r = DBInstance->Sql() << "SELECT email, verified FROM ["
                                       + DBTablesInstance->Table("USERS")
                                        + "] WHERE email=?;" << email << row;
        if (!r.empty()) {
            string dbEmail;
            string verified;
            r >> dbEmail >> verified;

            dbEmail = Crypto::Encrypt(dbEmail);

            if (CgiEnvInstance->GetVerifyReq() != dbEmail) {
                throw "";
            }

            if (verified == "0") {
                CDate::Now n;
                DBInstance->Update(DBTablesInstance->Table("USERS"), "email", email, "verified=?",
                                   1, "1");
                Mailer::SendGreetingMessage(email, email, n, CgiEnvInstance.get());
                root()->addWidget(new WText(msgSuccess));
            } else {
                root()->addWidget(new WText(errAlreadyVerified));
            }
        } else {
            throw "";
        }
    }
    catch (...) {
        root()->addWidget(new WText(errInvalidRequest));
    }
}


/**********************************
*    = Base Class Overrides =
**********************************/

/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


