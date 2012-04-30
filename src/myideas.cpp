/**********************************
*    = Header File Inclusion =
**********************************/

#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WSignalMapper>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WWidget>
#include "myideas.hpp"
#include "crypto.hpp"
#include "db.hpp"
#include "ideasubmit.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace boost;
using namespace cppdb;
using namespace Wt;
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

MyIdeas::MyIdeas(CgiRoot *cgi, Wt::WDialog *parentDialog) : BaseWidget(cgi),
    m_dlgParent(parentDialog)
{
    m_dlg = NULL;
    m_dlgDetails = NULL;

    this->clear();
    this->addWidget(Layout());

    this->setOverflow(WContainerWidget::OverflowAuto);

    WVBoxLayout *layout = new WVBoxLayout();

    layout->addWidget(this);
    layout->setStretchFactor(this, 0);


    m_dlgParent->contents()->setLayout(layout);
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

void MyIdeas::OnDetailsLinkClicked(WAnchor *sender)
{
    string rowId(sender->attributeValue("dbid").toUTF8());

    string title;
    int typegamesentertainment;
    int typehygienehealth;
    int typenewsinfo;
    int typelearning;
    int typenonepresenceservices;
    int typeonlineshopping;
    int typeothers;
    string typeotherstext;
    string description;
    int audienceteens;
    int audienceparents;
    int audienceboys;
    int audiencegirls;
    int audiencewomen;
    int audiencemen;
    int audiencechilds;
    int audienceallfamilymembers;
    string usage;
    string similar;
    string difference;

    result r = m_db->Sql() << "SELECT title, "
                              "typegamesentertainment, typehygienehealth, typenewsinfo, typelearning, "
                              "typenonepresenceservices, typeonlineshopping, typeothers, typeotherstext, "
                              "description, "
                              "audienceteens, audienceparents, audienceboys, audiencegirls, audiencewomen, "
                              "audiencemen, audiencechilds, audienceallfamilymembers, "
                              "usage, similar, difference "
                              "FROM [" + m_dbTables->Table("IDEAS")
                              + "] WHERE rowid=?;" << rowId << row;
    if (!r.empty()) {
        r >> title >> typegamesentertainment >> typehygienehealth
          >> typenewsinfo >> typelearning >> typenonepresenceservices >> typeonlineshopping
          >> typeothers >> typeotherstext >> description >> audienceteens >> audienceparents >> audienceboys
          >> audiencegirls >> audiencewomen >> audiencemen >> audiencechilds >> audienceallfamilymembers
          >> usage >> similar >> difference;

        title = Crypto::Decrypt(title);
        typeotherstext = Crypto::Decrypt(typeotherstext);
        description = Crypto::Decrypt(description);
        usage = Crypto::Decrypt(usage);
        similar = Crypto::Decrypt(similar);
        difference = Crypto::Decrypt(difference);

        boost::algorithm::replace_all(description, "\n", "<br />");
        boost::algorithm::replace_all(usage, "\n", "<br />");
        boost::algorithm::replace_all(similar, "\n", "<br />");
        boost::algorithm::replace_all(difference, "\n", "<br />");
    }

    WTable *table = new WTable();
    table->setStyleClass("tbl");

    table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_TITLE_TEXT")));
    table->elementAt(1, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_TYPE_TEXT")));
    table->elementAt(2, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_TEXT")));
    table->elementAt(3, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_DESCRIPTION_TEXT")));
    table->elementAt(4, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_USAGE_TEXT")));
    table->elementAt(5, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_SIMILAR_TEXT")));
    table->elementAt(6, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_DIFFERENCE_TEXT")));

    table->elementAt(0, 1)->addWidget(new WText(WString::fromUTF8(title)));

    wstring type;
    bool br = false;
    if (typegamesentertainment) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("HOME_MY_IDEAS_TYPE_GAME_ENTERTAINMENT");
        br = true;
    }
    if (typehygienehealth) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("HOME_MY_IDEAS_TYPE_HYGIENE_HEALTH");
        br = true;
    }
    if (typenewsinfo) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("HOME_MY_IDEAS_TYPE_NEWS_INFO");
        br = true;
    }
    if (typelearning) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("HOME_MY_IDEAS_TYPE_LEARNING");
        br = true;
    }
    if (typenonepresenceservices) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("HOME_MY_IDEAS_TYPE_NONE_PRESENCE_SERVICES");
        br = true;
    }
    if (typeonlineshopping) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("HOME_MY_IDEAS_TYPE_ONLINE_SHOPPING");
        br = true;
    }
    if (typeothers) {
        if (br)
            type += L"<br />";
        typeotherstext = Crypto::Decrypt(typeotherstext);
        type += m_lang->GetString("HOME_MY_IDEAS_TYPE_OTHER") + L": "
                + WString::fromUTF8(typeotherstext).value();
        br = true;
    }


    wstring audience;
    br = false;
    if (audienceteens) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_TEENS");
        br = true;
    }
    if (audienceparents) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_PARENTS");
        br = true;
    }
    if (audienceboys) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_BOYS");
        br = true;
    }
    if (audiencegirls) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_GIRLS");
        br = true;
    }
    if (audiencewomen) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_WOMEN");
        br = true;
    }
    if (audiencemen) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_MEN");
        br = true;
    }
    if (audiencechilds) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_CHILDS");
        br = true;
    }
    if (audienceallfamilymembers) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_ALL_FAMILY_MEMBERS");
        br = true;
    }

    table->elementAt(1, 1)->addWidget(new WText(type));
    table->elementAt(2, 1)->addWidget(new WText(audience));
    table->elementAt(3, 1)->addWidget(new WText(WString::fromUTF8(description)));
    table->elementAt(4, 1)->addWidget(new WText(WString::fromUTF8(usage)));
    table->elementAt(5, 1)->addWidget(new WText(WString::fromUTF8(similar)));
    table->elementAt(6, 1)->addWidget(new WText(WString::fromUTF8(difference)));


    WVBoxLayout *layout = new WVBoxLayout();
    WContainerWidget* idea = new WContainerWidget();
    idea->setOverflow(WContainerWidget::OverflowAuto);
    idea->addWidget(table);

    layout->addWidget(idea);
    layout->setStretchFactor(idea, 0);

    m_dlg = new WDialog(WString::fromUTF8(title));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(55, Wt::WLength::Percentage),
                  Wt::WLength(55, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);
    m_dlg->contents()->setLayout(layout);
    m_dlg->show();
}

