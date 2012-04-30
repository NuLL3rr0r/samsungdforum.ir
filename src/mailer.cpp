#include <thread>
#include <cstdlib>
#include <ctime>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <vmime/vmime.hpp>
#include <vmime/platforms/posix/posixHandler.hpp>
#include "mailer.hpp"
#include "cdate.hpp"
#include "cgienv.hpp"
#include "crypto.hpp"
#include "log.hpp"

using namespace std;
using namespace boost;
using namespace CDate;
using namespace SamsungDForumIr;

void Mailer::SendMailAsync(const string &from, const string &to, const string &subject, const string &body)
{
    thread t(SendMail, from, to, subject, body);
    t.detach();
}

void Mailer::SendMail(const string &from, const string &to, const string &subject, const string &body)
{
    try {
        vmime::platform::setHandler<vmime::platforms::posix::posixHandler>();

        vmime::messageBuilder mb;

        mb.setExpeditor(vmime::mailbox(from));
        mb.getRecipients().appendAddress(vmime::create<vmime::mailbox>(to));

        mb.setSubject(*vmime::text::newFromString(subject, vmime::charsets::UTF_8));

        mb.constructTextPart(vmime::mediaType(vmime::mediaTypes::TEXT, vmime::mediaTypes::TEXT_HTML));
        mb.getTextPart()->setCharset(vmime::charsets::UTF_8);
        mb.getTextPart()->setText(vmime::create<vmime::stringContentHandler>(body));

        vmime::ref<vmime::message> msg = mb.construct();

        vmime::utility::url url("smtp://localhost");
        vmime::ref<vmime::net::session> sess = vmime::create<vmime::net::session>();
        vmime::ref<vmime::net::transport> tr = sess->getTransport(url);

        tr->connect();
        tr->send(msg);
        tr->disconnect();
    }

    catch (vmime::exception &e) {
        ERR(e.what());
    }

    catch (std::exception &e) {
        ERR(e.what());
    }
}

void Mailer::SendAMessageToUsers(const unordered_map<string, string> &users,
                                 const string &subject, const string &body,
                                 const CDate::Now &now, const CgiEnv *cgiEnv)
{
    string localizedMailSubject;
    string localizedMessage;

    switch(cgiEnv->CurrentLang) {
    case CgiEnv::ELang_RootEn:
        localizedMailSubject = "[" + cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host) + "] " + subject;
        localizedMessage = "<div style=\"direction: ltr; text-align: justify; "
                "font-family: Tahoma; font-size: 14px; line-height: 33px;\">"
                "<br /><br />"
                "Dear %1%,<br />"
                "You recieved this message from " + cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host) + "."
                "<br /><br /><br />"
                + body
                + "<br /><br />"
                "</div>";
        break;

    case CgiEnv::ELang_RootFa:
        localizedMailSubject = "[" + cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host) + "] " + subject;
        localizedMessage = "<div style=\"direction: rtl; text-align: justify; "
                "font-family: Tahoma; font-size: 14px; line-height: 33px;\">"
                "<br /><br />"
                "کاربر گرامی %1%،<br />"
                "پیام زیر از سوی " + cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host) + " برای شما ارسال شده است."
                "<br /><br /><br />"
                + body
                + "<br /><br />"
                "</div>";
        break;

    default:
        break;
    }

    for (unordered_map<string, string>::const_iterator it =
         users.begin(); it != users.end(); ++it) {
        // It has to be SendMail instead of SendMailAsync
        // if you want to run it in another thread
        // you have to run Mailer::SendAMessageToUsers in
        // another thread.
        SendMail(cgiEnv->GetServerInfo(CgiEnv::EServerInfo_NoReplyAddr), it->first,
                 localizedMailSubject,
                 (format(localizedMessage) % it->second).str());
    }
}

