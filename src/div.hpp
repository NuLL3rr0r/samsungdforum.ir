#ifndef DIV_HPP
#define DIV_HPP


#include <string>

namespace Wt {
    class WContainerWidget;
}

namespace SamsungDForumIr {
    class Div;
}

class SamsungDForumIr::Div : public Wt::WContainerWidget
{
public:
    Div(Wt::WContainerWidget *parent, const std::string& id, const std::string& cssClass);
    Div(Wt::WContainerWidget *parent, const std::string& id);
    Div(Wt::WContainerWidget *parent);
    Div(const std::string& id, const std::string& cssClass);
    Div(const std::string& id);
    Div();
};


#endif /* DIV_HPP */


