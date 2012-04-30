#ifndef CMSPAGESPAGEEDITOR
#define CMSPAGESPAGEEDITOR


namespace Wt {
    class WContainerWidget;
    class WDropEvent;
    class WString;
    class WTextEdit;
}

namespace SamsungDForumIr {
    class CmsPagesPageEditor;
}

class SamsungDForumIr::CmsPagesPageEditor : public Wt::WTextEdit
{
private:
    static const std::string M_HOVER_STYLE_CLASS;
    static const std::string M_MIME_TYPE;

public:
    CmsPagesPageEditor(Wt::WContainerWidget *parent = 0);
    CmsPagesPageEditor(const Wt::WString &text, Wt::WContainerWidget *parent = 0);

protected:
    void dropEvent(Wt::WDropEvent event);
};


#endif /* CMSPAGESPAGEEDITOR */


