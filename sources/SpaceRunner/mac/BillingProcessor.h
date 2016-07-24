#include <string>
#include <vector>

struct ScoreItem
{
    std::string name;
    int score;
    int place;
};

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
	bool IsPlayServicesAvailable();

	void RequestBackup();
	void RequestRestore();

    void SyncScores();
    bool IsScoresUpdated();
    std::vector<ScoreItem> GetScores(bool weekly);

	void ShowAchievements();
	void ShowLeaderboard();
	bool ShowVideoAds();
	void ShowInterstitial();
	void UpdateScore(int score);
	void SyncAchievements();
	void RateApp();

	void ShareOnFacebook();
	void OpenLink(std::string link);
};