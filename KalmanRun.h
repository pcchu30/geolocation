#pragma once

#include "stdafx.h"
#include "Position.h"
#include "Position2.h"



std::vector<std::vector<double>> kalmanRunAdvance(std::vector< std::vector< Position > >& measurements);

// run a kalman filter work

std::vector<std::vector<double>> kalmanRun(std::vector< Position2 >& vecP2);
