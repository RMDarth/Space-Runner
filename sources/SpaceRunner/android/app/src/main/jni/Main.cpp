#define OGRE_STATIC_GLES2
#define OGRE_STATIC_ParticleFX
#define OGRE_STATIC_OctreeSceneManager
#include "OgreStaticPluginLoader.h"

#include <EGL/egl.h>
#include <android/log.h>
#include "android_native_app_glue.h"

#include "Ogre.h"
#include "OgreRenderWindow.h"
#include "OgreStringConverter.h"
#include <OgreGLES2GpuProgramManager.h>
#include "OgreRectangle2D.h"

#include "RTShaderHelper.h"
#include "Android/OgreAndroidEGLWindow.h"
#include "Android/OgreAPKFileSystemArchive.h"
#include "Android/OgreAPKZipArchive.h"

#include "OgreApplication.h"
#include "RenderProcessor.h"
#include "SoundSystem.h"
#include "BillingProcessor.h"
#include "Game/Game.h"
#include "Game/Config.h"
#include "Input.h"
#include "FileSystem.h"

#include <ctime>
#include <ratio>
#include <chrono>

#define GRAVITY_TOP 48 
#define GRAVITY_BOTTOMRIGHT 80 | 5
#define GRAVITY_BOTTOM 80

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "SpaceRunner", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "SpaceRunner", __VA_ARGS__))


static Ogre::RenderWindow* gRenderWnd = NULL;
static Ogre::Root* gRoot = NULL;
static Ogre::StaticPluginLoader* gStaticPluginLoader = NULL;
AAssetManager* gAssetMgr = NULL; 
static Ogre::SceneManager* gSceneMgr = NULL;
static Ogre::ShaderGeneratorTechniqueResolverListener* gMatListener = NULL;
static Game* gGame = NULL;
static Config* gConfig = NULL;
static JavaVM *javaVM;
static Input* gInput = NULL;
struct android_app* gState;
bool isPause;
bool backPressed = false;
float backPressedTime = 0;
bool isClosing;
bool windowTerminated = false;
bool activityStopped = false;

using namespace std;
using namespace CoreEngine;

void SetAds(GameState::State state, GameState::State stateNew);

static Ogre::DataStreamPtr openAPKFile(const Ogre::String& fileName)
{
	Ogre::DataStreamPtr stream;
	AAsset* asset = AAssetManager_open(gAssetMgr, fileName.c_str(), AASSET_MODE_BUFFER);
	if(asset)
	{
		off_t length = AAsset_getLength(asset);
		void* membuf = OGRE_MALLOC(length, Ogre::MEMCATEGORY_GENERAL);
		memcpy(membuf, AAsset_getBuffer(asset), length);
		AAsset_close(asset);

		stream = Ogre::DataStreamPtr(new Ogre::MemoryDataStream(membuf, length, true, true));
	}
	return stream;
}

void showAds(int gravity)
{
	// Get the android application's activity.
	ANativeActivity* activity = gState->activity;
	JavaVM* jvm = gState->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return; 
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "showAdPopup", "(I)V");
	env->CallVoidMethod(activity->clazz, methodID, gravity);
	jvm->DetachCurrentThread();
}

int getRAMSize()
{
	// Get the android application's activity.
	ANativeActivity* activity = gState->activity;
	JavaVM* jvm = gState->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return 0;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "GetRAMSize", "()I");
	int ramSize = env->CallIntMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();

	return ramSize;
}

int getLanguageId()
{
	// Get the android application's activity.
	ANativeActivity* activity = gState->activity;
	JavaVM* jvm = gState->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return 0;
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "GetLanguageId", "()I");
	int langId = env->CallIntMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();

	return langId;
}

void hideAds()
{
	// Get the android application's activity.
	ANativeActivity* activity = gState->activity;
	JavaVM* jvm = gState->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	jint res = jvm->AttachCurrentThread(&env, NULL);

	if (res == JNI_ERR)
	{
		// Failed to retrieve JVM environment
		return; 
	}

	jclass clazz = env->GetObjectClass(activity->clazz);
	jmethodID methodID = env->GetMethodID(clazz, "hideAdPopup", "()V");
	env->CallVoidMethod(activity->clazz, methodID);
	jvm->DetachCurrentThread();
}

