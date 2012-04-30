/**********************************
*    = Header File Inclusion =
**********************************/

#include "dbtables.hpp"
#include "log.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
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

DBTables::DBTables()
{
    InitHashes(CgiEnv::ELang_None);
}

DBTables::DBTables(const CgiEnv::ELang &lang)
{
    InitHashes(lang);
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

string DBTables::Table(const char *id)
{
    if (m_tablesHash.find(id) != m_tablesHash.end()) {
        return m_tablesHash[id];
    } else {
        DBG(id);
        return "{?}";
    }
}

string DBTables::Fields(const char *id)
{
    if (m_fieldsHash.find(id) != m_fieldsHash.end()) {
        return m_fieldsHash[id];
    } else {
        DBG(id);
        return "{?}";
    }
}


/**********************************
*    = Event Handlers =
**********************************/

/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

void DBTables::InitHashes(const CgiEnv::ELang &lang)
{
    m_tablesHash["ROOT"] = "root";
    m_tablesHash["APPOINTMENTS"] = "appointments";
    m_tablesHash["APPS"] = "apps";
    m_tablesHash["CONTACTS_EXTRA_INFO"] = "contactsextrainfo";
    m_tablesHash["IDEAS"] = "ideas";
    m_tablesHash["USERS"] = "users";
    m_tablesHash["USERS_COMPANY"] = "userscompany";
    m_tablesHash["USERS_COMPANY_EXTRA"] = "userscompanyextra";
    m_tablesHash["USERS_INDIVIDUAL"] = "usersindividual";
    m_tablesHash["USERS_INDIVIDUAL_EXTRA"] = "usersindividualextra";

    switch(lang) {
    case CgiEnv::ELang_Invalid:
    case CgiEnv::ELang_None:
        m_tablesHash["CONTACTS_EN"] = "contactsen";
        m_tablesHash["CONTACTS_FA"] = "contactsfa";
        m_tablesHash["NEWS_EN"] = "newsen";
        m_tablesHash["NEWS_FA"] = "newsfa";
        m_tablesHash["PAGES_EN"] = "pagesen";
        m_tablesHash["PAGES_FA"] = "pagesfa";
        break;

    case CgiEnv::ELang_En:
    case CgiEnv::ELang_RootEn:
        m_tablesHash["CONTACTS"] = "contactsen";
        m_tablesHash["NEWS"] = "newsen";
        m_tablesHash["PAGES"] = "pagesen";
        break;

    case CgiEnv::ELang_Fa:
    case CgiEnv::ELang_RootFa:
        m_tablesHash["CONTACTS"] = "contactsfa";
        m_tablesHash["NEWS"] = "newsfa";
        m_tablesHash["PAGES"] = "pagesfa";
        break;

    default:
        break;
    }


    m_fieldsHash["ROOT"] =
            " user TEXT NOT NULL, "
            " pwd TEXT NOT NULL, "
            " email TEXT NOT NULL, "
            " ip TEXT, "
            " location TEXT, "
            " gdate TEXT, "
            " jdate TEXT, "
            " time TEXT, "
            " agent TEXT, "
            " referer TEXT, "
            " rawdate TEXT, "
            " PRIMARY KEY (user ASC) ";

    m_fieldsHash["APPOINTMENTS"] =
            " email TEXT NOT NULL, "
            " year INTEGER NOT NULL, "
            " month INTEGER NOT NULL, "
            " day INTEGER NOT NULL, "
            " hour INTEGER NOT NULL, "
            " PRIMARY KEY (email ASC) ";

    m_fieldsHash["APPS"] =
            " email TEXT NOT NULL, "
            " title TEXT NOT NULL, "
            " type INTEGER NOT NULL, "
            " description TEXT NOT NULL, "
            " displaytype INTEGER NOT NULL, "
            " platformhtml5 INTEGER NOT NULL, "
            " platformjavascript INTEGER NOT NULL, "
            " platformflash INTEGER NOT NULL, "
            " tdsupportiprange INTEGER NOT NULL, "
            " tdpwprotected INTEGER NOT NULL, "
            " tdhascookies INTEGER NOT NULL, "
            " tdautologin INTEGER NOT NULL, "
            " tdhasfilm INTEGER NOT NULL, "
            " tdhasaudio INTEGER NOT NULL, "
            " avnoaudiovideo INTEGER NOT NULL, "
            " avnobuffer INTEGER NOT NULL, "
            " avwithplayback INTEGER NOT NULL, "
            " avfullscreenwithexit INTEGER NOT NULL, "
            " language TEXT NOT NULL, "
            " scr1st TEXT NOT NULL, "
            " scr2nd TEXT NOT NULL, "
            " scr3rd TEXT NOT NULL, "
            " scr4th TEXT NOT NULL, "
            " scrdescription TEXT NOT NULL, "
            " remotenums INTEGER NOT NULL, "
            " remotearrows INTEGER NOT NULL, "
            " remotetoolsinfo INTEGER NOT NULL, "
            " remotereturnexit INTEGER NOT NULL, "
            " remoteplayback INTEGER NOT NULL, "
            " remotecolorstuning INTEGER NOT NULL, "
            " remotevolumestuning INTEGER NOT NULL, "
            " binary TEXT NOT NULL ";


    m_fieldsHash["CONTACTS"] =
            " recipient TEXT NOT NULL, "
            " addr TEXT NOT NULL, "
            " PRIMARY KEY (recipient ASC) ";

    m_fieldsHash["CONTACTS_EXTRA_INFO"] =
            " lang TEXT NOT NULL, "
            " info TEXT NOT NULL, "
            " PRIMARY KEY (lang ASC) ";

    m_fieldsHash["IDEAS"] =
            " email TEXT NOT NULL, "
            " title TEXT NOT NULL, "
            " typegamesentertainment INTEGER NOT NULL, "
            " typehygienehealth INTEGER NOT NULL, "
            " typenewsinfo INTEGER NOT NULL, "
            " typelearning INTEGER NOT NULL, "
            " typenonepresenceservices INTEGER NOT NULL, "
            " typeonlineshopping INTEGER NOT NULL, "
            " typeothers INTEGER NOT NULL, "
            " typeotherstext TEXT NOT NULL, "
            " description TEXT NOT NULL, "
            " audienceteens INTEGER NOT NULL, "
            " audienceparents INTEGER NOT NULL, "
            " audienceboys INTEGER NOT NULL, "
            " audiencegirls INTEGER NOT NULL, "
            " audiencewomen INTEGER NOT NULL, "
            " audiencemen INTEGER NOT NULL, "
            " audiencechilds INTEGER NOT NULL, "
            " audienceallfamilymembers INTEGER NOT NULL, "
            " usage TEXT NOT NULL, "
            " similar TEXT NOT NULL, "
            " difference TEXT NOT NULL ";

    m_fieldsHash["NEWS"] =
            " title TEXT NOT NULL, "
            " body TEXT NOT NULL, "
            " date TEXT NOT NULL, "
            " archived TEXT NOT NULL "
            "  ";

    m_fieldsHash["PAGES"] =
            " pg TEXT NOT NULL, "
            " parent TEXT NOT NULL, "
            " fullpath TEXT NOT NULL, "
            " zindex INTEGER, "
            " body TEXT NOT NULL, "
            " viewcount INTEGER, "
            " url TEXT NOT NULL, "
            " PRIMARY KEY (fullpath ASC) ";

    m_fieldsHash["USERS"] =
            " email TEXT NOT NULL, "
            " pwd TEXT NOT NULL, "
            " ip TEXT, "
            " location TEXT, "
            " gdate TEXT, "
            " jdate TEXT, "
            " time TEXT, "
            " verified TEXT NOT NULL, "
            " PRIMARY KEY (email ASC) ";

    m_fieldsHash["USERS_COMPANY"] =
            " email TEXT NOT NULL, "
            " name TEXT NOT NULL, "
            " regid TEXT NOT NULL, "
            " phone TEXT NOT NULL, "
            " ceo TEXT NOT NULL, "
            " PRIMARY KEY (email ASC) ";

    m_fieldsHash["USERS_COMPANY_EXTRA"] =
            " email TEXT NOT NULL, "
            " altphone TEXT, "
            " economicalcode TEXT, "
            " foundingyear TEXT, "
            " addr TEXT, "
            " website TEXT, "
            " resumeurl TEXT, "
            " PRIMARY KEY (email ASC) ";

    m_fieldsHash["USERS_INDIVIDUAL"] =
            " email TEXT NOT NULL, "
            " firstname TEXT NOT NULL, "
            " lastname TEXT NOT NULL, "
            " phone TEXT NOT NULL, "
            " teammates TEXT NOT NULL, "
            " PRIMARY KEY (email ASC) ";

    m_fieldsHash["USERS_INDIVIDUAL_EXTRA"] =
            " email TEXT NOT NULL, "
            " altphone TEXT, "
            " nationalcode TEXT, "
            " educationlevel INTEGER, "
            " educationsubject TEXT, "
            " website TEXT, "
            " PRIMARY KEY (email ASC) ";
}


/**********************************
*    = Base Class Overrides =
**********************************/

/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


