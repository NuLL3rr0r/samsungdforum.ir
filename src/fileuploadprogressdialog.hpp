#ifndef FILEUPLOADPROGRESSDIALOG_HPP
#define FILEUPLOADPROGRESSDIALOG_HPP


#include <vector>
#include <Wt/WDialog>

namespace Wt {
    class ProgressBar;
}

namespace SamsungDForumIr {
    class FileUploadProgressDialog;
    class Lang;
}

class SamsungDForumIr::FileUploadProgressDialog : public Wt::WDialog
{
private:
    typedef std::shared_ptr<Lang> Lang_ptr;

private:
    Lang_ptr m_lang;

public:
    FileUploadProgressDialog(const Lang_ptr &lang);
};


#endif /* FILEUPLOADPROGRESSDIALOG_HPP */


