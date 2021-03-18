
#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "Position2.h"
#include "KalmanFilter.h"
#include "KalmanRun.h"



std::vector<std::vector<double>> kalmanRunAdvance(std::vector< std::vector< Position > >& measurements){
    
    int n = 4; // Number of hidden states, including x and y coordinates of position, and x and y coordinates of velocity.
    
    // each point has x and y coordinates, so we have 2*measurements[0].size() measurements.
    int m = 2*measurements[0].size(); // Number of observable measurements, x and y coordinates of position.

    double dt = 1.0; // Time step

    
    
    Eigen::MatrixXd A(n, n); // Transition matrix of the dynamic model
    Eigen::MatrixXd H(m, n); // Measurement model matrix
    Eigen::MatrixXd Q(n, n); // Process noise covariance
    Eigen::MatrixXd R(m, m); // Measurement noise covariance
    Eigen::MatrixXd P0(n, n); // Estimate error covariance


    // Transition matrix of the dynamic model
    // we have 4 hidden states, including x and y coordinates of position, and x and y coordinates of velocity.
    A << 1, 0, dt, 0, 
         0, 1, 0, dt, 
         0, 0, 1,  0,
         0, 0, 0,  1;
    
    // we have m observable measurements, x and y coordinates of position.
    /*    
    H << 1, 0, 0, 0,
         0, 1, 0, 0;
    */

    H.setZero();
    
    for (int i=0; i<m; i++){
        if ( i % 2 )
            H(i, 1) = 1; // y coordinate, that is Latitude
        else
            H(i, 0) = 1; // x coordinate, that is Longitude   
    }
    

    

    // Reasonable covariance matrices
    // we have only non-zero diagonal variances.
    double q=0.000025*0.3;

    Q << .0, .0, .0, .0,
         .0, .0, .0, .0,
         .0, .0,  q, .0,
         .0, .0, .0,  q;
    /*
    R << rv, .0,
         .0, rv;
    */

    Eigen::VectorXd vd(m);
     
    double rv=0.000025;

    for (int i=0; i<m; i++){
        vd(i)=rv;
    }

    // m x m Diagonal matrix for Measurement noise covariance
    R = vd.asDiagonal();
    
    // we assume initial values are certain, so all values are zero in P0. 
    P0 << .0, .0, .0, .0, 
          .0, .0, .0, .0,
          .0, .0, .0, .0,
          .0, .0, .0, .0;

    

    std::cout << "A: \n" << A << std::endl;
    std::cout << "H: \n" << H << std::endl;
    std::cout << "Q: \n" << Q << std::endl;
    std::cout << "R: \n" << R << std::endl;
    std::cout << "P0: \n" << P0 << std::endl;


    // Construct the filter
    KalmanFilter kf(dt, A, H, Q, R, P0);

    
    
    

    // initial states of position are certain, and the velocity
    // is assumed zero initially.

    
    Eigen::VectorXd x0(n);
    double t = 0;
    x0 << measurements[0][0].getLongitude(), measurements[0][0].getLatitude(), .0, .0;
    kf.init(t, x0);
    

    // declare 2D output matrix for saving states

    std::vector<std::vector<double>> outputVec;

    

    // display initial states, t0
    
    Eigen::VectorXd y(m);
    std::cout << "t = " << t << ", " << "x_hat[0]: " << kf.state().transpose() << std::endl;
    
    // saving initial states to output
    std::vector<double> rowVec0;
    rowVec0.push_back(kf.state()[0]);
    rowVec0.push_back(kf.state()[1]);
    outputVec.push_back(rowVec0);
    

    std::cout.precision(6);
    int i = 0;
    int nz = measurements[0].size();


    // Feed measurements into filter, output estimated states

    for (auto element = measurements.cbegin(); element != measurements.cend(); ++element ) {
        i++;        
        t += dt;
       
        int mm = 0;
        for (int k=0; k<nz; k++){
            y(mm++) = (*element)[k].getLongitude();
            y(mm++) = (*element)[k].getLatitude();
        }
        //std::cout << i << " " <<  y.transpose() << std::endl;

        kf.update(y);
        std::cout << "t = " << t << ", " << "y[" << i << "] = " << std::fixed << y.transpose()
        << ", x_hat[" << i << "] = " << kf.state().transpose() << std::endl;
        
        // saving states for output        
        std::vector<double> rowV;
        rowV.push_back(kf.state()[0]);
        rowV.push_back(kf.state()[1]);
        outputVec.push_back(rowV);
    } 

    return outputVec;
}

























