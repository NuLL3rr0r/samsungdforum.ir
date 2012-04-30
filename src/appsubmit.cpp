/**********************************
*    = Header File Inclusion =
**********************************/

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <Wt/WApplication>
#include <Wt/WButtonGroup>
#include <Wt/WCheckBox>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WFileUpload>
#include <Wt/WLengthValidator>
#include <Wt/WLineEdit>
#include <Wt/WMessageBox>
#include <Wt/WPushButton>
#include <Wt/WRadioButton>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WVBoxLayout>
#include <Wt/WWidget>
#include "appsubmit.hpp"
#include "base.hpp"
#include "cdate.hpp"
#include "cimage.hpp"
#include "crypto.hpp"
#include "system.hpp"


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

namespace fs = boost::filesystem;


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

AppSubmit::AppSubmit(CgiRoot *cgi, Wt::WDialog *parentDialog, bool editMode, size_t editRowId) : BaseWidget(cgi),
    m_dlgParent(parentDialog),
    m_editMode(editMode),
    m_editRowId(editRowId),
    m_editCompletedHandler(this)
{
    m_scr1stFileUploadHasFile = false;
    m_scr2ndFileUploadHasFile = false;
    m_scr3rdFileUploadHasFile = false;
    m_scr4thFileUploadHasFile = false;
    m_binaryFileUploadHasFile = false;

    m_scr1stFileUploaded = false;
    m_scr2ndFileUploaded = false;
    m_scr3rdFileUploaded = false;
    m_scr4thFileUploaded = false;
    m_binaryFileUploaded = false;


    this->clear();
    this->addWidget(Layout());

    this->setOverflow(WContainerWidget::OverflowAuto);

    Div *dvButtonWrappers = new Div();
    Div *dvButtons = new Div(dvButtonWrappers, "dvDialogButtons");
    if (!m_editMode)
        m_submitButton = new WPushButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_OK_BUTTON"), dvButtons);
    else
        m_submitButton = new WPushButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_OK_EDIT_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_RETURN_BUTTON"), dvButtons);

    m_submitButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    WVBoxLayout *layout = new WVBoxLayout();

    layout->addWidget(this);
    layout->setStretchFactor(this, 1);

    layout->addWidget(dvButtonWrappers );
    layout->setStretchFactor(dvButtonWrappers , 0);


    m_dlgParent->contents()->setLayout(layout);

    if (m_editMode) {
        string email;
        string title;
        int type;
        string description;
        int displaytype;
        int platformhtml5;
        int platformjavascript;
        int platformflash;
        int tdsupportiprange;
        int tdpwprotected;
        int tdhascookies;
        int tdautologin;
        int tdhasfilm;
        int tdhasaudio;
        int avnoaudiovideo;
        int avnobuffer;
        int avwithplayback;
        int avfullscreenwithexit;
        string language;
        string scr1st;
        string scr2nd;
        string scr3rd;
        string scr4th;
        string scrdescription;
        int remotenums;
        int remotearrows;
        int remotetoolsinfo;
        int remotereturnexit;
        int remoteplayback;
        int remotecolorstuning;
        int remotevolumestuning;
        string binary;

        result r = m_db->Sql() << "SELECT title, type, description, displaytype, "
                                  "platformhtml5, platformjavascript, platformflash, "
                                  "tdsupportiprange, tdpwprotected, tdhascookies, tdautologin, tdhasfilm, tdhasaudio, "
                                  "avnoaudiovideo, avnobuffer, avwithplayback, avfullscreenwithexit, "
                                  "language, scr1st, scr2nd, scr3rd, scr4th, scrdescription, "
                                  "remotenums, remotearrows, remotetoolsinfo, remotereturnexit, remoteplayback, "
                                  "remotecolorstuning, remotevolumestuning, "
                                  "binary FROM [" + m_dbTables->Table("APPS")
                                  + "] WHERE rowid=?;" << m_editRowId << row;
        if (!r.empty()) {
            r >> title >> type >> description >> displaytype
              >> platformhtml5 >> platformjavascript >> platformflash
              >> tdsupportiprange >> tdpwprotected >> tdhascookies >> tdautologin >> tdhasfilm >> tdhasaudio
              >> avnoaudiovideo >> avnobuffer >> avwithplayback >> avfullscreenwithexit
              >> language >> scr1st >> scr2nd >> scr3rd >> scr4th >> scrdescription
              >> remotenums >> remotearrows >> remotetoolsinfo >> remotereturnexit >> remoteplayback
              >> remotecolorstuning >> remotevolumestuning
              >> binary;

            title = Crypto::Decrypt(title);
            description = Crypto::Decrypt(description);
            language = Crypto::Decrypt(language);
            scrdescription = Crypto::Decrypt(scrdescription);

            m_titleLineEdit->setText(WString::fromUTF8(title));
            m_typeComboBox->setCurrentIndex(type);
            m_descriptionTextArea->setText(WString::fromUTF8(description));
            m_displayTypeComboBox->setCurrentIndex(displaytype);
            m_platformHTML5CheckBox->setChecked(platformhtml5);
            m_platformJavaScriptCheckBox->setChecked(platformjavascript);
            m_platformFlashCheckBox->setChecked(platformflash);
            m_tdSupportIPRangeButtonGroup->setSelectedButtonIndex(tdsupportiprange);
            m_tdPwdProtectedButtonGroup->setSelectedButtonIndex(tdpwprotected);
            m_tdHasCookiesButtonGroup->setSelectedButtonIndex(tdhascookies);
            m_tdAutoLoginButtonGroup->setSelectedButtonIndex(tdautologin);
            m_tdHasFilmButtonGroup->setSelectedButtonIndex(tdhasfilm);
            m_tdHasAudioButtonGroup->setSelectedButtonIndex(tdhasaudio);
            m_avNoAudioVideoCheckBox->setChecked(avnoaudiovideo);
            m_avNoBufferCheckBox->setChecked(avnobuffer);
            m_avWithPlaybackCheckBox->setChecked(avwithplayback);
            m_avFullScreenWithExitCheckBox->setChecked(avfullscreenwithexit);
            m_languageLineEdit->setText(WString::fromUTF8(language));
            m_scrDescriptionTextArea->setText(WString::fromUTF8(scrdescription));
            m_remoteNumsCheckBox->setChecked(remotenums);
            m_remoteArrowsCheckBox->setChecked(remotearrows);
            m_remoteToolsInfoCheckBox->setChecked(remotetoolsinfo);
            m_remoteReturnExitCheckBox->setChecked(remotereturnexit);
            m_remotePlaybackCheckBox->setChecked(remoteplayback);
            m_remoteColorsTuningCheckBox->setChecked(remotecolorstuning);
            m_remoteVolumesTuningCheckBox->setChecked(remotevolumestuning);

            m_oldScr1st = scr1st;
            m_oldScr2nd = scr2nd;
            m_oldScr3rd = scr3rd;
            m_oldScr4th = scr4th;
            m_oldBinary = binary;

            OnTypeComboBoxChanged(m_typeComboBox->currentIndex());
            OnDisplayTypeComboBoxChanged(m_displayTypeComboBox->currentIndex());
        }
    }

    m_submitButton->clicked().connect(this, &AppSubmit::OnSubmitButtonPressed);
    returnButton->clicked().connect(this, &AppSubmit::OnReturnButtonPressed);

    m_titleLineEdit->setFocus();
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

void AppSubmit::OnTypeComboBoxChanged(int index)
{
    if (index != EAppType_None) {
        m_typeComboBox->removeStyleClass("Wt-invalid", true);
    } else {
        m_typeComboBox->setStyleClass("Wt-invalid");
    }
}

void AppSubmit::OnDisplayTypeComboBoxChanged(int index)
{
    if (index != EDisplayType_None) {
        m_displayTypeComboBox->removeStyleClass("Wt-invalid", true);
    } else {
        m_displayTypeComboBox->setStyleClass("Wt-invalid");
    }
}

void AppSubmit::OnScr1stFileChanged()
{
    m_scr1stFileUploadHasFile = true;
}

void AppSubmit::OnScr2ndFileChanged()
{
    m_scr2ndFileUploadHasFile = true;
}

void AppSubmit::OnScr3rdFileChanged()
{
    m_scr3rdFileUploadHasFile = true;
}

void AppSubmit::OnScr4thFileChanged()
{
    m_scr4thFileUploadHasFile = true;
}

void AppSubmit::OnBinaryFileChanged()
{
    m_binaryFileUploadHasFile = true;
}

void AppSubmit::OnScr1stFileTooLarge()
{
    m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_FILE_TOO_LARGE_ERR_TITLE"),
                            m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_FILE_TOO_LARGE_ERR_BODY"),
                            Warning, NoButton);
    m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_FILE_TOO_LARGE_ERR_OK_BUTTON"), Ok);
    m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
    m_msg->show();
}