void InitLoadingScreen()
{
	std::string image = "loadlogo.jpg";
	// Create background material
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(image, "General");
	material->getTechnique(0)->getPass(0)->setVertexProgram("simpletex_vs");
	material->getTechnique(0)->getPass(0)->setFragmentProgram("simpletex_ps");
	Ogre::TextureUnitState* state;
	state = material->getTechnique(0)->getPass(0)->createTextureUnitState(image);

	state->setTextureNameAlias("diffuseMap");
	//material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);

	// Create background rectangle covering the whole screen
	Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);
	rect->setCorners(-1.0, 1.0, 1.0, -1);
	rect->setMaterial(image);

	// Render the background before everything else
	rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

	// Use infinite AAB to always stay visible
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	rect->setBoundingBox(aabInf);

	// Attach background to the scene
	Ogre::SceneNode* node = gSceneMgr->getRootSceneNode()->createChildSceneNode("Background");
	node->attachObject(rect);
}

static void setupScene(int dpi)
{
    LOGW("Setup scene started");
	RenderProcessor::Instance()->Init(dpi);
	gSceneMgr = RenderProcessor::Instance()->GetSceneManager();
	gRoot->renderOneFrame();
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/Materials", "APKFileSystem", "Loading");
    Ogre::LogManager::getSingleton().logMessage("TestTest");
    LOGW("Load starting resources");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Loading");
	InitLoadingScreen();
	gRoot->renderOneFrame();

	Ogre::ConfigFile cf;

	//if (getRAMSize() < 700)
		cf.load(openAPKFile("resources.cfg"));
	//else
		//cf.load(openAPKFile("resources.cfg"));

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements())
	{
		Ogre::String sec, type, arch;
		sec = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;

		for (i = settings->begin(); i != settings->end(); i++)
		{
			type = i->first;
			arch = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}

	Ogre::ResourceBackgroundQueue &rbq = Ogre::ResourceBackgroundQueue::getSingleton();
	auto ticket = rbq.initialiseResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	int ident, events;
	struct android_poll_source* source;

	while (!rbq.isProcessComplete(ticket))
	{	
		while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
		{
			if (source != NULL)
				source->process(gState, source);

			if (gState->destroyRequested != 0)
				return;
		}

		try{
		if (!isPause)
			gRoot->renderOneFrame();
		} catch (Ogre::Exception)
		{
		}

	}

	Ogre::RTShader::ShaderGenerator::initialize();
	Ogre::RTShader::ShaderGenerator::getSingletonPtr()->setTargetLanguage("glsles");
	gMatListener = new Ogre::ShaderGeneratorTechniqueResolverListener();
	Ogre::MaterialManager::getSingleton().addListener(gMatListener);

	RenderProcessor::Instance()->PostInit();

	Ogre::RTShader::ShaderGenerator::getSingletonPtr()->addSceneManager(gSceneMgr);
	Ogre::RTShader::ShaderGenerator::getSingletonPtr()->invalidateScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
}

static int32_t handleInput(struct android_app* app, AInputEvent* event) 
{
	if (windowTerminated || isPause)
			return 0;

	if (AKeyEvent_getKeyCode(event) == AKEYCODE_BACK) {
		// actions on back key
		if (gInput != NULL && !backPressed)
		{
			if (gGame != NULL && gGame->GetState() == GameState::MainMenu)
			{
				isClosing = true;
				return 1;
			}

			GameState::State state, stateNew;
			if (gGame != NULL)
				state = gGame->GetState();
			gInput->HandleInput(OIS::KC_ESCAPE);
			if (gGame != NULL)
			{
				stateNew = gGame->GetState();
				SetAds(state, stateNew);
			}

			backPressed = true;
			backPressedTime = 0.3;
		}
		return 1; 
	} else {
		backPressed = false;
	}

	if (gInput != NULL)
	{
		GameState::State state, stateNew;
		if (gGame != NULL)
			state = gGame->GetState();
		gInput->HandleInput(event);
		if (gGame != NULL)
		{
			stateNew = gGame->GetState();

			SetAds(state, stateNew);
		}
	}

	return 0;
}

