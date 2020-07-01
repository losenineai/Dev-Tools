/*
 *  Copyright (c) 2015, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

package com.auric.intell.commonlib.network.quality;


import com.auric.intell.commonlib.utils.LogTool;

import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicReference;


/**
 * <p>
 * Class used to calculate the approximate bandwidth of a user's connection.
 * </p>
 * <p>
 * This class notifies all subscribed {@link ConnectionClassStateChangeListener} with the new
 * ConnectionClass when the network's ConnectionClass changes.
 * </p>
 */
public class NetworkQualityManager {

    /*package*/ static final double DEFAULT_SAMPLES_TO_QUALITY_CHANGE = 5;
    private static final int BYTES_TO_BITS = 8;

    /**
     * Default values for determining quality of data connection.
     * Bandwidth numbers are in Kilobits per second (kbps).
     * 根据不同国情，网速的标准也是不一样，需要修改。by white.
     */
  /*package*/ static final double DEFAULT_POOR_BANDWIDTH = 1;
    /*package*/ static final double DEFAULT_MODERATE_BANDWIDTH = 100;
    /*package*/ static final double DEFAULT_GOOD_BANDWIDTH = 200;
    /*package*/ static final long DEFAULT_HYSTERESIS_PERCENT = 20;
    private static final double HYSTERESIS_TOP_MULTIPLIER = 100.0 / (100.0 - DEFAULT_HYSTERESIS_PERCENT);
    private static final double HYSTERESIS_BOTTOM_MULTIPLIER = (100.0 - DEFAULT_HYSTERESIS_PERCENT) / 100.0;

    /**
     * The factor used to calculate the current bandwidth
     * depending upon the previous calculated value for bandwidth.
     * <p>
     * The smaller this value is, the less responsive to new samples the moving average becomes.
     */
    private static final double DEFAULT_DECAY_CONSTANT = 0.05;
    private static final String TAG = "NetworkQualityManager";

    /**
     * Current bandwidth of the user's connection depending upon the response.
     */
    private ExponentialGeometricAverage mDownloadBandwidth
            = new ExponentialGeometricAverage(DEFAULT_DECAY_CONSTANT);
    private volatile boolean mInitiateStateChange = false;
    private AtomicReference<NetworkQuality> mCurrentBandwidthConnectionQuality =
            new AtomicReference<NetworkQuality>(NetworkQuality.UNKNOWN);
    private AtomicReference<NetworkQuality> mNextBandwidthConnectionQuality;
    private ArrayList<ConnectionClassStateChangeListener> mListenerList =
            new ArrayList<ConnectionClassStateChangeListener>();
    private int mSampleCounter;

    /**
     * The lower bound for measured bandwidth in bits/ms. Readings
     * lower than this are treated as effectively zero (therefore ignored).
     */
    static final long BANDWIDTH_LOWER_BOUND = 10;

    // Singleton.
    private static class ConnectionClassManagerHolder {
        public static final NetworkQualityManager instance = new NetworkQualityManager();
    }

    /**
     * Retrieval method for the DownloadBandwidthManager singleton.
     *
     * @return The singleton instance of DownloadBandwidthManager.
     */
    public static NetworkQualityManager getInstance() {
        return ConnectionClassManagerHolder.instance;
    }

    // Force constructor to be private.
    private NetworkQualityManager() {
    }

    /**
     * Adds bandwidth to the current filtered latency counter. Sends a broadcast to all
     * {@link ConnectionClassStateChangeListener} if the counter moves from one bucket
     * to another (i.e. poor bandwidth -> moderate bandwidth).
     */
    public synchronized void addBandwidth(long bytes, long timeInMs) {

        //Ignore garbage values.
//        if (timeInMs == 0 || (bytes) * 1.0 / (timeInMs) * BYTES_TO_BITS < BANDWIDTH_LOWER_BOUND) {
//            return;
//        }

        double bandwidth = (bytes) * 1.0 / (timeInMs) * BYTES_TO_BITS;
        mDownloadBandwidth.addMeasurement(bandwidth);

        if (mInitiateStateChange) {
            mSampleCounter += 1;
            if (getCurrentBandwidthQuality() != mNextBandwidthConnectionQuality.get()) {
                mInitiateStateChange = false;
                mSampleCounter = 1;
            }
            if (mSampleCounter >= DEFAULT_SAMPLES_TO_QUALITY_CHANGE && significantlyOutsideCurrentBand()) {
                mInitiateStateChange = false;
                mSampleCounter = 1;
                mCurrentBandwidthConnectionQuality.set(mNextBandwidthConnectionQuality.get());
                notifyListeners();
            }
            return;
        }

        if (mCurrentBandwidthConnectionQuality.get() != getCurrentBandwidthQuality()) {
            mInitiateStateChange = true;
            mNextBandwidthConnectionQuality =
                    new AtomicReference<NetworkQuality>(getCurrentBandwidthQuality());
        }
    }

