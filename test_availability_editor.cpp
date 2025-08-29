/***************************************************************************************
 * test_availability_editor.cpp
 * Tests for editing/removing availability (AvailabilityEditor).
 *
 * STANDARD LIBRARIES USED:
 *  <cassert>  : assertions
 *  <iostream> : summary output
 ****************************************************************************************/
#include <cassert>
#include <iostream>
#include <vector>
#include "Profile.hpp"
#include "AvailabilityManager.hpp" // to add initial slots
#include "AvailabilityEditor.hpp"
#include "CourseManager.hpp"

using namespace sb;

int main() {
    {
        // Test 1: Edit expands a slot to overlap the next; result should merge
        Profile p;
        auto norm = CourseManager::normalizeDedup({"CPSC 2150"});
        p.createOrReset("Me", "me@clemson.edu", norm);
        AvailabilityManager am;
        AvailabilityEditor ed;

        // Start with two separate Monday slots: 09:00-10:00 and 11:00-12:00
        am.addAvailability(p, Day::Mon, 9*60, 10*60);
        am.addAvailability(p, Day::Mon, 11*60, 12*60);
        assert(p.availability().size() == 2);

        // Edit the first slot to 09:00-11:30 → overlaps/merges with [11:00-12:00]
        bool ok = ed.editSlot(p, 1, Day::Mon, 9*60, 11*60 + 30);
        assert(ok);
        // Expect one merged slot: 09:00-12:00
        assert(p.availability().size() == 1);
        assert(p.availability()[0].start == 9*60);
        assert(p.availability()[0].end   == 12*60);
    }

    {
        // Test 2: Invalid edit (end<=start) is rejected; no changes
        Profile p;
        auto norm = CourseManager::normalizeDedup({"MATH 1080"});
        p.createOrReset("Me", "me@clemson.edu", norm);
        AvailabilityManager am;
        AvailabilityEditor ed;

        am.addAvailability(p, Day::Tue, 8*60, 9*60);
        size_t before = p.availability().size();
        bool ok1 = ed.editSlot(p, 1, Day::Tue, 9*60, 9*60);
        bool ok2 = ed.editSlot(p, 1, Day::Tue, 10*60, 9*60);
        assert(!ok1 && !ok2);
        assert(p.availability().size() == before);
        assert(p.availability()[0].start == 8*60);
        assert(p.availability()[0].end   == 9*60);
    }

    {
        // Test 3: Remove a slot by index
        Profile p;
        auto norm = CourseManager::normalizeDedup({"ENGL 1030"});
        p.createOrReset("Me", "me@clemson.edu", norm);
        AvailabilityManager am;
        AvailabilityEditor ed;

        am.addAvailability(p, Day::Wed, 14*60, 15*60); // [1]
        am.addAvailability(p, Day::Wed, 16*60, 17*60); // [2]
        assert(p.availability().size() == 2);

        bool ok = ed.removeSlot(p, 2);
        assert(ok);
        assert(p.availability().size() == 1);
        assert(p.availability()[0].start == 14*60);
    }

    std::cout << "[test_availability_editor] All tests passed.\n";
    return 0;
}