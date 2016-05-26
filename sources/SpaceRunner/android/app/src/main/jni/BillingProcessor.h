#include <string>

#define LEVELS_ITEM_ID "world4"
#define MODELS_ITEM_ID "models"

class BillingProcessor
{
	static BillingProcessor* _instance;

	struct android_app* _state;

	BillingProcessor();
public:
	static BillingProcessor* Instance();

	//void BuyItem(std::string id);
	//bool IsItemBought(std::string id);

	void BuyEnergy(int packId);
	//bool IsItemBought(int packId);
	int GetBoughtItem();

	void Init(struct android_app* state);

	void LogInGoogle();
	void LogOutGoogle();
	bool IsLoggedGoogle();

	void UpdateScore(int score);
	void SyncAchievements();

	void ShowAchievements();
	void ShowLeaderboard();
	void ShowVideoAds();
	void ShareOnFacebook();
	void RateApp();
	void OpenLink(std::string link);
};
