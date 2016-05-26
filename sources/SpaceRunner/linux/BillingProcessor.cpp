#include "BillingProcessor.h"

BillingProcessor* BillingProcessor::_instance = nullptr;

BillingProcessor::BillingProcessor()
{
	_logged = false;
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

void BillingProcessor::BuyEnergy(int packId)
{
	_boughtItems.push_back(packId);
}

int BillingProcessor::GetBoughtItem()
{
    if (_boughtItems.empty())
        return -1;
	auto boughtItem = _boughtItems.back();
	_boughtItems.pop_back();
	return boughtItem;
}

void BillingProcessor::ShowVideoAds()
{

}





