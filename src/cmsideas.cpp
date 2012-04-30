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
#include "cmsideas.hpp"
#include "base.hpp"
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

CmsIdeas::CmsIdeas(CgiRoot *cgi) : BaseWidget(cgi)
{
    m_dlg = NULL;
    m_dlgDetails = NULL;

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

void CmsIdeas::OnShowIdeasTableButtonPressed()
{
    WVBoxLayout *layout = new WVBoxLayout();
    WContainerWidget* dvTable = new WContainerWidget();
    dvTable->setOverflow(WContainerWidget::OverflowAuto);
    dvTable->addWidget(GetIdeasTable());

    layout->addWidget(dvTable);
    layout->setStretchFactor(dvTable, 0);

    m_dlg = new WDialog(m_lang->GetString("ROOT_CMSIDEAS_SHOW_IDEAS_TABLE_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(85, Wt::WLength::Percentage),
                  Wt::WLength(85, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);
    m_dlg->contents()->setLayout(layout);
    m_dlg->show();
}

void CmsIdeas::OnSubmitterLinkClicked(WAnchor *sender)
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

void CmsIdeas::OnDetailsLinkClicked(WAnchor *sender)
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
          >> typeothers >> typeotherstext >> description>> audienceteens >> audienceparents >> audienceboys
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

    table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_TITLE_TEXT")));
    table->elementAt(1, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_TYPE_TEXT")));
    table->elementAt(2, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_TEXT")));
    table->elementAt(3, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_DESCRIPTION_TEXT")));
    table->elementAt(4, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_USAGE_TEXT")));
    table->elementAt(5, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_SIMILAR_TEXT")));
    table->elementAt(6, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_DIFFERENCE_TEXT")));

    table->elementAt(0, 1)->addWidget(new WText(WString::fromUTF8(title)));

    wstring type;
    bool br = false;
    if (typegamesentertainment) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_GAME_ENTERTAINMENT");
        br = true;
    }
    if (typehygienehealth) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_HYGIENE_HEALTH");
        br = true;
    }
    if (typenewsinfo) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_NEWS_INFO");
        br = true;
    }
    if (typelearning) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_LEARNING");
        br = true;
    }
    if (typenonepresenceservices) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_NONE_PRESENCE_SERVICES");
        br = true;
    }
    if (typeonlineshopping) {
        if (br)
            type += L"<br />";
        type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_ONLINE_SHOPPING");
        br = true;
    }
    if (typeothers) {
        if (br)
            type += L"<br />";
        typeotherstext = Crypto::Decrypt(typeotherstext);
        type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_OTHER") + L": "
                + WString::fromUTF8(typeotherstext).value();
        br = true;
    }


    wstring audience;
    br = false;
    if (audienceteens) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_TEENS");
        br = true;
    }
    if (audienceparents) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_PARENTS");
        br = true;
    }
    if (audienceboys) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_BOYS");
        br = true;
    }
    if (audiencegirls) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_GIRLS");
        br = true;
    }
    if (audiencewomen) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_WOMEN");
        br = true;
    }
    if (audiencemen) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_MEN");
        br = true;
    }
    if (audiencechilds) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_CHILDS");
        br = true;
    }
    if (audienceallfamilymembers) {
        if (br)
            audience += L"<br />";
        audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_ALL_FAMILY_MEMBERS");
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


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

Wt::WWidget *CmsIdeas::GetIdeasTable()
{
    Div *root = new Div("IdeasTable");

    WTable *table = new WTable(root);
    table->setStyleClass("tbl");

    table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_NO_TEXT")));
    table->elementAt(0, 1)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_TITLE_TEXT")));
    table->elementAt(0, 2)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_SUBMITTER_TEXT")));
    table->elementAt(0, 3)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_TYPE_TEXT")));
    table->elementAt(0, 4)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_TEXT")));
    table->elementAt(0, 5)->addWidget(new WText(m_lang->GetString("ROOT_CMSIDEAS_DETAILS_TEXT")));
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
                              + "] ORDER BY rowid ASC;";
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

        WAnchor *submitterAnchor = new WAnchor(WLink(), WString::fromUTF8(submitter));
        submitterAnchor->setStyleClass("link");

        table->elementAt(i, 0)->addWidget(new WText(lexical_cast<wstring>(i)));
        table->elementAt(i, 1)->addWidget(new WText(WString::fromUTF8(title)));
        table->elementAt(i, 2)->addWidget(submitterAnchor);

        wstring type;
        bool br = false;
        if (typegamesentertainment) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_GAME_ENTERTAINMENT");
            br = true;
        }
        if (typehygienehealth) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_HYGIENE_HEALTH");
            br = true;
        }
        if (typenewsinfo) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_NEWS_INFO");
            br = true;
        }
        if (typelearning) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_LEARNING");
            br = true;
        }
        if (typenonepresenceservices) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_NONE_PRESENCE_SERVICES");
            br = true;
        }
        if (typeonlineshopping) {
            if (br)
                type += L"<br />";
            type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_ONLINE_SHOPPING");
            br = true;
        }
        if (typeothers) {
            if (br)
                type += L"<br />";
            typeotherstext = Crypto::Decrypt(typeotherstext);
            type += m_lang->GetString("ROOT_CMSIDEAS_TYPE_OTHER") + L": "
                    + WString::fromUTF8(typeotherstext).value();
            br = true;
        }

        wstring audience;
        br = false;
        if (audienceteens) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_TEENS");
            br = true;
        }
        if (audienceparents) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_PARENTS");
            br = true;
        }
        if (audienceboys) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_BOYS");
            br = true;
        }
        if (audiencegirls) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_GIRLS");
            br = true;
        }
        if (audiencewomen) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_WOMEN");
            br = true;
        }
        if (audiencemen) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_MEN");
            br = true;
        }
        if (audiencechilds) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_CHILDS");
            br = true;
        }
        if (audienceallfamilymembers) {
            if (br)
                audience += L"<br />";
            audience += m_lang->GetString("ROOT_CMSIDEAS_AUDIENCE_ALL_FAMILY_MEMBERS");
            br = true;
        }

        table->elementAt(i, 3)->addWidget(new WText(type));
        table->elementAt(i, 4)->addWidget(new WText(audience));

        WAnchor *detailsAnchor = new WAnchor(WLink(), m_lang->GetString("ROOT_CMSIDEAS_DETAILS_TEXT"));
        detailsAnchor->setStyleClass("link");

        table->elementAt(i, 5)->addWidget(detailsAnchor);

        submitterAnchor->setAttributeValue("dbid", WString::fromUTF8(email));
        submitterAnchor->setAttributeValue("title", WString::fromUTF8(title));
        detailsAnchor->setAttributeValue("dbid", boost::lexical_cast<wstring>(rowid));
        detailsAnchor->setAttributeValue("submitter", WString::fromUTF8(submitter));

        WSignalMapper<WAnchor *> *submitterSignalMapper = new WSignalMapper<WAnchor *>(this);
        submitterSignalMapper->mapped().connect(this, &CmsIdeas::OnSubmitterLinkClicked);
        submitterSignalMapper->mapConnect(submitterAnchor->clicked(), submitterAnchor);

        WSignalMapper<WAnchor *> *detailsSignalMapper = new WSignalMapper<WAnchor *>(this);
        detailsSignalMapper->mapped().connect(this, &CmsIdeas::OnDetailsLinkClicked);
        detailsSignalMapper->mapConnect(detailsAnchor->clicked(), detailsAnchor);
    }

    return root;
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *CmsIdeas::Layout()
{
    Div *root = new Div("CmsIdeas");

    WPushButton *showIdeasTableButton = new WPushButton(m_lang->GetString("ROOT_CMSIDEAS_SHOW_IDEAS_TABLE_BUTTON"), root);
    showIdeasTableButton->setStyleClass("formButtonNoWidth");

    showIdeasTableButton->clicked().connect(this, &CmsIdeas::OnShowIdeasTableButtonPressed);

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


