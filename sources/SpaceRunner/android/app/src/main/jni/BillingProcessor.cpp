#include "BillingProcessor.h"
#include "Game/Config.h"
#include <android_native_app_glue.h>

BillingProcessor* BillingProcessor::_instance = nullptr;

BillingProcessor::BillingProcessor()
{

}

BillingProcessor* BillingProcessor::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new BillingProcessor();
	}
	return _instance;
}

void BillingProcessor::Init(struct android_app* state)
{
	_state = state;
}


void BillingProcessor::RateApp()
{

}


void BillingProcessor::LogInGoogle()
{
#ifdef CHINA_SHOP
	return;
#endif

	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "LogInGoogle", "()V");
	env->CallVoidMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();
}

void BillingProcessor::LogOutGoogle()
{
#ifdef CHINA_SHOP
	return;
#endif

	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "LogOutGoogle", "()V");
	env->CallVoidMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();
}

bool BillingProcessor::IsLoggedGoogle()
{
#ifdef CHINA_SHOP
	return false;
#endif

	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return false;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "IsGoogleLogged", "()Z");
	int result = env->CallBooleanMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();

	return result;
}

void BillingProcessor::ShowAchievements()
{
#ifdef CHINA_SHOP
	return;
#endif

	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "ShowAchievements", "()V");
	env->CallVoidMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();
}

void BillingProcessor::ShowLeaderboard()
{
#ifdef CHINA_SHOP
	return;
#endif

	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "ShowLeaderboards", "()V");
	env->CallVoidMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();
}

bool BillingProcessor::ShowVideoAds()
{
	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return false;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "ShowVideoAd", "()Z");
	bool result = env->CallBooleanMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();

	return result;
}


void BillingProcessor::UpdateScore(int score)
{
#ifdef CHINA_SHOP
	return;
#endif

	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "UpdateScore", "(I)V");
	env->CallVoidMethod(activity->clazz, methodID, score);
	jvm->DetachCurrentThread();
}

void BillingProcessor::RequestBackup()
{
	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "RequestBackup", "()V");
	env->CallVoidMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();
}

void BillingProcessor::RequestRestore()
{
    ANativeActivity* activity = _state->activity;
    JavaVM* jvm = _state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = jvm->AttachCurrentThread(&env, NULL);

    if (res == JNI_ERR)
    {
        // Failed to retrieve JVM environment
        return;
    }

    jclass clazz = env->GetObjectClass(activity->clazz);
    jmethodID methodID = env->GetMethodID(clazz, "RequestRestore", "()V");
    env->CallVoidMethod(activity->clazz, methodID);
    jvm->DetachCurrentThread();
}

void BillingProcessor::SetProgressDialogVisible(bool visible) {
	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	if (visible)
	{
		jmethodID methodID = env->GetMethodID(clazz, "showLoadingDialog", "()V");
		env->CallVoidMethod(activity->clazz, methodID);
	} else {
		jmethodID methodID = env->GetMethodID(clazz, "hideLoadingDialog", "()V");
		env->CallVoidMethod(activity->clazz, methodID);
	}
	jvm->DetachCurrentThread();
}



void BillingProcessor::SyncAchievements()
{
#ifdef CHINA_SHOP
	return;
#endif

	int completed = 0;

	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	for (int i = 0; i < 15; i++)
	{
		if (CoreEngine::Config::Instance()->IsAchievementCompleted(i))
		{
			// Updating online status
			completed++;

			jclass clazz = env->GetObjectClass(activity->clazz);
			jmethodID methodID = env->GetMethodID(clazz, "UnlockAchievement", "(I)V");
			env->CallVoidMethod(activity->clazz, methodID, i);
		}

		if (!CoreEngine::Config::Instance()->IsAchievementCompleted(i))
		{
			/// Updating offline cache
			jclass clazz = env->GetObjectClass(activity->clazz);
			jmethodID methodID = env->GetMethodID(clazz, "IsAchievementUnlocked", "(I)Z");
			bool unlocked = env->CallBooleanMethod(activity->clazz, methodID, i );

			if (unlocked)
			{
				CoreEngine::Config::Instance()->SetAchievementCompleted(i);
				completed++;
			}
		}
	}

    if (completed > 0)
    {
        jclass clazz = env->GetObjectClass(activity->clazz);
        jmethodID methodID = env->GetMethodID(clazz, "UndateAchievement", "(II)V");
        env->CallVoidMethod(activity->clazz, methodID, 15, completed);
    }

	// Updating platinum achievement
	if (completed >= 15 && !CoreEngine::Config::Instance()->IsAchievementCompleted(15))
	{
		CoreEngine::Config::Instance()->SetAchievementCompleted(15);
	}

	jvm->DetachCurrentThread();
}

void BillingProcessor::ShareOnFacebook()
{
	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "ShareOnFacebook", "()V");
	env->CallVoidMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();
}

void BillingProcessor::OpenLink(std::string link)
{
	// Get the android application's activity.
	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	jstring jStr = env->NewStringUTF(link.c_str());

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "OpenLink", "(Ljava/lang/String;)V");
	env->CallVoidMethod(activity->clazz, methodID, jStr);
	jvm->DetachCurrentThread();
}

