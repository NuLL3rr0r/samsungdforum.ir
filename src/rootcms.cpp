/**********************************
*    = Header File Inclusion =
**********************************/

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WStackedWidget>
#include <Wt/WText>
#include <Wt/WWidget>
#include "rootcms.hpp"
#include "base.hpp"
#include "cgiroot.hpp"
#include "cmsappointments.hpp"
#include "cmsapps.hpp"
#include "cmscontacts.hpp"
#include "cmsideas.hpp"
#include "cmsnews.hpp"
#include "cmspages.hpp"
#include "cmspwdemail.hpp"
#include "cmsusers.hpp"
#include "cmswelcome.hpp"
#include "lang.hpp"
//#include "sysmon.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
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

RootCMS::RootCMS(CgiRoot *cgi) : BaseWidget(cgi)
{
    m_htmlRoot->clear();
    m_htmlRoot->addWidget(Layout());
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

void RootCMS::ExitCMS(WMenuItem *sender)
{
    if (sender->text() == m_lang->GetString("ROOT_CMS_MENU_EXIT")) {
        switch(m_cgiEnv->CurrentLang) {
        case CgiEnv::ELang_RootEn:
            Exit("./?enroot");
            break;
        case CgiEnv::ELang_RootFa:
            Exit("./?faroot");
            break;
        default:
            Exit(".");
            break;
        }
    }
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

WWidget *RootCMS::CWelcome()
{
    return new CmsWelcome(m_cgiRoot);
}

WWidget *RootCMS::CUsers()
{
    return new CmsUsers(m_cgiRoot);
}

WWidget *RootCMS::CIdeas()
{
    return new CmsIdeas(m_cgiRoot);
}

WWidget *RootCMS::CApps()
{
    return new CmsApps(m_cgiRoot);
}

WWidget *RootCMS::CAppointments()
{
    return new CmsAppointments(m_cgiRoot);
}

WWidget *RootCMS::CNews()
{
    return new CmsNews(m_cgiRoot);
}

WWidget *RootCMS::CPages()
{
    return new CmsPages(m_cgiRoot);
}

WWidget *RootCMS::CContacts()
{
    return new CmsContacts(m_cgiRoot);
}

WWidget *RootCMS::CPwdEmail()
{
    return new CmsPwdEmail(m_cgiRoot);
}

/*WWidget *RootCMS::CSysMon()
{
    return new SysMon(m_cgiRoot);
}*/

WWidget *RootCMS::CExit()
{
    return new WText(L"");
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *RootCMS::Layout()
{
    m_cgiRoot->setTitle(m_lang->GetString("ROOT_CMS_TITLE"));

    // it has to be child of 'this' object to clean-up gracefully
    Div *root = new Div(this, "RootCMS");
	
    Div *dvHeader = new Div(root, "dvHeader");
    dvHeader->addWidget(new Div("dvLogo"));

    Div *dvBody = new Div(root, "dvBody");
    Div *dvSideWrapper = new Div(dvBody, "dvSideWrapper");
    Div *dvContentsWrapper = new Div(dvBody, "dvContentsWrapper");
    Div *dvCopyright = new Div(root, "dvCopyright");

    dvCopyright->addWidget(new WText(Base::Copyright(m_cgiEnv->CurrentLang)));

    Div *dvSide = new Div(dvSideWrapper, "dvMenu");
    Div *dvContents = new Div(dvContentsWrapper, "dvContents");

    Div *dvMenu = new Div(dvSide, "dvSide");

    WStackedWidget *dvMainContents = new WStackedWidget(dvContents);
    dvMainContents->setId("dvMainContents");

    WMenu *mainMenu = new WMenu(dvMainContents, Vertical, dvMenu);
    mainMenu->setRenderAsList(true);

    mainMenu->addItem(m_lang->GetString("ROOT_CMS_MENU_WELCOME"), CWelcome(),
                      WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem(m_lang->GetString("ROOT_CMS_MENU_USERS"), CUsers(),
                      WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem(m_lang->GetString("ROOT_CMS_MENU_IDEAS"), CIdeas(),
                      WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem(m_lang->GetString("ROOT_CMS_MENU_APPS"), CApps(),
                      WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem(m_lang->GetString("ROOT_CMS_MENU_APPOINTMENTS"), CAppointments(),
                      WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem(m_lang->GetString("ROOT_CMS_MENU_NEWS"), CNews(),
                      WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem(m_lang->GetString("ROOT_CMS_MENU_PAGES"), CPages(),
                      WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem(m_lang->GetString("ROOT_CMS_MENU_CONTACTS"), CContacts(),
                      WMenuItem::PreLoading)->setPathComponent("");
    mainMenu->addItem(m_lang->GetString("ROOT_CMS_MENU_PWDEMAIL"), CPwdEmail(),
                      WMenuItem::PreLoading)->setPathComponent("");
    /*mainMenu->addItem(m_lang->GetString("ROOT_CMS_MENU_SYSMON"), CSysMon(),
                      WMenuItem::PreLoading)->setPathComponent("");*/
    mainMenu->addItem(m_lang->GetString("ROOT_CMS_MENU_EXIT"), CExit(),
                      WMenuItem::PreLoading)->setPathComponent("");

    mainMenu->itemSelected().connect(this, &RootCMS::ExitCMS);

    // see the begining of the Layout function
    return this;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


