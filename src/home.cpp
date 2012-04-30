/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WButtonGroup>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WFileUpload>
#include <Wt/WGridLayout>
#include <Wt/WGroupBox>
#include <Wt/WImage>
#include <Wt/WIntValidator>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WLink>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WMessageBox>
#include <Wt/WProgressBar>
#include <Wt/WPushButton>
#include <Wt/WRadioButton>
#include <Wt/WRegExpValidator>
#include <Wt/WSubMenuItem>
#include <Wt/WStackedWidget>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WVBoxLayout>
#include <Wt/WWidget>
#include "home.hpp"
#include "appointment.hpp"
#include "appsubmit.hpp"
#include "base.hpp"
#include "cdate.hpp"
#include "cimage.hpp"
#include "contactus.hpp"
#include "crypto.hpp"
#include "db.hpp"
#include "dbtables.hpp"
#include "fileuploadprogressdialog.hpp"
#include "ideasubmit.hpp"
#include "mailer.hpp"
#include "myapps.hpp"
#include "myideas.hpp"
#include "news.hpp"
#include "search.hpp"
#include "system.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace boost;
using namespace boost::algorithm;
using namespace Wt;
using namespace cppdb;
using namespace SamsungDForumIr;

namespace fs = boost::filesystem;


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

Home::Home(CgiRoot *cgi) : BaseWidget(cgi),
    m_jsRegister(this, "JSRegister"),
    m_jsSubmitIdea(this, "JSSubmitIdea"),
    m_jsSubmitApp(this, "JSSubmitApp")
{
    m_msg = NULL;
    m_dlg = NULL;

    m_fileUploadProgressDialog = NULL;

    m_isUserLoggedIn = false;
    m_initialSelectedMenuItem = NULL;

    m_isInSubmitAppProcess = false;
    m_isInSubmitIdeaProcess = false;

    this->clear();
    this->setId("HomePageWidget");
    this->addWidget(Layout());

    m_jsRegister.connect(this, &Home::OnJSRegisterTriggered);
    m_jsSubmitIdea.connect(this, &Home::OnJSSubmitIdeaTriggered);
    m_jsSubmitApp.connect(this, &Home::OnJSSubmitAppTriggered);
}


/**********************************
*    = Destructor =
**********************************/

Home::~Home()
{
    delete m_msg;
    delete m_dlg;
    m_msg = NULL;
    m_dlg = NULL;
}


/**********************************
*    = Public Methods =
**********************************/

/**********************************
*    = Event Handlers =
**********************************/

Wt::JSignal<> &Home::JSRegister()
{
    return m_jsRegister;
}

void Home::OnJSRegisterTriggered()
{
    if (!m_isUserLoggedIn) {
        OnRegisterLinkPressed();
    }
}

Wt::JSignal<> &Home::JSSubmitIdea()
{
    return m_jsSubmitIdea;
}

void Home::OnJSSubmitIdeaTriggered()
{
    if (m_isUserLoggedIn) {
        OnSubmitIdeaLinkPressed();
    } else {
        OnSignInLinkPressed();
    }
}

Wt::JSignal<> &Home::JSSubmitApp()
{
    return m_jsSubmitApp;
}

void Home::OnJSSubmitAppTriggered()
{
    if (m_isUserLoggedIn) {
        OnSubmitAppLinkPressed();
    } else {
        OnSignInLinkPressed();
    }
}

void Home::OnMenuItemSelected(WMenuItem *sender)
{
    if (sender->text() == m_lang->GetString("HOME_MENU_FORUMS")) {
        switch(m_cgiEnv->CurrentLang) {
        case CgiEnv::ELang_En:
            Exit("./forums/en/");
            break;
        case CgiEnv::ELang_Fa:
            Exit("./forums/fa/");
            break;
        default:
            Exit(".");
            break;
        }
    }
}

void Home::OnSearchEditEnterPressed()
{
    if (trim_copy(m_searchLineEdit->text().toUTF8()) == "") {
        return;
    }

    WVBoxLayout *layout = new WVBoxLayout();
    WContainerWidget* results = new WContainerWidget();
    results->setOverflow(WContainerWidget::OverflowAuto);
    results->addWidget(new Search(m_searchLineEdit->text(), m_cgiRoot));

    layout->addWidget(results);
    layout->setStretchFactor(results, 0);

    m_dlg = new WDialog((wformat(m_lang->GetString("HOME_SEARCH_RESULTS_DLG_TITLE"))
                         % m_searchLineEdit->text().value()).str());
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(768, Wt::WLength::Pixel),
                  Wt::WLength(64, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);
    m_dlg->contents()->setLayout(layout);
    m_dlg->show();
}

void Home::OnSearchResultsReturn()
{
    delete m_dlg;
    m_dlg = NULL;
}

void Home::OnSignInLinkPressed()
{
    m_dlg = new WDialog(m_lang->GetString("HOME_SIGN_IN_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->contents()->addWidget(GetSignInForm());
    m_dlg->rejectWhenEscapePressed();
    m_dlg->show();
    m_signInUserNameLineEdit->setFocus();
}

void Home::OnRegisterLinkPressed()
{
    m_dlg = new WDialog(m_lang->GetString("HOME_REGISTER_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->contents()->addWidget(GetRegisterForm());
    m_dlg->rejectWhenEscapePressed();
    m_dlg->show();
    m_registerEmailLineEdit->setFocus();
}

void Home::OnRegisterTypeRadioButtonCheckedChanged(WRadioButton *sender)
{
    m_selectedRegisterType = (ERegisterType)sender->group()->checkedId();

    m_dvRegisterForm->clear();

    Div *dvForm = new Div(m_dvRegisterForm);
    dvForm->setStyleClass("form");
    WGridLayout *dvFormLayout = new WGridLayout();

    m_registerEmailLineEdit = new WLineEdit();
    m_registerPwdLineEdit = new WLineEdit();
    m_registerPwdLineEdit->setEchoMode(WLineEdit::Password);
    m_registerPwdConfirmLineEdit = new WLineEdit();
    m_registerPwdConfirmLineEdit->setEchoMode(WLineEdit::Password);

    m_registerCaptchaLineEdit = new WLineEdit();


    dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_EMAIL_TEXT")),
                                      0, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(m_registerEmailLineEdit, 0, 1);

    dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_PWD_TEXT")),
                                      1, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(m_registerPwdLineEdit, 1, 1);
    dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_PWD_CONFIRM_TEXT")),
                                      2, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(m_registerPwdConfirmLineEdit, 2, 1);


    switch(sender->group()->checkedId()) {
    case ERegisterType_Individual:
        m_registerIndividualFirstNameLineEdit = new WLineEdit();
        m_registerIndividualLastNameLineEdit = new WLineEdit();
        m_registerIndividualContactNumberLineEdit = new WLineEdit();
        m_registerIndividualTeammatesTextArea = new WTextArea();
        m_registerIndividualTeammatesTextArea->setEmptyText(m_lang->GetString("HOME_REGISTER_TEAMMATES_HINT"));
        m_registerIndividualTeammatesTextArea->resize(WLength::Auto, 100);


        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_FIRST_NAME_TEXT")),
                                          3, 0, AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_registerIndividualFirstNameLineEdit, 3, 1);
        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_LAST_NAME_TEXT")),
                                          4, 0, AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_registerIndividualLastNameLineEdit, 4, 1);
        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_CONTACT_NUMBER_TEXT")),
                                          5, 0, AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_registerIndividualContactNumberLineEdit, 5, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_TEAMMATES_TEXT")),
                                          6, 0, AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_registerIndividualTeammatesTextArea, 6, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_CAPTCHA_TEXT")),
                                          7, 0, AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_registerCaptchaLineEdit, 7, 1);
        break;

    case ERegisterType_Company:
        m_registerCompanyNameLineEdit = new WLineEdit();
        m_registerCompanyRegisterationIdLineEdit = new WLineEdit();
        m_registerCompanyPhoneLineEdit = new WLineEdit();
        m_registerCompanyPhoneLineEdit->setEmptyText(m_lang->GetString("PHONE_GENERIC_HINT"));
        m_registerCompanyCEONameLineEdit = new WLineEdit();

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_COMPANY_NAME_TEXT")),
                                          3, 0, AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_registerCompanyNameLineEdit, 3, 1);
        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_REGISTERATION_ID_TEXT")),
                                          4, 0, AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_registerCompanyRegisterationIdLineEdit, 4, 1);
        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_PHONE_TEXT")),
                                          5, 0, AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_registerCompanyPhoneLineEdit, 5, 1);
        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_CEO_NAME_TEXT")),
                                          6, 0, AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_registerCompanyCEONameLineEdit, 6, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_REGISTER_CAPTCHA_TEXT")),
                                          7, 0, AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_registerCaptchaLineEdit, 7, 1);
        break;

    default:
        break;
    }

    dvFormLayout->setColumnStretch(0, 100);
    dvFormLayout->setColumnStretch(1, 200);
    dvFormLayout->setVerticalSpacing(11);
    dvForm->resize(300, WLength::Auto);
    dvForm->setLayout(dvFormLayout);

    Div *dvCaptcha = new Div(m_dvRegisterForm, "dvCaptcha");
    m_registerCaptchaImage = m_captcha->GenCap();
    dvCaptcha->addWidget(m_registerCaptchaImage);


    WRegExpValidator *emailValidator = new WRegExpValidator(Base::REGEX_EMAIL);
    WLengthValidator *pwdValidator = new WLengthValidator(Base::MIN_PASSWORD_LENGTH, Base::MAX_PASSWORD_LENGTH);
    WLengthValidator *individualFirstNameValidator;
    WLengthValidator *individualLastNameValidator;
    WRegExpValidator *individualConatactNumberValidator;
    WLengthValidator *companyNameValidator;
    WLengthValidator *companyRegisterationIdValidator;
    WRegExpValidator *companyPhoneValidator;
    WLengthValidator *companyCEONameValidator;
    m_registerCaptchaValidator = new WIntValidator(m_captcha->Result, m_captcha->Result);

    emailValidator->setFlags(MatchCaseInsensitive);

    emailValidator->setMandatory(true);
    pwdValidator->setMandatory(true);
    m_registerCaptchaValidator->setMandatory(true);

    m_registerEmailLineEdit->setValidator(emailValidator);
    m_registerPwdLineEdit->setValidator(pwdValidator);
    m_registerPwdConfirmLineEdit->setValidator(pwdValidator);
    m_registerCaptchaLineEdit->setValidator(m_registerCaptchaValidator);

    wstring pwdHint = (wformat(m_lang->GetString("PWD_HINT")) % Base::MIN_PASSWORD_LENGTH % Base::MAX_PASSWORD_LENGTH).str();
    switch(m_cgiEnv->CurrentLang) {
    case CgiEnv::ELang_Fa:
        pwdHint = CDate::DateConv::FormatToPersianNums(pwdHint);
        break;
    default:
        break;
    }

    m_registerPwdLineEdit->setEmptyText(pwdHint);
    m_registerCaptchaLineEdit->setEmptyText(m_lang->GetString("CAPTCHA_HINT"));

    m_registerEmailLineEdit->enterPressed().connect(this, &Home::OnRegisterOK);
    m_registerPwdLineEdit->enterPressed().connect(this, &Home::OnRegisterOK);
    m_registerPwdConfirmLineEdit->enterPressed().connect(this, &Home::OnRegisterOK);
    m_registerCaptchaLineEdit->enterPressed().connect(this, &Home::OnRegisterOK);

    switch(sender->group()->checkedId()) {
    case ERegisterType_Individual:
        emailValidator = new WRegExpValidator(Base::REGEX_EMAIL);
        pwdValidator = new WLengthValidator(Base::MIN_PASSWORD_LENGTH, Base::MAX_PASSWORD_LENGTH);
        individualFirstNameValidator = new WLengthValidator(Base::REGISTER_MIN_NAME_LEN, Base::REGISTER_MAX_NAME_LEN);
        individualLastNameValidator = new WLengthValidator(Base::REGISTER_MIN_NAME_LEN, Base::REGISTER_MAX_NAME_LEN);
        individualConatactNumberValidator = new WRegExpValidator(Base::REGEX_PHONE_GENERIC);

        individualFirstNameValidator->setMandatory(true);
        individualLastNameValidator->setMandatory(true);
        individualConatactNumberValidator->setMandatory(true);

        m_registerIndividualFirstNameLineEdit->setValidator(individualFirstNameValidator);
        m_registerIndividualLastNameLineEdit->setValidator(individualLastNameValidator);
        m_registerIndividualContactNumberLineEdit->setValidator(individualConatactNumberValidator);

        m_registerIndividualContactNumberLineEdit->setEmptyText(m_lang->GetString("PHONE_GENERIC_HINT"));

        m_registerIndividualFirstNameLineEdit->enterPressed().connect(this, &Home::OnRegisterOK);
        m_registerIndividualLastNameLineEdit->enterPressed().connect(this, &Home::OnRegisterOK);
        m_registerIndividualContactNumberLineEdit->enterPressed().connect(this, &Home::OnRegisterOK);
        break;

    case ERegisterType_Company:
        companyNameValidator = new WLengthValidator(1, 100);
        companyRegisterationIdValidator = new WLengthValidator(1, 10);
        companyPhoneValidator = new WRegExpValidator(Base::REGEX_PHONE_GENERIC);
        companyCEONameValidator = new WLengthValidator(Base::REGISTER_MIN_NAME_LEN, Base::REGISTER_MAX_NAME_LEN);

        companyNameValidator->setMandatory(true);
        companyRegisterationIdValidator->setMandatory(true);
        companyPhoneValidator->setMandatory(true);
        companyCEONameValidator->setMandatory(true);

        m_registerCompanyNameLineEdit->setValidator(companyNameValidator);
        m_registerCompanyRegisterationIdLineEdit->setValidator(companyRegisterationIdValidator);
        m_registerCompanyPhoneLineEdit->setValidator(companyPhoneValidator);
        m_registerCompanyCEONameLineEdit->setValidator(companyCEONameValidator);

        m_registerCompanyNameLineEdit->enterPressed().connect(this, &Home::OnRegisterOK);
        m_registerCompanyRegisterationIdLineEdit->enterPressed().connect(this, &Home::OnRegisterOK);
        m_registerCompanyPhoneLineEdit->enterPressed().connect(this, &Home::OnRegisterOK);
        m_registerCompanyCEONameLineEdit->enterPressed().connect(this, &Home::OnRegisterOK);
        break;

    default:
        break;
    }


    // Dirty HACK for 3.2.3-rc1
    switch(sender->group()->checkedId()) {
    case ERegisterType_Individual:
        // do not use if
        m_dlg->resize(WLength::Auto, 570);
        break;

    case ERegisterType_Company:
        // do not use if
        m_dlg->resize(WLength::Auto, 315); // not working because the above one called first
        break;

    default:
        break;
    }
    // End of Dirty HACK for 3.2.3-rc1

    m_registerEmailLineEdit->setFocus();
}