std::vector<std::vector<double>> kalmanRun(std::vector< Position2 >& vecP2){
    
    int n = 4; // Number of hidden states, including x and y coordinates of position, and x and y coordinates of velocity.
    int m = 2; // Number of observable measurements, x and y coordinates of position.

    double dt = 1.0; // Time step

    
    
    Eigen::MatrixXd A(n, n); // Transition matrix of the dynamic model
    Eigen::MatrixXd H(m, n); // Measurement model matrix
    Eigen::MatrixXd Q(n, n); // Process noise covariance
    Eigen::MatrixXd R(m, m); // Measurement noise covariance
    Eigen::MatrixXd P0(n, n); // Estimate error covariance


    // Transition matrix of the dynamic model
    // we have 4 hidden states, including x and y coordinates of position, and x and y coordinates of velocity.
    A << 1, 0, dt, 0, 
         0, 1, 0, dt, 
         0, 0, 1,  0,
         0, 0, 0,  1;
    
    // we have only 2 observable measurements, x and y coordinates of position.
    H << 1, 0, 0, 0,
         0, 1, 0, 0;

    // Reasonable covariance matrices
    // we have only non-zero diagonal variances.
    double q=0.000025*0.3;

    Q << .0, .0, .0, .0,
         .0, .0, .0, .0,
         .0, .0,  q, .0,
         .0, .0, .0,  q;
    
    double rv=0.000025;
    R << rv, .0,
         .0, rv;
    // we assume initial values are certain, so all values are zero in P0. 
    P0 << .0, .0, .0, .0, 
          .0, .0, .0, .0,
          .0, .0, .0, .0,
          .0, .0, .0, .0;

    std::cout << "A: \n" << A << std::endl;
    std::cout << "H: \n" << H << std::endl;
    std::cout << "Q: \n" << Q << std::endl;
    std::cout << "R: \n" << R << std::endl;
    std::cout << "P0: \n" << P0 << std::endl;


    // Construct the filter
    KalmanFilter kf(dt, A, H, Q, R, P0);

    
    // List of noisy position measurements (y)
    
    std::vector<std::vector<double>> measurements = {
        {121.491341, 25.048048},
        {121.482689, 25.053755},
        {121.472244, 25.070864},
        {121.477394, 25.064817},
        {121.460823, 25.064442},
    };


    // initial states of position are certain, and the velocity
    // is assumed zero initially.
    
    Eigen::VectorXd x0(n);
    double t = 0;
    x0 << vecP2[0].getLongitude(), vecP2[0].getLatitude(), .0, .0;
    kf.init(t, x0);
    

    // Feed measurements into filter, output estimated states

    std::vector<std::vector<double>> outputVec;
    
    Eigen::VectorXd y(m);
    std::cout << "t = " << t << ", " << "x_hat[0]: " << kf.state().transpose() << std::endl;

    std::vector<double> rowVec0;
    rowVec0.push_back(kf.state()[0]);
    rowVec0.push_back(kf.state()[1]);
    outputVec.push_back(rowVec0);

    std::cout.precision(6);
    int i = 0;
    for (auto element = (vecP2.cbegin()+1); element != vecP2.cend(); ++element ) {
        i++;        
        t += dt;
        y << element->getLongitude(), element->getLatitude();
        kf.update(y);
        std::cout << "t = " << t << ", " << "y[" << i << "] = " << std::fixed << y.transpose()
        << ", x_hat[" << i << "] = " << kf.state().transpose() << std::endl;
        std::vector<double> rowV;
        rowV.push_back(kf.state()[0]);
        rowV.push_back(kf.state()[1]);
        outputVec.push_back(rowV);
    }

    return outputVec;
}
