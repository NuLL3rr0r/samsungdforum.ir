#ifndef CMSCONTACTS_HPP
#define CMSCONTACTS_HPP


#include <string>
#include <Wt/WSignalMapper>
#include "basewidget.hpp"

namespace SamsungDForumIr {
    class CmsContacts;
}

class SamsungDForumIr::CmsContacts : public SamsungDForumIr::BaseWidget
{
private:
    Wt::WMessageBox *m_msg;

    Wt::WText *m_errAddContact;

    Wt::WLineEdit *m_contactRecipientEdit;
    Wt::WLineEdit *m_contactAddrEdit;

    Wt::WContainerWidget *m_dvContactsTable;
    Wt::WPushButton *m_tableEraseButton;

    Wt::WText *m_errExtraContactInfo;
    Wt::WTextEdit *m_extraContactInfoTextEdit;

public:
    CmsContacts(CgiRoot *cgi);

private:
    void OnAddContactOK();
    void OnSaveContactsTableCell(Wt::WInPlaceEdit *sender);
    void OnEraseContactsTableCell(Wt::WPushButton *sender);
    void OnEraseContactsTableCellOK(Wt::StandardButton resultButton);
    void OnExtraInfoSaveButtonPressed();

private:
    void FillContactsDataTable();
    Wt::WInPlaceEdit *GetContactsCell(const std::string &cell, const std::string &id, const std::string &field,
                                      Wt::WSignalMapper<Wt::WInPlaceEdit *> *signalMapper);

private:
    Wt::WWidget *Layout();
};


#endif /* CMSCONTACTS_HPP */


