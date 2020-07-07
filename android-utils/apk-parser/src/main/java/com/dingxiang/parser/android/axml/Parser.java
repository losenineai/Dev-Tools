package com.dingxiang.parser.android.axml;

import pxb.android.arsc.ArscDumper;
import pxb.android.arsc.ArscParser;
import pxb.android.arsc.Pkg;
import pxb.android.axml.AxmlParser;
import pxb.android.axml.Util;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/**
 * Created by acgmohu on 14-7-1.
 * <p/>
 * Parse AndroidManifest.xml and resources.arsc
 */
public class Parser {

    private HashMap<String, String> resources;
    private ManifestInfo manifestInfo = null;

    public Parser(File pFile) throws IOException {
        ZipFile zipFile = new ZipFile(pFile);
        InputStream aXMLInputStream;
        InputStream arscInputStream;


        ZipEntry zipEntry = zipFile.getEntry("resources.arsc");
        if (zipEntry != null) {
            arscInputStream = zipFile.getInputStream(zipEntry);
            readArsc(Util.readIs(arscInputStream));
        }

        zipEntry = zipFile.getEntry("AndroidManifest.xml");
        if (zipEntry != null) {
            aXMLInputStream = zipFile.getInputStream(zipEntry);
            readAxml(Util.readIs(aXMLInputStream));
        }

        zipFile.close();
    }

    /**
     * Notice : axml not NULL.
     *
     * @param axmlBytes It's not null.
     * @param arscBytes resource.arsc
     * @throws IOException
     */
    public Parser(byte[] axmlBytes, byte[] arscBytes) throws IOException {
        if (axmlBytes == null) {
            throw new IOException();
        }
        if (arscBytes != null) {
            readArsc(arscBytes);
        }

        readAxml(axmlBytes);
    }

    public ManifestInfo getManifestInfo() {
        return manifestInfo;
    }

    private void readArsc(final byte[] arsc) throws IOException {
        List<Pkg> pkgs = null;
        try {
            pkgs = new ArscParser(arsc).parse();
            resources = ArscDumper.dumpRes(pkgs);
        } catch (Exception e) {
            resources = new HashMap<>();
        }
    }

    /**
     * example to read an axml
     *
     * @param androidManifestData The content of AndroidManifest.xml inside apk
     * @throws IOException
     */
    private void readAxml(final byte[] androidManifestData) throws IOException {
        manifestInfo = new ManifestInfo();
        String key = null;
        AxmlParser parser = new AxmlParser(androidManifestData);

        final byte FLAG_DEFAULT = -1;
        final byte FLAG_ACTIVITY = 0;
        final byte FLAG_RECEIVER = 1;
        byte flag = FLAG_DEFAULT;

        out:
        while (true) {
            int event = parser.next();
            switch (event) {
                case AxmlParser.START_FILE:
                    break;
                case AxmlParser.END_FILE:
                    break out;
                case AxmlParser.START_TAG:
                    String tagName = parser.getName();
                    switch (tagName) {
                        case "manifest":
                            parseTagManifest(parser);
                            break;
                        case "application":
                            parseTagApplication(parser);
                            break;
                        case "uses-permission":
                            parseTagPermission(parser);
                            break;
                        case "activity":
                        case "activity-alias":
                            key = parseTagActivity(parser);
                            if (key != null) {
                                flag = FLAG_ACTIVITY;
                            } else {
                                flag = FLAG_DEFAULT;
                            }
                            break;
                        case "service":
                            parseTagService(parser);
                            break;
                        case "receiver":
                            key = parseTagReceiver(parser);
                            if (key != null) {
                                flag = FLAG_RECEIVER;
                            } else {
                                flag = FLAG_DEFAULT;
                            }
                            break;
                        case "action":
                            if (flag == FLAG_ACTIVITY) {
                                parseTagAction(parser, manifestInfo.activities.get(key));
                            } else if (flag == FLAG_RECEIVER) {
                                parseTagAction(parser, manifestInfo.receivers.get(key));

                            }
                            break;
                        case "category":
                            if (flag == FLAG_ACTIVITY) {
                                parseTagCategory(parser, manifestInfo.activities.get(key));
                            } else if (flag == FLAG_RECEIVER) {
                                parseTagCategory(parser, manifestInfo.receivers.get(key));
                            }
                            break;
                        case "meta-data":
                            parseTagMetaData(parser);
                            break;
                    }
                case AxmlParser.END_TAG:
            }
        } // end of while

        ArrayList<String> permArr = new ArrayList<>(manifestInfo.permissions);
        Collections.sort(permArr);
        manifestInfo.requestedPermissions = permArr;
        Collections.sort(manifestInfo.services);
    }

