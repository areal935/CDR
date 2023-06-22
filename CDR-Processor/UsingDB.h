#pragma once
#include <unordered_map>
#include "ContactDetails.h"

namespace experis
{

using PHONE_NUM = unsigned long long;
//phone number
using SIM_ID = unsigned int;
	//sim_id
using QUANTITY = unsigned long long;

using SUBSCRIBERS = std::unordered_map<SIM_ID, std::vector<PHONE_NUM>>;
using LINKED_GRAPH = std::unordered_map<PHONE_NUM, std::unordered_map<PHONE_NUM, std::pair <QUANTITY, QUANTITY>>>;
	//{phone_num:{phone_num:(q,q)}}

}