/***************************************************************************************
 * MatchSuggester.cpp — implementation
 ****************************************************************************************/
#include "MatchSuggester.hpp"
#include "Utils.hpp"
#include <algorithm>

namespace sb {

bool MatchSuggester::sameUser(const Profile& a, const Profile& b) {
    if (!a.email().empty() && !b.email().empty()) return a.email() == b.email();
    if (!a.name().empty() && !b.name().empty()) return a.name() == b.name();
    return &a == &b;
}

std::vector<std::string> MatchSuggester::sharedCoursesUpper(const Profile& a, const Profile& b) {
    std::vector<std::string> shared;
    for (const auto& ca : a.courses()) {
        for (const auto& cb : b.courses()) {
            if (ca == cb) shared.push_back(ca);
        }
    }
    // dedup (just in case)
    std::sort(shared.begin(), shared.end());
    shared.erase(std::unique(shared.begin(), shared.end()), shared.end());
    return shared;
}

static int overlapSpan(int a1, int a2, int b1, int b2) {
    // Overlap between [a1,a2) and [b1,b2)
    int lo = std::max(a1, b1);
    int hi = std::min(a2, b2);
    return std::max(0, hi - lo);
}

int MatchSuggester::totalOverlapMinutes(const Profile& a, const Profile& b) {
    int total = 0;
    // For each day, sum overlaps of slots
    for (int d = 0; d < 7; ++d) {
        Day day = static_cast<Day>(d);
        // collect slots for day
        std::vector<AvailabilitySlot> A, B;
        for (const auto& s : a.availability()) if (s.day == day) A.push_back(s);
        for (const auto& s : b.availability()) if (s.day == day) B.push_back(s);
        // both lists should already be merged; but even if not, logic still works
        for (const auto& sa : A)
            for (const auto& sb : B)
                total += overlapSpan(sa.start, sa.end, sb.start, sb.end);
    }
    return total;
}

std::vector<Match> MatchSuggester::suggest(const Profile& self,
                                           const std::vector<Profile>& all,
                                           int minOverlapMinutes,
                                           std::size_t maxResults) const {
    std::vector<Match> res;
    for (const auto& p : all) {
        if (sameUser(self, p)) continue;
        auto shared = sharedCoursesUpper(self, p);
        if (shared.empty()) continue;
        int overlap = totalOverlapMinutes(self, p);
        if (overlap < minOverlapMinutes) continue;
        res.push_back(Match{&p, shared, overlap});
    }
    std::sort(res.begin(), res.end(),
              [](const Match& x, const Match& y){
                  if (x.overlapMinutes != y.overlapMinutes)
                      return x.overlapMinutes > y.overlapMinutes; // DESC
                  const std::string& nx = x.person->name().empty() ? x.person->email() : x.person->name();
                  const std::string& ny = y.person->name().empty() ? y.person->email() : y.person->name();
                  return nx < ny;
              });
    if (res.size() > maxResults) res.resize(maxResults);
    return res;
}

} // namespace sb