    /**
     * get value from resource.arsc
     *
     * @param obj integer
     * @return value
     */
    private String getValue(Object obj) {
        String value;
        if (obj instanceof Integer) {
            value = resources.get(obj.toString());
            if (value == null) {
                value = obj.toString();
            }
        } else {
            value = obj.toString();
        }

        return value;
    }

    private void parseTagMetaData(AxmlParser parser) {
        String name = null;
        String value = null;
        for (int i = 0; i != parser.getAttributeCount(); ++i) {
            String attrName = parser.getAttrName(i);
            String attrValue = getValue(parser.getAttrResValue(i));

            if (attrName.contains("name")) {
                name = attrValue;
            } else if (attrName.contains("value") || attrName.contains("resource")) {
                value = attrValue;
            }
        }

        if (name != null) {
            manifestInfo.metaData.put(name, value);
        }
    }

    private void parseTagCategory(AxmlParser parser, ArrayList<String> arrayList) {
        for (int i = 0; i != parser.getAttributeCount(); ++i) {
            String attrName = parser.getAttrName(i);
            String attrValue = parser.getAttrResValue(i).toString();

            if (attrName.contains("name")) {
                arrayList.add(attrValue);
                break;
            }
        }
    }

    private String parseTagActivity(AxmlParser parser) {
        for (int i = 0; i != parser.getAttributeCount(); ++i) {
            String attrName = parser.getAttrName(i);
            String attrValue = getValue(parser.getAttrResValue(i));
            if (attrName.equals("name")) {
                manifestInfo.activities.put(attrValue, new ArrayList<String>());
                return attrValue;
            }
        }

        return null;
    }

    private void parseTagAction(AxmlParser parser, ArrayList<String> arrayList) {
        for (int i = 0; i != parser.getAttributeCount(); ++i) {
            String attrName = parser.getAttrName(i);
            String attrValue = parser.getAttrResValue(i).toString();

            if (attrName.contains("name")) {
                arrayList.add(attrValue);
                break;
            }
        }
    }

    private String parseTagReceiver(AxmlParser parser) {
        for (int i = 0; i != parser.getAttributeCount(); ++i) {
            String attrName = parser.getAttrName(i);
            String attrValue = getValue(parser.getAttrResValue(i));


            if (attrName.equals("name")) {
                manifestInfo.receivers.put(attrValue, new ArrayList<String>());
                return attrValue;
            } else if ("".equals(attrName)) {
                manifestInfo.receivers.put(attrValue + "[NOT Default AXML!]", new ArrayList<String>());
                return attrValue + "[NOT Default AXML!]";
            }
        }

        return null;
    }

    private void parseTagService(AxmlParser parser) {
        for (int i = 0; i != parser.getAttributeCount(); ++i) {
            String attrName = parser.getAttrName(i);
            String attrValue = getValue(parser.getAttrResValue(i));

            if (attrName.equals("name")) {
                manifestInfo.services.add(attrValue);
            } else if ("".equals(attrName)) {
                // <service ="service_name">
                manifestInfo.services.add(attrValue + "[NOT Default AXML!]");
            }
        }
    }

    private void parseTagApplication(AxmlParser parser) {
        for (int i = 0; i != parser.getAttributeCount(); ++i) {
            String attrName = parser.getAttrName(i);
            String attrValue = getValue(parser.getAttrResValue(i));


            if (attrName.contains("label")) {
                String tmp = resources.get(attrValue.replace("@", ""));
                if (tmp != null) {
                    manifestInfo.label = tmp;
                } else if (attrValue.startsWith("@0x")){
                    manifestInfo.label = "Unknown";
                } else {
                    manifestInfo.label = attrValue;
                }
            }

            if (attrName.contains("icon")) {
                String icon = resources.get(attrValue.replace("@", ""));
                if (icon != null){
                    manifestInfo.iconPath = icon;
                } else {
                    manifestInfo.iconPath = null;
                }
            }

        }
    }

    private void parseTagManifest(AxmlParser parser) {
        for (int i = 0; i != parser.getAttributeCount(); ++i) {
            String attrName = parser.getAttrName(i);
            String attrValue = parser.getAttrResValue(i).toString();

            if (attrName.contains("versionName")) {
                manifestInfo.versionName = attrValue;
            } else if (attrName.contains("versionCode")) {
                manifestInfo.versionCode = attrValue;
            } else if (attrName.contains("package")) {
                manifestInfo.packageName = attrValue;
            }
        }
    }

    private void parseTagPermission(AxmlParser parser) {
        for (int i = 0; i != parser.getAttributeCount(); ++i) {
            String attrName = parser.getAttrName(i);
            String attrValue = parser.getAttrResValue(i).toString();
            if (attrName.contains("name")) {
                manifestInfo.permissions.add(attrValue);
                break;
            }
        }
    }
}
