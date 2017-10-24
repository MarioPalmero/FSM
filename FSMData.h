
// - Mario Palmero [2017], zlib/libpng licensed.

#pragma once

struct FSMData
{
	// The most important data to have is the current state
	// Be careful when inheriting the FSM and the FSMData
	// E currentState;
	int currentState;
};
