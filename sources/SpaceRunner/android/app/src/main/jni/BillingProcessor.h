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

	struct android_app* _state;

	BillingProcessor();
public:
	static BillingProcessor* Instance();

	void BuyEnergy(int packId);
	int GetBoughtItem();

	void Init(struct android_app* state);

	void LogInGoogle();
	void LogOutGoogle();
	bool IsLoggedGoogle();
	bool IsPlayServicesAvailable();

	void UpdateScore(int score);
	void SyncAchievements();

	void SyncScores();
	bool IsScoresUpdated();
	std::vector<ScoreItem> GetScores(bool weekly);

	void ShowAchievements();
	void ShowLeaderboard();
	bool ShowVideoAds();
	void ShowInterstitial();
	void RequestBackup();
	void RequestRestore();
	void ShareOnFacebook();
	void RateApp();
	void OpenLink(std::string link);

	void SetProgressDialogVisible(bool visible);

private:
	int GetLeaderboardSize(bool weekly);
	std::string GetLeaderboardName(int place, bool weekly);
	int GetLeaderboardScore(int place, bool weekly);
	int GetLeaderboardRank(int place, bool weekly);

	bool IsAchivementUnlocked(int id);
	void UnlockAchievement(int id);
	void UpdateAchievement(int id, int data);
};
