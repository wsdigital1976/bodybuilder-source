package org.bodybuildercore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class BodybuilderQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File bodybuilderDir = new File(getFilesDir().getAbsolutePath() + "/.bodybuilder");
        if (!bodybuilderDir.exists()) {
            bodybuilderDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
