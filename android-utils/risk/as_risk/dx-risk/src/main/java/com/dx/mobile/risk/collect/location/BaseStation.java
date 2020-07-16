package com.dx.mobile.risk.collect.location;

import android.content.Context;
import android.os.Build;
import android.telephony.*;
import android.telephony.cdma.CdmaCellLocation;
import android.telephony.gsm.GsmCellLocation;
import android.util.Log;

import com.dx.mobile.risk.utils.*;

import org.json.JSONArray;
import org.json.JSONObject;

import java.text.DecimalFormat;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author white
 * @description：基站信息
 * @date 2020-01-09
 */
public class BaseStation {

    private static TelephonyManager telephoneMgr;

    static {
        if (telephoneMgr == null) {
            try {
                telephoneMgr = (TelephonyManager) ContextFinder.getApplication().getSystemService(Context.TELEPHONY_SERVICE);
                SimUtils.init(telephoneMgr);
            } catch (SecurityException e) {
            }

        }
    }

    /**
     * 获取基站信息
     */
    public static String getBaseStation() {

        JSONObject action_json = new JSONObject();

        if (!PermissionUtils.checkLocation())
            return "";

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN_MR1)
            return "";

        try {

            switch (telephoneMgr.getPhoneType()) {
                case 1:

                    GsmCellLocation gsmCellLocation = (GsmCellLocation) telephoneMgr.getCellLocation();
                    if (gsmCellLocation != null) {
                        JSONUtils.jsonPut(action_json, "K700", "gsm");
                        JSONUtils.jsonPut(action_json, "K701", String.valueOf(gsmCellLocation.getLac()));
                        JSONUtils.jsonPut(action_json, "K702", String.valueOf(gsmCellLocation.getCid()));
                        JSONUtils.jsonPut(action_json, "K703", String.valueOf(gsmCellLocation.getPsc()));

                        /**
                         * 反射方式获取，为了检测篡改，防止api被hook
                         */
                        JSONUtils.jsonPut(action_json, "K711", ReflectUtil.getFieldValue(gsmCellLocation, "mLac")+"");
                        JSONUtils.jsonPut(action_json, "K712", ReflectUtil.getFieldValue(gsmCellLocation, "mCid")+"");
                        JSONUtils.jsonPut(action_json, "K713", ReflectUtil.getFieldValue(gsmCellLocation, "mPsc")+"");
                    }
                    break;
                case 2:
                    CdmaCellLocation cdmaCellLocation = (CdmaCellLocation) telephoneMgr.getCellLocation();

                    if (cdmaCellLocation != null) {
                        JSONUtils.jsonPut(action_json, "K700", "cdma");
                        JSONUtils.jsonPut(action_json, "K704", String.valueOf(cdmaCellLocation.getNetworkId()));
                        JSONUtils.jsonPut(action_json, "K705", String.valueOf(cdmaCellLocation.getBaseStationId()));
                        JSONUtils.jsonPut(action_json, "K706", String.valueOf(cdmaCellLocation.getSystemId()));

                        /**
                         * 反射方式获取，为了检测篡改，防止api被hook
                         */
                        JSONUtils.jsonPut(action_json, "K714", ReflectUtil.getFieldValue(cdmaCellLocation, "mNetworkId")+"");
                        JSONUtils.jsonPut(action_json, "K715", ReflectUtil.getFieldValue(cdmaCellLocation, "mBaseStationId")+"");
                        JSONUtils.jsonPut(action_json, "K716", ReflectUtil.getFieldValue(cdmaCellLocation, "mSystemId")+"");


                        if (cdmaCellLocation.getBaseStationLatitude() != 0x7fffffff) {
                            JSONUtils.jsonPut(action_json, "K707", doFormatLongitude(cdmaCellLocation.getBaseStationLongitude()));
                            JSONUtils.jsonPut(action_json, "K708", doFormatLongitude(cdmaCellLocation.getBaseStationLatitude()));
                        }
                    }
                    break;
                default:
                    break;
            }
        } catch (Throwable e) {
//            e.printStackTrace();
        }

        JSONUtils.jsonPut(action_json, "K720", getAllBaseStation());
        JSONUtils.jsonPut(action_json, "K721", getNeighboringBaseStation());

