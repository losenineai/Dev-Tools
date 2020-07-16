/**
 * Copyright 2015 - 2016 KeepSafe Software, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.dx.mobile.risk.linker;

import android.content.Context;
import android.text.TextUtils;
import android.util.Log;

import java.io.File;
import java.io.FilenameFilter;
import java.util.HashSet;
import java.util.Locale;
import java.util.Set;

public class ReLinkerInstance {
    private static final String LIB_DIR = "lib";

    protected final Set<String> loadedLibraries = new HashSet<String>();
    protected final ReLinker.LibraryLoader libraryLoader;
    protected final ReLinker.LibraryInstaller libraryInstaller;

    protected boolean force;
    protected ReLinker.Logger logger;

    protected ReLinkerInstance() {
        this(new SystemLibraryLoader(), new ApkLibraryInstaller());
    }

    protected ReLinkerInstance(final ReLinker.LibraryLoader libraryLoader,
                               final ReLinker.LibraryInstaller libraryInstaller) {
        if (libraryLoader == null) {
            throw new IllegalArgumentException("Cannot pass null library loader");
        }

        if (libraryInstaller == null) {
            throw new IllegalArgumentException("Cannot pass null library installer");
        }

        this.libraryLoader = libraryLoader;
        this.libraryInstaller = libraryInstaller;
    }

    /**
     * Logs debugging related information to the {@link ReLinker.Logger} instance given
     */
    public ReLinkerInstance log(final ReLinker.Logger logger) {
        this.logger = logger;
        return this;
    }

    /**
     * Forces any previously extracted / re-linked libraries to be cleaned up before loading
     */
    public ReLinkerInstance force() {
        this.force = true;
        return this;
    }

    /**
     * Attemps to load the given library normally. If that fails, it loads the library utilizing
     * a workaround.
     *
     * @param context The {@link Context} to get a workaround directory from
     * @param library The library you wish to load
     */
    public void loadLibrary(final Context context, final String library) {

        if (context == null) {
            throw new IllegalArgumentException("Given context is null");
        }

        if (TextUtils.isEmpty(library)) {
            throw new IllegalArgumentException("Given library is either null or empty");
        }

        log("Beginning load of %s...", library);
        loadLibraryInternal(context, library);
    }

    private void loadLibraryInternal(final Context context, final String library) {

        if (loadedLibraries.contains(library) && !force) {
            log("%s already loaded previously!", library);
            return;
        }

        try {
            libraryLoader.loadLibrary(library);
            loadedLibraries.add(library);
            return;
        } catch (final UnsatisfiedLinkError e) {
            log("Loading the library normally failed: %s", Log.getStackTraceString(e));
        }

        log("%s was not loaded normally, re-linking...", library);
        final File workaroundFile = getWorkaroundLibFile(context, library);
        log("workaroundFile: %s", workaroundFile);


        if (!workaroundFile.exists() || force) {
            if (force) {
                log("Forcing a re-link of %s...", library);
            }

            cleanupOldLibFiles(context, library);
            libraryInstaller.installLibrary(context, libraryLoader.supportedAbis(),
                    libraryLoader.mapLibraryName(library), workaroundFile, this);
        }

        libraryLoader.loadPath(workaroundFile.getAbsolutePath());
        loadedLibraries.add(library);
        log("%s was re-linked!", library);
    }

    /**
     * @param context {@link Context} to describe the location of it's private directories
     * @return A {@link File} locating the directory that can store extracted libraries
     * for later use
     */
    protected File getWorkaroundLibDir(final Context context) {
        return context.getDir(LIB_DIR, Context.MODE_PRIVATE);
    }

    /**
     * @param context {@link Context} to retrieve the workaround directory from
     * @param library The name of the library to load
     * @return A {@link File} locating the workaround library file to load
     */
    protected File getWorkaroundLibFile(final Context context, final String library) {

        final String libName = libraryLoader.mapLibraryName(library);

        return new File(getWorkaroundLibDir(context), libName);
    }

    /**
     * Cleans up any <em>other</em> versions of the {@code library}. If {@code force} is used, all
     * versions of the {@code library} are deleted
     *
     * @param context {@link Context} to retrieve the workaround directory from
     * @param library The name of the library to load
     */
    protected void cleanupOldLibFiles(final Context context, final String library) {

        final File workaroundDir = getWorkaroundLibDir(context);
        final File workaroundFile = getWorkaroundLibFile(context, library);
        final String mappedLibraryName = libraryLoader.mapLibraryName(library);
        final File[] existingFiles = workaroundDir.listFiles(new FilenameFilter() {
            @Override
            public boolean accept(File dir, String filename) {
                return filename.startsWith(mappedLibraryName);
            }
        });

        if (existingFiles == null) return;

        for (final File file : existingFiles) {
            if (force || !file.getAbsolutePath().equals(workaroundFile.getAbsolutePath())) {
                file.delete();
            }
        }
    }

    public void log(final String format, final Object... args) {
        log(String.format(Locale.US, format, args));
    }

    public void log(final String message) {

        Log.i("DXRisk", "relinker: " + message);

        if (logger != null) {
            logger.log(message);
        }
    }
}
