#pragma once
#include <vector>
#include <string>

namespace experis
{

std::vector<std::string> Split(const std::string& a_toSplit, const std::vector<char>& a_howSplit);;
std::string JsonFormat(std::vector<std::pair<std::string, unsigned long long>> a_dataToOut);

}