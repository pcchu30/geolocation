#pragma once

#include <Eigen/Dense>


class KalmanFilter {

public:
    
    /**
    * Create a Kalman filter with the specified matrices.
    *   A - Transition matrix of the dynamic model
    *   H - Measurement model matrix
    *   Q - Process noise covariance
    *   R - Measurement noise covariance
    *   P - Estimate error covariance
    */
    
    
    KalmanFilter(
        double dt,
        const Eigen::MatrixXd& A,
        const Eigen::MatrixXd& H,
        const Eigen::MatrixXd& Q,
        const Eigen::MatrixXd& R,
        const Eigen::MatrixXd& P0
    );

    /**
    * Initialize the filter with a guess for initial states.
    */
    void init(double t0, const Eigen::VectorXd& x0);

    /**
    * Update the estimated state based on measured values. The
    * time step is assumed to remain constant.
    */
    void update(const Eigen::VectorXd& y);

    /**
    * Return the current state and time.
    */
    Eigen::VectorXd state() { return x_hat; };
    double time() { return t; };

private:
    
    // Matrices for computation
    Eigen::MatrixXd A, H, Q, R, P, K, P0;

    // System dimensions
    int m, n;

    // Initial and current time
    double t0, t;

    // Discrete time step
    double dt;

    // Is the filter initialized?
    bool initialized;

    // n-size identity
    Eigen::MatrixXd I;


    // Estimated states
    Eigen::VectorXd x_hat, x_hat_new;

};
