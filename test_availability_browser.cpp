/***************************************************************************************
 * test_availability_browser.cpp
 * Tests for browsing classmates’ availability by course (and optional day).
 *
 * STANDARD LIBRARIES USED:
 *  <cassert>  : assertions
 *  <iostream> : summary output
 *  <vector>   : classmate set
 ****************************************************************************************/
#include <cassert>
#include <iostream>
#include <vector>
#include "Profile.hpp"
#include "CourseManager.hpp"
#include "AvailabilityManager.hpp"
#include "AvailabilityBrowser.hpp"

using namespace sb;

static Profile makeProfile(const std::string& name,
                           const std::string& email,
                           const std::vector<std::string>& courses,
                           std::initializer_list<AvailabilitySlot> slots) {
    Profile p;
    auto norm = CourseManager::normalizeDedup(courses);
    p.createOrReset(name, email, norm);
    auto& v = p.availabilityMutable();
    v.assign(slots.begin(), slots.end());
    return p;
}

int main() {
    // Create three users
    Profile me   = makeProfile("Me", "me@clemson.edu", {"CPSC 2150"},
                               { {Day::Mon, 10*60, 12*60} });

    Profile a    = makeProfile("Alice", "alice@clemson.edu", {"CPSC 2150", "MATH 1080"},
                               { {Day::Mon, 11*60, 13*60}, {Day::Tue, 9*60, 10*60} });

    Profile b    = makeProfile("Bob", "bob@clemson.edu", {"ENGL 1030"},
                               { {Day::Mon, 15*60, 16*60} });

    Profile c    = makeProfile("Cara", "cara@clemson.edu", {"CPSC 2150"},
                               { {Day::Wed, 14*60, 16*60} });

    std::vector<Profile> all = {me, a, b, c};

    {
        // Test 1: browseByCourse returns only classmates in course (excluding self)
        auto list = AvailabilityBrowser::browseByCourse(all, me, "cpsc 2150");
        // Should include Alice and Cara (2 entries), not Me or Bob
        assert(list.size() == 2);

        // Names order not guaranteed; verify presence by counts of total slots
        int totalSlots = 0;
        for (auto& pr : list) totalSlots += static_cast<int>(pr.second.size());
        // Alice has 2 slots, Cara has 1 slot -> total 3
        assert(totalSlots == 3);
    }

    {
        // Test 2: browsing a course with no classmates returns empty
        auto list = AvailabilityBrowser::browseByCourse(all, me, "MATH 1080");
        // Only Alice has MATH 1080, but Me is not in it—still should list Alice (since requirement is classmates’ availability by course)
        // However, the story says "browse classmates’ availability" so this is correct: Me can browse any course.
        assert(list.size() == 1);
        // Now check one that nobody has:
        auto none = AvailabilityBrowser::browseByCourse(all, me, "HIST 1010");
        assert(none.empty());
    }

    {
        // Test 3: browseByCourseAndDay filters to a particular day
        auto mon = AvailabilityBrowser::browseByCourseAndDay(all, me, "CPSC 2150", Day::Mon);
        // Alice has Mon slot; Cara does not (only Wed)
        assert(mon.size() == 1);
        assert(mon[0].second.size() == 1);
        assert(mon[0].second[0].start == 11*60);
    }

    std::cout << "[test_availability_browser] All tests passed.\n";
    return 0;
}