void BillingProcessor::BuyEnergy(int packId)
{
	// Get the android application's activity.
	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	//jstring jStr = env->NewStringUTF(id.c_str());

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "BuyEnergy", "(I)V");
	env->CallVoidMethod(activity->clazz, methodID, packId);
	jvm->DetachCurrentThread();
}

int BillingProcessor::GetBoughtItem()
{
	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return false;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "ReturnLastBoughtItem", "()I");
	int result = env->CallIntMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();

	return result;
}


std::vector<ScoreItem> BillingProcessor::GetScores(bool weekly)
{
	std::vector<ScoreItem> scoreItems;

    int size = GetLeaderboardSize(weekly);
    if (size == 5)
        size++;

    for (auto i = 0; i < size; i++)
    {
        ScoreItem item;
        item.name = GetLeaderboardName(i, weekly);
        item.score = GetLeaderboardScore(i, weekly);
        item.place = GetLeaderboardRank(i, weekly);
        scoreItems.push_back(item);
    }

    return scoreItems;
}

void BillingProcessor::SyncScores()
{
    ANativeActivity* activity = _state->activity;
    JavaVM* jvm = _state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = jvm->AttachCurrentThread(&env, NULL);

    if (res == JNI_ERR)
    {
        // Failed to retrieve JVM environment
        return;
    }

    jclass clazz = env->GetObjectClass(activity->clazz);
    jmethodID methodID = env->GetMethodID(clazz, "GetTopScores", "()V");
    env->CallVoidMethod(activity->clazz, methodID);
    jvm->DetachCurrentThread();
}

bool BillingProcessor::IsScoresUpdated()
{
    ANativeActivity* activity = _state->activity;
    JavaVM* jvm = _state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = jvm->AttachCurrentThread(&env, NULL);

    if (res == JNI_ERR)
    {
        // Failed to retrieve JVM environment
        return false;
    }

    jclass clazz = env->GetObjectClass(activity->clazz);
    jmethodID methodID = env->GetMethodID(clazz, "IsLeaderboardsUpdated", "()Z");
    bool result = env->CallBooleanMethod(activity->clazz, methodID);
    jvm->DetachCurrentThread();

    return result;
}

int BillingProcessor::GetLeaderboardSize(bool weekly)
{
    ANativeActivity* activity = _state->activity;
    JavaVM* jvm = _state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = jvm->AttachCurrentThread(&env, NULL);

    if (res == JNI_ERR)
        return false;

    /// Updating offline cache
    jclass clazz = env->GetObjectClass(activity->clazz);
    jmethodID methodID = env->GetMethodID(clazz, "GetLeaderboardSize", "(Z)I");
    int size = env->CallIntMethod(activity->clazz, methodID, weekly);
    jvm->DetachCurrentThread();

    return size;
}

std::string BillingProcessor::GetLeaderboardName(int place, bool weekly)
{
    ANativeActivity* activity = _state->activity;
    JavaVM* jvm = _state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = jvm->AttachCurrentThread(&env, NULL);

    if (res == JNI_ERR)
        return false;

    /// Updating offline cache
    jclass clazz = env->GetObjectClass(activity->clazz);
    jmethodID methodID = env->GetMethodID(clazz, "GetLeaderboardName", "(IZ)Ljava/lang/String;");
    jstring text = (jstring)env->CallObjectMethod(activity->clazz, methodID, place, weekly);

    // convert the Java String to use it in C
    const char * str = env->GetStringUTFChars(text, 0);
    std::string result(str);
    env->ReleaseStringUTFChars(text, str);
    jvm->DetachCurrentThread();
    return result;
}

int BillingProcessor::GetLeaderboardScore(int place, bool weekly)
{
    ANativeActivity* activity = _state->activity;
    JavaVM* jvm = _state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = jvm->AttachCurrentThread(&env, NULL);

    if (res == JNI_ERR)
        return false;

    /// Updating offline cache
    jclass clazz = env->GetObjectClass(activity->clazz);
    jmethodID methodID = env->GetMethodID(clazz, "GetLeaderboardScore", "(IZ)I");
    int score = env->CallIntMethod(activity->clazz, methodID, place, weekly);
    jvm->DetachCurrentThread();

    return score;
}

int BillingProcessor::GetLeaderboardRank(int place, bool weekly)
{
    ANativeActivity* activity = _state->activity;
    JavaVM* jvm = _state->activity->vm;
    JNIEnv* env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    jint res = jvm->AttachCurrentThread(&env, NULL);

    if (res == JNI_ERR)
        return false;

    /// Updating offline cache
    jclass clazz = env->GetObjectClass(activity->clazz);
    jmethodID methodID = env->GetMethodID(clazz, "GetLeaderboardRank", "(IZ)I");
    int rank = env->CallIntMethod(activity->clazz, methodID, place, weekly);
    jvm->DetachCurrentThread();

    return rank;
}