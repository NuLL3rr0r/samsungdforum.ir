#ifndef APPOINTMENT_HPP
#define APPOINTMENT_HPP


namespace SamsungDForumIr {
    class Appointment;
}

#include "basewidget.hpp"

class SamsungDForumIr::Appointment : public SamsungDForumIr::BaseWidget
{
private:
    Wt::WDialog *m_dlgParent;
    Wt::WMessageBox *m_msg;

    Wt::WText *m_selectedDate;
    Wt::WPushButton *m_submitButton;

    Wt::WComboBox *m_dateYYComboBox;
    Wt::WComboBox *m_dateMMComboBox;
    Wt::WComboBox *m_dateDDComboBox;
    Wt::WComboBox *m_timeHHComboBox;

    bool m_editMode;
    size_t m_year;
    size_t m_month;
    size_t m_day;
    size_t m_hour;

    Wt::JSignal<int> m_jsNextMonthClicked;
    Wt::JSignal<int> m_jsPreviousMonthClicked;
    Wt::JSignal<int> m_jsADaySelected;
    Wt::JSignal<int> m_jsAnHourSelected;


public:
    Appointment(CgiRoot *cgi, Wt::WDialog *parentDialog);

public:
    Wt::JSignal<int> &JSNextMonthClicked();
    Wt::JSignal<int> &JSPreviousMonthClicked();
    Wt::JSignal<int> &JSADaySelected();
    Wt::JSignal<int> &JSAnHourSelected();

private:
    void OnNextMonthClickedTriggered(int index);
    void OnPreviousMonthClickedTriggered(int index);
    void OnADaySelectedTriggered(int index);
    void OnAnHourSelectedTriggered(int index);

    void OnDateYYComboBoxChanged(int index);
    void OnDateMMComboBoxChanged(int index);
    void OnDateDDComboBoxChanged(int index);
    void OnTimeHHComboBoxChanged(int index);

    void OnSubmitButtonPressed();
    void OnReturnButtonPressed();
    void OnSubmitFailedOK();
    void OnSubmitSuccessOK();

private:
    std::string GetReservedDays(size_t month);
    std::string GetReservedHours(size_t day);

    void ClearSelectedDate();
    void UpdateSelectedDate();

    Wt::WWidget *Layout();
};


#endif /* APPOINTMENT_HPP */


