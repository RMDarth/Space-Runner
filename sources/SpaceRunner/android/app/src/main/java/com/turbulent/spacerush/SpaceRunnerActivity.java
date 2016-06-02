package com.turbulent.spacerush;
import java.io.IOException;
import java.security.SecureRandom;
import java.security.acl.Owner;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Set;

//import org.codechimp.apprater.AppRater;

import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.app.NativeActivity;
import android.app.ProgressDialog;
import android.app.backup.BackupManager;
import android.app.backup.RestoreObserver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.widget.PopupWindow;
import android.widget.Toast;
import android.media.AudioManager;
import android.media.SoundPool;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings.Secure;
import android.util.Log;
import android.util.TypedValue;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.view.ViewGroup.LayoutParams;
import android.view.ViewGroup.MarginLayoutParams;

import com.vungle.publisher.AdConfig;
import com.vungle.publisher.VunglePub;

/*import com.facebook.*;
import com.facebook.model.*;
import com.facebook.widget.FacebookDialog;*/

import com.google.android.gms.ads.*;
import com.google.android.gms.ads.doubleclick.PublisherAdRequest;
import com.google.android.gms.ads.doubleclick.PublisherInterstitialAd;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.PendingResult;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesStatusCodes;
import com.google.android.gms.games.achievement.Achievement;
import com.google.android.gms.games.achievement.AchievementBuffer;
import com.google.android.gms.games.achievement.Achievements.LoadAchievementsResult;
import com.google.android.gms.games.leaderboard.LeaderboardBuffer;
import com.google.android.gms.games.leaderboard.LeaderboardScore;
import com.google.android.gms.games.leaderboard.LeaderboardScoreBuffer;
import com.google.android.gms.games.leaderboard.LeaderboardVariant;
import com.google.android.gms.games.leaderboard.Leaderboards;
import com.turbulent.spacerush.Consts;
import com.turbulent.spacerush.Consts.PurchaseState;
import com.turbulent.spacerush.Consts.ResponseCode;
import com.turbulent.spacerush.utils.IabBroadcastReceiver;
import com.turbulent.spacerush.utils.IabHelper;
import com.turbulent.spacerush.utils.IabResult;
import com.turbulent.spacerush.utils.Inventory;
import com.turbulent.spacerush.utils.Purchase;


public class SpaceRunnerActivity extends NativeActivity implements GameHelper.GameHelperListener, ResultCallback<LoadAchievementsResult>, IabBroadcastReceiver.IabBroadcastListener
{
    static {
        System.loadLibrary("SpaceRunner");
    }

    private static final String TAG = "SpaceRush";

    private static final int REQUEST_ACHIEVEMENTS = 12633536;
    private static final String LEADERBOARD_ID = "CgkIjqW0l9sVEAIQAQ";
    private static final int REQUEST_LEADERBOARD = 2134231;

    private String[] achievementList =
            {
                    "CgkIjqW0l9sVEAIQAg",
                    "CgkIjqW0l9sVEAIQAw",
                    "CgkIjqW0l9sVEAIQBA",
                    "CgkIjqW0l9sVEAIQBQ",
                    "CgkIjqW0l9sVEAIQBg",
                    "CgkIjqW0l9sVEAIQBw",
                    "CgkIjqW0l9sVEAIQCA",
                    "CgkIjqW0l9sVEAIQCQ",
                    "CgkIjqW0l9sVEAIQCg",
                    "CgkIjqW0l9sVEAIQCw",
                    "CgkIjqW0l9sVEAIQDA",
                    "CgkIjqW0l9sVEAIQDQ",
                    "CgkIjqW0l9sVEAIQDg",
                    "CgkIjqW0l9sVEAIQDw",
                    "CgkIjqW0l9sVEAIQEA",
                    "CgkIjqW0l9sVEAIQEQ"
            };

    private String[] skuList =
            {
                    "energypack1",
                    "energypack2",
                    "energypack3",
                    "energypack4",
                    "energypack5",
                    "resurrectitem"
            };

    private boolean[] _achievementUnlockedStatus = new boolean[20];

    class ScoreItem
    {
        public String name;
        public int score;
        public int place;
    }

