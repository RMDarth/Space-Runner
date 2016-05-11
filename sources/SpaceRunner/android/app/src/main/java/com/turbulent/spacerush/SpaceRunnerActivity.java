package com.turbulent.spacerush;
import java.io.IOException;
import java.security.SecureRandom;
import java.security.acl.Owner;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Set;

//import org.codechimp.apprater.AppRater;

import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.app.NativeActivity;
import android.content.Context;
import android.content.Intent;
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




/*import com.facebook.*;
import com.facebook.model.*;
import com.facebook.widget.FacebookDialog;
import com.google.android.gms.ads.*;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.PendingResult;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.achievement.Achievement;
import com.google.android.gms.games.achievement.AchievementBuffer;
import com.google.android.gms.games.achievement.Achievements.LoadAchievementsResult;
import com.turbulent.bubbleshooter3d.Consts;
import com.turbulent.bubbleshooter3d.Consts.PurchaseState;
import com.turbulent.bubbleshooter3d.Consts.ResponseCode;
import com.turbulent.bubbleshooter3d.util.IabHelper;
import com.turbulent.bubbleshooter3d.util.IabResult;
import com.turbulent.bubbleshooter3d.util.Inventory;
import com.turbulent.bubbleshooter3d.util.Purchase;*/


public class SpaceRunnerActivity extends NativeActivity
{
    static {
        System.loadLibrary("SpaceRunner");
    }

    private static final String TAG = "SpaceRunner";

    private static final int REQUEST_ACHIEVEMENTS = 12634236;
    private static final String LEADERBOARD_ID = "CgkI7a-1w_UEEAIQEQ";
    private static final int REQUEST_LEADERBOARD = 2134351;
    private String[] achievementList =
            {
                    "CgkI7a-1w_UEEAIQAQ",
                    "CgkI7a-1w_UEEAIQAg",
                    "CgkI7a-1w_UEEAIQAw",
                    "CgkI7a-1w_UEEAIQBA",
                    "CgkI7a-1w_UEEAIQBQ",
                    "CgkI7a-1w_UEEAIQBg",
                    "CgkI7a-1w_UEEAIQBw",
                    "CgkI7a-1w_UEEAIQCA",
                    "CgkI7a-1w_UEEAIQCQ",
                    "CgkI7a-1w_UEEAIQCg",
                    "CgkI7a-1w_UEEAIQCw",
                    "CgkI7a-1w_UEEAIQDA",
                    "CgkI7a-1w_UEEAIQDQ",
                    "CgkI7a-1w_UEEAIQDg",
                    "CgkI7a-1w_UEEAIQDw",
                    "CgkI7a-1w_UEEAIQEA",
                    "CgkI7a-1w_UEEAIQEQ"
            };

    private boolean[] _achievementUnlockedStatus = new boolean[20];

    private String encryptKey = "BadabeemKey";

    private SoundPool mSoundPool;

    //private UiLifecycleHelper uiHelper;

    // Ads
    //AdView adView;
    PopupWindow popUp;
    SpaceRunnerActivity _activity;
    LinearLayout layout;
    LinearLayout mainLayout;
    boolean adsinited = false;
    boolean adshidden = false;
    boolean googleSignedFinished = false;
    boolean achievementsSynced = false;


    /// Google services

    /// InApp purchases
    /**
     * The SharedPreferences key for recording whether we initialized the
     * database.  If false, then we perform a RestoreTransactions request
     * to get all the purchases for this user.
     */
    // The helper object
    private Set<String> mOwnedItems = new HashSet<String>();
    static final int RC_REQUEST = 43171;


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

        // sound
        mSoundPool = new SoundPool(5, AudioManager.STREAM_MUSIC, 0);
        //mSoundPool.setOnLoadCompleteListener(this);

    }

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
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
    }

    @Override
    public void onPause() {
        super.onPause();
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

    }

    public void ShowAchievements()
    {

    }



    public void BuyItem(String itemId)
    {
        String payload = "";
    }


    public boolean IsItemBought(String itemId)
    {
        return mOwnedItems.contains(itemId);
    }

    // Our popup window, you will call it from your C/C++ code later
    public void showAdPopup(int top)
    {
        /*final int gravity = top;
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
        }*/
    }

    public void hideAdPopup()
    {
        if (adshidden)
            return;

        /*Runnable myRunnable = new Runnable()  {
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
        }*/
    }

    /**
     * Called when this activity is no longer visible.
     */
    @Override
    protected void onStop() {
        super.onStop();
    }

    // Do some cleanup
    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    ///// google services

    @Override
    protected void onActivityResult(int request, int response, Intent data) {
        super.onActivityResult(request, response, data);
    }


    protected boolean isSignedIn() {
        return false;
    }

    protected void beginUserInitiatedSignIn() {

    }


    protected void SyncAchievements()
    {
        boolean fullLoad = false;  // set to 'true' to reload all achievements (ignoring cache)
    }

    protected void signOut() {

    }

    protected void showAlert(String message) {

    }

    protected void showAlert(String title, String message) {

    }

    protected void enableDebugLog(boolean enabled) {

    }

    @Deprecated
    protected void enableDebugLog(boolean enabled, String tag) {
        Log.w(TAG, "BaseGameActivity.enabledDebugLog(bool,String) is " +
                "deprecated. Use enableDebugLog(boolean)");
        enableDebugLog(enabled);
    }

    protected String getInvitationId() {
        return "";
    }

    protected void reconnectClient() {

    }

    protected boolean hasSignInError() {

        return true;
    }

    public void UpdateScore(int score)
    {

    }

    public void UnlockAchievement(int id)
    {

    }

    boolean IsAchievementUnlocked(int id)
    {
        return false;
    }

    void UndateAchievement(int id, int score)
    {
        ;
    }


    void ShareOnFacebook()
    {
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.facebook.com/bubbleshooter3d"));
        startActivity(browserIntent);

    }

    void OpenLink(String link)
    {
        Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(link));
        startActivity(browserIntent);
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