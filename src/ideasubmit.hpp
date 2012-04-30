#ifndef IDEASUBMIT_HPP
#define IDEASUBMIT_HPP


namespace SamsungDForumIr {
    class IdeaSubmit;
}

#include "basewidget.hpp"

class SamsungDForumIr::IdeaSubmit : public SamsungDForumIr::BaseWidget
{
private:
    Wt::WDialog *m_dlgParent;
    Wt::WMessageBox *m_msg;

    Wt::WCheckBox *m_typeGamesEntertainmentCheckBox;
    Wt::WCheckBox *m_typeHygieneHealthCheckBox;
    Wt::WCheckBox *m_typeNewsInfoCheckBox;
    Wt::WCheckBox *m_typeLearningCheckBox;
    Wt::WCheckBox *m_typeNonePresenceServicesCheckBox;
    Wt::WCheckBox *m_typeOnlineShoppingCheckBox;
    Wt::WCheckBox *m_typeOthersCheckBox;
    Wt::WLineEdit *m_typeOthersTextLineEdit;
    Wt::WLineEdit *m_titleLineEdit;
    Wt::WTextArea *m_descriptionTextArea;
    Wt::WCheckBox *m_audienceTeensCheckBox;
    Wt::WCheckBox *m_audienceParentsCheckBox;
    Wt::WCheckBox *m_audienceBoysCheckBox;
    Wt::WCheckBox *m_audienceGirlsCheckBox;
    Wt::WCheckBox *m_audienceWomenCheckBox;
    Wt::WCheckBox *m_audienceMenCheckBox;
    Wt::WCheckBox *m_audienceChildsCheckBox;
    Wt::WCheckBox *m_audienceAllFamilyMembersCheckBox;
    Wt::WTextArea *m_usageTextArea;
    Wt::WTextArea *m_similarTextArea;
    Wt::WTextArea *m_differenceTextArea;

    bool m_editMode;
    size_t m_editRowId;

    Wt::Signal<> m_editCompletedHandler;

public:
    IdeaSubmit(CgiRoot *cgi, Wt::WDialog *parentDialog, bool editMode = false, size_t editRowId = 0);

private:
    void OnTypeOthersCheckBoxChanged();
    void OnSubmitButtonPressed();
    void OnReturnButtonPressed();
    void OnSubmitSuccessOK();

public:
    Wt::Signal<> &EditCompletedHandler();

private:
    Wt::WWidget *Layout();
};


#endif /* IDEASUBMIT_HPP */


