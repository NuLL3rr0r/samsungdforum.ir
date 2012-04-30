/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/format.hpp>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WGridLayout>
#include <Wt/WText>
#include <Wt/WWidget>
#include "cmswelcome.hpp"
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

CmsWelcome::CmsWelcome(CgiRoot *cgi) : BaseWidget(cgi)
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

WWidget *CmsWelcome::Layout()
{
    Div *root = new Div("CmsWelcome");

    new WText(L"<h3>W E L C O M E !!</h3>", root);

    result r = m_db->Sql() << (boost::format("SELECT ip, location, gdate, jdate, time, agent, referer, rawdate"
                                             " FROM %1% WHERE user=?;")
                               % m_dbTables->Table("ROOT")).str()
                           << m_cgiEnv->LoggedInUserName << row;
    string ip;
    string location;
    string gdate;
    string jdate;
    string time;
    string agent;
    string referer;
    string rawdate;

    if (!r.empty()) {
        r >> ip >> location >> gdate >> jdate >> time >> agent >> referer >> rawdate;

        WText *m_lastLoginInfoText = new WText(root);
        m_lastLoginInfoText->setText(L"<center>"
                                     "<div style=\"font-family: 'Bitstream Vera Sans', arial, helvetica, sans-serif; "
                                     "font-size: 11px; font-weight: bold; color: #203070; "
                                     "letter-spacing: 1px; line-height: 17px; width: 550px; direction: ltr; text-align: center;\">"
                                     "<br /><h4>Your last login</h4>"
                                     "<blockquote>"
                                     "<table>"
                                     ""
                                     "<tr valign=\"top\">"
                                     "<td width=\"85\">IP</td>"
                                     "<td width=\"15\">-</td>"
                                     "<td><span style=\"color: #900; font-weight: normal;\">"
                                     + WString::fromUTF8(ip) + L"</span></td>"
                                     "</tr>"
                                     ""
                                     "<tr valign=\"top\">"
                                     "<td width=\"85\">Location</td>"
                                     "<td width=\"15\">-</td>"
                                     "<td><span style=\"color: #900; font-weight: normal;\">"
                                     + WString::fromUTF8(location)
                                     + L"</span></td>"
                                     "</tr>"
                                     ""
                                     "<tr valign=\"top\">"
                                     "<td width=\"85\">User Agent</td>"
                                     "<td width=\"15\">-</td>"
                                     "<td><span style=\"color: #900; font-weight: normal;\">"
                                     + WString::fromUTF8(agent) + L"</span></td>"
                                     "</tr>"
                                     ""
                                     "<tr valign=\"top\">"
                                     "<td width=\"85\">Referer</td>"
                                     "<td width=\"15\">-</td>"
                                     "<td><span style=\"color: #900; font-weight: normal;\">"
                                     + WString::fromUTF8(referer) + L"</span></td>"
                                     "</tr>"
                                     ""
                                     "<tr valign=\"top\">"
                                     "<td width=\"85\">Time</td>"
                                     "<td width=\"15\">-</td>"
                                     "<td><span style=\"color: #900; font-weight: normal;\">"
                                     + WString::fromUTF8(jdate) + L" * "
                                     + WString::fromUTF8(rawdate)
                                     + L"</span></td>"
                                     "</tr>"
                                     ""
                                     "</table>"
                                     "</blockquote>"
                                     "<br /></div>"
                                     "</center>");
        m_lastLoginInfoText->setTextFormat(XHTMLUnsafeText);
    }

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


