#include <string>
#include <vector>

class BillingProcessor
{
	static BillingProcessor* _instance;
	bool _logged;

	std::vector<int> _boughtItems;

	BillingProcessor();
public:
	static BillingProcessor* Instance();

	void BuyEnergy(int packId);
	int GetBoughtItem();

	void Init();

	void LogInGoogle();
	void LogOutGoogle();
	bool IsLoggedGoogle();

	void ShowAchievements();
	void ShowLeaderboard();
	void ShowVideoAds();
	void UpdateScore(int score);
	void SyncAchievements();
	void RateApp();

	void ShareOnFacebook();
	void OpenLink(std::string link);
};