/***************************************************************************************
 * SessionRequests.hpp
 * Feature: Send & confirm study session requests; emit notifications; list sessions.
 *
 * STANDARD LIBRARIES USED:
 *  <vector>      : store sessions
 *  <string>      : ids, emails, course codes
 *  <algorithm>   : remove_if, sort
 *  <utility>     : move
 ****************************************************************************************/
#pragma once
#include "Profile.hpp"
#include "NotificationCenter.hpp"
#include <vector>
#include <string>

namespace sb {

// A scheduled study session (pending or confirmed).
struct StudySession {
    std::string id;          // simple incrementing id (e.g., "S1")
    std::string course;      // normalized uppercase
    Day day;
    int start;               // minutes since midnight
    int end;                 // minutes since midnight
    std::string requester;   // email
    std::string invitee;     // email
    enum class Status { Pending, Confirmed, Declined };
    Status status;
};

class SessionRequests {
public:
    explicit SessionRequests(NotificationCenter* nc) : nc_(nc) {}

    // Send a request from 'from' to 'to' for 'course' and time window.
    // Returns the created session object reference.
    const StudySession& sendRequest(const Profile& from, const Profile& to,
                                    const std::string& courseUpper,
                                    Day day, int startMin, int endMin);

    // Confirm a pending request (only invitee may confirm).
    // Returns true if state changed to Confirmed and a notification was sent.
    bool confirmRequest(const std::string& sessionId, const Profile& byInvitee);

    // Get all pending requests where 'user' is the invitee.
    std::vector<StudySession> pendingFor(const Profile& user) const;

    // Get all confirmed sessions where 'user' is either requester or invitee.
    std::vector<StudySession> confirmedFor(const Profile& user) const;

    // Cancel a confirmed session (either party may cancel); removes it entirely.
    bool cancelConfirmed(const std::string& sessionId, const Profile& byEither);

private:
    static const std::string& userKey(const Profile& p); // email identity
    static std::string nextId();

    std::vector<StudySession> sessions_;
    NotificationCenter* nc_;
};

} // namespace sb