        return action_json.toString();
    }


    private static String doFormatLongitude(int data){
        DecimalFormat df = new DecimalFormat("#.000000");
        double result = ((double) data) / 14400.0d;
        return df.format(result);
    }


    /**
     * 获取全部基站信息
     */
    private static String getAllBaseStation() {

        JSONArray jsonArr = new JSONArray();

        if (!PermissionUtils.checkLocation())
            return "";

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN_MR1)
            return "";

        try {

            List<CellInfo> list = telephoneMgr.getAllCellInfo();

            if (list != null && list.size() > 0) {

                for (CellInfo cellInfo : list) {

                    if (cellInfo instanceof CellInfoGsm) {

                        JSONObject gsmJson = new JSONObject();
                        JSONObject ssGsmJson = new JSONObject();
                        JSONObject idGsmJson = new JSONObject();

                        CellSignalStrengthGsm ssGsm = ((CellInfoGsm) cellInfo).getCellSignalStrength();

                        ssGsmJson.put("AsuLevel", ssGsm.getAsuLevel());
                        ssGsmJson.put("Dbm", ssGsm.getDbm());
                        ssGsmJson.put("Level", ssGsm.getLevel());

                        CellIdentityGsm idGsm = ((CellInfoGsm) cellInfo).getCellIdentity();

                        idGsmJson.put("cid", idGsm.getCid());
                        idGsmJson.put("lac", idGsm.getLac());

                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                            idGsmJson.put("mcc", idGsm.getMccString());
                            idGsmJson.put("mnc", idGsm.getMncString());
                            idGsmJson.put("arfcn", idGsm.getArfcn());
                            idGsmJson.put("bsic", idGsm.getBsic());
                        } else {
                            idGsmJson.put("mcc", idGsm.getMcc());
                            idGsmJson.put("mnc", idGsm.getMnc());
                        }

                        gsmJson.put("signal_strength", ssGsmJson);
                        gsmJson.put("cell_identity", idGsmJson);
                        gsmJson.put("string", "");
                        gsmJson.put("type", "gsm");

                        jsonArr.put(gsmJson);

                    } else if (cellInfo instanceof CellInfoCdma) {

                        JSONObject cdmaJson = new JSONObject();
                        JSONObject ssCdmaJson = new JSONObject();
                        JSONObject idCdmaJson = new JSONObject();

                        CellSignalStrengthCdma ssCdma = ((CellInfoCdma) cellInfo).getCellSignalStrength();

                        ssCdmaJson.put("AsuLevel", ssCdma.getAsuLevel());
                        ssCdmaJson.put("CdmaDbm", ssCdma.getCdmaDbm());
                        ssCdmaJson.put("CdmaLevel", ssCdma.getCdmaLevel());
                        ssCdmaJson.put("CdmaEcio", ssCdma.getCdmaEcio());
                        ssCdmaJson.put("Dbm", ssCdma.getDbm());
                        ssCdmaJson.put("Level", ssCdma.getEvdoDbm());
                        ssCdmaJson.put("EvdoEcio", ssCdma.getEvdoEcio());
                        ssCdmaJson.put("EvdoLevel", ssCdma.getEvdoLevel());
                        ssCdmaJson.put("EvdoSnr", ssCdma.getEvdoSnr());
                        ssCdmaJson.put("Level", ssCdma.getLevel());

                        CellIdentityCdma idCdma = ((CellInfoCdma) cellInfo).getCellIdentity();

                        idCdmaJson.put("BasestationId", idCdma.getBasestationId());

                        if (idCdma.getLatitude() != 0x7FFFFFFF) {
                            String lon = doFormatLongitude(idCdma.getLongitude());
                            String lat = doFormatLongitude(idCdma.getLatitude());
                            idCdmaJson.put("Latitude", lon);
                            idCdmaJson.put("Longitude", lat);
                        }

                        idCdmaJson.put("NetworkId", idCdma.getNetworkId());
                        idCdmaJson.put("SystemId", idCdma.getSystemId());

                        cdmaJson.put("signal_strength", ssCdmaJson);
                        cdmaJson.put("cell_identity", idCdmaJson);
                        cdmaJson.put("string", "");
                        cdmaJson.put("type", "cdma");

                        cdmaJson.put("signal_strength", ssCdmaJson);
                        cdmaJson.put("cell_identity", idCdmaJson);
                        cdmaJson.put("string", "");
                        cdmaJson.put("type", "cdma");

                        jsonArr.put(cdmaJson);

                    } else if (cellInfo instanceof CellInfoWcdma) {

                        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN_MR2)
                            continue;

                        JSONObject wcdmaJson = new JSONObject();
                        JSONObject ssWcdmaJson = new JSONObject();
                        JSONObject idWcdmaJson = new JSONObject();

                        CellSignalStrengthWcdma ssWcdma = null;
                        ssWcdma = ((CellInfoWcdma) cellInfo).getCellSignalStrength();

                        ssWcdmaJson.put("AsuLevel", ssWcdma.getAsuLevel());
                        ssWcdmaJson.put("Dbm", ssWcdma.getDbm());
                        ssWcdmaJson.put("Level", ssWcdma.getLevel());

                        CellIdentityWcdma idWcdma = ((CellInfoWcdma) cellInfo).getCellIdentity();

                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                            idWcdmaJson.put("uarfcn", idWcdma.getUarfcn());
                        }
                        idWcdmaJson.put("cid", idWcdma.getCid());
                        idWcdmaJson.put("lac", idWcdma.getLac());
                        idWcdmaJson.put("psc", idWcdma.getPsc());

                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                            idWcdmaJson.put("mcc", idWcdma.getMccString());
                            idWcdmaJson.put("mnc", idWcdma.getMncString());
                            idWcdmaJson.put("mno", idWcdma.getMobileNetworkOperator());
                        } else {
                            idWcdmaJson.put("mcc", idWcdma.getMcc());
                            idWcdmaJson.put("mnc", idWcdma.getMnc());
                        }

                        wcdmaJson.put("signal_strength", ssWcdmaJson);
                        wcdmaJson.put("cell_identity", idWcdmaJson);
                        wcdmaJson.put("string", "");
                        wcdmaJson.put("type", "wcdma");

                        jsonArr.put(wcdmaJson);

                    } else if (cellInfo instanceof CellInfoLte) {

                        JSONObject lteJson = new JSONObject();
                        JSONObject ssLteJson = new JSONObject();
                        JSONObject ssLteInfoJson = new JSONObject();
                        JSONObject idLteJson = new JSONObject();

                        CellSignalStrengthLte ssLte = ((CellInfoLte) cellInfo).getCellSignalStrength();

                        ssLteJson.put("AsuLevel", ssLte.getAsuLevel());
                        ssLteJson.put("Dbm", ssLte.getDbm());
                        ssLteJson.put("Level", ssLte.getLevel());
                        ssLteJson.put("TimingAdvance", ssLte.getTimingAdvance());

                        String ssLteInfo = ssLte.toString();
                        if (!StringUtils.isEmpty(ssLteInfo)) {
                            Pattern p = Pattern.compile("(?<=.*?)(-?\\d+)");
                            Matcher m = p.matcher(ssLteInfo);

                            ssLteInfoJson.put("ss", m.find() ? ssLteInfo.substring(m.start(), m.end()) : "");
                            ssLteInfoJson.put("rsrp", m.find() ? ssLteInfo.substring(m.start(), m.end()) : "");
                            ssLteInfoJson.put("rsrq", m.find() ? ssLteInfo.substring(m.start(), m.end()) : "");
                            ssLteInfoJson.put("rssnr", m.find() ? ssLteInfo.substring(m.start(), m.end()) : "");
                            ssLteInfoJson.put("cqi", m.find() ? ssLteInfo.substring(m.start(), m.end()) : "");
                            ssLteInfoJson.put("ta", m.find() ? ssLteInfo.substring(m.start(), m.end()) : "");
                        }

                        CellIdentityLte idLte = ((CellInfoLte) cellInfo).getCellIdentity();

                        idLteJson.put("Ci", idLte.getCi());
                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
                            idLteJson.put("Earfcn", idLte.getEarfcn());
                        }
                        idLteJson.put("Pci", idLte.getPci());
                        idLteJson.put("Tac", idLte.getTac());

                        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                            idLteJson.put("mcc", idLte.getMccString());
                            idLteJson.put("mnc", idLte.getMncString());
                            idLteJson.put("mno", idLte.getMobileNetworkOperator());
                            idLteJson.put("Bwth", idLte.getBandwidth());
                        } else {
                            idLteJson.put("mcc", idLte.getMcc());
                            idLteJson.put("mnc", idLte.getMnc());
                        }

                        lteJson.put("signal_strength", ssLteJson);
                        lteJson.put("cell_identity", idLteJson);
                        lteJson.put("string", ssLteInfoJson);
                        lteJson.put("type", "lte");

                        jsonArr.put(lteJson);
                    }
                }
            }

        } catch (Throwable e) {
//            e.printStackTrace();
        }

        return jsonArr.length() > 0 ? jsonArr.toString() : "";
    }

    /**
     * 获取附近基站信息
     */
    private static String getNeighboringBaseStation() {

        JSONArray jsonArr = new JSONArray();

        if (!PermissionUtils.checkLocation())
            return "";

        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {

                List<NeighboringCellInfo> neighboringCellInfos = telephoneMgr.getNeighboringCellInfo();

                for (NeighboringCellInfo cellInfo : neighboringCellInfos) {
                    if (checkBbsData(cellInfo.getLac())) {

                        JSONObject action_json = new JSONObject();

                        JSONUtils.jsonPut(action_json, "cid", cellInfo.getCid());
                        JSONUtils.jsonPut(action_json, "lac", cellInfo.getLac());
                        JSONUtils.jsonPut(action_json, "psc", cellInfo.getPsc());
                        JSONUtils.jsonPut(action_json, "rssi", cellInfo.getRssi());
                        JSONUtils.jsonPut(action_json, "type", cellInfo.getNetworkType());

                        jsonArr.put(action_json);

                        break;
                    }
                }
            }
        } catch (Throwable e) {
        }

        return jsonArr.length() > 0 ? jsonArr.toString() : "";
    }

    private static boolean checkBbsData(int lac) {
        if (lac < 0 || lac > 65535) {
            return false;
        }
        return true;
    }

}
