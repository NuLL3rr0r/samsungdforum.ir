#ifndef CMSWELCOME_HPP
#define CMSWELCOME_HPP


namespace SamsungDForumIr {
    class CmsWelcome;
}

#include "basewidget.hpp"

class SamsungDForumIr::CmsWelcome : public SamsungDForumIr::BaseWidget
{
public:
    CmsWelcome(CgiRoot *cgi);

private:
    Wt::WWidget *Layout();
};


#endif /* CMSWELCOME_HPP */


