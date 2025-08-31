/***************************************************************************************
 * test_calendar_view.cpp
 * Tests for viewing and managing confirmed sessions (calendar-style).
 *
 * STANDARD LIBRARIES USED:
 *  <cassert>, <iostream>, <vector>, <string>
 ****************************************************************************************/
#include <cassert>
#include <iostream>
#include "CalendarView.hpp"
#include "CourseManager.hpp"
#include "Profile.hpp"

using namespace sb;

static Profile makeProfile(const std::string& name, const std::string& email,
                           const std::vector<std::string>& courses) {
    Profile p;
    auto norm = CourseManager::normalizeDedup(courses);
    p.createOrReset(name, email, norm);
    return p;
}

int main() {
    NotificationCenter nc;
    SessionRequests sr(&nc);

    Profile me = makeProfile("Me", "me@clemson.edu", {"CPSC 2150"});
    Profile al = makeProfile("Alice", "alice@clemson.edu", {"CPSC 2150"});
    Profile bo = makeProfile("Bob", "bob@clemson.edu", {"CPSC 2150"});

    // Create and confirm two sessions on different days/times
    const auto& s1 = sr.sendRequest(me, al, "CPSC 2150", Day::Tue, 14*60, 15*60);
    const auto& s2 = sr.sendRequest(bo, me, "CPSC 2150", Day::Mon, 10*60, 11*60);

    sr.confirmRequest(s1.id, al);  // Alice confirms session with Me
    sr.confirmRequest(s2.id, me);  // Me confirms session with Bob

    {
        // Test 1: list returns confirmed, sorted by day then start (Mon 10:00, Tue 14:00)
        auto list = CalendarView::list(me, sr);
        assert(list.size() == 2);
        assert(static_cast<int>(list[0].day) <= static_cast<int>(list[1].day));
        assert(list[0].day == Day::Mon && list[0].start == 10*60);
        assert(list[1].day == Day::Tue && list[1].start == 14*60);
    }

    {
        // Test 2: pretty formatting includes day, time range, course, partner, id
        auto list = CalendarView::list(me, sr);
        auto line = CalendarView::pretty(list[0], "me@clemson.edu");
        // basic checks
        assert(line.find("Mon") != std::string::npos);
        assert(line.find("10:00-11:00") != std::string::npos);
        assert(line.find("CPSC 2150") != std::string::npos);
        assert(line.find("[S") != std::string::npos);
    }

    {
        // Test 3: cancel removes session and notifies the partner
        auto list = CalendarView::list(me, sr);
        bool ok = CalendarView::cancel(list[0].id, me, sr);
        assert(ok);
        auto after = CalendarView::list(me, sr);
        assert(after.size() == 1); // one removed
    }

    std::cout << "[test_calendar_view] All tests passed.\n";
    return 0;
}