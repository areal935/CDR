#include "CDR.h"
#include "HendlerFunction.h"
#include <istream>

namespace experis
{

CDR::CDR(CDR_QUEUE& a_containerQueries, std::istream& a_whereInput)
	: m_containerCDRs{a_containerQueries}, m_producerCDRsThread{&CDR::GetCDRs, this}, m_whereInput{a_whereInput}
{
}

void CDR::GetCDRs()
{
	while (true)
	{
		std::string untrust_cdr = "";
		std::getline(m_whereInput, untrust_cdr);
		std::vector<std::string> query = Split(untrust_cdr, {'|', ' '});
		this->m_containerCDRs.Push(query);
	}
}

} // experis