import sun.security.pkcs.PKCS7;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.security.GeneralSecurityException;
import java.security.cert.X509Certificate;
public class SignApk {
    public static X509Certificate readSignatureBlock(InputStream in) throws IOException, GeneralSecurityException {
        PKCS7 pkcs7 = new PKCS7(in);
        return pkcs7.getCertificates()[0];
    }
    public static void main(String[] args) throws FileNotFoundException, IOException, GeneralSecurityException {
        if (args.length != 1) {
            System.out.println("not input CERT file.");
            return;
        }
        X509Certificate publicKey = readSignatureBlock(new FileInputStream(args[0]));

        System.out.printf("%08X\n", publicKey.hashCode());
        // System.out.println("issuer:" + publicKey.getIssuerDN());
        // System.out.println("subject:" + publicKey.getSubjectDN());
        // System.out.println(publicKey.getPublicKey());
    }
}