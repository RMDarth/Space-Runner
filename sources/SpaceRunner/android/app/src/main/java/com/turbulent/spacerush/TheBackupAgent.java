package com.turbulent.spacerush;

import android.app.backup.BackupAgentHelper;
import android.app.backup.FileBackupHelper;
import android.util.Log;

import java.io.File;

public class TheBackupAgent extends BackupAgentHelper {
    static final String CONFIG_FILENAME = "config.dat";

    static final String SCORES_FILENAME = "scores.dat";

    // A key to uniquely identify the set of backup data
    static final String FILES_BACKUP_KEY = "spacerunnerfiles";

    // Allocate a helper and add it to the backup agent
    @Override
    public void onCreate() {
        FileBackupHelper helper = new FileBackupHelper(this, CONFIG_FILENAME, SCORES_FILENAME);
        addHelper(FILES_BACKUP_KEY, helper);
    }
}