void SetAds(GameState::State state, GameState::State stateNew)
{
	if (state != stateNew)
	{
		if (stateNew == GameState::Level || stateNew == GameState::Store)
			hideAds();
		else if (stateNew == GameState::Pause || stateNew == GameState::SaveMe)
			showAds(GRAVITY_BOTTOM);
		else if (stateNew == GameState::ShipSelect || stateNew == GameState::Highscores)
		{
			hideAds();
			showAds(GRAVITY_BOTTOMRIGHT);
		}
		else if (state == GameState::ShipSelect)
		{
			hideAds();
			showAds(GRAVITY_TOP);
		}
		else if (stateNew == GameState::Storyboard)
		{
			hideAds();
			showAds(GRAVITY_TOP);
		}
		else if (stateNew == GameState::Score)
		{
			hideAds();
			showAds(GRAVITY_BOTTOM);
		}
		else if (stateNew == GameState::Credits)
		{
			hideAds();
			showAds(GRAVITY_BOTTOMRIGHT);
		}
		else if (state == GameState::Credits)
		{
			hideAds();
			showAds(GRAVITY_TOP);
		}
		else if (stateNew == GameState::Challenges)
		{
			hideAds();
			showAds(GRAVITY_BOTTOMRIGHT);
		}
		else {
			hideAds();
			showAds(GRAVITY_TOP);
		}
	}
}

static void handleCmd(struct android_app* app, int32_t cmd)
{
	switch (cmd) 
	{
	case APP_CMD_SAVE_STATE:
		break;

	case APP_CMD_STOP:
		activityStopped = true;
		break;

	case APP_CMD_PAUSE:
		isPause = true;
		if (gGame && gGame->GetState() == GameState::Level)
		{
			gGame->ChangeState(GameState::Pause);
		}
		if (gConfig && gConfig->IsMusicEnabled())
			SoundSystem::Instance()->StopBackgroundMusic();
		break;

	case APP_CMD_RESUME:
		if (!windowTerminated && !activityStopped)
		{
			isPause = false;
			if (gConfig && gConfig->IsMusicEnabled())
				SoundSystem::Instance()->StartBackgroundMusic();
		}
		break;

	case APP_CMD_START:
		activityStopped = false;
		break;

	case APP_CMD_INIT_WINDOW:
		if(app->window && gRoot)
		{
			AConfiguration* config = AConfiguration_new();
			AConfiguration_fromAssetManager(config, app->activity->assetManager);
				
			if(!gRenderWnd)
			{
				gAssetMgr = app->activity->assetManager;
				AssetManagerRef::gAssetMgr = gAssetMgr;

				// init internal folder
				AssetManagerRef::internalPath = app->activity->internalDataPath + std::string("/");
				struct stat sb;
				int32_t res = stat(AssetManagerRef::internalPath.c_str(), &sb);
				if (! (0 == res && sb.st_mode & S_IFDIR) )
				{
					res = mkdir(AssetManagerRef::internalPath.c_str(), 0770);
				}

				Ogre::ArchiveManager::getSingleton().addArchiveFactory( new Ogre::APKFileSystemArchiveFactory(app->activity->assetManager) );
				Ogre::ArchiveManager::getSingleton().addArchiveFactory( new Ogre::APKZipArchiveFactory(app->activity->assetManager) );

				Ogre::NameValuePairList opt;
				opt["externalWindowHandle"] = Ogre::StringConverter::toString((int)app->window);
				opt["androidConfig"] = Ogre::StringConverter::toString((int)config);
                opt["preserveContext"] = "true";

				gRenderWnd = gRoot->createRenderWindow("SpaceRush", 0, 0, false, &opt);
				OgreApplication::Instance()->Init(gRoot, gRenderWnd);

				setupScene(AConfiguration_getDensity(config));

				BillingProcessor::Instance()->Init(app);
				gConfig = Config::Instance();
				gConfig->Load();
				bool soundEnabled = SoundSystem::Instance()->Init(gAssetMgr, app);
				if (!soundEnabled)
					gConfig->SetSoundEnabled(false);

				gConfig->SetLanguage(getLanguageId());

				//if (getRAMSize() < 700 && gConfig->IsFirstRun())
				//	gConfig->SetSelectedQuality(1);

				gGame = Game::Instance();

				gInput = new Input(gRenderWnd);
				gInput->AddListener((IInputListener*)gGame);

				showAds(GRAVITY_TOP);
			}
			else
			{
                //BillingProcessor::Instance()->SetProgressDialogVisible(true);

				if (!windowTerminated)
					static_cast<Ogre::AndroidEGLWindow*>(gRenderWnd)->_destroyInternalResources();

				static_cast<Ogre::AndroidEGLWindow*>(gRenderWnd)->_createInternalResources(app->window, config);

				if (gConfig && gConfig->IsMusicEnabled())
					SoundSystem::Instance()->StartBackgroundMusic();

				if (Config::Instance()->IsSignedGoogle())
				{
					BillingProcessor::Instance()->LogInGoogle();
				}

                //BillingProcessor::Instance()->SetProgressDialogVisible(false);
			}
			AConfiguration_delete(config);
		}
		windowTerminated = false;
		isPause = false;
		OgreApplication::Instance()->WindowResized();
		break;

	case APP_CMD_TERM_WINDOW:
		if(gRoot && gRenderWnd && !windowTerminated)
			static_cast<Ogre::AndroidEGLWindow*>(gRenderWnd)->_destroyInternalResources();
		windowTerminated = true;

		break;
	case APP_CMD_GAINED_FOCUS:
		break;
	case APP_CMD_LOST_FOCUS:
		break;
	default:
		break;
	}
}

