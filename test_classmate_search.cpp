/***************************************************************************************
 * test_classmate_search.cpp
 * Tests for searching classmates by course or name.
 *
 * STANDARD LIBRARIES USED:
 *  <cassert>, <iostream>, <vector>, <string>
 ****************************************************************************************/
#include <cassert>
#include <iostream>
#include <vector>
#include "ClassmateSearch.hpp"
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
    Profile me = makeProfile("Me", "me@clemson.edu", {"CPSC 2150"});
    Profile a  = makeProfile("Alice Johnson", "alice@clemson.edu", {"CPSC 2150", "MATH 1080"});
    Profile b  = makeProfile("Bob Smith", "bob@clemson.edu", {"ENGL 1030"});
    Profile c  = makeProfile("ALINA Brown", "alina@clemson.edu", {"CPSC 2150"});

    std::vector<Profile> all{me, a, b, c};

    {
        // Test 1: byCourse finds Alice and Alina, excludes Me and Bob
        auto v = ClassmateSearch::byCourse(all, me, "cPsC 2150");
        assert(v.size() == 2);
        std::vector<std::string> emails;
        for (auto* p : v) emails.push_back(p->email());
        assert((emails[0] == "alice@clemson.edu" || emails[1] == "alice@clemson.edu"));
        assert((emails[0] == "alina@clemson.edu" || emails[1] == "alina@clemson.edu"));
    }

    {
        // Test 2: byCourse for a course no classmates have
        auto v = ClassmateSearch::byCourse(all, me, "PHYS 1220");
        assert(v.empty());
    }

    {
        // Test 3: byName is case-insensitive substring and excludes self
        auto v1 = ClassmateSearch::byName(all, me, "ali");  // matches Alice and Alina
        assert(v1.size() == 2);
        auto v2 = ClassmateSearch::byName(all, me, "smith"); // matches Bob
        assert(v2.size() == 1 && v2[0]->email() == "bob@clemson.edu");
    }

    std::cout << "[test_classmate_search] All tests passed.\n";
    return 0;
}