    private boolean significantlyOutsideCurrentBand() {
        if (mDownloadBandwidth == null) {
            // Make Infer happy. It wouldn't make any sense to call this while mDownloadBandwidth is null.
            return false;
        }
        NetworkQuality currentQuality = mCurrentBandwidthConnectionQuality.get();
        double bottomOfBand;
        double topOfBand;
        switch (currentQuality) {
            case POOR:
                bottomOfBand = 0;
                topOfBand = DEFAULT_POOR_BANDWIDTH;
                break;
            case MODERATE:
                bottomOfBand = DEFAULT_POOR_BANDWIDTH;
                topOfBand = DEFAULT_MODERATE_BANDWIDTH;
                break;
            case GOOD:
                bottomOfBand = DEFAULT_MODERATE_BANDWIDTH;
                topOfBand = DEFAULT_GOOD_BANDWIDTH;
                break;
            case EXCELLENT:
                bottomOfBand = DEFAULT_GOOD_BANDWIDTH;
                topOfBand = Float.MAX_VALUE;
                break;
            default: // If current quality is UNKNOWN, then changing is always valid.
                return true;
        }
        double average = mDownloadBandwidth.getAverage();
        if (average > topOfBand) {
            if (average > topOfBand * HYSTERESIS_TOP_MULTIPLIER) {
                return true;
            }
        } else if (average < bottomOfBand * HYSTERESIS_BOTTOM_MULTIPLIER) {
            return true;
        }
        return false;
    }

    /**
     * Resets the bandwidth average for this instance of the bandwidth manager.
     */
    public void reset() {
        if (mDownloadBandwidth != null) {
            mDownloadBandwidth.reset();
        }
        mCurrentBandwidthConnectionQuality.set(NetworkQuality.UNKNOWN);
    }

    /**
     * Get the NetworkQuality that the moving bandwidth average currently represents.
     *
     * @return A NetworkQuality representing the device's bandwidth at this exact moment.
     */
    public synchronized NetworkQuality getCurrentBandwidthQuality() {
        if (mDownloadBandwidth == null) {
            return NetworkQuality.UNKNOWN;
        }
        return mapBandwidthQuality(mDownloadBandwidth.getAverage());
    }

    private NetworkQuality mapBandwidthQuality(double average) {
        LogTool.d(TAG, "mapBandwidthQuality average:"+average);
        if (average <= 0) {
            return NetworkQuality.UNKNOWN;
        }
        if (average < DEFAULT_POOR_BANDWIDTH) {
            return NetworkQuality.POOR;
        }
        if (average < DEFAULT_MODERATE_BANDWIDTH) {
            return NetworkQuality.MODERATE;
        }
        if (average < DEFAULT_GOOD_BANDWIDTH) {
            return NetworkQuality.GOOD;
        }
        return NetworkQuality.EXCELLENT;
    }


    /**
     * Accessor method for the current bandwidth average.
     *
     * @return The current bandwidth average, or -1 if no average has been recorded.
     */
    public synchronized double getDownloadKBitsPerSecond() {
        return mDownloadBandwidth == null
                ? -1.0
                : mDownloadBandwidth.getAverage();
    }

    /**
     * Interface for listening to when {@link com.facebook.network.connectionclass.ConnectionClassManager}
     * changes state.
     */
    public interface ConnectionClassStateChangeListener {
        /**
         * The method that will be called when {@link com.facebook.network.connectionclass.ConnectionClassManager}
         * changes ConnectionClass.
         *
         * @param bandwidthState The new ConnectionClass.
         */
        public void onBandwidthStateChange(NetworkQuality bandwidthState);
    }

    /**
     * Method for adding new listeners to this class.
     *
     * @param listener {@link ConnectionClassStateChangeListener} to add as a listener.
     */
    public NetworkQuality register(ConnectionClassStateChangeListener listener) {
        if (listener != null) {
            mListenerList.add(listener);
        }
        return mCurrentBandwidthConnectionQuality.get();
    }

    /**
     * Method for removing listeners from this class.
     *
     * @param listener Reference to the {@link ConnectionClassStateChangeListener} to be removed.
     */
    public void remove(ConnectionClassStateChangeListener listener) {
        if (listener != null) {
            mListenerList.remove(listener);
        }
    }

    private void notifyListeners() {
        int size = mListenerList.size();
        for (int i = 0; i < size; i++) {
            mListenerList.get(i).onBandwidthStateChange(mCurrentBandwidthConnectionQuality.get());
        }
    }
}