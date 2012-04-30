/**********************************
*    = Header File Inclusion =
**********************************/

#include <vector>
#include <boost/algorithm/string.hpp>
#include <Wt/WIconPair>
#include <Wt/WImage>
#include <Wt/WMessageBox>
#include <Wt/WPopupMenu>
#include <Wt/WSignalMapper>
#include <Wt/WString>
#include <Wt/WText>
#include <Wt/WTreeNode>
#include "cmspagestreenode.hpp"
#include "lang.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace boost;
using namespace Wt;
using namespace SamsungDForumIr;


/**********************************
*    = Constants =
**********************************/

const string CmsPagesTreeNode::PATH_SEPARATOR_CHAR = "/";

const string CmsPagesTreeNode::M_ICON_SHADOW = "ico_cmspages_treenode_shadow_96x96.png";
const string CmsPagesTreeNode::M_MIME_TYPE = "cmspages-treenode-icon";


/**********************************
*    = Enumerations =
**********************************/

/**********************************
*    = Properties =
**********************************/

/**********************************
*    = Fields =
**********************************/

/**********************************
*    = Constructos =
**********************************/

CmsPagesTreeNode::CmsPagesTreeNode(const WString &labelText, EType type, const Lang_ptr &lang,
                                   WIconPair *labelIcon, WTreeNode *parentNode)
    : WTreeNode(labelText, labelIcon, parentNode),
            m_type(type),
            m_lang(lang),
            m_editContentsHandler(this),
            m_insertHandler(this),
            m_editHandler(this),
            m_eraseHandler(this),
            m_moveUpHandler(this),
            m_moveDownHandler(this)
{
    //WImage *shadow = new WImage(M_ICON_SHADOW, this);
    WImage *shadow = new WImage(M_ICON_SHADOW);
    this->label()->setDraggable(M_MIME_TYPE, shadow, true);

    SetContextMenu();
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

CmsPagesTreeNode::EType CmsPagesTreeNode::GetNodeType() const
{
    return m_type;
}

string CmsPagesTreeNode::GetFullPath() const
{
    string path(PATH_SEPARATOR_CHAR);

    vector<string> reverseHierarchy;

    WTreeNode *parent = this->parentNode();
    while(parent != nullptr) {
        string label(algorithm::trim_copy(parent->label()->text().toUTF8()));
        if (label != "")
            reverseHierarchy.push_back(label);

        parent = parent->parentNode();
    }

    for (vector<string>::const_reverse_iterator it =
         reverseHierarchy.rbegin(); it != reverseHierarchy.rend(); ++it) {
        path += (*it) + PATH_SEPARATOR_CHAR;
    }

    path += this->label()->text().toUTF8();

    return path;
}


/**********************************
*    = Event Handlers =
**********************************/

Signal<> &CmsPagesTreeNode::EditContentsHandler()
{
    return m_editContentsHandler;
}

Signal<> &CmsPagesTreeNode::InsertHandler()
{
    return m_insertHandler;
}

Signal<> &CmsPagesTreeNode::EditHandler()
{
    return m_editHandler;
}

Signal<> &CmsPagesTreeNode::EraseHandler()
{
    return m_eraseHandler;
}

Signal<> &CmsPagesTreeNode::MoveUpHandler()
{
    return m_moveUpHandler;
}

Signal<> &CmsPagesTreeNode::MoveDownHandler()
{
    return m_moveDownHandler;
}

void CmsPagesTreeNode::OnEditContentsClicked(WPopupMenuItem *sender)
{
    m_editContentsHandler.emit();
}

void CmsPagesTreeNode::OnInsertClicked(WPopupMenuItem *sender)
{
    m_insertHandler.emit();
}

void CmsPagesTreeNode::OnEditClicked(WPopupMenuItem *sender)
{
    m_editHandler.emit();
}

void CmsPagesTreeNode::OnEraseClicked(WPopupMenuItem *sender)
{
    m_eraseHandler.emit();
}

void CmsPagesTreeNode::OnMoveUpClicked(WPopupMenuItem *sender)
{
    m_moveUpHandler.emit();
}

void CmsPagesTreeNode::OnMoveDownClicked(WPopupMenuItem *sender)
{
    m_moveDownHandler.emit();
}

void CmsPagesTreeNode::OnEditContentsCompleted()
{
}

void CmsPagesTreeNode::OnInsertCompleted()
{
}

void CmsPagesTreeNode::OnEditCompleted()
{
}

void CmsPagesTreeNode::OnEraseCompleted()
{
}

void CmsPagesTreeNode::OnMoveUpCompleted()
{
    new WMessageBox("title", "msg", Warning, NoButton);
}

void CmsPagesTreeNode::OnMoveDownCompleted()
{
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

void CmsPagesTreeNode::SetContextMenu()
{
    WPopupMenu *popup = new WPopupMenu();
    WPopupMenuItem *item;
    WSignalMapper<WPopupMenuItem *> *itemSignalMapper;

    item = popup->addItem(m_lang->GetString("ROOT_CMSPAGES_CTX_ITEM_EDITCONTENTS"));
    itemSignalMapper = new WSignalMapper<WPopupMenuItem *>(this);
    itemSignalMapper->mapped().connect(this, &CmsPagesTreeNode::OnEditContentsClicked);
    itemSignalMapper->mapConnect(item->triggered(), item);
    m_ctxItemEditContents = item;

    popup->addSeparator();

    item = popup->addItem(m_lang->GetString("ROOT_CMSPAGES_CTX_ITEM_INSERT"));
    itemSignalMapper = new WSignalMapper<WPopupMenuItem *>(this);
    itemSignalMapper->mapped().connect(this, &CmsPagesTreeNode::OnInsertClicked);
    itemSignalMapper->mapConnect(item->triggered(), item);
    m_ctxItemInsert = item;

    item = popup->addItem(m_lang->GetString("ROOT_CMSPAGES_CTX_ITEM_EDIT"));
    itemSignalMapper = new WSignalMapper<WPopupMenuItem *>(this);
    itemSignalMapper->mapped().connect(this, &CmsPagesTreeNode::OnEditClicked);
    itemSignalMapper->mapConnect(item->triggered(), item);
    m_ctxItemEdit = item;

    item = popup->addItem(m_lang->GetString("ROOT_CMSPAGES_CTX_ITEM_ERASE"));
    itemSignalMapper = new WSignalMapper<WPopupMenuItem *>(this);
    itemSignalMapper->mapped().connect(this, &CmsPagesTreeNode::OnEraseClicked);
    itemSignalMapper->mapConnect(item->triggered(), item);
    m_ctxItemErase = item;

    popup->addSeparator();

    item = popup->addItem(m_lang->GetString("ROOT_CMSPAGES_CTX_ITEM_MOVEUP"));
    itemSignalMapper = new WSignalMapper<WPopupMenuItem *>(this);
    itemSignalMapper->mapped().connect(this, &CmsPagesTreeNode::OnMoveUpClicked);
    itemSignalMapper->mapConnect(item->triggered(), item);
    m_ctxItemMoveUp = item;

    item = popup->addItem(m_lang->GetString("ROOT_CMSPAGES_CTX_ITEM_MOVEDOWN"));
    itemSignalMapper = new WSignalMapper<WPopupMenuItem *>(this);
    itemSignalMapper->mapped().connect(this, &CmsPagesTreeNode::OnMoveDownClicked);
    itemSignalMapper->mapConnect(item->triggered(), item);
    m_ctxItemMoveDown = item;

    SetContextMenuStats();

    this->label()->clicked().connect(popup, &WPopupMenu::popup);
    this->label()->mouseWentUp().connect(popup, &WPopupMenu::popup);
    this->label()->setAttributeValue
            ("oncontextmenu",
             "event.cancelBubble = true; event.returnValue = false; return false;");
}

void CmsPagesTreeNode::SetContextMenuStats()
{
    switch (m_type) {
    case EType_Root:
        m_ctxItemEdit->setDisabled(true);
        m_ctxItemErase->setDisabled(true);
        m_ctxItemMoveUp->setDisabled(true);
        m_ctxItemMoveDown->setDisabled(true);
        break;
    case EType_Group:
        m_ctxItemEditContents->setDisabled(true);
        break;
    case EType_Target:
        break;
    default:
        break;
    }
}

void CmsPagesTreeNode::SetContextMenuStats(EType type)
{
    m_type = type;
    SetContextMenuStats();
}


/**********************************
*    = Base Class Overrides =
**********************************/

void CmsPagesTreeNode::descendantAdded(Wt::WTreeNode *node)
{
    if (m_type != EType_Root) {
        if (childNodes().size() > 0) {
            SetContextMenuStats(EType_Group);
        }
    }
}

void CmsPagesTreeNode::descendantRemoved(Wt::WTreeNode *node)
{
    if (m_type != EType_Root) {
        if (childNodes().size() <= 0) {
            SetContextMenuStats(EType_Target);
        }
    }
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


