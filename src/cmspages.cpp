/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WGridLayout>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WMessageBox>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WTextEdit>
#include <Wt/WTree>
#include <Wt/WTreeNode>
#include <Wt/WWidget>
#include "cmspages.hpp"
#include "base.hpp"
#include "cmspagespageeditor.hpp"
#include "cmspagestreenode.hpp"
#include "crypto.hpp"
#include "db.hpp"
#include "dbtables.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace boost;
using namespace Wt;
using namespace cppdb;
using namespace SamsungDForumIr;


/**********************************
*    = Constants =
**********************************/

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

CmsPages::CmsPages(CgiRoot *cgi) : BaseWidget(cgi)
{
    this->clear();
    this->addWidget(Layout());
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

/**********************************
*    = Event Handlers =
**********************************/

void CmsPages::OnCmsPagesTreeNodeEditContents(CmsPagesTreeNode *sender)
{
    m_currentNode = sender;

    result r = m_db->Sql() << "SELECT body FROM [" + m_dbTables->Table("PAGES")
                              + "] WHERE fullpath=?;" << sender->GetFullPath() << row;
    if (!r.empty()) {
        string body;
        r >> body;

        m_dvPagesTree->hide();
        m_dvPageEditor->show();
        m_contentsTextEdit->setText(WString::fromUTF8(Crypto::Decrypt(body)));
        m_contentsTextEdit->setFocus();

        sender->OnEditContentsCompleted();
    }
}

void CmsPages::OnCmsPagesTreeNodeInsert(CmsPagesTreeNode *sender)
{
    m_currentNode = sender;

    m_dlg = new WDialog(m_lang->GetString("ROOT_CMSPAGES_INSERT_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->contents()->addWidget(GetInsertNodeForm());
    m_dlg->rejectWhenEscapePressed();
    m_dlg->show();
    m_insertNodeTitleEdit->setFocus();

    // HACK for 3.2.3-rc1
    if (m_dlg->height().value() < 115)
        m_dlg->resize(WLength::Auto, 115);
}

void CmsPages::OnCmsPagesTreeNodeEdit(CmsPagesTreeNode *sender)
{
    m_currentNode = sender;

    m_dlg = new WDialog(m_lang->GetString("ROOT_CMSPAGES_EDIT_DLG_TITLE"));
    m_dlg->setModal(true);
    m_dlg->contents()->addWidget(GetEditNodeForm(m_currentNode));
    m_editNodeTitleEdit->setText(m_currentNode->label()->text());
    m_dlg->rejectWhenEscapePressed();
    m_dlg->show();
    m_editNodeTitleEdit->setFocus();

    // HACK for 3.2.3-rc1
    if (m_dlg->height().value() < 115)
        m_dlg->resize(WLength::Auto, 115);
}

void CmsPages::OnCmsPagesTreeNodeErase(CmsPagesTreeNode *sender)
{
    m_currentNode = sender;

    wstring question((wformat(m_lang->GetString("ROOT_CMSPAGES_ERASE_MSG_QUESTION"))
                  % m_currentNode->label()->text().value()).str());

    m_msg = new WMessageBox(m_lang->GetString("ROOT_CMSPAGES_ERASE_MSG_TITLE"),
                            question, Warning, NoButton);

    m_msg->addButton(m_lang->GetString("ROOT_CMSPAGES_ERASE_MSG_OK_BUTTON"), Ok);
    m_msg->addButton(m_lang->GetString("ROOT_CMSPAGES_ERASE_MSG_CANCEL_BUTTON"), Cancel);

    m_msg->buttonClicked().connect(this, &CmsPages::OnEraseNodeOK);

    m_msg->show();
}

void CmsPages::OnCmsPagesTreeNodeMoveUp(CmsPagesTreeNode *sender)
{
    CmsPagesTreeNode *parent = reinterpret_cast<CmsPagesTreeNode *>(sender->parentNode());

    int index = -1;
    bool found = false;
    const vector<CmsPagesTreeNode *> &childs = reinterpret_cast<const vector<CmsPagesTreeNode *> &>(parent->childNodes());
    for (vector<CmsPagesTreeNode *>::const_iterator it = childs.begin(); it != childs.end(); ++it) {
        ++index;
        if ((*it) == sender) {
            found = true;
            break;
        }
    }

    if (found && index > 0) {
        CmsPagesTreeNode *prevNode = reinterpret_cast<CmsPagesTreeNode *>(parent->childNodes()[index - 1]);

        parent->removeChildNode(sender);
        parent->insertChildNode(index - 1, sender);

        m_db->Update(m_dbTables->Table("PAGES"), "fullpath", sender->GetFullPath(),
                     "zindex=?", 1,
                     lexical_cast<string>(index - 1).c_str());
        m_db->Update(m_dbTables->Table("PAGES"), "fullpath", prevNode->GetFullPath(),
                     "zindex=?", 1,
                     lexical_cast<string>(index).c_str());

        sender->OnMoveUpCompleted();
    }
}

void CmsPages::OnCmsPagesTreeNodeMoveDown(CmsPagesTreeNode *sender)
{
    CmsPagesTreeNode *parent = reinterpret_cast<CmsPagesTreeNode *>(sender->parentNode());

    int index = -1;
    bool found = false;
    const vector<CmsPagesTreeNode *> &childs = reinterpret_cast<const vector<CmsPagesTreeNode *> &>(parent->childNodes());
    for (vector<CmsPagesTreeNode *>::const_iterator it = childs.begin(); it != childs.end(); ++it) {
        ++index;
        if ((*it) == sender) {
            found = true;
            break;
        }
    }

    if (found && (size_t)index < parent->childNodes().size() - 1) {
        CmsPagesTreeNode *nextNode = reinterpret_cast<CmsPagesTreeNode *>(parent->childNodes()[index + 1]);

        parent->removeChildNode(sender);
        parent->insertChildNode(index + 1, sender);

        m_db->Update(m_dbTables->Table("PAGES"), "fullpath", sender->GetFullPath(),
                     "zindex=?", 1,
                     lexical_cast<string>(index + 1).c_str());
        m_db->Update(m_dbTables->Table("PAGES"), "fullpath", nextNode->GetFullPath(),
                     "zindex=?", 1,
                     lexical_cast<string>(index).c_str());

        sender->OnMoveDownCompleted();
    }
}

void CmsPages::OnAddNodeOK()
{
    if (!Validate(m_insertNodeTitleEdit))
        return;

    WString label(algorithm::trim_copy(m_insertNodeTitleEdit->text().toUTF8()));

    if (label == "") {
        m_msg = new WMessageBox(m_lang->GetString("ROOT_CMSPAGES_INSERT_INVALID_TITLE"),
                                m_lang->GetString("ROOT_CMSPAGES_INSERT_INVALID_ERROR"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("ROOT_CMSPAGES_INSERT_INVALID_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &CmsPages::OnAddNodeInvalidOK);
        m_msg->show();
        return;
    }

    bool duplicate = false;
    const vector<WTreeNode *> &childs = m_currentNode->childNodes();
    for (vector<WTreeNode *>::const_iterator it = childs.begin(); it != childs.end(); ++it) {
        if ((*it)->label()->text() == label) {
            duplicate = true;
            break;
        }
    }

    if (duplicate) {
        m_msg = new WMessageBox(m_lang->GetString("ROOT_CMSPAGES_INSERT_DUPLICATE_TITLE"),
                                m_lang->GetString("ROOT_CMSPAGES_INSERT_DUPLICATE_ERROR"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("ROOT_CMSPAGES_INSERT_DUPLICATE_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &CmsPages::OnAddNodeDuplicateOK);
        m_msg->show();
        return;
    }

    CmsPagesTreeNode *node = new CmsPagesTreeNode(m_insertNodeTitleEdit->text(), CmsPagesTreeNode::EType_Target, m_lang);
    SetNodeContextEvents(node);
    m_currentNode->addChildNode(node);
    m_currentNode->expand();
    m_currentNode->OnInsertCompleted();

    m_db->Insert(m_dbTables->Table("PAGES"), "pg, parent, fullpath, zindex, body, viewcount, url",
                 7, algorithm::trim_copy(m_insertNodeTitleEdit->text().toUTF8()).c_str(),
                 m_currentNode->GetFullPath().c_str(),
                 node->GetFullPath().c_str(),
                 lexical_cast<string>(m_currentNode->childNodes().size() - 1).c_str(),
                 Crypto::Encrypt("").c_str(),
                 "0",
                 Crypto::GenHash(m_currentNode->GetFullPath()).c_str());

    delete m_insertNodeTitleEdit;
    delete m_dlg;
    m_dlg = NULL;
}

void CmsPages::OnAddNodeInvalidOK()
{
    delete m_msg;
    m_msg = NULL;
    m_insertNodeTitleEdit->setFocus();
}

void CmsPages::OnAddNodeDuplicateOK()
{
    delete m_msg;
    m_msg = NULL;
    m_insertNodeTitleEdit->setFocus();
}

void CmsPages::OnAddNodeReturn()
{
    delete m_dlg;
    m_dlg = NULL;
}

void CmsPages::OnEditNodeOK()
{
    if (!Validate(m_editNodeTitleEdit))
        return;

    WString label(algorithm::trim_copy(m_editNodeTitleEdit->text().toUTF8()));

    if (label == "") {
        m_msg = new WMessageBox(m_lang->GetString("ROOT_CMSPAGES_INSERT_INVALID_TITLE"),
                                m_lang->GetString("ROOT_CMSPAGES_INSERT_INVALID_ERROR"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("ROOT_CMSPAGES_INSERT_INVALID_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &CmsPages::OnEditNodeInvalidOK);
        m_msg->show();
        return;
    }

    bool duplicate = false;
    const vector<WTreeNode *> &childs = m_currentNode->parentNode()->childNodes();
    for (vector<WTreeNode *>::const_iterator it = childs.begin(); it != childs.end(); ++it) {
        if ((*it) != m_currentNode && (*it)->label()->text() == label) {
            duplicate = true;
            break;
        }
    }

    if (duplicate) {
        m_msg = new WMessageBox(m_lang->GetString("ROOT_CMSPAGES_INSERT_DUPLICATE_TITLE"),
                                m_lang->GetString("ROOT_CMSPAGES_INSERT_DUPLICATE_ERROR"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("ROOT_CMSPAGES_INSERT_DUPLICATE_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &CmsPages::OnEditNodeDuplicateOK);
        m_msg->show();
        return;
    }

    string oldFullPath(m_currentNode->GetFullPath());

    m_currentNode->label()->setText(m_editNodeTitleEdit->text());
    m_currentNode->OnEditCompleted();

    string fullPath(m_currentNode->GetFullPath());

    m_db->Sql() << (boost::format("UPDATE [%1%] SET "
                                  "parent = REPLACE(parent, ?, ?), "
                                  "fullpath = REPLACE(fullpath, ?, ?) "
                                  "WHERE parent LIKE ?;") % m_dbTables->Table("PAGES")).str()
                << oldFullPath << fullPath
                << oldFullPath << fullPath
                << oldFullPath + "%" << exec;
    m_db->Update(m_dbTables->Table("PAGES"), "fullpath", oldFullPath,
                 "pg=?, fullpath=?, url=?", 3,
                 algorithm::trim_copy(m_editNodeTitleEdit->text().toUTF8()).c_str(),
                 fullPath.c_str(),
                 Crypto::GenHash(fullPath).c_str());

    delete m_editNodeTitleEdit;
    delete m_dlg;
    m_dlg = NULL;
}

void CmsPages::OnEditNodeInvalidOK()
{
    delete m_msg;
    m_msg = NULL;
    m_editNodeTitleEdit->setFocus();
}

void CmsPages::OnEditNodeDuplicateOK()
{
    delete m_msg;
    m_msg = NULL;
    m_editNodeTitleEdit->setFocus();
}

void CmsPages::OnEditNodeReturn()
{
    delete m_dlg;
    m_dlg = NULL;
}

void CmsPages::OnEraseNodeOK(StandardButton result)
{
    if (result == Ok) {
        string fullPath(m_currentNode->GetFullPath());

        m_currentNode->parentNode()->removeChildNode(m_currentNode);
        m_currentNode->OnEraseCompleted();
        delete m_currentNode;
        m_currentNode = NULL;

        m_db->Sql() << (boost::format("DELETE FROM [%1%] "
                                      "WHERE (parent LIKE ? OR fullpath=?);") % m_dbTables->Table("PAGES")).str()
                    << fullPath + "%" << fullPath << exec;
    }

    delete m_msg;
    m_msg = NULL;
}

void CmsPages::OnCloseButtonOK()
{
    m_contentsTextEdit->setText("");
    m_dvPageEditor->hide();
    m_dvPagesTree->show();
}

void CmsPages::OnSaveButtonOK()
{
    m_db->Update(m_dbTables->Table("PAGES"), "fullpath", m_currentNode->GetFullPath(),
                 "body=?", 1,
                 Crypto::Encrypt(m_contentsTextEdit->text().toUTF8()).c_str());

    OnCloseButtonOK();
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

void CmsPages::ExpandAllChildNodes(CmsPagesTreeNode *node)
{
    const vector<WTreeNode *> &childs = node->childNodes();
    for (vector<WTreeNode *>::const_iterator it = childs.begin(); it != childs.end(); ++it) {
        WTreeNode *child = (*it);
        child->expand();
        ExpandAllChildNodes(reinterpret_cast<CmsPagesTreeNode *>(child));
    }
}

void CmsPages::GetChildNodes(CmsPagesTreeNode *node)
{
    result r = m_db->Sql() << "SELECT pg FROM ["
                              + m_dbTables->Table("PAGES")
                              + "] WHERE parent=? "
                              "ORDER BY parent ASC, zindex ASC;" << node->GetFullPath();
    while (r.next()) {
        string pg;
        r >> pg;

        CmsPagesTreeNode *child = new CmsPagesTreeNode(WString::fromUTF8(pg),
                                                       CmsPagesTreeNode::EType_Target, m_lang);
        // 1: Add it to the tree.  2: Get its childs
        node->addChildNode(child);
        GetChildNodes(child);
    }

    // 3: Set context menu  4. Expand it
    SetNodeContextEvents(node);
    node->expand();
}

void CmsPages::SetNodeContextEvents(CmsPagesTreeNode *node)
{
    CmsPagesTreeNode::EType nodeType = node->GetNodeType();

    WSignalMapper<CmsPagesTreeNode *> *nodeSignalMapper;

    if (nodeType == CmsPagesTreeNode::EType_Root || nodeType == CmsPagesTreeNode::EType_Target) {
        nodeSignalMapper = new WSignalMapper<CmsPagesTreeNode *>(this);
        nodeSignalMapper->mapped().connect(this, &CmsPages::OnCmsPagesTreeNodeEditContents);
        nodeSignalMapper->mapConnect(node->EditContentsHandler(), node);
    }

    nodeSignalMapper = new WSignalMapper<CmsPagesTreeNode *>(this);
    nodeSignalMapper->mapped().connect(this, &CmsPages::OnCmsPagesTreeNodeInsert);
    nodeSignalMapper->mapConnect(node->InsertHandler(), node);

    if (nodeType == CmsPagesTreeNode::EType_Group || nodeType == CmsPagesTreeNode::EType_Target) {
        nodeSignalMapper = new WSignalMapper<CmsPagesTreeNode *>(this);
        nodeSignalMapper->mapped().connect(this, &CmsPages::OnCmsPagesTreeNodeEdit);
        nodeSignalMapper->mapConnect(node->EditHandler(), node);

        nodeSignalMapper = new WSignalMapper<CmsPagesTreeNode *>(this);
        nodeSignalMapper->mapped().connect(this, &CmsPages::OnCmsPagesTreeNodeErase);
        nodeSignalMapper->mapConnect(node->EraseHandler(), node);

        nodeSignalMapper = new WSignalMapper<CmsPagesTreeNode *>(this);
        nodeSignalMapper->mapped().connect(this, &CmsPages::OnCmsPagesTreeNodeMoveUp);
        nodeSignalMapper->mapConnect(node->MoveUpHandler(), node);

        nodeSignalMapper = new WSignalMapper<CmsPagesTreeNode *>(this);
        nodeSignalMapper->mapped().connect(this, &CmsPages::OnCmsPagesTreeNodeMoveDown);
        nodeSignalMapper->mapConnect(node->MoveDownHandler(), node);
    }
}

WWidget *CmsPages::GetInsertNodeForm()
{
    Div *root = new Div("InsertNodeForm");

    Div *dvForm = new Div(root);
    dvForm->setStyleClass("form");
    WGridLayout *dvFormLayout = new WGridLayout();

    dvFormLayout->addWidget(new WText(m_lang->GetString("ROOT_CMSPAGES_INSERT_DLG_PAGE_TITLE_TEXT")),
                                      0, 0, AlignLeft | AlignMiddle);

    m_insertNodeTitleEdit = new WLineEdit();

    WLengthValidator *titleValidator = new WLengthValidator(1, 255);
    titleValidator->setMandatory(true);
    m_insertNodeTitleEdit->setValidator(titleValidator);

    dvFormLayout->addWidget(m_insertNodeTitleEdit, 0, 1);

    dvFormLayout->setColumnStretch(0, 100);
    dvFormLayout->setColumnStretch(1, 200);
    dvFormLayout->setVerticalSpacing(11);
    dvForm->setLayout(dvFormLayout);

    Div *dvButtons = new Div(root);
    WPushButton *addButton = new WPushButton(m_lang->GetString("ROOT_CMSPAGES_INSERT_DLG_ADD_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("ROOT_CMSPAGES_INSERT_DLG_RETURN_BUTTON"), dvButtons);

    addButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    m_insertNodeTitleEdit->enterPressed().connect(this, &CmsPages::OnAddNodeOK);
    addButton->clicked().connect(this, &CmsPages::OnAddNodeOK);
    returnButton->clicked().connect(this, &CmsPages::OnAddNodeReturn);

    return root;
}

WWidget *CmsPages::GetEditNodeForm(CmsPagesTreeNode *node)
{
    Div *root = new Div("InsertNodeForm");

    Div *dvForm = new Div(root);
    dvForm->setStyleClass("form");
    WGridLayout *dvFormLayout = new WGridLayout();

    dvFormLayout->addWidget(new WText(m_lang->GetString("ROOT_CMSPAGES_EDIT_DLG_PAGE_TITLE_TEXT")),
                                      0, 0, AlignLeft | AlignMiddle);

    m_editNodeTitleEdit = new WLineEdit();

    WLengthValidator *titleValidator = new WLengthValidator(1, 255);
    titleValidator->setMandatory(true);
    m_editNodeTitleEdit->setValidator(titleValidator);

    dvFormLayout->addWidget(m_editNodeTitleEdit, 0, 1);

    dvFormLayout->setColumnStretch(0, 100);
    dvFormLayout->setColumnStretch(1, 200);
    dvFormLayout->setVerticalSpacing(11);
    dvForm->setLayout(dvFormLayout);

    Div *dvButtons = new Div(root);
    WPushButton *saveButton = new WPushButton(m_lang->GetString("ROOT_CMSPAGES_EDIT_DLG_SAVE_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("ROOT_CMSPAGES_EDIT_DLG_RETURN_BUTTON"), dvButtons);

    saveButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    m_editNodeTitleEdit->enterPressed().connect(this, &CmsPages::OnEditNodeOK);
    saveButton->clicked().connect(this, &CmsPages::OnEditNodeOK);
    returnButton->clicked().connect(this, &CmsPages::OnEditNodeReturn);

    return root;
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *CmsPages::Layout()
{
    Div *root = new Div("CmsPages");

    m_dvPagesTree = new Div(root, "dvPagesTree");
    m_dvPageEditor = new Div(root, "dvPageEditor");
    Div *dvPageEditorButtons = new Div(m_dvPageEditor, "dvPageEditorButtons");

    WTree *tree = new WTree(m_dvPagesTree);
    tree->setSelectionMode(SingleSelection);

    CmsPagesTreeNode *rootNode = new CmsPagesTreeNode(Base::DB_PAGE_ID_HOME,
                                                      CmsPagesTreeNode::EType_Root, m_lang);
    tree->setTreeRoot(rootNode);
    rootNode->label()->setTextFormat(PlainText);
    rootNode->setImagePack("resources/");
    rootNode->setLoadPolicy(WTreeNode::PreLoading);
    GetChildNodes(rootNode);
    ExpandAllChildNodes(rootNode);

    m_saveButton = new WPushButton(m_lang->GetString("ROOT_CMSPAGES_EDITOR_SAVE_BUTTON"), dvPageEditorButtons);
    m_closeButton = new WPushButton(m_lang->GetString("ROOT_CMSPAGES_EDITOR_CLOSE_BUTTON"), dvPageEditorButtons);
    m_saveButton->setStyleClass("editorButton");
    m_closeButton->setStyleClass("editorButton");

    m_contentsTextEdit = new WTextEdit("", m_dvPageEditor);

    // http://www.tinymce.com/wiki.php?title=Configuration
    m_contentsTextEdit->setConfigurationSetting("cleanup", false);
    m_contentsTextEdit->setConfigurationSetting("valid_elements", std::string("*[*]"));

    m_contentsTextEdit->setExtraPlugins("pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount,advlist,autosave");
    m_contentsTextEdit->setToolBar(0, "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,styleselect,formatselect,fontselect,fontsizeselect");
    m_contentsTextEdit->setToolBar(1, "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor");
    m_contentsTextEdit->setToolBar(2, "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen");
    m_contentsTextEdit->setToolBar(3, "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak,restoredraft");
    m_contentsTextEdit->resize(WLength::Auto, 550);

    m_advancedVideoTipTextArea = new WTextArea(L""
                "<!-- HTML5 Cross Browser Video Code -->"
                "\n"
                "<video controls=\"controls\" width=\"320\" height=\"240\">"
                "\n"
                "	<source src=\"***movie.webm***\" type=\"video/webm; codecs='vp8.0, vorbis'\" />"
                "\n"
                "	<source src=\"***movie.ogv***\" type=\"video/ogg; codecs='theora, vorbis'\" />"
                "\n"
                "	<source src=\"***movie.mp4***\" type=\"video/mp4; codecs='avc1.4D401E, mp4a.40.2'\" />"
                "\n"
                "</video>",
                m_dvPageEditor);
    m_advancedVideoTipTextArea->setStyleClass("tipsBox");
    m_advancedVideoTipTextArea->setReadOnly(true);

    m_dvPageEditor->hide();

    m_closeButton->clicked().connect(this, &CmsPages::OnCloseButtonOK);
    m_saveButton->clicked().connect(this, &CmsPages::OnSaveButtonOK);

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


