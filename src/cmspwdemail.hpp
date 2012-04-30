#ifndef CMSPWDEMAIL_HPP
#define CMSPWDEMAIL_HPP


namespace SamsungDForumIr {
    class CmsPwdEmail;
}

#include "basewidget.hpp"

class SamsungDForumIr::CmsPwdEmail : public SamsungDForumIr::BaseWidget
{
private:
    Wt::WLineEdit *m_currentPwdEdit;
    Wt::WLineEdit *m_newPwdEdit;
    Wt::WLineEdit *m_confirmPwdEdit;
    Wt::WLineEdit *m_currentEmailEdit;
    Wt::WLineEdit *m_currentEmailPwdEdit;

    Wt::WText *m_errPwd;
    Wt::WText *m_errEmail;

public:
    CmsPwdEmail(CgiRoot *cgi);

private:
    void OnPwdOK();
    void OnEmailOK();

private:
    Wt::WWidget *Layout();
};


#endif /* CMSPWDEMAIL_HPP */


