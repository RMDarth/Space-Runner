#include "BillingProcessor.h"
#include "Config.h"
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

void BillingProcessor::BuyItem(std::string id)
{
#ifdef CHINA_SHOP
	return;
#endif
	// Get the android application's activity.
	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	jstring jStr = env->NewStringUTF(id.c_str());

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "BuyItem", "(Ljava/lang/String;)V");
	env->CallVoidMethod(activity->clazz, methodID, jStr);
	jvm->DetachCurrentThread();
}

void BillingProcessor::RateApp()
{

}

bool BillingProcessor::IsItemBought(std::string id)
{
#ifdef CHINA_SHOP
	return true;
#endif

	// Get the android application's activity.
	ANativeActivity* activity = _state->activity;
	JavaVM* jvm = _state->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	jstring jStr = env->NewStringUTF(id.c_str());

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return false;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "IsItemBought", "(Ljava/lang/String;)Z");
	int result = env->CallBooleanMethod(activity->clazz, methodID, jStr);
	jvm->DetachCurrentThread();

	return result;
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


void BillingProcessor::UpdateEndlessScore(int score)
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

	for (int i = 0; i < 16; i++)
	{
		if (CoreEngine::Config::Instance()->IsAchievementCompleted(i) && CoreEngine::Config::Instance()->GetAchievementData(i) == 0)
		{
			// Updating online status
			completed++;

			jclass clazz = env->GetObjectClass(activity->clazz);
			jmethodID methodID = env->GetMethodID(clazz, "UnlockAchievement", "(I)V");
			env->CallVoidMethod(activity->clazz, methodID, i);

		} else if (CoreEngine::Config::Instance()->GetAchievementData(i) > 0)
		{
			if (CoreEngine::Config::Instance()->IsAchievementCompleted(i))
				completed++;

			jclass clazz = env->GetObjectClass(activity->clazz);
			jmethodID methodID = env->GetMethodID(clazz, "UndateAchievement", "(II)V");
			env->CallVoidMethod(activity->clazz, methodID, i, CoreEngine::Config::Instance()->GetAchievementData(i) );
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

	// Updating platinum achievement
	if (completed >= 15 && !CoreEngine::Config::Instance()->IsAchievementCompleted(15))
	{
		CoreEngine::Config::Instance()->SetAchievementCompleted(15);

		jclass clazz = env->GetObjectClass(activity->clazz);
		jmethodID methodID = env->GetMethodID(clazz, "UndateAchievement", "(II)V");
		env->CallVoidMethod(activity->clazz, methodID, 15, 15);
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