void MyIdeas::OnEditLinkClicked(WAnchor *sender)
{
    size_t rowId = 0;

    try {
        rowId = lexical_cast<size_t>(sender->attributeValue("dbid").toUTF8());
    } catch (...) {
    }

    delete m_dlg;
    m_dlg = NULL;

    m_dlg = new WDialog(m_lang->GetString("HOME_SUBMIT_IDEA_EDIT_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(768, Wt::WLength::Pixel),
                  Wt::WLength(64, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(false);

    IdeaSubmit *idea = new IdeaSubmit(m_cgiRoot, m_dlg, true, rowId);

    WSignalMapper<IdeaSubmit *> *ideaSignalMapper = new WSignalMapper<IdeaSubmit *>(this);;
    ideaSignalMapper->mapped().connect(this, &MyIdeas::OnEditIdeaCompleted);
    ideaSignalMapper->mapConnect(idea->EditCompletedHandler(), idea);

    m_dlg->show();
}

void MyIdeas::OnEditIdeaCompleted()
{
    m_dvTable->clear();
    m_dvTable->addWidget(GetIdeasTable());
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

Wt::WWidget *MyIdeas::GetIdeasTable()
{
    Div *root = new Div();

    WTable *table = new WTable(root);
    table->setStyleClass("tbl");

    table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_NO_TEXT")));
    table->elementAt(0, 1)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_TITLE_TEXT")));
    table->elementAt(0, 2)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_TYPE_TEXT")));
    table->elementAt(0, 3)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_TEXT")));
    table->elementAt(0, 4)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_DETAILS_TEXT")));
    table->elementAt(0, 5)->addWidget(new WText(m_lang->GetString("HOME_MY_IDEAS_EDIT_TEXT")));
    table->elementAt(0, 0)->setStyleClass("tblHeader");
    table->elementAt(0, 1)->setStyleClass("tblHeader");
    table->elementAt(0, 2)->setStyleClass("tblHeader");
    table->elementAt(0, 3)->setStyleClass("tblHeader");
    table->elementAt(0, 4)->setStyleClass("tblHeader");
    table->elementAt(0, 5)->setStyleClass("tblHeader");

    result r = m_db->Sql() << "SELECT rowid, email, title, "
                              "typegamesentertainment, typehygienehealth, typenewsinfo, typelearning, "
                              "typenonepresenceservices, typeonlineshopping, typeothers, typeotherstext, "
                              "audienceteens, audienceparents, audienceboys, audiencegirls, audiencewomen, "
                              "audiencemen, audiencechilds, audienceallfamilymembers FROM ["
                              + m_dbTables->Table("IDEAS")
                              + "] WHERE email=? ORDER BY rowid ASC;" << m_cgiEnv->LoggedInUserName;
    size_t i = 0;
    while(r.next()) {
        ++i;
        size_t rowid;
        string email;
        string title;
        int typegamesentertainment;
        int typehygienehealth;
        int typenewsinfo;
        int typelearning;
        int typenonepresenceservices;
        int typeonlineshopping;
        int typeothers;
        string typeotherstext;
        int audienceteens;
        int audienceparents;
        int audienceboys;
        int audiencegirls;
        int audiencewomen;
        int audiencemen;
        int audiencechilds;
        int audienceallfamilymembers;

        r >> rowid >> email >> title >> typegamesentertainment >> typehygienehealth
          >> typenewsinfo >> typelearning >> typenonepresenceservices >> typeonlineshopping
          >> typeothers >> typeotherstext >> audienceteens >> audienceparents >> audienceboys
          >> audiencegirls >> audiencewomen >> audiencemen >> audiencechilds >> audienceallfamilymembers;

        title = Crypto::Decrypt(title);

        table->elementAt(i, 0)->addWidget(new WText(lexical_cast<wstring>(i)));
        table->elementAt(i, 1)->addWidget(new WText(WString::fromUTF8(title)));

        wstring type;
        bool br = false;
        if (typegamesentertainment) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("HOME_MY_IDEAS_TYPE_GAME_ENTERTAINMENT");
            br = true;
        }
        if (typehygienehealth) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("HOME_MY_IDEAS_TYPE_HYGIENE_HEALTH");
            br = true;
        }
        if (typenewsinfo) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("HOME_MY_IDEAS_TYPE_NEWS_INFO");
            br = true;
        }
        if (typelearning) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("HOME_MY_IDEAS_TYPE_LEARNING");
            br = true;
        }
        if (typenonepresenceservices) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("HOME_MY_IDEAS_TYPE_NONE_PRESENCE_SERVICES");
            br = true;
        }
        if (typeonlineshopping) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("HOME_MY_IDEAS_TYPE_ONLINE_SHOPPING");
            br = true;
        }
        if (typeothers) {
            if (br)
                type += L"<br />";
            typeotherstext = Crypto::Decrypt(typeotherstext);
            type += m_lang->GetString("HOME_MY_IDEAS_TYPE_OTHER") + L": "
                    + WString::fromUTF8(typeotherstext).value();
            br = true;
        }

        wstring audience;
        br = false;
        if (audienceteens) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_TEENS");
            br = true;
        }
        if (audienceparents) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_PARENTS");
            br = true;
        }
        if (audienceboys) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_BOYS");
            br = true;
        }
        if (audiencegirls) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_GIRLS");
            br = true;
        }
        if (audiencewomen) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_WOMEN");
            br = true;
        }
        if (audiencemen) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_MEN");
            br = true;
        }
        if (audiencechilds) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_CHILDS");
            br = true;
        }
        if (audienceallfamilymembers) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("HOME_MY_IDEAS_AUDIENCE_ALL_FAMILY_MEMBERS");
            br = true;
        }

        table->elementAt(i, 2)->addWidget(new WText(type));
        table->elementAt(i, 3)->addWidget(new WText(audience));

        WAnchor *detailsAnchor = new WAnchor(WLink(), m_lang->GetString("HOME_MY_IDEAS_DETAILS_TEXT"));
        detailsAnchor->setStyleClass("link");

        table->elementAt(i, 4)->addWidget(detailsAnchor);

        WAnchor *editAnchor = new WAnchor(WLink(), m_lang->GetString("HOME_MY_IDEAS_EDIT_TEXT"));
        editAnchor->setStyleClass("link");

        table->elementAt(i, 5)->addWidget(editAnchor);


        detailsAnchor->setAttributeValue("dbid", boost::lexical_cast<wstring>(rowid));
        editAnchor->setAttributeValue("dbid", boost::lexical_cast<wstring>(rowid));

        WSignalMapper<WAnchor *> *detailsSignalMapper = new WSignalMapper<WAnchor *>(this);
        detailsSignalMapper->mapped().connect(this, &MyIdeas::OnDetailsLinkClicked);
        detailsSignalMapper->mapConnect(detailsAnchor->clicked(), detailsAnchor);

        WSignalMapper<WAnchor *> *editSignalMapper = new WSignalMapper<WAnchor *>(this);
        editSignalMapper->mapped().connect(this, &MyIdeas::OnEditLinkClicked);
        editSignalMapper->mapConnect(editAnchor->clicked(), editAnchor);
    }

    return root;
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *MyIdeas::Layout()
{
    Div *root = new Div("MyIdeas");

    m_dvTable = new Div(root);
    m_dvTable->addWidget(GetIdeasTable());

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


