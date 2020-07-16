package com.dingxiang.mobile.test;

import android.content.Context;

import com.dx.mobile.risk.utils.ContextFinder;
import com.dx.mobile.risk.utils.StringUtils;
import com.dx.mobile.risk.dx.a;

import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(MockitoJUnitRunner.class)
public class ExampleUnitTest {

    @Mock
    Context mMockContext;

    @Test
    public void test_a(){

        ContextFinder.setTokenContext(mMockContext);
        for (int i = 1; i <= 42; i++) {
            System.out.println("test_a i:"+i);
            try {
                String result = a.a(i);
                System.out.println("test_a result:"+result);
//                assertNotEquals("", result);
            } catch (Exception e) {
            }
        }

    }


    @Test
    public void test_StringUtils(){
        Assert.assertTrue(StringUtils.isEmpty(""));
        Assert.assertTrue(StringUtils.isEmpty(null));

        String str = "dingxiang";
        byte[] compress = StringUtils.compress(str);
        String decompress = StringUtils.decompress(compress);
        Assert.assertEquals(decompress, str);
    }
}