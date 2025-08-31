/***************************************************************************************
 * test_session_requests.cpp
 * Tests for sending & confirming study session requests (with notifications).
 *
 * STANDARD LIBRARIES USED:
 *  <cassert>, <iostream>, <vector>, <string>
 ****************************************************************************************/
#include <cassert>
#include <iostream>
#include "SessionRequests.hpp"
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
    Profile al = makeProfile("Alice", "alice@clemson.edu", {"CPSC 2150", "MATH 1080"});
    Profile bo = makeProfile("Bob", "bob@clemson.edu", {"CPSC 2150"});

    {
        // Test 1: sendRequest creates Pending and notifies invitee
        const auto& s = sr.sendRequest(me, al, "cpsc 2150", Day::Mon, 10*60, 11*60);
        assert(s.status == StudySession::Status::Pending);
        auto inbox = nc.peek("alice@clemson.edu");
        assert(!inbox.empty()); // received notification
    }

    {
        // Test 2: pendingFor lists invitee's pending requests
        auto pending = sr.pendingFor(al);
        assert(pending.size() == 1);
        assert(pending[0].invitee == "alice@clemson.edu");
    }

    {
        // Test 3: confirmRequest changes status, sends notifications to both parties
        auto pending = sr.pendingFor(al);
        bool ok = sr.confirmRequest(pending[0].id, al);
        assert(ok);

        auto confMe = nc.peek("me@clemson.edu");
        auto confAl = nc.peek("alice@clemson.edu");
        // at least 1 new message each since last peek
        assert(!confMe.empty());
        assert(confAl.size() >= 2); // had earlier "New request", now "You confirmed..."
        // Confirmed should appear in confirmedFor for both
        auto cm1 = sr.confirmedFor(al);
        auto cm2 = sr.confirmedFor(me);
        assert(cm1.size() == 1 && cm2.size() == 1);
    }

    std::cout << "[test_session_requests] All tests passed.\n";
    return 0;
}