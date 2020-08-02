#include "smps/smps.h"

#include "gtest/gtest.h"


TEST(TestSmps, ThrowsBogusDataFiles)
{
    smps::Smps smps;

    ASSERT_THROW(smps.read("some bogus location"), std::runtime_error);
}
