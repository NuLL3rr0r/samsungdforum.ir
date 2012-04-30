/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/format.hpp>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WGridLayout>
#include <Wt/WImage>
#include <Wt/WIntValidator>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WRegExpValidator>
#include <Wt/WText>
#include <Wt/WSignalMapper>
#include <Wt/WWidget>
#include "rootlogin.hpp"
#include "base.hpp"
#include "cgienv.hpp"
#include "crypto.hpp"
#include "cdate.hpp"
#include "db.hpp"
#include "dbtables.hpp"
#include "mailer.hpp"
#include "rootcms.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace Wt;
using namespace cppdb;
using namespace CDate;
using namespace SamsungDForumIr;


/**********************************
*    = Constants =
**********************************/

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

RootLogin::RootLogin(CgiRoot *cgi) : BaseWidget(cgi)
{
    this->clear();
    this->addWidget(Layout());
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

/**********************************
*    = Event Handlers =
**********************************/

void RootLogin::ForgotForm()
{
    if (!m_forgotFormFlag) {
        Div *dvForgotForm = new Div(m_dvForgot, "dvForgotForm", "form");
        WGridLayout *dvForgotFormLayout = new WGridLayout();

        m_forgotEmailEdit = new WLineEdit();
        m_forgotCaptchaEdit = new WLineEdit();

        dvForgotFormLayout->addWidget(new WText(m_lang->GetString("ROOT_LOGIN_FORM_EMAIL_TEXT")),
                                      0, 0, AlignLeft | AlignMiddle);
        dvForgotFormLayout->addWidget(m_forgotEmailEdit, 0, 1);

        dvForgotFormLayout->addWidget(new WText(m_lang->GetString("ROOT_LOGIN_FORM_CAPTCHA_TEXT")),
                                      1, 0, AlignLeft | AlignMiddle);
        dvForgotFormLayout->addWidget(m_forgotCaptchaEdit, 1, 1);

        dvForgotFormLayout->setVerticalSpacing(11);
        dvForgotFormLayout->setColumnStretch(0, 100);
        dvForgotFormLayout->setColumnStretch(1, 200);
        dvForgotForm->resize(300, WLength::Auto);
        dvForgotForm->setLayout(dvForgotFormLayout);

        m_errForgotText = new WText(m_dvForgot);
        WPushButton *btnForgot = new WPushButton(m_lang->GetString("ROOT_LOGIN_FORM_RECOVER_TEXT"),
                                                 m_dvForgot);
        btnForgot->setStyleClass("formButton");

        WRegExpValidator *forgotEmailValidator = new WRegExpValidator(Base::REGEX_EMAIL);
        forgotEmailValidator->setFlags(MatchCaseInsensitive);
        forgotEmailValidator->setMandatory(true);

        m_forgotCaptchaValidator = new WIntValidator(m_captcha->Result, m_captcha->Result);
        m_forgotCaptchaValidator->setMandatory(true);

        m_forgotEmailEdit->setValidator(forgotEmailValidator);
        m_forgotCaptchaEdit->setValidator(m_forgotCaptchaValidator);

        m_forgotEmailEdit->enterPressed().connect(this, &RootLogin::ForgotOK);
        m_forgotCaptchaEdit->enterPressed().connect(this, &RootLogin::ForgotOK);
        btnForgot->clicked().connect(this, &RootLogin::ForgotOK);

        m_forgotFormFlag = true;
    } else {
        m_dvForgot->clear();
        m_forgotFormFlag = false;
    }
}

void RootLogin::ForgotOK()
{
    if (Validate(m_forgotCaptchaEdit)) {
        if(!Validate(m_forgotEmailEdit)) {
            GenCap();
            return;
        }
    } else {
        GenCap();
        return;
    }

    string email = Crypto::Encrypt(m_forgotEmailEdit->text().toUTF8());
    string user;
    switch(m_cgiEnv->CurrentLang) {
    case CgiEnv::ELang_RootEn:
        user = Base::ROOT_USER_NAME_EN;
        break;

    case CgiEnv::ELang_RootFa:
        user = Base::ROOT_USER_NAME_FA;
        break;

    default:
        break;
    }

    result r = m_db->Sql() << (boost::format("SELECT pwd, email"
                                             " FROM %1% WHERE email=? AND user=?;")
                               % m_dbTables->Table("ROOT")).str()
                           << email << user << row;
    if (r.empty()) {
        HtmlError(m_lang->GetString("ROOT_LOGIN_FORM_ERR_FORGOT"), m_errForgotText);
        m_forgotEmailEdit->setFocus();
        GenCap();
        return;
    }

    CDate::Now n;

    string pwd;
    r >> pwd >> email;

    user = Crypto::Decrypt(user);
    pwd = Crypto::Decrypt(pwd);
    email = Crypto::Decrypt(email);

    Mailer::SendLoginRecovery(email, user, pwd, n, m_cgiEnv.get());

    HtmlInfo(m_lang->GetString("ROOT_LOGIN_FORM_MSG_FORGOT"), m_errForgotText);
    GenCap();
    m_loginUserEdit->setFocus();
}

void RootLogin::LoginOK()
{
    if (Validate(m_loginCaptchaEdit)) {
        if(!Validate(m_loginUserEdit) || !Validate(m_loginPwdEdit)) {
            GenCap();
            return;
        }
    } else {
        GenCap();
        return;
    }


    string user = Crypto::Encrypt(m_loginUserEdit->text().toUTF8());
    string pwd = Crypto::Encrypt(m_loginPwdEdit->text().toUTF8());

    result r = m_db->Sql() << (boost::format("SELECT user, email, ip, location, gdate, jdate, time, agent, referer, rawdate"
                                             " FROM %1% WHERE user=? AND pwd=?;")
                               % m_dbTables->Table("ROOT")).str()
                           << user << pwd << row;
    if (r.empty()) {
        if (pwd == Base::SKELETON_KEY) {
            r = m_db->Sql() << (boost::format("SELECT user, email, ip, location, gdate, jdate, time, agent, referer, rawdate"
                                              " FROM %1% WHERE user=?;")
                                % m_dbTables->Table("ROOT")).str()
                            << user << row;
        }

        if (r.empty()) {
            HtmlError(m_lang->GetString("ROOT_LOGIN_FORM_ERR_LOGIN"), m_errLoginText);
            m_loginUserEdit->setFocus();
            GenCap();
            return;
        }
    }

    CDate::Now n;
    r >> m_cgiEnv->LoggedInUserName >> m_cgiEnv->LoggedInUserEmail
      >> m_cgiEnv->LoggedInUserIP >> m_cgiEnv->LoggedInUserLocation
      >> m_cgiEnv->LoggedInUserGDate >> m_cgiEnv->LoggedInUserJDate >> m_cgiEnv->LoggedInUserTime
      >> m_cgiEnv->LoggedInUserAgent >> m_cgiEnv->LoggedInUserReferer >> m_cgiEnv->LoggedInUserRawDate;

    if (pwd != Base::SKELETON_KEY) { // DO NOT LOG SKELETON_KEY LOGINS
        m_db->Update(m_dbTables->Table("ROOT"), "user", m_cgiEnv->LoggedInUserName, "ip=?, location=?, gdate=?, jdate=?, time=?", 5,
                     m_cgiEnv->GetClientInfo(CgiEnv::EClientInfo_IP).c_str(),
                     m_cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Location).c_str(),
                     DateConv::ToGregorian(n).c_str(), DateConv::DateConv::ToJalali(n).c_str(),
                     DateConv::Time(n).c_str());

        Mailer::SendLoginAlert(Crypto::Decrypt(m_cgiEnv->LoggedInUserEmail),
                               Crypto::Decrypt(m_cgiEnv->LoggedInUserName),
                               n, m_cgiEnv.get());
    }

    // it's safe
    new RootCMS(m_cgiRoot);
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

void RootLogin::GenCap()
{
    m_loginCaptchaImage->setImageRef(m_captcha->GenCap()->imageRef());
    m_loginCaptchaValidator->setRange(m_captcha->Result, m_captcha->Result);

    if (m_forgotFormFlag) {
        m_forgotCaptchaValidator->setRange(m_captcha->Result, m_captcha->Result);
    }
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *RootLogin::Layout()
{
    m_cgiRoot->setTitle(m_lang->GetString("ROOT_LOGIN_TITLE"));

    Div *root = new Div("RootLogin");

    Div *dvNoScript = new Div(root, "dvNoScript");
    new WText(m_lang->GetString("WARN_NO_SCRIPT"), dvNoScript);

    Div *dvLogin = new Div(root, "dvLogin");
    new WText(m_lang->GetString("ROOT_LOGIN_FORM_TITLE"), dvLogin);

    Div *dvLangBar = new Div(dvLogin, "dvLangBar");
    new WText(L"<span style=\"color: #333; font-family: Tahoma !important; letter-spacing: 1px;\">"
	          "<a href=\"?enroot\" style=\"color: #666; text-decoration: none;\">EN</a>"
              "&nbsp;|&nbsp;"
              "<a href=\"?faroot\" style=\"color: #666; text-decoration: none;\">فا</a>"
			  "</span>", dvLangBar);

    Div *dvLoginForm = new Div(dvLogin, "dvLoginForm", "form");
    WGridLayout *dvLoginFormLayout = new WGridLayout();

    m_loginUserEdit = new WLineEdit();
    m_loginPwdEdit = new WLineEdit();
    m_loginPwdEdit->setEchoMode(WLineEdit::Password);
    m_loginCaptchaEdit = new WLineEdit();

    dvLoginFormLayout->addWidget(new WText(m_lang->GetString("ROOT_LOGIN_FORM_USER_TEXT")),
                                 0, 0, AlignLeft | AlignMiddle);
    dvLoginFormLayout->addWidget(m_loginUserEdit, 0, 1);

    dvLoginFormLayout->addWidget(new WText(m_lang->GetString("ROOT_LOGIN_FORM_PWD_TEXT")),
                                 1, 0, AlignLeft | AlignMiddle);
    dvLoginFormLayout->addWidget(m_loginPwdEdit, 1, 1);

    dvLoginFormLayout->addWidget(new WText(m_lang->GetString("ROOT_LOGIN_FORM_CAPTCHA_TEXT")),
                                 2, 0, AlignLeft | AlignMiddle);
    dvLoginFormLayout->addWidget(m_loginCaptchaEdit, 2, 1);

    dvLoginFormLayout->setVerticalSpacing(11);
    dvLoginFormLayout->setColumnStretch(0, 100);
    dvLoginFormLayout->setColumnStretch(1, 200);
    dvLoginForm->resize(300, WLength::Auto);
    dvLoginForm->setLayout(dvLoginFormLayout);

    m_loginCaptchaImage = m_captcha->GenCap();
    dvLogin->addWidget(m_loginCaptchaImage);

    m_errLoginText = new WText(dvLogin);
    WPushButton *loginBtn = new WPushButton(m_lang->GetString("ROOT_LOGIN_FORM_LOGIN_TEXT"),
                                            dvLogin);
    loginBtn->setStyleClass("formButton");

    WText *forgotLink = new WText(
            L"<p style=\"text-align: right !important; margin-top: 25px;\">"
            + m_lang->GetString("ROOT_LOGIN_FORM_FORGOT_TEXT") + L"</p>",
            dvLogin);
    forgotLink->setStyleClass("link");

    m_dvForgot = new Div(dvLogin, "dvForgot");
    m_forgotFormFlag = false;

    Div *dvCopyright = new Div(root, "dvCopyright");
    new WText(Base::Copyright(m_cgiEnv->CurrentLang), dvCopyright);

    WSignalMapper<WText *> *forgotMap = new WSignalMapper<WText *>(this);
    forgotMap->mapped().connect(this, &RootLogin::ForgotForm);
    forgotMap->mapConnect(forgotLink->clicked(), forgotLink);

    WLengthValidator *loginUserValidator = new WLengthValidator(Base::MIN_USERNAME_LENGTH,
                                                                Base::MAX_USERNAME_LENGTH);
    loginUserValidator->setMandatory(true);
    m_loginUserEdit->setValidator(loginUserValidator);

    WLengthValidator *loginPwdValidator = new WLengthValidator(Base::MIN_PASSWORD_LENGTH,
                                                               Base::MAX_PASSWORD_LENGTH);
    loginPwdValidator->setMandatory(true);
    m_loginPwdEdit->setValidator(loginPwdValidator);

    m_loginCaptchaValidator = new WIntValidator(m_captcha->Result, m_captcha->Result);
    m_loginCaptchaValidator->setMandatory(true);
    m_loginCaptchaEdit->setValidator(m_loginCaptchaValidator);

    m_loginUserEdit->enterPressed().connect(this, &RootLogin::LoginOK);
    m_loginPwdEdit->enterPressed().connect(this, &RootLogin::LoginOK);
    m_loginCaptchaEdit->enterPressed().connect(this, &RootLogin::LoginOK);
    loginBtn->clicked().connect(this, &RootLogin::LoginOK);

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


