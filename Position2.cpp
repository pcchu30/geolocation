#include "Position2.h"
#include "utils.h"

using namespace std;

Position2::Position2(unsigned long int timestamp, unsigned long int originalTimestamp, double lon, double lat)
: PositionBase(lon,lat), timestamp(timestamp), originalTimestamp(originalTimestamp){}



///////////////////////////////////////////////
//
//   Tests
//
///////////////////////////////////////////////

static void testPosition2Members() {
    unsigned long int t=1511395270;
    unsigned long int ot=1511395268; 
    double lon=121.301285;
    double lat=25.05085;    
    Position2 p(t, ot, lon, lat);

    unsigned long int res=p.getTimestamp();
    ASSERT( res==t );
    cout << res << endl;

    res=p.getOriginalTimestamp();
    ASSERT( res==ot );
    cout << res << endl;

    double res1=p.getLongitude();
    ASSERT( res1==lon );
    cout << res1 << endl;

    res1=p.getLatitude();
    ASSERT( res1==lat );
    cout << res1 << endl;
}

void testPosition2(){
    TEST( testPosition2Members );
}


