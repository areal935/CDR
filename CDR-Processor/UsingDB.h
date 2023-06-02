#pragma once
#include <unordered_map>
#include "ContactDetails.h"

namespace experis
{

using MSISDN = unsigned long long;
using IMSI = unsigned int;
using DATA = unsigned long long;
using DB = std::unordered_map<MSISDN, ContactDetails>;
using SUBSCRIBERS = std::unordered_map<IMSI, std::vector<MSISDN>>;
using LINKED_GRAPH = std::unordered_map<MSISDN, std::unordered_map<MSISDN, std::pair <DATA, DATA>>>;

}