#include "LOG.h"

int main()
{
    /**
     * test
     * >gcc -o log LOG.cpp -D DEBUG
     */
    LOG_INFO("debug something %d !\n", 1);
    LOG_ERROR("something error %d !\n", 1);
    return 0;
}