/***************************************************************************************
 * SessionRequests.cpp — hardened identity & matching
 ****************************************************************************************/
#include "SessionRequests.hpp"
#include "Utils.hpp"
#include <algorithm>

namespace sb {

// Prefer email; if blank, fallback to name. Always trimmed.
static std::string primaryKey(const Profile& p) {
    std::string e = trim(p.email());
    if (!e.empty()) return e;
    return trim(p.name());
}

// Secondary identifier: if email exists, this is the trimmed name; else empty.
static std::string secondaryKey(const Profile& p) {
    std::string e = trim(p.email());
    if (!e.empty()) return trim(p.name());
    return std::string{};
}

// Keep the class-private declaration intact for ABI; do not use elsewhere.
const std::string& SessionRequests::userKey(const Profile& p) {
    return p.email().empty() ? p.name() : p.email();
}

std::string SessionRequests::nextId() {
    static int counter = 0;
    return "S" + std::to_string(++counter);
}

const StudySession& SessionRequests::sendRequest(const Profile& from, const Profile& to,
                                                 const std::string& courseUpper,
                                                 Day day, int startMin, int endMin) {
    StudySession s;
    s.id        = nextId();
    s.course    = upperCopy(trim(courseUpper));
    s.day       = day;
    s.start     = startMin;
    s.end       = endMin;

    // Store robust, TRIMMED identifiers
    s.requester = primaryKey(from);
    s.invitee   = primaryKey(to);
    s.status    = StudySession::Status::Pending;

    sessions_.push_back(s);

    if (nc_) {
        nc_->notify(s.invitee, "New study request " + s.id + " from " + s.requester +
                               " for " + s.course);
    }
    return sessions_.back();
}

bool SessionRequests::confirmRequest(const std::string& sessionId, const Profile& byInvitee) {
    const std::string whoP = primaryKey(byInvitee);
    const std::string whoS = secondaryKey(byInvitee);

    for (auto& s : sessions_) {
        if (s.id != sessionId || s.status != StudySession::Status::Pending) continue;

        if (s.invitee == whoP || (!whoS.empty() && s.invitee == whoS)) {
            s.status = StudySession::Status::Confirmed;
            if (nc_) {
                nc_->notify(s.requester, "Study request " + s.id + " confirmed by " + s.invitee);
                nc_->notify(s.invitee,   "You confirmed study request " + s.id);
            }
            return true;
        }
    }
    return false;
}

std::vector<StudySession> SessionRequests::pendingFor(const Profile& user) const {
    const std::string whoP = primaryKey(user);
    const std::string whoS = secondaryKey(user);

    std::vector<StudySession> out;
    for (const auto& s : sessions_) {
        if (s.status == StudySession::Status::Pending &&
            (s.invitee == whoP || (!whoS.empty() && s.invitee == whoS))) {
            out.push_back(s);
        }
    }
    return out;
}

std::vector<StudySession> SessionRequests::confirmedFor(const Profile& user) const {
    const std::string whoP = primaryKey(user);
    const std::string whoS = secondaryKey(user);

    std::vector<StudySession> out;
    for (const auto& s : sessions_) {
        if (s.status != StudySession::Status::Confirmed) continue;

        bool matches =
            (s.invitee == whoP || s.requester == whoP) ||
            (!whoS.empty() && (s.invitee == whoS || s.requester == whoS));

        if (matches) out.push_back(s);
    }

    std::sort(out.begin(), out.end(), [](const StudySession& a, const StudySession& b){
        if (a.day != b.day) return static_cast<int>(a.day) < static_cast<int>(b.day);
        return a.start < b.start;
    });
    return out;
}

bool SessionRequests::cancelConfirmed(const std::string& sessionId, const Profile& byEither) {
    const std::string whoP = primaryKey(byEither);
    const std::string whoS = secondaryKey(byEither);

    for (auto it = sessions_.begin(); it != sessions_.end(); ++it) {
        if (it->id == sessionId && it->status == StudySession::Status::Confirmed) {
            bool matches = (it->requester == whoP || it->invitee == whoP) ||
                           (!whoS.empty() && (it->requester == whoS || it->invitee == whoS));
            if (matches) {
                if (nc_) {
                    const std::string other =
                        (it->requester == whoP || (!whoS.empty() && it->requester == whoS))
                        ? it->invitee : it->requester;
                    nc_->notify(other, "Study session " + it->id + " was canceled by " + whoP);
                }
                sessions_.erase(it);
                return true;
            }
        }
    }
    return false;
}

} // namespace sb