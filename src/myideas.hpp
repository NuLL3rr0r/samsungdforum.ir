#ifndef MYIDEAS_HPP
#define MYIDEAS_HPP


namespace SamsungDForumIr {
    class MyIdeas;
}

#include "basewidget.hpp"

class SamsungDForumIr::MyIdeas : public SamsungDForumIr::BaseWidget
{
private:
    Wt::WDialog *m_dlgParent;
    Wt::WDialog *m_dlg;
    Wt::WDialog *m_dlgDetails;
    Div *m_dvTable;

public:
    MyIdeas(CgiRoot *cgi, Wt::WDialog *parentDialog);

private:
    void OnDetailsLinkClicked(Wt::WAnchor *sender);
    void OnEditLinkClicked(Wt::WAnchor *sender);
    void OnEditIdeaCompleted();

private:
    Wt::WWidget *GetIdeasTable();

    Wt::WWidget *Layout();
};


#endif /* MYIDEAS_HPP */


