/**********************************
*    = Header File Inclusion =
**********************************/

#include <unordered_map>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <Wt/WApplication>
#include <Wt/WComboBox>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WGridLayout>
#include <Wt/WMessageBox>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WVBoxLayout>
#include <Wt/WWidget>
#include "appointment.hpp"
#include "cdate.hpp"
#include "crypto.hpp"
#include "db.hpp"


/**********************************
*    = PreProcessor Directives =
**********************************/

/**********************************
*    = Importing NameSpaces =
**********************************/

using namespace std;
using namespace boost;
using namespace cppdb;
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
*    = Constructos =
**********************************/

Appointment::Appointment(CgiRoot *cgi, Wt::WDialog *parentDialog) : BaseWidget(cgi),
    m_dlgParent(parentDialog),
    m_editMode(false),
    m_jsNextMonthClicked(this, "JSNextMonthClicked"),
    m_jsPreviousMonthClicked(this, "JSPreviousMonthClicked"),
    m_jsADaySelected(this, "JSADaySelected"),
    m_jsAnHourSelected(this, "JSAnHourSelected")
{
    this->clear();
    this->setId("AppointmentWidget");
    this->addWidget(Layout());

    m_jsNextMonthClicked.connect(this, &Appointment::OnNextMonthClickedTriggered);
    m_jsPreviousMonthClicked.connect(this, &Appointment::OnPreviousMonthClickedTriggered);
    m_jsADaySelected.connect(this, &Appointment::OnADaySelectedTriggered);
    m_jsAnHourSelected.connect(this, &Appointment::OnAnHourSelectedTriggered);

    m_dlgParent->contents()->addWidget(this);
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

JSignal<int> &Appointment::JSNextMonthClicked()
{
    return m_jsNextMonthClicked;
}

JSignal<int> &Appointment::JSPreviousMonthClicked()
{
    return m_jsPreviousMonthClicked;
}

JSignal<int> &Appointment::JSADaySelected()
{
    return m_jsADaySelected;
}

JSignal<int> &Appointment::JSAnHourSelected()
{
    return m_jsAnHourSelected;
}

void Appointment::OnNextMonthClickedTriggered(int index)
{
    m_month = index;
    doJavaScript("preperTaghvim(" + lexical_cast<string>(index) + ", '" + GetReservedDays(index) + "');");
}

void Appointment::OnPreviousMonthClickedTriggered(int index)
{
    m_month = index;
    doJavaScript("preperTaghvim(" + lexical_cast<string>(index) + ", '" + GetReservedDays(index) + "');");
}

void Appointment::OnADaySelectedTriggered(int index)
{
    ClearSelectedDate();
    m_day = index;
    doJavaScript("preperHour('" + GetReservedHours(index) + "');");
}

void Appointment::OnAnHourSelectedTriggered(int index)
{
    m_hour = index;

    UpdateSelectedDate();

    if (m_submitButton->isDisabled()) {
        m_submitButton->enable();
    }
}

void Appointment::OnDateYYComboBoxChanged(int index)
{
    if (index != 0) {
        m_dateYYComboBox->removeStyleClass("Wt-invalid", true);
    } else {
        m_dateYYComboBox->setStyleClass("Wt-invalid");
    }
}

void Appointment::OnDateMMComboBoxChanged(int index)
{
    if (index != 0) {
        m_dateMMComboBox->removeStyleClass("Wt-invalid", true);
    } else {
        m_dateMMComboBox->setStyleClass("Wt-invalid");
    }
}

void Appointment::OnDateDDComboBoxChanged(int index)
{
    if (index != 0) {
        m_dateDDComboBox->removeStyleClass("Wt-invalid", true);
    } else {
        m_dateDDComboBox->setStyleClass("Wt-invalid");
    }
}

void Appointment::OnTimeHHComboBoxChanged(int index)
{
    if (index != 0) {
        m_timeHHComboBox->removeStyleClass("Wt-invalid", true);
    } else {
        m_timeHHComboBox->setStyleClass("Wt-invalid");
    }
}

void Appointment::OnSubmitButtonPressed()
{
    /*if (m_dateYYComboBox->currentIndex() == 0) {
        m_dateYYComboBox->setFocus();
        return;
    }
    if (m_dateMMComboBox->currentIndex() == 0) {
        m_dateMMComboBox->setFocus();
        return;
    }
    if (m_dateDDComboBox->currentIndex() == 0) {
        m_dateDDComboBox->setFocus();
        return;
    }
    if (m_timeHHComboBox->currentIndex() == 0) {
        m_timeHHComboBox->setFocus();
        return;
    }

    result r = m_db->Sql() << "SELECT rowid FROM ["
                              + m_dbTables->Table("APPOINTMENTS")
                              + "] WHERE email!=? AND year=? AND month=? AND day=? AND hour=?;"
                           << m_cgiEnv->LoggedInUserName
                           << m_year
                           << m_dateMMComboBox->currentIndex()
                           << m_dateDDComboBox->currentIndex()
                           << m_timeHHComboBox->currentIndex() - 1
                           << row;
    if (!r.empty()) {
        m_msg = new WMessageBox(L"عدم ثبت تاریخ",
                                L"متاسفانه زمان مورد نظر قبلا توسط کاربر دیگری ثبت شده است.", Warning, NoButton);
        m_msg->addButton(L"تائید", Ok);
        m_msg->buttonClicked().connect(this, &Appointment::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!m_editMode) {
        m_db->Insert(m_dbTables->Table("APPOINTMENTS"), "email, year, month, day, hour",
                     5, m_cgiEnv->LoggedInUserName.c_str(),
                     lexical_cast<string>(m_year).c_str(),
                     lexical_cast<string>(m_dateMMComboBox->currentIndex()).c_str(),
                     lexical_cast<string>(m_dateDDComboBox->currentIndex()).c_str(),
                     lexical_cast<string>(m_timeHHComboBox->currentIndex() - 1).c_str());
    } else {
        m_db->Update(m_dbTables->Table("APPOINTMENTS"), "email", m_cgiEnv->LoggedInUserName,
                     "year=?, month=?, day=?, hour=?", 4,
                     lexical_cast<string>(m_year).c_str(),
                     lexical_cast<string>(m_dateMMComboBox->currentIndex()).c_str(),
                     lexical_cast<string>(m_dateDDComboBox->currentIndex()).c_str(),
                     lexical_cast<string>(m_timeHHComboBox->currentIndex() - 1).c_str());
    }

    m_msg = new WMessageBox(L"ثبت شد",
                            L"با تشکر، هماهنگی لازم با شما انجام خواهد شد.", Warning, NoButton);
    m_msg->addButton(L"تائید", Ok);
    m_msg->buttonClicked().connect(this, &Appointment::OnSubmitSuccessOK);
    m_msg->show();*/

    result r = m_db->Sql() << "SELECT rowid FROM ["
                              + m_dbTables->Table("APPOINTMENTS")
                              + "] WHERE email!=? AND year=? AND month=? AND day=? AND hour=?;"
                           << m_cgiEnv->LoggedInUserName
                           << m_year
                           << m_month
                           << m_day
                           << m_hour
                           << row;
    if (!r.empty()) {
        m_msg = new WMessageBox(L"عدم ثبت تاریخ",
                                L"متاسفانه زمان مورد نظر قبلا توسط کاربر دیگری ثبت شده است.", Warning, NoButton);
        m_msg->addButton(L"تائید", Ok);
        m_msg->buttonClicked().connect(this, &Appointment::OnSubmitFailedOK);
        m_msg->show();

        return;
    }

    if (!m_editMode) {
        m_db->Insert(m_dbTables->Table("APPOINTMENTS"), "email, year, month, day, hour",
                     5, m_cgiEnv->LoggedInUserName.c_str(),
                     lexical_cast<string>(m_year).c_str(),
                     lexical_cast<string>(m_month).c_str(),
                     lexical_cast<string>(m_day).c_str(),
                     lexical_cast<string>(m_hour).c_str());
    } else {
        m_db->Update(m_dbTables->Table("APPOINTMENTS"), "email", m_cgiEnv->LoggedInUserName,
                     "year=?, month=?, day=?, hour=?", 4,
                     lexical_cast<string>(m_year).c_str(),
                     lexical_cast<string>(m_month).c_str(),
                     lexical_cast<string>(m_day).c_str(),
                     lexical_cast<string>(m_hour).c_str());
    }

    m_msg = new WMessageBox(L"ثبت شد",
                            L"با تشکر، هماهنگی لازم با شما انجام خواهد شد.", Warning, NoButton);
    m_msg->addButton(L"تائید", Ok);
    m_msg->buttonClicked().connect(this, &Appointment::OnSubmitSuccessOK);
    m_msg->show();
}

void Appointment::OnReturnButtonPressed()
{
    m_dlgParent->hide();
}

void Appointment::OnSubmitFailedOK()
{
    delete m_msg;
    m_msg = NULL;
}

void Appointment::OnSubmitSuccessOK()
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

string Appointment::GetReservedDays(size_t month)
{
    string json("{\"Months\":[{\"ReservedDays\":[");

    size_t minHour = 8;
    size_t maxHour = 19;

    bool found = false;
    for (size_t d = 1; d <= 31; ++d) {
        unordered_map<size_t, bool> reservedHours;
        for (size_t i = minHour; i <= maxHour; ++i) {
            reservedHours[i] = false;
        }

        result r = m_db->Sql() << "SELECT hour "
                                  "FROM [" + m_dbTables->Table("APPOINTMENTS")
                                  + "] WHERE year=? AND month=? AND day=?;" << m_year << month << d;
        while (r.next()) {
            size_t hour;
            r >> hour;

            if (hour < minHour || hour > maxHour) {
                continue;
            }

            reservedHours[hour] = true;
        }

        bool full = true;
        for (size_t i = minHour; i <= maxHour; ++i) {
            if (!reservedHours[i]) {
                full = false;
                break;
            }
        }

        if (full) {
            if (found)
                json += ", ";

            if (!found)
                found = true;

            json += (format("{\"Day%1%\": %1%}") % d).str();
        }
    }

    json += "]}]}";

    return json;
}

string Appointment::GetReservedHours(size_t day)
{
    string json("{\"Days\":[{\"ReservedHours\":[");

    result r = m_db->Sql() << "SELECT hour "
                              "FROM [" + m_dbTables->Table("APPOINTMENTS")
                              + "] WHERE year=? AND month=? AND day=?;" << m_year << m_month << day;
    bool found = false;
    while (r.next()) {
        if (found)
            json += ", ";

        if (!found)
            found = true;

        size_t h;
        r >> h;

        json += (format("{\"Hour%1%\": %1%}") % h).str();
    }

    json += "]}]}";

    return json;
}

void Appointment::ClearSelectedDate()
{
    m_submitButton->disable();
    m_selectedDate->setText(L"");
}

void Appointment::UpdateSelectedDate()
{
    wstring yearStr = boost::lexical_cast<wstring>(m_year);
    wstring monthStr = boost::lexical_cast<wstring>(m_month);
    wstring dayStr = boost::lexical_cast<wstring>(m_day);
    wstring hourStr = boost::lexical_cast<wstring>(m_hour);

    if (m_month < 10)
        monthStr = L"0" + monthStr;
    if (m_day < 10)
        dayStr = L"0" + dayStr;
    if (m_hour < 10)
        hourStr = L"0" + hourStr;

    m_selectedDate->setText((wformat(L"زمان انتخابی شما: %1%/%2%/%3% - %4%:00")
                             % yearStr
                             % monthStr
                             % dayStr
                             % hourStr).str());
}


/**********************************
*    = Base Class Overrides =
**********************************/

WWidget *Appointment::Layout()
{
    Div *root = new Div("Appointment");

    /*
    m_dateYYComboBox = new WComboBox();
    m_dateMMComboBox = new WComboBox();
    m_dateDDComboBox = new WComboBox();
    m_timeHHComboBox = new WComboBox();

    m_dateYYComboBox->addItem(m_lang->GetString("HOME_APPOINTMENT_DATE_YY_TEXT"));
    m_dateMMComboBox->addItem(m_lang->GetString("HOME_APPOINTMENT_DATE_MM_TEXT"));
    m_dateDDComboBox->addItem(m_lang->GetString("HOME_APPOINTMENT_DATE_DD_TEXT"));
    m_timeHHComboBox->addItem(m_lang->GetString("HOME_APPOINTMENT_TIME_HH_TEXT"));

    string date = CDate::DateConv::ToJalali();
    size_t year = lexical_cast<size_t>(date.substr(0, 4));
    m_year = year;
    size_t month = lexical_cast<size_t>(date.substr(5, 2));
    size_t day = lexical_cast<size_t>(date.substr(8, 2));
    size_t hour = 0;

    m_dateYYComboBox->addItem(CDate::DateConv::FormatToPersianNums(lexical_cast<wstring>(year)));

    m_dateMMComboBox->addItem(L"فروردین");
    m_dateMMComboBox->addItem(L"اردیبهشت");
    m_dateMMComboBox->addItem(L"خرداد");
    m_dateMMComboBox->addItem(L"تیر");
    m_dateMMComboBox->addItem(L"مرداد");
    m_dateMMComboBox->addItem(L"شهریور");
    m_dateMMComboBox->addItem(L"مهر");

    int yIndex = m_dateYYComboBox->findText(CDate::DateConv::FormatToPersianNums(lexical_cast<wstring>(year)));
    if (yIndex > -1)
        m_dateYYComboBox->setCurrentIndex(yIndex);
    else
        m_dateYYComboBox->setCurrentIndex(1);

    m_dateMMComboBox->setCurrentIndex(month);
    m_dateDDComboBox->setCurrentIndex(day);
    m_timeHHComboBox->setCurrentIndex(hour + 1);

    m_dateMMComboBox->addItem(L"آبان");
    m_dateMMComboBox->addItem(L"آذر");
    m_dateMMComboBox->addItem(L"دی");
    m_dateMMComboBox->addItem(L"بهمن");
    m_dateMMComboBox->addItem(L"اسفند");

    for (size_t i = 1; i < 32; ++i) {
        wstring d(lexical_cast<wstring>(i));
        if (i < 10)
            d = L"0" + d;
        m_dateDDComboBox->addItem(CDate::DateConv::FormatToPersianNums(d));
    }

    for (size_t i = 0; i < 24; ++i) {
        wstring h(lexical_cast<wstring>(i));
        if (i < 10)
            h = L"0" + h;
        m_timeHHComboBox->addItem(CDate::DateConv::FormatToPersianNums(h));
    }

    //// YOU SHOULD QUERY HERE !!
    result r = m_db->Sql() << "SELECT year, month, day, hour "
                              "FROM [" + m_dbTables->Table("APPOINTMENTS")
                              + "] WHERE email=?;" << m_cgiEnv->LoggedInUserName << row;
    if (!r.empty()) {
        m_editMode = true;

        r >> year >> month >> day >> hour;
    }

    if (!m_editMode) {
        m_dateYYComboBox->setCurrentIndex(1);
        m_dateMMComboBox->setCurrentIndex(month);
        m_dateDDComboBox->setCurrentIndex(day);
        m_timeHHComboBox->setCurrentIndex(10);
    } else {
        int yIndex = m_dateYYComboBox->findText(CDate::DateConv::FormatToPersianNums(lexical_cast<wstring>(year)));
        if (yIndex > -1)
            m_dateYYComboBox->setCurrentIndex(yIndex);
        else
            m_dateYYComboBox->setCurrentIndex(1);

        m_dateMMComboBox->setCurrentIndex(month);
        m_dateDDComboBox->setCurrentIndex(day);
        m_timeHHComboBox->setCurrentIndex(hour + 1);
    }


    Div *dvForm = new Div(root);
    dvForm->setStyleClass("form");
    WGridLayout *dvFormLayout = new WGridLayout();

    Div *dvDate = new Div();
    dvDate->addWidget(m_dateYYComboBox);
    dvDate->addWidget(new WText("&nbsp;&nbsp;&nbsp;&nbsp;"));
    dvDate->addWidget(m_dateMMComboBox);
    dvDate->addWidget(new WText("&nbsp;&nbsp;&nbsp;&nbsp;"));
    dvDate->addWidget(m_dateDDComboBox);

    Div *dvTime = new Div();
    dvTime->addWidget(m_timeHHComboBox);

    dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_APPOINTMENT_DATE_TEXT")),
                                      0, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(dvDate, 0, 1);

    dvFormLayout->addWidget(new WText(m_lang->GetString("HOME_APPOINTMENT_TIME_TEXT")),
                                      1, 0, AlignLeft | AlignMiddle);
    dvFormLayout->addWidget(dvTime, 1, 1);

    dvFormLayout->setColumnStretch(0, 100);
    dvFormLayout->setColumnStretch(1, 300);
    dvFormLayout->setVerticalSpacing(11);
    dvForm->resize(400, WLength::Auto);
    dvForm->setLayout(dvFormLayout);

    Div *dvButtons = new Div(root, "dvDialogButtons");
    WPushButton *submitButton = new WPushButton(m_lang->GetString("HOME_APPOINTMENT_DLG_OK_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("HOME_APPOINTMENT_DLG_RETURN_BUTTON"), dvButtons);

    submitButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    OnDateYYComboBoxChanged(m_dateYYComboBox->currentIndex());
    OnDateMMComboBoxChanged(m_dateMMComboBox->currentIndex());
    OnDateDDComboBoxChanged(m_dateDDComboBox->currentIndex());
    OnTimeHHComboBoxChanged(m_timeHHComboBox->currentIndex());

    m_dateYYComboBox->activated().connect(this, &Appointment::OnDateYYComboBoxChanged);
    m_dateMMComboBox->activated().connect(this, &Appointment::OnDateMMComboBoxChanged);
    m_dateDDComboBox->activated().connect(this, &Appointment::OnDateDDComboBoxChanged);
    m_timeHHComboBox->activated().connect(this, &Appointment::OnTimeHHComboBoxChanged);

    submitButton->clicked().connect(this, &Appointment::OnSubmitButtonPressed);
    returnButton->clicked().connect(this, &Appointment::OnReturnButtonPressed);
    */

    new Div(root, "taghvimScript");
    new Div(root, "showHour");
    new Div(root, "numMo");
    new Div(root, "selectedDay");
    new Div(root, "selectedHour");

    size_t year;
    size_t month;
    size_t day;
    size_t hour;

    result r = m_db->Sql() << "SELECT year, month, day, hour "
                              "FROM [" + m_dbTables->Table("APPOINTMENTS")
                              + "] WHERE email=?;" << m_cgiEnv->LoggedInUserName << row;
    if (!r.empty()) {
        m_editMode = true;

        r >> year >> month >> day >> hour;
    }

    if (!m_editMode) {
        string date = CDate::DateConv::ToJalali();
        m_year = lexical_cast<size_t>(date.substr(0, 4));
        m_month = lexical_cast<size_t>(date.substr(5, 2));
        m_day = lexical_cast<size_t>(date.substr(8, 2));
        m_hour = 0;
    } else {
        m_year = year;
        m_month = month;
        m_day = day;
        m_hour = hour;
    }

    doJavaScript("preperTaghvim(" + lexical_cast<string>(m_month) + ", '" + GetReservedDays(m_month) + "');");


    Div *dvSelectedDate = new Div(root, "dvSelectedDate");
    m_selectedDate = new WText(dvSelectedDate);

    if (m_editMode)
        UpdateSelectedDate();

    Div *dvButtons = new Div(root, "dvDialogButtons");
    m_submitButton = new WPushButton(m_lang->GetString("HOME_APPOINTMENT_DLG_OK_BUTTON"), dvButtons);
    WPushButton *returnButton = new WPushButton(m_lang->GetString("HOME_APPOINTMENT_DLG_RETURN_BUTTON"), dvButtons);

    m_submitButton->setStyleClass("dialogButton");
    returnButton->setStyleClass("dialogButton");

    m_submitButton->disable();

    m_submitButton->clicked().connect(this, &Appointment::OnSubmitButtonPressed);
    returnButton->clicked().connect(this, &Appointment::OnReturnButtonPressed);


    return root;
}


/**********************************
*    = Utility Methods =
**********************************/

/**********************************
*    = Debug Methods =
**********************************/


