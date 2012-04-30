#ifndef ROOTCMS_HPP
#define ROOTCMS_HPP


namespace SamsungDForumIr {
    class RootCMS;
}

#include "basewidget.hpp"

class SamsungDForumIr::RootCMS : public SamsungDForumIr::BaseWidget
{
public:
    RootCMS(CgiRoot *cgi);

private:
    void ExitCMS(Wt::WMenuItem *sender);

    Wt::WWidget *CWelcome();
    Wt::WWidget *CUsers();
    Wt::WWidget *CIdeas();
    Wt::WWidget *CApps();
    Wt::WWidget *CAppointments();
    Wt::WWidget *CNews();
    Wt::WWidget *CPages();
    Wt::WWidget *CContacts();
    Wt::WWidget *CPwdEmail();
    Wt::WWidget *CSysMon();
    Wt::WWidget *CExit();

private:
    Wt::WWidget *Layout();
};


#endif /* ROOTCMS_HPP */


