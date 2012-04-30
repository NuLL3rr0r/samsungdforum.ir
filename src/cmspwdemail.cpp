/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/format.hpp>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WGridLayout>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WRegExpValidator>
#include <Wt/WText>
#include <Wt/WWidget>
#include "cmspwdemail.hpp"
#include "base.hpp"
#include "crypto.hpp"
#include "db.hpp"
#include "dbtables.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace boost;
using namespace Wt;
using namespace cppdb;
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

CmsPwdEmail::CmsPwdEmail(CgiRoot *cgi) : BaseWidget(cgi)
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

void CmsPwdEmail::OnPwdOK()
{
    if(!Validate(m_currentPwdEdit) ||
       !Validate(m_newPwdEdit) || !Validate(m_confirmPwdEdit)) {
        return;
    }

    result r = m_db->Sql() << (boost::format("SELECT pwd FROM %1%"
                                             " WHERE user=? AND pwd=?;")
                               % m_dbTables->Table("ROOT")).str()
                           << m_cgiEnv->LoggedInUserName
                           << Crypto::Encrypt(m_currentPwdEdit->text().toUTF8())
                           << row;
    if (r.empty()) {
        HtmlError(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_INVALID_ERR"), m_errPwd);
        m_currentPwdEdit->setFocus();
        return;
    }

    if (m_newPwdEdit->text() == m_currentPwdEdit->text()) {
        HtmlError(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_SAME_ERR"), m_errPwd);
        m_newPwdEdit->setFocus();
        return;
    }

    if (m_newPwdEdit->text() != m_confirmPwdEdit->text()) {
        HtmlError(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_CONFIRM_ERR"), m_errPwd);
        m_confirmPwdEdit->setFocus();
        return;
    }

    m_db->Update(m_dbTables->Table("ROOT"), "user", m_cgiEnv->LoggedInUserName, "pwd=?", 1,
                 Crypto::Encrypt(m_newPwdEdit->text().toUTF8()).c_str());

    m_currentPwdEdit->setText("");
    m_newPwdEdit->setText("");
    m_confirmPwdEdit->setText("");
    m_currentPwdEdit->setFocus();

    HtmlInfo(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_SUCCESS_MSG"), m_errPwd);
}

void CmsPwdEmail::OnEmailOK()
{
    if(!Validate(m_currentEmailEdit) || !Validate(m_currentEmailPwdEdit)) {
        return;
    }

    result r = m_db->Sql() << (boost::format("SELECT pwd FROM %1%"
                                             " WHERE user=? AND pwd=?;")
                               % m_dbTables->Table("ROOT")).str()
                           << m_cgiEnv->LoggedInUserName
                           << Crypto::Encrypt(m_currentEmailPwdEdit->text().toUTF8())
                           << row;
    if (r.empty()) {
        HtmlError(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_INVALID_PWD_ERR"), m_errEmail);
        m_currentEmailPwdEdit->setFocus();
        return;
    }

    m_db->Update(m_dbTables->Table("ROOT"), "user", m_cgiEnv->LoggedInUserName, "email=?", 1,
                 Crypto::Encrypt(m_currentEmailEdit->text().toUTF8()).c_str());

    m_currentEmailPwdEdit->setText("");
    m_currentEmailEdit->setFocus();

    HtmlInfo(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_SUCCESS_MSG"), m_errEmail);
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *CmsPwdEmail::Layout()
{
    Div *root = new Div("CmsPwdEmail");

    new WText(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_TTITLE"), root);

    Div *dvChangePw = new Div(root, "dvChangePw", "form");
    WGridLayout *dvChangePwLayout = new WGridLayout();

    m_currentPwdEdit = new WLineEdit();
    m_currentPwdEdit->setEchoMode(WLineEdit::Password);
    m_newPwdEdit = new WLineEdit();
    m_newPwdEdit->setEchoMode(WLineEdit::Password);
    m_confirmPwdEdit = new WLineEdit();
    m_confirmPwdEdit->setEchoMode(WLineEdit::Password);

    dvChangePwLayout->addWidget(new WText(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_CURRENT_PWD_TEXT")), 0, 0, AlignLeft | AlignMiddle);
    dvChangePwLayout->addWidget(m_currentPwdEdit, 0, 1);

    dvChangePwLayout->addWidget(new WText(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_NEW_PWD_TEXT")), 1, 0, AlignLeft | AlignMiddle);
    dvChangePwLayout->addWidget(m_newPwdEdit, 1, 1);

    dvChangePwLayout->addWidget(new WText(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_CONFIRM_PWD_TEXT")), 2, 0, AlignLeft | AlignMiddle);
    dvChangePwLayout->addWidget(m_confirmPwdEdit, 2, 1);

    dvChangePwLayout->setVerticalSpacing(11);
    dvChangePwLayout->setColumnStretch(0, 150);
    dvChangePwLayout->setColumnStretch(1, 200);
    dvChangePw->resize(350, WLength::Auto);
    dvChangePw->setLayout(dvChangePwLayout);

    m_errPwd = new WText(root);

    WPushButton *changePwdButton = new WPushButton(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_BUTTON_TEXT"), root);
    changePwdButton->setStyleClass("formButton");

    new WBreak(root);
    new WBreak(root);

    new WText(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_TITLE"), root);

    Div *dvChangeEmail = new Div(root, "dvChangeEmail", "form");
    WGridLayout *dvChangeEmailLayout = new WGridLayout();

    m_currentEmailEdit = new WLineEdit();
    m_currentEmailPwdEdit = new WLineEdit();
    m_currentEmailPwdEdit->setEchoMode(WLineEdit::Password);

    m_currentEmailEdit->setText(Crypto::Decrypt(m_cgiEnv->LoggedInUserEmail));


    dvChangeEmailLayout->addWidget(new WText(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_EMAIL_TEXT")), 0, 0, AlignLeft | AlignMiddle);
    dvChangeEmailLayout->addWidget(m_currentEmailEdit, 0, 1);

    dvChangeEmailLayout->addWidget(new WText(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_PWD_TEXT")), 1, 0, AlignLeft | AlignMiddle);
    dvChangeEmailLayout->addWidget(m_currentEmailPwdEdit, 1, 1);

    dvChangeEmailLayout->setVerticalSpacing(11);
    dvChangeEmailLayout->setColumnStretch(0, 150);
    dvChangeEmailLayout->setColumnStretch(1, 200);
    dvChangeEmail->resize(350, WLength::Auto);
    dvChangeEmail->setLayout(dvChangeEmailLayout);

    m_errEmail = new WText(root);

    WPushButton *changeEmailButton= new WPushButton(m_lang->GetString("ROOT_CMSPWDEMAIL_CHANGE_BUTTON_TEXT"), root);
    changeEmailButton->setStyleClass("formButton");

    WLengthValidator *currentPwdValidator = new WLengthValidator(Base::MIN_USERNAME_LENGTH, Base::MAX_USERNAME_LENGTH);
    WLengthValidator *newPwdValidator = new WLengthValidator(Base::MIN_PASSWORD_LENGTH, Base::MAX_PASSWORD_LENGTH);
    WLengthValidator *confirmPwdValidator = new WLengthValidator(Base::MIN_PASSWORD_LENGTH, Base::MAX_PASSWORD_LENGTH);
    WRegExpValidator *currentEmailValidator = new WRegExpValidator(Base::REGEX_EMAIL);
    WLengthValidator *currentEmailPwdValidator = new WLengthValidator(Base::MIN_PASSWORD_LENGTH, Base::MAX_PASSWORD_LENGTH);

    currentEmailValidator->setFlags(MatchCaseInsensitive);

    currentPwdValidator->setMandatory(true);
    newPwdValidator->setMandatory(true);
    confirmPwdValidator->setMandatory(true);
    currentEmailValidator->setMandatory(true);
    currentEmailPwdValidator->setMandatory(true);

    m_currentPwdEdit->setValidator(currentPwdValidator);
    m_newPwdEdit->setValidator(newPwdValidator);
    m_confirmPwdEdit->setValidator(confirmPwdValidator);
    m_currentEmailEdit->setValidator(currentEmailValidator);
    m_currentEmailPwdEdit->setValidator(currentEmailPwdValidator);

    m_currentPwdEdit->enterPressed().connect(this, &CmsPwdEmail::OnPwdOK);
    m_newPwdEdit->enterPressed().connect(this, &CmsPwdEmail::OnPwdOK);
    m_confirmPwdEdit->enterPressed().connect(this, &CmsPwdEmail::OnPwdOK);
    m_currentEmailEdit->enterPressed().connect(this, &CmsPwdEmail::OnEmailOK);
    m_currentEmailPwdEdit->enterPressed().connect(this, &CmsPwdEmail::OnEmailOK);
    changePwdButton->clicked().connect(this, &CmsPwdEmail::OnPwdOK);
    changeEmailButton->clicked().connect(this, &CmsPwdEmail::OnEmailOK);

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