void Home::OnSignInOK()
{
    if (Validate(m_signInCaptchaLineEdit)) {
        if(!Validate(m_signInUserNameLineEdit) || !Validate(m_signInPwdLineEdit)) {
            GenCap(m_signInCaptchaImage, m_signInCaptchaValidator);
            return;
        }
    } else {
        GenCap(m_signInCaptchaImage, m_signInCaptchaValidator);
        return;
    }

    string email(trim_copy(m_signInUserNameLineEdit->text().toUTF8()));
    string pwd(Crypto::Encrypt(m_signInPwdLineEdit->text().toUTF8()));

    result r = m_db->Sql() << "SELECT verified FROM ["
                              + m_dbTables->Table("USERS")
                              + "] WHERE email=? AND pwd=?;" << email << pwd << row;
    if (r.empty()) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_TITLE"),
                                m_lang->GetString("HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_RETRY_BUTTON"), Retry);
        m_msg->addButton(m_lang->GetString("HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_RECOVER_BUTTON"), NoButton);
        m_msg->buttonClicked().connect(this, &Home::OnSignInFailedOK);
        m_msg->show();

        return;
    }

    string verified;
    r >> verified;
    if (verified == "0") {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_TITLE"),
                                (wformat(m_lang->GetString("HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_BODY"))
                                 % m_signInUserNameLineEdit->text()).str(),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_RESEND_BUTTON"), Retry);
        m_msg->addButton(m_lang->GetString("HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_CANCEL_BUTTON"), Cancel);
        m_msg->buttonClicked().connect(this, &Home::OnSignInNotVerifiedOK);
        m_msg->show();

        return;
    }

    m_cgiEnv->LoggedInUserName = email;

    m_dvLogin->clear();

    WText *myAppsLink = new WText(m_lang->GetString("HOME_MY_APPS_LINK_TEXT"), m_dvLogin);
    myAppsLink->setStyleClass("link");
    myAppsLink->clicked().connect(this, &Home::OnMyAppsLinkPressed);

    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", m_dvLogin);

    WText *myIdeasLink = new WText(m_lang->GetString("HOME_MY_IDEAS_LINK_TEXT"), m_dvLogin);
    myIdeasLink->setStyleClass("link");
    myIdeasLink->clicked().connect(this, &Home::OnMyIdeasLinkPressed);

    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", m_dvLogin);

    if (m_cgiEnv->CurrentLang == CgiEnv::ELang_Fa) {
        WText *appointmentLink = new WText(m_lang->GetString("HOME_APPOINTMENT_LINK_TEXT"), m_dvLogin);
        appointmentLink->setStyleClass("link");
        appointmentLink->clicked().connect(this, &Home::OnAppointmentLinkPressed);

        new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", m_dvLogin);
    }

    WText *myProfileLink = new WText(m_signInUserNameLineEdit->text(), m_dvLogin);
    myProfileLink->setStyleClass("link");
    myProfileLink->clicked().connect(this, &Home::OnMyProfileLinkPressed);

    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", m_dvLogin);

    WText *pwdLink = new WText(m_lang->GetString("HOME_PWD_LINK_TEXT"), m_dvLogin);
    pwdLink->setStyleClass("link");
    pwdLink->clicked().connect(this, &Home::OnPwdLinkPressed);

    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", m_dvLogin);

    WText *signInLink = new WText(m_lang->GetString("HOME_SIGNOUT_LINK_TEXT"), m_dvLogin);
    signInLink->setStyleClass("link");
    signInLink->clicked().connect(this, &Home::OnSignOutLinkPressed);

    m_isUserLoggedIn = true;
    m_isIndividualUser = IsIndividualUser(m_cgiEnv->LoggedInUserName);

    delete m_dlg;
    m_dlg = NULL;
}

void Home::OnSignInReturn()
{
    delete m_dlg;
    m_dlg = NULL;
}

void Home::OnSignInFailedOK(StandardButton result)
{
    delete m_msg;
    m_msg = NULL;

    GenCap(m_signInCaptchaImage, m_signInCaptchaValidator);

    switch(result) {
    case Retry:
        m_signInUserNameLineEdit->setFocus();
        break;

    case NoButton: // Recover Button
        {
            string email(trim_copy(m_signInUserNameLineEdit->text().toUTF8()));

            cppdb::result r = m_db->Sql() << "SELECT pwd FROM ["
                                             + m_dbTables->Table("USERS")
                                             + "] WHERE email=?;" << email << row;
            if (r.empty()) {
                wstring body((wformat(m_lang->GetString("HOME_SIGN_IN_DLG_WRONG_EMAIL_ERR_BODY"))
                              % m_signInUserNameLineEdit->text().value()).str());
                m_msg = new WMessageBox(m_lang->GetString("HOME_SIGN_IN_DLG_WRONG_EMAIL_ERR_TITLE"),
                                        body, Warning, NoButton);
                m_msg->addButton(m_lang->GetString("HOME_SIGN_IN_DLG_WRONG_EMAIL_ERR_OK_BUTTON"), Ok);
                m_msg->buttonClicked().connect(this, &Home::OnSignInRecoverInvalidEmailOK);
                m_msg->show();

                return;
            } else {
                CDate::Now n;

                string pwd;
                r >> pwd;
                pwd = Crypto::Decrypt(pwd);

                Mailer::SendLoginRecovery(email, email, pwd, n, m_cgiEnv.get());

                m_msg = new WMessageBox(m_lang->GetString("HOME_SIGN_IN_DLG_RECOVERED_PWD_MSG_TITLE"),
                                        m_lang->GetString("HOME_SIGN_IN_DLG_RECOVERED_PWD_MSG_BODY"),
                                        Warning, NoButton);
                m_msg->addButton(m_lang->GetString("HOME_SIGN_IN_DLG_RECOVERED_PWD_MSG_OK_BUTTON"), Ok);
                m_msg->buttonClicked().connect(this, &Home::OnSignInRecoverSuccessOK);
                m_msg->show();

                return;
            }
        }
        break;

    default:
        break;
    }

}

void Home::OnSignInRecoverInvalidEmailOK(StandardButton result)
{
    delete m_msg;
    m_msg = NULL;
}

void Home::OnSignInRecoverSuccessOK(StandardButton result)
{
    delete m_msg;
    m_msg = NULL;
}

void Home::OnSignInNotVerifiedOK(StandardButton result)
{
    delete m_msg;
    m_msg = NULL;

    CDate::Now n;
    string email;
    wstring body;

    switch(result) {
    case Retry: // 'Resend' Button
        email = trim_copy(m_signInUserNameLineEdit->text().toUTF8());

        Mailer::SendVerificationMessage(email, email, n, m_cgiEnv.get());

        body = (wformat(m_lang->GetString("HOME_SIGN_IN_DLG_VERIFICATION_RESENT_MSG_BODY"))
                % m_signInUserNameLineEdit->text().value()).str();
        m_msg = new WMessageBox(m_lang->GetString("HOME_SIGN_IN_DLG_VERIFICATION_RESENT_MSG_TITLE"),
                                body, Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SIGN_IN_DLG_VERIFICATION_RESENT_MSG_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &Home::OnSignInNotVerifiedResendSuccessOK);
        m_msg->show();
        break;

    case Cancel: // 'Cancel Sign-In' Button
        break;

    default:
        break;
    }
}

