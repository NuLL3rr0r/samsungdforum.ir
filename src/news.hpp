#ifndef NEWS_HPP
#define NEWS_HPP


namespace SamsungDForumIr {
    class News;
}

#include "basewidget.hpp"

class SamsungDForumIr::News : public SamsungDForumIr::BaseWidget
{
private:
    Wt::WDialog *m_dlg;
    Wt::WMessageBox *m_msg;

    Div *m_dvOlderNewsCollection;
    bool m_isOlderNewsCollectionShown;

public:
    News(CgiRoot *cgi);

private:
    bool ReadNews(std::string archived, Div *parent);

private:
    void OnNewsLinkClicked(Wt::WText *sender);
    void OnOlderNewsLinkClicked(Wt::WText *sender);

private:
    Wt::WWidget *Layout();
};


#endif /* NEWS_HPP */


