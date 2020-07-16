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

// from https://github.com/KeepSafe/ReLinker

package com.dx.mobile.risk.linker;

import android.content.Context;

import java.io.File;

/**
 * ReLinker is a small library to help alleviate {@link UnsatisfiedLinkError} exceptions thrown due
 * to Android's inability to properly install / load native libraries for Android versions before
 * API 21
 */
public class ReLinker {

    public interface Logger {
        void log(String message);
    }

    public interface LibraryLoader {
        void loadLibrary(String libraryName);
        void loadPath(String libraryPath);
        String mapLibraryName(String libraryName);
        String unmapLibraryName(String mappedLibraryName);
        String[] supportedAbis();
    }

    public interface LibraryInstaller {
        void installLibrary(Context context, String[] abis, String mappedLibraryName,
                            File destination, ReLinkerInstance logger);
    }

    private static ReLinkerInstance mRelinkerInstance;

    static {
        mRelinkerInstance = new ReLinkerInstance();
    }

    public static boolean loadLibrary(final Context context, final String library, final LinkageError ori_e) {

        boolean ret = false;

        try {
            mRelinkerInstance.loadLibrary(context, library);
            ret = true;
        } catch (LinkageError e) {
            ReLinkerErrorHandler.handleLoadLibraryError(context, e, ori_e);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return ret;
    }

    public static void force() {
        mRelinkerInstance.force();
    }

    public static void log(final Logger logger) {
        mRelinkerInstance.log(logger);
    }

    private ReLinker() {}
}
