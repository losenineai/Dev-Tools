/*
 * Copyright 2008 Android4ME
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.dingxiang.parser.android.axml.res;


import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Dmitry Skiba
 *         <p/>
 *         Parser for Android's binary xml files (axml).
 *         <p/>
 *         Parser follows XmlPullParser interface, but does not implement it.
 *         You can implement XmlPullParser ontop of this class and contribute
 *         it to Android4ME project. See http://code.google.com/p/android4me
 *         <p/>
 *         TODO:
 *         - clarify interface methods,
 *         not all behavior from XmlPullParser is supported
 *         - add more sanity checks
 *         - understand ? values
 */
public class AXMLParser {

    public static final int
            CHUNK_AXML_FILE = 0x00080003,
            CHUNK_RESOURCEIDS = 0x00080180,
            CHUNK_XML_FIRST = 0x00100100,
            CHUNK_XML_START_NAMESPACE = 0x00100100,
            CHUNK_XML_END_NAMESPACE = 0x00100101,
            CHUNK_XML_START_TAG = 0x00100102,
            CHUNK_XML_END_TAG = 0x00100103,
            CHUNK_XML_TEXT = 0x00100104,
            CHUNK_XML_LAST = 0x00100104;
    private IntReader m_reader;
    @SuppressWarnings("unused")
    private InputStream m_stream;
    private int m_offset;
    private int m_event = 0;
    private boolean m_started;
    private StringBlock m_strings;
    private int[] m_resourceIDs;
    private int m_tagType;
    private int m_tagSourceLine;
    private int m_tagName;
    private TagAttribute[] m_tagAttributes;
    private Map<Integer, Integer> m_namespaces = new HashMap<Integer, Integer>();

    ///////////////////////////////////////////// implementation

    public AXMLParser(InputStream stream) throws IOException {
        m_reader = new IntReader(stream, false);
        m_offset = 0;
        m_started = false;
        start();
    }

    // See next() in XmlPullParser.
    public int next() throws IOException {

        m_tagType = m_reader.readInt();/*other 3 bytes?*/

        if (m_tagType == CHUNK_RESOURCEIDS) {
            int chunkSize = m_reader.readInt();
            if (chunkSize < 8 || (chunkSize % 4) != 0) {
                throw new IOException("Invalid resource ids size (" + chunkSize + ").");
            }
            m_resourceIDs = m_reader.readIntArray(chunkSize / 4 - 2);
            return next();
        }

        if (m_tagType < CHUNK_XML_FIRST || m_tagType > CHUNK_XML_LAST) {
            throw new IOException("Invalid chunk type (" + m_tagType + ").");
        }


		/*common header*/
        /*some source length*/
        m_reader.readInt();
        m_tagSourceLine = m_reader.readInt();
		/*0xFFFFFFFF*/
        m_reader.readInt();

        m_tagName = -1;
        m_tagAttributes = null;


        switch (m_tagType) {
            case CHUNK_XML_START_NAMESPACE: {
                int prefix = m_reader.readInt();
                int uri = m_reader.readInt();
                m_namespaces.put(uri, prefix);
                m_event++;
                if (m_event == 1)
                    return 0;
                return next();
            }
            case CHUNK_XML_START_TAG: {
				/*0xFFFFFFFF*/
                m_reader.readInt();
                m_tagName = m_reader.readInt();
				/*flags?*/
                m_reader.readInt();
                int attributeCount = m_reader.readInt();
				/*?*/
                m_reader.readInt();
                m_tagAttributes = new TagAttribute[attributeCount];
                for (int i = 0; i != attributeCount; ++i) {
                    TagAttribute attribute = new TagAttribute();
                    attribute.namespace = m_reader.readInt();
                    attribute.name = m_reader.readInt();
                    attribute.valueString = m_reader.readInt();
                    attribute.valueType = (m_reader.readInt() >>> 24);/*other 3 bytes?*/
                    attribute.value = m_reader.readInt();
                    m_tagAttributes[i] = attribute;
                }
                break;
            }
            case CHUNK_XML_END_TAG: {
				/*0xFFFFFFFF*/
                m_reader.readInt();
                m_tagName = m_reader.readInt();
                break;
            }
            case CHUNK_XML_TEXT: {
                m_tagName = m_reader.readInt();
				/*?*/
                m_reader.readInt();
				/*?*/
                m_reader.readInt();
                break;
            }
            case CHUNK_XML_END_NAMESPACE: {
				/*namespace?*/
                m_reader.readInt();
				/*name?*/
                m_reader.readInt();
                m_event--;
                if (m_event == 0)
                    return 1;
                else
                    return next();
            }
            default: {
                throw new IOException("Invalid tag type (" + m_tagType + ").");
            }
        }
        return m_tagType;
    }

