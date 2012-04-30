/**********************************
*    = Header File Inclusion =
**********************************/

#include<Wt/WApplication>
#include<Wt/WContainerWidget>
#include<Wt/WFormWidget>
#include<Wt/WText>
#include "basewidget.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace Wt;
using namespace boost;
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

BaseWidget::BaseWidget(CgiRoot *cgi) : WContainerWidget(), m_cgiRoot(cgi)
{
    m_htmlRoot = m_cgiRoot->HtmlRoot;
    m_captcha = m_cgiRoot->CaptchaInstance;
    m_cgiEnv = m_cgiRoot->CgiEnvInstance;
    m_db = m_cgiRoot->DBInstance;
    m_dbTables = m_cgiRoot->DBTablesInstance;
    m_lang = m_cgiRoot->LangInstance;
}


/**********************************
*    = Destructor =
**********************************/

BaseWidget::~BaseWidget()
{
}


/**********************************
*    = Public Methods =
**********************************/

/**********************************
*    = Event Handlers =
**********************************/

/**********************************
*    = Protected Methods =
**********************************/

void BaseWidget::Redirect(const string& url)
{
    m_cgiRoot->Redirect(url);
}

void BaseWidget::Exit(const std::string& url)
{
    m_cgiRoot->Exit(url);
}

void BaseWidget::Exit()
{
    m_cgiRoot->Exit();
}

bool BaseWidget::Validate(WFormWidget *widget)
{
    switch (widget->validate()) {
    case WValidator::Valid:
        return true;
        break;
    case WValidator::InvalidEmpty:
        break;
    case WValidator::Invalid:
        break;
    default:
        break;
    }

    widget->setFocus();
    return false;
}

void BaseWidget::HtmlError(const string& err, WText *txt)
{
    txt->setText(err);
    txt->setStyleClass("boldTextErr");
}

void BaseWidget::HtmlError(const wstring& err, WText *txt)
{
    txt->setText(err);
    txt->setStyleClass("boldTextErr");
}

void BaseWidget::HtmlInfo(const string& msg, WText *txt)
{
    txt->setText(msg);
    txt->setStyleClass("boldText");
}

void BaseWidget::HtmlInfo(const wstring& msg, WText *txt)
{
    txt->setText(msg);
    txt->setStyleClass("boldText");
}


/**********************************
*    = Private Methods =
**********************************/

/**********************************
*    = Base Class Overrides =
**********************************/

/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