void Mailer::SendGreetingMessage(const string &to, const string &user, const Now &now,
                                 const CgiEnv *cgiEnv)
{
    string lang;
    string localizedMailSubject;
    string overallStyle;
    string localizedGreetings;

    switch(cgiEnv->CurrentLang) {
    case CgiEnv::ELang_En:
        lang = "en";
        localizedMailSubject = "Your account has been activated";
        overallStyle = "direction: ltr; text-align: justify; "
                "font-family: Tahoma; font-size: 14px; line-height: 33px;";
        localizedGreetings = "Welcome to Iranian Samsung developers community. "
                "Your account has been activated. You may now login at "
                "<a href=\"http://%1%/?lang=%2%\" style=\"color: #900; font-weight: normal;\">"
                "http://%1%/?lang=%2%</a>";
        break;

    case CgiEnv::ELang_Fa:
        lang = "fa";
        localizedMailSubject = "حساب کاربری شما فعال شد";
        overallStyle = "direction: rtl; text-align: justify; "
                "font-family: Tahoma; font-size: 14px; line-height: 33px;";
        localizedGreetings = "به جامعه توسعه دهندگان سامسونگ خوش آمدید. "
                "هم اکنون حساب کاربری شما فعال است. جهت ورود به حساب کاربری تان از آدرس "
                "<a href=\"http://%1%/?lang=%2%\" style=\"color: #900; font-weight: normal;\">"
                "http://%1%/?lang=%2%</a> استفاده نمائید.";
        break;

    default:
        break;
    }

    localizedGreetings = (format(localizedGreetings)
                          % cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host)
                          % lang).str();

    SendMailAsync(cgiEnv->GetServerInfo(CgiEnv::EServerInfo_NoReplyAddr), to,
                  "[" + cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host) + "] " + localizedMailSubject,
                  "<div style=\"" + overallStyle + "\">"
                  "<br /><br />"
                  + localizedGreetings
                  + "<br /><br />"
                  "</div>"
                  );
}

void Mailer::SendLoginAlert(const string &to, const string &user, const Now &now,
                            const CgiEnv *cgiEnv)
{
    SendMailAsync(cgiEnv->GetServerInfo(CgiEnv::EServerInfo_NoReplyAddr), to,
                  "[" + cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host) + "] ALERT: Access from " + user,
             "<div style=\"font-family: 'Bitstream Vera Sans', arial, helvetica, sans-serif; "
             "font-size: 11px; font-weight: bold; color: #203070; "
             "letter-spacing: 1px; line-height: 17px;\">"
             "<br /><h3>Access From " + user + "</h3>"
             "<blockquote>"
             "<table>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">IP</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_IP) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Location</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Location)
             + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">User Agent</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Browser) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Referer</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Referer) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Time</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + DateConv::ToJalali(now) + " * "
             + algorithm::trim_copy(DateConv::RawLocalDateTime(now))
             + "</span></td>"
             "</tr>"
             ""
             "</table>"
             "</blockquote>"
             "<br /></div>");
}

