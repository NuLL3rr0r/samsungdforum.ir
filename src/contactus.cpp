/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WComboBox>
#include <Wt/WGridLayout>
#include <Wt/WPushButton>
#include <Wt/WImage>
#include <Wt/WIntValidator>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WRegExpValidator>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WWidget>
#include "contactus.hpp"
#include "base.hpp"
#include "cdate.hpp"
#include "crypto.hpp"
#include "db.hpp"
#include "dbtables.hpp"
#include "mailer.hpp"


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

ContactUs::ContactUs(CgiRoot *cgi) : BaseWidget(cgi)
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

void ContactUs::OnToComboBoxSelectionChanged(Wt::WString to)
{
    if (to != m_lang->GetString("HOME_CONTACTUS_RECIPIENT_CMB_TITLE"))
        SetFormStatus(true);
}

void ContactUs::OnSendButtonPressed()
{
    if (Validate(m_captchaLineEdit)) {
        if(!Validate(m_fromLineEdit) || !Validate(m_emailLineEdit) || !Validate(m_urlLineEdit)
                || !Validate(m_subjectLineEdit) || !Validate(m_bodyTextArea)) {
            GenCap();
            return;
        }
    } else {
        GenCap();
        return;
    }

    result r = m_db->Sql() << "SELECT addr FROM ["
                              + m_dbTables->Table("CONTACTS")
                              + "] WHERE recipient=?;" << algorithm::trim_copy(m_toComboBox->currentText().toUTF8()) << row;
    if (!r.empty()) {
        CDate::Now n;

        string to;
        r >> to;
        to = Crypto::Decrypt(to);

        Mailer::SendUserMessage(to,
                                algorithm::trim_copy(m_emailLineEdit->text().toUTF8()),
                                algorithm::trim_copy(m_fromLineEdit->text().toUTF8()),
                                algorithm::trim_copy(m_urlLineEdit->text().toUTF8()),
                                algorithm::trim_copy(m_subjectLineEdit->text().toUTF8()),
                                algorithm::trim_copy(boost::algorithm::replace_all_copy(m_bodyTextArea->text().toUTF8(), "\n", "<br />")),
                                n, m_cgiEnv.get());
    }

    SetFormStatus(false);
    GenCap();
    HtmlInfo(L"<br/><center>" + m_lang->GetString("HOME_CONTACTUS_SEND_SUCCESS_MSG") + L"</center>", m_errContactForm);
}

