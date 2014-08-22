#include <mosquittopp.h>
#include <iostream>

int main()
{
    mosquittopp::mosquittopp *m;

    m = new mosquittopp::mosquittopp("hi");

    m->lib_init();

    m->lib_cleanup();

}
