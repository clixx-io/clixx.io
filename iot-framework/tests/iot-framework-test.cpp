#include "cpptest.h"

class clixxIOTestSuite: public Test::Suite
{
public:
    clixxIOTestSuite() { TEST_ADD(clixxIOTestSuite::test) }
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
    clixxIOTestSuite sts;
    Test::TextOutput output(Test::TextOutput::Verbose);
    return sts.run(output, false); // Note the 'false' parameter
}

int main()
{
    run_tests();

}
