/***************************************************************************************
 * test_notifications.cpp
 * Tests for NotificationCenter push/fetch semantics.
 *
 * STANDARD LIBRARIES USED:
 *  <cassert>, <iostream>, <vector>, <string>
 ****************************************************************************************/
#include <cassert>
#include <iostream>
#include "NotificationCenter.hpp"

using namespace sb;

int main() {
    NotificationCenter nc;

    {
        // Test 1: notify queues messages per email
        nc.notify("a@x.com", "hello");
        nc.notify("a@x.com", "world");
        auto peek = nc.peek("a@x.com");
        assert(peek.size() == 2);
    }

    {
        // Test 2: fetchAndClear returns then clears
        auto msgs = nc.fetchAndClear("a@x.com");
        assert(msgs.size() == 2);
        auto empty = nc.fetchAndClear("a@x.com");
        assert(empty.empty());
    }

    {
        // Test 3: independent inboxes
        nc.notify("b@x.com", "one");
        nc.notify("c@x.com", "two");
        auto b = nc.peek("b@x.com");
        auto c = nc.peek("c@x.com");
        assert(b.size() == 1 && c.size() == 1);
    }

    std::cout << "[test_notifications] All tests passed.\n";
    return 0;
}