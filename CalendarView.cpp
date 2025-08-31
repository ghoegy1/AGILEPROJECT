/***************************************************************************************
 * CalendarView.cpp — implementation
 ****************************************************************************************/
#include "CalendarView.hpp"
#include "Profile.hpp"

namespace sb {

std::vector<StudySession> CalendarView::list(const Profile& user, const SessionRequests& sr) {
    return sr.confirmedFor(user);
}

std::string CalendarView::pretty(const StudySession& s, const std::string& selfEmail) {
    const char* kDayNames[7] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
    std::string times = formatHHMM(s.start) + "-" + formatHHMM(s.end);
    std::string partner = (s.requester == selfEmail) ? s.invitee : s.requester;
    return std::string(kDayNames[static_cast<int>(s.day)]) + " " + times +
           " " + s.course + " with " + partner + " [" + s.id + "]";
}

bool CalendarView::cancel(const std::string& sessionId, const Profile& by, SessionRequests& sr) {
    return sr.cancelConfirmed(sessionId, by);
}

} // namespace sb