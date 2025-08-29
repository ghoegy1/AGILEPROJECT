/***************************************************************************************
 * test_match_suggester.cpp
 * Tests for suggesting partners by shared courses + overlapping availability.
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
#include "MatchSuggester.hpp"

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
    Profile me = makeProfile("Me", "me@clemson.edu", {"CPSC 2150"},
                             { {Day::Mon, 10*60, 12*60}, {Day::Tue, 14*60, 16*60} });

    Profile a  = makeProfile("Alice", "alice@clemson.edu", {"CPSC 2150"},
                             { {Day::Mon, 11*60, 13*60} }); // Overlap Mon: 60 min (11-12)

    Profile b  = makeProfile("Bob", "bob@clemson.edu", {"CPSC 2150", "MATH 1080"},
                             { {Day::Tue, 15*60, 17*60} }); // Overlap Tue: 60 min (15-16)

    Profile c  = makeProfile("Cara", "cara@clemson.edu", {"ENGL 1030"},
                             { {Day::Mon, 10*60, 12*60} }); // No shared courses → excluded

    Profile d  = makeProfile("Dan", "dan@clemson.edu", {"CPSC 2150"},
                             { {Day::Mon, 8*60, 9*60} }); // Shared course but no overlap

    std::vector<Profile> all = {me, a, b, c, d};

    {
        // Test 1: suggestions require shared course & overlap >= min (default 30)
        MatchSuggester ms;
        auto matches = ms.suggest(me, all); // minOverlap=30
        // Should include Alice (60) and Bob (60), exclude Cara (no shared course) and Dan (no overlap)
        assert(matches.size() == 2);
        // Both have 60 overlap; ties sorted by name ASC: Alice then Bob
        assert(matches[0].person->name() == "Alice");
        assert(matches[0].overlapMinutes == 60);
        assert(matches[0].sharedCourses.size() == 1 && matches[0].sharedCourses[0] == "CPSC 2150");
        assert(matches[1].person->name() == "Bob");
        assert(matches[1].overlapMinutes == 60);
    }

    {
        // Test 2: raise minOverlap to 90 → no one qualifies
        MatchSuggester ms;
        auto matches = ms.suggest(me, all, /*minOverlapMinutes*/ 90);
        assert(matches.empty());
    }

    {
        // Test 3: multiple shared courses and different overlap magnitudes
        // Add Eve with larger overlap (Mon 10-12 + Tue 14-16 -> full 240?)
        // Actually overlap with me is: Mon 10-12 (120) + Tue 14-16 (120) = 240
        Profile e  = makeProfile("Eve", "eve@clemson.edu", {"CPSC 2150", "ENGL 1030"},
                                 { {Day::Mon, 10*60, 12*60}, {Day::Tue, 14*60, 16*60} });

        std::vector<Profile> all2 = {me, a, b, c, d, e};
        MatchSuggester ms;
        auto matches = ms.suggest(me, all2, 30, 3);
        // Expect top is Eve (240), then Alice (60), then Bob (60) but truncated to 3 results
        assert(matches.size() == 3);
        assert(matches[0].person->name() == "Eve" && matches[0].overlapMinutes == 240);
        // Next two (60 each) sorted by name
        assert(matches[1].person->name() == "Alice");
        assert(matches[2].person->name() == "Bob");
    }

    std::cout << "[test_match_suggester] All tests passed.\n";
    return 0;
}