static double now_ms(void) {

    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;

}

void android_main(struct android_app* state)
{
	app_dummy();
	gState = state;
	isPause = false;
	isClosing = false;
	GameState::State oldState = GameState::MainMenu;
	GameState::State newState;

	if(gRoot == NULL)
	{
		//Ogre::LogManager * lm = new Ogre::LogManager();
		//lm->createLog("Ogre.log", true, false, true);

		gRoot = new Ogre::Root();
#ifdef OGRE_STATIC_LIB
		gStaticPluginLoader = new Ogre::StaticPluginLoader();
		gStaticPluginLoader->load();
#endif
		gRoot->setRenderSystem(gRoot->getAvailableRenderers().at(0));
        LOGW(gRoot->getAvailableRenderers().at(0)->getName().c_str());
		gRoot->initialise(false);

		Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_LOW);
	}

	state->onAppCmd = &handleCmd;
	state->onInputEvent = &handleInput;

	int ident, events;
	struct android_poll_source* source;

	double oldtime = now_ms();
	double time;
	float deltaTime;

	while (!OgreApplication::Instance()->IsExit())
	{
		while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
		{
			if (source != NULL)
				source->process(state, source);

			if (state->destroyRequested != 0)
			{
				state->activity->vm->DetachCurrentThread();
				exit(0);
				return;
			}
		}

		if(isPause == false && gRenderWnd != NULL && gRenderWnd->isActive())
		{
			time = now_ms();
			double time_span = (time - oldtime);
			deltaTime = time_span / 1000.0f;

			if (deltaTime > 0.2f)
				deltaTime = 0.2f;

			RenderProcessor::Instance()->Update(deltaTime);
			gInput->Update(deltaTime);
			gGame->Update(deltaTime);
			newState = gGame->GetState();

			if (newState == GameState::Score)
			{
				showAds(GRAVITY_BOTTOM);
			} else if (oldState != newState && newState == GameState::Level)
			{
				hideAds();
			}

			try{
				OgreApplication::Instance()->LoopStep(deltaTime);
			} catch (Ogre::Exception)
			{

			}

			oldState = newState;
			oldtime = time;

			if (backPressedTime > 0)
				backPressedTime -= deltaTime;
			else
				backPressed = false;
		}

		if (isClosing)
		{
			ANativeActivity_finish(state->activity);
		}

	}
}
