/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WFileResource>
#include <Wt/WLink>
#include <Wt/WPushButton>
#include <Wt/WSignalMapper>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WWidget>
#include "cmsapps.hpp"
#include "base.hpp"
#include "crypto.hpp"


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

CmsApps::CmsApps(CgiRoot *cgi) : BaseWidget(cgi)
{
    m_dlg = NULL;
    m_dlgDetails = NULL;

    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_NONE_TEXT"));
    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_DIPLOMA_TEXT"));
    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_AD_TEXT"));
    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_BA_TEXT"));
    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_MA_TEXT"));
    m_educationLevel.push_back(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_PHD_TEXT"));

    m_type.push_back(m_lang->GetString("ROOT_CMSAPPS_TYPE_NONE_TEXT"));
    m_type.push_back(m_lang->GetString("ROOT_CMSAPPS_TYPE_GAME_ENTERTAINMENT_TEXT"));
    m_type.push_back(m_lang->GetString("ROOT_CMSAPPS_TYPE_ONLINE_GAME_TEXT"));
    m_type.push_back(m_lang->GetString("ROOT_CMSAPPS_TYPE_LEARN_INFO_TEXT"));
    m_type.push_back(m_lang->GetString("ROOT_CMSAPPS_TYPE_SERVICES_TEXT"));
    m_type.push_back(m_lang->GetString("ROOT_CMSAPPS_TYPE_TOOLS_TEXT"));

    m_displayType.push_back(m_lang->GetString("ROOT_CMSAPPS_DISPLAY_TYPE_NONE_TEXT"));
    m_displayType.push_back(m_lang->GetString("ROOT_CMSAPPS_DISPLAY_TYPE_FULLSCREEN_TEXT"));
    m_displayType.push_back(m_lang->GetString("ROOT_CMSAPPS_DISPLAY_TYPE_ON_CORNER_OF_SCREEN_TEXT"));
    m_displayType.push_back(m_lang->GetString("ROOT_CMSAPPS_DISPLAY_TYPE_ACTIVE_ON_SCREEN_TEXT"));


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

void CmsApps::OnShowAppsTableButtonPressed()
{
    WVBoxLayout *layout = new WVBoxLayout();
    WContainerWidget* dvTable = new WContainerWidget();
    dvTable->setOverflow(WContainerWidget::OverflowAuto);
    dvTable->addWidget(GetAppsTable());

    layout->addWidget(dvTable);
    layout->setStretchFactor(dvTable, 0);

    m_dlg = new WDialog(m_lang->GetString("ROOT_CMSAPPS_SHOW_APPS_TABLE_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(85, Wt::WLength::Percentage),
                  Wt::WLength(85, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);
    m_dlg->contents()->setLayout(layout);
    m_dlg->show();
}

void CmsApps::OnSubmitterLinkClicked(WAnchor *sender)
{
    string email(sender->attributeValue("dbid").toUTF8());
    //string submitter(sender->attributeValue("submitter").toUTF8());

    bool isIndividual = false;

    result rIndividual = m_db->Sql() << "SELECT firstname, lastname, phone, teammates FROM ["
                                        + m_dbTables->Table("USERS_INDIVIDUAL")
                                        + "] WHERE email=?;" << email << row;
    string individualFirstName;
    string individualLastName;
    string individualPhone;
    string individualTeammates;
    string individualAltPhone;
    string individualNationalCode;
    int individualEducationLevel = EIEDULEVEL_NONE;
    string individualEducationSubject;
    string individualWebsite;
    string companyName;
    string companyRegId;
    string companyPhone;
    string companyCEO;
    string companyAltPhone;
    string companyEconomicalCode;
    string companyFoundingYear;
    string companyAddr;
    string companyWebsite;
    string companyResumeURL;

    if (!rIndividual.empty()) {
        isIndividual = true;
        rIndividual >> individualFirstName >> individualLastName >> individualPhone >> individualTeammates;

        individualFirstName = Crypto::Decrypt(individualFirstName);
        individualLastName = Crypto::Decrypt(individualLastName);
        individualPhone = Crypto::Decrypt(individualPhone);
        individualTeammates = Crypto::Decrypt(individualTeammates);
        boost::algorithm::replace_all(individualTeammates, "\n", "<br />");


        result rIndividualExtra = m_db->Sql() << "SELECT altphone, nationalcode, educationlevel, educationsubject, website FROM ["
                                                 + m_dbTables->Table("USERS_INDIVIDUAL_EXTRA")
                                                 + "] WHERE email=?;" << email << row;
        if (!rIndividual.empty()) {
            rIndividualExtra >> individualAltPhone >> individualNationalCode >> individualEducationLevel >> individualEducationSubject >> individualWebsite;

            individualAltPhone = Crypto::Decrypt(individualAltPhone);
            individualNationalCode = Crypto::Decrypt(individualNationalCode);
            individualEducationSubject = Crypto::Decrypt(individualEducationSubject);
            individualWebsite = Crypto::Decrypt(individualWebsite);
        }
    } else {
        isIndividual = false;

        result rCompany = m_db->Sql() << "SELECT name, regid, phone, ceo FROM ["
                                         + m_dbTables->Table("USERS_COMPANY")
                                         + "] WHERE email=?;" << email << row;
        if (!rCompany.empty()) {
            rCompany >> companyName >> companyRegId >> companyPhone >> companyCEO;

            companyName = Crypto::Decrypt(companyName);
            companyRegId = Crypto::Decrypt(companyRegId);
            companyPhone = Crypto::Decrypt(companyPhone);
            companyCEO = Crypto::Decrypt(companyCEO);

            result rCompanyExtra = m_db->Sql() << "SELECT altphone, economicalcode, foundingyear, addr, website, resumeurl FROM ["
                                                  + m_dbTables->Table("USERS_COMPANY_EXTRA")
                                                  + "] WHERE email=?;" << email << row;
            if (!rCompanyExtra.empty()) {
                rCompanyExtra >> companyAltPhone >> companyEconomicalCode >> companyFoundingYear >> companyAddr >> companyWebsite >> companyResumeURL;

                companyAltPhone = Crypto::Decrypt(companyAltPhone);
                companyEconomicalCode = Crypto::Decrypt(companyEconomicalCode);
                companyFoundingYear = Crypto::Decrypt(companyFoundingYear);
                companyAddr = Crypto::Decrypt(companyAddr);
                companyWebsite = Crypto::Decrypt(companyWebsite);
                companyResumeURL = companyResumeURL;
            }
        }
    }

    WTable *table = new WTable();
    table->setStyleClass("tbl");

    string submitter;
    if (isIndividual) {
        submitter = individualFirstName + " " + individualLastName;

        table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_EMAIL_TEXT")));
        table->elementAt(1, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_FIRST_NAME_TEXT")));
        table->elementAt(2, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_LAST_NAME_TEXT")));
        table->elementAt(3, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_CONTACT_NUMBER_TEXT")));
        table->elementAt(4, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_ALTERNATIVE_CONTACT_NUMBER")));
        table->elementAt(5, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_TEAMMATES_TEXT")));
        table->elementAt(6, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_NATIONAL_CODE_TEXT")));
        table->elementAt(7, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_TEXT")));
        table->elementAt(8, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_SUBJECT_TEXT")));
        table->elementAt(9, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_INDIVIDUAL_WEBSITE_TEXT")));

        table->elementAt(0, 1)->addWidget(new WText(WString::fromUTF8(email)));
        table->elementAt(1, 1)->addWidget(new WText(WString::fromUTF8(individualFirstName)));
        table->elementAt(2, 1)->addWidget(new WText(WString::fromUTF8(individualLastName)));
        table->elementAt(3, 1)->addWidget(new WText(WString::fromUTF8(individualPhone)));
        table->elementAt(4, 1)->addWidget(new WText(WString::fromUTF8(individualAltPhone)));
        table->elementAt(5, 1)->addWidget(new WText(WString::fromUTF8(individualTeammates)));
        table->elementAt(6, 1)->addWidget(new WText(WString::fromUTF8(individualNationalCode)));
        table->elementAt(7, 1)->addWidget(new WText(m_educationLevel[individualEducationLevel]));
        table->elementAt(8, 1)->addWidget(new WText(WString::fromUTF8(individualEducationSubject)));
        table->elementAt(9, 1)->addWidget(new WText(WString::fromUTF8(individualWebsite)));
    } else {
        submitter = companyName;

        table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_EMAIL_TEXT")));
        table->elementAt(1, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_NAME_TEXT")));
        table->elementAt(2, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_REGISTERATION_ID_TEXT")));
        table->elementAt(3, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_PHONE_TEXT")));
        table->elementAt(4, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_ALTERNATIVE_PHONE_TEXT")));
        table->elementAt(5, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_CEO_NAME_TEXT")));
        table->elementAt(6, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_ECONOMICAL_CODE_TEXT")));
        table->elementAt(7, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_FOUNDING_YEAR_TEXT")));
        table->elementAt(8, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_ADDRESS_TEXT")));
        table->elementAt(9, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_WEBSITE_TEXT")));
        table->elementAt(10, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSUSERS_COMPANY_RESUME_TEXT")));

        table->elementAt(0, 1)->addWidget(new WText(WString::fromUTF8(email)));
        table->elementAt(1, 1)->addWidget(new WText(WString::fromUTF8(companyName)));
        table->elementAt(2, 1)->addWidget(new WText(WString::fromUTF8(companyRegId)));
        table->elementAt(3, 1)->addWidget(new WText(WString::fromUTF8(companyPhone)));
        table->elementAt(4, 1)->addWidget(new WText(WString::fromUTF8(companyAltPhone)));
        table->elementAt(5, 1)->addWidget(new WText(WString::fromUTF8(companyCEO)));
        table->elementAt(6, 1)->addWidget(new WText(WString::fromUTF8(companyEconomicalCode)));
        table->elementAt(7, 1)->addWidget(new WText(companyFoundingYear));
        table->elementAt(8, 1)->addWidget(new WText(WString::fromUTF8(companyAddr)));
        table->elementAt(9, 1)->addWidget(new WText(WString::fromUTF8(companyWebsite)));

        if (algorithm::trim_copy(companyResumeURL) != "") {
            Wt::WFileResource *resumeFile = new Wt::WFileResource("application/pdf", companyResumeURL);
            resumeFile->suggestFileName(email + "__" + Base::RESUME_FILE_NAME);
            Wt::WAnchor *resumeAnchor = new Wt::WAnchor(resumeFile, m_lang->GetString("ROOT_CMSUSERS_COMPANY_RESUME_TEXT"));
            table->elementAt(10, 1)->addWidget(resumeAnchor);
        }
    }

    m_dlg = new WDialog(WString::fromUTF8(submitter));
    m_dlg->setModal(true);
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);
    m_dlg->contents()->addWidget(table);
    m_dlg->show();
}

void CmsApps::OnDetailsLinkClicked(WAnchor *sender)
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


    table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_TITLE_TEXT")));
    table->elementAt(1, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_TYPE_TEXT")));
    table->elementAt(2, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_PLATFORM_TEXT")));
    table->elementAt(3, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_LANGUAGE_TEXT")));
    table->elementAt(4, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_DESCRIPTION_TEXT")));
    table->elementAt(5, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_SCREENSHOTS_TEXT")));
    table->elementAt(6, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_SCREENSHOTS_DESCRIPTION_TEXT")));
    table->elementAt(7, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_DISPLAY_TYPE_TEXT")));
    table->elementAt(8, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_TEXT")));
    table->elementAt(9, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_TEXT")));
    table->elementAt(10, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_REMOTE_BUTTONS_TEXT")));
    table->elementAt(11, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_APP_BINARY_TEXT")));

    table->elementAt(0, 1)->addWidget(new WText(WString::fromUTF8(title)));
    table->elementAt(1, 1)->addWidget(new WText(m_type[type]));

    wstring platform;
    bool br = false;
    if (platformhtml5) {
        if (br)
            platform += L"<br />";
        platform += m_lang->GetString("ROOT_CMSAPPS_PLATFORM_HTML5_TEXT");
        br = true;
    }
    if (platformjavascript) {
        if (br)
            platform += L"<br />";
        platform += m_lang->GetString("ROOT_CMSAPPS_PLATFORM_JAVASCRIPT_TEXT");
        br = true;
    }
    if (platformflash) {
        if (br)
            platform += L"<br />";
        platform += m_lang->GetString("ROOT_CMSAPPS_PLATFORM_FLASH_TEXT");
        br = true;
    }

    table->elementAt(2, 1)->addWidget(new WText(platform));
    table->elementAt(3, 1)->addWidget(new WText(WString::fromUTF8(language)));
    table->elementAt(4, 1)->addWidget(new WText(WString::fromUTF8(description)));

    WContainerWidget *screenshots = new WContainerWidget();

    if (algorithm::trim_copy(scr1st) != "") {
        Wt::WFileResource *scr1stFile = new Wt::WFileResource("image/jpeg", scr1st);
        scr1stFile->suggestFileName(email + "__" + scr1st.substr(scr1st.find_last_of("/") + 1));
        Wt::WAnchor *scr1stAnchor = new Wt::WAnchor(scr1stFile, m_lang->GetString("ROOT_CMSAPPS_SCREENSHOTS_1ST_TEXT"));
        screenshots->addWidget(scr1stAnchor);
        new WBreak(screenshots);
    }
    if (algorithm::trim_copy(scr2nd) != "") {
        Wt::WFileResource *scr2ndFile = new Wt::WFileResource("image/jpeg", scr2nd);
        scr2ndFile->suggestFileName(email + "__" + scr2nd.substr(scr2nd.find_last_of("/") + 1));
        Wt::WAnchor *scr2ndAnchor = new Wt::WAnchor(scr2ndFile, m_lang->GetString("ROOT_CMSAPPS_SCREENSHOTS_2ND_TEXT"));
        screenshots->addWidget(scr2ndAnchor);
        new WBreak(screenshots);
    }
    if (algorithm::trim_copy(scr3rd) != "") {
        Wt::WFileResource *scr3rdFile = new Wt::WFileResource("image/jpeg", scr3rd);
        scr3rdFile->suggestFileName(email + "__" + scr3rd.substr(scr3rd.find_last_of("/") + 1));
        Wt::WAnchor *scr3rdAnchor = new Wt::WAnchor(scr3rdFile, m_lang->GetString("ROOT_CMSAPPS_SCREENSHOTS_3RD_TEXT"));
        screenshots->addWidget(scr3rdAnchor);
        new WBreak(screenshots);
    }
    if (algorithm::trim_copy(scr4th) != "") {
        Wt::WFileResource *scr4thFile = new Wt::WFileResource("image/jpeg", scr4th);
        scr4thFile->suggestFileName(email + "__" + scr4th.substr(scr4th.find_last_of("/") + 1));
        Wt::WAnchor *scr4thAnchor = new Wt::WAnchor(scr4thFile, m_lang->GetString("ROOT_CMSAPPS_SCREENSHOTS_4TH_TEXT"));
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
        technicalDescription += m_lang->GetString("ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_SUPPORTS_IP_RANGE_TEXT");
        br = true;
    }
    if (tdpwprotected) {
        if (br)
            technicalDescription += L"<br />";
        technicalDescription += m_lang->GetString("ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_PWD_PROTECTED_TEXT");
        br = true;
    }
    if (tdhascookies) {
        if (br)
            technicalDescription += L"<br />";
        technicalDescription += m_lang->GetString("ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_HAS_COOKIES_TEXT");
        br = true;
    }
    if (tdautologin) {
        if (br)
            technicalDescription += L"<br />";
        technicalDescription += m_lang->GetString("ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_AUTO_LOGIN_TEXT");
        br = true;
    }
    if (tdhasfilm) {
        if (br)
            technicalDescription += L"<br />";
        technicalDescription += m_lang->GetString("ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_HAS_FILM_TEXT");
        br = true;
    }
    if (tdhasaudio) {
        if (br)
            technicalDescription += L"<br />";
        technicalDescription += m_lang->GetString("ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_HAS_AUDIO_TEXT");
        br = true;
    }

    table->elementAt(8, 1)->addWidget(new WText(technicalDescription));

    wstring avStatus;
    br = false;
    if (avnoaudiovideo) {
        if (br)
            avStatus += L"<br />";
        avStatus += m_lang->GetString("ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_NO_AUDIO_NO_VIDEO_TEXT");
        br = true;
    }
    if (avnobuffer) {
        if (br)
            avStatus += L"<br />";
        avStatus += m_lang->GetString("ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_NO_BUFFER_TEXT");
        br = true;
    }
    if (avwithplayback) {
        if (br)
            avStatus += L"<br />";
        avStatus += m_lang->GetString("ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_WITH_PLAYBACK_TEXT");
        br = true;
    }
    if (avfullscreenwithexit) {
        if (br)
            avStatus += L"<br />";
        avStatus += m_lang->GetString("ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_FULLSCREEN_WITH_EXIT_TEXT");
        br = true;
    }

    table->elementAt(9, 1)->addWidget(new WText(avStatus));

    wstring remoteButtons;
    br = false;
    if (remotenums) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("ROOT_CMSAPPS_REMOTE_BUTTONS_NUMS_TEXT");
        br = true;
    }
    if (remotearrows) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("ROOT_CMSAPPS_REMOTE_BUTTONS_ARROWS_TEXT");
        br = true;
    }
    if (remotetoolsinfo) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("ROOT_CMSAPPS_REMOTE_BUTTONS_TOOLS_INFO_TEXT");
        br = true;
    }
    if (remotereturnexit) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("ROOT_CMSAPPS_REMOTE_BUTTONS_RETURN_EXIT_TEXT");
        br = true;
    }
    if (remoteplayback) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("ROOT_CMSAPPS_REMOTE_BUTTONS_PLAYBACK_TEXT");
        br = true;
    }
    if (remotecolorstuning) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("ROOT_CMSAPPS_REMOTE_BUTTONS_COLORS_TUNING_TEXT");
        br = true;
    }
    if (remotevolumestuning) {
        if (br)
            remoteButtons += L"<br />";
        remoteButtons += m_lang->GetString("ROOT_CMSAPPS_REMOTE_BUTTONS_VOLUME_TUNING_TEXT");
        br = true;
    }

    table->elementAt(10, 1)->addWidget(new WText(remoteButtons));

    if (algorithm::trim_copy(binary) != "") {
        Wt::WFileResource *binaryFile = new Wt::WFileResource("application/zip", binary);
        binaryFile->suggestFileName(email + "__" + binary.substr(binary.find_last_of("/") + 1));
        Wt::WAnchor *binaryAnchor = new Wt::WAnchor(binaryFile, m_lang->GetString("ROOT_CMSAPPS_APP_BINARY_TEXT"));
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


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

Wt::WWidget *CmsApps::GetAppsTable()
{
    Div *root = new Div("AppsTable");

    WTable *table = new WTable(root);
    table->setStyleClass("tbl");

    table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_NO_TEXT")));
    table->elementAt(0, 1)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_TITLE_TEXT")));
    table->elementAt(0, 2)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_SUBMITTER_TEXT")));
    table->elementAt(0, 3)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_TYPE_TEXT")));
    table->elementAt(0, 4)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_PLATFORM_TEXT")));
    table->elementAt(0, 5)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_LANGUAGE_TEXT")));
    table->elementAt(0, 6)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPS_DETAILS_TEXT")));
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
                              + "] ORDER BY rowid ASC;";
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

        result rIndividual = m_db->Sql() << "SELECT firstname, lastname FROM ["
                                            + m_dbTables->Table("USERS_INDIVIDUAL")
                                            + "] WHERE email=?;" << email << row;

        string individualFirstName;
        string individualLastName;
        string companyName;
        string submitter;

        if (!rIndividual.empty()) {
            rIndividual >> individualFirstName >> individualLastName;
            individualFirstName = Crypto::Decrypt(individualFirstName);
            individualLastName = Crypto::Decrypt(individualLastName);
            submitter = individualFirstName + " " + individualLastName;
        } else {
            result rCompany = m_db->Sql() << "SELECT name FROM ["
                                             + m_dbTables->Table("USERS_COMPANY")
                                             + "] WHERE email=?;" << email << row;
            if (!rCompany.empty()) {
                rCompany >> companyName;
                companyName = Crypto::Decrypt(companyName);
                submitter = companyName;
            }
        }

        title = Crypto::Decrypt(title);
        language = Crypto::Decrypt(language);

        WAnchor *submitterAnchor = new WAnchor(WLink(), WString::fromUTF8(submitter));
        submitterAnchor->setStyleClass("link");

        table->elementAt(i, 0)->addWidget(new WText(lexical_cast<wstring>(i)));
        table->elementAt(i, 1)->addWidget(new WText(WString::fromUTF8(title)));
        table->elementAt(i, 2)->addWidget(submitterAnchor);
        table->elementAt(i, 3)->addWidget(new WText(m_type[type]));

        wstring platform;
        bool br = false;
        if (platformhtml5) {
            if (br)
                platform += L"<br />";
            platform += m_lang->GetString("ROOT_CMSAPPS_PLATFORM_HTML5_TEXT");
            br = true;
        }
        if (platformjavascript) {
            if (br)
                platform += L"<br />";
            platform += m_lang->GetString("ROOT_CMSAPPS_PLATFORM_JAVASCRIPT_TEXT");
            br = true;
        }
        if (platformflash) {
            if (br)
                platform += L"<br />";
            platform += m_lang->GetString("ROOT_CMSAPPS_PLATFORM_FLASH_TEXT");
            br = true;
        }

        table->elementAt(i, 4)->addWidget(new WText(platform));
        table->elementAt(i, 5)->addWidget(new WText(WString::fromUTF8(language)));

        WAnchor *detailsAnchor = new WAnchor(WLink(), m_lang->GetString("ROOT_CMSAPPS_DETAILS_TEXT"));
        detailsAnchor->setStyleClass("link");
        table->elementAt(i, 6)->addWidget(detailsAnchor);

        submitterAnchor->setAttributeValue("dbid", WString::fromUTF8(email));
        submitterAnchor->setAttributeValue("title", WString::fromUTF8(title));
        detailsAnchor->setAttributeValue("dbid", boost::lexical_cast<wstring>(rowid));
        detailsAnchor->setAttributeValue("submitter", WString::fromUTF8(submitter));

        WSignalMapper<WAnchor *> *submitterSignalMapper = new WSignalMapper<WAnchor *>(this);
        submitterSignalMapper->mapped().connect(this, &CmsApps::OnSubmitterLinkClicked);
        submitterSignalMapper->mapConnect(submitterAnchor->clicked(), submitterAnchor);

        WSignalMapper<WAnchor *> *detailsSignalMapper = new WSignalMapper<WAnchor *>(this);
        detailsSignalMapper->mapped().connect(this, &CmsApps::OnDetailsLinkClicked);
        detailsSignalMapper->mapConnect(detailsAnchor->clicked(), detailsAnchor);
    }

    return root;
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *CmsApps::Layout()
{
    Div *root = new Div("CmsApps");

    WPushButton *showAppsTableButton = new WPushButton(m_lang->GetString("ROOT_CMSAPPS_SHOW_APPS_TABLE_BUTTON"), root);
    showAppsTableButton->setStyleClass("formButtonNoWidth");

    showAppsTableButton->clicked().connect(this, &CmsApps::OnShowAppsTableButtonPressed);

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


