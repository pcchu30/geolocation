#include "utils.h"
#include "Position.h"
#include "PositionTable.h"

#include "Position2.h"
#include "PositionBase.h"

#include "KalmanRun.h"

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


    /////////////////////////////////
    //
    //  Advanced Mission I
    //  (route_user_1)
    //  
    /////////////////////////////////


       
    string path1("data/route_user_1.csv");
    PositionTable pt1(path1);  // create a PositionTable object
    pt1.setTable(); // load csv file, convert string dateTime to Unix timestamp, and create a new column timestamp
    pt1.sortTable();// sort table
    //pt1.displayContents();// display result after sorting

    int m1 = 9; // prepare data for Kalman filter
    vector< vector< Position > > measurements1 = pt1.getKalmanMatrix(m1); // each time 9 points are sent to kalman filter, 
                                                                         // and each point has x and y coordinates, so we have 2*9=18 measurements.
    //display2DMeasurements(measurements1); // display measurements
    vector<vector<double>> kStates1 = kalmanRunAdvance(measurements1);   // run kalman filter, and get hidden states.
    string advanceIOutput_user_1("data/output/advanced_I_user_1_linestring.wkt"); // file path and name
    writeLinestring( kStates1, advanceIOutput_user_1 );  // write file, and we can upload wkt file to geojson website, and save it as GeoJSON file.
    //matrixToLinestringOnScreen(kStates1);               // we can also output it on screen
    
    
    /////////////////////////////////
    //
    //  Advanced Mission II
    //  (route_user_1)
    //  
    /////////////////////////////////

        
    string path11("data/linestring.wkt"); // loading data from database
    string advanceIIOutput_user_1("data/output1/advance_II_user_1_linestring.wkt"); // set output path

    // go through every point from database linestring, 
    // and then find the closest point from user_1 data for each point, and add it to new polyline.
    pt1.writeAdjustedLineString(path11, advanceIIOutput_user_1); 


    /////////////////////////////////
    //
    //  Advanced Mission I
    //  (route_user_2)
    //  
    /////////////////////////////////

    /*
    string path2("data/route_user_2.csv");
    PositionTable pt2(path2);
    pt2.setTable(); // load csv file, convert string dateTime to Unix timestamp, and create a new column timestamp
    pt2.sortTable();// sort table
    //pt2.displayContents();// display result after sorting

    
    
    int m2 = 9; // prepare data for Kalman filter
    vector< vector< Position > > measurements2 = pt2.getKalmanMatrix(m2); // each time 9 points are sent to kalman filter, 
                                                                        // and each point has x and y coordinates, so we have 2*9=18 measurements.
    //display2DMeasurements(measurements2);  // display measurements

    vector<vector<double>> kStates2 = kalmanRunAdvance(measurements2); // run kalman filter, and get hidden states.
    string advanceIOutput_user_2("data/output/advanced_I_user_2_linestring.wkt"); // file path and name
    writeLinestring( kStates2, advanceIOutput_user_2 ); // write file, and we can upload wkt file to geojson website, and save it as GeoJSON file.
    //matrixToLinestringOnScreen(kStates2); // we can also output it on screen
    
    */


    /////////////////////////////////
    //
    //  Advanced Mission II
    //  (route_user_2)
    //  
    /////////////////////////////////
    
    /*    
    string path22("data/linestring.wkt"); // loading data from database
    string advanceIIOutput_user_2("data/output1/advance_II_user_2_linestring.wkt"); // set output path

    // go through every point from database linestring, 
    // and then find the closest point from user_1 data for each point, and add it to new polyline.
    pt2.writeAdjustedLineString(path22, advanceIIOutput_user_2); 
    */


    
    


    return 0;
}
