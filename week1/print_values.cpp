#include "test_runner.h"

#include <ostream>
using namespace std;

#define PRINT_VALUES(out, x, y)   ((out) << (x) << endl << (y) << endl)

int main() {
    TestRunner tr;
    tr.RunTest([] {
        ostringstream output;
        PRINT_VALUES(output, 5, "red belt");
        ASSERT_EQUAL(output.str(), "5\nred belt\n");
        for (int i = 0; i < 10; i++)
            PRINT_VALUES(output, 5, 4);

        ASSERT_EQUAL(output.str(), "5\nred belt\n5\n4\n5\n4\n5\n4\n5\n4\n5\n4\n5\n4\n5\n4\n5\n4\n5\n4\n5\n4\n");
    }, "PRINT_VALUES usage example");
    return 0;
}