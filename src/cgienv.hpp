#ifndef CGIENV_HPP
#define CGIENV_HPP


#include <string>
#include <unordered_map>

namespace Wt {
    class WEnvironment;
}

namespace SamsungDForumIr {
    class CgiEnv;
}

class SamsungDForumIr::CgiEnv
{
public:
    struct ClientInfoTag {
        std::string country_code;
        std::string country_code3;
        std::string country_name;
        std::string region;
        std::string city;
        std::string postal_code;
        std::string latitude;
        std::string longitude;
        std::string metro_code;
        std::string dma_code;
        std::string area_code;
        std::string charset;
        std::string continent_code;
        std::string country_conf;
        std::string region_conf;
        std::string city_conf;
        std::string postal_conf;
        std::string accuracy_radius;
    } ClientInfoRecord;

    enum ELang {
        ELang_None,
        ELang_Invalid,
        ELang_En,
        ELang_Fa,
        ELang_RootEn,
        ELang_RootFa
    };

    enum EClientInfo {
        EClientInfo_IP,
        EClientInfo_Browser,
        EClientInfo_Referer,
        EClientInfo_Location
    };

    enum EServerInfo {
        EServerInfo_Host,
        EServerInfo_URL,
        EServerInfo_NoReplyAddr
    };

    std::unordered_map<std::string, std::string> HttpGet;
    ELang CurrentLang;

    std::string LoggedInUserName;
    std::string LoggedInUserEmail;
    std::string LoggedInUserIP;
    std::string LoggedInUserLocation;
    std::string LoggedInUserGDate;
    std::string LoggedInUserJDate;
    std::string LoggedInUserTime;
    std::string LoggedInUserAgent;
    std::string LoggedInUserReferer;
    std::string LoggedInUserRawDate;

private:
    std::string m_clientInfoIP;
    std::string m_clientInfoBrowser;
    std::string m_clientInfoReferer;
    std::string m_clientInfoLocation;

    std::string m_serverInfoHost;
    std::string m_serverInfoURL;
    std::string m_serverInfoNoReplyAddr;

	bool m_foundXSS;
	bool m_isRootReq;
	bool m_isSiteMapReq;
    bool m_isVerifyReq;

    std::string m_initialURL;
    std::string m_newsId;
    std::string m_verifyReq;

public:
    CgiEnv(const Wt::WEnvironment &env);
    
    std::string GetClientInfo(const EClientInfo &key) const;
    std::string GetServerInfo(const EServerInfo &key) const;

	bool FoundXSS();
	bool IsRootReq();
	bool IsSiteMapReq();
    bool IsVerifyReq();
    std::string GetInitialURL();
    std::string GetNewsId();
    std::string GetVerifyReq();

private:
	void ExtractClientInfoDetail();
    
    inline static std::string CStrToStr(const char *cstr);
};


#endif /* CGIENV_HPP */


