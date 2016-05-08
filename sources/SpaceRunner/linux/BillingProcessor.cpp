#include "BillingProcessor.h"

BillingProcessor* BillingProcessor::_instance = nullptr;

BillingProcessor::BillingProcessor()
{
	_logged = false;
	_buyItem = true;
}

BillingProcessor* BillingProcessor::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new BillingProcessor();
	}
	return _instance;
}

void BillingProcessor::Init()
{

}

void BillingProcessor::BuyItem(std::string id)
{
	_buyItem = !_buyItem;
}

bool BillingProcessor::IsItemBought(std::string id)
{
	return _buyItem;
}

void BillingProcessor::LogInGoogle()
{
	_logged = true;
}

void BillingProcessor::LogOutGoogle()
{
	_logged = false;
}

bool BillingProcessor::IsLoggedGoogle()
{
	return _logged;
}

void BillingProcessor::ShareOnFacebook()
{

}

void BillingProcessor::ShowAchievements()
{

}

void BillingProcessor::ShowLeaderboard()
{

}

void BillingProcessor::UpdateScore(int score)
{

}

void BillingProcessor::SyncAchievements()
{

}

void BillingProcessor::OpenLink(std::string link)
{

}

void BillingProcessor::RateApp()
{

}

bool BillingProcessor::BuyEnergy(int packId)
{
	return true;
}

