#pragma once
#include "TaskQueue.h"
#include <thread>

namespace experis
{

class CDR
{
public:
	using CDR_QUEUE = TaskQueue_mt<std::vector<std::string>>;
	CDR() = delete;
	CDR(CDR_QUEUE& a_containerQueries, std::istream& a_whereInput);
	CDR(const CDR& a_other) = delete;
	CDR& operator=(const CDR& a_other) = delete;
	~CDR() = default;

	void GetCDRs();

private:
	CDR_QUEUE& m_containerCDRs;
	std::istream& m_whereInput;
	std::thread m_producerCDRsThread;
};

}