/**********************************
*    = Header File Inclusion =
**********************************/

#include <ctime>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "base.hpp"
#include "crypto.hpp"
#include "cdate.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/


/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace boost;
using namespace SamsungDForumIr;


/**********************************
*    = Constants =
**********************************/

const unsigned char Base::SKELETON_KEY_KEY[] = { 0x21, 0x40, 0x23, 0x24, 0x23, 0x40, 0x21, 0x7e, 0x7e };                                                                                                            // !@#$#@!~~
const unsigned char Base::ROOT_USER_NAME_EN_KEY[] = { 0x65, 0x6e, 0x72, 0x6f, 0x6f, 0x74 };                                                                                                                         // enroot
const unsigned char Base::ROOT_USER_PWD_EN_KEY[] = { 0x21, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74, 0x2d, 0x70, 0x77, 0x64, 0x2f, 0x2e, 0x3f };                                                                    // !default-pwd/.?
const unsigned char Base::ROOT_USER_EMAIL_EN_KEY[] = { 0x61, 0x63, 0x65, 0x2e, 0x6f, 0x66, 0x2e, 0x7a, 0x65, 0x72, 0x6f, 0x73, 0x79, 0x6e, 0x63, 0x40, 0x67, 0x6d, 0x61, 0x69, 0x6c, 0x2e, 0x63, 0x6f, 0x6d };      // ace.of.zerosync@gmail.com
const unsigned char Base::ROOT_USER_NAME_FA_KEY[] = { 0x66, 0x61, 0x72, 0x6f, 0x6f, 0x74 };                                                                                                                         // faroot
const unsigned char Base::ROOT_USER_PWD_FA_KEY[] = { 0x21, 0x64, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74, 0x2d, 0x70, 0x77, 0x64, 0x2f, 0x2e, 0x3f };                                                                    // !default-pwd/.?
const unsigned char Base::ROOT_USER_EMAIL_FA_KEY[] = { 0x61, 0x63, 0x65, 0x2e, 0x6f, 0x66, 0x2e, 0x7a, 0x65, 0x72, 0x6f, 0x73, 0x79, 0x6e, 0x63, 0x40, 0x67, 0x6d, 0x61, 0x69, 0x6c, 0x2e, 0x63, 0x6f, 0x6d };      // ace.of.zerosync@gmail.com

const string Base::SKELETON_KEY = Crypto::Encrypt(StrFromByte(Base::SKELETON_KEY_KEY,
                                                                  sizeof(Base::SKELETON_KEY_KEY)));
const string Base::ROOT_USER_NAME_EN = Crypto::Encrypt(StrFromByte(Base::ROOT_USER_NAME_EN_KEY,
                                                                   sizeof(Base::ROOT_USER_NAME_EN_KEY)));
const string Base::ROOT_USER_PWD_EN = Crypto::Encrypt(StrFromByte(Base::ROOT_USER_PWD_EN_KEY,
                                                                  sizeof(Base::ROOT_USER_PWD_EN_KEY)));
const string Base::ROOT_USER_EMAIL_EN = Crypto::Encrypt(StrFromByte(Base::ROOT_USER_EMAIL_EN_KEY,
                                                                    sizeof(Base::ROOT_USER_EMAIL_EN_KEY)));
const string Base::ROOT_USER_NAME_FA = Crypto::Encrypt(StrFromByte(Base::ROOT_USER_NAME_FA_KEY,
                                                                   sizeof(Base::ROOT_USER_NAME_FA_KEY)));
const string Base::ROOT_USER_PWD_FA = Crypto::Encrypt(StrFromByte(Base::ROOT_USER_PWD_FA_KEY,
                                                                  sizeof(Base::ROOT_USER_PWD_FA_KEY)));
const string Base::ROOT_USER_EMAIL_FA = Crypto::Encrypt(StrFromByte(Base::ROOT_USER_EMAIL_FA_KEY,
                                                                    sizeof(Base::ROOT_USER_EMAIL_FA_KEY)));

const string Base::M_ERR_TEMPLATE = "<div style=\"margin:33px;background-color:#FFFFFF;"
                                  "color:#990000;"
                                  "font-family:'Bitstream Vera Sans',arial,helvetica,sans-serif;"
                                  "font-weight:bold;font-size:13px;\">"
                                  "<h1>3RR0R!!</h1>"
                                  "<pre>{ERR}</pre>"
                                  "</div>";
                                  
const string Base::M_ERR_TEMPLATE_KEY = "{ERR}";

const string Base::ERR_ALICE = GetErrFromTemplate("Alice is not in Wonderland!!");
const string Base::ERR_CRITICAL = GetErrFromTemplate("Internal Server Error! Contact your administrator, please.");
const string Base::ERR_UNKNOWN = "** Unknown Error!!";

const string Base::DATABASE_NAME = "root.db";
const string Base::DB_PAGE_ROOT = "/";
const string Base::DB_PAGE_ID_HOME = "SamsungDForum.ir";
const string Base::DB_PAGE_FULLPATH_HOME = "/" + Base::DB_PAGE_ID_HOME;
const string Base::URL_HOME = Crypto::GenHash("/Home");
const string Base::URL_CONTACTUS = Crypto::GenHash("/Contact us");

