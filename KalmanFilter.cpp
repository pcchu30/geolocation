#include <iostream>
#include <stdexcept>

#include "KalmanFilter.h"


KalmanFilter::KalmanFilter(
    double dt,
    const Eigen::MatrixXd& A,
    const Eigen::MatrixXd& H,
    const Eigen::MatrixXd& Q,
    const Eigen::MatrixXd& R,
    const Eigen::MatrixXd& P0)
  : A(A), H(H), Q(Q), R(R), P0(P0),
    m(H.rows()), n(A.rows()), dt(dt), initialized(false),
    I(n, n), x_hat(n), x_hat_new(n)
{
  I.setIdentity();
}


void KalmanFilter::init(double t0, const Eigen::VectorXd& x0) {
    x_hat = x0;
    P = P0;
    this->t0 = t0;
    t = t0;
    initialized = true;
}


void KalmanFilter::update(const Eigen::VectorXd& y) {

    if(!initialized)
    throw std::runtime_error("Filter is not initialized!");

    x_hat_new = A * x_hat;
    P = A*P*A.transpose() + Q;

    K = P*H.transpose()*(H*P*H.transpose() + R).inverse();
    x_hat_new += K * (y - H*x_hat_new);
    P = (I - K*H)*P;
    x_hat = x_hat_new;

    t += dt;
}

