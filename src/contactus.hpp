#ifndef CONTACTUS_HPP
#define CONTACTUS_HPP


namespace SamsungDForumIr {
    class ContactUs;
}

#include "basewidget.hpp"

class SamsungDForumIr::ContactUs : public SamsungDForumIr::BaseWidget {
private:
    Wt::WComboBox *m_toComboBox;
    Wt::WLineEdit *m_fromLineEdit;
    Wt::WLineEdit *m_emailLineEdit;
    Wt::WLineEdit *m_urlLineEdit;
    Wt::WLineEdit *m_subjectLineEdit;
    Wt::WTextArea *m_bodyTextArea;
    Wt::WLineEdit *m_captchaLineEdit;
    Wt::WIntValidator *m_captchaValidator;
    Wt::WImage *m_captchaImage;

    Wt::WPushButton *m_sendButton;
    Wt::WPushButton *m_clearButton;

    Wt::WText *m_errContactForm;

public:
    ContactUs(CgiRoot *cgi);

private:
    void OnToComboBoxSelectionChanged(Wt::WString to);
    void OnSendButtonPressed();
    void OnClearButtonPressed();

    void GenCap();
    void SetFormStatus(bool flag);

private:
    Wt::WWidget *Layout();
};


#endif /* HMCONTACTUS_HPP */