    // See getEventType() in XmlPullParser.
    public int getEventType() {
        return m_tagType;
    }


//	private final String readString() throws IOException {
//		int length=readShort();
//		StringBuilder builder=new StringBuilder(length);
//		for (int i=0;i!=length;++i) {
//			builder.append((char)readShort());
//		}
//		readShort();
//		return builder.toString();
//	}

    /////////////////////////////////// data

    // See getName() in XmlPullParser.
    public String getName() {
        if (m_tagName == -1) {
            return null;
        }
        return m_strings.getString(m_tagName);
    }

    // See getLineNumber() in XmlPullParser.
    public int getLineNumber() {
        return m_tagSourceLine;
    }

    // See getAttributeCount() in XmlPullParser.
    public int getAttributeCount() {
        if (m_tagAttributes == null) {
            return -1;
        }
        return m_tagAttributes.length;
    }

    // See getAttributeNamespace() in XmlPullParser.
    public String getAttributeNamespace(int index) {
        return m_strings.getString(getAttribute(index).namespace);
    }

    public String getAttributePrefix(int index) {
        int uri = getAttribute(index).namespace;
        Integer prefix = m_namespaces.get(uri);
        if (prefix == null) {
            return "";
        }
        return m_strings.getString(prefix);
    }

    // See getAttributeName() in XmlPullParser.
    public String getAttributeName(int index) {
        return m_strings.getString(getAttribute(index).name);
    }

    // Returns resource ID for attribute name.
    public int getAttributeNameResourceID(int index) {
        int resourceIndex = getAttribute(index).name;
        if (m_resourceIDs == null ||
                resourceIndex < 0 || resourceIndex >= m_resourceIDs.length) {
            return 0;
        }
        return m_resourceIDs[resourceIndex];
    }

    // See TypedValue.TYPE_ values.
    public int getAttributeValueType(int index) {
        return getAttribute(index).valueType;
    }

    // Returns string value if attribute type is TypedValue.TYPE_STRING,
    //  or empty string otherwise.
    public String getAttributeValueString(int index) {
        return m_strings.getString(getAttribute(index).valueString);
    }

    // Returns integer value for attribute.
    // Value interpretation is based on type.
    // For TypedValue.TYPE_STRING meaning is unknown.
    public int getAttributeValue(int index) {
        return getAttribute(index).value;
    }

    private void start() throws IOException {
        if (m_started) {
            return;
        }
        m_started = true;

        int signature = m_reader.readInt();
        if (signature != CHUNK_AXML_FILE) {
            throw new IOException("Invalid signature (" + signature + ").");
        }
		/*chunk size*/
        m_reader.skipInt();

        m_strings = new StringBlock(m_reader);

        // Align to 4byte boundary
        m_reader.readInt(m_offset % 4);

		/*chunk signature*/
        m_reader.readInt();
        int resourceIDLength = m_reader.readInt() - 8;

        if (resourceIDLength < 0) {
            throw new IOException("Invalid resource id length (" + resourceIDLength + ").");
        }
        m_resourceIDs = new int[resourceIDLength / 4];
        for (int i = 0; i != m_resourceIDs.length; ++i) {
            m_resourceIDs[i] = m_reader.readInt();
        }
    }

    private TagAttribute getAttribute(int index) {
        if (m_tagAttributes == null) {
            throw new IndexOutOfBoundsException("Attributes are not available.");
        }
        if (index >= m_tagAttributes.length) {
            throw new IndexOutOfBoundsException("Invalid attribute index (" + index + ").");
        }
        return m_tagAttributes[index];
    }

    private static final class TagAttribute {
        public int namespace;
        public int name;
        public int valueString;
        public int valueType;
        public int value;
    }


}