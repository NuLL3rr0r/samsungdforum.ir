#ifndef HOME_HPP
#define HOME_HPP


#include <string>
#include <Wt/WJavaScript>

namespace SamsungDForumIr {
    class FileUploadProgressDialog;
    class Home;
}

#include "basewidget.hpp"

class SamsungDForumIr::Home : public SamsungDForumIr::BaseWidget
{
private:
    enum ERegisterType {
        ERegisterType_Individual = 0,
        ERegisterType_Company = 1
    };

    enum EIEDULEVEL {
        EIEDULEVEL_NONE = 0,
        EIEDULEVEL_AD = 1,
        EIEDULEVEL_BA = 2,
        EIEDULEVEL_MA = 3,
        EIEDULEVEL_PHD = 4
    };

private:
    Wt::WMenuItem *m_initialSelectedMenuItem;
    Wt::WLineEdit *m_searchLineEdit;

    Wt::WDialog *m_dlg;
    Wt::WMessageBox *m_msg;
    Wt::WDialog *m_dlgProgressBar;

    FileUploadProgressDialog *m_fileUploadProgressDialog;

    bool m_isUserLoggedIn;
    bool m_isIndividualUser;

    Div *m_dvLogin;

    Wt::WLineEdit *m_signInUserNameLineEdit;
    Wt::WLineEdit *m_signInPwdLineEdit;
    Wt::WLineEdit *m_signInCaptchaLineEdit;
    Wt::WImage *m_signInCaptchaImage;
    Wt::WIntValidator *m_signInCaptchaValidator;

    Div *m_dvRegisterForm;
    Wt::WLineEdit *m_registerEmailLineEdit;
    Wt::WLineEdit *m_registerPwdLineEdit;
    Wt::WLineEdit *m_registerPwdConfirmLineEdit;
    Wt::WLineEdit *m_registerIndividualFirstNameLineEdit;
    Wt::WLineEdit *m_registerIndividualLastNameLineEdit;
    Wt::WLineEdit *m_registerIndividualContactNumberLineEdit;
    Wt::WTextArea *m_registerIndividualTeammatesTextArea;
    Wt::WLineEdit *m_registerCompanyNameLineEdit;
    Wt::WLineEdit *m_registerCompanyRegisterationIdLineEdit;
    Wt::WLineEdit *m_registerCompanyPhoneLineEdit;
    Wt::WLineEdit *m_registerCompanyCEONameLineEdit;
    Wt::WLineEdit *m_registerCaptchaLineEdit;
    Wt::WImage *m_registerCaptchaImage;
    Wt::WIntValidator *m_registerCaptchaValidator;
    ERegisterType m_selectedRegisterType;

    Wt::WLineEdit *m_changePwdCurrentLineEdit;
    Wt::WLineEdit *m_changePwdNewLineEdit;
    Wt::WLineEdit *m_changePwdConfirmLineEdit;

    Wt::WLineEdit *m_myProfileIndividualFirstNameLineEdit;
    Wt::WLineEdit *m_myProfileIndividualLastNameLineEdit;
    Wt::WLineEdit *m_myProfileIndividualContactNumberLineEdit;
    Wt::WLineEdit *m_myProfileIndividualAlternativeContactNumberLineEdit;
    Wt::WTextArea *m_myProfileIndividualTeammatesTextArea;
    Wt::WLineEdit *m_myProfileCompanyNameLineEdit;
    Wt::WLineEdit *m_myProfileCompanyRegisterationIdLineEdit;
    Wt::WLineEdit *m_myProfileCompanyPhoneLineEdit;
    Wt::WLineEdit *m_myProfileCompanyAlternativePhoneLineEdit;
    Wt::WLineEdit *m_myProfileCompanyCEONameLineEdit;
    Wt::WLineEdit *m_myProfileIndividualNationalCodeLineEdit;
    Wt::WComboBox *m_myProfileIndividualEducationLevelComboBox;
    Wt::WLineEdit *m_myProfileIndividualEducationSubjectLineEdit;
    Wt::WLineEdit *m_myProfileIndividualWebsiteLineEdit;
    Wt::WLineEdit *m_myProfileCompanyEconomicalCodeLineEdit;
    Wt::WComboBox *m_myProfileCompanyFoundingYearComboBox;
    Wt::WLineEdit *m_myProfileCompanyAddressLineEdit;
    Wt::WLineEdit *m_myProfileCompanyWebsiteLineEdit;
    Wt::WFileUpload *m_myProfileCompanyResumeFileUpload;
    bool m_myProfileCompanyResumeFileUploadHasFile;
    Wt::WPushButton *m_myProfileSaveButton;

