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

	void BuyItem(std::string id);
	bool IsItemBought(std::string id);
	void Init(struct android_app* state);

	void LogInGoogle();
	void LogOutGoogle();
	bool IsLoggedGoogle();

	void UpdateEndlessScore(int score);
	void SyncAchievements();

	void ShowAchievements();
	void ShowLeaderboard();
	void ShareOnFacebook();
	void RateApp();
	void OpenLink(std::string link);
};
