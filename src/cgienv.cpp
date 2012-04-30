/**********************************
*    = Header File Inclusion =
**********************************/

#include <map>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <Wt/WEnvironment>
#include <GeoIP.h>
#include <GeoIPCity.h>
#include "cgienv.hpp"
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
*    = Constructors =
**********************************/

CgiEnv::CgiEnv(const WEnvironment &env)
{
    m_clientInfoIP = env.getCgiValue("REMOTE_ADDR");
    m_clientInfoBrowser = env.getCgiValue("HTTP_USER_AGENT");
    m_clientInfoReferer = env.getCgiValue("HTTP_REFERER");

    ExtractClientInfoDetail();

    m_clientInfoLocation = ClientInfoRecord.city + "; "
                           + ClientInfoRecord.region + "; "
                           + ClientInfoRecord.country_code + "; "
                           + ClientInfoRecord.country_code3 + "; "
                           + ClientInfoRecord.country_name + "; "
                           + ClientInfoRecord.continent_code;

    m_serverInfoHost = env.getCgiValue("HTTP_HOST");
    m_serverInfoURL = "http://" + m_serverInfoHost + env.getCgiValue("REQUEST_URI");
    m_serverInfoNoReplyAddr = "no-reply@" + m_serverInfoHost;

	string queryStr = env.getCgiValue("QUERY_STRING");
	m_foundXSS = (queryStr.find("<") != string::npos ||
					queryStr.find(">") != string::npos ||
					queryStr.find("%3C") != string::npos ||
					queryStr.find("%3E") != string::npos ||
					queryStr.find("%3c") != string::npos ||
					queryStr.find("%3e") != string::npos)
				   ? true : false;

	m_isRootReq = false;
	m_isSiteMapReq = false;
    m_isVerifyReq = false;
    HttpGet["lang"] = "";

    Http::ParameterMap map = env.getParameterMap();
    for (std::map<string, Http::ParameterValues>::const_iterator it = map.begin(); it != map.end(); ++it) {
        if (it->first == "enroot") {
            m_isRootReq = true;
            CurrentLang = ELang_RootEn;
        } else if (it->first == "faroot") {
            m_isRootReq = true;
            CurrentLang = ELang_RootFa;
        } else if (it->first == "lang") {
            HttpGet["lang"] = env.getParameterValues(it->first)[0];
        } else if (it->first == "sitemap") {
			m_isSiteMapReq = true;
        } else if (it->first == "url") {
            m_initialURL = env.getParameterValues(it->first)[0];
        } else if (it->first == "news") {
            m_newsId = env.getParameterValues(it->first)[0];
        } else if (it->first == "verify") {
            m_isVerifyReq = true;
            m_verifyReq = env.getParameterValues(it->first)[0];
        }
    }

    if (!m_isRootReq) {
        if (HttpGet["lang"] == "") {
            CurrentLang = ELang_None;
        } else if (HttpGet["lang"] == "en") {
            CurrentLang = ELang_En;
        } else if (HttpGet["lang"] == "fa") {
            CurrentLang = ELang_Fa;
        } else {
            CurrentLang = ELang_Invalid;
        }
    }
}


/**********************************
*    = Destructor =
**********************************/

/**********************************
*    = Public Methods =
**********************************/

string CgiEnv::GetClientInfo(const EClientInfo &key) const
{
    switch(key) {
    
    case EClientInfo_IP:
        return m_clientInfoIP;
        
    case EClientInfo_Browser:
        return m_clientInfoBrowser;
        
    case EClientInfo_Referer:
        return m_clientInfoReferer;
        
    case EClientInfo_Location:
        return m_clientInfoLocation;
        
    default:
        return "";
    }
}

string CgiEnv::GetServerInfo(const EServerInfo &key) const
{
    switch(key) {
    
    case EServerInfo_Host:
        return m_serverInfoHost;
        
    case EServerInfo_URL:
        return m_serverInfoURL;
        
    case EServerInfo_NoReplyAddr:
        return m_serverInfoNoReplyAddr;
        
    default:
        return "";
    }
}

bool CgiEnv::FoundXSS()
{
	return m_foundXSS;
}

bool CgiEnv::IsRootReq()
{
	return m_isRootReq;
}

bool CgiEnv::IsSiteMapReq()
{
    return m_isSiteMapReq;
}

bool CgiEnv::IsVerifyReq()
{
    return m_isVerifyReq;
}

string CgiEnv::GetNewsId()
{
    return m_newsId;
}

string CgiEnv::GetInitialURL()
{
    return m_initialURL;
}

string CgiEnv::GetVerifyReq()
{
    return m_verifyReq;
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

void CgiEnv::ExtractClientInfoDetail()
{
    try {
        GeoIP *geoLiteCity;
        if (System::FileExists("/usr/local/share/GeoIP/GeoLiteCity.dat")) {
            geoLiteCity = GeoIP_open("/usr/local/share/GeoIP/GeoLiteCity.dat", GEOIP_STANDARD);
        } else if (System::FileExists("/usr/share/GeoIP/GeoLiteCity.dat")) {
            geoLiteCity = GeoIP_open("/usr/share/GeoIP/GeoLiteCity.dat", GEOIP_STANDARD);
        } else {
            return;
        }

        GeoIPRecordTag *record = GeoIP_record_by_name(geoLiteCity, m_clientInfoIP.c_str());

        if (record != NULL) {
            ClientInfoRecord.country_code = CStrToStr(record->country_code);
            ClientInfoRecord.country_code3 = CStrToStr(record->country_code3);
            ClientInfoRecord.country_name = CStrToStr(record->country_name);
            ClientInfoRecord.region = CStrToStr(record->region);
            ClientInfoRecord.city = CStrToStr(record->city);
            ClientInfoRecord.postal_code = CStrToStr(record->postal_code);
            ClientInfoRecord.latitude = lexical_cast<string>(record->latitude);
            ClientInfoRecord.longitude = lexical_cast<string>(record->longitude);
            ClientInfoRecord.metro_code = lexical_cast<string>(record->metro_code);
            ClientInfoRecord.dma_code = lexical_cast<string>(record->dma_code);
            ClientInfoRecord.area_code = lexical_cast<string>(record->area_code);
            ClientInfoRecord.charset = lexical_cast<string>(record->charset);
            ClientInfoRecord.continent_code = CStrToStr(record->continent_code);
            ClientInfoRecord.country_conf = lexical_cast<string>(record->country_conf);
            ClientInfoRecord.region_conf = lexical_cast<string>(record->region_conf);
            ClientInfoRecord.city_conf = lexical_cast<string>(record->city_conf);
            ClientInfoRecord.postal_conf = lexical_cast<string>(record->postal_conf);
            ClientInfoRecord.accuracy_radius = lexical_cast<string>(record->accuracy_radius);
        }
    }
    catch(...) {
    }
}


/**********************************
*    = Base Class Overrides =
**********************************/

/**********************************
*    = Utility Methods =
**********************************/

string CgiEnv::CStrToStr(const char *cstr)
{
    return cstr != NULL ? cstr : "";
}


/**********************************
*    = Debug Methods =
**********************************/


