/**********************************
*    = Header File Inclusion =
**********************************/

#include "lang.hpp"
#include "log.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
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
*    = Pimpl Things =
**********************************/

class Lang::Impl
{
public:
    std::unordered_map<std::string, std::wstring> LangStringsHash;

public:
    Impl(const CgiEnv::ELang &lang)
    {
        switch(lang) {
        case CgiEnv::ELang_En:
        case CgiEnv::ELang_RootEn:
            LangStringsHash["WARN_NO_SCRIPT"] = L"<noscript>"
                    "<div class=\"dvNoScript\">"
                    "Sorry, It looks like that either you have turned off "
                    "the browser's javascript feature or your browser originally "
                    "didn't support javascript feature at all. Some functions "
                    "will not behave correctly unless you fix it."
                    "</div>"
                    "</noscript>";
            LangStringsHash["FILE_UPLOAD_PROGRESS_DIALOG_TITLE"] = L"Uploading files...";
            LangStringsHash["FILE_UPLOAD_PROGRESS_DIALOG_BODY"] = L"Please wait while uploading file(s) to server...";
            break;

        case CgiEnv::ELang_Fa:
        case CgiEnv::ELang_RootFa:
            LangStringsHash["WARN_NO_SCRIPT"] = L"<noscript>"
                    "<div class=\"dvNoScript\">"
                    "کاربر گرامی به نظر می رسد که جاوا اسکریپت در مرورگر شما غیر فعال می باشد "
                    "و یا مرورگر شما از جاوا اسکریپت پشتیبانی نمی نماید. "
                    "متاسفانه در صورت عدم رفع این مشکل از سوی شما ممکن است برخی از قابلیت های "
                    "وب سایت به درستی کار نکند."
                    "</div>"
                    "</noscript>";
            LangStringsHash["FILE_UPLOAD_PROGRESS_DIALOG_TITLE"] = L"ارسال فایل ها...";
            LangStringsHash["FILE_UPLOAD_PROGRESS_DIALOG_BODY"] = L"در حال ارسال فایل(ها) به سرور. لطفا صبر کنید...";
            break;

        default:
            break;
        }


        switch(lang) {
        case CgiEnv::ELang_En:
            LangStringsHash["HOME_TITLE"] = L"Iran Samsung Developer Forum";
            LangStringsHash["CAPTCHA_HINT"] = L"e.g. 5 x 5 = 25";
            LangStringsHash["PWD_HINT"] = L"%1%-%2% characters length";
            LangStringsHash["PHONE_GENERIC_HINT"] = L"e.g. 0xxxxxxxxxx";
            LangStringsHash["PHONE_MOBILE_HINT"] = L"e.g. 09xxxxxxxxx";
            LangStringsHash["PHONE_TEL_HINT"] = L"e.g. 0xxxxxxxxxx";
            LangStringsHash["HOME_MENU_HOME"] = L"Home";
            LangStringsHash["HOME_MENU_FORUMS"] = L"Forums";
            LangStringsHash["HOME_MENU_CONTACT"] = L"Contact us";
            LangStringsHash["HOME_NEWS_TITLE_BAR_TEXT"] = L"News & Notice";
            LangStringsHash["HOME_NEWS_CONTINUE_READING_TEXT"] = L"&nbsp;[Continue Reading&hellip;]";
            LangStringsHash["HOME_NEWS_OLDER_NEWS_TEXT"] = L"[Older news&hellip;]";
            LangStringsHash["HOME_CONTACTUS_TITLE_MESSAGE"] = L"<h4>Please enter your message below:</h4>";
            LangStringsHash["HOME_CONTACTUS_RECIPIENT_CMB_TITLE"] = L".: Contact With :.";
            LangStringsHash["HOME_CONTACTUS_RECIPIENT_TEXT"] = L"Recipient";
            LangStringsHash["HOME_CONTACTUS_FROM_NAME_TEXT"] = L"Name";
            LangStringsHash["HOME_CONTACTUS_EMAIL_TEXT"] = L"Email";
            LangStringsHash["HOME_CONTACTUS_URL_TEXT"] = L"URL";
            LangStringsHash["HOME_CONTACTUS_SUBJECT_TEXT"] = L"Subject";
            LangStringsHash["HOME_CONTACTUS_BODY_TEXT"] = L"Body";
            LangStringsHash["HOME_CONTACTUS_CAPTCHA_TEXT"] = L"Captcha";
            LangStringsHash["HOME_CONTACTUS_SEND_BUTTON"] = L"Send";
            LangStringsHash["HOME_CONTACTUS_CLEAR_BUTTON"] = L"Clear";
            LangStringsHash["HOME_CONTACTUS_SEND_SUCCESS_MSG"] = L"Thank you! "
                    "Your message has been sent successfully. We will contact you very soon!";
            LangStringsHash["HOME_SERARCH_BOX_EMPTY_TEXT"] = L"Search...";
            LangStringsHash["HOME_SERARCH_NOT_FOUND_MSG"] = L"Your search - <strong>%1%</strong> - did not match any results.";
            LangStringsHash["HOME_SEARCH_RESULTS_DLG_TITLE"] = L"Search results for (%1%)";
            LangStringsHash["HOME_REGISTER_LINK_TEXT"] = L"REGISTER";
            LangStringsHash["HOME_SIGNIN_LINK_TEXT"] = L"SIGN IN";
            LangStringsHash["HOME_SIGNOUT_LINK_TEXT"] = L"SIGN OUT";
            LangStringsHash["HOME_PWD_LINK_TEXT"] = L"PASSWORD";
            LangStringsHash["HOME_MY_PROFILE_LINK_TEXT"] = L"PROFILE";
            LangStringsHash["HOME_MY_IDEAS_LINK_TEXT"] = L"MY IDEAS";
            LangStringsHash["HOME_APPOINTMENT_LINK_TEXT"] = L"APPOINTMENT";
            LangStringsHash["HOME_MY_APPS_LINK_TEXT"] = L"MY APPS";
            LangStringsHash["HOME_SIGN_IN_DLG_TITLE"] = L"Sign In";
            LangStringsHash["HOME_SIGN_IN_USER_NAME_TEXT"] = L"User Name";
            LangStringsHash["HOME_SIGN_IN_PWD_TEXT"] = L"Password";
            LangStringsHash["HOME_SIGN_IN_PWD_TEXT"] = L"Password";
            LangStringsHash["HOME_SIGN_IN_CAPTCHA_TEXT"] = L"Captcha";
            LangStringsHash["HOME_SIGN_IN_DLG_LOGIN_BUTTON"] = L"Login";
            LangStringsHash["HOME_SIGN_IN_DLG_RETURN_BUTTON"] = L"Return";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_TITLE"] = L"Login failed";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_BODY"] = L"The username or password you entered is incorrect.";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_RETRY_BUTTON"] = L"Retry";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_RECOVER_BUTTON"] = L"Recover Password";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_EMAIL_ERR_TITLE"] = L"Invalid Email";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_EMAIL_ERR_BODY"] = L"There is no registered user with email address <strong>%1%</strong>.";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_EMAIL_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SIGN_IN_DLG_RECOVERED_PWD_MSG_TITLE"] = L"Password Recovered";
            LangStringsHash["HOME_SIGN_IN_DLG_RECOVERED_PWD_MSG_BODY"] = L"The password has been sent to your email address.";
            LangStringsHash["HOME_SIGN_IN_DLG_RECOVERED_PWD_MSG_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_TITLE"] = L"ِNot Verified";
            LangStringsHash["HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_BODY"] = L"Your email address <strong>%1%</strong> has not been verified.";
            LangStringsHash["HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_RESEND_BUTTON"] = L"Verify Email";
            LangStringsHash["HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_CANCEL_BUTTON"] = L"Cancel Login";
            LangStringsHash["HOME_SIGN_IN_DLG_VERIFICATION_RESENT_MSG_TITLE"] = L"Verification";
            LangStringsHash["HOME_SIGN_IN_DLG_VERIFICATION_RESENT_MSG_BODY"] = L"Please check your inbox <strong>(%1%)</strong>"
                    "<br />for verification email with activation instructions.";
            LangStringsHash["HOME_SIGN_IN_DLG_VERIFICATION_RESENT_MSG_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_REGISTER_DLG_TITLE"] = L"Register";
            LangStringsHash["HOME_REGISTER_REGISTERATION_TYPE_TEXT"] = L"Registeration Type";
            LangStringsHash["HOME_REGISTER_INDIVIDUAL_TEXT"] = L"Individual";
            LangStringsHash["HOME_REGISTER_COMPANY_TEXT"] = L"Company";
            LangStringsHash["HOME_REGISTER_EMAIL_TEXT"] = L"Email";
            LangStringsHash["HOME_REGISTER_PWD_TEXT"] = L"Password";
            LangStringsHash["HOME_REGISTER_PWD_CONFIRM_TEXT"] = L"Confirm Password";
            LangStringsHash["HOME_REGISTER_FIRST_NAME_TEXT"] = L"First Name";
            LangStringsHash["HOME_REGISTER_LAST_NAME_TEXT"] = L"Last Name";
            LangStringsHash["HOME_REGISTER_CONTACT_NUMBER_TEXT"] = L"Phone";
            LangStringsHash["HOME_REGISTER_TEAMMATES_TEXT"] = L"Teammates";
            LangStringsHash["HOME_REGISTER_TEAMMATES_HINT"] = L"Please enter your each teammates name in a separate line.";
            LangStringsHash["HOME_REGISTER_COMPANY_NAME_TEXT"] = L"Company";
            LangStringsHash["HOME_REGISTER_REGISTERATION_ID_TEXT"] = L"Registeration Id";
            LangStringsHash["HOME_REGISTER_PHONE_TEXT"] = L"Phone";
            LangStringsHash["HOME_REGISTER_CEO_NAME_TEXT"] = L"CEO";
            LangStringsHash["HOME_REGISTER_CAPTCHA_TEXT"] = L"Captcha";
            LangStringsHash["HOME_REGISTER_DLG_REGISTER_BUTTON"] = L"Register";
            LangStringsHash["HOME_REGISTER_DLG_RETURN_BUTTON"] = L"Return";
            LangStringsHash["HOME_REGISTER_DLG_ALREADY_EXISTS_ERR_TITLE"] = L"Register";
            LangStringsHash["HOME_REGISTER_DLG_ALREADY_EXISTS_ERR_BODY"] = L"User <strong>%1%</strong> already exists!!";
            LangStringsHash["HOME_REGISTER_DLG_ALREADY_EXISTS_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_REGISTER_DLG_REGISTERED_MSG_TITLE"] = L"Congratulations";
            LangStringsHash["HOME_REGISTER_DLG_REGISTERED_MSG_BODY"] = L"You have been registered successfully!<br />"
                    "Please check your email <strong>(%1%)</strong> to activate your account.";
            LangStringsHash["HOME_REGISTER_DLG_REGISTERED_MSG_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_CHANGE_PWD_DLG_TITLE"] = L"Change Password";
            LangStringsHash["HOME_CHANGE_PWD_DLG_CURRENT_PWD_TEXT"] = L"Current Password";
            LangStringsHash["HOME_CHANGE_PWD_DLG_NEW_PWD_TEXT"] = L"New Password";
            LangStringsHash["HOME_CHANGE_PWD_DLG_CONFIRM_PWD_TEXT"] = L"Confirm Password";
            LangStringsHash["HOME_CHANGE_PWD_DLG_CHANGE_BUTTON"] = L"Change";
            LangStringsHash["HOME_CHANGE_PWD_DLG_RETURN_BUTTON"] = L"Cancel";
            LangStringsHash["HOME_CHANGE_PWD_DLG_WRONG_PWD_ERR_TITLE"] = L"Wrong Password";
            LangStringsHash["HOME_CHANGE_PWD_DLG_WRONG_PWD_ERR_BODY"] = L"You have entered the wrong password.";
            LangStringsHash["HOME_CHANGE_PWD_DLG_WRONG_PWD_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_CHANGE_PWD_DLG_SUCCESS_MSG_TITLE"] = L"Password Changed";
            LangStringsHash["HOME_CHANGE_PWD_DLG_SUCCESS_MSG_BODY"] = L"Password changed successfully.";
            LangStringsHash["HOME_CHANGE_PWD_DLG_SUCCESS_MSG_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_MY_PROFILE_DLG_TITLE"] = L"Profile";
            LangStringsHash["HOME_MY_PROFILE_DLG_EMAIL_TEXT"] = L"Email";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_FIRST_NAME_TEXT"] = L"First Name";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_LAST_NAME_TEXT"] = L"Last Name";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_CONTACT_NUMBER"] = L"Phone";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_ALTERNATIVE_CONTACT_NUMBER"] = L"2nd Phone";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_TEAMMATES_TEXT"] = L"Teammates";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_TEAMMATES_HINT"] = L"Please enter your each teammates name in a separate line.";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_NATIONAL_CODE_TEXT"] = L"National Code";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_TEXT"] = L"Educational Level";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_NONE_TEXT"] = L"----";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_DIPLOMA_TEXT"] = L"Diploma";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_AD_TEXT"] = L"Associate degree";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_BA_TEXT"] = L"B.A. / B.Sc.";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_MA_TEXT"] = L"M.A. / M.Sc.";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_PHD_TEXT"] = L"Ph.D";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_SUBJECT_TEXT"] = L"Educational Subject";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_WEBSITE"] = L"Website";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_NAME_TEXT"] = L"Company";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_REGISTERATION_ID_TEXT"] = L"Registeration Id";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_PHONE_TEXT"] = L"Phone";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_ALTERNATIVE_PHONE_TEXT"] = L"2nd Phone";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_CEO_NAME_TEXT"] = L"CEO";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_ECONOMICAL_CODE_TEXT"] = L"Economical Code";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_FOUNDING_YEAR_TEXT"] = L"Founding Year";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_ADDRESS_TEXT"] = L"Address";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_WEBSITE_TEXT"] = L"Website";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_RESUME_TEXT"] = L"Resume";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_RESUME_HINE"] = L"* Resume file format: PDF / 1024 KB Max";
            LangStringsHash["HOME_MY_PROFILE_DLG_SAVE_BUTTON"] = L"Save";
            LangStringsHash["HOME_MY_PROFILE_DLG_RETURN_BUTTON"] = L"Return";
            LangStringsHash["HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_TITLE"] = L"Saved";
            LangStringsHash["HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_BODY"] = L"Your profile saved successfully.";
            LangStringsHash["HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_TITLE"] = L"Upload Failed";
            LangStringsHash["HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_BODY"] = L"Sorry, can't upload your resume.";
            LangStringsHash["HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_TOO_LARGE_ERR_TITLE"] = L"Too Large";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_TOO_LARGE_ERR_BODY"] = L"Sorry, your resume file is too large.";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_NOT_PDF_ERR_TITLE"] = L"Invalid Format";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_NOT_PDF_ERR_BODY"] = L"Your resume is not in PDF format.";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_NOT_PDF_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_IDEA_DLG_TITLE"] = L"Submit Your Idea";
            LangStringsHash["HOME_SUBMIT_IDEA_EDIT_DLG_TITLE"] = L"Edit Your Idea";
            LangStringsHash["HOME_SUBMIT_IDEA_DLG_OK_BUTTON"] = L"Submit";
            LangStringsHash["HOME_SUBMIT_IDEA_DLG_OK_EDIT_BUTTON"] = L"Save Changes";
            LangStringsHash["HOME_SUBMIT_IDEA_DLG_RETURN_BUTTON"] = L"Return";
            LangStringsHash["HOME_SUBMIT_APP_DLG_TITLE"] = L"Submit Your App";
            LangStringsHash["HOME_SUBMIT_APP_EDIT_DLG_TITLE"] = L"Submit Your App";
            LangStringsHash["HOME_SUBMIT_APP_DLG_OK_BUTTON"] = L"Submit";
            LangStringsHash["HOME_SUBMIT_APP_DLG_OK_EDIT_BUTTON"] = L"Save Changes";
            LangStringsHash["HOME_SUBMIT_APP_DLG_RETURN_BUTTON"] = L"Return";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_GAME_ENTERTAINMENT_TEXT"] = L"Game / Entertainment";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_ONLINE_GAME_TEXT"] = L"Online Game";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_LEARN_INFO_TEXT"] = L"Learning / Information";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_SERVICES_TEXT"] = L"Services";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_TOOLS_TEXT"] = L"Tools / Utility";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_FULLSCREEN_TEXT"] = L"Fullscreen";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_ON_CORNER_OF_SCREEN_TEXT"] = L"On Corner of Screen";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_ACTIVE_ON_SCREEN_TEXT"] = L"Active on Screen";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_NO_FILE_ERR_TITLE"] = L"Screenshots";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_NO_FILE_ERR_BODY"] = L"Sorry, you have to upload 4 screenshots.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR__FILE_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_NO_FILE_ERR_TITLE"] = L"App";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_NO_FILE_ERR_BODY"] = L"Sorry, you have to upload your app file.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_NO_FILE_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_FILE_TOO_LARGE_ERR_TITLE"] = L"Too Large";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_FILE_TOO_LARGE_ERR_BODY"] = L"Sorry, your 1st screenshot file is too large.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_FILE_TOO_LARGE_ERR_TITLE"] = L"Too Large";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_FILE_TOO_LARGE_ERR_BODY"] = L"Sorry, your 2nd screenshot file is too large.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_FILE_TOO_LARGE_ERR_TITLE"] = L"Too Large";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_FILE_TOO_LARGE_ERR_BODY"] = L"Sorry, your 3rd screenshot file is too large.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_FILE_TOO_LARGE_ERR_TITLE"] = L"Too Large";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_FILE_TOO_LARGE_ERR_BODY"] = L"Sorry, your 4th screenshot file is too large.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_FILE_TOO_LARGE_ERR_TITLE"] = L"Too Large";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_FILE_TOO_LARGE_ERR_BODY"] = L"Sorry, your app file is too large.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_FORMAT_ERR_TITLE"] = L"Invalid Format";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_FORMAT_ERR_BODY"] = L"Sorry, your 1st screenshot is not a valid image.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_FORMAT_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_FORMAT_ERR_TITLE"] = L"Invalid Format";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_FORMAT_ERR_BODY"] = L"Sorry, your 1st screenshot is not a valid image.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_FORMAT_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_FORMAT_ERR_TITLE"] = L"Invalid Format";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_FORMAT_ERR_BODY"] = L"Sorry, your 1st screenshot is not a valid image.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_FORMAT_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_FORMAT_ERR_TITLE"] = L"Invalid Format";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_FORMAT_ERR_BODY"] = L"Sorry, your 1st screenshot is not a valid image.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_FORMAT_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_DPI_ERR_TITLE"] = L"Wrong DPI";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_DPI_ERR_BODY"] = L"Sorry, your 1st screenshot has wrong dpi.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_DPI_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_DPI_ERR_TITLE"] = L"Wrong DPI";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_DPI_ERR_BODY"] = L"Sorry, your 1st screenshot has wrong dpi.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_DPI_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_DPI_ERR_TITLE"] = L"Wrong DPI";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_DPI_ERR_BODY"] = L"Sorry, your 1st screenshot has wrong dpi.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_DPI_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_DPI_ERR_TITLE"] = L"Wrong DPI";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_DPI_ERR_BODY"] = L"Sorry, your 1st screenshot has wrong dpi.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_DPI_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_RESOLUTION_ERR_TITLE"] = L"Invalid Resolution";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_RESOLUTION_ERR_BODY"] = L"Sorry, your 1st screenshot resolution is too high.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_RESOLUTION_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_RESOLUTION_ERR_TITLE"] = L"Invalid Resolution";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_RESOLUTION_ERR_BODY"] = L"Sorry, your 1st screenshot resolution is too high.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_RESOLUTION_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_RESOLUTION_ERR_TITLE"] = L"Invalid Resolution";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_RESOLUTION_ERR_BODY"] = L"Sorry, your 1st screenshot resolution is too high.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_RESOLUTION_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_RESOLUTION_ERR_TITLE"] = L"Invalid Resolution";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_RESOLUTION_ERR_BODY"] = L"Sorry, your 1st screenshot resolution is too high.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_RESOLUTION_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_INVALID_ZIP_ERR_TITLE"] = L"Invalid ZIP";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_INVALID_ZIP_ERR_BODY"] = L"Sorry, your app is not a compressed ZIP file.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_INVALID_ZIP_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_MY_APPS_DLG_TITLE"] = L"My Apps";
            LangStringsHash["HOME_MY_APPS_NO_TEXT"] = L"No";
            LangStringsHash["HOME_MY_APPS_TITLE_TEXT"] = L"Title";
            LangStringsHash["HOME_MY_APPS_SUBMITTER_TEXT"] = L"Submitter";
            LangStringsHash["HOME_MY_APPS_TYPE_TEXT"] = L"Type";
            LangStringsHash["HOME_MY_APPS_PLATFORM_TEXT"] = L"Platform";
            LangStringsHash["HOME_MY_APPS_LANGUAGE_TEXT"] = L"Language";
            LangStringsHash["HOME_MY_APPS_DETAILS_TEXT"] = L"Details";
            LangStringsHash["HOME_MY_APPS_EDIT_TEXT"] = L"Edit";
            LangStringsHash["HOME_MY_APPS_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["HOME_MY_APPS_TYPE_GAME_ENTERTAINMENT_TEXT"] = L"Game / Entertainment";
            LangStringsHash["HOME_MY_APPS_TYPE_ONLINE_GAME_TEXT"] = L"Online Game";
            LangStringsHash["HOME_MY_APPS_TYPE_LEARN_INFO_TEXT"] = L"Learning / Information";
            LangStringsHash["HOME_MY_APPS_TYPE_SERVICES_TEXT"] = L"Services";
            LangStringsHash["HOME_MY_APPS_TYPE_TOOLS_TEXT"] = L"Tools / Utility";
            LangStringsHash["HOME_MY_APPS_DISPLAY_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["HOME_MY_APPS_DISPLAY_TYPE_FULLSCREEN_TEXT"] = L"Fullscreen";
            LangStringsHash["HOME_MY_APPS_DISPLAY_TYPE_ON_CORNER_OF_SCREEN_TEXT"] = L"On Corner of Screen";
            LangStringsHash["HOME_MY_APPS_DISPLAY_TYPE_ACTIVE_ON_SCREEN_TEXT"] = L"Active on Screen";
            LangStringsHash["HOME_MY_APPS_PLATFORM_HTML5_TEXT"] = L"HTML5";
            LangStringsHash["HOME_MY_APPS_PLATFORM_JAVASCRIPT_TEXT"] = L"JavaScript";
            LangStringsHash["HOME_MY_APPS_PLATFORM_FLASH_TEXT"] = L"Flash";
            LangStringsHash["HOME_MY_APPS_DESCRIPTION_TEXT"] = L"Description";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_TEXT"] = L"Screenshots";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_1ST_TEXT"] = L"1st screenshot";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_2ND_TEXT"] = L"2nd screenshot";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_3RD_TEXT"] = L"3rd screenshot";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_4TH_TEXT"] = L"4th screenshot";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_DESCRIPTION_TEXT"] = L"Screenshots Description";
            LangStringsHash["HOME_MY_APPS_DISPLAY_TYPE_TEXT"] = L"Display Type";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_TEXT"] = L"Technical Description";
            LangStringsHash["HOME_MY_APPS_AUDIO_VIDEO_STATUS_TEXT"] = L"Audio & Video Status";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_TEXT"] = L"Remote Buttons";
            LangStringsHash["HOME_MY_APPS_APP_BINARY_TEXT"] = L"App binary file";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_SUPPORTS_IP_RANGE_TEXT"] = L"Supports IP Range";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_PWD_PROTECTED_TEXT"] = L"Password Protected";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_HAS_COOKIES_TEXT"] = L"Has Cookies";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_AUTO_LOGIN_TEXT"] = L"Auto Login";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_HAS_FILM_TEXT"] = L"Has Film";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_HAS_AUDIO_TEXT"] = L"Has Audio";
            LangStringsHash["HOME_MY_APPS_AUDIO_VIDEO_STATUS_NO_AUDIO_NO_VIDEO_TEXT"] = L"No Audio or Video";
            LangStringsHash["HOME_MY_APPS_AUDIO_VIDEO_STATUS_NO_BUFFER_TEXT"] = L"Audio/Video included, No need for buffering";
            LangStringsHash["HOME_MY_APPS_AUDIO_VIDEO_STATUS_WITH_PLAYBACK_TEXT"] = L"With playback buttons";
            LangStringsHash["HOME_MY_APPS_AUDIO_VIDEO_STATUS_FULLSCREEN_WITH_EXIT_TEXT"] = L"Fullscreen with exit button";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_NUMS_TEXT"] = L"Nums";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_ARROWS_TEXT"] = L"Arrows";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_TOOLS_INFO_TEXT"] = L"Tools / Info";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_RETURN_EXIT_TEXT"] = L"Return / Exit";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_PLAYBACK_TEXT"] = L"Playback";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_COLORS_TUNING_TEXT"] = L"Color Tuning";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_VOLUME_TUNING_TEXT"] = L"Volume Tuning";
            LangStringsHash["HOME_APPOINTMENT_DLG_TITLE"] = L"Appointment to use a SmartTV";
            LangStringsHash["HOME_APPOINTMENT_DLG_OK_BUTTON"] = L"OK";
            LangStringsHash["HOME_APPOINTMENT_DLG_RETURN_BUTTON"] = L"Return";
            LangStringsHash["HOME_APPOINTMENT_DATE_TEXT"] = L"Date";
            LangStringsHash["HOME_APPOINTMENT_DATE_YY_TEXT"] = L".:: Year ::.";
            LangStringsHash["HOME_APPOINTMENT_DATE_MM_TEXT"] = L".:: Month ::.";
            LangStringsHash["HOME_APPOINTMENT_DATE_DD_TEXT"] = L".:: Day ::.";
            LangStringsHash["HOME_APPOINTMENT_TIME_TEXT"] = L"Time";
            LangStringsHash["HOME_APPOINTMENT_TIME_HH_TEXT"] = L".:: Hour ::.";
            LangStringsHash["HOME_MY_IDEAS_DLG_TITLE"] = L"My Ideas";
            LangStringsHash["HOME_MY_IDEAS_NO_TEXT"] = L"No";
            LangStringsHash["HOME_MY_IDEAS_TITLE_TEXT"] = L"Title";
            LangStringsHash["HOME_MY_IDEAS_TYPE_TEXT"] = L"Type";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_TEXT"] = L"Audience";
            LangStringsHash["HOME_MY_IDEAS_DETAILS_TEXT"] = L"Details";
            LangStringsHash["HOME_MY_IDEAS_EDIT_TEXT"] = L"Edit";
            LangStringsHash["HOME_MY_IDEAS_DESCRIPTION_TEXT"] = L"Description";
            LangStringsHash["HOME_MY_IDEAS_USAGE_TEXT"] = L"Usage";
            LangStringsHash["HOME_MY_IDEAS_SIMILAR_TEXT"] = L"Similar";
            LangStringsHash["HOME_MY_IDEAS_DIFFERENCE_TEXT"] = L"Difference";
            LangStringsHash["HOME_MY_IDEAS_TYPE_GAME_ENTERTAINMENT"] = L"Game & Entertainment";
            LangStringsHash["HOME_MY_IDEAS_TYPE_HYGIENE_HEALTH"] = L"Hygien & Health";
            LangStringsHash["HOME_MY_IDEAS_TYPE_NEWS_INFO"] = L"News & Information";
            LangStringsHash["HOME_MY_IDEAS_TYPE_LEARNING"] = L"Learning";
            LangStringsHash["HOME_MY_IDEAS_TYPE_NONE_PRESENCE_SERVICES"] = L"None Presence Services";
            LangStringsHash["HOME_MY_IDEAS_TYPE_ONLINE_SHOPPING"] = L"Online Shopping";
            LangStringsHash["HOME_MY_IDEAS_TYPE_OTHER"] = L"Other";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_TEENS"] = L"Teens";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_PARENTS"] = L"Parents";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_BOYS"] = L"Boys";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_GIRLS"] = L"Girls";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_WOMEN"] = L"Women";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_MEN"] = L"Men";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_CHILDS"] = L"Childs";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_ALL_FAMILY_MEMBERS"] = L"All Family Members";
            break;

        case CgiEnv::ELang_Fa:
            LangStringsHash["HOME_TITLE"] = L"انجمن توسعه دهندگان سامسونگ";
            LangStringsHash["CAPTCHA_HINT"] = L"مثال: ۲۵ = ۵ × ۵";
            LangStringsHash["PWD_HINT"] = L"%1%-%2% حرف طول";
            LangStringsHash["PHONE_GENERIC_HINT"] = L"××××××××××۰ ۱۱ رقم";
            LangStringsHash["PHONE_MOBILE_HINT"] = L"×××××××××۰۹ ۱۱ رقم";
            LangStringsHash["PHONE_TEL_HINT"] = L"××××××××××۰ ۱۱ رقم";
            LangStringsHash["HOME_MENU_HOME"] = L"صفحه اصلی";
            LangStringsHash["HOME_MENU_FORUMS"] = L"انجمن ها";
            LangStringsHash["HOME_MENU_CONTACT"] = L"تماس با ما";
            LangStringsHash["HOME_NEWS_TITLE_BAR_TEXT"] = L"اخبار";
            LangStringsHash["HOME_NEWS_CONTINUE_READING_TEXT"] = L"&nbsp;[ادامه خبر]&hellip;";
            LangStringsHash["HOME_NEWS_OLDER_NEWS_TEXT"] = L"[خبرهای قدیمی تر&hellip;]";
            LangStringsHash["HOME_CONTACTUS_TITLE_MESSAGE"] = L"<h4>لطفا پیام خود را در قسمت زیر وارد نمائید:</h4>";
            LangStringsHash["HOME_CONTACTUS_RECIPIENT_CMB_TITLE"] = L".: تماس با :.";
            LangStringsHash["HOME_CONTACTUS_RECIPIENT_TEXT"] = L"مخاطب";
            LangStringsHash["HOME_CONTACTUS_FROM_NAME_TEXT"] = L"نام فرستنده";
            LangStringsHash["HOME_CONTACTUS_EMAIL_TEXT"] = L"پست الکترونیک";
            LangStringsHash["HOME_CONTACTUS_URL_TEXT"] = L"صفحه ی وب";
            LangStringsHash["HOME_CONTACTUS_SUBJECT_TEXT"] = L"موضوع پیام";
            LangStringsHash["HOME_CONTACTUS_BODY_TEXT"] = L"متن پیام";
            LangStringsHash["HOME_CONTACTUS_CAPTCHA_TEXT"] = L"کد امنیتی";
            LangStringsHash["HOME_CONTACTUS_SEND_BUTTON"] = L"ارسال";
            LangStringsHash["HOME_CONTACTUS_CLEAR_BUTTON"] = L"پیام جدید";
            LangStringsHash["HOME_CONTACTUS_SEND_SUCCESS_MSG"] = L"با تشکر! "
                    "پیام با موفقیت ارسال شد. بزودی با شما تماس خواهیم گرفت.";
            LangStringsHash["HOME_SERARCH_BOX_EMPTY_TEXT"] = L"جستجو...";
            LangStringsHash["HOME_SERARCH_NOT_FOUND_MSG"] = L"جستجوی شما - <strong>%1%</strong> - نتیجه ای در بر نداشت.";
            LangStringsHash["HOME_SEARCH_RESULTS_DLG_TITLE"] = L"نتایج جستجو برای (%1%)";
            LangStringsHash["HOME_REGISTER_LINK_TEXT"] = L"ثبت نام";
            LangStringsHash["HOME_SIGNIN_LINK_TEXT"] = L"ورود";
            LangStringsHash["HOME_SIGNOUT_LINK_TEXT"] = L"خروج";
            LangStringsHash["HOME_PWD_LINK_TEXT"] = L"کلمه عبور";
            LangStringsHash["HOME_MY_PROFILE_LINK_TEXT"] = L"پروفایل";
            LangStringsHash["HOME_MY_IDEAS_LINK_TEXT"] = L"ایده های من";
            LangStringsHash["HOME_APPOINTMENT_LINK_TEXT"] = L"قرار تست";
            LangStringsHash["HOME_MY_APPS_LINK_TEXT"] = L"برنامه های من";
            LangStringsHash["HOME_SIGN_IN_DLG_TITLE"] = L"ورود";
            LangStringsHash["HOME_SIGN_IN_USER_NAME_TEXT"] = L"نام کاربری";
            LangStringsHash["HOME_SIGN_IN_PWD_TEXT"] = L"کلمه عبور";
            LangStringsHash["HOME_SIGN_IN_CAPTCHA_TEXT"] = L"کد امنیتی";
            LangStringsHash["HOME_SIGN_IN_DLG_LOGIN_BUTTON"] = L"ورود";
            LangStringsHash["HOME_SIGN_IN_DLG_RETURN_BUTTON"] = L"بازگشت";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_TITLE"] = L"خطای ورود";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_BODY"] = L"نام کاربری یا کلمه عبور ورودی اشتباه است.";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_RETRY_BUTTON"] = L"سعی مجدد";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_USER_PWD_ERR_RECOVER_BUTTON"] = L"بازیابی کلمه عبور";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_EMAIL_ERR_TITLE"] = L"پست الکترونیکی نامعتبر";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_EMAIL_ERR_BODY"] = L"کاربری با صندوق پست الکترونیکی <strong>%1%</strong> ثبت نام نکرده است.";
            LangStringsHash["HOME_SIGN_IN_DLG_WRONG_EMAIL_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SIGN_IN_DLG_RECOVERED_PWD_MSG_TITLE"] = L"بازیابی کلمه عبور";
            LangStringsHash["HOME_SIGN_IN_DLG_RECOVERED_PWD_MSG_BODY"] = L"کلمه عبور با موفقیت به صندوق پست الکترونیک شما ارسال شد";
            LangStringsHash["HOME_SIGN_IN_DLG_RECOVERED_PWD_MSG_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_TITLE"] = L"عدم تائید پست الکترونیکی";
            LangStringsHash["HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_BODY"] = L"آدرس پست الکترونیکی شما <strong>%1%</strong>"
                    " تائید نشده است.";
            LangStringsHash["HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_RESEND_BUTTON"] = L"تائید آدرس ایمیل";
            LangStringsHash["HOME_SIGN_IN_DLG_NOT_VERIFIED_ERR_CANCEL_BUTTON"] = L"لغو ورود";
            LangStringsHash["HOME_SIGN_IN_DLG_VERIFICATION_RESENT_MSG_TITLE"] = L"فعال سازی";
            LangStringsHash["HOME_SIGN_IN_DLG_VERIFICATION_RESENT_MSG_BODY"] = L"لطفا جهت فعال سازی، صندوق پستی تان <strong>(%1%)</strong> را چک نمائید.";
            LangStringsHash["HOME_SIGN_IN_DLG_VERIFICATION_RESENT_MSG_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_REGISTER_DLG_TITLE"] = L"ثبت نام";
            LangStringsHash["HOME_REGISTER_REGISTERATION_TYPE_TEXT"] = L"نوع ثبت نام";
            LangStringsHash["HOME_REGISTER_INDIVIDUAL_TEXT"] = L"حقیقی";
            LangStringsHash["HOME_REGISTER_COMPANY_TEXT"] = L"حقوقی";
            LangStringsHash["HOME_REGISTER_EMAIL_TEXT"] = L"پست الکترونیکی";
            LangStringsHash["HOME_REGISTER_PWD_TEXT"] = L"کلمه عبور";
            LangStringsHash["HOME_REGISTER_PWD_CONFIRM_TEXT"] = L"تائید کلمه عبور";
            LangStringsHash["HOME_REGISTER_FIRST_NAME_TEXT"] = L"نام";
            LangStringsHash["HOME_REGISTER_LAST_NAME_TEXT"] = L"نام خانوادگی";
            LangStringsHash["HOME_REGISTER_CONTACT_NUMBER_TEXT"] = L"تلفن تماس";
            LangStringsHash["HOME_REGISTER_TEAMMATES_TEXT"] = L"همکاران";
            LangStringsHash["HOME_REGISTER_TEAMMATES_HINT"] = L"لطفا اسامی هریک از همکاران خود را در یک خط جداگانه وارد نمائید.";
            LangStringsHash["HOME_REGISTER_COMPANY_NAME_TEXT"] = L"نام شرکت";
            LangStringsHash["HOME_REGISTER_REGISTERATION_ID_TEXT"] = L"شماره ثبت";
            LangStringsHash["HOME_REGISTER_PHONE_TEXT"] = L"تلفن تماس";
            LangStringsHash["HOME_REGISTER_CEO_NAME_TEXT"] = L"نام مدیر عامل";
            LangStringsHash["HOME_REGISTER_CAPTCHA_TEXT"] = L"کد امنیتی";
            LangStringsHash["HOME_REGISTER_DLG_REGISTER_BUTTON"] = L"ثبت نام";
            LangStringsHash["HOME_REGISTER_DLG_RETURN_BUTTON"] = L"بازگشت";
            LangStringsHash["HOME_REGISTER_DLG_ALREADY_EXISTS_ERR_TITLE"] = L"ثبت نام";
            LangStringsHash["HOME_REGISTER_DLG_ALREADY_EXISTS_ERR_BODY"] = L"نام کاربری <strong>%1%</strong> قبلا ثبت شده است!!";
            LangStringsHash["HOME_REGISTER_DLG_ALREADY_EXISTS_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_REGISTER_DLG_REGISTERED_MSG_TITLE"] = L"ثبت نام";
            LangStringsHash["HOME_REGISTER_DLG_REGISTERED_MSG_BODY"] = L"ثبت نام شما با موفقیت انجام شد!<br />"
                    "لطفا جهت تکمیل ثبت نام صندوق پستی تان <strong>(%1%)</strong> را چک نمائید.";
            LangStringsHash["HOME_REGISTER_DLG_REGISTERED_MSG_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_CHANGE_PWD_DLG_TITLE"] = L"تغییر کلمه عبور";
            LangStringsHash["HOME_CHANGE_PWD_DLG_CURRENT_PWD_TEXT"] = L"کلمه عبور فعلی";
            LangStringsHash["HOME_CHANGE_PWD_DLG_NEW_PWD_TEXT"] = L"کلمه عبور جدید";
            LangStringsHash["HOME_CHANGE_PWD_DLG_CONFIRM_PWD_TEXT"] = L"تائید کلمه عبور";
            LangStringsHash["HOME_CHANGE_PWD_DLG_CHANGE_BUTTON"] = L"تغییر";
            LangStringsHash["HOME_CHANGE_PWD_DLG_RETURN_BUTTON"] = L"لغو";
            LangStringsHash["HOME_CHANGE_PWD_DLG_WRONG_PWD_ERR_TITLE"] = L"کلمه عبور اشتباه";
            LangStringsHash["HOME_CHANGE_PWD_DLG_WRONG_PWD_ERR_BODY"] = L"کلمه عبور فعلی را اشتباه وارد نموده اید.";
            LangStringsHash["HOME_CHANGE_PWD_DLG_WRONG_PWD_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_CHANGE_PWD_DLG_SUCCESS_MSG_TITLE"] = L"تغییر کلمه عبور";
            LangStringsHash["HOME_CHANGE_PWD_DLG_SUCCESS_MSG_BODY"] = L"کلمه عبور با موفقیت تغییر یافت.";
            LangStringsHash["HOME_CHANGE_PWD_DLG_SUCCESS_MSG_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_MY_PROFILE_DLG_TITLE"] = L"پروفایل";
            LangStringsHash["HOME_MY_PROFILE_DLG_EMAIL_TEXT"] = L"پست الکترونیکی";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_FIRST_NAME_TEXT"] = L"نام";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_LAST_NAME_TEXT"] = L"نام خانوادگی";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_CONTACT_NUMBER"] = L"تلفن تماس";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_ALTERNATIVE_CONTACT_NUMBER"] = L"تلفن جایگزین";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_TEAMMATES_TEXT"] = L"همکاران";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_TEAMMATES_HINT"] = L"لطفا اسامی هریک از همکاران خود را در یک خط جداگانه وارد نمائید.";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_NATIONAL_CODE_TEXT"] = L"کد ملی";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_TEXT"] = L"سطح تحصیلات";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_NONE_TEXT"] = L"----";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_DIPLOMA_TEXT"] = L"دیپلم";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_AD_TEXT"] = L"کاردانی";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_BA_TEXT"] = L"کارشناسی";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_MA_TEXT"] = L"کارشناسی ارشد";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_LEVEL_PHD_TEXT"] = L"دکترا";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_EDUCATIONAL_SUBJECT_TEXT"] = L"رشته تحصیلی";
            LangStringsHash["HOME_MY_PROFILE_DLG_INDIVIDUAL_WEBSITE"] = L"وب سایت";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_NAME_TEXT"] = L"نام شرکت";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_REGISTERATION_ID_TEXT"] = L"شماره ثبت";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_PHONE_TEXT"] = L"تلفن تماس";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_ALTERNATIVE_PHONE_TEXT"] = L"تلفن جایگزین";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_CEO_NAME_TEXT"] = L"نام مدیرعامل";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_ECONOMICAL_CODE_TEXT"] = L"کد اقتصادی";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_FOUNDING_YEAR_TEXT"] = L"سال تاسیس";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_ADDRESS_TEXT"] = L"آدرس";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_WEBSITE_TEXT"] = L"وب سایت";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_RESUME_TEXT"] = L"رزومه فعالیت ها";
            LangStringsHash["HOME_MY_PROFILE_DLG_COMPANY_RESUME_HINE"] = L"قالب قابل قبول فایل رزومه PDF و حداکثر ۱۰۲۴ کیلوبایت می باشد.";
            LangStringsHash["HOME_MY_PROFILE_DLG_SAVE_BUTTON"] = L"ذخیره";
            LangStringsHash["HOME_MY_PROFILE_DLG_RETURN_BUTTON"] = L"بازگشت";
            LangStringsHash["HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_TITLE"] = L"ذخیره شد";
            LangStringsHash["HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_BODY"] = L"پروفایل شما با موفقیت ذخیره شد.";
            LangStringsHash["HOME_MY_PROFILE_DLG_SAVE_SUCCESS_MSG_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_TITLE"] = L"ارسال فایل";
            LangStringsHash["HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_BODY"] = L"متاسفانه امکان ارسال فایل رزومه وجود ندارد.";
            LangStringsHash["HOME_MY_PROFILE_DLG_CANT_UPLOAD_RESUME_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_TOO_LARGE_ERR_TITLE"] = L"حجم نامعتبر";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_TOO_LARGE_ERR_BODY"] = L"متاسفانه حجم فایل رزومه شما بسیار زیاد می باشد.";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_NOT_PDF_ERR_TITLE"] = L"قالب نامعتبر";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_NOT_PDF_ERR_BODY"] = L"متاسفانه قالب فایل رزومه شما PDF نمی باشد.";
            LangStringsHash["HOME_MY_PROFILE_DLG_RESUME_FILE_NOT_PDF_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_IDEA_DLG_TITLE"] = L"ارسال ایده";
            LangStringsHash["HOME_SUBMIT_IDEA_EDIT_DLG_TITLE"] = L"ویرایش ایده";
            LangStringsHash["HOME_SUBMIT_IDEA_DLG_OK_BUTTON"] = L"ارسال";
            LangStringsHash["HOME_SUBMIT_IDEA_DLG_OK_EDIT_BUTTON"] = L"ذخیره تغییرات";
            LangStringsHash["HOME_SUBMIT_IDEA_DLG_RETURN_BUTTON"] = L"بازگشت";
            LangStringsHash["HOME_SUBMIT_APP_DLG_TITLE"] = L"ارسال برنامه";
            LangStringsHash["HOME_SUBMIT_APP_EDIT_DLG_TITLE"] = L"ویرایش برنامه";
            LangStringsHash["HOME_SUBMIT_APP_DLG_OK_BUTTON"] = L"ارسال";
            LangStringsHash["HOME_SUBMIT_APP_DLG_OK_EDIT_BUTTON"] = L"ذخیره تغییرات";
            LangStringsHash["HOME_SUBMIT_APP_DLG_RETURN_BUTTON"] = L"بازگشت";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_GAME_ENTERTAINMENT_TEXT"] = L"بازی / سرگرمی";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_ONLINE_GAME_TEXT"] = L"بازی آنلاین";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_LEARN_INFO_TEXT"] = L"آموزش و اطلاعات";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_SERVICES_TEXT"] = L"خدمات غیرحضوری";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_TYPE_TOOLS_TEXT"] = L"نرم افزارهای کمکی";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_FULLSCREEN_TEXT"] = L"نرم افزار صفحه کامل";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_ON_CORNER_OF_SCREEN_TEXT"] = L"در گوشه ای از صفحه نمایش اجرا می شود";
            LangStringsHash["HOME_SUBMIT_APP_DLG_APP_DISPLAY_TYPE_ACTIVE_ON_SCREEN_TEXT"] = L"فعال بر روی صفحه نمایش";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_NO_FILE_ERR_TITLE"] = L"نمای نرم افزار";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_NO_FILE_ERR_BODY"] = L"جهت ارسال نرم افزار، ارسال ۴ تصویر از نمای نرم افزار الزامیست.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR__FILE_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_NO_FILE_ERR_TITLE"] = L"فایل برنامه";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_NO_FILE_ERR_BODY"] = L"ارسال فایل برنامه الزامیست.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_NO_FILE_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_FILE_TOO_LARGE_ERR_TITLE"] = L"حجم زیاد";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_FILE_TOO_LARGE_ERR_BODY"] = L"متاسفانه حجم فایل تصویر شما بیشتر از میزان مجاز می باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_FILE_TOO_LARGE_ERR_TITLE"] = L"حجم زیاد";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_FILE_TOO_LARGE_ERR_BODY"] = L"متاسفانه حجم فایل تصویر دوم شما بیشتر از میزان مجاز می باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_FILE_TOO_LARGE_ERR_TITLE"] = L"حجم زیاد";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_FILE_TOO_LARGE_ERR_BODY"] = L"متاسفانه حجم فایل تصویر سوم شما بیشتر از میزان مجاز می باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_FILE_TOO_LARGE_ERR_TITLE"] = L"حجم زیاد";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_FILE_TOO_LARGE_ERR_BODY"] = L"متاسفانه حجم فایل تصویر چهارم بشما یشتر از میزان مجاز می باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_FILE_TOO_LARGE_ERR_TITLE"] = L"حجم زیاد";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_FILE_TOO_LARGE_ERR_BODY"] = L"متاسفانه حجم فایل برنامه شما بیشتر از میزان مجاز می باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_FILE_TOO_LARGE_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_FORMAT_ERR_TITLE"] = L"قالب نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_FORMAT_ERR_BODY"] = L"متاسفانه قالب تصویر اول معتبر نمی باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_FORMAT_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_FORMAT_ERR_TITLE"] = L"قالب نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_FORMAT_ERR_BODY"] = L"متاسفانه قالب تصویر دوم معتبر نمی باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_FORMAT_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_FORMAT_ERR_TITLE"] = L"قالب نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_FORMAT_ERR_BODY"] = L"متاسفانه قالب تصویر سوم معتبر نمی باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_FORMAT_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_FORMAT_ERR_TITLE"] = L"قالب نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_FORMAT_ERR_BODY"] = L"متاسفانه قالب تصویر چهارم معتبر نمی باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_FORMAT_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_DPI_ERR_TITLE"] = L"DPI نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_DPI_ERR_BODY"] = L"متاسفانه DPI تصویر اول نامعتبر می باشید.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_DPI_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_DPI_ERR_TITLE"] = L"DPI نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_DPI_ERR_BODY"] = L"متاسفانه DPI تصویر دوم نامعتبر می باشید.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_DPI_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_DPI_ERR_TITLE"] = L"DPI نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_DPI_ERR_BODY"] = L"متاسفانه DPI تصویر سوم نامعتبر می باشید.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_DPI_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_DPI_ERR_TITLE"] = L"DPI نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_DPI_ERR_BODY"] = L"متاسفانه DPI تصویر چهارم نامعتبر می باشید.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_DPI_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_RESOLUTION_ERR_TITLE"] = L"ابعاد نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_RESOLUTION_ERR_BODY"] = L"متاسفانه رزولوشن تصویر اول بیش از حد مجاز می باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_1ST_INVALID_RESOLUTION_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_RESOLUTION_ERR_TITLE"] = L"ابعاد نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_RESOLUTION_ERR_BODY"] = L"متاسفانه رزولوشن تصویر دوم بیش از حد مجاز می باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_2ND_INVALID_RESOLUTION_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_RESOLUTION_ERR_TITLE"] = L"ابعاد نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_RESOLUTION_ERR_BODY"] = L"متاسفانه رزولوشن تصویر سوم بیش از حد مجاز می باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_3RD_INVALID_RESOLUTION_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_RESOLUTION_ERR_TITLE"] = L"ابعاد نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_RESOLUTION_ERR_BODY"] = L"متاسفانه رزولوشن تصویر چهارم بیش از حد مجاز می باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_SCR_4TH_INVALID_RESOLUTION_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_INVALID_ZIP_ERR_TITLE"] = L"قالب نامعتبر";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_INVALID_ZIP_ERR_BODY"] = L"متاسفانه قالب فایل برنامه شما ZIP نمی باشد.";
            LangStringsHash["HOME_SUBMIT_APP_DLG_BINARY_INVALID_ZIP_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_MY_APPS_DLG_TITLE"] = L"برنامه های من";
            LangStringsHash["HOME_MY_APPS_NO_TEXT"] = L"ردیف";
            LangStringsHash["HOME_MY_APPS_TITLE_TEXT"] = L"عنوان";
            LangStringsHash["HOME_MY_APPS_SUBMITTER_TEXT"] = L"ارسال کننده";
            LangStringsHash["HOME_MY_APPS_TYPE_TEXT"] = L"دسته بندی";
            LangStringsHash["HOME_MY_APPS_PLATFORM_TEXT"] = L"پلتفرم";
            LangStringsHash["HOME_MY_APPS_LANGUAGE_TEXT"] = L"زبان";
            LangStringsHash["HOME_MY_APPS_DETAILS_TEXT"] = L"جزئیات";
            LangStringsHash["HOME_MY_APPS_EDIT_TEXT"] = L"ویرایش";
            LangStringsHash["HOME_MY_APPS_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["HOME_MY_APPS_TYPE_GAME_ENTERTAINMENT_TEXT"] = L"بازی / سرگرمی";
            LangStringsHash["HOME_MY_APPS_TYPE_ONLINE_GAME_TEXT"] = L"بازی آنلاین";
            LangStringsHash["HOME_MY_APPS_TYPE_LEARN_INFO_TEXT"] = L"آموزش و اطلاعات";
            LangStringsHash["HOME_MY_APPS_TYPE_SERVICES_TEXT"] = L"خدمات غیرحضوری";
            LangStringsHash["HOME_MY_APPS_TYPE_TOOLS_TEXT"] = L"نرم افزارهای کمکی";
            LangStringsHash["HOME_MY_APPS_DISPLAY_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["HOME_MY_APPS_DISPLAY_TYPE_FULLSCREEN_TEXT"] = L"نرم افزار صفحه کامل";
            LangStringsHash["HOME_MY_APPS_DISPLAY_TYPE_ON_CORNER_OF_SCREEN_TEXT"] = L"در گوشه ای از صفحه نمایش اجرا می شود";
            LangStringsHash["HOME_MY_APPS_DISPLAY_TYPE_ACTIVE_ON_SCREEN_TEXT"] = L"فعال بر روی صفحه نمایش";
            LangStringsHash["HOME_MY_APPS_PLATFORM_HTML5_TEXT"] = L"HTML5";
            LangStringsHash["HOME_MY_APPS_PLATFORM_JAVASCRIPT_TEXT"] = L"JavaScript";
            LangStringsHash["HOME_MY_APPS_PLATFORM_FLASH_TEXT"] = L"Flash";
            LangStringsHash["HOME_MY_APPS_DESCRIPTION_TEXT"] = L"توضیحات";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_TEXT"] = L"نمای نرم افزار";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_1ST_TEXT"] = L"تصویر اول";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_2ND_TEXT"] = L"تصویر دوم";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_3RD_TEXT"] = L"تصویر سوم";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_4TH_TEXT"] = L"تصویر چهارم";
            LangStringsHash["HOME_MY_APPS_SCREENSHOTS_DESCRIPTION_TEXT"] = L"توضیحات نمای نرم افزار";
            LangStringsHash["HOME_MY_APPS_DISPLAY_TYPE_TEXT"] = L"نوع صفحه نمایش";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_TEXT"] = L"توضیحات فنی";
            LangStringsHash["HOME_MY_APPS_AUDIO_VIDEO_STATUS_TEXT"] = L"وضعیت صدا و تصویر";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_TEXT"] = L"دکمه های ریموت";
            LangStringsHash["HOME_MY_APPS_APP_BINARY_TEXT"] = L"فایل برنامه";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_SUPPORTS_IP_RANGE_TEXT"] = L"پشتیبانی از محدوده IP";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_PWD_PROTECTED_TEXT"] = L"محافظت با کلمه عبور";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_HAS_COOKIES_TEXT"] = L"کوکی";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_AUTO_LOGIN_TEXT"] = L"لاگین خودکار";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_HAS_FILM_TEXT"] = L"فیلم تصویری";
            LangStringsHash["HOME_MY_APPS_TECHNICAL_DESCRIPTION_HAS_AUDIO_TEXT"] = L"صوت / موسیقی";
            LangStringsHash["HOME_MY_APPS_AUDIO_VIDEO_STATUS_NO_AUDIO_NO_VIDEO_TEXT"] = L"فاقد فیلم / صدا";
            LangStringsHash["HOME_MY_APPS_AUDIO_VIDEO_STATUS_NO_BUFFER_TEXT"] = L"فیلم / صدا به همراه نرم افزار بدون نیاز به بافرینگ";
            LangStringsHash["HOME_MY_APPS_AUDIO_VIDEO_STATUS_WITH_PLAYBACK_TEXT"] = L"قابلیت کنترل صوت و تصویر به همراه پلی بک";
            LangStringsHash["HOME_MY_APPS_AUDIO_VIDEO_STATUS_FULLSCREEN_WITH_EXIT_TEXT"] = L"تمام صفحه به همراه دکمه خروج";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_NUMS_TEXT"] = L"اعداد";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_ARROWS_TEXT"] = L"۴ جهت";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_TOOLS_INFO_TEXT"] = L"Tools / Info";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_RETURN_EXIT_TEXT"] = L"بازگشت و خروج";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_PLAYBACK_TEXT"] = L"Playback";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_COLORS_TUNING_TEXT"] = L"تنظیم رنگ";
            LangStringsHash["HOME_MY_APPS_REMOTE_BUTTONS_VOLUME_TUNING_TEXT"] = L"تنظیم صدا";
            LangStringsHash["HOME_APPOINTMENT_DLG_TITLE"] = L"تعیین قرار ملاقات تست جهت استفاده از تلویزیون هوشمند";
            LangStringsHash["HOME_APPOINTMENT_DLG_OK_BUTTON"] = L"تائید";
            LangStringsHash["HOME_APPOINTMENT_DLG_RETURN_BUTTON"] = L"بازگشت";
            LangStringsHash["HOME_APPOINTMENT_DATE_TEXT"] = L"تاریخ";
            LangStringsHash["HOME_APPOINTMENT_DATE_YY_TEXT"] = L".:: سال ::.";
            LangStringsHash["HOME_APPOINTMENT_DATE_MM_TEXT"] = L".:: ماه ::.";
            LangStringsHash["HOME_APPOINTMENT_DATE_DD_TEXT"] = L".:: روز ::.";
            LangStringsHash["HOME_APPOINTMENT_TIME_TEXT"] = L"زمان";
            LangStringsHash["HOME_APPOINTMENT_TIME_HH_TEXT"] = L".:: ساعت ::.";
            LangStringsHash["HOME_MY_IDEAS_DLG_TITLE"] = L"ایده های من";
            LangStringsHash["HOME_MY_IDEAS_NO_TEXT"] = L"ردیف";
            LangStringsHash["HOME_MY_IDEAS_TITLE_TEXT"] = L"عنوان";
            LangStringsHash["HOME_MY_IDEAS_SUBMITTER_TEXT"] = L"ارسال کننده";
            LangStringsHash["HOME_MY_IDEAS_TYPE_TEXT"] = L"نوع";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_TEXT"] = L"مخاطب";
            LangStringsHash["HOME_MY_IDEAS_DETAILS_TEXT"] = L"جزئیات";
            LangStringsHash["HOME_MY_IDEAS_EDIT_TEXT"] = L"ویرایش";
            LangStringsHash["HOME_MY_IDEAS_DESCRIPTION_TEXT"] = L"شرح ایده";
            LangStringsHash["HOME_MY_IDEAS_USAGE_TEXT"] = L"کاربرد ایده";
            LangStringsHash["HOME_MY_IDEAS_SIMILAR_TEXT"] = L"مشابه";
            LangStringsHash["HOME_MY_IDEAS_DIFFERENCE_TEXT"] = L"تفاوت";
            LangStringsHash["HOME_MY_IDEAS_TYPE_GAME_ENTERTAINMENT"] = L"بازی و سرگرمی";
            LangStringsHash["HOME_MY_IDEAS_TYPE_HYGIENE_HEALTH"] = L"بهداشت و سلامت";
            LangStringsHash["HOME_MY_IDEAS_TYPE_NEWS_INFO"] = L"اخبار و اطلاعات";
            LangStringsHash["HOME_MY_IDEAS_TYPE_LEARNING"] = L"آموزش";
            LangStringsHash["HOME_MY_IDEAS_TYPE_NONE_PRESENCE_SERVICES"] = L"خدمات غیرحضوری";
            LangStringsHash["HOME_MY_IDEAS_TYPE_ONLINE_SHOPPING"] = L"خرید آنلاین";
            LangStringsHash["HOME_MY_IDEAS_TYPE_OTHER"] = L"سایر";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_TEENS"] = L"نوجوانان";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_PARENTS"] = L"والدین";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_BOYS"] = L"پسران";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_GIRLS"] = L"دختران";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_WOMEN"] = L"زنان";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_MEN"] = L"مردان";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_CHILDS"] = L"کودکان";
            LangStringsHash["HOME_MY_IDEAS_AUDIENCE_ALL_FAMILY_MEMBERS"] = L"تمامی اعضای خانواده";
            break;

        case CgiEnv::ELang_RootEn:
            LangStringsHash["ROOT_LOGIN_TITLE"] = L".:: Login ::.";
            LangStringsHash["ROOT_CMS_TITLE"] = L"SamsungDForum.ir CMS";
            LangStringsHash["ROOT_LOGIN_FORM_TITLE"] = L"<h3>Login to Conference Panel</h3>";
            LangStringsHash["ROOT_LOGIN_FORM_USER_TEXT"] = L"Username";
            LangStringsHash["ROOT_LOGIN_FORM_PWD_TEXT"] = L"Password";
            LangStringsHash["ROOT_LOGIN_FORM_CAPTCHA_TEXT"] = L"Captcha";
            LangStringsHash["ROOT_LOGIN_FORM_EMAIL_TEXT"] = L"Email";
            LangStringsHash["ROOT_LOGIN_FORM_LOGIN_TEXT"] = L"Login";
            LangStringsHash["ROOT_LOGIN_FORM_RECOVER_TEXT"] = L"Recover";
            LangStringsHash["ROOT_LOGIN_FORM_FORGOT_TEXT"] = L"Forgot Password";
            LangStringsHash["ROOT_LOGIN_FORM_ERR_LOGIN"] = L"Invalid username or password.";
            LangStringsHash["ROOT_LOGIN_FORM_ERR_FORGOT"] = L"Invalid user.";
            LangStringsHash["ROOT_LOGIN_FORM_MSG_FORGOT"] = L"Password recovered successfully, Please check your email.";
            LangStringsHash["ROOT_CMS_MENU_WELCOME"] = L"Welcome";
            LangStringsHash["ROOT_CMS_MENU_USERS"] = L"Users";
            LangStringsHash["ROOT_CMS_MENU_IDEAS"] = L"Ideas";
            LangStringsHash["ROOT_CMS_MENU_APPOINTMENTS"] = L"Appointments";
            LangStringsHash["ROOT_CMS_MENU_APPS"] = L"Apps";
            LangStringsHash["ROOT_CMS_MENU_NEWS"] = L"News";
            LangStringsHash["ROOT_CMS_MENU_PAGES"] = L"Pages";
            LangStringsHash["ROOT_CMS_MENU_CONTACTS"] = L"Contacts";
            LangStringsHash["ROOT_CMS_MENU_PWDEMAIL"] = L"Password / Email";
            LangStringsHash["ROOT_CMS_MENU_SYSMON"] = L"System Monitor";
            LangStringsHash["ROOT_CMS_MENU_EXIT"] = L"Exit";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_TABLE_TITLE"] = L"<h3>Individual Users</h3>";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_TABLE_TITLE"] = L"<h3>Company Users</h3>";
            LangStringsHash["ROOT_CMSUSERS_NO_TEXT"] = L"No";
            LangStringsHash["ROOT_CMSUSERS_EMAIL_TEXT"] = L"Email";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_FIRST_NAME_TEXT"] = L"First Name";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_LAST_NAME_TEXT"] = L"Last Name";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_CONTACT_NUMBER_TEXT"] = L"Phone";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_ALTERNATIVE_CONTACT_NUMBER"] = L"2nd Phone";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_TEAMMATES_TEXT"] = L"Teammates";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_NATIONAL_CODE_TEXT"] = L"National Code";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_TEXT"] = L"Educational Level";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_TEXT"] = L"Educational Level";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_NONE_TEXT"] = L"----";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_DIPLOMA_TEXT"] = L"Diploma";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_AD_TEXT"] = L"Associate degree";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_BA_TEXT"] = L"B.A. / B.Sc.";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_MA_TEXT"] = L"M.A. / M.Sc.";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_PHD_TEXT"] = L"Ph.D";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_SUBJECT_TEXT"] = L"Educational Subject";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_WEBSITE_TEXT"] = L"Website";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_NAME_TEXT"] = L"Company";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_REGISTERATION_ID_TEXT"] = L"Registeration Id";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_PHONE_TEXT"] = L"Phone";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_ALTERNATIVE_PHONE_TEXT"] = L"2nd Phone";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_CEO_NAME_TEXT"] = L"CEO";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_ECONOMICAL_CODE_TEXT"] = L"Economical Code";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_FOUNDING_YEAR_TEXT"] = L"Founding Year";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_ADDRESS_TEXT"] = L"Address";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_WEBSITE_TEXT"] = L"Website";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_RESUME_TEXT"] = L"Resume";
            LangStringsHash["ROOT_CMSUSERS_GET_INDIVIDUALS_TABLE_BUTTON"] = L"Show Registered Individual Users";
            LangStringsHash["ROOT_CMSUSERS_GET_COMPANIES_TABLE_BUTTON"] = L"Show Registered Company Users";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUALS_TABLE_DLG_TITLE"] = L"Registered Individual Users";
            LangStringsHash["ROOT_CMSUSERS_COMPANIES_TABLE_DLG_TITLE"] = L"Registered Company Users";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_BUTTON"] = L"Send a message to all users";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_SUBJECT_EMPTY_TEXT"] = L"Message subject....";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_BODY_EMPTY_TEXT"] = L"Message body....";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_TITLE"] = L"Send Message";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_BODY"] = L"You are going to send a message to all users."
                    "<br /><br />"
                    "Are you sure?"
                    "<br /><br />";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_YES_BUTTON"] = L"Yes";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_No_BUTTON"] = L"No";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_SENT_DLG_TITLE"] = L"Done";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_SENT_DLG_BODY"] = L"Message sent to all users successfully.";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_SENT_DLG_OK_BUTTON"] = L"OK";
            LangStringsHash["ROOT_CMSNEWS_NEWS_MANAGEMENT_TITLE"] = L"<h3>News Management</h3>";
            LangStringsHash["ROOT_CMSNEWS_ADD_BUTTON"] = L"Add News";
            LangStringsHash["ROOT_CMSNEWS_ADD_DLG_TITLE"] = L"Add News";
            LangStringsHash["ROOT_CMSNEWS_ADD_DLG_ADD_BUTTON"] = L"Add";
            LangStringsHash["ROOT_CMSNEWS_ADD_DLG_RETURN_BUTTON"] = L"Return";
            LangStringsHash["ROOT_CMSNEWS_NO_TEXT"] = L"No";
            LangStringsHash["ROOT_CMSNEWS_TITLE_TEXT"] = L"Title";
            LangStringsHash["ROOT_CMSNEWS_BODY_TEXT"] = L"Body";
            LangStringsHash["ROOT_CMSNEWS_DATE_TEXT"] = L"Date";
            LangStringsHash["ROOT_CMSNEWS_ARCHIVED_TEXT"] = L"Archive";
            LangStringsHash["ROOT_CMSNEWS_EDIT_TEXT"] = L"Edit";
            LangStringsHash["ROOT_CMSNEWS_EDIT_DLG_TITLE"] = L"Edit";
            LangStringsHash["ROOT_CMSNEWS_EDIT_DLG_EDIT_BUTTON"] = L"Edit";
            LangStringsHash["ROOT_CMSNEWS_ERASE_TEXT"] = L"Erase";
            LangStringsHash["ROOT_CMSNEWS_ERASE_MSG_TITLE"] = L"Erase";
            LangStringsHash["ROOT_CMSNEWS_ERASE_MSG_QUESTION"] = L"Do you want to remove <strong>%1%</strong>? Are you sure?";
            LangStringsHash["ROOT_CMSNEWS_ERASE_MSG_OK_BUTTON"] = L"OK";
            LangStringsHash["ROOT_CMSNEWS_ERASE_MSG_CANCEL_BUTTON"] = L"Cancel";
            LangStringsHash["ROOT_CMSNEWS_UNARCHIVE_TEXT"] = L"Unarchive";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_EDITCONTENTS"] = L"Edit Contents";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_INSERT"] = L"Insert";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_EDIT"] = L"Edit";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_ERASE"] = L"Erase";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_MOVEUP"] = L"Move Up";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_MOVEDOWN"] = L"Move Down";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DLG_TITLE"] = L"Insert Page";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DLG_PAGE_TITLE_TEXT"] = L"Page Title";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DLG_ADD_BUTTON"] = L"Add";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DLG_RETURN_BUTTON"] = L"Return";
            LangStringsHash["ROOT_CMSPAGES_INSERT_INVALID_TITLE"] = L"Error";
            LangStringsHash["ROOT_CMSPAGES_INSERT_INVALID_ERROR"] = L"Invalid page name!";
            LangStringsHash["ROOT_CMSPAGES_INSERT_INVALID_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DUPLICATE_TITLE"] = L"Error";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DUPLICATE_ERROR"] = L"Duplicate page name";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DUPLICATE_ERR_OK_BUTTON"] = L"OK";
            LangStringsHash["ROOT_CMSPAGES_EDIT_DLG_TITLE"] = L"Edit Page";
            LangStringsHash["ROOT_CMSPAGES_EDIT_DLG_PAGE_TITLE_TEXT"] = L"Page Title";
            LangStringsHash["ROOT_CMSPAGES_EDIT_DLG_SAVE_BUTTON"] = L"Save";
            LangStringsHash["ROOT_CMSPAGES_EDIT_DLG_RETURN_BUTTON"] = L"Return";
            LangStringsHash["ROOT_CMSPAGES_ERASE_MSG_TITLE"] = L"Erase";
            LangStringsHash["ROOT_CMSPAGES_ERASE_MSG_QUESTION"] = L"Do you want to remove <strong>%1%</strong>? Are you sure?";
            LangStringsHash["ROOT_CMSPAGES_ERASE_MSG_OK_BUTTON"] = L"OK";
            LangStringsHash["ROOT_CMSPAGES_ERASE_MSG_CANCEL_BUTTON"] = L"Cancel";
            LangStringsHash["ROOT_CMSPAGES_EDITOR_SAVE_BUTTON"] = L"Save";
            LangStringsHash["ROOT_CMSPAGES_EDITOR_CLOSE_BUTTON"] = L"Close";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_BUTTON_TEXT"] = L"Change";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_TTITLE"] = L"<h3>Change Root Password</h3>";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_CURRENT_PWD_TEXT"] = L"Current Password";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_NEW_PWD_TEXT"] = L"New Password";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_CONFIRM_PWD_TEXT"] = L"Confirm Password";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_TITLE"] = L"<h3>Change Root Email</h3>";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_EMAIL_TEXT"] = L"Email";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_PWD_TEXT"] = L"Password";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_INVALID_ERR"] = L"Invalid password.";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_SAME_ERR"] = L"Your new password must be different from your current password.";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_CONFIRM_ERR"] = L"Please confirm the new password.";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_SUCCESS_MSG"] = L"Password changed successfully.";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_INVALID_PWD_ERR"] = L"Invalid password.";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_SUCCESS_MSG"] = L"Email changed successfully.";
            LangStringsHash["ROOT_CMSCONTACTS_EDIT_CONTACTS_TITLE"] = L"<h3>Edit Contacts</h3>";
            LangStringsHash["ROOT_CMSCONTACTS_RECIPIENT_TEXT"] = L"Recipient";
            LangStringsHash["ROOT_CMSCONTACTS_ADDR_TEXT"] = L"Address";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_TEXT"] = L"Erase";
            LangStringsHash["ROOT_CMSCONTACTS_ADD_BUTTON"] = L"Add";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_BUTTON"] = L"X";
            LangStringsHash["ROOT_CMSCONTACTS_SAVE_BUTTON"] = L"Save";
            LangStringsHash["ROOT_CMSCONTACTS_CANCEL_BUTTON"] = L"Cancel";
            LangStringsHash["ROOT_CMSCONTACTS_EDIT_HINT"] = L"Note: Click on a table cell to edit contact information.";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_MSG_TITLE"] = L"Erase";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_MSG_QUESTION"] = L"Do you want to remove <strong>%1%</strong>? Are you sure?";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_MSG_OK_BUTTON"] = L"OK";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_MSG_CANCEL_BUTTON"] = L"Cancel";
            LangStringsHash["ROOT_CMSCONTACTS_ADD_DUPLICATE_CONTACT_ERR"] = L"Contact already exists!!";
            LangStringsHash["ROOT_CMSCONTACTS_EXTRA_INFO_EDITOR_SAVE_BUTTON"] = L"Save Extra Contact Info";
            LangStringsHash["ROOT_CMSCONTACTS_EXTRA_INFO_EDITOR_SAVE_SUCCESS_MSG"] = L"Extra Contact Info saved successfully.";
            LangStringsHash["ROOT_CMSIDEAS_SHOW_IDEAS_TABLE_BUTTON"] = L"Show Ideas";
            LangStringsHash["ROOT_CMSIDEAS_SHOW_IDEAS_TABLE_DLG_TITLE"] = L"Registered Ideas";
            LangStringsHash["ROOT_CMSIDEAS_NO_TEXT"] = L"No";
            LangStringsHash["ROOT_CMSIDEAS_TITLE_TEXT"] = L"Title";
            LangStringsHash["ROOT_CMSIDEAS_SUBMITTER_TEXT"] = L"Submitter";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_TEXT"] = L"Type";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_TEXT"] = L"Audience";
            LangStringsHash["ROOT_CMSIDEAS_DETAILS_TEXT"] = L"Details";
            LangStringsHash["ROOT_CMSIDEAS_DESCRIPTION_TEXT"] = L"Description";
            LangStringsHash["ROOT_CMSIDEAS_USAGE_TEXT"] = L"Usage";
            LangStringsHash["ROOT_CMSIDEAS_SIMILAR_TEXT"] = L"Similar";
            LangStringsHash["ROOT_CMSIDEAS_DIFFERENCE_TEXT"] = L"Difference";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_GAME_ENTERTAINMENT"] = L"Game & Entertainment";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_HYGIENE_HEALTH"] = L"Hygien & Health";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_NEWS_INFO"] = L"News & Information";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_LEARNING"] = L"Learning";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_NONE_PRESENCE_SERVICES"] = L"None Presence Services";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_ONLINE_SHOPPING"] = L"Online Shopping";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_OTHER"] = L"Other";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_TEENS"] = L"Teens";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_PARENTS"] = L"Parents";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_BOYS"] = L"Boys";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_GIRLS"] = L"Girls";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_WOMEN"] = L"Women";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_MEN"] = L"Men";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_CHILDS"] = L"Childs";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_ALL_FAMILY_MEMBERS"] = L"All Family Members";
            LangStringsHash["ROOT_CMSAPPS_SHOW_APPS_TABLE_BUTTON"] = L"Show Apps";
            LangStringsHash["ROOT_CMSAPPS_SHOW_APPS_TABLE_DLG_TITLE"] = L"Registered Apps";
            LangStringsHash["ROOT_CMSAPPS_NO_TEXT"] = L"No";
            LangStringsHash["ROOT_CMSAPPS_TITLE_TEXT"] = L"Title";
            LangStringsHash["ROOT_CMSAPPS_SUBMITTER_TEXT"] = L"Submitter";
            LangStringsHash["ROOT_CMSAPPS_TYPE_TEXT"] = L"Type";
            LangStringsHash["ROOT_CMSAPPS_PLATFORM_TEXT"] = L"Platform";
            LangStringsHash["ROOT_CMSAPPS_LANGUAGE_TEXT"] = L"Language";
            LangStringsHash["ROOT_CMSAPPS_DETAILS_TEXT"] = L"Details";
            LangStringsHash["ROOT_CMSAPPS_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["ROOT_CMSAPPS_TYPE_GAME_ENTERTAINMENT_TEXT"] = L"Game / Entertainment";
            LangStringsHash["ROOT_CMSAPPS_TYPE_ONLINE_GAME_TEXT"] = L"Online Game";
            LangStringsHash["ROOT_CMSAPPS_TYPE_LEARN_INFO_TEXT"] = L"Learning / Information";
            LangStringsHash["ROOT_CMSAPPS_TYPE_SERVICES_TEXT"] = L"Services";
            LangStringsHash["ROOT_CMSAPPS_TYPE_TOOLS_TEXT"] = L"Tools / Utility";
            LangStringsHash["ROOT_CMSAPPS_DISPLAY_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["ROOT_CMSAPPS_DISPLAY_TYPE_FULLSCREEN_TEXT"] = L"Fullscreen";
            LangStringsHash["ROOT_CMSAPPS_DISPLAY_TYPE_ON_CORNER_OF_SCREEN_TEXT"] = L"On Corner of Screen";
            LangStringsHash["ROOT_CMSAPPS_DISPLAY_TYPE_ACTIVE_ON_SCREEN_TEXT"] = L"Active on Screen";
            LangStringsHash["ROOT_CMSAPPS_PLATFORM_HTML5_TEXT"] = L"HTML5";
            LangStringsHash["ROOT_CMSAPPS_PLATFORM_JAVASCRIPT_TEXT"] = L"JavaScript";
            LangStringsHash["ROOT_CMSAPPS_PLATFORM_FLASH_TEXT"] = L"Flash";
            LangStringsHash["ROOT_CMSAPPS_DESCRIPTION_TEXT"] = L"Description";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_TEXT"] = L"Screenshots";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_1ST_TEXT"] = L"1st screenshot";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_2ND_TEXT"] = L"2nd screenshot";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_3RD_TEXT"] = L"3rd screenshot";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_4TH_TEXT"] = L"4th screenshot";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_DESCRIPTION_TEXT"] = L"Screenshots Description";
            LangStringsHash["ROOT_CMSAPPS_DISPLAY_TYPE_TEXT"] = L"Display Type";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_TEXT"] = L"Technical Description";
            LangStringsHash["ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_TEXT"] = L"Audio & Video Status";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_TEXT"] = L"Remote Buttons";
            LangStringsHash["ROOT_CMSAPPS_APP_BINARY_TEXT"] = L"App binary file";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_SUPPORTS_IP_RANGE_TEXT"] = L"Supports IP Range";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_PWD_PROTECTED_TEXT"] = L"Password Protected";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_HAS_COOKIES_TEXT"] = L"Has Cookies";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_AUTO_LOGIN_TEXT"] = L"Auto Login";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_HAS_FILM_TEXT"] = L"Has Film";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_HAS_AUDIO_TEXT"] = L"Has Audio";
            LangStringsHash["ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_NO_AUDIO_NO_VIDEO_TEXT"] = L"No Audio or Video";
            LangStringsHash["ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_NO_BUFFER_TEXT"] = L"Audio/Video included, No need for buffering";
            LangStringsHash["ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_WITH_PLAYBACK_TEXT"] = L"With playback buttons";
            LangStringsHash["ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_FULLSCREEN_WITH_EXIT_TEXT"] = L"Fullscreen with exit button";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_NUMS_TEXT"] = L"Nums";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_ARROWS_TEXT"] = L"Arrows";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_TOOLS_INFO_TEXT"] = L"Tools / Info";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_RETURN_EXIT_TEXT"] = L"Return / Exit";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_PLAYBACK_TEXT"] = L"Playback";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_COLORS_TUNING_TEXT"] = L"Color Tuning";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_VOLUME_TUNING_TEXT"] = L"Volume Tuning";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_SHOW_APPOINTMENTS_TABLE_BUTTON"] = L"Show Appointments";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_SHOW_APPOINTMENTS_TABLE_DLG_TITLE"] = L"Registered Appointments";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_NO_TEXT"] = L"No";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_SUPPLIANT_TEXT"] = L"Suppliant";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_DATE_TEXT"] = L"Date";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_HOUR_TEXT"] = L"Hour";
            break;

        case CgiEnv::ELang_RootFa:
            LangStringsHash["ROOT_LOGIN_TITLE"] = L".:: ورود ::.";
            LangStringsHash["ROOT_CMS_TITLE"] = L"سیستم مدیریت محتوای SamsungDForum.ir";
            LangStringsHash["ROOT_LOGIN_FORM_TITLE"] = L"<h3>ورود به پنل کنفرانس</h3>";
            LangStringsHash["ROOT_LOGIN_FORM_USER_TEXT"] = L"نام کاربری";
            LangStringsHash["ROOT_LOGIN_FORM_PWD_TEXT"] = L"گذر واژه";
            LangStringsHash["ROOT_LOGIN_FORM_CAPTCHA_TEXT"] = L"کد امنیتی";
            LangStringsHash["ROOT_LOGIN_FORM_EMAIL_TEXT"] = L"پست الکترونیک";
            LangStringsHash["ROOT_LOGIN_FORM_LOGIN_TEXT"] = L"ورود";
            LangStringsHash["ROOT_LOGIN_FORM_RECOVER_TEXT"] = L"یادآوری";
            LangStringsHash["ROOT_LOGIN_FORM_FORGOT_TEXT"] = L"کلمه عبور را فراموش کرده ام";
            LangStringsHash["ROOT_LOGIN_FORM_ERR_LOGIN"] = L"نام کاربری یا کلمه عبور نامعتبر می باشد";
            LangStringsHash["ROOT_LOGIN_FORM_ERR_FORGOT"] = L"نام کاربری نامعتبر می باشد";
            LangStringsHash["ROOT_LOGIN_FORM_MSG_FORGOT"] = L"کلمه عبور با موفقیت بازیابی شد، لطفا صندوق پست الکترونیک تان را چک نمائید";
            LangStringsHash["ROOT_CMS_MENU_WELCOME"] = L"خوش آمدید";
            LangStringsHash["ROOT_CMS_MENU_USERS"] = L"کاربران";
            LangStringsHash["ROOT_CMS_MENU_IDEAS"] = L"ایده ها";
            LangStringsHash["ROOT_CMS_MENU_APPS"] = L"برنامه ها";
            LangStringsHash["ROOT_CMS_MENU_APPOINTMENTS"] = L"قرارهای ملاقات";
            LangStringsHash["ROOT_CMS_MENU_NEWS"] = L"اخبار";
            LangStringsHash["ROOT_CMS_MENU_PAGES"] = L"صفحات";
            LangStringsHash["ROOT_CMS_MENU_CONTACTS"] = L"تماس ها";
            LangStringsHash["ROOT_CMS_MENU_PWDEMAIL"] = L"کلمه عبور / پست الکترونیک";
            LangStringsHash["ROOT_CMS_MENU_SYSMON"] = L"وضعیت سیستم";
            LangStringsHash["ROOT_CMS_MENU_EXIT"] = L"خروج";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_TABLE_TITLE"] = L"<h3>کاربران حقیقی</h3>";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_TABLE_TITLE"] = L"<h3>کاربران حقوقی</h3>";
            LangStringsHash["ROOT_CMSUSERS_NO_TEXT"] = L"ردیف";
            LangStringsHash["ROOT_CMSUSERS_EMAIL_TEXT"] = L"پست الکترونیکی";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_FIRST_NAME_TEXT"] = L"نام";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_LAST_NAME_TEXT"] = L"نام خانوادگی";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_CONTACT_NUMBER_TEXT"] = L"تلفن تماس";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_ALTERNATIVE_CONTACT_NUMBER"] = L"تلفن جایگزین";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_TEAMMATES_TEXT"] = L"همکاران";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_NATIONAL_CODE_TEXT"] = L"کد ملی";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_TEXT"] = L"سطح تحصیلات";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_TEXT"] = L"سطح تحصیلات";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_NONE_TEXT"] = L"----";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_DIPLOMA_TEXT"] = L"دیپلم";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_AD_TEXT"] = L"کاردانی";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_BA_TEXT"] = L"کارشناسی";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_MA_TEXT"] = L"کارشناسی ارشد";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_LEVEL_PHD_TEXT"] = L"دکترا";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_EDUCATIONAL_SUBJECT_TEXT"] = L"رشته تحصیلی";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUAL_WEBSITE_TEXT"] = L"وب سایت";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_NAME_TEXT"] = L"نام شرکت";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_REGISTERATION_ID_TEXT"] = L"شماره ثبت";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_PHONE_TEXT"] = L"تلفن تماس";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_ALTERNATIVE_PHONE_TEXT"] = L"تلفن جایگزین";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_CEO_NAME_TEXT"] = L"نام مدیر عامل";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_ECONOMICAL_CODE_TEXT"] = L"کد اقتصادی";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_FOUNDING_YEAR_TEXT"] = L"سال تاسیس";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_ADDRESS_TEXT"] = L"آدرس";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_WEBSITE_TEXT"] = L"وب سایت";
            LangStringsHash["ROOT_CMSUSERS_COMPANY_RESUME_TEXT"] = L"رزومه";
            LangStringsHash["ROOT_CMSUSERS_GET_INDIVIDUALS_TABLE_BUTTON"] = L"نمایش لیست کاربران حقیقی";
            LangStringsHash["ROOT_CMSUSERS_GET_COMPANIES_TABLE_BUTTON"] = L"نمایش لیست کاربران حقوقی";
            LangStringsHash["ROOT_CMSUSERS_INDIVIDUALS_TABLE_DLG_TITLE"] = L"کاربران حقیقی";
            LangStringsHash["ROOT_CMSUSERS_COMPANIES_TABLE_DLG_TITLE"] = L"کاربران حقو.قی";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_BUTTON"] = L"ارسال پیام برای تمامی کاربران";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_SUBJECT_EMPTY_TEXT"] = L"عنوان پیام....";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_BODY_EMPTY_TEXT"] = L"متن پیام....";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_TITLE"] = L"ارسال پیام";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_BODY"] = L"شما در حال ارسال پیام به تمامی کاربران می باشید."
                    "<br /><br />"
                    "آیا از ارسال پیام مطمئن می باشید؟"
                    "<br /><br />";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_YES_BUTTON"] = L"بلی";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_CONFIRMATION_DLG_No_BUTTON"] = L"خیر";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_SENT_DLG_TITLE"] = L"فرستاده شده";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_SENT_DLG_BODY"] = L"پیام با موفقیت برای تمامی کاربران ارسال شد.";
            LangStringsHash["ROOT_CMSUSERS_SEND_MESSAGE_SENT_DLG_OK_BUTTON"] = L"تائید";
            LangStringsHash["ROOT_CMSNEWS_NEWS_MANAGEMENT_TITLE"] = L"<h3>مدیریت اخبار</h3>";
            LangStringsHash["ROOT_CMSNEWS_ADD_BUTTON"] = L"درج خبر جدید";
            LangStringsHash["ROOT_CMSNEWS_ADD_DLG_TITLE"] = L"درج خبر";
            LangStringsHash["ROOT_CMSNEWS_ADD_DLG_ADD_BUTTON"] = L"درج";
            LangStringsHash["ROOT_CMSNEWS_ADD_DLG_RETURN_BUTTON"] = L"بازگشت";
            LangStringsHash["ROOT_CMSNEWS_NO_TEXT"] = L"ردیف";
            LangStringsHash["ROOT_CMSNEWS_TITLE_TEXT"] = L"عنوان";
            LangStringsHash["ROOT_CMSNEWS_BODY_TEXT"] = L"متن";
            LangStringsHash["ROOT_CMSNEWS_DATE_TEXT"] = L"تاریخ";
            LangStringsHash["ROOT_CMSNEWS_ARCHIVED_TEXT"] = L"آرشیو";
            LangStringsHash["ROOT_CMSNEWS_EDIT_TEXT"] = L"ویرایش";
            LangStringsHash["ROOT_CMSNEWS_EDIT_DLG_TITLE"] = L"ویرایش";
            LangStringsHash["ROOT_CMSNEWS_EDIT_DLG_EDIT_BUTTON"] = L"ویرایش";
            LangStringsHash["ROOT_CMSNEWS_ERASE_TEXT"] = L"حذف";
            LangStringsHash["ROOT_CMSNEWS_ERASE_MSG_TITLE"] = L"حذف";
            LangStringsHash["ROOT_CMSNEWS_ERASE_MSG_QUESTION"] = L"آیا مایل به حذف خبر <strong>%1%</strong> می باشید?";
            LangStringsHash["ROOT_CMSNEWS_ERASE_MSG_OK_BUTTON"] = L"تائید";
            LangStringsHash["ROOT_CMSNEWS_ERASE_MSG_CANCEL_BUTTON"] = L"لغو";
            LangStringsHash["ROOT_CMSNEWS_UNARCHIVE_TEXT"] = L"خروج از آرشیو";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_EDITCONTENTS"] = L"ویرایش محتویات";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_INSERT"] = L"درج";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_EDIT"] = L"ویرایش";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_ERASE"] = L"حذف";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_MOVEUP"] = L"حرکت به بالا";
            LangStringsHash["ROOT_CMSPAGES_CTX_ITEM_MOVEDOWN"] = L"حرکت به پائین";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DLG_TITLE"] = L"درج صفحه";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DLG_PAGE_TITLE_TEXT"] = L"عنوان صفحه";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DLG_ADD_BUTTON"] = L"درج";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DLG_RETURN_BUTTON"] = L"بازگشت";
            LangStringsHash["ROOT_CMSPAGES_INSERT_INVALID_TITLE"] = L"خطا";
            LangStringsHash["ROOT_CMSPAGES_INSERT_INVALID_ERROR"] = L"نام صفحه نا معتبر می باشد!";
            LangStringsHash["ROOT_CMSPAGES_INSERT_INVALID_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DUPLICATE_TITLE"] = L"خطا";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DUPLICATE_ERROR"] = L"نام صفحه تکراری است!";
            LangStringsHash["ROOT_CMSPAGES_INSERT_DUPLICATE_ERR_OK_BUTTON"] = L"تائید";
            LangStringsHash["ROOT_CMSPAGES_EDIT_DLG_TITLE"] = L"ویرایش صفحه";
            LangStringsHash["ROOT_CMSPAGES_EDIT_DLG_PAGE_TITLE_TEXT"] = L"عنوان صفحه";
            LangStringsHash["ROOT_CMSPAGES_EDIT_DLG_SAVE_BUTTON"] = L"ذخیره";
            LangStringsHash["ROOT_CMSPAGES_EDIT_DLG_RETURN_BUTTON"] = L"بازگشت";
            LangStringsHash["ROOT_CMSPAGES_ERASE_MSG_TITLE"] = L"حذف";
            LangStringsHash["ROOT_CMSPAGES_ERASE_MSG_QUESTION"] = L"آیا مایل به حذف <strong>%1%</strong> می باشید?";
            LangStringsHash["ROOT_CMSPAGES_ERASE_MSG_OK_BUTTON"] = L"تائید";
            LangStringsHash["ROOT_CMSPAGES_ERASE_MSG_CANCEL_BUTTON"] = L"لغو";
            LangStringsHash["ROOT_CMSPAGES_EDITOR_SAVE_BUTTON"] = L"دخیره";
            LangStringsHash["ROOT_CMSPAGES_EDITOR_CLOSE_BUTTON"] = L"بستن";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_BUTTON_TEXT"] = L"تغییر";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_TTITLE"] = L"<h3>تغییر کلمه عبور مدیر</h3>";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_CURRENT_PWD_TEXT"] = L"کلمه عبور فعلی";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_NEW_PWD_TEXT"] = L"کلمه عبور جدید";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_CONFIRM_PWD_TEXT"] = L"تائید کلمه عبور";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_TITLE"] = L"<h3>تغییر پست الکترونیکی مدیر</h3>";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_EMAIL_TEXT"] = L"پست الکترونیکی";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_PWD_TEXT"] = L"کلمه عبور";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_INVALID_ERR"] = L"کلمه عبور نامعتبر می باشد.";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_SAME_ERR"] = L"کلمه عبور جدید باسیتی با کلمه عبور فعلی متفاوت باشد.";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_CONFIRM_ERR"] = L"لطفا کلمه عبور جدید را تائید نمائید.";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_PWD_SUCCESS_MSG"] = L"کلمه عبور با موفقیت تغییر یافت.";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_INVALID_PWD_ERR"] = L"کلمه عبور نامعتبر می باشد.";
            LangStringsHash["ROOT_CMSPWDEMAIL_CHANGE_ROOT_EMAIL_SUCCESS_MSG"] = L"آدر پست الکترونیکی با موفقیت تغییر یافت.";
            LangStringsHash["ROOT_CMSCONTACTS_EDIT_CONTACTS_TITLE"] = L"<h3>ویرایش تماس ها</h3>";
            LangStringsHash["ROOT_CMSCONTACTS_RECIPIENT_TEXT"] = L"گیرنده";
            LangStringsHash["ROOT_CMSCONTACTS_ADDR_TEXT"] = L"پست الکترونیکی";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_TEXT"] = L"حذف";
            LangStringsHash["ROOT_CMSCONTACTS_ADD_BUTTON"] = L"افزودن";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_BUTTON"] = L"X";
            LangStringsHash["ROOT_CMSCONTACTS_SAVE_BUTTON"] = L"ذخیره";
            LangStringsHash["ROOT_CMSCONTACTS_CANCEL_BUTTON"] = L"لغو";
            LangStringsHash["ROOT_CMSCONTACTS_EDIT_HINT"] = L"جهت ویرایش اطلاعات تماس بر روی خانه موردنظر از جدول کلیک نمائید.";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_MSG_TITLE"] = L"حذف";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_MSG_QUESTION"] = L"آیا مایل به حذف <strong>%1%</strong> می باشید?";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_MSG_OK_BUTTON"] = L"تائید";
            LangStringsHash["ROOT_CMSCONTACTS_ERASE_MSG_CANCEL_BUTTON"] = L"لغو";
            LangStringsHash["ROOT_CMSCONTACTS_ADD_DUPLICATE_CONTACT_ERR"] = L"تماس مورد نظر قبلا ایجاد شده است!!";
            LangStringsHash["ROOT_CMSCONTACTS_EXTRA_INFO_EDITOR_SAVE_BUTTON"] = L"ذخیره اطلاعات اضافی تماس";
            LangStringsHash["ROOT_CMSCONTACTS_EXTRA_INFO_EDITOR_SAVE_SUCCESS_MSG"] = L"اطلاعات اضافی تماس با موفقیت ذخیره شد.";
            LangStringsHash["ROOT_CMSIDEAS_SHOW_IDEAS_TABLE_BUTTON"] = L"مشاهده ایده ها";
            LangStringsHash["ROOT_CMSIDEAS_SHOW_IDEAS_TABLE_DLG_TITLE"] = L"ایده های ثبت شده";
            LangStringsHash["ROOT_CMSIDEAS_NO_TEXT"] = L"ردیف";
            LangStringsHash["ROOT_CMSIDEAS_TITLE_TEXT"] = L"عنوان";
            LangStringsHash["ROOT_CMSIDEAS_SUBMITTER_TEXT"] = L"ارسال کننده";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_TEXT"] = L"نوع";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_TEXT"] = L"مخاطب";
            LangStringsHash["ROOT_CMSIDEAS_DETAILS_TEXT"] = L"جزئیات";
            LangStringsHash["ROOT_CMSIDEAS_DESCRIPTION_TEXT"] = L"شرح ایده";
            LangStringsHash["ROOT_CMSIDEAS_USAGE_TEXT"] = L"کاربرد ایده";
            LangStringsHash["ROOT_CMSIDEAS_SIMILAR_TEXT"] = L"مشابه";
            LangStringsHash["ROOT_CMSIDEAS_DIFFERENCE_TEXT"] = L"تفاوت";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_GAME_ENTERTAINMENT"] = L"بازی و سرگرمی";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_HYGIENE_HEALTH"] = L"بهداشت و سلامت";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_NEWS_INFO"] = L"اخبار و اطلاعات";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_LEARNING"] = L"آموزش";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_NONE_PRESENCE_SERVICES"] = L"خدمات غیرحضوری";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_ONLINE_SHOPPING"] = L"خرید آنلاین";
            LangStringsHash["ROOT_CMSIDEAS_TYPE_OTHER"] = L"سایر";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_TEENS"] = L"نوجوانان";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_PARENTS"] = L"والدین";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_BOYS"] = L"پسران";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_GIRLS"] = L"دختران";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_WOMEN"] = L"زنان";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_MEN"] = L"مردان";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_CHILDS"] = L"کودکان";
            LangStringsHash["ROOT_CMSIDEAS_AUDIENCE_ALL_FAMILY_MEMBERS"] = L"تمامی اعضای خانواده";
            LangStringsHash["ROOT_CMSAPPS_SHOW_APPS_TABLE_BUTTON"] = L"مشاهده برنامه ها";
            LangStringsHash["ROOT_CMSAPPS_SHOW_APPS_TABLE_DLG_TITLE"] = L"برنامه های ثبت شده";
            LangStringsHash["ROOT_CMSAPPS_NO_TEXT"] = L"ردیف";
            LangStringsHash["ROOT_CMSAPPS_TITLE_TEXT"] = L"عنوان";
            LangStringsHash["ROOT_CMSAPPS_SUBMITTER_TEXT"] = L"ارسال کننده";
            LangStringsHash["ROOT_CMSAPPS_TYPE_TEXT"] = L"دسته بندی";
            LangStringsHash["ROOT_CMSAPPS_PLATFORM_TEXT"] = L"پلتفرم";
            LangStringsHash["ROOT_CMSAPPS_LANGUAGE_TEXT"] = L"زبان";
            LangStringsHash["ROOT_CMSAPPS_DETAILS_TEXT"] = L"جزئیات";
            LangStringsHash["ROOT_CMSAPPS_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["ROOT_CMSAPPS_TYPE_GAME_ENTERTAINMENT_TEXT"] = L"بازی / سرگرمی";
            LangStringsHash["ROOT_CMSAPPS_TYPE_ONLINE_GAME_TEXT"] = L"بازی آنلاین";
            LangStringsHash["ROOT_CMSAPPS_TYPE_LEARN_INFO_TEXT"] = L"آموزش و اطلاعات";
            LangStringsHash["ROOT_CMSAPPS_TYPE_SERVICES_TEXT"] = L"خدمات غیرحضوری";
            LangStringsHash["ROOT_CMSAPPS_TYPE_TOOLS_TEXT"] = L"نرم افزارهای کمکی";
            LangStringsHash["ROOT_CMSAPPS_DISPLAY_TYPE_NONE_TEXT"] = L"----";
            LangStringsHash["ROOT_CMSAPPS_DISPLAY_TYPE_FULLSCREEN_TEXT"] = L"نرم افزار صفحه کامل";
            LangStringsHash["ROOT_CMSAPPS_DISPLAY_TYPE_ON_CORNER_OF_SCREEN_TEXT"] = L"در گوشه ای از صفحه نمایش اجرا می شود";
            LangStringsHash["ROOT_CMSAPPS_DISPLAY_TYPE_ACTIVE_ON_SCREEN_TEXT"] = L"فعال بر روی صفحه نمایش";
            LangStringsHash["ROOT_CMSAPPS_PLATFORM_HTML5_TEXT"] = L"HTML5";
            LangStringsHash["ROOT_CMSAPPS_PLATFORM_JAVASCRIPT_TEXT"] = L"JavaScript";
            LangStringsHash["ROOT_CMSAPPS_PLATFORM_FLASH_TEXT"] = L"Flash";
            LangStringsHash["ROOT_CMSAPPS_DESCRIPTION_TEXT"] = L"توضیحات";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_TEXT"] = L"نمای نرم افزار";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_1ST_TEXT"] = L"تصویر اول";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_2ND_TEXT"] = L"تصویر دوم";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_3RD_TEXT"] = L"تصویر سوم";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_4TH_TEXT"] = L"تصویر چهارم";
            LangStringsHash["ROOT_CMSAPPS_SCREENSHOTS_DESCRIPTION_TEXT"] = L"توضیحات نمای نرم افزار";
            LangStringsHash["ROOT_CMSAPPS_DISPLAY_TYPE_TEXT"] = L"نوع صفحه نمایش";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_TEXT"] = L"توضیحات فنی";
            LangStringsHash["ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_TEXT"] = L"وضعیت صدا و تصویر";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_TEXT"] = L"دکمه های ریموت";
            LangStringsHash["ROOT_CMSAPPS_APP_BINARY_TEXT"] = L"فایل برنامه";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_SUPPORTS_IP_RANGE_TEXT"] = L"پشتیبانی از محدوده IP";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_PWD_PROTECTED_TEXT"] = L"محافظت با کلمه عبور";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_HAS_COOKIES_TEXT"] = L"کوکی";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_AUTO_LOGIN_TEXT"] = L"لاگین خودکار";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_HAS_FILM_TEXT"] = L"فیلم تصویری";
            LangStringsHash["ROOT_CMSAPPS_TECHNICAL_DESCRIPTION_HAS_AUDIO_TEXT"] = L"صوت / موسیقی";
            LangStringsHash["ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_NO_AUDIO_NO_VIDEO_TEXT"] = L"فاقد فیلم / صدا";
            LangStringsHash["ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_NO_BUFFER_TEXT"] = L"فیلم / صدا به همراه نرم افزار بدون نیاز به بافرینگ";
            LangStringsHash["ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_WITH_PLAYBACK_TEXT"] = L"قابلیت کنترل صوت و تصویر به همراه پلی بک";
            LangStringsHash["ROOT_CMSAPPS_AUDIO_VIDEO_STATUS_FULLSCREEN_WITH_EXIT_TEXT"] = L"تمام صفحه به همراه دکمه خروج";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_NUMS_TEXT"] = L"اعداد";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_ARROWS_TEXT"] = L"۴ جهت";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_TOOLS_INFO_TEXT"] = L"Tools / Info";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_RETURN_EXIT_TEXT"] = L"بازگشت و خروج";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_PLAYBACK_TEXT"] = L"Playback";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_COLORS_TUNING_TEXT"] = L"تنظیم رنگ";
            LangStringsHash["ROOT_CMSAPPS_REMOTE_BUTTONS_VOLUME_TUNING_TEXT"] = L"تنظیم صدا";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_SHOW_APPOINTMENTS_TABLE_BUTTON"] = L"مشاهده قرارهای ملاقات";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_SHOW_APPOINTMENTS_TABLE_DLG_TITLE"] = L"قرارهای ملاقات ثبت شده";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_NO_TEXT"] = L"ردیف";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_SUPPLIANT_TEXT"] = L"متقاضی";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_DATE_TEXT"] = L"تاریخ";
            LangStringsHash["ROOT_CMSAPPOINTMENTS_HOUR_TEXT"] = L"ساعت";
            break;

        default:
            break;
        }
    }
};


/**********************************
*    = Constructos =
**********************************/

Lang::Lang(const CgiEnv::ELang &lang) : m_pimpl(new Impl(lang))
{
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

wstring Lang::GetString(const char *stringId)
{
    if (m_pimpl->LangStringsHash.find(stringId) != m_pimpl->LangStringsHash.end()) {
        return m_pimpl->LangStringsHash[stringId];
    } else {
        DBG(stringId);
        return L"{?}";
    }
}


/**********************************
*    = Event Handlers =
**********************************/

/**********************************
*    = Protected Methods =
**********************************/

/**********************************
*    = Private Methods =
**********************************/

/**********************************
*    = Base Class Overrides =
**********************************/

/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


