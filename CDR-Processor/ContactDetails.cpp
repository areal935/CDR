#include "ContactDetails.h"
#include <optional>
#include <queue>

namespace experis
{

ContactDetails::ContactDetails(const MSISDN& a_msisdn)
	: m_msisdn{a_msisdn}, m_details{0}
{
}

ContactDetails::ContactDetails(const ContactDetails& a_other)
	: m_msisdn{a_other.m_msisdn}, m_details{0}
{
}

ContactDetails& ContactDetails::operator=(const ContactDetails& a_other)
{
	this->m_msisdn = a_other.m_msisdn;
	this->m_details = {0};
	return *this;
}

void ContactDetails::Add(const UsageType a_usageType, const DATA a_data)
{
	this->m_details.at(a_usageType) += a_data;
}

ContactDetails::DATA ContactDetails::Get(const UsageType a_usageType) const
{
	return this->m_details.at(a_usageType);
}

}