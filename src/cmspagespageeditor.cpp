/**********************************
*    = Header File Inclusion =
**********************************/

#include <string>
#include <Wt/WContainerWidget>
#include <Wt/WImage>
#include <Wt/WString>
#include <Wt/WTextEdit>
#include "cmspagespageeditor.hpp"


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

const string CmsPagesPageEditor::M_HOVER_STYLE_CLASS = "cmspages-drop-site";
const string CmsPagesPageEditor::M_MIME_TYPE = "cmspages-treenode-icon";


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

CmsPagesPageEditor::CmsPagesPageEditor(WContainerWidget *parent)
    : WTextEdit(parent)
{
    acceptDrops(M_MIME_TYPE, M_HOVER_STYLE_CLASS);
}

CmsPagesPageEditor::CmsPagesPageEditor(const WString &text, WContainerWidget *parent)
    : WTextEdit(text, parent)
{
    acceptDrops(M_MIME_TYPE, M_HOVER_STYLE_CLASS);
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

void CmsPagesPageEditor::dropEvent(WDropEvent event)
{
    if (event.mimeType() == M_MIME_TYPE) {
        //WImage *node = reinterpret_cast<WImage *>(event.source());
        //m_text->setText(participant->attributeValue("name"));
    }
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


