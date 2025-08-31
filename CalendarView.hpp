/***************************************************************************************
 * CalendarView.hpp
 * Feature: View/manage upcoming confirmed sessions (calendar-style list).
 *
 * STANDARD LIBRARIES USED:
 *  <vector>    : listing sessions
 *  <string>    : pretty labels
 ****************************************************************************************/
#pragma once
#include "SessionRequests.hpp"
#include "Utils.hpp"
#include <vector>
#include <string>

namespace sb {

class CalendarView {
public:
    // Return confirmed sessions for 'user' (already sorted by day/time by SessionRequests).
    static std::vector<StudySession> list(const Profile& user, const SessionRequests& sr);

    // Pretty format a single session, e.g., "Mon 10:00-11:00 CPSC 2150 with alice@..."
    static std::string pretty(const StudySession& s, const std::string& selfEmail);

    // Cancel a confirmed session by id (delegates to SessionRequests).
    static bool cancel(const std::string& sessionId, const Profile& by, SessionRequests& sr);
};

} // namespace sb