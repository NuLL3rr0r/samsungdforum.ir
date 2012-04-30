/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/algorithm/string.hpp>
#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WFileResource>
#include <Wt/WSignalMapper>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WWidget>
#include "myapps.hpp"
#include "appsubmit.hpp"
#include "crypto.hpp"
#include "db.hpp"


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

MyApps::MyApps(CgiRoot *cgi, Wt::WDialog *parentDialog) : BaseWidget(cgi),
    m_dlgParent(parentDialog)
{
    m_dlg = NULL;
    m_dlgDetails = NULL;

    m_type.push_back(m_lang->GetString("HOME_MY_APPS_TYPE_NONE_TEXT"));
    m_type.push_back(m_lang->GetString("HOME_MY_APPS_TYPE_GAME_ENTERTAINMENT_TEXT"));
    m_type.push_back(m_lang->GetString("HOME_MY_APPS_TYPE_ONLINE_GAME_TEXT"));
    m_type.push_back(m_lang->GetString("HOME_MY_APPS_TYPE_LEARN_INFO_TEXT"));
    m_type.push_back(m_lang->GetString("HOME_MY_APPS_TYPE_SERVICES_TEXT"));
    m_type.push_back(m_lang->GetString("HOME_MY_APPS_TYPE_TOOLS_TEXT"));

    m_displayType.push_back(m_lang->GetString("HOME_MY_APPS_DISPLAY_TYPE_NONE_TEXT"));
    m_displayType.push_back(m_lang->GetString("HOME_MY_APPS_DISPLAY_TYPE_FULLSCREEN_TEXT"));
    m_displayType.push_back(m_lang->GetString("HOME_MY_APPS_DISPLAY_TYPE_ON_CORNER_OF_SCREEN_TEXT"));
    m_displayType.push_back(m_lang->GetString("HOME_MY_APPS_DISPLAY_TYPE_ACTIVE_ON_SCREEN_TEXT"));

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

void MyApps::OnDetailsLinkClicked(WAnchor *sender)
{
    string rowId(sender->attributeValue("dbid").toUTF8());

    string email;
    string title;
    int type;
    string description;
    int displaytype;
    int platformhtml5;
    int platformjavascript;
    int platformflash;
    int tdsupportiprange;
    int tdpwprotected;
    int tdhascookies;
    int tdautologin;
    int tdhasfilm;
    int tdhasaudio;
    int avnoaudiovideo;
    int avnobuffer;
    int avwithplayback;
    int avfullscreenwithexit;
    string language;
    string scr1st;
    string scr2nd;
    string scr3rd;
    string scr4th;
    string scrdescription;
    int remotenums;
    int remotearrows;
    int remotetoolsinfo;
    int remotereturnexit;
    int remoteplayback;
    int remotecolorstuning;
    int remotevolumestuning;
    string binary;

    result r = m_db->Sql() << "SELECT email, title, type, description, displaytype, "
                              "platformhtml5, platformjavascript, platformflash, "
                              "tdsupportiprange, tdpwprotected, tdhascookies, tdautologin, tdhasfilm, tdhasaudio, "
                              "avnoaudiovideo, avnobuffer, avwithplayback, avfullscreenwithexit, "
                              "language, scr1st, scr2nd, scr3rd, scr4th, scrdescription, "
                              "remotenums, remotearrows, remotetoolsinfo, remotereturnexit, remoteplayback, "
                              "remotecolorstuning, remotevolumestuning, "
                              "binary FROM [" + m_dbTables->Table("APPS")
                              + "] WHERE rowid=?;" << rowId << row;
    if (!r.empty()) {
        r >> email >> title >> type >> description >> displaytype
          >> platformhtml5 >> platformjavascript >> platformflash
          >> tdsupportiprange >> tdpwprotected >> tdhascookies >> tdautologin >> tdhasfilm >> tdhasaudio
          >> avnoaudiovideo >> avnobuffer >> avwithplayback >> avfullscreenwithexit
          >> language >> scr1st >> scr2nd >> scr3rd >> scr4th >> scrdescription
          >> remotenums >> remotearrows >> remotetoolsinfo >> remotereturnexit >> remoteplayback
          >> remotecolorstuning >> remotevolumestuning
          >> binary;

        title = Crypto::Decrypt(title);
        description = Crypto::Decrypt(description);
        language = Crypto::Decrypt(language);
        scrdescription = Crypto::Decrypt(scrdescription);

        boost::algorithm::replace_all(description, "\n", "<br />");
        boost::algorithm::replace_all(scrdescription, "\n", "<br />");
    }

    WTable *table = new WTable();
    table->setStyleClass("tbl");


    table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_TITLE_TEXT")));
    table->elementAt(1, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_TYPE_TEXT")));
    table->elementAt(2, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_PLATFORM_TEXT")));
    table->elementAt(3, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_LANGUAGE_TEXT")));
    table->elementAt(4, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_DESCRIPTION_TEXT")));
    table->elementAt(5, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_SCREENSHOTS_TEXT")));
    table->elementAt(6, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_SCREENSHOTS_DESCRIPTION_TEXT")));
    table->elementAt(7, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_DISPLAY_TYPE_TEXT")));
    table->elementAt(8, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_TECHNICAL_DESCRIPTION_TEXT")));
    table->elementAt(9, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_AUDIO_VIDEO_STATUS_TEXT")));
    table->elementAt(10, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_REMOTE_BUTTONS_TEXT")));
    table->elementAt(11, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_APP_BINARY_TEXT")));

    table->elementAt(0, 1)->addWidget(new WText(WString::fromUTF8(title)));
    table->elementAt(1, 1)->addWidget(new WText(m_type[type]));

    wstring platform;
    bool br = false;
    if (platformhtml5) {
        if (br)
            platform += L"<br />";
        platform += m_lang->GetString("HOME_MY_APPS_PLATFORM_HTML5_TEXT");
        br = true;
    }
    if (platformjavascript) {
        if (br)
            platform += L"<br />";
        platform += m_lang->GetString("HOME_MY_APPS_PLATFORM_JAVASCRIPT_TEXT");
        br = true;
    }
    if (platformflash) {
        if (br)
            platform += L"<br />";
        platform += m_lang->GetString("HOME_MY_APPS_PLATFORM_FLASH_TEXT");
        br = true;
    }

    table->elementAt(2, 1)->addWidget(new WText(platform));
    table->elementAt(3, 1)->addWidget(new WText(WString::fromUTF8(language)));
    table->elementAt(4, 1)->addWidget(new WText(WString::fromUTF8(description)));

    WContainerWidget *screenshots = new WContainerWidget();

    if (algorithm::trim_copy(scr1st) != "") {
        Wt::WFileResource *scr1stFile = new Wt::WFileResource("image/jpeg", scr1st);
        scr1stFile->suggestFileName(email + "__" + scr1st.substr(scr1st.find_last_of("/") + 1));
        Wt::WAnchor *scr1stAnchor = new Wt::WAnchor(scr1stFile, m_lang->GetString("HOME_MY_APPS_SCREENSHOTS_1ST_TEXT"));
        screenshots->addWidget(scr1stAnchor);
        new WBreak(screenshots);
    }
    if (algorithm::trim_copy(scr2nd) != "") {
        Wt::WFileResource *scr2ndFile = new Wt::WFileResource("image/jpeg", scr2nd);
        scr2ndFile->suggestFileName(email + "__" + scr2nd.substr(scr2nd.find_last_of("/") + 1));
        Wt::WAnchor *scr2ndAnchor = new Wt::WAnchor(scr2ndFile, m_lang->GetString("HOME_MY_APPS_SCREENSHOTS_2ND_TEXT"));
        screenshots->addWidget(scr2ndAnchor);
        new WBreak(screenshots);
    }
    if (algorithm::trim_copy(scr3rd) != "") {
        Wt::WFileResource *scr3rdFile = new Wt::WFileResource("image/jpeg", scr3rd);
        scr3rdFile->suggestFileName(email + "__" + scr3rd.substr(scr3rd.find_last_of("/") + 1));
        Wt::WAnchor *scr3rdAnchor = new Wt::WAnchor(scr3rdFile, m_lang->GetString("HOME_MY_APPS_SCREENSHOTS_3RD_TEXT"));
        screenshots->addWidget(scr3rdAnchor);
        new WBreak(screenshots);
    }
    if (algorithm::trim_copy(scr4th) != "") {
        Wt::WFileResource *scr4thFile = new Wt::WFileResource("image/jpeg", scr4th);
        scr4thFile->suggestFileName(email + "__" + scr4th.substr(scr4th.find_last_of("/") + 1));
        Wt::WAnchor *scr4thAnchor = new Wt::WAnchor(scr4thFile, m_lang->GetString("HOME_MY_APPS_SCREENSHOTS_4TH_TEXT"));
        screenshots->addWidget(scr4thAnchor);
        new WBreak(screenshots);
    }

    table->elementAt(5, 1)->addWidget(screenshots);

    table->elementAt(6, 1)->addWidget(new WText(WString::fromUTF8(scrdescription)));
    table->elementAt(7, 1)->addWidget(new WText(m_displayType[displaytype]));

    wstring technicalDescription;
    br = false;
    if (tdsupportiprange) {
        if (br)
            technicalDescription += L"<br />";
        technicalDescription += m_lang->GetString("HOME_MY_APPS_TECHNICAL_DESCRIPTION_SUPPORTS_IP_RANGE_TEXT");
        br = true;
    }
    if (tdpwprotected) {
        if (br)
            technicalDescription += L"<br />";
        technicalDescription += m_lang->GetString("HOME_MY_APPS_TECHNICAL_DESCRIPTION_PWD_PROTECTED_TEXT");
        br = true;
    }
    if (tdhascookies) {
        if (br)
            technicalDescription += L"<br />";
        technicalDescription += m_lang->GetString("HOME_MY_APPS_TECHNICAL_DESCRIPTION_HAS_COOKIES_TEXT");
        br = true;
    }
    if (tdautologin) {
        if (br)
            technicalDescription += L"<br />";
        technicalDescription += m_lang->GetString("HOME_MY_APPS_TECHNICAL_DESCRIPTION_AUTO_LOGIN_TEXT");
        br = true;
    }
    if (tdhasfilm) {
        if (br)
            technicalDescription += L"<br />";
        technicalDescription += m_lang->GetString("HOME_MY_APPS_TECHNICAL_DESCRIPTION_HAS_FILM_TEXT");
        br = true;
    }
    if (tdhasaudio) {
        if (br)
            technicalDescription += L"<br />";
        technicalDescription += m_lang->GetString("HOME_MY_APPS_TECHNICAL_DESCRIPTION_HAS_AUDIO_TEXT");
        br = true;
    }

    table->elementAt(8, 1)->addWidget(new WText(technicalDescription));

    wstring avStatus;
    br = false;
    if (avnoaudiovideo) {
        if (br)
            avStatus += L"<br />";
        avStatus += m_lang->GetString("HOME_MY_APPS_AUDIO_VIDEO_STATUS_NO_AUDIO_NO_VIDEO_TEXT");
        br = true;
    }
    if (avnobuffer) {
        if (br)
            avStatus += L"<br />";
        avStatus += m_lang->GetString("HOME_MY_APPS_AUDIO_VIDEO_STATUS_NO_BUFFER_TEXT");
        br = true;
    }
    if (avwithplayback) {
        if (br)
            avStatus += L"<br />";
        avStatus += m_lang->GetString("HOME_MY_APPS_AUDIO_VIDEO_STATUS_WITH_PLAYBACK_TEXT");
        br = true;
    }
    if (avfullscreenwithexit) {
        if (br)
            avStatus += L"<br />";
        avStatus += m_lang->GetString("HOME_MY_APPS_AUDIO_VIDEO_STATUS_FULLSCREEN_WITH_EXIT_TEXT");
        br = true;
    }

    table->elementAt(9, 1)->addWidget(new WText(avStatus));

    wstring remoteButtons;
    br = false;
    if (remotenums) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("HOME_MY_APPS_REMOTE_BUTTONS_NUMS_TEXT");
        br = true;
    }
    if (remotearrows) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("HOME_MY_APPS_REMOTE_BUTTONS_ARROWS_TEXT");
        br = true;
    }
    if (remotetoolsinfo) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("HOME_MY_APPS_REMOTE_BUTTONS_TOOLS_INFO_TEXT");
        br = true;
    }
    if (remotereturnexit) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("HOME_MY_APPS_REMOTE_BUTTONS_RETURN_EXIT_TEXT");
        br = true;
    }
    if (remoteplayback) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("HOME_MY_APPS_REMOTE_BUTTONS_PLAYBACK_TEXT");
        br = true;
    }
    if (remotecolorstuning) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("HOME_MY_APPS_REMOTE_BUTTONS_COLORS_TUNING_TEXT");
        br = true;
    }
    if (remotevolumestuning) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("HOME_MY_APPS_REMOTE_BUTTONS_VOLUME_TUNING_TEXT");
        br = true;
    }

    table->elementAt(10, 1)->addWidget(new WText(remoteButtons));

    if (algorithm::trim_copy(binary) != "") {
        Wt::WFileResource *binaryFile = new Wt::WFileResource("application/zip", binary);
        binaryFile->suggestFileName(email + "__" + binary.substr(binary.find_last_of("/") + 1));
        Wt::WAnchor *binaryAnchor = new Wt::WAnchor(binaryFile, m_lang->GetString("HOME_MY_APPS_APP_BINARY_TEXT"));
        table->elementAt(11, 1)->addWidget(binaryAnchor);
    }

    WVBoxLayout *layout = new WVBoxLayout();
    WContainerWidget* app = new WContainerWidget();
    app->setOverflow(WContainerWidget::OverflowAuto);
    app->addWidget(table);

    layout->addWidget(app);
    layout->setStretchFactor(app, 0);

    m_dlg = new WDialog(WString::fromUTF8(title));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(55, Wt::WLength::Percentage),
                  Wt::WLength(55, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);
    m_dlg->contents()->setLayout(layout);
    m_dlg->show();
}

void MyApps::OnEditLinkClicked(WAnchor *sender)
{
    size_t rowId = 0;

    try {
        rowId = lexical_cast<size_t>(sender->attributeValue("dbid").toUTF8());
    } catch (...) {
    }

    delete m_dlg;
    m_dlg = NULL;

    m_dlg = new WDialog(m_lang->GetString("HOME_SUBMIT_APP_EDIT_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(768, Wt::WLength::Pixel),
                  Wt::WLength(64, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(false);

    AppSubmit *app = new AppSubmit(m_cgiRoot, m_dlg, true, rowId);

    WSignalMapper<AppSubmit *> *appSignalMapper = new WSignalMapper<AppSubmit *>(this);;
    appSignalMapper->mapped().connect(this, &MyApps::OnEditAppCompleted);
    appSignalMapper->mapConnect(app->EditCompletedHandler(), app);

    m_dlg->show();
}

void MyApps::OnEditAppCompleted()
{
    m_dvTable->clear();
    m_dvTable->addWidget(GetAppsTable());
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

Wt::WWidget *MyApps::GetAppsTable()
{
    Div *root = new Div();

    WTable *table = new WTable(root);
    table->setStyleClass("tbl");

    table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_NO_TEXT")));
    table->elementAt(0, 1)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_TITLE_TEXT")));
    table->elementAt(0, 2)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_TYPE_TEXT")));
    table->elementAt(0, 3)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_PLATFORM_TEXT")));
    table->elementAt(0, 4)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_LANGUAGE_TEXT")));
    table->elementAt(0, 5)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_DETAILS_TEXT")));
    table->elementAt(0, 6)->addWidget(new WText(m_lang->GetString("HOME_MY_APPS_EDIT_TEXT")));
    table->elementAt(0, 0)->setStyleClass("tblHeader");
    table->elementAt(0, 1)->setStyleClass("tblHeader");
    table->elementAt(0, 2)->setStyleClass("tblHeader");
    table->elementAt(0, 3)->setStyleClass("tblHeader");
    table->elementAt(0, 4)->setStyleClass("tblHeader");
    table->elementAt(0, 5)->setStyleClass("tblHeader");
    table->elementAt(0, 6)->setStyleClass("tblHeader");

    result r = m_db->Sql() << "SELECT rowid, email, title, "
                              "type, platformhtml5, platformjavascript, platformflash, "
                              "language FROM ["
                              + m_dbTables->Table("APPS")
                              + "] WHERE email=? ORDER BY rowid ASC;" << m_cgiEnv->LoggedInUserName;
    size_t i = 0;
    while(r.next()) {
        ++i;
        size_t rowid;
        string email;
        string title;
        int type;
        int platformhtml5;
        int platformjavascript;
        int platformflash;
        string language;

        r >> rowid >> email >> title
          >> type >> platformhtml5 >> platformjavascript >> platformflash
          >> language;

        title = Crypto::Decrypt(title);
        language = Crypto::Decrypt(language);

        table->elementAt(i, 0)->addWidget(new WText(lexical_cast<wstring>(i)));
        table->elementAt(i, 1)->addWidget(new WText(WString::fromUTF8(title)));
        table->elementAt(i, 2)->addWidget(new WText(m_type[type]));

        wstring platform;
        bool br = false;
        if (platformhtml5) {
            if (br)
                platform += L"<br />";
            platform += m_lang->GetString("HOME_MY_APPS_PLATFORM_HTML5_TEXT");
            br = true;
        }
        if (platformjavascript) {
            if (br)
                platform += L"<br />";
            platform += m_lang->GetString("HOME_MY_APPS_PLATFORM_JAVASCRIPT_TEXT");
            br = true;
        }
        if (platformflash) {
            if (br)
                platform += L"<br />";
            platform += m_lang->GetString("HOME_MY_APPS_PLATFORM_FLASH_TEXT");
            br = true;
        }

        table->elementAt(i, 3)->addWidget(new WText(platform));
        table->elementAt(i, 4)->addWidget(new WText(WString::fromUTF8(language)));

        WAnchor *detailsAnchor = new WAnchor(WLink(), m_lang->GetString("HOME_MY_APPS_DETAILS_TEXT"));
        detailsAnchor->setStyleClass("link");
        table->elementAt(i, 5)->addWidget(detailsAnchor);

        WAnchor *editAnchor = new WAnchor(WLink(), m_lang->GetString("HOME_MY_APPS_EDIT_TEXT"));
        editAnchor->setStyleClass("link");
        table->elementAt(i, 6)->addWidget(editAnchor);

        detailsAnchor->setAttributeValue("dbid", boost::lexical_cast<wstring>(rowid));
        editAnchor->setAttributeValue("dbid", boost::lexical_cast<wstring>(rowid));

        WSignalMapper<WAnchor *> *detailsSignalMapper = new WSignalMapper<WAnchor *>(this);
        detailsSignalMapper->mapped().connect(this, &MyApps::OnDetailsLinkClicked);
        detailsSignalMapper->mapConnect(detailsAnchor->clicked(), detailsAnchor);

        WSignalMapper<WAnchor *> *editSignalMapper = new WSignalMapper<WAnchor *>(this);
        editSignalMapper->mapped().connect(this, &MyApps::OnEditLinkClicked);
        editSignalMapper->mapConnect(editAnchor->clicked(), editAnchor);
    }

    return root;
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *MyApps::Layout()
{
    Div *root = new Div("MyApps");

    m_dvTable = new Div(root);
    m_dvTable->addWidget(GetAppsTable());

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


