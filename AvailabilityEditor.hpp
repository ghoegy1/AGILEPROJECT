/***************************************************************************************
 * AvailabilityEditor.hpp
 * Feature: Remove or EDIT availability slots on a Profile (merges after edits).
 *
 * STANDARD LIBRARIES USED:
 *  <cstddef>   : size_t
 *  <string>    : error messages (optional, not exposed here)
 *  <vector>    : internal merging
 *  <algorithm> : sort, max for merge logic
 *  <iostream>  : optional feedback (implemented in .cpp)
 ****************************************************************************************/
#pragma once
#include "Profile.hpp"
#include <cstddef>

namespace sb {

class AvailabilityEditor {
public:
    // Edit an existing slot by 1-based index:
    // - Validates bounds and new time (end > start, same-day invariant not required here).
    // - Replaces the indexed slot with the new [start,end).
    // - Re-sorts and merges overlapping same-day slots.
    // Returns true on success, false if validation fails.
    bool editSlot(Profile& prof, int oneBasedIndex, Day day, int newStartMin, int newEndMin);

    // Remove an existing slot by 1-based index. Returns true if removed.
    bool removeSlot(Profile& prof, int oneBasedIndex);

private:
    static void mergeAll(Profile& prof);
};

} // namespace sb