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

bool BillingProcessor::ShowVideoAds()
{
	return true;
}

void BillingProcessor::RequestBackup()
{

}

void BillingProcessor::RequestRestore()
{

}

std::vector<ScoreItem> BillingProcessor::GetScores(bool weekly)
{
	std::vector<ScoreItem> result;
	if (weekly)
	{
		result.push_back({"Chuck Norris", 10000, 1});
		result.push_back({"Vladimir Putin", 8000, 2});
		result.push_back({"Fduch", 7123, 3});
		result.push_back({"Barak Obama", 4321, 4});
		result.push_back({"Elvis Presley", 1200, 5});
		result.push_back({"Fduch", 7123, 3});
	} else {
		result.push_back({"Chuck Norris", 10000, 1});
		result.push_back({"Vladimir Putin", 8000, 2});
		result.push_back({"Fduch", 875, 3});
	}
	return result;
}

void BillingProcessor::SyncScores()
{

}

bool BillingProcessor::IsScoresUpdated()
{
	return false;
}

void BillingProcessor::ShowInterstitial()
{

}

bool BillingProcessor::IsPlayServicesAvailable()
{
	return true;
}
















