#ifndef CMSPAGES_HPP
#define CMSPAGES_HPP


namespace SamsungDForumIr {
    class CmsPages;
    class CmsPagesTreeNode;
}

#include "basewidget.hpp"

class SamsungDForumIr::CmsPages : public SamsungDForumIr::BaseWidget
{
private:
    Wt::WDialog *m_dlg;
    Wt::WMessageBox *m_msg;

    CmsPagesTreeNode *m_currentNode;

    Div *m_dvPagesTree;
    Div *m_dvPageEditor;
    Wt::WPushButton *m_saveButton;
    Wt::WPushButton *m_closeButton;
    Wt::WTextEdit *m_contentsTextEdit;
    Wt::WTextArea *m_advancedVideoTipTextArea;

    Wt::WLineEdit *m_insertNodeTitleEdit;
    Wt::WLineEdit *m_editNodeTitleEdit;

public:
    CmsPages(CgiRoot *cgi);

public:
    void OnCmsPagesTreeNodeEditContents(CmsPagesTreeNode *sender);
    void OnCmsPagesTreeNodeInsert(CmsPagesTreeNode *sender);
    void OnCmsPagesTreeNodeEdit(CmsPagesTreeNode *sender);
    void OnCmsPagesTreeNodeErase(CmsPagesTreeNode *sender);
    void OnCmsPagesTreeNodeMoveUp(CmsPagesTreeNode *sender);
    void OnCmsPagesTreeNodeMoveDown(CmsPagesTreeNode *sender);

    void OnAddNodeOK();
    void OnAddNodeInvalidOK();
    void OnAddNodeDuplicateOK();
    void OnAddNodeReturn();
    void OnEditNodeOK();
    void OnEditNodeInvalidOK();
    void OnEditNodeDuplicateOK();
    void OnEditNodeReturn();
    void OnEraseNodeOK(Wt::StandardButton result);
    void OnCloseButtonOK();
    void OnSaveButtonOK();

private:
    void ExpandAllChildNodes(CmsPagesTreeNode *node);
    void GetChildNodes(CmsPagesTreeNode *node);
    void SetNodeContextEvents(CmsPagesTreeNode *node);

    WWidget *GetInsertNodeForm();
    WWidget *GetEditNodeForm(CmsPagesTreeNode *node);

private:
    Wt::WWidget *Layout();
};


#endif /* CMSPAGES_HPP */


