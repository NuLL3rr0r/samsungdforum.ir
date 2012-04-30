#ifndef SEARCH_HPP
#define SEARCH_HPP


namespace SamsungDForumIr {
    class Search;
}

#include "basewidget.hpp"

class SamsungDForumIr::Search : public SamsungDForumIr::BaseWidget
{
private:
    Wt::WString m_phrase;

public:
    Search(const Wt::WString &phrase, CgiRoot *cgi);

private:
    Wt::WWidget *Layout();
};


#endif /* SEARCH_HPP */