    bool m_isInSubmitAppProcess;
    bool m_isInSubmitIdeaProcess;

    Wt::JSignal<> m_jsRegister;
    Wt::JSignal<> m_jsSubmitIdea;
    Wt::JSignal<> m_jsSubmitApp;

public:
    Home(CgiRoot *cgi);
    ~Home();

public:
    Wt::JSignal<> &JSRegister();
    Wt::JSignal<> &JSSubmitIdea();
    Wt::JSignal<> &JSSubmitApp();

private:
    void OnJSRegisterTriggered();
    void OnJSSubmitIdeaTriggered();
    void OnJSSubmitAppTriggered();
    void OnMenuItemSelected(Wt::WMenuItem *sender);
    void OnSearchEditEnterPressed();
    void OnSearchResultsReturn();
    void OnSignInLinkPressed();
    void OnRegisterLinkPressed();
    void OnRegisterTypeRadioButtonCheckedChanged(Wt::WRadioButton *sender);
    void OnSignInOK();
    void OnSignInReturn();
    void OnSignInFailedOK(Wt::StandardButton result);
    void OnSignInRecoverInvalidEmailOK(Wt::StandardButton result);
    void OnSignInRecoverSuccessOK(Wt::StandardButton result);
    void OnSignInNotVerifiedOK(Wt::StandardButton result);
    void OnSignInNotVerifiedResendSuccessOK(Wt::StandardButton result);
    void OnRegisterOK();
    void OnRegisterAlreadyExistsOK(Wt::StandardButton result);
    void OnRegisterSuccessOK(Wt::StandardButton result);
    void OnRegisterReturn();
    void OnMyAppsLinkPressed();
    void OnMyIdeasLinkPressed();
    void OnAppointmentLinkPressed();
    void OnMyProfileLinkPressed();
    void OnMyProfileEducationLevelComboBoxChanged(int index);
    void OnMyProfileFoundingYearComboBoxChanged(int index);
    void OnMyProfileOK();
    void OnMyProfileResumeFileChanged();
    void OnMyProfileResumeFileTooLarge();
    void OnMyProfileResumeFileUploaded();
    void OnMyProfileSaveSuccessOK(Wt::StandardButton result);
    void OnMyProfileSaveFailedOK(Wt::StandardButton result);
    void OnMyProfileReturn();
    void OnPwdLinkPressed();
    void OnChangePwdOK();
    void OnChangePwdInvalidOK();
    void OnChangePwdSuccessOK();
    void OnChangePwdReturn();
    void OnSignOutLinkPressed();
    void OnSubmitIdeaLinkPressed();
    void OnSubmitAppLinkPressed();

private:
    void GenCap(Wt::WImage *captchaImage, Wt::WIntValidator *captchaValidator);

    void GetSubMenues(Wt::WMenu *menu, const std::string &fullpath, Wt::WStackedWidget *dvContents);

    bool IsIndividualUser(std::string email);

    Wt::WWidget *CHome();
    Wt::WWidget *CContact();
    Wt::WWidget *CPageContent(std::string &fullpath);

    Wt::WWidget *GetSignInForm();
    Wt::WWidget *GetRegisterForm();
    Wt::WWidget *GetMyProfileForm();
    Wt::WWidget *GetChangePwdForm();

    void ShowSubmitIdeaDialog();
    void ShowSubmitAppDialog();

private:
    Wt::WWidget *Layout();
};


#endif /* HOME_HPP */


