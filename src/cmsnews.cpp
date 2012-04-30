/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WGridLayout>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WMessageBox>
#include <Wt/WPushButton>
#include <Wt/WSignalMapper>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WWidget>
#include "cmsnews.hpp"
#include "base.hpp"
#include "cdate.hpp"
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

CmsNews::CmsNews(CgiRoot *cgi) : BaseWidget(cgi)
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

void CmsNews::OnAddButtonPressed()
{
    m_dlg = new WDialog(m_lang->GetString("ROOT_CMSNEWS_ADD_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->contents()->addWidget(GetAddNewsForm());
    m_dlg->rejectWhenEscapePressed();
    m_dlg->show();

    // HACK for 3.2.3-rc1
    if (m_dlg->height().value() < 415)
        m_dlg->resize(WLength::Auto, 415);
}

void CmsNews::OnAddDialogAddButtonPressed()
{
    if(!Validate(m_titleLineEdit) || !Validate(m_bodyTextArea)) {
        return;
    }

    CDate::Now n;

    WString date;
    switch(m_cgiEnv->CurrentLang) {
    case CgiEnv::ELang_RootEn:
        date = WString::fromUTF8(CDate::DateConv::ToGregorian(n));
        break;

    case CgiEnv::ELang_RootFa:
        date = CDate::DateConv::FormatToPersianNums(CDate::DateConv::ToJalali(n));
        break;

    default:
        break;
    }

    m_db->Insert(m_dbTables->Table("NEWS"), "title, body, date, archived", 4,
                 Crypto::Encrypt(trim_copy(m_titleLineEdit->text().toUTF8())).c_str(),
                 Crypto::Encrypt(trim_copy(m_bodyTextArea->text().toUTF8())).c_str(),
                 Crypto::Encrypt(date.toUTF8()).c_str(), "0");

    FillNewsDataTable();

    delete m_dlg;
    m_dlg = NULL;
}

void CmsNews::OnAddDialogReturnButtonPressed()
{
    delete m_dlg;
    m_dlg = NULL;
}

void CmsNews::OnArchiveButtonPressed(Wt::WPushButton *sender)
{
    string id(sender->attributeValue("dbid").toUTF8());

    result r = m_db->Sql() << "SELECT archived FROM ["
                              + m_dbTables->Table("NEWS")
                              + "] WHERE rowid=?;" << id << row;
    if (!r.empty()) {
        string archived;
        r >> archived;

        if (archived == "0") {
            m_db->Update(m_dbTables->Table("NEWS"), "rowid", id, "archived=?", 1, "1");
        } else {
            m_db->Update(m_dbTables->Table("NEWS"), "rowid", id, "archived=?", 1, "0");
        }
    }

    FillNewsDataTable();
}

void CmsNews::OnEditButtonPressed(Wt::WPushButton *sender)
{
    m_tableEditButton = sender;

    string id(sender->attributeValue("dbid").toUTF8());

    m_dlg = new WDialog(m_lang->GetString("ROOT_CMSNEWS_EDIT_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->contents()->addWidget(GetEditNewsForm(id));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->show();

    // HACK for 3.2.3-rc1
    if (m_dlg->height().value() < 415)
        m_dlg->resize(WLength::Auto, 415);
}

void CmsNews::OnEditDialogEditButtonPressed()
{
    if(!Validate(m_titleLineEdit) || !Validate(m_bodyTextArea)) {
        return;
    }

    string id(m_tableEditButton->attributeValue("dbid").toUTF8());

    m_db->Update(m_dbTables->Table("NEWS"), "rowid", id, "title=?, body=?", 2,
                 Crypto::Encrypt(trim_copy(m_titleLineEdit->text().toUTF8())).c_str(),
                 Crypto::Encrypt(trim_copy(m_bodyTextArea->text().toUTF8())).c_str());

    FillNewsDataTable();

    delete m_dlg;
    m_dlg = NULL;
    m_tableEditButton = NULL;
}

void CmsNews::OnEditDialogReturnButtonPressed()
{
    delete m_dlg;
    m_dlg = NULL;
}

void CmsNews::OnEraseButtonPressed(Wt::WPushButton *sender)
{
    m_tableEraseButton = sender;

    wstring question((wformat(m_lang->GetString("ROOT_CMSNEWS_ERASE_MSG_QUESTION"))
                  % m_tableEraseButton->attributeValue("dbid").value()).str());

    m_msg = new WMessageBox(m_lang->GetString("ROOT_CMSNEWS_ERASE_MSG_TITLE"),
                            question, Warning, NoButton);

    m_msg->addButton(m_lang->GetString("ROOT_CMSNEWS_ERASE_MSG_OK_BUTTON"), Ok);
    m_msg->addButton(m_lang->GetString("ROOT_CMSNEWS_ERASE_MSG_CANCEL_BUTTON"), Cancel);

    m_msg->buttonClicked().connect(this, &CmsNews::OnEraseMessageBoxClosed);

    m_msg->show();
}

void CmsNews::OnEraseMessageBoxClosed(Wt::StandardButton result)
{
    if (result == Ok) {
        string id(m_tableEraseButton->attributeValue("dbid").toUTF8());

        cppdb::result r = m_db->Sql() << "SELECT rowid FROM ["
                                         + m_dbTables->Table("NEWS")
                                         + "] WHERE rowid=?;" << id << row;
        if (!r.empty()) {
            m_db->Delete(m_dbTables->Table("NEWS"), "rowid", id);
        }

        FillNewsDataTable();
    }

    delete m_msg;
    m_msg = NULL;
    m_tableEraseButton = NULL;
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

void CmsNews::FillNewsDataTable()
{
    m_dvNewsTable->clear();

    WTable *newsTable = new WTable(m_dvNewsTable);
    newsTable->setStyleClass("tbl");

    newsTable->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSNEWS_NO_TEXT")));
    newsTable->elementAt(0, 1)->addWidget(new WText(m_lang->GetString("ROOT_CMSNEWS_TITLE_TEXT")));
    newsTable->elementAt(0, 2)->addWidget(new WText(m_lang->GetString("ROOT_CMSNEWS_DATE_TEXT")));
    newsTable->elementAt(0, 3)->addWidget(new WText(m_lang->GetString("ROOT_CMSNEWS_ARCHIVED_TEXT")));
    newsTable->elementAt(0, 4)->addWidget(new WText(m_lang->GetString("ROOT_CMSNEWS_EDIT_TEXT")));
    newsTable->elementAt(0, 5)->addWidget(new WText(m_lang->GetString("ROOT_CMSNEWS_ERASE_TEXT")));
    newsTable->elementAt(0, 0)->setStyleClass("tblHeader");
    newsTable->elementAt(0, 1)->setStyleClass("tblHeader");
    newsTable->elementAt(0, 2)->setStyleClass("tblHeader");
    newsTable->elementAt(0, 3)->setStyleClass("tblHeader");
    newsTable->elementAt(0, 4)->setStyleClass("tblHeader");
    newsTable->elementAt(0, 5)->setStyleClass("tblHeader");

    result r = m_db->Sql() << "SELECT rowid, title, date, archived FROM ["
                              + m_dbTables->Table("NEWS")
                              + "] ORDER BY rowid DESC";
    size_t i = 0;
    while(r.next()) {
        ++i;
        size_t rowid;
        string title;
        string date;
        string archived;

        r >> rowid >> title >> date >> archived;

        title = Crypto::Decrypt(title);
        date = Crypto::Decrypt(date);

        newsTable->elementAt(i, 0)->addWidget(new WText(lexical_cast<wstring>(i)));
        newsTable->elementAt(i, 1)->addWidget(new WText(WString::fromUTF8(title)));
        newsTable->elementAt(i, 2)->addWidget(new WText(WString::fromUTF8(date)));

        WSignalMapper<WPushButton *> *archiveSignalMapper = new WSignalMapper<WPushButton *>(this);
        WSignalMapper<WPushButton *> *editSignalMapper = new WSignalMapper<WPushButton *>(this);
        WSignalMapper<WPushButton *> *eraseSignalMapper = new WSignalMapper<WPushButton *>(this);

        archiveSignalMapper->mapped().connect(this, &CmsNews::OnArchiveButtonPressed);
        editSignalMapper->mapped().connect(this, &CmsNews::OnEditButtonPressed);
        eraseSignalMapper->mapped().connect(this, &CmsNews::OnEraseButtonPressed);

        wstring archiveText;
        if (archived == "0") {
            archiveText = m_lang->GetString("ROOT_CMSNEWS_ARCHIVED_TEXT");
        } else {
            archiveText = m_lang->GetString("ROOT_CMSNEWS_UNARCHIVE_TEXT");
        }

        WPushButton *archiveButton = new WPushButton(archiveText);
        WPushButton *editButton = new WPushButton(m_lang->GetString("ROOT_CMSNEWS_EDIT_TEXT"));
        WPushButton *eraseButton = new WPushButton(m_lang->GetString("ROOT_CMSNEWS_ERASE_TEXT"));

        archiveSignalMapper->mapConnect(archiveButton->clicked(), archiveButton);
        editSignalMapper->mapConnect(editButton->clicked(), editButton);
        eraseSignalMapper->mapConnect(eraseButton->clicked(), eraseButton);

        archiveButton->setStyleClass("tblButton");
        editButton->setStyleClass("tblButton");
        eraseButton->setStyleClass("tblButton");

        archiveButton->setAttributeValue("dbid", boost::lexical_cast<wstring>(rowid));
        editButton->setAttributeValue("dbid", boost::lexical_cast<wstring>(rowid));
        eraseButton->setAttributeValue("dbid", boost::lexical_cast<wstring>(rowid));

        newsTable->elementAt(i, 3)->addWidget(archiveButton);
        newsTable->elementAt(i, 4)->addWidget(editButton);
        newsTable->elementAt(i, 5)->addWidget(eraseButton);
    }
}

Wt::WWidget *CmsNews::GetAddNewsForm()
{
    Div *root = new Div("AddNewsForm");

    Div *dvForm = new Div(root);
    dvForm->setStyleClass("form");
    WGridLayout *dvFormLayout = new WGridLayout();

    m_titleLineEdit = new WLineEdit();
    m_bodyTextArea = new WTextArea();

    m_bodyTextArea->resize(WLength::Auto, 285);

    dvFormLayout->addWidget(new WText(m_lang->GetString("ROOT_CMSNEWS_TITLE_TEXT")),
                                      0, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(m_titleLineEdit, 0, 1);
    dvFormLayout->addWidget(new WText(m_lang->GetString("ROOT_CMSNEWS_BODY_TEXT")),
                                      1, 0, AlignLeft | AlignTop);
    dvFormLayout->addWidget(m_bodyTextArea, 1, 1);

    dvFormLayout->setColumnStretch(0, 50);
    dvFormLayout->setColumnStretch(1, 450);
    dvFormLayout->setVerticalSpacing(11);
    dvForm->resize(500, WLength::Auto);
    dvForm->setLayout(dvFormLayout);

    Div *dvButtons = new Div(root, "dvDialogButtons");
    WPushButton *addButton = new WPushButton(m_lang->GetString("ROOT_CMSNEWS_ADD_DLG_ADD_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("ROOT_CMSNEWS_ADD_DLG_RETURN_BUTTON"), dvButtons);

    addButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    WLengthValidator *titleValidator = new WLengthValidator(Base::NEWS_MIN_TITLE_LEN, Base::NEWS_MAX_TITLE_LEN);
    WLengthValidator *bodyValidator = new WLengthValidator(Base::NEWS_MIN_BODY_LEN, Base::NEWS_MAX_BODY_LEN);

    titleValidator->setMandatory(true);
    bodyValidator->setMandatory(true);

    m_titleLineEdit->setValidator(titleValidator);
    m_bodyTextArea->setValidator(bodyValidator);

    m_titleLineEdit->setFocus();

    m_titleLineEdit->enterPressed().connect(this, &CmsNews::OnAddDialogAddButtonPressed);
    addButton->clicked().connect(this, &CmsNews::OnAddDialogAddButtonPressed);
    returnButton->clicked().connect(this, &CmsNews::OnAddDialogReturnButtonPressed);

    return root;
}

Wt::WWidget *CmsNews::GetEditNewsForm(string id)
{
    Div *root = new Div("EditNewsForm");

    Div *dvForm = new Div(root);
    dvForm->setStyleClass("form");
    WGridLayout *dvFormLayout = new WGridLayout();

    m_titleLineEdit = new WLineEdit();
    m_bodyTextArea = new WTextArea();

    m_bodyTextArea->resize(WLength::Auto, 285);

    result r = m_db->Sql() << "SELECT title, body FROM ["
                              + m_dbTables->Table("NEWS")
                              + "] WHERE rowid=?;" << id << row;
    if (!r.empty()) {
        string title;
        string body;
        r >> title >> body;

        title = Crypto::Decrypt(title);
        body = Crypto::Decrypt(body);

        m_titleLineEdit->setText(WString::fromUTF8(title));
        m_bodyTextArea->setText(WString::fromUTF8(body));
    }

    dvFormLayout->addWidget(new WText(m_lang->GetString("ROOT_CMSNEWS_TITLE_TEXT")),
                                      0, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(m_titleLineEdit, 0, 1);
    dvFormLayout->addWidget(new WText(m_lang->GetString("ROOT_CMSNEWS_BODY_TEXT")),
                                      1, 0, AlignLeft | AlignTop);
    dvFormLayout->addWidget(m_bodyTextArea, 1, 1);

    dvFormLayout->setColumnStretch(0, 50);
    dvFormLayout->setColumnStretch(1, 450);
    dvFormLayout->setVerticalSpacing(11);
    dvForm->resize(500, WLength::Auto);
    dvForm->setLayout(dvFormLayout);

    Div *dvButtons = new Div(root, "dvDialogButtons");
    WPushButton *editButton = new WPushButton(m_lang->GetString("ROOT_CMSNEWS_EDIT_DLG_EDIT_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("ROOT_CMSNEWS_ADD_DLG_RETURN_BUTTON"), dvButtons);

    editButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    WLengthValidator *titleValidator = new WLengthValidator(Base::NEWS_MIN_TITLE_LEN, Base::NEWS_MAX_TITLE_LEN);
    WLengthValidator *bodyValidator = new WLengthValidator(Base::NEWS_MIN_BODY_LEN, Base::NEWS_MAX_BODY_LEN);

    titleValidator->setMandatory(true);
    bodyValidator->setMandatory(true);

    m_titleLineEdit->setValidator(titleValidator);
    m_bodyTextArea->setValidator(bodyValidator);

    m_titleLineEdit->setFocus();

    m_titleLineEdit->enterPressed().connect(this, &CmsNews::OnEditDialogEditButtonPressed);
    editButton->clicked().connect(this, &CmsNews::OnEditDialogEditButtonPressed);
    returnButton->clicked().connect(this, &CmsNews::OnEditDialogReturnButtonPressed);

    return root;
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *CmsNews::Layout()
{
    Div *root = new Div("CmsNews");

    new WText(m_lang->GetString("ROOT_CMSNEWS_NEWS_MANAGEMENT_TITLE"), root);

    Div *dvCNews = new Div(root, "dvCNews");

    WPushButton *addButton = new WPushButton(m_lang->GetString("ROOT_CMSNEWS_ADD_BUTTON"),
                                                 dvCNews);
    addButton->setStyleClass("formButton");

    m_dvNewsTable = new Div(dvCNews, "dvNewsTable");
    FillNewsDataTable();

    addButton->clicked().connect(this, &CmsNews::OnAddButtonPressed);

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


