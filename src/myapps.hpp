#ifndef MYAPPS_HPP
#define MYAPPS_HPP


namespace SamsungDForumIr {
    class MyApps;
}

#include "basewidget.hpp"

class SamsungDForumIr::MyApps : public SamsungDForumIr::BaseWidget
{
private:
    std::vector<std::wstring> m_type;
    std::vector<std::wstring> m_displayType;
    Wt::WDialog *m_dlgParent;
    Wt::WDialog *m_dlg;
    Wt::WDialog *m_dlgDetails;
    Div *m_dvTable;

public:
    MyApps(CgiRoot *cgi, Wt::WDialog *parentDialog);

private:
    void OnDetailsLinkClicked(Wt::WAnchor *sender);
    void OnEditLinkClicked(Wt::WAnchor *sender);
    void OnEditAppCompleted();

private:
    Wt::WWidget *GetAppsTable();

    Wt::WWidget *Layout();
};


#endif /* MYAPPS_HPP */