void Home::OnSignInNotVerifiedResendSuccessOK(StandardButton result)
{
    delete m_msg;
    m_msg = NULL;
}

void Home::OnRegisterOK()
{
    if (Validate(m_registerCaptchaLineEdit)) {
        switch(m_selectedRegisterType) {
        case ERegisterType_Individual:
            if(!Validate(m_registerEmailLineEdit) || !Validate(m_registerPwdLineEdit)
                    || !Validate(m_registerPwdConfirmLineEdit)
                    || !Validate(m_registerIndividualFirstNameLineEdit) || !Validate(m_registerIndividualLastNameLineEdit)
                    || !Validate(m_registerIndividualContactNumberLineEdit)) {
                GenCap(m_registerCaptchaImage, m_registerCaptchaValidator);
                return;
            }
            break;

        case ERegisterType_Company:
            if(!Validate(m_registerEmailLineEdit) || !Validate(m_registerPwdLineEdit)
                    || !Validate(m_registerPwdConfirmLineEdit)
                    || !Validate(m_registerCompanyNameLineEdit) || !Validate(m_registerCompanyRegisterationIdLineEdit)
                    || !Validate(m_registerCompanyPhoneLineEdit) || !Validate(m_registerCompanyCEONameLineEdit)) {
                GenCap(m_registerCaptchaImage, m_registerCaptchaValidator);
                return;
            }
            break;

        default:
            break;
        }

        if (m_registerPwdLineEdit->text() != m_registerPwdConfirmLineEdit->text()) {
            m_registerPwdConfirmLineEdit->setFocus();
            GenCap(m_registerCaptchaImage, m_registerCaptchaValidator);
            return;
        }
    } else {
        GenCap(m_registerCaptchaImage, m_registerCaptchaValidator);
        return;
    }



    string email(trim_copy(m_registerEmailLineEdit->text().toUTF8()));

    result r = m_db->Sql() << "SELECT email FROM ["
                              + m_dbTables->Table("USERS")
                              + "] WHERE email=?;" << email << row;

    if (!r.empty()) {
        wstring body = (wformat(m_lang->GetString("HOME_REGISTER_DLG_ALREADY_EXISTS_ERR_BODY"))
                        % m_registerEmailLineEdit->text().value()).str();

        m_msg = new WMessageBox(m_lang->GetString("HOME_REGISTER_DLG_ALREADY_EXISTS_ERR_TITLE"),
                                body, Warning, NoButton);

        m_msg->addButton(m_lang->GetString("HOME_REGISTER_DLG_ALREADY_EXISTS_ERR_OK_BUTTON"), Ok);

        m_msg->buttonClicked().connect(this, &Home::OnRegisterAlreadyExistsOK);

        m_msg->show();

        return;
    }

    m_db->Insert(m_dbTables->Table("USERS"), "email, pwd, verified",
                 3, email.c_str(),
                 Crypto::Encrypt(m_registerPwdLineEdit->text().toUTF8()).c_str(),
                 "0");

    switch(m_selectedRegisterType) {
    case ERegisterType_Individual:
        m_db->Insert(m_dbTables->Table("USERS_INDIVIDUAL"), "email, firstname, lastname, phone, teammates",
                     5, email.c_str(),
                     Crypto::Encrypt(m_registerIndividualFirstNameLineEdit->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_registerIndividualLastNameLineEdit->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_registerIndividualContactNumberLineEdit->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_registerIndividualTeammatesTextArea->text().toUTF8()).c_str()
                     );
        break;

    case ERegisterType_Company:
        m_db->Insert(m_dbTables->Table("USERS_COMPANY"), "email, name, regid, phone, ceo",
                     5, email.c_str(),
                     Crypto::Encrypt(m_registerCompanyNameLineEdit->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_registerCompanyRegisterationIdLineEdit->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_registerCompanyPhoneLineEdit->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_registerCompanyCEONameLineEdit->text().toUTF8()).c_str()
                     );
        break;

    default:
        break;
    }

    CDate::Now n;
    Mailer::SendVerificationMessage(email, email, n, m_cgiEnv.get());

    wstring body = (wformat(m_lang->GetString("HOME_REGISTER_DLG_REGISTERED_MSG_BODY"))
                    % m_registerEmailLineEdit->text().value()).str();
    m_msg = new WMessageBox(m_lang->GetString("HOME_REGISTER_DLG_REGISTERED_MSG_TITLE"),
                            body, Warning, NoButton);
    m_msg->addButton(m_lang->GetString("HOME_REGISTER_DLG_REGISTERED_MSG_OK_BUTTON"), Ok);
    m_msg->buttonClicked().connect(this, &Home::OnRegisterSuccessOK);
    m_msg->show();
}

void Home::OnRegisterSuccessOK(StandardButton result)
{
    delete m_msg;
    m_msg = NULL;

    delete m_dlg;
    m_dlg = NULL;
}

void Home::OnRegisterAlreadyExistsOK(StandardButton result)
{
    m_registerEmailLineEdit->setFocus();
    delete m_msg;
    m_msg = NULL;
}

void Home::OnRegisterReturn()
{
    delete m_dlg;
    m_dlg = NULL;
}

