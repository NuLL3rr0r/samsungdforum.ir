#ifndef CMSPAGESTREENODE_HPP
#define CMSPAGESTREENODE_HPP


#include <memory>

namespace Wt {
    class WContainerWidget;
    class WIconPair;
    class WMessageBox;
    class WPopupMenuItem;
    class WString;
    class WTreeNode;
}

namespace SamsungDForumIr {
    class CmsPagesTreeNode;
    class Lang;
}

class SamsungDForumIr::CmsPagesTreeNode : public Wt::WTreeNode
{
private:
    typedef std::shared_ptr<Lang> Lang_ptr;

public:
    static const std::string PATH_SEPARATOR_CHAR;

private:
    static const std::string M_ICON_SHADOW;
    static const std::string M_MIME_TYPE;

public:
    enum EType {
        EType_Root,
        EType_Group,
        EType_Target
    };

private:
    EType m_type;

    Lang_ptr m_lang;

    Wt::WMessageBox *m_msgBox;

    Wt::Signal<> m_editContentsHandler;
    Wt::Signal<> m_insertHandler;
    Wt::Signal<> m_editHandler;
    Wt::Signal<> m_eraseHandler;
    Wt::Signal<> m_moveUpHandler;
    Wt::Signal<> m_moveDownHandler;

    Wt::WPopupMenuItem *m_ctxItemEditContents;
    Wt::WPopupMenuItem *m_ctxItemInsert;
    Wt::WPopupMenuItem *m_ctxItemEdit;
    Wt::WPopupMenuItem *m_ctxItemErase;
    Wt::WPopupMenuItem *m_ctxItemMoveUp;
    Wt::WPopupMenuItem *m_ctxItemMoveDown;

public:
    CmsPagesTreeNode(const Wt::WString &labelText, EType type, const Lang_ptr &lang,
                     Wt::WIconPair *labelIcon = 0, Wt::WTreeNode *parentNode = 0);

public:
    EType GetNodeType() const;
    std::string GetFullPath() const;

    Wt::Signal<> &EditContentsHandler();
    Wt::Signal<> &InsertHandler();
    Wt::Signal<> &EditHandler();
    Wt::Signal<> &EraseHandler();
    Wt::Signal<> &MoveUpHandler();
    Wt::Signal<> &MoveDownHandler();

    void OnEditContentsCompleted();
    void OnInsertCompleted();
    void OnEditCompleted();
    void OnEraseCompleted();
    void OnMoveUpCompleted();
    void OnMoveDownCompleted();

private:
    void OnEditContentsClicked(Wt::WPopupMenuItem *sender);
    void OnInsertClicked(Wt::WPopupMenuItem *sender);
    void OnEditClicked(Wt::WPopupMenuItem *sender);
    void OnEraseClicked(Wt::WPopupMenuItem *sender);
    void OnMoveUpClicked(Wt::WPopupMenuItem *sender);
    void OnMoveDownClicked(Wt::WPopupMenuItem *sender);

private:
    void SetContextMenu();
    void SetContextMenuStats();
    void SetContextMenuStats(EType type);

private:
    void descendantAdded(Wt::WTreeNode *node);
    void descendantRemoved(Wt::WTreeNode *node);
};


#endif /* CMSPAGESTREENODE_HPP */


