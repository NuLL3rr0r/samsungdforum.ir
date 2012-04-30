#ifndef CMSCmsAppointmentsS_HPP
#define CMSCmsAppointmentsS_HPP


namespace SamsungDForumIr {
    class CmsAppointments;
}

#include "basewidget.hpp"

class SamsungDForumIr::CmsAppointments : public SamsungDForumIr::BaseWidget
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
    CmsAppointments(CgiRoot *cgi);

private:
    void OnShowAppointmentsTableButtonPressed();
    void OnSuppliantLinkClicked(Wt::WAnchor *sender);

private:
    Wt::WWidget *GetAppointmentsTable();

    Wt::WWidget *Layout();
};


#endif /* CMSCmsAppointmentsS_HPP */


