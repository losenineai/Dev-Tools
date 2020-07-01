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

/**
 * A general enumeration for different connection qualities.
 *
 * <p>
 * In order to compare qualities use the {@link .compareTo()} method. Qualities are ordered in increasing
 * order of declaration as per the java docs for {@link Enum}.
 * </p>
 */
public enum NetworkQuality {
  POOR,
  MODERATE,
  GOOD,
  EXCELLENT,
  /**
   * Placeholder for unknown bandwidth. This is the initial value and will stay at this value
   * if a bandwidth cannot be accurately found.
   */
  UNKNOWN
}
