/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/algorithm/string.hpp>
#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WMessageBox>
#include <Wt/WSignalMapper>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WWidget>
#include "news.hpp"
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

News::News(CgiRoot *cgi) : BaseWidget(cgi)
{
    m_dlg = NULL;

    this->clear();
    this->addWidget(Layout());
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

bool News::ReadNews(string archived, Div *parent)
{
    result r = m_db->Sql() << "SELECT rowid, title, body, date, archived FROM ["
                              + m_dbTables->Table("NEWS")
                              + "] WHERE archived=?"
                              "ORDER BY rowid DESC;" << archived;
    size_t i = 0;

    while(r.next()) {
        ++i;

        string id;
        string title;
        string body;
        string date;
        string archived;

        r >> id >> title >> body >> date >> archived;

        title = Crypto::Decrypt(title);
        body = Crypto::Decrypt(body);
        date = Crypto::Decrypt(date);

        string summary;
        size_t found = body.find("\n");
        if (found != string::npos) {
            summary = body.substr(0, found);
        } else {
            summary = body;
        }

        Div *dvNewsRow = new Div(parent, "dvNewsRow");
        Div *dvNewsHeader = new Div(dvNewsRow, "dvNewsHeader");
        Div *dvNewsDate = new Div(dvNewsHeader, "dvNewsDate");
        Div *dvNewsTitle = new Div(dvNewsHeader, "dvNewsTitle");
        Div *dvNewsSummary = new Div(dvNewsRow, "dvNewsSummary");

        WText *dateLink = new WText(WString::fromUTF8(date), dvNewsDate);
        WText *titleLink = new WText(WString::fromUTF8(title), dvNewsTitle);
        WText *summaryLink = new WText(WString::fromUTF8(summary) + m_lang->GetString("HOME_NEWS_CONTINUE_READING_TEXT"), dvNewsSummary);

        dateLink->setStyleClass("newslink");
        titleLink->setStyleClass("newslink");
        summaryLink->setStyleClass("newslink");

        dateLink->setAttributeValue("dbid", id);
        titleLink->setAttributeValue("dbid", id);
        summaryLink->setAttributeValue("dbid", id);

        WSignalMapper<WText *> *newsLinkMap = new WSignalMapper<WText *>(this);
        newsLinkMap->mapped().connect(this, &News::OnNewsLinkClicked);
        newsLinkMap->mapConnect(dateLink->clicked(), dateLink);
        newsLinkMap->mapConnect(titleLink->clicked(), titleLink);
        newsLinkMap->mapConnect(summaryLink->clicked(), summaryLink);
    }

    if (i != 0)
        return true;
    else
        return false;
}

/**********************************
*    = Event Handlers =
**********************************/

void News::OnNewsLinkClicked(WText *sender)
{
    ////// NOTE:
    /// IF YOU ARE GOING TO CHANGE THIS FUNCTION,
    /// YOU HAVE TO CHANGE NEWS HANDLING SECTION CODE
    /// AT THE END OF
    /// WWidget *Home::Layout() function

    string id(sender->attributeValue("dbid").toUTF8());

    string title;
    string body;
    string date;

    result r = m_db->Sql() << "SELECT title, body, date FROM ["
                              + m_dbTables->Table("NEWS")
                              + "] WHERE rowid=?;" << id << row;
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
                                         + Crypto::Encrypt(id)
                                         + "\">"+ title + "</a></h5>"));
    new WText(WString::fromUTF8(body), dvNewsComplete);

    layout->addWidget(dvNewsComplete);
    layout->setStretchFactor(dvNewsComplete, 0);

    delete m_dlg;
    m_dlg = NULL;

    m_dlg = new WDialog(WString::fromUTF8(title));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(768, Wt::WLength::Pixel),
                  Wt::WLength(64, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);
    m_dlg->contents()->setLayout(layout);
    m_dlg->show();
}

void News::OnOlderNewsLinkClicked(WText *sender)
{
    if (!m_isOlderNewsCollectionShown) {
        m_isOlderNewsCollectionShown = true;

        if (ReadNews("1", m_dvOlderNewsCollection)) {

        } else {
            m_dvOlderNewsCollection->clear();
        }
    } else {
        m_dvOlderNewsCollection->clear();
        m_isOlderNewsCollectionShown = false;
    }
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

WWidget *News::Layout()
{
    Div *root = new Div("News");

    Div *dvNews = new Div(root, "dvNews");

    Div *dvNewsCollection = new Div("dvNewsCollection");

    if (ReadNews("0", dvNewsCollection)) {
        Div *dvNewsTitleBar = new Div(dvNews, "dvNewsTitleBar");
        new WText(m_lang->GetString("HOME_NEWS_TITLE_BAR_TEXT"), dvNewsTitleBar);

        dvNews->addWidget(dvNewsCollection);

        m_isOlderNewsCollectionShown = false;
        result r = m_db->Sql() << "SELECT rowid, title, body, date, archived FROM ["
                                  + m_dbTables->Table("NEWS")
                                  + "] WHERE archived=?"
                                  "ORDER BY rowid DESC;" << "1" << row;
        if (!r.empty()) {
            WText *olderNewsLink = new WText(m_lang->GetString("HOME_NEWS_OLDER_NEWS_TEXT"), dvNews);
            olderNewsLink->setStyleClass("link");
            WSignalMapper<WText *> *olderNewsLinkMap = new WSignalMapper<WText *>(this);
            olderNewsLinkMap->mapped().connect(this, &News::OnOlderNewsLinkClicked);
            olderNewsLinkMap->mapConnect(olderNewsLink->clicked(), olderNewsLink);

            Div *dvOlderNews = new Div(dvNews, "dvOlderNews");
            new WBreak(dvOlderNews);
            dvOlderNews->addWidget(olderNewsLink);
            m_dvOlderNewsCollection = new Div(dvOlderNews, "dvOlderNewsCollection");
        }
    } else {
        delete dvNewsCollection;
    }

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


