#include <gmock/gmock.h>
#include "hiker.h"

using namespace ::testing;

namespace {

    TEST(Hiker, Life_the_universe_and_everything)
    {
        ASSERT_THAT(answer(), Eq(42));
    }

}
