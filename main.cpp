#include "utils.h"
#include "Position.h"
#include "PositionTable.h"

#include "Position2.h"
#include "PositionBase.h"

//#include "KalmanRun.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/geometry/geometries/linestring.hpp>
#include <boost/assign.hpp>

using namespace std;
typedef boost::geometry::model::d2::point_xy<double> point;




int main()
{
    /*    
    testPositionBase();    
    testPosition();  
    testPosition2(); 
    testPositionTable(); 
    */

    /////////////////////////////////
    //
    //  Basic Mission
    //  
    /////////////////////////////////


    // This Unit test covers all Basic Mission. Please check "PositionTable.cpp" and "PositionTable.h".    
    testPositionTable();
    



    return 0;
}
