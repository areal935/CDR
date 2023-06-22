#include "CppUnitTest.h"
#include "DataBase.h"
#include <exception>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DBTest
{
	TEST_CLASS(DBTest)
	{
	public:
		
		TEST_METHOD(MSISDNTest)
		{
			experis::DataBase_mt db{};
			db.Insert(40052, 9720524410144, experis::DO, 567);
			Assert::AreEqual(567ULL, db.GetPhoneNumInfo(9720524410144ULL, experis::DO));

			db.Insert(40052, 9720528391949, experis::DO, 893);
			Assert::AreEqual(893ULL, db.GetPhoneNumInfo(9720528391949ULL, experis::DO));

			db.Insert(40052, 9720524410144, 9720528391949, experis::MOC, 567);
			db.Insert(40052, 9720528391949, 9720524410144, experis::MTC, 567);
			Assert::AreEqual(567ULL, db.GetPhoneNumInfo(9720524410144ULL, experis::MOC));
			Assert::AreEqual(567ULL, db.GetPhoneNumInfo(9720528391949ULL, experis::MTC));

			db.Insert(40052, 9720524410144, 9720523355899, experis::MOC, 900);
			db.Insert(40052, 9720523355899, 9720524410144, experis::MTC, 900);
			Assert::AreEqual(1467ULL, db.GetPhoneNumInfo(9720524410144ULL, experis::MOC));
			Assert::AreEqual(900ULL, db.GetPhoneNumInfo(9720523355899ULL, experis::MTC));

			bool isThrow = false;
			try
			{
				db.GetPhoneNumInfo(9720523350899ULL, experis::MTC);
			}
			catch (std::exception e)
			{
				isThrow = true;
			}
			Assert::IsTrue(isThrow);
		}

		TEST_METHOD(IMSITest)
		{
			experis::DataBase_mt db{};
			db.Insert(40052, 9720524410144, 9720528391949, experis::MOC, 567);
			db.Insert(40052, 9720528391949, 9720524410144, experis::MTC, 567);
			db.Insert(40052, 9720524410144, 9720523355899, experis::MOC, 900);
			db.Insert(40052, 9720523355899, 9720524410144, experis::MTC, 900);

			db.Insert(40052, 9720524410144, 9720528391949, experis::SMS_MO, 567);
			db.Insert(40052, 9720528391949, 9720524410144, experis::SMS_MT, 567);
			db.Insert(40052, 9720524410144, 9720523355899, experis::SMS_MO, 900);
			db.Insert(40052, 9720523355899, 9720524410144, experis::SMS_MT, 900);

			Assert::AreEqual(1467ULL, db.GetOperator(unsigned int(40052), experis::MOC));
			Assert::AreEqual(1467ULL, db.GetOperator(unsigned int(40052), experis::MTC));
			Assert::AreEqual(1467ULL, db.GetOperator(unsigned int(40052), experis::SMS_MO));
			Assert::AreEqual(1467ULL, db.GetOperator(unsigned int(40052), experis::SMS_MT));
		}

		TEST_METHOD(LinkesTest)
		{
			experis::DataBase_mt db{};
			db.Insert(40052, 9720524410144, 9720528391949, experis::MOC, 567);
			db.Insert(40052, 9720528391949, 9720524410144, experis::MTC, 567);
			db.Insert(40052, 9720524410144, 9720528391949, experis::MOC, 900);
			db.Insert(40052, 9720528391949, 9720524410144, experis::MTC, 900);

			db.Insert(40052, 9720524410144, 9720528391949, experis::SMS_MO, 567);
			db.Insert(40052, 9720528391949, 9720524410144, experis::SMS_MT, 567);
			db.Insert(40052, 9720524410144, 9720528391949, experis::SMS_MO, 900);
			db.Insert(40052, 9720528391949, 9720524410144, experis::SMS_MT, 900);

			Assert::AreEqual(1467ULL, db.GetPath(9720524410144, 9720528391949).first);
			Assert::AreEqual(1467ULL, db.GetPath(9720524410144, 9720528391949).second);
		}
	};
}
