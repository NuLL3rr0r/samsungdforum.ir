#ifndef CMSAPPS_HPP
#define CMSAPPS_HPP


namespace SamsungDForumIr {
    class CmsApps;
}

#include "basewidget.hpp"

class SamsungDForumIr::CmsApps : public SamsungDForumIr::BaseWidget
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
    std::vector<std::wstring> m_type;
    std::vector<std::wstring> m_displayType;
    Wt::WDialog *m_dlg;
    Wt::WDialog *m_dlgDetails;

public:
    CmsApps(CgiRoot *cgi);

private:
    void OnShowAppsTableButtonPressed();
    void OnSubmitterLinkClicked(Wt::WAnchor *sender);
    void OnDetailsLinkClicked(Wt::WAnchor *sender);

private:
    Wt::WWidget *GetAppsTable();

    Wt::WWidget *Layout();
};


#endif /* CMSAPPS_HPP */


