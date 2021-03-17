#include "PositionBase.h"

using namespace std;




//////////////////////////////
//
//  Tests
//
//////////////////////////////


static void testPositionBaseMembers() {
    double lon=121.301285;
    double lat=25.05085;    
    PositionBase p( lon, lat);
    double res=p.getLongitude();
    ASSERT( res==121.301285 );
    cout << res << endl;

    res=p.getLatitude();
    ASSERT( res==25.05085 );
    cout << res << endl;
}

void testPositionBase(){
    TEST( testPositionBaseMembers );
}

