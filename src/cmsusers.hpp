#ifndef CMSUSERS_HPP
#define CMSUSERS_HPP


#include <string>
#include <vector>

namespace SamsungDForumIr {
    class CmsUsers;
}

#include "basewidget.hpp"

class SamsungDForumIr::CmsUsers : public SamsungDForumIr::BaseWidget
{
private:
    enum EIEDULEVEL {
        EIEDULEVEL_NONE = 0,
        EIEDULEVEL_AD = 1,
        EIEDULEVEL_BA = 2,
        EIEDULEVEL_MA = 3,
        EIEDULEVEL_PHD = 4
    };

private:
    std::vector<std::wstring> m_educationLevel;

    Wt::WDialog *m_dlg;
    Wt::WMessageBox *m_msg;
    Wt::WLineEdit *m_messageSubjectLineEdit;
    Wt::WTextEdit *m_messageBodyTextEdit;

public:
    CmsUsers(CgiRoot *cgi);

private:
    void OnShowIndividualsTableButtonPressed();
    void OnShowCompaniesTableButtonPressed();
    void OnSendMessageButtonPressed();
    void OnSendMessageConfirmDialogClosed(Wt::StandardButton result);
    void OnSendMessageSuccessDialogClosed(Wt::StandardButton result);

private:
    Wt::WWidget *GetIndividualsTable();
    Wt::WWidget *GetCompaniesTable();

private:
    Wt::WWidget *Layout();
};


#endif /* CMSUSERS_HPP */