    private ScoreItem[] _weeklyScores = new ScoreItem[5];
    private int _weeklyScoresSize = 0;
    private ScoreItem[] _allTimeScores = new ScoreItem[5];
    private int _allTimeScoresSize = 0;
    private ScoreItem _myScoreWeekly = null;
    private ScoreItem _myScoreAlltime = null;
    private boolean _scoresUpdated = false;

    private SoundPool mSoundPool;

    BackupManager backupManager;

    //facebook
    //private UiLifecycleHelper uiHelper;

    // Loading dialog
    ProgressDialog progressDialog;

    // Ads
    AdView adView;
    PopupWindow popUp;
    SpaceRunnerActivity _activity;
    LinearLayout layout;
    LinearLayout mainLayout;
    boolean adsinited = false;
    boolean adshidden = false;

    // Interstitial
    PublisherInterstitialAd interstitialAd;

    // VideoAd
    final VunglePub vunglePub = VunglePub.getInstance();

    boolean googleSignedFinished = false;
    boolean achievementsSynced = false;


    /// Google services

    // The game helper object. This class is mainly a wrapper around this object.
    protected GameHelper mHelper;

    // We expose these constants here because we don't want users of this class
    // to have to know about GameHelper at all.
    public static final int CLIENT_GAMES = GameHelper.CLIENT_GAMES;

    // Requested clients. By default, that's just the games client.
    protected int mRequestedClients = CLIENT_GAMES;


    /// InApp purchases
    // The helper object
    IabHelper mPurchaseHelper;

    // Provides purchase notification while this app is running
    IabBroadcastReceiver mBroadcastReceiver;

    private ArrayList<String> mOwnedItems = new ArrayList<String>();
    static final int RC_REQUEST = 43156;


    /**
     * Sets the requested clients. The preferred way to set the requested clients is
     * via the constructor, but this method is available if for some reason your code
     * cannot do this in the constructor. This must be called before onCreate or getGameHelper()
     * in order to have any effect. If called after onCreate()/getGameHelper(), this method
     * is a no-op.
     *
     * @param requestedClients A combination of the flags CLIENT_GAMES, CLIENT_PLUS
     *         and CLIENT_APPSTATE, or CLIENT_ALL to request all available clients.
     */
    protected void setRequestedClients(int requestedClients) {
        mRequestedClients = requestedClients;
    }

    public GameHelper getGameHelper() {
        if (mHelper == null) {
            mHelper = new GameHelper(this, mRequestedClients);
            mHelper.enableDebugLog(Consts.DEBUG);
        }
        return mHelper;
    }