void ContactUs::OnClearButtonPressed()
{
    HtmlInfo(L"", m_errContactForm);
    SetFormStatus(false);
    GenCap();
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

void ContactUs::GenCap()
{
    m_captchaImage->setImageRef(m_captcha->GenCap()->imageRef());
    m_captchaValidator->setRange(m_captcha->Result, m_captcha->Result);
}

void ContactUs::SetFormStatus(bool flag)
{
    m_toComboBox->setDisabled(flag);
    m_fromLineEdit->setDisabled(!flag);
    m_emailLineEdit->setDisabled(!flag);
    m_urlLineEdit->setDisabled(!flag);
    m_subjectLineEdit->setDisabled(!flag);
    m_bodyTextArea->setDisabled(!flag);
    m_captchaLineEdit->setDisabled(!flag);
    m_sendButton->setDisabled(!flag);
    m_clearButton->setDisabled(!flag);

    if (flag) {
        HtmlInfo(L"", m_errContactForm);
        m_fromLineEdit->setFocus();
    }
    else {
        m_fromLineEdit->setText("");
        m_emailLineEdit->setText("");
        m_urlLineEdit->setText("");
        m_subjectLineEdit->setText("");
        m_bodyTextArea->setText("");
        m_captchaLineEdit->setText("");
        m_toComboBox->setCurrentIndex(0);
        m_toComboBox->setFocus();
    }
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *ContactUs::Layout()
{
    Div *root = new Div("ContactUs");


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

    new WAnchor((format("?lang=%1%&url=%2%") % lang % Base::URL_CONTACTUS).str(),
                m_lang->GetString("HOME_MENU_CONTACT"),
                root);
    new WBreak(root);
    new WBreak(root);


    m_errContactForm = new WText(root);

    Div *dvContactFormWrapper = new Div(root, "dvContactFormWrapper");
    new WText(m_lang->GetString("HOME_CONTACTUS_TITLE_MESSAGE"), dvContactFormWrapper);

    Div *dvContactForm = new Div(dvContactFormWrapper, "dvContactForm", "form");
    WGridLayout *dvContactFormLayout = new WGridLayout();

    m_toComboBox = new WComboBox();
    m_toComboBox->addItem(m_lang->GetString("HOME_CONTACTUS_RECIPIENT_CMB_TITLE"));

    result r = m_db->Sql() << "SELECT recipient FROM ["
                              + m_dbTables->Table("CONTACTS")
                              + "] ORDER BY recipient ASC;";
    while(r.next()) {
        string recipient;
        r >> recipient;
        m_toComboBox->addItem(WString::fromUTF8(recipient));
    }

    m_fromLineEdit = new WLineEdit();
    m_emailLineEdit = new WLineEdit();
    m_urlLineEdit = new WLineEdit();
    m_subjectLineEdit = new WLineEdit();
    m_bodyTextArea = new WTextArea();
    m_bodyTextArea->resize(WLength::Auto, 100);
    m_captchaLineEdit = new WLineEdit();

    dvContactFormLayout->addWidget(new Wt::WText(m_lang->GetString("HOME_CONTACTUS_RECIPIENT_TEXT")), 0, 0, AlignLeft | AlignMiddle);
    dvContactFormLayout->addWidget(m_toComboBox, 0, 1);

    dvContactFormLayout->addWidget(new WText(m_lang->GetString("HOME_CONTACTUS_FROM_NAME_TEXT")), 1, 0, AlignLeft | AlignMiddle);
    dvContactFormLayout->addWidget(m_fromLineEdit, 1, 1);

    dvContactFormLayout->addWidget(new WText(m_lang->GetString("HOME_CONTACTUS_EMAIL_TEXT")), 2, 0, AlignLeft | AlignMiddle);
    dvContactFormLayout->addWidget(m_emailLineEdit, 2, 1);

    dvContactFormLayout->addWidget(new WText(m_lang->GetString("HOME_CONTACTUS_URL_TEXT")), 3, 0, AlignLeft | AlignMiddle);
    dvContactFormLayout->addWidget(m_urlLineEdit, 3, 1);

    dvContactFormLayout->addWidget(new WText(m_lang->GetString("HOME_CONTACTUS_SUBJECT_TEXT")), 4, 0, AlignLeft | AlignMiddle);
    dvContactFormLayout->addWidget(m_subjectLineEdit, 4, 1);

    dvContactFormLayout->addWidget(new WText(m_lang->GetString("HOME_CONTACTUS_BODY_TEXT")), 5, 0, AlignLeft | AlignTop);
    dvContactFormLayout->addWidget(m_bodyTextArea, 5, 1);

    dvContactFormLayout->addWidget(new WText(m_lang->GetString("HOME_CONTACTUS_CAPTCHA_TEXT")), 6, 0, AlignLeft | AlignMiddle);
    dvContactFormLayout->addWidget(m_captchaLineEdit, 6, 1);

    dvContactFormLayout->setVerticalSpacing(11);
    dvContactFormLayout->setColumnStretch(0, 125);
    dvContactFormLayout->setColumnStretch(1, 275);
    dvContactForm->resize(300, WLength::Auto);
    dvContactForm->setLayout(dvContactFormLayout);

    m_captchaImage = m_captcha->GenCap();
    dvContactFormWrapper->addWidget(m_captchaImage);

    m_sendButton = new WPushButton(m_lang->GetString("HOME_CONTACTUS_SEND_BUTTON"), dvContactFormWrapper);
    m_sendButton->setStyleClass("formButton");
    m_clearButton = new WPushButton(m_lang->GetString("HOME_CONTACTUS_CLEAR_BUTTON"), dvContactFormWrapper);
    m_clearButton->setStyleClass("formButton");

    SetFormStatus(false);

    WLengthValidator *fromValidator = new WLengthValidator(Base::CONTACTUS_MIN_FROM_NAME_LEN, Base::CONTACTUS_MAX_FROM_NAME_LEN);
    WRegExpValidator *emailValidator = new WRegExpValidator(Base::REGEX_EMAIL);
    WRegExpValidator *urlValidator = new WRegExpValidator(Base::REGEX_URI);
    WLengthValidator *subjectValidator = new WLengthValidator(Base::CONTACTUS_MIN_SUBJECT_LEN, Base::CONTACTUS_MAX_SUBJECT_LEN);
    WLengthValidator *bodyValidator = new WLengthValidator(Base::CONTACTUS_MIN_BODY_LEN, Base::CONTACTUS_MAX_BODY_LEN);
    m_captchaValidator = new WIntValidator(m_captcha->Result, m_captcha->Result);

    emailValidator->setFlags(MatchCaseInsensitive);
    urlValidator->setFlags(MatchCaseInsensitive);

    fromValidator->setMandatory(true);
    emailValidator->setMandatory(true);
    subjectValidator->setMandatory(true);
    bodyValidator->setMandatory(true);
    m_captchaValidator->setMandatory(true);

    m_fromLineEdit->setValidator(fromValidator);
    m_emailLineEdit->setValidator(emailValidator);
    m_urlLineEdit->setValidator(urlValidator);
    m_subjectLineEdit->setValidator(subjectValidator);
    m_bodyTextArea->setValidator(bodyValidator);
    m_captchaLineEdit->setValidator(m_captchaValidator);

    m_captchaLineEdit->setEmptyText(m_lang->GetString("CAPTCHA_HINT"));

    m_toComboBox->sactivated().connect(this, &ContactUs::OnToComboBoxSelectionChanged);

    m_fromLineEdit->enterPressed().connect(this, &ContactUs::OnSendButtonPressed);
    m_emailLineEdit->enterPressed().connect(this, &ContactUs::OnSendButtonPressed);
    m_urlLineEdit->enterPressed().connect(this, &ContactUs::OnSendButtonPressed);
    m_subjectLineEdit->enterPressed().connect(this, &ContactUs::OnSendButtonPressed);
    m_captchaLineEdit->enterPressed().connect(this, &ContactUs::OnSendButtonPressed);
    m_sendButton->clicked().connect(this, &ContactUs::OnSendButtonPressed);
    m_clearButton->clicked().connect(this, &ContactUs::OnClearButtonPressed);

    root->addWidget(new WBreak());

    /*string lang;
    switch(m_cgiEnv->CurrentLang) {
    case CgiEnv::ELang_En:
        lang = "en";
        break;

    case CgiEnv::ELang_Fa:
        lang = "fa";
        break;

    default:
        break;
    }*/

    r = m_db->Sql() << "SELECT info FROM ["
                       + m_dbTables->Table("CONTACTS_EXTRA_INFO")
                       + "] WHERE lang=?;" << lang << row;
    string info;
    if (!r.empty()) {
        r >> info;
    }

    root->addWidget(new WText(WString::fromUTF8(Crypto::Decrypt(info))));

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


