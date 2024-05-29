#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

#include "../../src/helper.hpp"

class GlossTestingFixture : public ::testing::TestWithParam<std::tuple<const char*, const char*>> {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_P(GlossTestingFixture, ProgramTests) {

    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf(ss.rdbuf());

    auto params = GetParam();
    const char* programName = std::get<0>(params);

    char* argv[] = { const_cast<char*>(programName) };
    int argc = sizeof(argv) / sizeof(argv[0]);
    runProgram(argc, argv);

    std::cout.rdbuf(old_cout);

    std::string expectedOutput = std::get<1>(params);
    EXPECT_EQ(ss.str(), expectedOutput);
}

// Define the parameterized test cases
INSTANTIATE_TEST_SUITE_P(
    GlossTests,
    GlossTestingFixture,
    ::testing::Values(
        std::make_tuple("../test/gls/basic.gls", "\"one\"\ntrue\n3"),
        std::make_tuple("../test/gls/scope.gls", "\"inner 'a' variable\"\n3\n\"outer 'a' variable\"\n1")
    )
);