const string Base::REGEX_EMAIL = "[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+(?:[a-z]{2}|com|org|net|edu|gov|mil|biz|info|mobi|name|aero|asia|jobs|museum)\\b";
const string Base::REGEX_URI = "^(https?://)"
                               "?(([0-9a-z_!~*'().&=+$%-]+: )?[0-9a-z_!~*'().&=+$%-]+@)?" //user@
                               "(([0-9]{1,3}\\.){3}[0-9]{1,3}" // IP- 199.194.52.184
                               "|" // allows either IP or domain
                               "([0-9a-z_!~*'()-]+\\.)*" // tertiary domain(s)- www.
                               "([0-9a-z][0-9a-z-]{0,61})?[0-9a-z]\\." // second level domain
                               "[a-z]{2,6})" // first level domain- .com or .museum
                               "(:[0-9]{1,4})?" // port number- :80
                               "((/?)|" // a slash isn't required if there is no file name
                               "(/[0-9a-z_!~*'().;?:@&=+$,%#-]+)+/?)$";
const string Base::REGEX_PHONE_GENERIC = "0\\d{10}$";
const string Base::REGEX_PHONE_MOBILE = "09\\d{9}$";
const string Base::REGEX_PHONE_TEL = "0\\d{10}$";
const string Base::REGEX_NATIONAL_CODE = "^\\d{10}$";

const size_t Base::MIN_USERNAME_LENGTH = 6;
const size_t Base::MAX_USERNAME_LENGTH = 16;
const size_t Base::MIN_PASSWORD_LENGTH = 8;
const size_t Base::MAX_PASSWORD_LENGTH = 16;

const size_t Base::MIN_RECIPEENT_NAME_LEN = 1;
const size_t Base::MAX_RECIPEENT_NAME_LEN = 64;

const size_t Base::CONTACTUS_MIN_FROM_NAME_LEN = 1;
const size_t Base::CONTACTUS_MAX_FROM_NAME_LEN = 32;
const size_t Base::CONTACTUS_MIN_SUBJECT_LEN = 1;
const size_t Base::CONTACTUS_MAX_SUBJECT_LEN = 64;  // standard is 78, we need that extra chracters
const size_t Base::CONTACTUS_MIN_BODY_LEN = 1;
const size_t Base::CONTACTUS_MAX_BODY_LEN = 1792;

const size_t Base::REGISTER_MIN_NAME_LEN = 1;
const size_t Base::REGISTER_MAX_NAME_LEN = 50;

const size_t Base::NEWS_MIN_TITLE_LEN = 1;
const size_t Base::NEWS_MAX_TITLE_LEN = 64;
const size_t Base::NEWS_MIN_BODY_LEN = 1;
const size_t Base::NEWS_MAX_BODY_LEN = 32768;

const string Base::FSDB_PATH = "../fsdb";
const string Base::FSDB_USERS_PATH = "../fsdb/users";
const string Base::RESUME_FILE_NAME = "resume.pdf";

const string Base::APP_SCR_1ST_FILE_NAME = "scr1st.jpg";
const string Base::APP_SCR_2ND_FILE_NAME = "scr2nd.jpg";
const string Base::APP_SCR_3RD_FILE_NAME = "scr3rd.jpg";
const string Base::APP_SCR_4TH_FILE_NAME = "scr4th.jpg";
const string Base::APP_BIN_FILE_NAME = "app.zip";

const size_t Base::FILE_SIZE_MAX_RESUME = 1048576;
const size_t Base::FILE_SIZE_MAX_APP_SCR = 262144;
const size_t Base::FILE_SIZE_MAX_APP_BIN = 5242880;

const size_t Base::DPI_APP_SCR = 72;

const size_t Base::RESOLUTION_MAX_X_APP_SCR = 1920;
const size_t Base::RESOLUTION_MAX_Y_APP_SCR = 1080;


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
*    = Constructors =
**********************************/

/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

wstring Base::Copyright(const CgiEnv::ELang &lang)
{
    CDate::Now n;

    switch (lang) {
    case CgiEnv::ELang_En:
    case CgiEnv::ELang_RootEn:
        return L"&copy; " + lexical_cast<wstring>(n.Year)
                + L" Samsung Electronics Co., Ltd.";
        break;
    case CgiEnv::ELang_Fa:
    case CgiEnv::ELang_RootFa:
        return L"&copy; " + CDate::DateConv::FormatToPersianNums(lexical_cast<string>(n.Year))
                + L" سامسونگ، تمامی حقوق محفوظ است.";
        break;
    default:
        return L"";
        break;
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

std::string Base::GetErrFromTemplate(const std::string &err)
{
    std::string result(M_ERR_TEMPLATE);
    
    boost::algorithm::replace_all(result, M_ERR_TEMPLATE_KEY, err);
    
    return result;
}

std::wstring Base::GetErrFromTemplate(const std::wstring &err)
{
    std::wstring result(M_ERR_TEMPLATE.begin(), M_ERR_TEMPLATE.end());
    
    boost::algorithm::replace_all(result, M_ERR_TEMPLATE_KEY, err);
    
    return result;
}

string Base::StrFromByte(const unsigned char *arr, size_t len)
{
    ostringstream oss;

    for (size_t i = 0; i < len; ++i) {
        oss << arr[i];
    }

    return oss.str();
}

wstring Base::StrFromByte(const wchar_t *arr, size_t len)
{
    wostringstream oss;

    for (size_t i = 0; i < len; ++i) {
        oss << (wchar_t) arr[i];
    }

    return oss.str();
}


/**********************************
*    = Debug Methods =
**********************************/


