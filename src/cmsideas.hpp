#ifndef CMSIDEAS_HPP
#define CMSIDEAS_HPP


namespace SamsungDForumIr {
    class CmsIdeas;
}

#include "basewidget.hpp"

class SamsungDForumIr::CmsIdeas : public SamsungDForumIr::BaseWidget
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
    Wt::WDialog *m_dlgDetails;

public:
    CmsIdeas(CgiRoot *cgi);

private:
    void OnShowIdeasTableButtonPressed();
    void OnSubmitterLinkClicked(Wt::WAnchor *sender);
    void OnDetailsLinkClicked(Wt::WAnchor *sender);

private:
    Wt::WWidget *GetIdeasTable();

    Wt::WWidget *Layout();
};


#endif /* CMSIDEAS_HPP */


