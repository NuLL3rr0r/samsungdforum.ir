/**********************************
*    = Header File Inclusion =
**********************************/

#include <string>
#include <Wt/WContainerWidget>
#include "div.hpp"


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

Div::Div(WContainerWidget *parent, const string& id, const string& cssClass) : WContainerWidget(parent)
{
    setId(id);
    setStyleClass(cssClass);
}

Div::Div(WContainerWidget *parent, const string& id) : WContainerWidget(parent)
{
    setId(id);
}

Div::Div(WContainerWidget *parent) : WContainerWidget(parent)
{
}

Div::Div(const string& id, const string& cssClass) : WContainerWidget()
{
    setId(id);
    setStyleClass(cssClass);
}

Div::Div(const string& id) : WContainerWidget()
{
    setId(id);
}

Div::Div() : WContainerWidget()
{
}


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

/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


