/**********************************
*    = Header File Inclusion =
**********************************/

#include <Wt/WText>
#include "fileuploadprogressdialog.hpp"
#include "div.hpp"
#include "lang.hpp"


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

FileUploadProgressDialog::FileUploadProgressDialog(const Lang_ptr &lang)
    : WDialog(),
      m_lang(lang)
{
    setModal(true);
    setClosable(false);
    setResizable(false);

    setWindowTitle(m_lang->GetString("FILE_UPLOAD_PROGRESS_DIALOG_TITLE"));

    Div *dvContents = new Div();
    dvContents->setStyleClass("FileUploadProgressDialog");

    new WText(m_lang->GetString("FILE_UPLOAD_PROGRESS_DIALOG_BODY"), dvContents);

    contents()->addWidget(dvContents);
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

/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