    public int GetRAMSize()
    {
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.JELLY_BEAN) {
            // only for gingerbread and newer versions

            ActivityManager actManager = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
            MemoryInfo memInfo = new ActivityManager.MemoryInfo();
            actManager.getMemoryInfo(memInfo);
            int totalMemory = (int)(memInfo.totalMem / 1048576);
            return totalMemory;
        }
        return 0;
    }

    public int GetLanguageId()
    {
        String language = Locale.getDefault().getLanguage();

        if (language.equals("ru"))
            return 1;

        if (language.equals("de"))
            return 2;

        if (language.equals("fr"))
            return 3;

        if (language.equals("pt"))
            return 4;

        if (language.equals("es"))
            return 5;

        if (language.equals("it"))
            return 6;

        if (language.equals("zh"))
            return 7;

        if (language.equals("ja"))
            return 8;

        return 0;
    }


    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        backupManager = new BackupManager(this);
        //facebook
        //uiHelper = new UiLifecycleHelper(this, null);
       // uiHelper.onCreate(savedInstanceState);

        // rate
        //AppRater.app_launched(this);
        //AppRater.showRateDialog(this, null);


        // sound
        mSoundPool = new SoundPool(5, AudioManager.STREAM_MUSIC, 0);
        //mSoundPool.setOnLoadCompleteListener(this);

        /// google services
        if (mHelper == null) {
            getGameHelper();
        }
        mHelper.setup(this);


        /// Ads
        // Make your custom init here
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        _activity = this;
        // Create our ad view here
        adView = new AdView(this);
        adView.setAdSize(AdSize.BANNER);
        //adView.setAdUnitId("ca-app-pub-4708479882450965/2339704886");
        adView.setAdUnitId("ca-app-pub-3940256099942544/6300978111");

        MarginLayoutParams params = new MarginLayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        adView.setLayoutParams(params);

        // Interstitial ads
        interstitialAd = new PublisherInterstitialAd(this);
        interstitialAd.setAdUnitId("ca-app-pub-3940256099942544/1033173712");

        interstitialAd.setAdListener(new AdListener() {
            @Override
            public void onAdClosed() {
                requestNewInterstitial();
            }
        });
        requestNewInterstitial();

        // Video ads
        final String app_id = "Test_Android";
        vunglePub.init(this, app_id);
        final AdConfig globalAdConfig = vunglePub.getGlobalAdConfig();
        globalAdConfig.setTransitionAnimationEnabled(true);

        // inapp
        String base64EncodedPublicKey = SecurityConsts.PUBLIC_KEY;
        mPurchaseHelper = new IabHelper(this, base64EncodedPublicKey);
        mPurchaseHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            public void onIabSetupFinished(IabResult result) {

                if (!result.isSuccess()) {
                    // Oh noes, there was a problem.
                    Log.i(TAG, "Failed to initialize billing system.");
                    return;
                }

                // Have we been disposed of in the meantime? If so, quit.
                if (mPurchaseHelper == null) return;

                mBroadcastReceiver = new IabBroadcastReceiver(SpaceRunnerActivity.this);
                IntentFilter broadcastFilter = new IntentFilter(IabBroadcastReceiver.ACTION);
                registerReceiver(mBroadcastReceiver, broadcastFilter);

                try {
                    mPurchaseHelper.queryInventoryAsync(mGotInventoryListener);
                } catch (IabHelper.IabAsyncInProgressException e) {
                    Log.e(TAG, "Error querying inventory. Another async operation in progress.");
                }

            }
        });

        RequestBackup();
    }

    // Listener that's called when we finish querying the items and subscriptions we own
    IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
            // Have we been disposed of in the meantime? If so, quit.
            if (mPurchaseHelper == null) return;

            // Is it a failure?
            if (result.isFailure()) {
                Log.i(TAG, "[Billing] Failed to query inventory: " + result);
                return;
            }

            for (String sku : skuList)
            {
                Purchase energyPurchase = inventory.getPurchase(sku);
                if (energyPurchase != null)// && verifyDeveloperPayload(energyPurchase))
                {
                    Log.d(TAG, "We have energy. Consuming it.");
                    try {
                        mPurchaseHelper.consumeAsync(inventory.getPurchase(sku), mConsumeFinishedListener);
                    } catch (IabHelper.IabAsyncInProgressException e) {
                        Log.e(TAG, "Error consuming energy. Another async operation in progress.");
                    }
                    return;
                }
            }

            //updateUi();
            //setWaitScreen(false);
        }
    };

    /**
     * Called when this activity becomes visible.
     */
    @Override
    protected void onStart() {
        super.onStart();
    }


    @Override
    protected void onResume()
    {
        super.onResume();
        vunglePub.onResume();
       // uiHelper.onResume();
       // AppEventsLogger.activateApp(this);
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        //uiHelper.onSaveInstanceState(outState);
    }

    @Override
    public void onPause() {
        super.onPause();
        vunglePub.onPause();
        //uiHelper.onPause();
        //AppEventsLogger.deactivateApp(this);
    }

    public int LoadSound(String name)
    {
        try {
            return mSoundPool.load(getAssets().openFd(name), 1);
        } catch (IOException e)
        {
            return -1;
        }
    }

    public void PlaySound(int id)
    {
        mSoundPool.play(id, 1, 1, 0, 0, 1);
    }

    public void LogInGoogle()
    {
        googleSignedFinished = false;
        mHelper.onStart(this);
    }

    public void LogOutGoogle()
    {
        googleSignedFinished = false;
        signOut();
    }

    public boolean IsGoogleLogged()
    {
        return isSignedIn();
    }

    public void ShowLeaderboards()
    {
        if (isSignedIn())
            startActivityForResult(Games.Leaderboards.getLeaderboardIntent(getApiClient(), LEADERBOARD_ID), REQUEST_LEADERBOARD);
        else
            beginUserInitiatedSignIn();
    }

    public void ShowAchievements()
    {
        if (isSignedIn())
            startActivityForResult(Games.Achievements.getAchievementsIntent(getApiClient()), REQUEST_ACHIEVEMENTS);
        else
            beginUserInitiatedSignIn();
    }

    private void requestNewInterstitial() {
        PublisherAdRequest adRequest = new PublisherAdRequest.Builder()
                .addTestDevice("CEC26E9D5815BCF8D34E8AE04DAE41EB")
                .build();

        interstitialAd.loadAd(adRequest);
    }

    public boolean ShowVideoAd()
    {
        if (vunglePub.isAdPlayable()) {
            vunglePub.playAd();
            return true;
        }
        return false;
    }

    public void RequestBackup() {
        backupManager.dataChanged();
    }

    public void RequestRestore()
    {
        backupManager.requestRestore(
                new RestoreObserver() {
                    public void restoreFinished(int error) {
                        /** Done with the restore!  Now draw the new state of our data */
                        Log.v(TAG, "Restore finished, error = " + error);
                    }
                }
        );
    }

    public void BuyEnergy(int id)
    {
        // TODO: Generate random string
        String payload = "";

        //setWaitScreen(true);
        //Log.d(TAG, "Launching purchase flow for energy.");

        try {
            mPurchaseHelper.launchPurchaseFlow(this, skuList[id], RC_REQUEST,
                    mPurchaseFinishedListener, payload);
        } catch (IabHelper.IabAsyncInProgressException e) {
            Log.e(TAG, "Error launching purchase flow. Another async operation in progress.");
            //setWaitScreen(false);
        }
    }

    public void BuyItem(String itemId)
    {
        // TODO: Generate random string
        String payload = "";

        //setWaitScreen(true);
        //Log.d(TAG, "Launching purchase flow for energy.");

        try {
            mPurchaseHelper.launchPurchaseFlow(this, itemId, RC_REQUEST,
                    mPurchaseFinishedListener, payload);
        } catch (IabHelper.IabAsyncInProgressException e) {
            Log.e(TAG, "Error launching purchase flow. Another async operation in progress.");
            //setWaitScreen(false);
        }
    }

    // Callback for when a purchase is finished
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            Log.i(TAG, "Purchase finished: " + result + ", purchase: " + purchase);

            // if we were disposed of in the meantime, quit.
            if (mPurchaseHelper == null) return;

            if (result.isFailure()) {
                return;
            }

            for (String sku : skuList) {
                if (purchase.getSku().equals(sku)) {
                    // bought energy pack. So consume it.
                    Log.d(TAG, "Purchase is energy. Starting energy consumption.");
                    try {
                        mPurchaseHelper.consumeAsync(purchase, mConsumeFinishedListener);
                    } catch (IabHelper.IabAsyncInProgressException e) {
                        Log.e(TAG, "Error consuming energy. Another async operation in progress.");
                        //setWaitScreen(false);
                        return;
                    }
                }
            }

        }
    };

    // Called when consumption is complete
    IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
        public void onConsumeFinished(Purchase purchase, IabResult result) {
            Log.d(TAG, "Consumption finished. Purchase: " + purchase + ", result: " + result);

            // if we were disposed of in the meantime, quit.
            if (mPurchaseHelper == null) return;

            // We know this is the "energy" sku because it's the only one we consume,
            // so we don't check which sku was consumed. If you have more than one
            // sku, you probably should check...
            if (result.isSuccess()) {
                // successfully consumed, so we apply the effects of the item in our
                // game world's logic, which in our case means filling the gas tank a bit
                Log.d(TAG, "Consumption successful. Provisioning.");
                mOwnedItems.add(purchase.getSku());
            }
            else {
                Log.e(TAG, "Error while consuming: " + result);
            }

            //setWaitScreen(false);
            Log.d(TAG, "End consumption flow.");
        }
    };

    public boolean IsItemBought(String itemId)
    {
        return mOwnedItems.contains(itemId);
    }

    public boolean CheckIsBoughtAndConsume(int id)
    {
        if (mOwnedItems.contains(skuList[id]))
        {
            mOwnedItems.remove(skuList[id]);
            return true;
        }
        return false;
    }

    public int ReturnLastBoughtItem()
    {
        if (mOwnedItems.size() == 0)
        {
            return -1;
        }
        String boughtSku = mOwnedItems.get(0);
        for (int i = 0; i < skuList.length; i++)
        {
            if (skuList[i].equalsIgnoreCase(boughtSku))
            {
                mOwnedItems.remove(0);
                return i;
            }
        }
        mOwnedItems.remove(0);
        return -1;
    }

    public void showLoadingDialog()
    {
        _activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                progressDialog = ProgressDialog.show(_activity, "",
                        "Loading...", true);
            }
        });
    }

    public void hideLoadingDialog()
    {
        progressDialog.dismiss();
        progressDialog = null;
    }

    // Our popup window, you will call it from your C/C++ code later
    public void showAdPopup(int top)
    {
        final int gravity = top;
        if(adsinited)
        {
            if (adshidden)
            {
                _activity.runOnUiThread(new Runnable()  {
                    @Override
                    public void run()  {
                        popUp.showAtLocation(mainLayout, gravity, 0, 0);
                        popUp.update();
                        adshidden = false;
                    }
                });
            }
            return;
        }

        if(adView != null)  {
            _activity.runOnUiThread(new Runnable()  {
                @Override
                public void run()  {
                    adsinited = true;
                    // Out popup window
                    popUp = new PopupWindow(_activity);
                    // This is the minimum size for AdMob, we need to set this in case our target device run at 320x480 resolution (Otherwise no ad will be shown, see the padding kill below)
                    popUp.getBackground().setAlpha(0);
                    int height = (int)TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 53, getResources().getDisplayMetrics());
                    int width = (int)TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 325, getResources().getDisplayMetrics());

                    popUp.setWidth(width);
                    popUp.setHeight(height);
                    //popUp.setWindowLayoutMode(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
                    popUp.setClippingEnabled(false);
                    layout = new LinearLayout(_activity);
                    mainLayout = new LinearLayout(_activity);
                    // The layout system for the PopupWindow will kill some pixels due to margins/paddings etc. (No way to remove it), so padd it to adjust
                    layout.setPadding(-5, -5, -5, -5);
                    MarginLayoutParams params = new MarginLayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
                    params.setMargins(0, 0, 0, 0);
                    layout.setOrientation(LinearLayout.HORIZONTAL);
                    layout.addView(adView, params);

                    _activity.setContentView(mainLayout, params);


                    // Enable this if your are testing AdMob, otherwise you'll risk to be banned!

                    AdRequest.Builder adRequestBuilder = new AdRequest.Builder();

                    // Optionally populate the ad request builder.
                    adRequestBuilder.addTestDevice(AdRequest.DEVICE_ID_EMULATOR);
                    adRequestBuilder.addTestDevice("8C82A7CAD9096B2E6F7F8FA534C75C90");
                    adRequestBuilder.addTestDevice("CEC26E9D5815BCF8D34E8AE04DAE41EB");
                    _activity.adView.loadAd(adRequestBuilder.build());

                    popUp.setContentView(layout);
                    // Show our popup window
                    popUp.showAtLocation(mainLayout, gravity, 0, 0);
                    popUp.update();
                }});
        }
    }

    public void hideAdPopup()
    {
        if (adshidden)
            return;

        Runnable myRunnable = new Runnable()  {
            @Override
            public void run()  {

                if (popUp != null)
                    popUp.dismiss();
                adshidden = true;
                synchronized (this) {
                    notifyAll();
                }
            }
        };

        synchronized (myRunnable) {
            try {
                _activity.runOnUiThread(myRunnable);
                myRunnable.wait(1000);
            } catch (InterruptedException e) {
            }
        }
    }

    /**
     * Called when this activity is no longer visible.
     */
    @Override
    protected void onStop() {
        super.onStop();

        mHelper.onStop();
    }

    // Do some cleanup
    @Override
    public void onDestroy() {
        if (adView != null) {
            adView.destroy();
        }
        super.onDestroy();

        //uiHelper.onDestroy();

        if (mBroadcastReceiver != null) {
            unregisterReceiver(mBroadcastReceiver);
        }

        if (mPurchaseHelper != null) {
            mPurchaseHelper.disposeWhenFinished();
            mPurchaseHelper = null;
        }
    }

    ///// google services

    @Override
    protected void onActivityResult(int request, int response, Intent data) {
        super.onActivityResult(request, response, data);
        mHelper.onActivityResult(request, response, data);

        /*uiHelper.onActivityResult(request, response, data, new FacebookDialog.Callback() {
            @Override
            public void onError(FacebookDialog.PendingCall pendingCall, Exception error, Bundle data) {
                Log.e("Activity", String.format("Error: %s", error.toString()));
            }

            @Override
            public void onComplete(FacebookDialog.PendingCall pendingCall, Bundle data) {
                Log.i("Activity", "Success!");
            }
        });*/

        if (mPurchaseHelper != null)
            mPurchaseHelper.handleActivityResult(request, response, data);
    }

    protected GoogleApiClient getApiClient() {
        return mHelper.getApiClient();
    }

    protected boolean isSignedIn() {
        return mHelper.isSignedIn();
    }

    protected void beginUserInitiatedSignIn() {
        mHelper.beginUserInitiatedSignIn();
    }

    @Override
    public void onResult(LoadAchievementsResult arg0) {
        Achievement ach;
        AchievementBuffer aBuffer = arg0.getAchievements();
        Iterator<Achievement> aIterator = aBuffer.iterator();

        while (aIterator.hasNext()) {
            ach = aIterator.next();
            for (int k = 0; k < achievementList.length; k++)
            {
                if (achievementList[k].equals(ach.getAchievementId())) {
                    if (ach.getState() == Achievement.STATE_UNLOCKED) {
                        _achievementUnlockedStatus[k] = true;
                    } else {
                        _achievementUnlockedStatus[k] = false;
                    }

                }
            }
        }
        aBuffer.close();
    }

    protected void SyncAchievements()
    {
        boolean fullLoad = false;  // set to 'true' to reload all achievements (ignoring cache)

        // load achievements
        PendingResult<LoadAchievementsResult> p = Games.Achievements.load( mHelper.getApiClient(), fullLoad );
        p.setResultCallback(this);
    }

    protected void signOut() {
        mHelper.signOut();
    }

    protected void showAlert(String message) {
        mHelper.makeSimpleDialog(message).show();
    }

    protected void showAlert(String title, String message) {
        mHelper.makeSimpleDialog(title, message).show();
    }

    protected void enableDebugLog(boolean enabled) {
        if (mHelper != null) {
            mHelper.enableDebugLog(enabled);
        }
    }

    @Deprecated
    protected void enableDebugLog(boolean enabled, String tag) {
        Log.w(TAG, "BaseGameActivity.enabledDebugLog(bool,String) is " +
                "deprecated. Use enableDebugLog(boolean)");
        enableDebugLog(enabled);
    }

    protected String getInvitationId() {
        return mHelper.getInvitationId();
    }

    protected void reconnectClient() {
        mHelper.reconnectClient();
    }

    protected boolean hasSignInError() {
        return mHelper.hasSignInError();
    }

    protected GameHelper.SignInFailureReason getSignInError() {
        return mHelper.getSignInError();
    }


    @Override
    public void onSignInSucceeded() {
        // handle sign-in succeess
        googleSignedFinished = true;
        //Toast.makeText(this, "Signed in to Google Services!", Toast.LENGTH_LONG).show();
        SyncAchievements();
        GetTopScores();
    }
    @Override
    public void onSignInFailed() {
        // handle sign-in failure (e.g. show Sign In button)
        googleSignedFinished = true;
    }

    void UpdateScore(int score)
    {
        if (isSignedIn())
            Games.Leaderboards.submitScore(getApiClient(), LEADERBOARD_ID, score);
    }

    void UnlockAchievement(int id)
    {
        if (isSignedIn())
        {
            Games.Achievements.unlock(getApiClient(), achievementList[id]);
            _achievementUnlockedStatus[id] = true;
        }
    }

    boolean IsAchievementUnlocked(int id)
    {
        if (isSignedIn())
        {
            if (!achievementsSynced)
                SyncAchievements();
            else
                return _achievementUnlockedStatus[id];
        }
        return false;
    }

    void UndateAchievement(int id, int score)
    {
        if (isSignedIn())
            Games.Achievements.setSteps(getApiClient(), achievementList[id], score);
    }


    void GetTopScores()
    {
        PendingResult<Leaderboards.LoadScoresResult> result = Games.Leaderboards.loadTopScores(getApiClient(), LEADERBOARD_ID, LeaderboardVariant.TIME_SPAN_WEEKLY,  LeaderboardVariant.COLLECTION_PUBLIC, 5);
        result.setResultCallback(new ResultCallback<Leaderboards.LoadScoresResult>() {
            @Override
            public void onResult(Leaderboards.LoadScoresResult result) {
                // check if valid score
                if (result != null
                        && GamesStatusCodes.STATUS_OK == result.getStatus().getStatusCode()) {

                    _weeklyScoresSize = result.getScores().getCount();
                    for(int i = 0; i < result.getScores().getCount(); i++)
                    {
                        _weeklyScores[i] = new ScoreItem();
                        _weeklyScores[i].name = result.getScores().get(i).getScoreHolderDisplayName();
                        _weeklyScores[i].score = (int)result.getScores().get(i).getRawScore();
                        _weeklyScores[i].place = (int)result.getScores().get(i).getRank();
                    }
                    result.getScores().close();
                    _scoresUpdated = true;
                }
            }
        });

        PendingResult<Leaderboards.LoadScoresResult> result2 = Games.Leaderboards.loadTopScores(getApiClient(), LEADERBOARD_ID, LeaderboardVariant.TIME_SPAN_ALL_TIME,  LeaderboardVariant.COLLECTION_PUBLIC, 5);
        result2.setResultCallback(new ResultCallback<Leaderboards.LoadScoresResult>() {
            @Override
            public void onResult(Leaderboards.LoadScoresResult result) {
                // check if valid score
                if (result != null
                        && GamesStatusCodes.STATUS_OK == result.getStatus().getStatusCode()) {

                    _allTimeScoresSize = result.getScores().getCount();
                    for(int i = 0; i < result.getScores().getCount(); i++)
                    {
                        _allTimeScores[i] = new ScoreItem();
                        _allTimeScores[i].name = result.getScores().get(i).getScoreHolderDisplayName();
                        _allTimeScores[i].score = (int)result.getScores().get(i).getRawScore();
                        _allTimeScores[i].place = (int)result.getScores().get(i).getRank();
                    }
                    result.getScores().close();
                    _scoresUpdated = true;
                }
            }
        });

        PendingResult<Leaderboards.LoadPlayerScoreResult> results3 = Games.Leaderboards.loadCurrentPlayerLeaderboardScore(getApiClient(), LEADERBOARD_ID, LeaderboardVariant.TIME_SPAN_WEEKLY,  LeaderboardVariant.COLLECTION_PUBLIC);
        results3.setResultCallback(new ResultCallback<Leaderboards.LoadPlayerScoreResult>() {
            @Override
            public void onResult(Leaderboards.LoadPlayerScoreResult result) {
                // check if valid score
                if (result != null
                        && GamesStatusCodes.STATUS_OK == result.getStatus().getStatusCode()
                        && result.getScore() != null) {

                    _myScoreWeekly = new ScoreItem();
                    _myScoreWeekly.name = result.getScore().getScoreHolderDisplayName();
                    _myScoreWeekly.score = (int)result.getScore().getRawScore();
                    _myScoreWeekly.place = (int)result.getScore().getRank();

                    if (_myScoreWeekly.place <= 10 && !_achievementUnlockedStatus[9])
                    {
                        UnlockAchievement(9);
                    }

                    _scoresUpdated = true;
                }
            }
        });

        PendingResult<Leaderboards.LoadPlayerScoreResult> results4 = Games.Leaderboards.loadCurrentPlayerLeaderboardScore(getApiClient(), LEADERBOARD_ID, LeaderboardVariant.TIME_SPAN_ALL_TIME,  LeaderboardVariant.COLLECTION_PUBLIC);
        results4.setResultCallback(new ResultCallback<Leaderboards.LoadPlayerScoreResult>() {
            @Override
            public void onResult(Leaderboards.LoadPlayerScoreResult result) {
                // check if valid score
                if (result != null
                        && GamesStatusCodes.STATUS_OK == result.getStatus().getStatusCode()
                        && result.getScore() != null) {

                    _myScoreAlltime = new ScoreItem();
                    _myScoreAlltime.name = result.getScore().getScoreHolderDisplayName();
                    _myScoreAlltime.score = (int)result.getScore().getRawScore();
                    _myScoreAlltime.place = (int)result.getScore().getRank();

                    if (_myScoreAlltime.place <= 10 && !_achievementUnlockedStatus[9])
                    {
                        UnlockAchievement(9);
                    }

                    _scoresUpdated = true;
                }
            }
        });
    }

    boolean IsLeaderboardsUpdated()
    {
        if (_scoresUpdated)
        {
            _scoresUpdated = false;
            return true;
        }
        return false;
    }

    int GetLeaderboardSize(boolean weekly)
    {
        return weekly ? _weeklyScoresSize : _allTimeScoresSize;
    }

    String GetLeaderboardName(int place, boolean weekly)
    {
        if (place == 5)
        {
            if (weekly && _myScoreWeekly == null)
                return "Player";
            if (!weekly && _myScoreAlltime == null)
                return "Player";

            return weekly ? _myScoreWeekly.name : _myScoreAlltime.name;
        }
        return weekly ? _weeklyScores[place].name : _allTimeScores[place].name;
    }

    int GetLeaderboardScore(int place, boolean weekly)
    {
        if (place == 5)
        {
            if (weekly && _myScoreWeekly == null)
                return 0;
            if (!weekly && _myScoreAlltime == null)
                return -1;

            return weekly ? _myScoreWeekly.score : _myScoreAlltime.score;
        }
        return weekly ? _weeklyScores[place].score : _allTimeScores[place].score;
    }

    int GetLeaderboardRank(int place, boolean weekly)
    {
        if (place == 5)
        {
            if (weekly && _myScoreWeekly == null)
                return 999;
            if (!weekly && _myScoreAlltime == null)
                return 999;

            return weekly ? _myScoreWeekly.place : _myScoreAlltime.place;
        }
        return weekly ? _weeklyScores[place].place : _allTimeScores[place].place;
    }

    void ShareOnFacebook()
    {
        /*if (FacebookDialog.canPresentShareDialog(getApplicationContext(),
                FacebookDialog.ShareDialogFeature.SHARE_DIALOG)) {

            // Publish the post using the Share Dialog
            FacebookDialog shareDialog = new FacebookDialog.ShareDialogBuilder(this)
                    .setName("Bubble Shooter 3D")
                    .setLink("https://play.google.com/store/apps/details?id=com.turbulent.bubbleshooter3d")
                    .setCaption("Play for free on Android!")
                    .setDescription("Shoot the bubbles! Classic bubble shooter game goes 3D.")
                    .setApplicationName("Bubble Shooter 3D")
                    .setPicture("https://scontent-cdg.xx.fbcdn.net/hphotos-xfp1/v/t1.0-9/19520_1386775741640166_395959818253468647_n.png?oh=c70333467011e089ff6ab83964ee417d&oe=557C95FD")
                    .build();
            uiHelper.trackPendingDialogCall(shareDialog.present());
        } else {*/
            Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.facebook.com/bubbleshooter3d"));
            startActivity(browserIntent);
            // Fallback. For example, publish the post using the Feed Dialog
        //}

    }

    void OpenLink(String link)
    {
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(link));
        startActivity(browserIntent);
    }

    @Override
    public void receivedBroadcast() {
        Log.d(TAG, "Received broadcast notification. Querying inventory.");
        try {
            mPurchaseHelper.queryInventoryAsync(mGotInventoryListener);
        } catch (IabHelper.IabAsyncInProgressException e) {
            Log.e(TAG, "Error querying inventory. Another async operation in progress.");
        }
    }

    /*@Override
    public void onWindowFocusChanged(boolean hasFocus) {
            super.onWindowFocusChanged(hasFocus);
        if (hasFocus && android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.KITKAT) {
        	getWindow().getDecorView().setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);}
    }*/
}