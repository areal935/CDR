#pragma once
#include <array>
#include <string>
#include <unordered_map>

namespace experis
{

enum UsageType
{
	MOC, // outgoing voice call
	MTC, // incoming voice call
	SMS_MO, // outgoing message
	SMS_MT, // incoming message
	DI, // Data Received
	DO, // Data Transmitted
	U, // call not answered
	B, // busy call
	X, // failed call
	USAGE_TYPE_SIZE
};

class ContactDetails
{
	using MSISDN = unsigned long long;
	using DATA = unsigned long long;

public:
	ContactDetails() = default;
	ContactDetails(const MSISDN& a_phoneNum);
	ContactDetails(const ContactDetails& a_other);
	ContactDetails& operator=(const ContactDetails& a_other);
	~ContactDetails() = default;

	void Add(const UsageType a_usageType, const DATA a_quantity);
	DATA Get(const UsageType a_usageType) const;

private:
	MSISDN m_msisdn;

	std::array<unsigned long long, UsageType::USAGE_TYPE_SIZE> m_details;
};

}