/**********************************
*    = Header File Inclusion =
**********************************/

#include <thread>
#include <unordered_map>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WFileResource>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WMessageBox>
#include <Wt/WPushButton>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WTextEdit>
#include <Wt/WVBoxLayout>
#include <Wt/WWidget>
#include "cmsusers.hpp"
#include "base.hpp"
#include "cdate.hpp"
#include "crypto.hpp"
#include "db.hpp"
#include "dbtables.hpp"
#include "lang.hpp"
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

CmsUsers::CmsUsers(CgiRoot *cgi) : BaseWidget(cgi)
{
    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_NONE_TEXT"));
    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_DIPLOMA_TEXT"));
    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_AD_TEXT"));
    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_BA_TEXT"));
    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_MA_TEXT"));
    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_PHD_TEXT"));

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

void CmsUsers::OnShowIndividualsTableButtonPressed()
{
    WVBoxLayout *layout = new WVBoxLayout();
    WContainerWidget* dvTable = new WContainerWidget();
    dvTable->setOverflow(WContainerWidget::OverflowAuto);
    dvTable->addWidget(GetIndividualsTable());

    layout->addWidget(dvTable);
    layout->setStretchFactor(dvTable, 0);

    m_dlg = new WDialog(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUALS_TABLE_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(85, Wt::WLength::Percentage),
                  Wt::WLength(85, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);
    m_dlg->contents()->setLayout(layout);
    m_dlg->show();
}

void CmsUsers::OnShowCompaniesTableButtonPressed()
{
    WVBoxLayout *layout = new WVBoxLayout();
    WContainerWidget* dvTable = new WContainerWidget();
    dvTable->setOverflow(WContainerWidget::OverflowAuto);
    dvTable->addWidget(GetCompaniesTable());

    layout->addWidget(dvTable);
    layout->setStretchFactor(dvTable, 0);

    m_dlg = new WDialog(m_lang->GetString("ROOT_CMSUSERS_COMPANIES_TABLE_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(85, Wt::WLength::Percentage),
                  Wt::WLength(85, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);
    m_dlg->contents()->setLayout(layout);
    m_dlg->show();
}

void CmsUsers::OnSendMessageButtonPressed()
{
    if (!Validate(m_messageSubjectLineEdit)/*
            || !Validate(m_messageBodyTextEdit)*/) {
        return;
    }

    m_msg = new WMessageBox(m_lang->GetString("ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_TITLE"),
                            m_lang->GetString("ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_BODY"),
                            Warning, NoButton);
    m_msg->addButton(m_lang->GetString("ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_YES_BUTTON"), Yes);
    m_msg->addButton(m_lang->GetString("ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_No_BUTTON"), No);
    m_msg->buttonClicked().connect(this, &CmsUsers::OnSendMessageConfirmDialogClosed);
    m_msg->show();
}

void CmsUsers::OnSendMessageConfirmDialogClosed(Wt::StandardButton result)
{
    delete m_msg;
    m_msg = NULL;

    switch(result) {
    case Yes:
        {
            CDate::Now n;

            unordered_map<string, string> users;

            cppdb::result r = m_db->Sql() << "SELECT rowid, email, firstname, lastname FROM ["
                                      + m_dbTables->Table("USERS_INDIVIDUAL")
                                      + "] ORDER BY rowid ASC;";
            while(r.next()) {
                int rowId;
                string email;
                string firstName;
                string lastName;

                r >> rowId >> email >> firstName >> lastName;

                firstName = Crypto::Decrypt(firstName);
                lastName = Crypto::Decrypt(lastName);

                users[email] = firstName + " " + lastName;
            }

            r = m_db->Sql() << "SELECT rowid, email, name FROM ["
                               + m_dbTables->Table("USERS_COMPANY")
                               + "] ORDER BY rowid ASC;";
            while(r.next()) {
                int rowId;
                string email;
                string name;

                r >> rowId >> email >> name;

                name = Crypto::Decrypt(name);

                users[email] = name;
            }

            thread t(Mailer::SendAMessageToUsers, users,
                     m_messageSubjectLineEdit->text().toUTF8(), m_messageBodyTextEdit->text().toUTF8(),
                     n, m_cgiEnv.get());
            t.detach();


            m_msg = new WMessageBox(m_lang->GetString("ROOT_CMSUSERS_SEND_MESSAGE_SENT_DLG_TITLE"),
                                    m_lang->GetString("ROOT_CMSUSERS_SEND_MESSAGE_SENT_DLG_BODY"),
                                    Warning, NoButton);
            m_msg->addButton(m_lang->GetString("ROOT_CMSUSERS_SEND_MESSAGE_SENT_DLG_OK_BUTTON"), Ok);
            m_msg->buttonClicked().connect(this, &CmsUsers::OnSendMessageSuccessDialogClosed);
            m_msg->show();
        }
        break;

    case No:
        break;

    default:
        break;
    }
}

void CmsUsers::OnSendMessageSuccessDialogClosed(Wt::StandardButton result)
{
    delete m_msg;
    m_msg = NULL;
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

Wt::WWidget *CmsUsers::GetIndividualsTable()
{
    Div *root = new Div("IndividualsTable");

    WTable *table = new WTable(root);
    table->setStyleClass("tbl");

    table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_NO_TEXT")));
    table->elementAt(0, 1)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_FIRST_NAME_TEXT")));
    table->elementAt(0, 2)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_LAST_NAME_TEXT")));
    table->elementAt(0, 3)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_EMAIL_TEXT")));
    table->elementAt(0, 4)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_CONTACT_NUMBER_TEXT")));
    table->elementAt(0, 5)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_ALTERNATIVE_CONTACT_NUMBER")));
    table->elementAt(0, 6)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_TEAMMATES_TEXT")));
    table->elementAt(0, 7)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_NATIONAL_CODE_TEXT")));
    table->elementAt(0, 8)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_TEXT")));
    table->elementAt(0, 9)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_SUBJECT_TEXT")));
    table->elementAt(0, 10)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_WEBSITE_TEXT")));
    table->elementAt(0, 0)->setStyleClass("tblHeader");
    table->elementAt(0, 1)->setStyleClass("tblHeader");
    table->elementAt(0, 2)->setStyleClass("tblHeader");
    table->elementAt(0, 3)->setStyleClass("tblHeader");
    table->elementAt(0, 4)->setStyleClass("tblHeader");
    table->elementAt(0, 5)->setStyleClass("tblHeader");
    table->elementAt(0, 6)->setStyleClass("tblHeader");
    table->elementAt(0, 7)->setStyleClass("tblHeader");
    table->elementAt(0, 8)->setStyleClass("tblHeader");
    table->elementAt(0, 9)->setStyleClass("tblHeader");
    table->elementAt(0, 10)->setStyleClass("tblHeader");

    result r = m_db->Sql() << "SELECT rowid, email, firstname, lastname, phone, teammates FROM ["
                              + m_dbTables->Table("USERS_INDIVIDUAL")
                              + "] ORDER BY rowid ASC;";
    size_t i = 0;
    while(r.next()) {
        ++i;
        size_t rowid;
        string email;
        string firstname;
        string lastname;
        string phone;
        string teammates;

        r >> rowid >> email >> firstname >> lastname >> phone >> teammates;


        string altphone;
        string nationalcode;
        int educationlevel = EIEDULEVEL_NONE;
        string educationsubject;
        string website;
        result rExtra = m_db->Sql() << "SELECT altphone, nationalcode, educationlevel, educationsubject, website FROM ["
                                       + m_dbTables->Table("USERS_INDIVIDUAL_EXTRA")
                                       + "] WHERE email=?;" << email << row;
        if (!rExtra.empty()) {
            rExtra >> altphone >> nationalcode >> educationlevel >> educationsubject >> website;

            altphone = Crypto::Decrypt(altphone);
            nationalcode = Crypto::Decrypt(nationalcode);
            educationsubject = Crypto::Decrypt(educationsubject);
            website = Crypto::Decrypt(website);
        }

        firstname = Crypto::Decrypt(firstname);
        lastname = Crypto::Decrypt(lastname);
        phone = Crypto::Decrypt(phone);
        teammates = algorithm::replace_all_copy(Crypto::Decrypt(teammates), "\n", "<br />");

        table->elementAt(i, 0)->addWidget(new WText(lexical_cast<wstring>(i)));
        table->elementAt(i, 1)->addWidget(new WText(WString::fromUTF8(firstname)));
        table->elementAt(i, 2)->addWidget(new WText(WString::fromUTF8(lastname)));
        table->elementAt(i, 3)->addWidget(new WText(WString::fromUTF8(email)));
        table->elementAt(i, 4)->addWidget(new WText(WString::fromUTF8(phone)));
        table->elementAt(i, 5)->addWidget(new WText(WString::fromUTF8(altphone)));
        table->elementAt(i, 6)->addWidget(new WText(WString::fromUTF8(teammates)));
        table->elementAt(i, 7)->addWidget(new WText(WString::fromUTF8(nationalcode)));
        table->elementAt(i, 8)->addWidget(new WText(m_educationLevel[educationlevel]));
        table->elementAt(i, 9)->addWidget(new WText(WString::fromUTF8(educationsubject)));
        table->elementAt(i, 10)->addWidget(new WText(WString::fromUTF8(website)));
    }

    return root;
}

Wt::WWidget *CmsUsers::GetCompaniesTable()
{
    Div *root = new Div("CompaniesTable");

    WTable *table = new WTable(root);
    table->setStyleClass("tbl");

    table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_NO_TEXT")));
    table->elementAt(0, 1)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_NAME_TEXT")));
    table->elementAt(0, 2)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_REGISTERATION_ID_TEXT")));
    table->elementAt(0, 3)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_CEO_NAME_TEXT")));
    table->elementAt(0, 4)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_ECONOMICAL_CODE_TEXT")));
    table->elementAt(0, 5)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_FOUNDING_YEAR_TEXT")));
    table->elementAt(0, 6)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_RESUME_TEXT")));
    table->elementAt(0, 7)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_EMAIL_TEXT")));
    table->elementAt(0, 8)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_WEBSITE_TEXT")));
    table->elementAt(0, 9)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_PHONE_TEXT")));
    table->elementAt(0, 10)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_ALTERNATIVE_PHONE_TEXT")));
    table->elementAt(0, 11)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_ADDRESS_TEXT")));
    table->elementAt(0, 0)->setStyleClass("tblHeader");
    table->elementAt(0, 1)->setStyleClass("tblHeader");
    table->elementAt(0, 2)->setStyleClass("tblHeader");
    table->elementAt(0, 3)->setStyleClass("tblHeader");
    table->elementAt(0, 4)->setStyleClass("tblHeader");
    table->elementAt(0, 5)->setStyleClass("tblHeader");
    table->elementAt(0, 6)->setStyleClass("tblHeader");
    table->elementAt(0, 7)->setStyleClass("tblHeader");
    table->elementAt(0, 8)->setStyleClass("tblHeader");
    table->elementAt(0, 9)->setStyleClass("tblHeader");
    table->elementAt(0, 10)->setStyleClass("tblHeader");
    table->elementAt(0, 11)->setStyleClass("tblHeader");

    result r = m_db->Sql() << "SELECT rowid, email, name, regid, phone, ceo FROM ["
                              + m_dbTables->Table("USERS_COMPANY")
                              + "] ORDER BY rowid ASC;";

    size_t i = 0;
    while(r.next()) {
        ++i;
        size_t rowid;
        string email;
        string name;
        string regid;
        string phone;
        string ceo;

        r >> rowid >> email >> name >> regid >> phone >> ceo;

        string altphone;
        string economicalcode;
        string foundingyear;
        string addr;
        string website;
        string resumeurl;
        result rExtra = m_db->Sql() << "SELECT altphone, economicalcode, foundingyear, addr, website, resumeurl FROM ["
                                       + m_dbTables->Table("USERS_COMPANY_EXTRA")
                                       + "] WHERE email=?;" << email << row;
        if (!rExtra.empty()) {
            rExtra >> altphone >> economicalcode >> foundingyear >> addr >> website >> resumeurl;

            altphone = Crypto::Decrypt(altphone);
            economicalcode = Crypto::Decrypt(economicalcode);
            foundingyear = Crypto::Decrypt(foundingyear);
            addr = Crypto::Decrypt(addr);
            website = Crypto::Decrypt(website);
        }

        name = Crypto::Decrypt(name);
        regid = Crypto::Decrypt(regid);
        phone = Crypto::Decrypt(phone);
        ceo = Crypto::Decrypt(ceo);


        table->elementAt(i, 0)->addWidget(new WText(lexical_cast<wstring>(i)));
        table->elementAt(i, 1)->addWidget(new WText(WString::fromUTF8(name)));
        table->elementAt(i, 2)->addWidget(new WText(WString::fromUTF8(regid)));
        table->elementAt(i, 3)->addWidget(new WText(WString::fromUTF8(ceo)));
        table->elementAt(i, 4)->addWidget(new WText(WString::fromUTF8(economicalcode)));
        table->elementAt(i, 5)->addWidget(new WText(WString::fromUTF8(foundingyear)));

        if (algorithm::trim_copy(resumeurl) != "") {
            Wt::WFileResource *resumeFile = new Wt::WFileResource("application/pdf", resumeurl);
            resumeFile->suggestFileName(Base::RESUME_FILE_NAME);
            Wt::WAnchor *resumeAnchor = new Wt::WAnchor(resumeFile, m_lang->GetString("ROOT_CMSUSERS_COMPANY_RESUME_TEXT"));
            table->elementAt(i, 6)->addWidget(resumeAnchor);
        }
        table->elementAt(i, 7)->addWidget(new WText(WString::fromUTF8(email)));
        table->elementAt(i, 8)->addWidget(new WText(WString::fromUTF8(website)));
        table->elementAt(i, 9)->addWidget(new WText(WString::fromUTF8(phone)));
        table->elementAt(i, 10)->addWidget(new WText(WString::fromUTF8(altphone)));
        table->elementAt(i, 11)->addWidget(new WText(WString::fromUTF8(addr)));
    }

    return root;
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *CmsUsers::Layout()
{
    Div *root = new Div("CmsUsers");

    new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_TABLE_TITLE"), root);
    WPushButton *showIndividualsTableButton = new WPushButton(m_lang->GetString("ROOT_CMSUSERS_GET_INDIVIDUALS_TABLE_BUTTON"), root);
    showIndividualsTableButton->setStyleClass("formButtonNoWidth");

    new WBreak(root);

    new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_TABLE_TITLE"), root);
    WPushButton *showCompaniesTableButton = new WPushButton(m_lang->GetString("ROOT_CMSUSERS_GET_COMPANIES_TABLE_BUTTON"), root);
    showCompaniesTableButton->setStyleClass("formButtonNoWidth");

    showIndividualsTableButton->clicked().connect(this, &CmsUsers::OnShowIndividualsTableButtonPressed);
    showCompaniesTableButton->clicked().connect(this, &CmsUsers::OnShowCompaniesTableButtonPressed);


    new WBreak(root);
    new WBreak(root);
    Div *dvSendMessage = new Div(root, "dvSendMessage");

    WPushButton *sendMessageButton = new WPushButton(m_lang->GetString("ROOT_CMSUSERS_SEND_MESSAGE_BUTTON"), dvSendMessage);
    sendMessageButton->setStyleClass("editorButton");

    m_messageSubjectLineEdit = new WLineEdit(dvSendMessage);
    m_messageBodyTextEdit = new WTextEdit(dvSendMessage);

    m_messageSubjectLineEdit->setEmptyText(m_lang->GetString("ROOT_CMSUSERS_SEND_MESSAGE_SUBJECT_EMPTY_TEXT"));
    m_messageSubjectLineEdit->resize(535, WLength::Auto);
    // causing bug
    //m_messageBodyTextEdit->setEmptyText(m_lang->GetString("ROOT_CMSUSERS_SEND_MESSAGE_BODY_EMPTY_TEXT"));
    // use this instead
    m_messageBodyTextEdit->setText(m_lang->GetString("ROOT_CMSUSERS_SEND_MESSAGE_BODY_EMPTY_TEXT"));
    m_messageBodyTextEdit->resize(550, 400);

    WLengthValidator *subjectValidator = new WLengthValidator(Base::CONTACTUS_MIN_SUBJECT_LEN, Base::CONTACTUS_MAX_SUBJECT_LEN);
    //WLengthValidator *bodyValidator = new WLengthValidator(Base::CONTACTUS_MIN_BODY_LEN, Base::CONTACTUS_MAX_BODY_LEN);

    subjectValidator->setMandatory(true);
    //bodyValidator->setMandatory(true);

    m_messageSubjectLineEdit->setValidator(subjectValidator);
    //m_messageBodyTextEdit->setValidator(bodyValidator);

    sendMessageButton->clicked().connect(this, &CmsUsers::OnSendMessageButtonPressed);

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


