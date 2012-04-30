#ifndef CMSNEWS_HPP
#define CMSNEWS_HPP


namespace SamsungDForumIr {
    class CmsNews;
}

#include "basewidget.hpp"

class SamsungDForumIr::CmsNews : public SamsungDForumIr::BaseWidget
{
private:
    Wt::WDialog *m_dlg;
    Wt::WMessageBox *m_msg;

    Wt::WContainerWidget *m_dvNewsTable;
    Wt::WPushButton *m_tableEditButton;
    Wt::WPushButton *m_tableEraseButton;

    Wt::WLineEdit *m_titleLineEdit;
    Wt::WTextArea *m_bodyTextArea;

public:
    CmsNews(CgiRoot *cgi);

private:
    void OnAddButtonPressed();
    void OnAddDialogAddButtonPressed();
    void OnAddDialogReturnButtonPressed();
    void OnArchiveButtonPressed(Wt::WPushButton *sender);
    void OnEditButtonPressed(Wt::WPushButton *sender);
    void OnEditDialogEditButtonPressed();
    void OnEditDialogReturnButtonPressed();
    void OnEraseButtonPressed(Wt::WPushButton *sender);
    void OnEraseMessageBoxClosed(Wt::StandardButton result);

private:
    void FillNewsDataTable();
    Wt::WWidget *GetAddNewsForm();
    Wt::WWidget *GetEditNewsForm(std::string id);

private:
    Wt::WWidget *Layout();
};


#endif /* CMSNEWS_HPP */


