#ifndef APPSUBMIT_HPP
#define APPSUBMIT_HPP


namespace SamsungDForumIr {
    class AppSubmit;
}

#include "basewidget.hpp"

class SamsungDForumIr::AppSubmit : public SamsungDForumIr::BaseWidget
{
private:
    enum EAppType {
        EAppType_None = 0,
        EAppType_GameEntertainment = 1,
        EAppType_OnlineGame = 2,
        EAppType_LearnInfo = 3,
        EAppType_Services = 4,
        EAppType_Tools = 5
    };

    enum EDisplayType {
        EDisplayType_None = 0,
        EDisplayType_FullScreen = 1,
        EDisplayType_OnCornerOfScreen = 2,
        EDisplayType_ActiveOnScreen = 3
    };

    enum EYNQuestion {
        EYNQuestion_None = -1,
        EYNQuestion_No = 0,
        EYNQuestion_Yes = 1
    };

private:
    Wt::WDialog *m_dlgParent;
    Wt::WMessageBox *m_msg;

    Wt::WPushButton *m_submitButton;

    Wt::WLineEdit *m_titleLineEdit;
    Wt::WComboBox *m_typeComboBox;
    Wt::WTextArea *m_descriptionTextArea;
    Wt::WComboBox *m_displayTypeComboBox;
    Wt::WCheckBox *m_platformHTML5CheckBox;
    Wt::WCheckBox *m_platformJavaScriptCheckBox;
    Wt::WCheckBox *m_platformFlashCheckBox;
    Wt::WRadioButton *m_tdSupportIPRangeYesRadioButton;
    Wt::WRadioButton *m_tdSupportIPRangeNoRadioButton;
    Wt::WRadioButton *m_tdPwdProtectedYesRadioButton;
    Wt::WRadioButton *m_tdPwdProtectedNoRadioButton;
    Wt::WRadioButton *m_tdHasCookiesYesRadioButton;
    Wt::WRadioButton *m_tdHasCookiesNoRadioButton;
    Wt::WRadioButton *m_tdAutoLoginYesRadioButton;
    Wt::WRadioButton *m_tdAutoLoginNoRadioButton;
    Wt::WRadioButton *m_tdHasFilmYesRadioButton;
    Wt::WRadioButton *m_tdHasFilmNoRadioButton;
    Wt::WRadioButton *m_tdHasAudioYesRadioButton;
    Wt::WRadioButton *m_tdHasAudioNoRadioButton;
    Wt::WCheckBox *m_avNoAudioVideoCheckBox;
    Wt::WCheckBox *m_avNoBufferCheckBox;
    Wt::WCheckBox *m_avWithPlaybackCheckBox;
    Wt::WCheckBox *m_avFullScreenWithExitCheckBox;
    Wt::WLineEdit *m_languageLineEdit;
    Wt::WFileUpload *m_scr1stFileUpload;
    Wt::WFileUpload *m_scr2ndFileUpload;
    Wt::WFileUpload *m_scr3rdFileUpload;
    Wt::WFileUpload *m_scr4thFileUpload;
    Wt::WTextArea *m_scrDescriptionTextArea;
    Wt::WCheckBox *m_remoteNumsCheckBox;
    Wt::WCheckBox *m_remoteArrowsCheckBox;
    Wt::WCheckBox *m_remoteToolsInfoCheckBox;
    Wt::WCheckBox *m_remoteReturnExitCheckBox;
    Wt::WCheckBox *m_remotePlaybackCheckBox;
    Wt::WCheckBox *m_remoteColorsTuningCheckBox;
    Wt::WCheckBox *m_remoteVolumesTuningCheckBox;
    Wt::WFileUpload *m_binaryFileUpload;

    Wt::WButtonGroup *m_tdSupportIPRangeButtonGroup;
    Wt::WButtonGroup *m_tdPwdProtectedButtonGroup;
    Wt::WButtonGroup *m_tdHasCookiesButtonGroup;
    Wt::WButtonGroup *m_tdAutoLoginButtonGroup;
    Wt::WButtonGroup *m_tdHasFilmButtonGroup;
    Wt::WButtonGroup *m_tdHasAudioButtonGroup;

    bool m_scr1stFileUploadHasFile;
    bool m_scr2ndFileUploadHasFile;
    bool m_scr3rdFileUploadHasFile;
    bool m_scr4thFileUploadHasFile;
    bool m_binaryFileUploadHasFile;

    bool m_scr1stFileUploaded;
    bool m_scr2ndFileUploaded;
    bool m_scr3rdFileUploaded;
    bool m_scr4thFileUploaded;
    bool m_binaryFileUploaded;

    bool m_editMode;
    size_t m_editRowId;

    Wt::Signal<> m_editCompletedHandler;

    std::string m_oldScr1st;
    std::string m_oldScr2nd;
    std::string m_oldScr3rd;
    std::string m_oldScr4th;
    std::string m_oldBinary;

public:
    AppSubmit(CgiRoot *cgi, Wt::WDialog *parentDialog, bool editMode = false, size_t editRowId = 0);

private:
    void OnTypeComboBoxChanged(int index);
    void OnDisplayTypeComboBoxChanged(int index);

    void OnScr1stFileChanged();
    void OnScr2ndFileChanged();
    void OnScr3rdFileChanged();
    void OnScr4thFileChanged();
    void OnBinaryFileChanged();

    void OnScr1stFileTooLarge();
    void OnScr2ndFileTooLarge();
    void OnScr3rdFileTooLarge();
    void OnScr4thFileTooLarge();
    void OnBinaryFileTooLarge();

    void OnScr1stFileUploaded();
    void OnScr2ndFileUploaded();
    void OnScr3rdFileUploaded();
    void OnScr4thFileUploaded();
    void OnBinaryFileUploaded();

    void OnSubmitButtonPressed();
    void OnReturnButtonPressed();
    void OnSubmitFailedOK();
    void OnSubmitSuccessOK();

public:
    Wt::Signal<> &EditCompletedHandler();

private:
    Wt::WWidget *Layout();
};


#endif /* APPSUBMIT_HPP */


