#ifndef MAILER_HPP
#define MAILER_HPP


#include <unordered_map>
#include <string>

namespace CDate {
    class Now;
}

namespace SamsungDForumIr {
    class CgiEnv;
    class Mailer;
}

class SamsungDForumIr::Mailer
{
public:
    static void SendMailAsync(const std::string &from, const std::string &to,
                              const std::string &subject, const std::string &body);

    static void SendMail(const std::string &from, const std::string &to,
                         const std::string &subject, const std::string &body);

    static void SendAMessageToUsers(const std::unordered_map<std::string, std::string> &users,
                                    const std::string &subject, const std::string &body,
                                    const CDate::Now &now, const CgiEnv *cgiEnv);

    static void SendAMessageToUsersAsync(const std::unordered_map<std::string, std::string> &users,
                                         const std::string &subject, const std::string &body,
                                         const CDate::Now &now, const CgiEnv *cgiEnv);

    static void SendGreetingMessage(const std::string &to, const std::string &user,
                                    const CDate::Now &now, const CgiEnv *cgiEnv);

    static void SendLoginAlert(const std::string &to, const std::string &user,
                               const CDate::Now &now, const CgiEnv *cgiEnv);

    static void SendLoginRecovery(const std::string &to, const std::string &user,
                                  const std::string &pw, const CDate::Now &now, const CgiEnv *cgiEnv);

    static void SendUserMessage(const std::string &to, const std::string &from,
                                const std::string &user, const std::string &uri,
                                const std::string &subject, const std::string &body,
                                const CDate::Now &now, const CgiEnv *cgiEnv);

    static void SendVerificationMessage(const std::string &to, const std::string &user,
                                        const CDate::Now &now, const CgiEnv *cgiEnv);
};


#endif /* MAILER_HPP */


