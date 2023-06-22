#include "ContactDetails.h"
#include <optional>
#include <queue>

namespace experis
{

ContactDetails::ContactDetails(const MSISDN& a_phoneNum)
	: m_msisdn{a_phoneNum}, m_details{0}
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

void ContactDetails::Add(const UsageType a_usageType, const DATA a_quantity)
{
	this->m_details.at(a_usageType) += a_quantity;
}

ContactDetails::DATA ContactDetails::Get(const UsageType a_usageType) const
{
	return this->m_details.at(a_usageType);
}

}