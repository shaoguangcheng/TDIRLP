#include "test.h"

void testHalfPlane()
{
    halfPlane hp(-1,1,1);

    if(LEFT == hp.direction)
        cout << "left" << endl;
    else
        cout << "right" << endl;
}