void Mailer::SendLoginRecovery(const string &to, const string &user, const string &pw, const Now &now,
                               const CgiEnv *cgiEnv)
{
    string localizedMailSubject;
    string overallStyle;
    string localizedPasswordRecovery;
    string localizedUser;
    string localizedPassword;

    switch(cgiEnv->CurrentLang) {
    case CgiEnv::ELang_En:
    case CgiEnv::ELang_RootEn:
        localizedMailSubject = "password recovery request from";
        overallStyle = "font-family: 'Bitstream Vera Sans', arial, helvetica, sans-serif; "
                "font-size: 11px; font-weight: bold; color: #203070; "
                "letter-spacing: 1px; line-height: 17px;";
        localizedPasswordRecovery = "Password Recovery";
        localizedUser = "User";
        localizedPassword = "Password";
        break;

    case CgiEnv::ELang_Fa:
    case CgiEnv::ELang_RootFa:
        localizedMailSubject = "درخواست بازیابی کلمه عبور از سوی";
        overallStyle = "direction: rtl; font-family:Tahoma;"
                "font-size: 11px; font-weight: bold; color: #203070; "
                "letter-spacing: 1px; line-height: 17px;";
        localizedPasswordRecovery = "بازیابی کلمه عبور";
        localizedUser = "نام کاربری";
        localizedPassword = "کلمه عبور";
        break;

    default:
        break;
    }


    SendMailAsync(cgiEnv->GetServerInfo(CgiEnv::EServerInfo_NoReplyAddr), to,
             "[" + cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host) + "] " + localizedMailSubject + " " + user,
             "<div style=\"" + overallStyle  + "\">"
             "<br /><h3>" + localizedPasswordRecovery + "</h3>"
             "<blockquote>"
             "<table>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">" + localizedUser + "</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" + user + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">" + localizedPassword + "</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">" + pw + "</span></td>"
             "</tr>"
             ""
             "</table>"
             "</blockquote></div>"
             ""
             "<div style=\"font-family: 'Bitstream Vera Sans', arial, helvetica, sans-serif; "
             "font-size: 11px; font-weight: bold; color: #203070; "
             "letter-spacing: 1px; line-height: 17px;\">"
             "<br /><h3>Requested By Client</h3>"
             "<blockquote>"
             "<table>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">IP</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_IP) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Location</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Location) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">User Agent</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Browser) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Referer</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Referer) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Time</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + DateConv::ToJalali(now) + " * "
             + algorithm::trim_copy(DateConv::RawLocalDateTime(now))
             + "</span></td>"
             "</tr>"
             ""
             "</table>"
             "</blockquote>"
             "<br /></div>");
}

void Mailer::SendUserMessage(const string &to, const string &from,
                             const string &user, const string &uri,
                             const string &subject, const string &body,
                             const Now &now, const CgiEnv *cgiEnv) {
    string localizedMailSubject;
    string overallStyle;
    string localizedDetail;
    string localizedName;
    string localizedEmail;
    string localizedURL;
    string localizedSubject;
    string localizedMessage;

    switch(cgiEnv->CurrentLang) {
    case CgiEnv::ELang_En:
        localizedMailSubject = "User Message";
        overallStyle = "position: relative; width: 89%; direction: ltr; text-align: justify; "
                "font-family: Tahoma; font-size: 14px; line-height: 33px;";
        localizedDetail = "Detail";
        localizedName = "Name";
        localizedEmail = "Email";
        localizedURL = "URL";
        localizedSubject = "Subject";
        localizedMessage = "Message";
        break;

    case CgiEnv::ELang_Fa:
        localizedMailSubject = "پیام کاربر";
        overallStyle = "position: relative; width: 89%; direction: rtl; text-align: justify; "
                "font-family: Tahoma; font-size: 14px; line-height: 33px;";
        localizedDetail = "جزئیات";
        localizedName = "نام";
        localizedEmail = "پست الکترونیکی";
        localizedURL = "وب سایت";
        localizedSubject = "موضوع";
        localizedMessage = "متن پیام";
        break;

    default:
        break;
    }

    SendMailAsync(from, to, "[" + cgiEnv->GetServerInfo(cgiEnv->EServerInfo_Host) + "] " + localizedMailSubject + ": " + subject,
             "<center><div style=\"" + overallStyle + "\">"
             "<p>"
             "<h6 style=\"color: #0000FF; font-size: 11px;\">"
             + localizedDetail +
             "</h6>"
             + localizedName + ":&nbsp;&nbsp;&nbsp;"
             + user +
             "<br />"
             + localizedEmail + ":&nbsp;&nbsp;&nbsp;"
             + from +
             "<br />"
             + localizedURL + ":&nbsp;&nbsp;&nbsp;"
             + (uri != "" ? uri : " - ") +
             "<br />"
             "</p>"
             "<br />"
             "<p>"
             "<h6 style=\"color: #0000FF; font-size: 11px;\">"
             + localizedSubject
             + "</h6>"
             + subject
             + "</p>"
             "<br />"
             "<p>"
             "<h6 style=\"color: #0000FF; font-size: 11px;\">"
             + localizedMessage
             + "</h6>"
             + body
             + "</p>"
             "<br />"
             "</div></center>"

             "<br /><br />"

             "<div style=\"font-family: 'Bitstream Vera Sans', arial, helvetica, sans-serif; "
             "font-size: 11px; font-weight: bold; color: #203070; "
             "letter-spacing: 1px; line-height: 17px;\">"
             "<br /><h3>Sent By Client</h3>"
             "<blockquote>"
             "<table>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">IP</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_IP) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Location</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Location) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">User Agent</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Browser) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Referer</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Referer) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Time</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + DateConv::ToJalali(now) + " * "
             + algorithm::trim_copy(DateConv::RawLocalDateTime(now))
             + "</span></td>"
             "</tr>"
             ""
             "</table>"
             "</blockquote>"
             "<br /></div>");
}

