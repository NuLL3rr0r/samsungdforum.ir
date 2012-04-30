/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WString>
#include <Wt/WText>
#include <Wt/WWidget>
#include "search.hpp"
#include "base.hpp"
#include "crypto.hpp"
#include "db.hpp"
#include "dbtables.hpp"
#include "div.hpp"


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

Search::Search(const WString &phrase, CgiRoot *cgi) : BaseWidget(cgi),
    m_phrase(phrase)
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

/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *Search::Layout()
{
    Div *root = new Div("Search");

    Div *dvSearchResults = new Div(root, "dvSearchResults");

    result r = m_db->Sql() << "SELECT fullpath, body, url FROM ["
                              + m_dbTables->Table("PAGES")
                              + "] WHERE parent <> ?;" << Base::DB_PAGE_ROOT;
    bool found = false;

    while (r.next()) {
        string fullpath;
        string body;
        string url;

        r >> fullpath >> body >> url;

        string phrase(to_lower_copy(algorithm::trim_copy(m_phrase.toUTF8())));
        body = to_lower_copy(Crypto::Decrypt(body));

        size_t posBody = body.find(phrase);

        if(posBody != string::npos) {
            regex htmlStripRegex("<[^>]+>"); // <.*?> is valid too
            string fmt = "$`";

            string plainText(regex_replace(body, htmlStripRegex, fmt, regex_constants::format_no_copy));

            size_t posPlainText = plainText.find(phrase);
            if(posPlainText != string::npos) {
                if (!found)
                    found = true;

                string lang;
                string pointer;
                switch(m_cgiEnv->CurrentLang) {
                case CgiEnv::ELang_En:
                    lang = "en";
                    pointer = " &gt; ";
                    break;

                case CgiEnv::ELang_Fa:
                    lang = "fa";
                    //pointer = " &lt; ";
                    pointer = " &gt; ";
                    break;

                default:
                    break;
                }

                new WAnchor((format("?lang=%1%&url=%2%") % lang % url).str(),
                            WString::fromUTF8(replace_all_copy(fullpath.substr(Base::DB_PAGE_FULLPATH_HOME.size() + 1), "/", pointer)),
                            dvSearchResults);
                new WBreak(dvSearchResults);
                new WBreak(dvSearchResults);
            }
        }
    }

    if (!found) {
        new WText((wformat(m_lang->GetString("HOME_SERARCH_NOT_FOUND_MSG"))
                   % algorithm::trim_copy(m_phrase.value())).str(), dvSearchResults);
    }

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


