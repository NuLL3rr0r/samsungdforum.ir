#ifndef ROOTLOGIN_HPP
#define ROOTLOGIN_HPP


#include "basewidget.hpp"

namespace SamsungDForumIr {
    class RootLogin;
}

class SamsungDForumIr::RootLogin : public SamsungDForumIr::BaseWidget
{
private:
    Wt::WLineEdit *m_loginUserEdit;
    Wt::WLineEdit *m_loginPwdEdit;
    Wt::WLineEdit *m_loginCaptchaEdit;
    Wt::WImage *m_loginCaptchaImage;
    Wt::WText *m_errLoginText;
    Wt::WIntValidator *m_loginCaptchaValidator;

    bool m_forgotFormFlag;
    Div *m_dvForgot;
    Wt::WLineEdit *m_forgotEmailEdit;
    Wt::WLineEdit *m_forgotCaptchaEdit;
    Wt::WText *m_errForgotText;
    Wt::WIntValidator *m_forgotCaptchaValidator;

public:
    RootLogin(CgiRoot *cgi);

private:
    void ForgotForm();
    void ForgotOK();
    void LoginOK();

    void GenCap();

    Wt::WWidget *Layout();
};


#endif /* ROOTLOGIN_HPP */


