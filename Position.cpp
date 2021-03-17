#include "Position.h"
#include "utils.h"

using namespace std;

Position::Position(string dt, double lon, double lat, string tZone): PositionBase(lon,lat), dateTime(dt), tZone(tZone)
{        
    vector<string> parts = dateTimeSplit(dateTime);
    timestamp = stringToTimestamp(parts[0], parts[1], tZone); 
}



///////////////////////////////////////////////
//
//   TESTS
//
///////////////////////////////////////////////

static void testPositionMembers() {
    string dt("2017-11-23 08:01:10");    
    double lon=121.301285;
    double lat=25.05085;    
    Position p(dt, lon, lat);

    string dateTime=p.getDateTime();
    ASSERT( dateTime==dt );
    cout << dateTime << endl;

    unsigned long int res=p.getTimestamp();
    ASSERT( res==1511395270 );
    cout << res << endl;

    double resLon=p.getLongitude();
    ASSERT( resLon==lon );
    cout << resLon << endl;

    double resLat=p.getLatitude();
    ASSERT( resLat==lat );
    cout << resLat << endl;

    string tZone=p.getTZone();
    ASSERT( tZone=="Asia/Taipei" );
    cout << tZone << endl;
}

void testPosition(){
    TEST( testPositionMembers );
}


