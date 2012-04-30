/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WInPlaceEdit>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WMessageBox>
#include <Wt/WPushButton>
#include <Wt/WRegExpValidator>
#include <Wt/WSignalMapper>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WTextEdit>
#include <Wt/WWidget>
#include "cmscontacts.hpp"
#include "base.hpp"
#include "cgienv.hpp"
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

CmsContacts::CmsContacts(CgiRoot *cgi) : BaseWidget(cgi)
{
    this->clear();
    this->addWidget(Layout());
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

/**********************************
*    = Event Handlers =
**********************************/

void CmsContacts::OnAddContactOK()
{
    if(Validate(m_contactRecipientEdit) && Validate(m_contactAddrEdit)) {
        string recipient(algorithm::trim_copy(m_contactRecipientEdit->text().toUTF8()));
        string addr(Crypto::Encrypt(algorithm::trim_copy(m_contactAddrEdit->text().toUTF8())));

        result r = m_db->Sql() << "SELECT recipient FROM ["
                                  + m_dbTables->Table("CONTACTS")
                                  + "] WHERE recipient=?;" << recipient << row;

        if (!r.empty()) {
            HtmlError(m_lang->GetString("ROOT_CMSCONTACTS_ADD_DUPLICATE_CONTACT_ERR"), m_errAddContact);
            m_contactRecipientEdit->setFocus();
            return;
        }
        else {
            HtmlError(L"", m_errAddContact);

            m_db->Insert(m_dbTables->Table("CONTACTS"), "recipient, addr", 2,
                         recipient.c_str(), addr.c_str());

            m_contactRecipientEdit->setText("");
            m_contactAddrEdit->setText("");
            FillContactsDataTable();
        }

        m_contactRecipientEdit->setFocus();
    }
}

void CmsContacts::OnSaveContactsTableCell(WInPlaceEdit *sender)
{
    string recipient(sender->attributeValue("dbid").toUTF8());
    string field(sender->attributeValue("dbfield").toUTF8());
    string value(algorithm::trim_copy(sender->text().toUTF8()));

    result r = m_db->Sql() << "SELECT recipient FROM ["
                              + m_dbTables->Table("CONTACTS")
                              + "] WHERE recipient=?;" << recipient << row;

    if (!r.empty()) {
        if (field == "recipient" && recipient != value) {
            r = m_db->Sql() << "SELECT recipient FROM ["
                               + m_dbTables->Table("CONTACTS")
                               + "] WHERE recipient=?;" << value << row;
            if (!r.empty()) {
                FillContactsDataTable();
                HtmlError(m_lang->GetString("ROOT_CMSCONTACTS_ADD_DUPLICATE_CONTACT_ERR"), m_errAddContact);
                return;
            }
        }

        if (field == "addr")
            value = Crypto::Encrypt(value);

        HtmlError(L"", m_errAddContact);
        m_db->Update(m_dbTables->Table("CONTACTS"), "recipient", recipient, field + "=?", 1, value.c_str());
     }

    FillContactsDataTable();
    m_contactRecipientEdit->setFocus();
}

void CmsContacts::OnEraseContactsTableCell(WPushButton *sender)
{
    m_tableEraseButton = sender;

    wstring question((wformat(m_lang->GetString("ROOT_CMSCONTACTS_ERASE_MSG_QUESTION"))
                  % m_tableEraseButton->attributeValue("dbid").value()).str());

    m_msg = new WMessageBox(m_lang->GetString("ROOT_CMSCONTACTS_ERASE_MSG_TITLE"),
                            question, Warning, NoButton);

    m_msg->addButton(m_lang->GetString("ROOT_CMSCONTACTS_ERASE_MSG_OK_BUTTON"), Ok);
    m_msg->addButton(m_lang->GetString("ROOT_CMSCONTACTS_ERASE_MSG_CANCEL_BUTTON"), Cancel);

    m_msg->buttonClicked().connect(this, &CmsContacts::OnEraseContactsTableCellOK);

    m_msg->show();
}

void CmsContacts::OnEraseContactsTableCellOK(StandardButton resultButton)
{
    if (resultButton == Ok) {
        string recipient(m_tableEraseButton->attributeValue("dbid").toUTF8());

        result r = m_db->Sql() << "SELECT recipient FROM ["
                                  + m_dbTables->Table("CONTACTS")
                                  + "] WHERE recipient=?;" << recipient << row;
        if (!r.empty()) {
            m_db->Delete(m_dbTables->Table("CONTACTS"), "recipient", recipient);
        }

        FillContactsDataTable();
        m_contactRecipientEdit->setFocus();
    }

    delete m_msg;
    m_msg = NULL;
    m_tableEraseButton = NULL;
}

void CmsContacts::OnExtraInfoSaveButtonPressed()
{
    string lang;
    switch(m_cgiEnv->CurrentLang) {
    case CgiEnv::ELang_RootEn:
        lang = "en";
        break;

    case CgiEnv::ELang_RootFa:
        lang = "fa";
        break;

    default:
        break;
    }

    m_db->Update(m_dbTables->Table("CONTACTS_EXTRA_INFO"), "lang", lang,
                 "info=?", 1,
                 Crypto::Encrypt(m_extraContactInfoTextEdit->text().toUTF8()).c_str());

    HtmlError(m_lang->GetString("ROOT_CMSCONTACTS_EXTRA_INFO_EDITOR_SAVE_SUCCESS_MSG"), m_errExtraContactInfo);
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

void CmsContacts::FillContactsDataTable()
{
    m_dvContactsTable->clear();

    WTable *contactsTable = new WTable(m_dvContactsTable);
    contactsTable->setStyleClass("tbl");

    contactsTable->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSCONTACTS_RECIPIENT_TEXT")));
    contactsTable->elementAt(0, 1)->addWidget(new WText(m_lang->GetString("ROOT_CMSCONTACTS_ADDR_TEXT")));
    contactsTable->elementAt(0, 2)->addWidget(new WText(m_lang->GetString("ROOT_CMSCONTACTS_ERASE_TEXT")));
    contactsTable->elementAt(0, 0)->setStyleClass("tblHeader");
    contactsTable->elementAt(0, 1)->setStyleClass("tblHeader");
    contactsTable->elementAt(0, 2)->setStyleClass("tblHeader");

    result r = m_db->Sql() << "SELECT recipient, addr FROM ["
                              + m_dbTables->Table("CONTACTS")
                              + "] ORDER BY recipient ASC";

    int i = 0;
    while(r.next()) {
        ++i;
        string recipient, addr;
        r >> recipient >> addr;

        addr = Crypto::Decrypt(addr);

        WSignalMapper<WInPlaceEdit *> *cellSignalMapper = new WSignalMapper<WInPlaceEdit *>(this);
        cellSignalMapper->mapped().connect(this, &CmsContacts::OnSaveContactsTableCell);

        contactsTable->elementAt(i, 0)->addWidget(GetContactsCell(recipient, recipient, "recipient", cellSignalMapper));
        contactsTable->elementAt(i, 1)->addWidget(GetContactsCell(addr, recipient, "addr", cellSignalMapper));

        WSignalMapper<WPushButton *> *eraseSignalMapper = new WSignalMapper<WPushButton *>(this);
        eraseSignalMapper->mapped().connect(this, &CmsContacts::OnEraseContactsTableCell);
        WPushButton *eraseButton = new WPushButton(m_lang->GetString("ROOT_CMSCONTACTS_ERASE_BUTTON"));
        eraseSignalMapper->mapConnect(eraseButton->clicked(), eraseButton);
        eraseButton->setStyleClass("tblButton");
        eraseButton->setAttributeValue("dbid", WString::fromUTF8(recipient));
        contactsTable->elementAt(i, 2)->addWidget(eraseButton);
    }
}

Wt::WInPlaceEdit *CmsContacts::GetContactsCell(const string &cell, const string &id, const string &field,
                                               WSignalMapper<WInPlaceEdit *> *signalMapper)
{
    WInPlaceEdit *edit = new WInPlaceEdit(WString::fromUTF8(cell));
    edit->setStyleClass("inplace");
    edit->saveButton()->setText(m_lang->GetString("ROOT_CMSCONTACTS_SAVE_BUTTON"));
    edit->cancelButton()->setText(m_lang->GetString("ROOT_CMSCONTACTS_CANCEL_BUTTON"));
    edit->saveButton()->setStyleClass("tblButton");
    edit->cancelButton()->setStyleClass("tblButton");
    edit->setAttributeValue("dbid", WString::fromUTF8(id));
    edit->setAttributeValue("dbfield", WString::fromUTF8(field));
    signalMapper->mapConnect(edit->valueChanged(), edit);

    if (field == "recipient") {
        WLengthValidator *validator = new WLengthValidator(Base::MIN_RECIPEENT_NAME_LEN, Base::MAX_RECIPEENT_NAME_LEN);
        validator->setMandatory(true);
        edit->lineEdit()->setValidator(validator);
    }
    else {
        WRegExpValidator *validator = new WRegExpValidator(Base::REGEX_EMAIL);
        validator->setFlags(MatchCaseInsensitive);
        validator->setMandatory(true);
        edit->lineEdit()->setValidator(validator);
    }

    return edit;
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *CmsContacts::Layout()
{
    Div *root = new Div("CmsContacts");

    new WText(m_lang->GetString("ROOT_CMSCONTACTS_EDIT_CONTACTS_TITLE"), root);

    Div *dvCContacts = new Div(root, "dvCContacts");

    m_errAddContact = new WText(dvCContacts);
    new WBreak(dvCContacts);

    m_contactRecipientEdit = new WLineEdit(dvCContacts);
    m_contactRecipientEdit->resize(150, WLength::Auto);
    m_contactRecipientEdit->setEmptyText(m_lang->GetString("ROOT_CMSCONTACTS_RECIPIENT_TEXT"));
    new WText("&nbsp;&nbsp;&nbsp;", dvCContacts);
    m_contactAddrEdit = new WLineEdit(dvCContacts);
    m_contactAddrEdit->resize(150, WLength::Auto);
    m_contactAddrEdit->setEmptyText(m_lang->GetString("ROOT_CMSCONTACTS_ADDR_TEXT"));
    new WText("&nbsp;&nbsp;&nbsp;", dvCContacts);
    WPushButton *contactAddButton = new WPushButton(m_lang->GetString("ROOT_CMSCONTACTS_ADD_BUTTON"), dvCContacts);
    contactAddButton->setStyleClass("tblButton");

    new WBreak(dvCContacts);
    new WBreak(dvCContacts);
    new WText(m_lang->GetString("ROOT_CMSCONTACTS_EDIT_HINT"), dvCContacts);

    m_dvContactsTable = new Div(dvCContacts, "dvContactTable");
    FillContactsDataTable();

    new WBreak(root);
    new WBreak(root);

    Div *dvExtraInfoEditor = new Div(root, "dvExtraInfoEditor");
    m_errExtraContactInfo = new WText(dvExtraInfoEditor);
    new WBreak(dvExtraInfoEditor);
    WPushButton *extraInfoSaveButton = new WPushButton(m_lang->GetString("ROOT_CMSCONTACTS_EXTRA_INFO_EDITOR_SAVE_BUTTON"),
                                                       dvExtraInfoEditor);
    extraInfoSaveButton->setStyleClass("editorButton");

    string lang;
    switch(m_cgiEnv->CurrentLang) {
    case CgiEnv::ELang_RootEn:
        lang = "en";
        break;

    case CgiEnv::ELang_RootFa:
        lang = "fa";
        break;

    default:
        break;
    }

    result r = m_db->Sql() << "SELECT info FROM ["
                              + m_dbTables->Table("CONTACTS_EXTRA_INFO")
                              + "] WHERE lang=?;" << lang << row;
    string info;
    if (!r.empty()) {
        r >> info;
    }

    m_extraContactInfoTextEdit = new Wt::WTextEdit(WString::fromUTF8(Crypto::Decrypt(info)), dvExtraInfoEditor);
    m_extraContactInfoTextEdit->resize(550, 400);

    Wt::WLengthValidator *contactRecipientValidator = new WLengthValidator(Base::MIN_RECIPEENT_NAME_LEN, Base::MAX_RECIPEENT_NAME_LEN);
    Wt::WRegExpValidator *contactAddrValidator = new WRegExpValidator(Base::REGEX_EMAIL);
    contactAddrValidator->setFlags(MatchCaseInsensitive);

    contactRecipientValidator->setMandatory(true);
    contactAddrValidator->setMandatory(true);

    m_contactRecipientEdit->setValidator(contactRecipientValidator);
    m_contactAddrEdit->setValidator(contactAddrValidator);

    m_contactRecipientEdit->enterPressed().connect(this, &CmsContacts::OnAddContactOK);
    m_contactAddrEdit->enterPressed().connect(this, &CmsContacts::OnAddContactOK);
    contactAddButton->clicked().connect(this, &CmsContacts::OnAddContactOK);

    extraInfoSaveButton->clicked().connect(this, &CmsContacts::OnExtraInfoSaveButtonPressed);

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


