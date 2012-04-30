#ifndef BASE_HPP
#define BASE_HPP


#include <string>
#include "cgienv.hpp"

namespace SamsungDForumIr {
    class Base;
}

class SamsungDForumIr::Base
{
public:
    static const unsigned char SKELETON_KEY_KEY[];
    static const unsigned char ROOT_USER_NAME_EN_KEY[];
    static const unsigned char ROOT_USER_PWD_EN_KEY[];
    static const unsigned char ROOT_USER_EMAIL_EN_KEY[];
    static const unsigned char ROOT_USER_NAME_FA_KEY[];
    static const unsigned char ROOT_USER_PWD_FA_KEY[];
    static const unsigned char ROOT_USER_EMAIL_FA_KEY[];

    static const std::string SKELETON_KEY;
    static const std::string ROOT_USER_NAME_EN;
    static const std::string ROOT_USER_PWD_EN;
    static const std::string ROOT_USER_EMAIL_EN;
    static const std::string ROOT_USER_NAME_FA;
    static const std::string ROOT_USER_PWD_FA;
    static const std::string ROOT_USER_EMAIL_FA;

    static const std::string ERR_ALICE;
    static const std::string ERR_CRITICAL;
    static const std::string ERR_UNKNOWN;

    static const std::string DATABASE_NAME;
    static const std::string DB_PAGE_ROOT;
    static const std::string DB_PAGE_ID_HOME;
    static const std::string DB_PAGE_FULLPATH_HOME;
    static const std::string URL_HOME;
    static const std::string URL_CONTACTUS;

    static const std::string REGEX_EMAIL;
    static const std::string REGEX_URI;
    static const std::string REGEX_PHONE_GENERIC;
    static const std::string REGEX_PHONE_MOBILE;
    static const std::string REGEX_PHONE_TEL;
    static const std::string REGEX_NATIONAL_CODE;

    static const std::size_t MIN_USERNAME_LENGTH;
    static const std::size_t MAX_USERNAME_LENGTH;
    static const std::size_t MIN_PASSWORD_LENGTH;
    static const std::size_t MAX_PASSWORD_LENGTH;

    static const std::size_t MIN_RECIPEENT_NAME_LEN;
    static const std::size_t MAX_RECIPEENT_NAME_LEN;

    static const std::size_t CONTACTUS_MIN_FROM_NAME_LEN;
    static const std::size_t CONTACTUS_MAX_FROM_NAME_LEN;
    static const std::size_t CONTACTUS_MIN_SUBJECT_LEN;
    static const std::size_t CONTACTUS_MAX_SUBJECT_LEN;
    static const std::size_t CONTACTUS_MIN_BODY_LEN;
    static const std::size_t CONTACTUS_MAX_BODY_LEN;

    static const std::size_t REGISTER_MIN_NAME_LEN;
    static const std::size_t REGISTER_MAX_NAME_LEN;

    static const std::size_t NEWS_MIN_TITLE_LEN;
    static const std::size_t NEWS_MAX_TITLE_LEN;
    static const std::size_t NEWS_MIN_BODY_LEN;
    static const std::size_t NEWS_MAX_BODY_LEN;

    static const std::string FSDB_PATH;
    static const std::string FSDB_USERS_PATH;

    static const std::string RESUME_FILE_NAME;

    static const std::string APP_SCR_1ST_FILE_NAME;
    static const std::string APP_SCR_2ND_FILE_NAME;
    static const std::string APP_SCR_3RD_FILE_NAME;
    static const std::string APP_SCR_4TH_FILE_NAME;
    static const std::string APP_BIN_FILE_NAME;

    static const std::size_t FILE_SIZE_MAX_RESUME;
    static const std::size_t FILE_SIZE_MAX_APP_SCR;
    static const std::size_t FILE_SIZE_MAX_APP_BIN;

    static const std::size_t DPI_APP_SCR;
    static const std::size_t RESOLUTION_MAX_X_APP_SCR;
    static const std::size_t RESOLUTION_MAX_Y_APP_SCR;

private:
    static const std::string M_ERR_TEMPLATE;
    static const std::string M_ERR_TEMPLATE_KEY;

public:
    static std::wstring Copyright(const CgiEnv::ELang &lang);

private:
    static std::string GetErrFromTemplate(const std::string &err);
    static std::wstring GetErrFromTemplate(const std::wstring &err);
    static std::string StrFromByte(const unsigned char *arr, size_t len);
    static std::wstring StrFromByte(const wchar_t *arr, size_t len);
};


#endif /* BASE_HPP */


