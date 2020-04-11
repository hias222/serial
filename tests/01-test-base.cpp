#include <stdio.h>
#include <string.h>
#include <string>

#include "listener.h"

using namespace std;

#include "Base.hh"

bool testBasic()
{
    bool check = checkPort();
    TEST_ASSERT_THROW(check);
    //ALEPH_ASSERT_EQUAL(1, 2);
    return check;
}

void testAdvanced()
{
    // a more advanced test
    bool check = start();
    TEST_ASSERT_THROW(check);
}

int main(int, char **)
{
    if (testBasic())
    {
        testAdvanced();
    }
}