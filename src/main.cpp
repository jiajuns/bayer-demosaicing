#include "Demosaic.h"

using namespace std;

int main()
{
    Demosaic demosaic = Demosaic("cam3_0_26_904206.tif");
    demosaic.generateRGBComponents();
    demosaic.interpolate();
    demosaic.colorize();
    demosaic.display();
    return 0;
}
