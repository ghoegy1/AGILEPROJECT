/***************************************************************************************
 * AvailabilityManager.hpp
 * Feature 3: Add/Remove availability slots (with overlap merge on the same day).
 *
 * STANDARD LIBRARIES USED:
 *  <vector>     : for merged list construction.
 *  <algorithm>  : std::sort, std::max for merge step.
 *  <iostream>   : feedback and listing.
 ****************************************************************************************/
#pragma once
#include "Profile.hpp"
#include <string>

namespace sb {

class AvailabilityManager {
public:
    // Add a slot with merge (validates start<end).
    void addAvailability(Profile& prof, Day day, int startMin, int endMin);

    // Remove a slot by 1-based index as displayed.
    void removeAvailability(Profile& prof, int oneBasedIndex);

    // Helper to prompt a day (1..7) from the user.
    static Day promptDay();

    // Helper to list availability with indices (for removal).
    static void listIndexed(const Profile& prof);

private:
    static bool overlapsSameDay(const AvailabilitySlot& a, const AvailabilitySlot& b);
    static void addMerged(Profile& prof, const AvailabilitySlot& s);
};

} // namespace sb