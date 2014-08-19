#include "cpptest.h"

class SomeTestSuite: public Test::Suite
{
public:
    SomeTestSuite() { TEST_ADD(SomeTestSuite::test) }
private:
    void test()
    {

      // Will succeed since the expression evaluates to true
      //
      TEST_ASSERT(1 + 1 == 2)
    
      // Will fail since the expression evaluates to false
      //
      TEST_ASSERT(0 == 0);
    }
};

bool run_tests()
{
    SomeTestSuite sts;
    Test::TextOutput output(Test::TextOutput::Verbose);
    return sts.run(output, false); // Note the 'false' parameter
}

int main()
{
    run_tests();

}
