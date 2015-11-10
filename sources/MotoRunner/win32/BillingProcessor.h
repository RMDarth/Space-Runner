#include <string>

#define LEVELS_ITEM_ID "world4"
#define MODELS_ITEM_ID "models"

class BillingProcessor
{
	static BillingProcessor* _instance;
	bool _logged;
	bool _buyItem;

	BillingProcessor();
public:
	static BillingProcessor* Instance();

	void BuyItem(std::string id);
	bool IsItemBought(std::string id);
	void Init();

	void LogInGoogle();
	void LogOutGoogle();
	bool IsLoggedGoogle();

	void ShowAchievements();
	void ShowLeaderboard();
	void UpdateEndlessScore(int score);
	void SyncAchievements();
	void RateApp();

	void ShareOnFacebook();
	void OpenLink(std::string link);
};