void Home::OnMyAppsLinkPressed()
{
    delete m_dlg;
    m_dlg = NULL;

    m_dlg = new WDialog(m_lang->GetString("HOME_MY_APPS_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(85, Wt::WLength::Percentage),
                  Wt::WLength(85, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);

    new MyApps(m_cgiRoot, m_dlg);

    m_dlg->show();
}

void Home::OnMyIdeasLinkPressed()
{
    delete m_dlg;
    m_dlg = NULL;

    m_dlg = new WDialog(m_lang->GetString("HOME_MY_IDEAS_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(85, Wt::WLength::Percentage),
                  Wt::WLength(85, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);

    new MyIdeas(m_cgiRoot, m_dlg);

    m_dlg->show();
}

void Home::OnAppointmentLinkPressed()
{
    delete m_dlg;
    m_dlg = NULL;

    m_dlg = new WDialog(m_lang->GetString("HOME_APPOINTMENT_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(false);

    new Appointment(m_cgiRoot, m_dlg);

    m_dlg->show();

    // HACK for 3.2.3-rc1
    m_dlg->resize(750, 550);
}

void Home::OnMyProfileLinkPressed()
{
    m_dlg = new WDialog(m_lang->GetString("HOME_MY_PROFILE_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->contents()->addWidget(GetMyProfileForm());
    m_dlg->rejectWhenEscapePressed();
    m_dlg->show();
    if (m_isIndividualUser) {
        m_myProfileIndividualFirstNameLineEdit->setFocus();

        // HACK for 3.2.3-rc1
        if (m_dlg->height().value() < 455)
            m_dlg->resize(WLength::Auto, 455);
    }
    else {
        m_myProfileCompanyNameLineEdit->setFocus();

        // HACK for 3.2.3-rc1
        if (m_dlg->height().value() < 435)
            m_dlg->resize(WLength::Auto, 435);
    }
}

void Home::OnMyProfileEducationLevelComboBoxChanged(int index)
{
    if (index != EIEDULEVEL_NONE) {
        m_myProfileIndividualEducationLevelComboBox->removeStyleClass("Wt-invalid", true);
    } else {
        m_myProfileIndividualEducationLevelComboBox->setStyleClass("Wt-invalid");
    }
}

void Home::OnMyProfileFoundingYearComboBoxChanged(int index)
{
    if (index != 0) {
        m_myProfileCompanyFoundingYearComboBox->removeStyleClass("Wt-invalid", true);
    } else {
        m_myProfileCompanyFoundingYearComboBox->setStyleClass("Wt-invalid");
    }
}

void Home::OnMyProfileOK()
{
    if (m_isIndividualUser) {
        if (!Validate(m_myProfileIndividualFirstNameLineEdit)
                || !Validate(m_myProfileIndividualLastNameLineEdit)
                || !Validate(m_myProfileIndividualContactNumberLineEdit)
                || !Validate(m_myProfileIndividualAlternativeContactNumberLineEdit)
                || !Validate(m_myProfileIndividualTeammatesTextArea)
                || !Validate(m_myProfileIndividualNationalCodeLineEdit)
                || !Validate(m_myProfileIndividualEducationSubjectLineEdit)
                || !Validate(m_myProfileIndividualWebsiteLineEdit)) {
            return;
        }

        if (m_myProfileIndividualEducationLevelComboBox->currentIndex() == EIEDULEVEL_NONE) {
            m_myProfileIndividualEducationLevelComboBox->setFocus();
            return;
        }
    } else {
        if (!Validate(m_myProfileCompanyNameLineEdit)
                || !Validate(m_myProfileCompanyRegisterationIdLineEdit)
                || !Validate(m_myProfileCompanyPhoneLineEdit)
                || !Validate(m_myProfileCompanyAlternativePhoneLineEdit)
                || !Validate(m_myProfileCompanyCEONameLineEdit)
                || !Validate(m_myProfileCompanyEconomicalCodeLineEdit)
                || !Validate(m_myProfileCompanyAddressLineEdit)
                || !Validate(m_myProfileCompanyWebsiteLineEdit)) {
            return;
        }

        if (m_myProfileCompanyFoundingYearComboBox->currentIndex() == 0) {
            m_myProfileCompanyFoundingYearComboBox->setFocus();
            return;
        }
    }


    if (m_isIndividualUser) {
        result r = m_db->Sql() << "SELECT email FROM ["
                                  + m_dbTables->Table("USERS_INDIVIDUAL")
                                  + "] WHERE email=?;"
                           << m_cgiEnv->LoggedInUserName
                           << row;
        if (!r.empty()) {
            m_db->Update(m_dbTables->Table("USERS_INDIVIDUAL"), "email", m_cgiEnv->LoggedInUserName,
                         "firstname=?, lastname=?, phone=?, teammates=?", 4,
                         Crypto::Encrypt(m_myProfileIndividualFirstNameLineEdit->text().toUTF8()).c_str(),
                         Crypto::Encrypt(m_myProfileIndividualLastNameLineEdit->text().toUTF8()).c_str(),
                         Crypto::Encrypt(m_myProfileIndividualContactNumberLineEdit->text().toUTF8()).c_str(),
                         Crypto::Encrypt(m_myProfileIndividualTeammatesTextArea->text().toUTF8()).c_str());
        }

        string altphone(Crypto::Encrypt(m_myProfileIndividualAlternativeContactNumberLineEdit->text().toUTF8()));
        string nationalcode(Crypto::Encrypt(m_myProfileIndividualNationalCodeLineEdit->text().toUTF8()));
        string educationlevel(lexical_cast<string>(m_myProfileIndividualEducationLevelComboBox->currentIndex()));
        string educationsubject(Crypto::Encrypt(m_myProfileIndividualEducationSubjectLineEdit->text().toUTF8()));
        string website(Crypto::Encrypt(m_myProfileIndividualWebsiteLineEdit->text().toUTF8()));

        r = m_db->Sql() << "SELECT email FROM ["
                           + m_dbTables->Table("USERS_INDIVIDUAL_EXTRA")
                           + "] WHERE email=?;"
                        << m_cgiEnv->LoggedInUserName
                        << row;
        if (!r.empty()) {
            m_db->Update(m_dbTables->Table("USERS_INDIVIDUAL_EXTRA"), "email", m_cgiEnv->LoggedInUserName,
                         "altphone=?, nationalcode=?, educationlevel=?, educationsubject=?, website=?", 5,
                         altphone.c_str(),
                         nationalcode.c_str(),
                         educationlevel.c_str(),
                         educationsubject.c_str(),
                         website.c_str());
        } else {
            m_db->Insert(m_dbTables->Table("USERS_INDIVIDUAL_EXTRA"),
                         "email, altphone, nationalcode, educationlevel, educationsubject, website",
                         6, m_cgiEnv->LoggedInUserName.c_str(),
                         altphone.c_str(),
                         nationalcode.c_str(),
                         educationlevel.c_str(),
                         educationsubject.c_str(),
                         website.c_str());
        }

        m_msg = new WMessageBox(m_lang->GetString("HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_TITLE"),
                                m_lang->GetString("HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &Home::OnMyProfileSaveSuccessOK);
        m_msg->show();
    } else {
        // We need to disable this button upon click inorder
        // to make m_myProfileCompanyResumeFileUpload work correctly
        m_myProfileSaveButton->setDisabled(true);

        if (m_myProfileCompanyResumeFileUploadHasFile) {
            if (!m_myProfileCompanyResumeFileUpload->canUpload()) {
                m_msg = new WMessageBox(m_lang->GetString("HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_TITLE"),
                                        m_lang->GetString("HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_BODY"),
                                        Warning, NoButton);
                m_msg->addButton(m_lang->GetString("HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_OK_BUTTON"), Ok);
                m_msg->buttonClicked().connect(this, &Home::OnMyProfileSaveFailedOK);
                m_msg->show();

                return;
            }

            /*m_fileUploadProgressDialog = new FileUploadProgressDialog(m_lang);
            m_fileUploadProgressDialog->show();*/
            m_myProfileCompanyResumeFileUpload->upload();
        } else {
            OnMyProfileResumeFileUploaded();
        }
    }
}

void Home::OnMyProfileResumeFileChanged()
{
    m_myProfileCompanyResumeFileUploadHasFile = true;
}

void Home::OnMyProfileResumeFileTooLarge()
{
    m_msg = new WMessageBox(m_lang->GetString("HOME_MY_PROFILE_DLG_RESUME_FILE_TOO_LARGE_ERR_TITLE"),
                            m_lang->GetString("HOME_MY_PROFILE_DLG_RESUME_FILE_TOO_LARGE_ERR_BODY"),
                            Warning, NoButton);
    m_msg->addButton(m_lang->GetString("HOME_MY_PROFILE_DLG_RESUME_FILE_TOO_LARGE_ERR_OK_BUTTON"), Ok);
    m_msg->buttonClicked().connect(this, &Home::OnMyProfileSaveFailedOK);
    m_msg->show();
}

void Home::OnMyProfileResumeFileUploaded()
{
    /*delete m_fileUploadProgressDialog;
    m_fileUploadProgressDialog = NULL;*/

    if (m_myProfileCompanyResumeFileUploadHasFile) {
        if (!m_myProfileCompanyResumeFileUpload->empty()) {
            if (!CImage::IsPDF(m_myProfileCompanyResumeFileUpload->spoolFileName())) {
                m_msg = new WMessageBox(m_lang->GetString("HOME_MY_PROFILE_DLG_RESUME_FILE_NOT_PDF_ERR_TITLE"),
                                        m_lang->GetString("HOME_MY_PROFILE_DLG_RESUME_FILE_NOT_PDF_ERR_BODY"),
                                        Warning, NoButton);
                m_msg->addButton(m_lang->GetString("HOME_MY_PROFILE_DLG_RESUME_FILE_NOT_PDF_ERR_OK_BUTTON"), Ok);
                m_msg->buttonClicked().connect(this, &Home::OnMyProfileSaveFailedOK);
                m_msg->show();

                return;
            }
            if (!CImage::IsResumeFileSizeValid(m_myProfileCompanyResumeFileUpload->spoolFileName())) {
                OnMyProfileResumeFileTooLarge();
                return;
            }
        } else {
            m_msg = new WMessageBox(m_lang->GetString("HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_TITLE"),
                                    m_lang->GetString("HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_BODY"),
                                    Warning, NoButton);
            m_msg->addButton(m_lang->GetString("HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_OK_BUTTON"), Ok);
            m_msg->buttonClicked().connect(this, &Home::OnMyProfileSaveFailedOK);
            m_msg->show();
            return;
        }
    }

    string resumeurl;

    if (m_myProfileCompanyResumeFileUploadHasFile) {
        string userDir(Base::FSDB_USERS_PATH + "/" + m_cgiEnv->LoggedInUserName);

        try {
            if (!fs::exists(userDir)) {
                fs::create_directories(userDir);
            }
        }
        catch(const fs::filesystem_error &ex) {
            ERR(ex.what());
        }
        catch(...) {
        }

        try {
            resumeurl = userDir + "/" + Base::RESUME_FILE_NAME;
            System::CopyFile(m_myProfileCompanyResumeFileUpload->spoolFileName(),
                             resumeurl);
            //m_myProfileCompanyResumeFileUpload->stealSpooledFile();
        }
        catch(const fs::filesystem_error &ex) {
            ERR(ex.what());
        }
        catch(...) {
        }
    }

    result r = m_db->Sql() << "SELECT email FROM ["
                              + m_dbTables->Table("USERS_COMPANY")
                              + "] WHERE email=?;"
                       << m_cgiEnv->LoggedInUserName
                       << row;
    if (!r.empty()) {
        m_db->Update(m_dbTables->Table("USERS_COMPANY"), "email", m_cgiEnv->LoggedInUserName,
                     "name=?, regid=?, phone=?, ceo=?", 4,
                     Crypto::Encrypt(m_myProfileCompanyNameLineEdit->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_myProfileCompanyRegisterationIdLineEdit->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_myProfileCompanyPhoneLineEdit->text().toUTF8()).c_str(),
                     Crypto::Encrypt(m_myProfileCompanyCEONameLineEdit->text().toUTF8()).c_str());
    }

    string altphone(Crypto::Encrypt(m_myProfileCompanyAlternativePhoneLineEdit->text().toUTF8()));
    string economicalcode(Crypto::Encrypt(m_myProfileCompanyEconomicalCodeLineEdit->text().toUTF8()));
    string foundingyear(Crypto::Encrypt(m_myProfileCompanyFoundingYearComboBox->currentText().toUTF8()));
    string addr(Crypto::Encrypt(m_myProfileCompanyAddressLineEdit->text().toUTF8()));
    string website(Crypto::Encrypt(m_myProfileCompanyWebsiteLineEdit->text().toUTF8()));

    r = m_db->Sql() << "SELECT email FROM ["
                       + m_dbTables->Table("USERS_COMPANY_EXTRA")
                       + "] WHERE email=?;"
                    << m_cgiEnv->LoggedInUserName
                    << row;
    if (!r.empty()) {
        if (!m_myProfileCompanyResumeFileUploadHasFile) {
            m_db->Update(m_dbTables->Table("USERS_COMPANY_EXTRA"), "email", m_cgiEnv->LoggedInUserName,
                         "altphone=?, economicalcode=?, foundingyear=?, addr=?, website=?", 5,
                         altphone.c_str(),
                         economicalcode.c_str(),
                         foundingyear.c_str(),
                         addr.c_str(),
                         website.c_str());
        } else {
            m_db->Update(m_dbTables->Table("USERS_COMPANY_EXTRA"), "email", m_cgiEnv->LoggedInUserName,
                         "altphone=?, economicalcode=?, foundingyear=?, addr=?, website=?, resumeurl=?", 6,
                         altphone.c_str(),
                         economicalcode.c_str(),
                         foundingyear.c_str(),
                         addr.c_str(),
                         website.c_str(),
                         resumeurl.c_str());
        }
    } else {
        if (!m_myProfileCompanyResumeFileUploadHasFile) {
            m_db->Insert(m_dbTables->Table("USERS_COMPANY_EXTRA"),
                         "email, altphone, economicalcode, foundingyear, addr, website",
                         6, m_cgiEnv->LoggedInUserName.c_str(),
                         altphone.c_str(),
                         economicalcode.c_str(),
                         foundingyear.c_str(),
                         addr.c_str(),
                         website.c_str());
        } else {
            m_db->Insert(m_dbTables->Table("USERS_COMPANY_EXTRA"),
                         "email, altphone, economicalcode, foundingyear, addr, website, resumeurl",
                         7, m_cgiEnv->LoggedInUserName.c_str(),
                         altphone.c_str(),
                         economicalcode.c_str(),
                         foundingyear.c_str(),
                         addr.c_str(),
                         website.c_str(),
                         resumeurl.c_str());
        }
    }

    m_msg = new WMessageBox(m_lang->GetString("HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_TITLE"),
                            m_lang->GetString("HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_BODY"),
                            Warning, NoButton);
    m_msg->addButton(m_lang->GetString("HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_OK_BUTTON"), Ok);
    m_msg->buttonClicked().connect(this, &Home::OnMyProfileSaveSuccessOK);
    m_msg->show();

    m_myProfileCompanyResumeFileUploadHasFile = false;
}

void Home::OnMyProfileSaveSuccessOK(StandardButton result)
{
    m_myProfileCompanyResumeFileUploadHasFile = false;

    delete m_msg;
    m_msg = NULL;

    delete m_dlg;
    m_dlg = NULL;

    if (m_isInSubmitAppProcess) {
        m_isInSubmitAppProcess = false;
        ShowSubmitAppDialog();
    }

    if (m_isInSubmitIdeaProcess) {
        m_isInSubmitIdeaProcess = false;
        ShowSubmitIdeaDialog();
    }
}

void Home::OnMyProfileSaveFailedOK(StandardButton result)
{
    if (m_isIndividualUser) {
        m_myProfileIndividualFirstNameLineEdit->setFocus();
    }
    else {
        m_myProfileCompanyNameLineEdit->setFocus();
    }

    delete m_msg;
    m_msg = NULL;

    m_myProfileSaveButton->setEnabled(true);
}

void Home::OnMyProfileReturn()
{
    m_myProfileCompanyResumeFileUploadHasFile = false;
    m_isInSubmitAppProcess = false;
    m_isInSubmitIdeaProcess = false;

    delete m_dlg;
    m_dlg = NULL;
}

void Home::OnPwdLinkPressed()
{
    m_dlg = new WDialog(m_lang->GetString("HOME_CHANGE_PWD_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->contents()->addWidget(GetChangePwdForm());
    m_dlg->rejectWhenEscapePressed();
    m_dlg->show();
    m_changePwdCurrentLineEdit->setFocus();

    // HACK for 3.2.3-rc1
    if (m_dlg->height().value() < 195)
        m_dlg->resize(WLength::Auto, 195);
}

void Home::OnChangePwdOK()
{
    if(!Validate(m_changePwdCurrentLineEdit) || !Validate(m_changePwdNewLineEdit)
            || !Validate(m_changePwdConfirmLineEdit)) {
        return;
    }

    if (m_changePwdNewLineEdit->text() != m_changePwdConfirmLineEdit->text()) {
        m_changePwdConfirmLineEdit->setFocus();
        return;
    }

    result r = m_db->Sql() << "SELECT email FROM ["
                              + m_dbTables->Table("USERS")
                              + "] WHERE email=? AND pwd=?;"
                           << m_cgiEnv->LoggedInUserName
                           << Crypto::Encrypt(m_changePwdCurrentLineEdit->text().toUTF8())
                           << row;
    if (r.empty()) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_CHANGE_PWD_DLG_WRONG_PWD_ERR_TITLE"),
                                m_lang->GetString("HOME_CHANGE_PWD_DLG_WRONG_PWD_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_CHANGE_PWD_DLG_WRONG_PWD_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &Home::OnChangePwdInvalidOK);
        m_msg->show();

        return;
    }

    m_db->Update(m_dbTables->Table("USERS"), "email", m_cgiEnv->LoggedInUserName, "pwd=?", 1,
                 Crypto::Encrypt(m_changePwdNewLineEdit->text().toUTF8()).c_str());

    m_msg = new WMessageBox(m_lang->GetString("HOME_CHANGE_PWD_DLG_SUCCESS_MSG_TITLE"),
                            m_lang->GetString("HOME_CHANGE_PWD_DLG_SUCCESS_MSG_BODY"),
                            Warning, NoButton);
    m_msg->addButton(m_lang->GetString("HOME_CHANGE_PWD_DLG_SUCCESS_MSG_OK_BUTTON"), Ok);
    m_msg->buttonClicked().connect(this, &Home::OnChangePwdSuccessOK);
    m_msg->show();
}

void Home::OnChangePwdInvalidOK()
{
    delete m_msg;
    m_msg = NULL;

    m_changePwdCurrentLineEdit->setFocus();
}

void Home::OnChangePwdSuccessOK()
{
    delete m_msg;
    m_msg = NULL;

    delete m_dlg;
    m_dlg = NULL;
}

void Home::OnChangePwdReturn()
{
    delete m_dlg;
    m_dlg = NULL;
}

void Home::OnSignOutLinkPressed()
{
    Exit("./");
}

void Home::OnSubmitIdeaLinkPressed()
{
    bool hasExtraInfo = false;

    if (m_isIndividualUser) {
        result r = m_db->Sql() << "SELECT email FROM ["
                                  + m_dbTables->Table("USERS_INDIVIDUAL_EXTRA")
                                  + "] WHERE email=?;" << m_cgiEnv->LoggedInUserName << row;
        if (!r.empty()) {
            hasExtraInfo = true;
        }
    } else {
        result r = m_db->Sql() << "SELECT email FROM ["
                                  + m_dbTables->Table("USERS_COMPANY_EXTRA")
                                  + "] WHERE email=?;" << m_cgiEnv->LoggedInUserName << row;
        if (!r.empty()) {
            hasExtraInfo = true;
        }
    }

    if (hasExtraInfo) {
        ShowSubmitIdeaDialog();
    } else {
        m_isInSubmitIdeaProcess = true;
        OnMyProfileLinkPressed();
    }
}

void Home::OnSubmitAppLinkPressed()
{
    bool hasExtraInfo = false;

    if (m_isIndividualUser) {
        result r = m_db->Sql() << "SELECT email FROM ["
                                  + m_dbTables->Table("USERS_INDIVIDUAL_EXTRA")
                                  + "] WHERE email=?;" << m_cgiEnv->LoggedInUserName << row;
        if (!r.empty()) {
            hasExtraInfo = true;
        }
    } else {
        result r = m_db->Sql() << "SELECT email FROM ["
                                  + m_dbTables->Table("USERS_COMPANY_EXTRA")
                                  + "] WHERE email=?;" << m_cgiEnv->LoggedInUserName << row;
        if (!r.empty()) {
            hasExtraInfo = true;
        }
    }

    if (hasExtraInfo) {
        ShowSubmitAppDialog();
    } else {
        m_isInSubmitAppProcess = true;
        OnMyProfileLinkPressed();
    }
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

void Home::GenCap(WImage *captchaImage, WIntValidator *captchaValidator)
{
    captchaImage->setImageRef(m_captcha->GenCap()->imageRef());
    captchaValidator->setRange(m_captcha->Result, m_captcha->Result);
}

void Home::GetSubMenues(WMenu *menu, const string &fullpath, WStackedWidget *dvContents)
{
    result r = m_db->Sql() << "SELECT pg, fullpath, url FROM ["
                              + m_dbTables->Table("PAGES")
                              + "] WHERE parent=? "
                              "ORDER BY parent ASC, zindex ASC;" << fullpath;
    while (r.next()) {
        string pg;
        string childFullpath;
        string url;
        r >> pg >> childFullpath >> url;

        result rChild =  m_db->Sql() << "SELECT pg FROM ["
                                        + m_dbTables->Table("PAGES")
                                        + "] WHERE parent=? "
                                        "ORDER BY parent ASC, zindex ASC;" << childFullpath << row;
        if (rChild.empty()) {
            WMenuItem *menuItem = new WMenuItem(WString::fromUTF8(pg), CPageContent(childFullpath),
                                                       WMenuItem::PreLoading);
            menu->addItem(menuItem)->setPathComponent("");

            if (m_initialSelectedMenuItem == NULL) {
                if (m_cgiEnv->GetInitialURL() == url) {
                    menuItem->select();
                    m_initialSelectedMenuItem = menuItem;
                }
            }
        } else {
            WSubMenuItem *subMenuItem = new WSubMenuItem(WString::fromUTF8(pg), new WText(L""));
            Wt::WMenu *subMenu = new Wt::WMenu(dvContents, Wt::Vertical);
            subMenu->setRenderAsList(true);
            subMenuItem->setSubMenu(subMenu);
            menu->addItem(subMenuItem);
            subMenu->setStyleClass("menu submenu");
            GetSubMenues(subMenu, childFullpath, dvContents);
        }
    }
}

bool Home::IsIndividualUser(string email)
{

    cppdb::result r = m_db->Sql() << "SELECT email FROM ["
                              + m_dbTables->Table("USERS_INDIVIDUAL")
                              + "] WHERE email=?;" << email << row;
    if (!r.empty()) {
        return true;
    }

    /*r = m_db->Sql() << "SELECT email FROM ["
                       + m_dbTables->Table("USERS_COMPANY")
                       + "];" << row;
    if (!r.empty()) {
    }*/

    return false;
}

WWidget *Home::CHome()
{
    Div *root = new Div();

    result r = m_db->Sql() << "SELECT body, url FROM ["
                              + m_dbTables->Table("PAGES")
                              + "] WHERE fullpath=?;" << Base::DB_PAGE_FULLPATH_HOME << row;
    if (!r.empty()) {
        string body;
        string url;
        r >> body >> url;

        WText *contents = new WText(root);
        contents->setTextFormat(XHTMLUnsafeText);
        contents->setText(WString::fromUTF8(Crypto::Decrypt(body)));
    }

    root->addWidget(new News(m_cgiRoot));

    return root;
}

WWidget *Home::CContact()
{
    return new ContactUs(m_cgiRoot);
}

WWidget *Home::CPageContent(string &fullpath)
{
    Div *root = new Div();

    result r = m_db->Sql() << "SELECT body, url FROM ["
                              + m_dbTables->Table("PAGES")
                              + "] WHERE fullpath=?;" << fullpath << row;
    if (!r.empty()) {
        string body;
        string url;
        r >> body >> url;

        string lang;
        string pointer;
        switch(m_cgiEnv->CurrentLang) {
        case CgiEnv::ELang_En:
            lang = "en";
            pointer = " &gt; ";
            break;

        case CgiEnv::ELang_Fa:
            lang = "fa";
            //pointer = " &lt; ";
            pointer = " &gt; ";
            break;

        default:
            break;
        }

        Div *dvPageURL = new Div(root, "dvPageURL");
        new WAnchor((format("?lang=%1%&url=%2%") % lang % url).str(),
                    WString::fromUTF8(replace_all_copy(fullpath.substr(Base::DB_PAGE_FULLPATH_HOME.size() + 1), "/", pointer)),
                    dvPageURL);

        new WBreak(dvPageURL);
        new WBreak(dvPageURL);

        new WText(WString::fromUTF8(Crypto::Decrypt(body)), root);
    }

    return root;
}

Wt::WWidget *Home::GetSignInForm()
{
    Div *root = new Div("SignInForm");

    Div *dvForm = new Div(root);
    dvForm->setStyleClass("form");
    WGridLayout *dvFormLayout = new WGridLayout();

    m_signInUserNameLineEdit = new WLineEdit();
    m_signInPwdLineEdit = new WLineEdit();
    m_signInPwdLineEdit->setEchoMode(WLineEdit::Password);
    m_signInCaptchaLineEdit = new WLineEdit();

    dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_SIGN_IN_USER_NAME_TEXT")),
                                      0, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(m_signInUserNameLineEdit, 0, 1);

    dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_SIGN_IN_PWD_TEXT")),
                                      1, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(m_signInPwdLineEdit, 1, 1);
    dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_SIGN_IN_CAPTCHA_TEXT")),
                                      2, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(m_signInCaptchaLineEdit, 2, 1);

    dvFormLayout->setColumnStretch(0, 100);
    dvFormLayout->setColumnStretch(1, 200);
    dvFormLayout->setVerticalSpacing(11);
    dvForm->resize(300, WLength::Auto);
    dvForm->setLayout(dvFormLayout);

    Div *dvCaptcha = new Div(root, "dvCaptcha");
    m_signInCaptchaImage = m_captcha->GenCap();
    dvCaptcha->addWidget(m_signInCaptchaImage);

    Div *dvButtons = new Div(root, "dvDialogButtons");
    WPushButton *loginButton = new WPushButton(m_lang->GetString("HOME_SIGN_IN_DLG_LOGIN_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("HOME_SIGN_IN_DLG_RETURN_BUTTON"), dvButtons);

    loginButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    WRegExpValidator *userNameValidator = new WRegExpValidator(Base::REGEX_EMAIL);
    WLengthValidator *pwdValidator = new WLengthValidator(Base::MIN_PASSWORD_LENGTH,
                                                         Base::MAX_PASSWORD_LENGTH);
    m_signInCaptchaValidator = new WIntValidator(m_captcha->Result, m_captcha->Result);

    userNameValidator->setFlags(MatchCaseInsensitive);

    userNameValidator->setMandatory(true);
    pwdValidator->setMandatory(true);
    m_signInCaptchaValidator->setMandatory(true);

    m_signInUserNameLineEdit->setValidator(userNameValidator);
    m_signInPwdLineEdit->setValidator(pwdValidator);
    m_signInCaptchaLineEdit->setValidator(m_signInCaptchaValidator);

    m_signInCaptchaLineEdit->setEmptyText(m_lang->GetString("CAPTCHA_HINT"));

    m_signInUserNameLineEdit->enterPressed().connect(this, &Home::OnSignInOK);
    m_signInPwdLineEdit->enterPressed().connect(this, &Home::OnSignInOK);
    m_signInCaptchaLineEdit->enterPressed().connect(this, &Home::OnSignInOK);
    loginButton->clicked().connect(this, &Home::OnSignInOK);
    returnButton->clicked().connect(this, &Home::OnSignInReturn);

    return root;
}

Wt::WWidget *Home::GetRegisterForm()
{
    Div *root = new Div("RegisterForm");

    Div *dvRegType = new Div(root);

    WGroupBox *regTypeGroupBox = new WGroupBox(m_lang->GetString("HOME_REGISTER_REGISTERATION_TYPE_TEXT"), dvRegType);
    WButtonGroup *regTypeButtonGroup = new WButtonGroup(dvRegType);

    Wt::WRadioButton *individualRadioButton = new Wt::WRadioButton(m_lang->GetString("HOME_REGISTER_INDIVIDUAL_TEXT"), regTypeGroupBox);
    new WText("&nbsp;&nbsp;&nbsp;&nbsp;", regTypeGroupBox);
    regTypeButtonGroup->addButton(individualRadioButton, ERegisterType_Individual);
    Wt::WRadioButton *companyRadioButton = new Wt::WRadioButton(m_lang->GetString("HOME_REGISTER_COMPANY_TEXT"), regTypeGroupBox);
    regTypeButtonGroup->addButton(companyRadioButton, ERegisterType_Company);

    new WBreak(root);
    m_dvRegisterForm = new Div(root);
    // not necessary, we do it inside the OnRegisterTypeRadioButtonCheckedChanged function
    //m_dvRegisterForm->setStyleClass("form");

    Div *dvButtons = new Div(root, "dvDialogButtons");
    WPushButton *registerButton = new WPushButton(m_lang->GetString("HOME_REGISTER_DLG_REGISTER_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("HOME_REGISTER_DLG_RETURN_BUTTON"), dvButtons);

    registerButton->setDisabled(true);

    registerButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    regTypeButtonGroup->checkedChanged().connect(this, &Home::OnRegisterTypeRadioButtonCheckedChanged);

    registerButton->clicked().connect(this, &Home::OnRegisterOK);
    returnButton->clicked().connect(this, &Home::OnRegisterReturn);

    // Tip: ** You have to disable the okButton if the lines below deleted ** (also comment m_registerEmailLineEdit->setFocus() in caller function)
    // event was not triggered
    regTypeButtonGroup->setCheckedButton(individualRadioButton);
    // then call it manually
    OnRegisterTypeRadioButtonCheckedChanged(individualRadioButton);
    // end tip

    registerButton->setDisabled(false);

    return root;
}

Wt::WWidget *Home::GetMyProfileForm()
{
    Div *root = new Div("MyProfileForm");


    Div *dvForm = new Div(root);
    dvForm->setStyleClass("form");
    WGridLayout *dvFormLayout = new WGridLayout();

    if (m_isIndividualUser) {
        m_myProfileIndividualFirstNameLineEdit = new WLineEdit();
        m_myProfileIndividualLastNameLineEdit = new WLineEdit();
        m_myProfileIndividualContactNumberLineEdit = new WLineEdit();
        m_myProfileIndividualAlternativeContactNumberLineEdit = new WLineEdit();
        m_myProfileIndividualTeammatesTextArea = new WTextArea();
        m_myProfileIndividualNationalCodeLineEdit = new WLineEdit();
        m_myProfileIndividualEducationLevelComboBox = new WComboBox();
        m_myProfileIndividualEducationSubjectLineEdit = new WLineEdit();
        m_myProfileIndividualWebsiteLineEdit = new WLineEdit();

        m_myProfileIndividualEducationLevelComboBox->addItem(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_NONE_TEXT"));
        m_myProfileIndividualEducationLevelComboBox->addItem(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_DIPLOMA_TEXT"));
        m_myProfileIndividualEducationLevelComboBox->addItem(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_AD_TEXT"));
        m_myProfileIndividualEducationLevelComboBox->addItem(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_BA_TEXT"));
        m_myProfileIndividualEducationLevelComboBox->addItem(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_MA_TEXT"));
        m_myProfileIndividualEducationLevelComboBox->addItem(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_PHD_TEXT"));


        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_FIRST_NAME_TEXT")), 0, 0,
                                AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_myProfileIndividualFirstNameLineEdit, 0, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_LAST_NAME_TEXT")), 1, 0,
                                AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_myProfileIndividualLastNameLineEdit, 1, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_CONTACT_NUMBER")), 2, 0,
                                AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_myProfileIndividualContactNumberLineEdit, 2, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_ALTERNATIVE_CONTACT_NUMBER")), 3, 0,
                                AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_myProfileIndividualAlternativeContactNumberLineEdit, 3, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_TEAMMATES_TEXT")), 4, 0,
                                AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_myProfileIndividualTeammatesTextArea, 4, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_NATIONAL_CODE_TEXT")), 5, 0,
                                AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_myProfileIndividualNationalCodeLineEdit, 5, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_TEXT")), 6, 0,
                                AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_myProfileIndividualEducationLevelComboBox, 6, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_SUBJECT_TEXT")), 7, 0,
                                AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_myProfileIndividualEducationSubjectLineEdit, 7, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_INDIVIDUAL_WEBSITE")), 8, 0,
                                AlignLeft | AlignMiddle);
        dvFormLayout->addWidget(m_myProfileIndividualWebsiteLineEdit, 8, 1);


        string firstname;
        string lastname;
        string phone;
        string teammates;
        string altphone;
        string nationalcode;
        int educationlevel;
        string educationsubject;
        string website;

        result r = m_db->Sql() << "SELECT firstname, lastname, phone, teammates FROM ["
                                  + m_dbTables->Table("USERS_INDIVIDUAL")
                                  + "] WHERE email=?;" << m_cgiEnv->LoggedInUserName << row;
        if (!r.empty()) {
            r >> firstname >> lastname >> phone >> teammates;

           firstname = Crypto::Decrypt(firstname);
           lastname = Crypto::Decrypt(lastname);
           phone = Crypto::Decrypt(phone);
           teammates = Crypto::Decrypt(teammates);

           m_myProfileIndividualFirstNameLineEdit->setText(WString::fromUTF8(firstname));
           m_myProfileIndividualLastNameLineEdit->setText(WString::fromUTF8(lastname));
           m_myProfileIndividualContactNumberLineEdit->setText(WString::fromUTF8(phone));
           m_myProfileIndividualTeammatesTextArea->setText(WString::fromUTF8(teammates));
        }

        r = m_db->Sql() << "SELECT altphone, nationalcode, educationlevel, educationsubject, website FROM ["
                           + m_dbTables->Table("USERS_INDIVIDUAL_EXTRA")
                           + "] WHERE email=?;" << m_cgiEnv->LoggedInUserName << row;
        if (!r.empty()) {
            r >> altphone >> nationalcode >> educationlevel >> educationsubject >> website;

           altphone = Crypto::Decrypt(altphone);
           nationalcode = Crypto::Decrypt(nationalcode);
           educationsubject = Crypto::Decrypt(educationsubject);
           website = Crypto::Decrypt(website);

           m_myProfileIndividualAlternativeContactNumberLineEdit->setText(WString::fromUTF8(altphone));
           m_myProfileIndividualNationalCodeLineEdit->setText(WString::fromUTF8(nationalcode));
           m_myProfileIndividualEducationLevelComboBox->setCurrentIndex(educationlevel);
           m_myProfileIndividualEducationSubjectLineEdit->setText(WString::fromUTF8(educationsubject));
           m_myProfileIndividualWebsiteLineEdit->setText(WString::fromUTF8(website));
        }


        OnMyProfileEducationLevelComboBoxChanged(m_myProfileIndividualEducationLevelComboBox->currentIndex());
        m_myProfileIndividualEducationLevelComboBox->activated().connect(this,
                                                                         &Home::OnMyProfileEducationLevelComboBoxChanged);

        WLengthValidator *firstNameValidator = new WLengthValidator(Base::REGISTER_MIN_NAME_LEN, Base::REGISTER_MAX_NAME_LEN);
        WLengthValidator *lastNameValidator = new WLengthValidator(Base::REGISTER_MIN_NAME_LEN, Base::REGISTER_MAX_NAME_LEN);
        WRegExpValidator *conatactNumberValidator = new WRegExpValidator(Base::REGEX_PHONE_GENERIC);
        WRegExpValidator *alternativeConatactNumberValidator = new WRegExpValidator(Base::REGEX_PHONE_GENERIC);
        WRegExpValidator *nationalCodeValidator = new WRegExpValidator(Base::REGEX_NATIONAL_CODE);
        WLengthValidator *educationSubjectValidator = new WLengthValidator(1, 50);
        WRegExpValidator *websiteValidator = new WRegExpValidator(Base::REGEX_URI);

        websiteValidator->setFlags(MatchCaseInsensitive);

        firstNameValidator->setMandatory(true);
        lastNameValidator->setMandatory(true);
        conatactNumberValidator->setMandatory(true);
        alternativeConatactNumberValidator->setMandatory(false);
        nationalCodeValidator->setMandatory(true);
        educationSubjectValidator->setMandatory(true);
        websiteValidator->setMandatory(false);

        m_myProfileIndividualFirstNameLineEdit->setValidator(firstNameValidator);
        m_myProfileIndividualLastNameLineEdit->setValidator(lastNameValidator);
        m_myProfileIndividualContactNumberLineEdit->setValidator(conatactNumberValidator);
        m_myProfileIndividualAlternativeContactNumberLineEdit->setValidator(alternativeConatactNumberValidator);
        m_myProfileIndividualNationalCodeLineEdit->setValidator(nationalCodeValidator);
        m_myProfileIndividualEducationSubjectLineEdit->setValidator(educationSubjectValidator);
        m_myProfileIndividualWebsiteLineEdit->setValidator(websiteValidator);

        m_myProfileIndividualFirstNameLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileIndividualLastNameLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileIndividualContactNumberLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileIndividualAlternativeContactNumberLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileIndividualTeammatesTextArea->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileIndividualNationalCodeLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileIndividualEducationLevelComboBox->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileIndividualEducationSubjectLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileIndividualWebsiteLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
    } else {
        m_myProfileCompanyNameLineEdit = new WLineEdit();
        m_myProfileCompanyRegisterationIdLineEdit = new WLineEdit();
        m_myProfileCompanyPhoneLineEdit = new WLineEdit();
        m_myProfileCompanyAlternativePhoneLineEdit = new WLineEdit();
        m_myProfileCompanyCEONameLineEdit = new WLineEdit();
        m_myProfileCompanyEconomicalCodeLineEdit = new WLineEdit();
        m_myProfileCompanyFoundingYearComboBox = new WComboBox();
        m_myProfileCompanyAddressLineEdit = new WLineEdit();
        m_myProfileCompanyWebsiteLineEdit = new WLineEdit();
        m_myProfileCompanyResumeFileUpload = new WFileUpload();
        m_myProfileCompanyResumeFileUploadHasFile = false;


        m_myProfileCompanyResumeFileUpload->setMultiple(false);

        m_myProfileCompanyFoundingYearComboBox->addItem("----");
        size_t baseYear = lexical_cast<size_t>(CDate::DateConv::ToJalali().substr(0, 4)) + 1;
        size_t capYear = 1300;
        for (size_t i = baseYear; i > capYear; --i) {
            m_myProfileCompanyFoundingYearComboBox->addItem(lexical_cast<wstring>(i));
        }
        m_myProfileCompanyFoundingYearComboBox->setCurrentIndex(0);


        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_COMPANY_NAME_TEXT")), 0, 0,
                                AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_myProfileCompanyNameLineEdit, 0, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_COMPANY_REGISTERATION_ID_TEXT")), 1, 0,
                                AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_myProfileCompanyRegisterationIdLineEdit, 1, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_COMPANY_PHONE_TEXT")), 2, 0,
                                AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_myProfileCompanyPhoneLineEdit, 2, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_COMPANY_ALTERNATIVE_PHONE_TEXT")), 3, 0,
                                AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_myProfileCompanyAlternativePhoneLineEdit, 3, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_COMPANY_CEO_NAME_TEXT")), 4, 0,
                                AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_myProfileCompanyCEONameLineEdit, 4, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_COMPANY_ECONOMICAL_CODE_TEXT")), 5, 0,
                                AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_myProfileCompanyEconomicalCodeLineEdit, 5, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_COMPANY_FOUNDING_YEAR_TEXT")), 6, 0,
                                AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_myProfileCompanyFoundingYearComboBox, 6, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_COMPANY_ADDRESS_TEXT")), 7, 0,
                                AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_myProfileCompanyAddressLineEdit, 7, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_COMPANY_WEBSITE_TEXT")), 8, 0,
                                AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_myProfileCompanyWebsiteLineEdit, 8, 1);

        dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_COMPANY_RESUME_TEXT")), 9, 0,
                                AlignLeft | AlignTop);
        dvFormLayout->addWidget(m_myProfileCompanyResumeFileUpload, 9, 1);

        string name;
        string regid;
        string phone;
        string ceo;
        string altphone;
        string economicalcode;
        string foundingyear;
        string addr;
        string website;

        result r = m_db->Sql() << "SELECT name, regid, phone, ceo FROM ["
                                  + m_dbTables->Table("USERS_COMPANY")
                                  + "] WHERE email=?;" << m_cgiEnv->LoggedInUserName << row;
        if (!r.empty()) {
            r >> name >> regid >> phone >> ceo;

           name = Crypto::Decrypt(name);
           regid = Crypto::Decrypt(regid);
           phone = Crypto::Decrypt(phone);
           ceo = Crypto::Decrypt(ceo);

           m_myProfileCompanyNameLineEdit->setText(WString::fromUTF8(name));
           m_myProfileCompanyRegisterationIdLineEdit->setText(WString::fromUTF8(regid));
           m_myProfileCompanyPhoneLineEdit->setText(WString::fromUTF8(phone));
           m_myProfileCompanyCEONameLineEdit->setText(WString::fromUTF8(ceo));
        }

        r = m_db->Sql() << "SELECT altphone, economicalcode, foundingyear, addr, website FROM ["
                           + m_dbTables->Table("USERS_COMPANY_EXTRA")
                           + "] WHERE email=?;" << m_cgiEnv->LoggedInUserName << row;
        if (!r.empty()) {
            r >> altphone >> economicalcode >> foundingyear >> addr >> website;

           altphone = Crypto::Decrypt(altphone);
           economicalcode = Crypto::Decrypt(economicalcode);
           foundingyear = Crypto::Decrypt(foundingyear);
           addr = Crypto::Decrypt(addr);
           website = Crypto::Decrypt(website);

           m_myProfileCompanyAlternativePhoneLineEdit->setText(WString::fromUTF8(altphone));
           m_myProfileCompanyEconomicalCodeLineEdit->setText(WString::fromUTF8(economicalcode));
           /*m_myProfileCompanyFoundingYearComboBox->setCurrentIndex(m_myProfileCompanyFoundingYearComboBox->count()
                                                                   - (lexical_cast<size_t>(foundingyear) - capYear));*/
           m_myProfileCompanyFoundingYearComboBox->setCurrentIndex(m_myProfileCompanyFoundingYearComboBox->findText(foundingyear));
           m_myProfileCompanyAddressLineEdit->setText(WString::fromUTF8(addr));
           m_myProfileCompanyWebsiteLineEdit->setText(WString::fromUTF8(website));
        }


        OnMyProfileFoundingYearComboBoxChanged(m_myProfileCompanyFoundingYearComboBox->currentIndex());
        m_myProfileCompanyFoundingYearComboBox->activated().connect(this,
                                                             &Home::OnMyProfileFoundingYearComboBoxChanged);


        WLengthValidator *nameValidator = new WLengthValidator(1, 100);
        WLengthValidator *registerationIdValidator = new WLengthValidator(1, 10);
        WRegExpValidator *phoneValidator = new WRegExpValidator(Base::REGEX_PHONE_GENERIC);
        WRegExpValidator *alternativePhoneValidator = new WRegExpValidator(Base::REGEX_PHONE_GENERIC);
        WLengthValidator *ceoNameValidator = new WLengthValidator(Base::REGISTER_MIN_NAME_LEN, Base::REGISTER_MAX_NAME_LEN);
        WLengthValidator *economicalCodeValidator = new WLengthValidator(1, 15);
        WLengthValidator *addressCodeValidator = new WLengthValidator(1, 255);
        WRegExpValidator *websiteValidator = new WRegExpValidator(Base::REGEX_URI);

        websiteValidator->setFlags(MatchCaseInsensitive);

        nameValidator->setMandatory(true);
        registerationIdValidator->setMandatory(true);
        phoneValidator->setMandatory(true);
        alternativePhoneValidator->setMandatory(false);
        ceoNameValidator->setMandatory(true);
        economicalCodeValidator->setMandatory(true);
        addressCodeValidator->setMandatory(true);
        websiteValidator->setMandatory(false);

        m_myProfileCompanyNameLineEdit->setValidator(nameValidator);
        m_myProfileCompanyRegisterationIdLineEdit->setValidator(registerationIdValidator);
        m_myProfileCompanyPhoneLineEdit->setValidator(phoneValidator);
        m_myProfileCompanyAlternativePhoneLineEdit->setValidator(alternativePhoneValidator);
        m_myProfileCompanyCEONameLineEdit->setValidator(ceoNameValidator);
        m_myProfileCompanyEconomicalCodeLineEdit->setValidator(economicalCodeValidator);
        m_myProfileCompanyAddressLineEdit->setValidator(addressCodeValidator);
        m_myProfileCompanyWebsiteLineEdit->setValidator(websiteValidator);

        m_myProfileCompanyNameLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileCompanyRegisterationIdLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileCompanyPhoneLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileCompanyAlternativePhoneLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileCompanyCEONameLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileCompanyEconomicalCodeLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileCompanyFoundingYearComboBox->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileCompanyAddressLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);
        m_myProfileCompanyWebsiteLineEdit->enterPressed().connect(this, &Home::OnMyProfileOK);

        m_myProfileCompanyResumeFileUpload->changed().connect(this, &Home::OnMyProfileResumeFileChanged);
        m_myProfileCompanyResumeFileUpload->fileTooLarge().connect(this, &Home::OnMyProfileResumeFileTooLarge);
        m_myProfileCompanyResumeFileUpload->uploaded().connect(this, &Home::OnMyProfileResumeFileUploaded);

        Div *dvMyProfileFormHint = new Div(root, "dvMyProfileFormHint");
        new WText(m_lang->GetString("HOME_MY_PROFILE_DLG_COMPANY_RESUME_HINE"), dvMyProfileFormHint);

        m_myProfileCompanyResumeFileUpload->setStyleClass("fileupload");
    }

    dvFormLayout->setColumnStretch(0, 135);
    dvFormLayout->setColumnStretch(1, 230);
    dvFormLayout->setVerticalSpacing(11);
    dvForm->resize(365, WLength::Auto);
    dvForm->setLayout(dvFormLayout);


    Div *dvButtons = new Div(root, "dvDialogButtons");
    m_myProfileSaveButton = new WPushButton(m_lang->GetString("HOME_MY_PROFILE_DLG_SAVE_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("HOME_MY_PROFILE_DLG_RETURN_BUTTON"), dvButtons);

    m_myProfileSaveButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    m_myProfileSaveButton->clicked().connect(this, &Home::OnMyProfileOK);
    returnButton->clicked().connect(this, &Home::OnMyProfileReturn);

    return root;
}

Wt::WWidget *Home::GetChangePwdForm()
{
    Div *root = new Div("ChangePwdForm");

    Div *dvForm = new Div(root);
    dvForm->setStyleClass("form");
    WGridLayout *dvFormLayout = new WGridLayout();

    m_changePwdCurrentLineEdit = new WLineEdit();
    m_changePwdNewLineEdit = new WLineEdit();
    m_changePwdConfirmLineEdit = new WLineEdit();

    m_changePwdCurrentLineEdit->setEchoMode(WLineEdit::Password);
    m_changePwdNewLineEdit->setEchoMode(WLineEdit::Password);
    m_changePwdConfirmLineEdit->setEchoMode(WLineEdit::Password);

    dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_CHANGE_PWD_DLG_CURRENT_PWD_TEXT")),
                                      0, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(m_changePwdCurrentLineEdit, 0, 1);

    dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_CHANGE_PWD_DLG_NEW_PWD_TEXT")),
                                      1, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(m_changePwdNewLineEdit, 1, 1);
    dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_CHANGE_PWD_DLG_CONFIRM_PWD_TEXT")),
                                      2, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(m_changePwdConfirmLineEdit, 2, 1);

    dvFormLayout->setColumnStretch(0, 100);
    dvFormLayout->setColumnStretch(1, 200);
    dvFormLayout->setVerticalSpacing(11);
    dvForm->resize(300, WLength::Auto);
    dvForm->setLayout(dvFormLayout);

    Div *dvButtons = new Div(root, "dvDialogButtons");
    WPushButton *changeButton = new WPushButton(m_lang->GetString("HOME_CHANGE_PWD_DLG_CHANGE_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("HOME_CHANGE_PWD_DLG_RETURN_BUTTON"), dvButtons);

    changeButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    WLengthValidator *pwdValidator = new WLengthValidator(Base::MIN_PASSWORD_LENGTH,
                                                          Base::MAX_PASSWORD_LENGTH);
    pwdValidator->setMandatory(true);

    m_changePwdCurrentLineEdit->setValidator(pwdValidator);
    m_changePwdNewLineEdit->setValidator(pwdValidator);
    m_changePwdConfirmLineEdit->setValidator(pwdValidator);

    m_changePwdCurrentLineEdit->enterPressed().connect(this, &Home::OnChangePwdOK);
    m_changePwdNewLineEdit->enterPressed().connect(this, &Home::OnChangePwdOK);
    m_changePwdConfirmLineEdit->enterPressed().connect(this, &Home::OnChangePwdOK);
    changeButton->clicked().connect(this, &Home::OnChangePwdOK);
    returnButton->clicked().connect(this, &Home::OnChangePwdReturn);

    return root;
}

void Home::ShowSubmitIdeaDialog()
{
    delete m_dlg;
    m_dlg = NULL;

    m_dlg = new WDialog(m_lang->GetString("HOME_SUBMIT_IDEA_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(768, Wt::WLength::Pixel),
                  Wt::WLength(64, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(false);

    new IdeaSubmit(m_cgiRoot, m_dlg);

    m_dlg->show();
}

void Home::ShowSubmitAppDialog()
{
    delete m_dlg;
    m_dlg = NULL;

    m_dlg = new WDialog(m_lang->GetString("HOME_SUBMIT_APP_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(768, Wt::WLength::Pixel),
                  Wt::WLength(64, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(false);

    new AppSubmit(m_cgiRoot, m_dlg);

    m_dlg->show();
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *Home::Layout()
{
    m_cgiRoot->setTitle(m_lang->GetString("HOME_TITLE"));

    Div *root = new Div("Home");

    Div *dvNoScript = new Div(root, "dvNoScript");
    new WText(m_lang->GetString("WARN_NO_SCRIPT"), dvNoScript);

    Div *dvHeader = new Div(root, "dvHeader");
    new Div(root, "dvHeaderShadow");
    Div *dvHeaderInner = new Div(dvHeader, "dvHeaderInner");

    Div *dvBody = new Div(root, "dvBody");
    Div *dvBodyInner = new Div(dvBody, "dvBodyInner");

    new Div(root, "dvFooterShadow");
    Div *dvFooter = new Div(root, "dvFooter");
    Div *dvFooterInner = new Div(dvFooter, "dvFooterInner");

    Div *dvLogoBar = new Div(dvHeaderInner, "dvLogoBar");
    Div *dvMenuBar = new Div(dvHeaderInner, "dvMenuBar");

    Div *dvUtilsBar = new Div(dvBodyInner, "dvUtilsBar");

    Div *dvDForumLogo = new Div(dvLogoBar, "dvDForumLogo");
    Div *dvOtherLogo = new Div(dvLogoBar, "dvOtherLogo");


    WLink homeLink(Wt::WLink::Url, ".");
    WLink logoImageLink(Wt::WLink::Url, "bg_headerlogo.jpg");
    new WAnchor(homeLink,
                new WImage(logoImageLink), dvDForumLogo);

    WLink logoUniversityImageLink(Wt::WLink::Url, "bg_headerlogo_university.png");
    new WImage(logoUniversityImageLink, dvOtherLogo);
    WLink logoSamsungImageLink(Wt::WLink::Url, "bg_headerlogo_samsung.png");
    new WImage(logoSamsungImageLink, dvOtherLogo);


    Div *dvLang = new Div(dvUtilsBar, "dvLang");
    Div *dvSearch = new Div(dvUtilsBar, "dvSearch");
    m_dvLogin = new Div(dvUtilsBar, "dvLogin");


    //Div *dvSitemap = new Div(dvFooterInner, "dvSitemap");
    Div *dvCopyright = new Div(dvFooterInner, "dvCopyright");

	WText *webgozar = new WText(dvCopyright);
	webgozar->setTextFormat(XHTMLUnsafeText);
    webgozar->setText(L"<iframe width=\"0\" height=\"0\" frameborder=\"0\" scrolling=\"no\" src=\"stat.html\"></iframe>");
	new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", dvCopyright);
    WLink footerLogoImageLink(Wt::WLink::Url, "bg_footerlogo.png");
    new WImage(footerLogoImageLink, dvCopyright);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", dvCopyright);
    new WText(Base::Copyright(m_cgiEnv->CurrentLang), dvCopyright);


    new WText(L"<span style=\"font-weight: bold; color: #203070;\">"
              "<a href=\"?lang=en\" style=\"font-weight: bold; color: #203070; text-decoration: none;\">EN</a>"
              "&nbsp;&nbsp;|&nbsp;&nbsp;"
              "<a href=\"?lang=fa\" style=\"font-weight: bold; color: #203070; text-decoration: none;\">فارسی</a></span>", dvLang);

    m_searchLineEdit = new WLineEdit(dvSearch);
    m_searchLineEdit->resize(128, WLength::Auto);
    m_searchLineEdit->setEmptyText(m_lang->GetString("HOME_SERARCH_BOX_EMPTY_TEXT"));
    m_searchLineEdit->enterPressed().connect(this, &Home::OnSearchEditEnterPressed);

    WText *signInLink = new WText(m_lang->GetString("HOME_SIGNIN_LINK_TEXT"), m_dvLogin);
    signInLink->setStyleClass("link");
    signInLink->clicked().connect(this, &Home::OnSignInLinkPressed);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", m_dvLogin);
    WText *registerLink = new WText(m_lang->GetString("HOME_REGISTER_LINK_TEXT"), m_dvLogin);
    registerLink->setStyleClass("link");
    registerLink->clicked().connect(this, &Home::OnRegisterLinkPressed);

    Div *dvSmoothMenu = new Div(dvMenuBar, "dvSmoothMenu");
    dvSmoothMenu->setStyleClass("ddsmoothmenu");
    Div *dvContents = new Div(dvBodyInner, "dvContents");

    WStackedWidget *dvMainContents = new WStackedWidget(dvContents);
    dvMainContents->setId("dvMainContents");

    WMenu *mainMenu = new WMenu(dvMainContents, Horizontal, dvSmoothMenu);
    mainMenu->setRenderAsList(true);
    mainMenu->setStyleClass("menu");

    mainMenu->addItem
            (m_lang->GetString("HOME_MENU_HOME"), CHome(), WMenuItem::PreLoading)->setPathComponent("");

    GetSubMenues(mainMenu, Base::DB_PAGE_FULLPATH_HOME, dvMainContents);

    WMenuItem *contactMenuItem = new WMenuItem(m_lang->GetString("HOME_MENU_CONTACT"), CContact(), WMenuItem::PreLoading);
    mainMenu->addItem(contactMenuItem)->setPathComponent("");

    mainMenu->addItem
            (m_lang->GetString("HOME_MENU_FORUMS"), new WText(L""), WMenuItem::PreLoading)->setPathComponent("");

    mainMenu->itemSelected().connect(this, &Home::OnMenuItemSelected);

    if (m_initialSelectedMenuItem == NULL) {
        if (m_cgiEnv->GetInitialURL() == Base::URL_CONTACTUS) {
            contactMenuItem->select();
            m_initialSelectedMenuItem = contactMenuItem;
        } else {
            if (algorithm::trim_copy(m_cgiEnv->GetInitialURL()) != "") {
                Exit(".");
            }
        }
    }

    if (algorithm::trim_copy(m_cgiEnv->GetNewsId()) != "") {
        string newsId(Crypto::Decrypt(m_cgiEnv->GetNewsId()));
        if (newsId != "") {
            ////////// NOTICE
            /// THE CODE INSIDE THIS IF BLOCK MUST BE EXACTLY LOOK LIKE
            /// News::OnNewsLinkClicked function !!

            string title;
            string body;
            string date;

            result r = m_db->Sql() << "SELECT title, body, date FROM ["
                                      + m_dbTables->Table("NEWS")
                                      + "] WHERE rowid=?;" << newsId << row;
            if (!r.empty()) {
                r >> title >> body >> date;

                title = Crypto::Decrypt(title);
                body = Crypto::Decrypt(body);
                date = Crypto::Decrypt(date);

                boost::algorithm::replace_all(body, "\n", "<br />");
            }

            WVBoxLayout *layout = new WVBoxLayout();
            Div *dvNewsComplete = new Div("dvNewsComplete");
            dvNewsComplete->setOverflow(WContainerWidget::OverflowAuto);

            string lang;
            switch(m_cgiEnv->CurrentLang) {
            case CgiEnv::ELang_En:
                lang = "en";
                break;

            case CgiEnv::ELang_Fa:
                lang = "fa";
                break;

            default:
                break;
            }

            WText *titleLink = new WText(dvNewsComplete);
            titleLink->setTextFormat(XHTMLUnsafeText);
            titleLink->setText(WString::fromUTF8("<h5>" + date + "&nbsp;&nbsp;&nbsp;"
                                                 "<a href=\"?lang="
                                                 + lang
                                                 + "&news="
                                                 + Crypto::Encrypt(newsId)
                                                 + "\">"+ title + "</a></h5>"));
            new WText(WString::fromUTF8(body), dvNewsComplete);

            layout->addWidget(dvNewsComplete);
            layout->setStretchFactor(dvNewsComplete, 0);

            m_dlg = new WDialog(WString::fromUTF8(title));
            m_dlg->setModal(true);
            m_dlg->resize(Wt::WLength(768, Wt::WLength::Pixel),
                          Wt::WLength(64, Wt::WLength::Percentage));
            m_dlg->rejectWhenEscapePressed();
            m_dlg->setClosable(true);
            m_dlg->contents()->setLayout(layout);
            m_dlg->show();
        }
    }

/*
    ///////////// TEST
    m_dvLogin->clear();

    string ei("abc@gmail.com");
    string ei2("m3drug@gmail.com");
    string ei3("AMIRIMAG@YAHOO.COM");
    string ei4("mortezaqofrani@gmail.com");
    string ei5("sanaz.shahmohamadi@yahoo.com");
    string ei6("info@silvapc.com");
    string ec("qwe@gmail.com");
    string e = ei4;

    m_cgiEnv->LoggedInUserName = e;

    WText *myAppsLink = new WText(m_lang->GetString("HOME_MY_APPS_LINK_TEXT"), m_dvLogin);
    myAppsLink->setStyleClass("link");
    myAppsLink->clicked().connect(this, &Home::OnMyAppsLinkPressed);

    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", m_dvLogin);

    WText *myIdeasLink = new WText(m_lang->GetString("HOME_MY_IDEAS_LINK_TEXT"), m_dvLogin);
    myIdeasLink->setStyleClass("link");
    myIdeasLink->clicked().connect(this, &Home::OnMyIdeasLinkPressed);

    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", m_dvLogin);

    if (m_cgiEnv->CurrentLang == CgiEnv::ELang_Fa) {
        WText *appointmentLink = new WText(m_lang->GetString("HOME_APPOINTMENT_LINK_TEXT"), m_dvLogin);
        appointmentLink->setStyleClass("link");
        appointmentLink->clicked().connect(this, &Home::OnAppointmentLinkPressed);

        new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", m_dvLogin);
    }

    WText *myProfileLink = new WText(e, m_dvLogin);
    myProfileLink->setStyleClass("link");
    myProfileLink->clicked().connect(this, &Home::OnMyProfileLinkPressed);

    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", m_dvLogin);

    WText *pwdLink = new WText(m_lang->GetString("HOME_PWD_LINK_TEXT"), m_dvLogin);
    pwdLink->setStyleClass("link");
    pwdLink->clicked().connect(this, &Home::OnPwdLinkPressed);

    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", m_dvLogin);

    signInLink = new WText(m_lang->GetString("HOME_SIGNOUT_LINK_TEXT"), m_dvLogin);
    signInLink->setStyleClass("link");
    signInLink->clicked().connect(this, &Home::OnSignOutLinkPressed);

    m_isUserLoggedIn = true;
    if (e != ec)
        m_isIndividualUser = true;
    else
        m_isIndividualUser = false;

    ///////////////////
*/

	return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


