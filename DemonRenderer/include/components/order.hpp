#include <limits>
#include <stdint.h>
#pragma once

/** \struct Order
*	\brief Very simple component to allow game object to have an order.
*
*/

struct Order
{
public:
	uint32_t order{ std::numeric_limits<uint32_t>::max() };
};