void AppSubmit::OnScr2ndFileTooLarge()
{
    m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_FILE_TOO_LARGE_ERR_TITLE"),
                            m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_FILE_TOO_LARGE_ERR_BODY"),
                            Warning, NoButton);
    m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_FILE_TOO_LARGE_ERR_OK_BUTTON"), Ok);
    m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
    m_msg->show();
}

void AppSubmit::OnScr3rdFileTooLarge()
{
    m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_FILE_TOO_LARGE_ERR_TITLE"),
                            m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_FILE_TOO_LARGE_ERR_BODY"),
                            Warning, NoButton);
    m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_FILE_TOO_LARGE_ERR_OK_BUTTON"), Ok);
    m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
    m_msg->show();
}

void AppSubmit::OnScr4thFileTooLarge()
{
    m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_FILE_TOO_LARGE_ERR_TITLE"),
                            m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_FILE_TOO_LARGE_ERR_BODY"),
                            Warning, NoButton);
    m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_FILE_TOO_LARGE_ERR_OK_BUTTON"), Ok);
    m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
    m_msg->show();
}

void AppSubmit::OnBinaryFileTooLarge()
{
    m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_BINARY_FILE_TOO_LARGE_ERR_TITLE"),
                            m_lang->GetString("HOME_SUBMIT_APP_DLG_BINARY_FILE_TOO_LARGE_ERR_BODY"),
                            Warning, NoButton);
    m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_BINARY_FILE_TOO_LARGE_ERR_OK_BUTTON"), Ok);
    m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
    m_msg->show();
}

void AppSubmit::OnScr1stFileUploaded()
{
    m_scr1stFileUploaded = true;
    m_scr2ndFileUpload->upload();
}

void AppSubmit::OnScr2ndFileUploaded()
{
    m_scr2ndFileUploaded = true;
    m_scr3rdFileUpload->upload();
}

void AppSubmit::OnScr3rdFileUploaded()
{
    m_scr3rdFileUploaded = true;
    m_scr4thFileUpload->upload();
}

void AppSubmit::OnScr4thFileUploaded()
{
    m_scr4thFileUploaded = true;
    m_binaryFileUpload->upload();
}

