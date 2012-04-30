/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WFileResource>
#include <Wt/WPushButton>
#include <Wt/WSignalMapper>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WWidget>
#include "cmsappointments.hpp"
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

CmsAppointments::CmsAppointments(CgiRoot *cgi) : BaseWidget(cgi)
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

void CmsAppointments::OnShowAppointmentsTableButtonPressed()
{
    WVBoxLayout *layout = new WVBoxLayout();
    WContainerWidget* dvTable = new WContainerWidget();
    dvTable->setOverflow(WContainerWidget::OverflowAuto);
    dvTable->addWidget(GetAppointmentsTable());

    layout->addWidget(dvTable);
    layout->setStretchFactor(dvTable, 0);

    m_dlg = new WDialog(m_lang->GetString("ROOT_CMSAPPOINTMENTS_SHOW_APPOINTMENTS_TABLE_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->resize(Wt::WLength(85, Wt::WLength::Percentage),
                  Wt::WLength(85, Wt::WLength::Percentage));
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);
    m_dlg->contents()->setLayout(layout);
    m_dlg->show();
}

void CmsAppointments::OnSuppliantLinkClicked(WAnchor *sender)
{
    string email(sender->attributeValue("dbid").toUTF8());
    //string suppliant(sender->attributeValue("suppliant").toUTF8());

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

    string suppliant;
    if (isIndividual) {
        suppliant = individualFirstName + " " + individualLastName;

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
        suppliant = companyName;

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

    m_dlg = new WDialog(WString::fromUTF8(suppliant));
    m_dlg->setModal(true);
    m_dlg->rejectWhenEscapePressed();
    m_dlg->setClosable(true);
    m_dlg->contents()->addWidget(table);
    m_dlg->show();
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

WWidget *CmsAppointments::GetAppointmentsTable()
{
    Div *root = new Div("AppointmentsTable");

    WTable *table = new WTable(root);
    table->setStyleClass("tbl");

    table->elementAt(0, 0)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPOINTMENTS_NO_TEXT")));
    table->elementAt(0, 1)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPOINTMENTS_SUPPLIANT_TEXT")));
    table->elementAt(0, 2)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPOINTMENTS_DATE_TEXT")));
    table->elementAt(0, 3)->addWidget(new WText(m_lang->GetString("ROOT_CMSAPPOINTMENTS_HOUR_TEXT")));
    table->elementAt(0, 0)->setStyleClass("tblHeader");
    table->elementAt(0, 1)->setStyleClass("tblHeader");
    table->elementAt(0, 2)->setStyleClass("tblHeader");
    table->elementAt(0, 3)->setStyleClass("tblHeader");

    result r = m_db->Sql() << "SELECT rowid, email, "
                              "year, month, day, hour FROM ["
                              + m_dbTables->Table("APPOINTMENTS")
                              + "] ORDER BY year DESC, month DESC, day DESC, hour DESC;";
    size_t i = 0;
    while(r.next()) {
        ++i;
        size_t rowid;
        string email;
        int year;
        int month;
        int day;
        int hour;

        r >> rowid >> email >> year >> month >> day >> hour;

        wstring yearFormatted = boost::lexical_cast<wstring>(year);
        wstring monthFormatted = month < 10 ? L"0" + boost::lexical_cast<wstring>(month) : boost::lexical_cast<wstring>(month);
        wstring dayFormatted = day < 10 ? L"0" + boost::lexical_cast<wstring>(day) : boost::lexical_cast<wstring>(day);
        wstring hourFormatted = hour < 10 ? L"0" + boost::lexical_cast<wstring>(hour) : boost::lexical_cast<wstring>(hour);

        result rIndividual = m_db->Sql() << "SELECT firstname, lastname FROM ["
                                            + m_dbTables->Table("USERS_INDIVIDUAL")
                                            + "] WHERE email=?;" << email << row;

        string individualFirstName;
        string individualLastName;
        string companyName;
        string suppliant;

        if (!rIndividual.empty()) {
            rIndividual >> individualFirstName >> individualLastName;
            individualFirstName = Crypto::Decrypt(individualFirstName);
            individualLastName = Crypto::Decrypt(individualLastName);
            suppliant = individualFirstName + " " + individualLastName;
        } else {
            result rCompany = m_db->Sql() << "SELECT name FROM ["
                                             + m_dbTables->Table("USERS_COMPANY")
                                             + "] WHERE email=?;" << email << row;
            if (!rCompany.empty()) {
                rCompany >> companyName;
                companyName = Crypto::Decrypt(companyName);
                suppliant = companyName;
            }
        }

        WAnchor *suppliantAnchor = new WAnchor(WLink(), WString::fromUTF8(suppliant));
        suppliantAnchor->setStyleClass("link");

        table->elementAt(i, 0)->addWidget(new WText(lexical_cast<wstring>(i)));
        table->elementAt(i, 1)->addWidget(suppliantAnchor);
        table->elementAt(i, 2)->addWidget(new WText((wformat(L"%1%/%2%/%3%") % yearFormatted % monthFormatted % dayFormatted).str()));
        table->elementAt(i, 3)->addWidget(new WText((wformat(L"%1%:00") % hourFormatted).str()));

        suppliantAnchor->setAttributeValue("dbid", WString::fromUTF8(email));

        WSignalMapper<WAnchor *> *suppliantSignalMapper = new WSignalMapper<WAnchor *>(this);
        suppliantSignalMapper->mapped().connect(this, &CmsAppointments::OnSuppliantLinkClicked);
        suppliantSignalMapper->mapConnect(suppliantAnchor->clicked(), suppliantAnchor);
    }

    return root;
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *CmsAppointments::Layout()
{
    Div *root = new Div("CmsAppointments");

    WPushButton *showAppointmentsTableButton = new WPushButton(m_lang->GetString("ROOT_CMSAPPOINTMENTS_SHOW_APPOINTMENTS_TABLE_BUTTON"),
                                                               root);
    showAppointmentsTableButton->setStyleClass("formButtonNoWidth");

    showAppointmentsTableButton->clicked().connect(this, &CmsAppointments::OnShowAppointmentsTableButtonPressed);

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