void Mailer::SendVerificationMessage(const string &to, const string &user, const Now &now,
                                     const CgiEnv *cgiEnv)
{
    string lang;
    string localizedMailSubject;
    string overallStyle;
    string localizedInstructions;

    switch(cgiEnv->CurrentLang) {
    case CgiEnv::ELang_En:
        lang = "en";
        localizedMailSubject = "Please confirm your registration";
        overallStyle = "direction: ltr; text-align: justify; "
                "font-family: Tahoma; font-size: 14px; line-height: 33px;";
        localizedInstructions = "Thank you for registering with " + cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host)
                + ". Please follow the link below to verify your email address:";
        break;

    case CgiEnv::ELang_Fa:
        lang = "fa";
        localizedMailSubject = "لطفا ثبت نام خود را تائید نمائید";
        overallStyle = "direction: rtl; text-align: justify; "
                "font-family: Tahoma; font-size: 14px; line-height: 33px;";
        localizedInstructions = "کاربر گرامی،"
                "<br />"
                "از ثبت نام شما در وب سایت " + cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host)
                + " متشکریم."
                " لطفا جهت تائید و تکمیل فرآیند ثبت نام، آدرس زیر را دنبال نمائید:";
        break;

    default:
        break;
    }

    string href((format("<a href=\"http://%1%/?verify=%2%&lang=%3%\" style=\"color: #900; font-weight: normal;\">"
                        "http://%1%/?verify=%2%&lang=%3%</a>")
                 % cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host) % Crypto::Encrypt(user) % lang).str());

    SendMailAsync(cgiEnv->GetServerInfo(CgiEnv::EServerInfo_NoReplyAddr), to,
             "[" + cgiEnv->GetServerInfo(CgiEnv::EServerInfo_Host) + "] "
             + localizedMailSubject,
             "<div style=\"" + overallStyle + "\">"
             "<br /><br />"
             + localizedInstructions
             + "<br />"
             + href
             + ""
             "</div>"
             "<br /><br />"
             "<div style=\"font-family: 'Bitstream Vera Sans', arial, helvetica, sans-serif; "
             "font-size: 11px; font-weight: bold; color: #203070; "
             "letter-spacing: 1px; line-height: 17px;\">"
             "<br /><h3>Requested By Client</h3>"
             "<blockquote>"
             "<table>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">IP</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_IP) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Location</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Location) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">User Agent</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Browser) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Referer</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + cgiEnv->GetClientInfo(CgiEnv::EClientInfo_Referer) + "</span></td>"
             "</tr>"
             ""
             "<tr valign=\"top\">"
             "<td width=\"85\">Time</td>"
             "<td width=\"15\">-</td>"
             "<td><span style=\"color: #900; font-weight: normal;\">"
             + DateConv::ToJalali(now) + " * "
             + algorithm::trim_copy(DateConv::RawLocalDateTime(now))
             + "</span></td>"
             "</tr>"
             ""
             "</table>"
             "</blockquote>"
             "<br /></div>");
}