void AppSubmit::OnBinaryFileUploaded()
{
    m_binaryFileUploaded = true;

    if (!CImage::IsScreenshotFileSizeValid(m_scr1stFileUpload->spoolFileName())) {
        OnScr1stFileTooLarge();
        return;
    }

    if (!CImage::IsScreenshotFileSizeValid(m_scr2ndFileUpload->spoolFileName())) {
        OnScr2ndFileTooLarge();
        return;
    }

    if (!CImage::IsScreenshotFileSizeValid(m_scr3rdFileUpload->spoolFileName())) {
        OnScr3rdFileTooLarge();
        return;
    }

    if (!CImage::IsScreenshotFileSizeValid(m_scr4thFileUpload->spoolFileName())) {
        OnScr4thFileTooLarge();
        return;
    }

    if (!CImage::IsImage(m_scr1stFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_FORMAT_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_FORMAT_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_FORMAT_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!CImage::IsImage(m_scr2ndFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_FORMAT_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_FORMAT_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_FORMAT_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!CImage::IsImage(m_scr3rdFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_FORMAT_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_FORMAT_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_FORMAT_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!CImage::IsImage(m_scr4thFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_FORMAT_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_FORMAT_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_FORMAT_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!CImage::IsScreenshotDpiValid(m_scr1stFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_DPI_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_DPI_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_DPI_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!CImage::IsScreenshotDpiValid(m_scr2ndFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_DPI_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_DPI_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_DPI_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!CImage::IsScreenshotDpiValid(m_scr3rdFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_DPI_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_DPI_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_DPI_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!CImage::IsScreenshotDpiValid(m_scr4thFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_DPI_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_DPI_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_DPI_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!CImage::IsScreenshotResolutionValid(m_scr1stFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_RESOLUTION_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_RESOLUTION_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_RESOLUTION_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!CImage::IsScreenshotResolutionValid(m_scr2ndFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_RESOLUTION_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_RESOLUTION_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_RESOLUTION_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!CImage::IsScreenshotResolutionValid(m_scr3rdFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_RESOLUTION_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_RESOLUTION_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_RESOLUTION_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!CImage::IsScreenshotResolutionValid(m_scr4thFileUpload->spoolFileName())) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_RESOLUTION_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_RESOLUTION_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_RESOLUTION_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if(System::FileSize(m_binaryFileUpload->spoolFileName()) >= Base::FILE_SIZE_MAX_APP_BIN) {
        OnBinaryFileTooLarge();
        return;
    }

    wstring clientFileName(m_binaryFileUpload->clientFileName().value());
    wstring clientFileExt;
    size_t clientFileDotPos = clientFileName.find_last_of(L".");
    if (clientFileDotPos != string::npos) {
        clientFileExt = to_lower_copy(clientFileName.substr(clientFileDotPos + 1));
    }

    if (clientFileExt != "zip") {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_BINARY_INVALID_ZIP_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_BINARY_INVALID_ZIP_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_BINARY_INVALID_ZIP_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    CDate::Now n;
    string prefix(replace_all_copy(CDate::DateConv::ToJalali(n), "/", "-")
                  + "_"
                  + replace_all_copy(CDate::DateConv::Time(n), ":", "-")
                  + "__");

    string userDir(Base::FSDB_USERS_PATH + "/" + m_cgiEnv->LoggedInUserName);

    string scr1stURL(userDir + "/" + prefix + Base::APP_SCR_1ST_FILE_NAME);
    string scr2ndURL(userDir + "/" + prefix + Base::APP_SCR_2ND_FILE_NAME);
    string scr3rdURL(userDir + "/" + prefix + Base::APP_SCR_3RD_FILE_NAME);
    string scr4thURL(userDir + "/" + prefix + Base::APP_SCR_4TH_FILE_NAME);
    string binURL(userDir + "/" + prefix + Base::APP_BIN_FILE_NAME);

    try {
        if (!fs::exists(userDir)) {
            fs::create_directories(userDir);
        }
    }
    catch(const fs::filesystem_error &ex) {
        ERR(ex.what());
    }
    catch(...) {
    }

    try {
        if (CImage::IsJPEG(m_scr1stFileUpload->spoolFileName())) {
            System::CopyFile(m_scr1stFileUpload->spoolFileName(), scr1stURL);
            //m_scr1stFileUpload->stealSpooledFile();
        } else {
            CImage::SaveAsJPEG(m_scr1stFileUpload->spoolFileName(), scr1stURL);
        }
    }
    catch(const fs::filesystem_error &ex) {
        ERR(ex.what());
    }
    catch(...) {
    }

    try {
        if (CImage::IsJPEG(m_scr2ndFileUpload->spoolFileName())) {
            System::CopyFile(m_scr2ndFileUpload->spoolFileName(), scr2ndURL);
            //m_scr2ndFileUpload->stealSpooledFile();
        } else {
            CImage::SaveAsJPEG(m_scr2ndFileUpload->spoolFileName(), scr2ndURL);
        }
    }
    catch(const fs::filesystem_error &ex) {
        ERR(ex.what());
    }
    catch(...) {
    }

    try {
        if (CImage::IsJPEG(m_scr3rdFileUpload->spoolFileName())) {
            System::CopyFile(m_scr3rdFileUpload->spoolFileName(), scr3rdURL);
            //m_scr3rdFileUpload->stealSpooledFile();
        } else {
            CImage::SaveAsJPEG(m_scr3rdFileUpload->spoolFileName(), scr3rdURL);
        }
    }
    catch(const fs::filesystem_error &ex) {
        ERR(ex.what());
    }
    catch(...) {
    }

    try {
        if (CImage::IsJPEG(m_scr4thFileUpload->spoolFileName())) {
            System::CopyFile(m_scr4thFileUpload->spoolFileName(), scr4thURL);
            //m_scr4thFileUpload->stealSpooledFile();
        } else {
            CImage::SaveAsJPEG(m_scr4thFileUpload->spoolFileName(), scr4thURL);
        }
    }
    catch(const fs::filesystem_error &ex) {
        ERR(ex.what());
    }
    catch(...) {
    }

    try {
        System::CopyFile(m_binaryFileUpload->spoolFileName(), binURL);
        //m_binaryFileUpload->stealSpooledFile();
    }
    catch(const fs::filesystem_error &ex) {
        ERR(ex.what());
    }
    catch(...) {
    }


    if (!m_editMode) {
        m_db->Insert(m_dbTables->Table("APPS"), "email, title, type, description, displaytype, "
                     "platformhtml5, platformjavascript, platformflash, "
                     "tdsupportiprange, tdpwprotected, tdhascookies, tdautologin, tdhasfilm, tdhasaudio, "
                     "avnoaudiovideo, avnobuffer, avwithplayback, avfullscreenwithexit, "
                     "language, scr1st, scr2nd, scr3rd, scr4th, scrdescription, "
                     "remotenums, remotearrows, remotetoolsinfo, remotereturnexit, remoteplayback, remotecolorstuning, remotevolumestuning, "
                     "binary",
                     32, m_cgiEnv->LoggedInUserName.c_str(),
                     Crypto::Encrypt(m_titleLineEdit->text().toUTF8()).c_str(),
                     lexical_cast<string>(m_typeComboBox->currentIndex()).c_str(),
                     Crypto::Encrypt(m_descriptionTextArea->text().toUTF8()).c_str(),
                     lexical_cast<string>(m_displayTypeComboBox->currentIndex()).c_str(),
                     lexical_cast<string>(m_platformHTML5CheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_platformJavaScriptCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_platformFlashCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_tdSupportIPRangeButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_tdPwdProtectedButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_tdHasCookiesButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_tdAutoLoginButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_tdHasFilmButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_tdHasAudioButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_avNoAudioVideoCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_avNoBufferCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_avWithPlaybackCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_avFullScreenWithExitCheckBox->isChecked()).c_str(),
                     Crypto::Encrypt(m_languageLineEdit->text().toUTF8()).c_str(),
                     scr1stURL.c_str(),
                     scr2ndURL.c_str(),
                     scr3rdURL.c_str(),
                     scr4thURL.c_str(),
                     Crypto::Encrypt(m_scrDescriptionTextArea->text().toUTF8()).c_str(),
                     lexical_cast<string>(m_remoteNumsCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remoteArrowsCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remoteToolsInfoCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remoteReturnExitCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remotePlaybackCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remoteColorsTuningCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remoteVolumesTuningCheckBox->isChecked()).c_str(),
                     binURL.c_str()
                     );

        m_msg = new WMessageBox(L"ارسال شد",
                                L"با تشکر، برنامه شما با موفقیت ارسال شد.", Warning, NoButton);
        m_msg->addButton(L"تائید", Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitSuccessOK);
        m_msg->show();
    } else {
        m_db->Update(m_dbTables->Table("APPS"), "rowid", lexical_cast<string>(m_editRowId),
                     "title=?, type=?, description=?, displaytype=?, "
                     "platformhtml5=?, platformjavascript=?, platformflash=?, "
                     "tdsupportiprange=?, tdpwprotected=?, tdhascookies=?, tdautologin=?, tdhasfilm=?, tdhasaudio=?, "
                     "avnoaudiovideo=?, avnobuffer=?, avwithplayback=?, avfullscreenwithexit=?, "
                     "language=?, scr1st=?, scr2nd=?, scr3rd=?, scr4th=?, scrdescription=?, "
                     "remotenums=?, remotearrows=?, remotetoolsinfo=?, remotereturnexit=?, remoteplayback=?, remotecolorstuning=?, remotevolumestuning=?, "
                     "binary=?",
                     31,
                     Crypto::Encrypt(m_titleLineEdit->text().toUTF8()).c_str(),
                     lexical_cast<string>(m_typeComboBox->currentIndex()).c_str(),
                     Crypto::Encrypt(m_descriptionTextArea->text().toUTF8()).c_str(),
                     lexical_cast<string>(m_displayTypeComboBox->currentIndex()).c_str(),
                     lexical_cast<string>(m_platformHTML5CheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_platformJavaScriptCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_platformFlashCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_tdSupportIPRangeButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_tdPwdProtectedButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_tdHasCookiesButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_tdAutoLoginButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_tdHasFilmButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_tdHasAudioButtonGroup->checkedId()).c_str(),
                     lexical_cast<string>(m_avNoAudioVideoCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_avNoBufferCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_avWithPlaybackCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_avFullScreenWithExitCheckBox->isChecked()).c_str(),
                     Crypto::Encrypt(m_languageLineEdit->text().toUTF8()).c_str(),
                     scr1stURL.c_str(),
                     scr2ndURL.c_str(),
                     scr3rdURL.c_str(),
                     scr4thURL.c_str(),
                     Crypto::Encrypt(m_scrDescriptionTextArea->text().toUTF8()).c_str(),
                     lexical_cast<string>(m_remoteNumsCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remoteArrowsCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remoteToolsInfoCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remoteReturnExitCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remotePlaybackCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remoteColorsTuningCheckBox->isChecked()).c_str(),
                     lexical_cast<string>(m_remoteVolumesTuningCheckBox->isChecked()).c_str(),
                     binURL.c_str()
                     );

        System::EraseFile(m_oldScr1st);
        System::EraseFile(m_oldScr2nd);
        System::EraseFile(m_oldScr3rd);
        System::EraseFile(m_oldScr4th);
        System::EraseFile(m_oldBinary);

        m_editCompletedHandler.emit();

        m_msg = new WMessageBox(L"ذخیره شد",
                                L"با تشکر، تغییرات شما با موفقیت ذخیره شد.", Warning, NoButton);
        m_msg->addButton(L"تائید", Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitSuccessOK);
        m_msg->show();
    }

}

void AppSubmit::OnSubmitButtonPressed()
{
    if (!Validate(m_titleLineEdit)
            || !Validate(m_descriptionTextArea)
            || !Validate(m_languageLineEdit)
            || !Validate(m_scrDescriptionTextArea)) {
        return;
    }

    if (m_typeComboBox->currentIndex() == EAppType_None) {
        m_typeComboBox->setFocus();
        return;
    }

    if (m_displayTypeComboBox->currentIndex() == EDisplayType_None) {
        m_displayTypeComboBox->setFocus();
        return;
    }

    if (!m_platformHTML5CheckBox->isChecked()
            && !m_platformJavaScriptCheckBox->isChecked()
            && !m_platformFlashCheckBox->isChecked()) {
        m_platformHTML5CheckBox->setFocus();
        return;
    }

    if ((EYNQuestion)m_tdSupportIPRangeButtonGroup->checkedId() == EYNQuestion_None) {
        m_tdSupportIPRangeYesRadioButton->setFocus();
        return;
    }

    if ((EYNQuestion)m_tdPwdProtectedButtonGroup->checkedId() == EYNQuestion_None) {
        m_tdPwdProtectedYesRadioButton->setFocus();
        return;
    }

    if ((EYNQuestion)m_tdHasCookiesButtonGroup->checkedId() == EYNQuestion_None) {
        m_tdHasCookiesYesRadioButton->setFocus();
        return;
    }

    if ((EYNQuestion)m_tdAutoLoginButtonGroup->checkedId() == EYNQuestion_None) {
        m_tdAutoLoginYesRadioButton->setFocus();
        return;
    }

    if ((EYNQuestion)m_tdHasFilmButtonGroup->checkedId() == EYNQuestion_None) {
        m_tdHasFilmYesRadioButton->setFocus();
        return;
    }

    if ((EYNQuestion)m_tdHasAudioButtonGroup->checkedId() == EYNQuestion_None) {
        m_tdHasAudioYesRadioButton->setFocus();
        return;
    }

    if (!m_avNoAudioVideoCheckBox->isChecked()
            && !m_avNoBufferCheckBox->isChecked()
            && !m_avWithPlaybackCheckBox->isChecked()
            && !m_avFullScreenWithExitCheckBox->isChecked()) {
        m_avNoAudioVideoCheckBox->setFocus();
        return;
    }

    if (!m_remoteNumsCheckBox->isChecked()
            && !m_remoteArrowsCheckBox->isChecked()
            && !m_remoteToolsInfoCheckBox->isChecked()
            && !m_remoteReturnExitCheckBox->isChecked()
            && !m_remotePlaybackCheckBox->isChecked()
            && !m_remoteColorsTuningCheckBox->isChecked()
            && !m_remoteVolumesTuningCheckBox->isChecked()) {
        m_remoteNumsCheckBox->setFocus();
        return;
    }

    if (!m_scr1stFileUploadHasFile
            || !m_scr2ndFileUploadHasFile
            || !m_scr3rdFileUploadHasFile
            || !m_scr4thFileUploadHasFile) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_NO_FILE_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR_NO_FILE_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_SCR__FILE_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!m_binaryFileUploadHasFile) {
        m_msg = new WMessageBox(m_lang->GetString("HOME_SUBMIT_APP_DLG_BINARY_NO_FILE_ERR_TITLE"),
                                m_lang->GetString("HOME_SUBMIT_APP_DLG_BINARY_NO_FILE_ERR_BODY"),
                                Warning, NoButton);
        m_msg->addButton(m_lang->GetString("HOME_SUBMIT_APP_DLG_BINARY_NO_FILE_ERR_OK_BUTTON"), Ok);
        m_msg->buttonClicked().connect(this, &AppSubmit::OnSubmitFailedOK);
        m_msg->show();

        return;
    }


    // We need to disable this button upon click inorder
    // to make FileUploads work correctly
    m_submitButton->setDisabled(true);

    m_scr1stFileUpload->upload();
}

void AppSubmit::OnReturnButtonPressed()
{
    m_dlgParent->hide();
}

void AppSubmit::OnSubmitFailedOK()
{
    delete m_msg;
    m_msg = NULL;

    if (m_submitButton->isDisabled())
        m_submitButton->setEnabled(true);
}

void AppSubmit::OnSubmitSuccessOK()
{
    delete m_msg;
    m_msg = NULL;

    m_dlgParent->hide();
}


/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

Signal<> &AppSubmit::EditCompletedHandler()
{
    return m_editCompletedHandler;
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *AppSubmit::Layout()
{
    Div *root = new Div("AppSubmit");

    m_titleLineEdit = new WLineEdit();
    m_typeComboBox = new WComboBox();
    m_descriptionTextArea = new WTextArea();
    m_displayTypeComboBox = new WComboBox();
    m_platformHTML5CheckBox = new WCheckBox();
    m_platformJavaScriptCheckBox = new WCheckBox();
    m_platformFlashCheckBox = new WCheckBox();
    m_tdSupportIPRangeYesRadioButton = new WRadioButton();
    m_tdSupportIPRangeNoRadioButton = new WRadioButton();
    m_tdPwdProtectedYesRadioButton = new WRadioButton();
    m_tdPwdProtectedNoRadioButton = new WRadioButton();
    m_tdHasCookiesYesRadioButton = new WRadioButton();
    m_tdHasCookiesNoRadioButton = new WRadioButton();
    m_tdAutoLoginYesRadioButton = new WRadioButton();
    m_tdAutoLoginNoRadioButton = new WRadioButton();
    m_tdHasFilmYesRadioButton = new WRadioButton();
    m_tdHasFilmNoRadioButton = new WRadioButton();
    m_tdHasAudioYesRadioButton = new WRadioButton();
    m_tdHasAudioNoRadioButton = new WRadioButton();
    m_avNoAudioVideoCheckBox = new WCheckBox();
    m_avNoBufferCheckBox = new WCheckBox();
    m_avWithPlaybackCheckBox = new WCheckBox();
    m_avFullScreenWithExitCheckBox = new WCheckBox();
    m_languageLineEdit = new WLineEdit();
    m_scr1stFileUpload = new WFileUpload();
    m_scr2ndFileUpload = new WFileUpload();
    m_scr3rdFileUpload = new WFileUpload();
    m_scr4thFileUpload = new WFileUpload();
    m_scrDescriptionTextArea = new WTextArea();
    m_remoteNumsCheckBox = new WCheckBox();
    m_remoteArrowsCheckBox = new WCheckBox();
    m_remoteToolsInfoCheckBox = new WCheckBox();
    m_remoteReturnExitCheckBox = new WCheckBox();
    m_remotePlaybackCheckBox = new WCheckBox();
    m_remoteColorsTuningCheckBox = new WCheckBox();
    m_remoteVolumesTuningCheckBox = new WCheckBox();
    m_binaryFileUpload = new WFileUpload();

    m_tdSupportIPRangeButtonGroup = new Wt::WButtonGroup(this);
    m_tdPwdProtectedButtonGroup = new Wt::WButtonGroup(this);
    m_tdHasCookiesButtonGroup = new Wt::WButtonGroup(this);
    m_tdAutoLoginButtonGroup = new Wt::WButtonGroup(this);
    m_tdHasFilmButtonGroup = new Wt::WButtonGroup(this);
    m_tdHasAudioButtonGroup = new Wt::WButtonGroup(this);

    m_tdSupportIPRangeButtonGroup->addButton(m_tdSupportIPRangeYesRadioButton, EYNQuestion_Yes);
    m_tdSupportIPRangeButtonGroup->addButton(m_tdSupportIPRangeNoRadioButton, EYNQuestion_No);
    m_tdPwdProtectedButtonGroup->addButton(m_tdPwdProtectedYesRadioButton, EYNQuestion_Yes);
    m_tdPwdProtectedButtonGroup->addButton(m_tdPwdProtectedNoRadioButton, EYNQuestion_No);
    m_tdHasCookiesButtonGroup->addButton(m_tdHasCookiesYesRadioButton, EYNQuestion_Yes);
    m_tdHasCookiesButtonGroup->addButton(m_tdHasCookiesNoRadioButton, EYNQuestion_No);
    m_tdAutoLoginButtonGroup->addButton(m_tdAutoLoginYesRadioButton, EYNQuestion_Yes);
    m_tdAutoLoginButtonGroup->addButton(m_tdAutoLoginNoRadioButton, EYNQuestion_No);
    m_tdHasFilmButtonGroup->addButton(m_tdHasFilmYesRadioButton, EYNQuestion_Yes);
    m_tdHasFilmButtonGroup->addButton(m_tdHasFilmNoRadioButton, EYNQuestion_No);
    m_tdHasAudioButtonGroup->addButton(m_tdHasAudioYesRadioButton, EYNQuestion_Yes);
    m_tdHasAudioButtonGroup->addButton(m_tdHasAudioNoRadioButton, EYNQuestion_No);

    m_typeComboBox->addItem(m_lang->GetString("HOME_SUBMIT_APP_DLG_APP_TYPE_NONE_TEXT"));
    m_typeComboBox->addItem(m_lang->GetString("HOME_SUBMIT_APP_DLG_APP_TYPE_GAME_ENTERTAINMENT_TEXT"));
    m_typeComboBox->addItem(m_lang->GetString("HOME_SUBMIT_APP_DLG_APP_TYPE_ONLINE_GAME_TEXT"));
    m_typeComboBox->addItem(m_lang->GetString("HOME_SUBMIT_APP_DLG_APP_TYPE_LEARN_INFO_TEXT"));
    m_typeComboBox->addItem(m_lang->GetString("HOME_SUBMIT_APP_DLG_APP_TYPE_SERVICES_TEXT"));
    m_typeComboBox->addItem(m_lang->GetString("HOME_SUBMIT_APP_DLG_APP_TYPE_TOOLS_TEXT"));

    m_displayTypeComboBox->addItem(m_lang->GetString("HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_NONE_TEXT"));
    m_displayTypeComboBox->addItem(m_lang->GetString("HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_FULLSCREEN_TEXT"));
    m_displayTypeComboBox->addItem(m_lang->GetString("HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_ON_CORNER_OF_SCREEN_TEXT"));
    m_displayTypeComboBox->addItem(m_lang->GetString("HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_ACTIVE_ON_SCREEN_TEXT"));

    m_titleLineEdit->resize(200, WLength::Auto);
    m_typeComboBox->resize(200, WLength::Auto);
    m_descriptionTextArea->resize(350, 150);
    m_displayTypeComboBox->resize(200, WLength::Auto);
    m_languageLineEdit->resize(200, WLength::Auto);
    m_scrDescriptionTextArea->resize(350, 150);


    new WText(L"<h4>فرم مسابقه تولید نرم‌افزار</h4>", root);

    new WText(L"<hr />", root);
    new WBreak(root);
    new WText(L"عنوان نرم افزار", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_titleLineEdit);
    new WBreak(root);
    new WBreak(root);
    new WText(L"راهنما: نام نرم‌افزار بیش از ۳۰ کاراکتر نباشد. بگونه‌ای انتخابش کنید که جذاب‌ باشد و منحصر به فرد باشد.", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"<hr />", root);
    new WBreak(root);
    new WText(L"دسته بندی نرم‌افزار", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_typeComboBox);
    new WBreak(root);
    new WBreak(root);
    new WText(L"راهنما: دسته‌بندی نرم‌افزارتان را بدقت انتخاب کنید. به جای کاربر فکر کنید. کاربر برای یافتن نرم‌افزاری با قابلیت نرم‌افزار شما، به کدام گزینه رجوع خواهد کرد؟", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"<hr />", root);
    new WBreak(root);
    new WText(L"توضیح کامل نرم‌افزار", root);
    new WBreak(root);
    new WBreak(root);
    root->addWidget(m_descriptionTextArea);
    new WBreak(root);
    new WBreak(root);
    new WText(L"راهنما: هدف نرم‌افزار، محتوای آن، خصوصیات و قابلیت‌های نرم‌افزارتان را توضیح دهید.", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"<hr />", root);
    new WBreak(root);
    new WText(L"صفحه نمایش نرم‌افزار چگونه است؟", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_displayTypeComboBox);
    new WBreak(root);
    new WBreak(root);

    WTable *table = new WTable(root);
    table->setStyleClass("tbl");

    table->elementAt(0, 0)->addWidget(new WText(L"نرم‌افزار صفحه کامل"));
    table->elementAt(0, 1)->addWidget(new WText(L"نرم افزار از کل صفحه نمایش برای اجرا استفاده می‌کند"));
    table->elementAt(1, 0)->addWidget(new WText(L"در گوشه‌ای از صفحه نمایش اجرا می‌شود"));
    table->elementAt(1, 1)->addWidget(new WText(L"نرم‌افزار تنها گوشه‌ای  از صفحه تصویر را برای اجرا استفاده می‌کند"));
    table->elementAt(2, 0)->addWidget(new WText(L"فعال بر روي صفحه نمايش"));
    table->elementAt(2, 1)->addWidget(new WText(L"نرم‌افزار در حالی که دیگر برنامه‌های در حال استفاده‌اند در جلو بقیه نرم‌افزار‌ها فعال می‌ماند"));

    new WBreak(root);
    new WText(L"<hr />", root);
    new WBreak(root);
    new WText(L"نرم‌افزار شما مبتنی بر کدام یک از موارد زیر است؟", root);
    new WBreak(root);
    new WBreak(root);
    root->addWidget(m_platformHTML5CheckBox);
    new WText(L"HTML5", root);
    new WBreak(root);
    root->addWidget(m_platformJavaScriptCheckBox);
    new WText(L"JavaScript", root);
    new WBreak(root);
    root->addWidget(m_platformFlashCheckBox);
    new WText(L"Flash", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"<hr />", root);
    new WBreak(root);
    new WText(L"توضیحات فنی نرم‌افزار", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"آیا نرم‌افزار محدوده خاصی از IP را پشتیبانی می‌کند", root);
    new WBreak(root);
    root->addWidget(m_tdSupportIPRangeYesRadioButton);
    new WText(L"بلی", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_tdSupportIPRangeNoRadioButton);
    new WText(L"خیر", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"آیا نرم‌افزار با رمز عبور محافظت می‌شود", root);
    new WBreak(root);
    root->addWidget(m_tdPwdProtectedYesRadioButton);
    new WText(L"بلی", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_tdPwdProtectedNoRadioButton);
    new WText(L"خیر", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"آیا نرم‌افزار بر روی تلویزیون کوکی به همراه دارد", root);
    new WBreak(root);
    root->addWidget(m_tdHasCookiesYesRadioButton);
    new WText(L"بلی", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_tdHasCookiesNoRadioButton);
    new WText(L"خیر", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"آیا نرم‌افزار قابلیت لاگین خودکار دارد", root);
    new WBreak(root);
    root->addWidget(m_tdAutoLoginYesRadioButton);
    new WText(L"بلی", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_tdAutoLoginNoRadioButton);
    new WText(L"خیر", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"آیا درون نرم‌افزار فیلم تصویری وجود دارد", root);
    new WBreak(root);
    root->addWidget(m_tdHasFilmYesRadioButton);
    new WText(L"بلی", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_tdHasFilmNoRadioButton);
    new WText(L"خیر", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"آیا نرم‌افزار دارای صوت و یا موسیقی است؟", root);
    new WBreak(root);
    root->addWidget(m_tdHasAudioYesRadioButton);
    new WText(L"بلی", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_tdHasAudioNoRadioButton);
    new WText(L"خیر", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"<hr />", root);
    new WBreak(root);
    new WText(L"وضعیت صوت و تصویر نرم‌افزار", root);
    new WBreak(root);
    new WBreak(root);

    root->addWidget(m_avNoAudioVideoCheckBox);
    new WText(L"نرم‌افزار فیلم و یا صوت ندارد", root);
    new WBreak(root);
    root->addWidget(m_avNoBufferCheckBox);
    new WText(L"صوت یا فیلم به صورت یکنواخت در حال اجرا است و صوت و فیلم به همراه نرم‌افزار بوده و به بافر کردن و یا تنظیم کیفیت آن توسط کاربر نیاز نیست", root);
    new WBreak(root);
    root->addWidget(m_avWithPlaybackCheckBox);
    new WText(L"نرم‌افزار قابلیت کنترل صوت و تصویر را دارد و پلی بک دارد", root);
    new WBreak(root);
    root->addWidget(m_avFullScreenWithExitCheckBox);
    new WText(L"نرم‌افزار برای پخش تصویر دارای قابلیت تمام صفحه است و نیز گزینه خروج از حالت تمام صفحه را دارد", root);
    new WBreak(root);
    new WBreak(root);


    new WText(L"<hr />", root);
    new WBreak(root);
    new WText(L"زبان نرم‌افزار", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_languageLineEdit);
    new WBreak(root);
    new WBreak(root);

    new WText(L"<hr />", root);
    new WBreak(root);
    new WText(L"نمای تمام صفحات واسط کاربر نرم‌افزار را بارگذاری کنید:", root);
    new WBreak(root);
    new WBreak(root);
    new WText(L"حداکثر رزولوشن و حجم قابل قبول برای هر تصویر ۱۹۲۰X۱۰۸۰ - ۷۲ dpi و ۲۵۶ کیلوبایت می باشد.", root);
    new WBreak(root);
    new WBreak(root);
    //new WText(L"قالب های قابل قبول: PNG / JPEG", root);
    new WText(L"قالب قابل قبول: JPEG", root);
    new WBreak(root);
    new WBreak(root);
    new WText(L"تصویر ۱", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_scr1stFileUpload);
    new WBreak(root);
    new WText(L"تصویر ۲", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_scr2ndFileUpload);
    new WBreak(root);
    new WText(L"تصویر ۳", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_scr3rdFileUpload);
    new WBreak(root);
    new WText(L"تصویر ۴", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_scr4thFileUpload);
    new WBreak(root);
    new WBreak(root);

    new WBreak(root);
    new WText(L"ساختار و عملکرد هریک از صفحات واسط کاربر را شرح دهید.", root);
    new WBreak(root);
    new WBreak(root);
    root->addWidget(m_scrDescriptionTextArea);
    new WBreak(root);
    new WBreak(root);

    new WBreak(root);
    new WText(L"نرم‌افزار از چه دگمه‌هایی بر روی کنترل تلویزیون استفاده می‌کند؟", root);
    new WBreak(root);
    new WBreak(root);
    root->addWidget(m_remoteNumsCheckBox);
    new WText(L"اعداد", root);
    new WBreak(root);
    root->addWidget(m_remoteArrowsCheckBox);
    new WText(L"۴ جهت", root);
    new WBreak(root);
    root->addWidget(m_remoteToolsInfoCheckBox);
    new WText(L"دکمه tools و info", root);
    new WBreak(root);
    root->addWidget(m_remoteReturnExitCheckBox);
    new WText(L"دکمه‌های بازگشت و خروج", root);
    new WBreak(root);
    root->addWidget(m_remotePlaybackCheckBox);
    new WText(L"دکمه‌های Play Back", root);
    new WBreak(root);
    root->addWidget(m_remoteColorsTuningCheckBox);
    new WText(L"دکمه‌های تنظیم رنگ", root);
    new WBreak(root);
    root->addWidget(m_remoteVolumesTuningCheckBox);
    new WText(L"دکمه‌های تنظیم صدا", root);
    new WBreak(root);
    new WBreak(root);

    new WText(L"<hr />", root);
    new WBreak(root);
    new WText(L"ارسال نرم‌افزار", root);
    new WBreak(root);
    new WBreak(root);
    new WText(L"نرم‎افزار ارسالی شما باید در قالب یک فایل فشرده ZIP بوده و شامل کلیه فایل‎های لازم برای اجرای نرم‎فزار و مستندات مربوطه باشد. در غیر این‎صورت نرم‎افزار شما در مسابقه شرکت داده نخواهد شد. حداکثر حجم قابل قبول برای فایل فشرده شده ۵۱۲۰ کیلوبایت می‎باشد.", root);
    new WBreak(root);
    new WBreak(root);
    new WText(L"فایل برنامه", root);
    new WText(L"&nbsp;&nbsp;&nbsp;&nbsp;", root);
    root->addWidget(m_binaryFileUpload);
    new WBreak(root);

    new WBreak(root);
    new WBreak(root);
    new WBreak(root);
    new WBreak(root);

    WLengthValidator *titleValidator = new WLengthValidator(1, 128);
    WLengthValidator *descriptionValidator = new WLengthValidator(1, 65536);
    WLengthValidator *languageValidator = new WLengthValidator(1, 256);
    WLengthValidator *srcDescriptionValidator = new WLengthValidator(1, 65536);

    titleValidator->setMandatory(true);
    descriptionValidator->setMandatory(true);
    languageValidator->setMandatory(true);
    srcDescriptionValidator->setMandatory(true);

    m_titleLineEdit->setValidator(titleValidator);
    m_descriptionTextArea->setValidator(descriptionValidator);
    m_languageLineEdit->setValidator(languageValidator);
    m_scrDescriptionTextArea->setValidator(srcDescriptionValidator);

    OnTypeComboBoxChanged(m_typeComboBox->currentIndex());
    m_typeComboBox->activated().connect(this, &AppSubmit::OnTypeComboBoxChanged);

    OnDisplayTypeComboBoxChanged(m_displayTypeComboBox->currentIndex());
    m_displayTypeComboBox->activated().connect(this, &AppSubmit::OnDisplayTypeComboBoxChanged);

    m_scr1stFileUpload->changed().connect(this, &AppSubmit::OnScr1stFileChanged);
    m_scr1stFileUpload->fileTooLarge().connect(this, &AppSubmit::OnScr1stFileTooLarge);
    m_scr1stFileUpload->uploaded().connect(this, &AppSubmit::OnScr1stFileUploaded);

    m_scr2ndFileUpload->changed().connect(this, &AppSubmit::OnScr2ndFileChanged);
    m_scr2ndFileUpload->fileTooLarge().connect(this, &AppSubmit::OnScr2ndFileTooLarge);
    m_scr2ndFileUpload->uploaded().connect(this, &AppSubmit::OnScr2ndFileUploaded);

    m_scr3rdFileUpload->changed().connect(this, &AppSubmit::OnScr3rdFileChanged);
    m_scr3rdFileUpload->fileTooLarge().connect(this, &AppSubmit::OnScr3rdFileTooLarge);
    m_scr3rdFileUpload->uploaded().connect(this, &AppSubmit::OnScr3rdFileUploaded);

    m_scr4thFileUpload->changed().connect(this, &AppSubmit::OnScr4thFileChanged);
    m_scr4thFileUpload->fileTooLarge().connect(this, &AppSubmit::OnScr4thFileTooLarge);
    m_scr4thFileUpload->uploaded().connect(this, &AppSubmit::OnScr4thFileUploaded);

    m_binaryFileUpload->changed().connect(this, &AppSubmit::OnBinaryFileChanged);
    m_binaryFileUpload->fileTooLarge().connect(this, &AppSubmit::OnBinaryFileTooLarge);
    m_binaryFileUpload->uploaded().connect(this, &AppSubmit::OnBinaryFileUploaded);

    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


