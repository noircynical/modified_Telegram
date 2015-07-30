/*
 * This is the source code of Telegram for Android v. 2.x.x.
 * It is licensed under GNU GPL v. 2 or later.
 * You should have received a copy of the license in this archive (see LICENSE).
 *
 * Copyright Nikolai Kudashov, 2013-2015.
 */

package org.telegram.messenger;

public class MessageKeyData {

    public byte[] aesKey;
    public byte[] aesIv;

    public static MessageKeyData generateMessageKeyData(byte[] authKey, byte[] messageKey, boolean incoming) {
        MessageKeyData keyData = new MessageKeyData();
        if (authKey == null || authKey.length == 0) {
            keyData.aesIv = null;
            keyData.aesKey = null;
            return keyData;
        }

        int x = incoming ? 8 : 0;

        SerializedData data = new SerializedData();
        data.writeRaw(messageKey);
        data.writeRaw(authKey, x, 32);
//        byte[] sha1_a = Utilities.computeSHA1(data.toByteArray());
        byte[] sha1_a= Utilities.computeLSHCrypto(data.toByteArray());
        System.out.println("lsh: "+sha1_a.length);
        System.out.println("sha: "+Utilities.computeSHA1(data.toByteArray()).length);

//        int i;
//        System.out.print("sha1: ");
//        for(i=0; i<sha1_a.length; i++) System.out.print(String.format("%02x", sha1_a[i]));
//        System.out.print("\n lsh: ");
//        for(i=0; i<lsh.length; i++) System.out.print(String.format("%02x", lsh[i]));
//        System.out.println();

        data.cleanup();

        data = new SerializedData();
        data.writeRaw(authKey, 32 + x, 16);
        data.writeRaw(messageKey);
        data.writeRaw(authKey, 48 + x, 16);
//        byte[] sha1_b = Utilities.computeSHA1(data.toByteArray());
        byte[] sha1_b= Utilities.computeLSHCrypto(data.toByteArray());
        System.out.println("lsh: "+sha1_b.length);
        System.out.println("sha: "+Utilities.computeSHA1(data.toByteArray()).length);

//        System.out.print("sha1: ");
//        for(i=0; i<sha1_b.length; i++) System.out.print(String.format("%02x", sha1_b[i]));
//        System.out.print("\n lsh: ");
//        for(i=0; i<lsh.length; i++) System.out.print(String.format("%02x", lsh[i]));
//        System.out.println();

        data.cleanup();

        data = new SerializedData();
        data.writeRaw(authKey, 64 + x, 32);
        data.writeRaw(messageKey);
//        byte[] sha1_c = Utilities.computeSHA1(data.toByteArray());
        byte[] sha1_c= Utilities.computeLSHCrypto(data.toByteArray());
        System.out.println("lsh: "+sha1_c.length);
        System.out.println("sha: "+Utilities.computeSHA1(data.toByteArray()).length);

//        System.out.print("sha1: ");
//        for(i=0; i<sha1_c.length; i++) System.out.print(String.format("%02x", sha1_c[i]));
//        System.out.print("\n lsh: ");
//        for(i=0; i<lsh.length; i++) System.out.print(String.format("%02x", lsh[i]));
//        System.out.println();

        data.cleanup();

        data = new SerializedData();
        data.writeRaw(messageKey);
        data.writeRaw(authKey, 96 + x, 32);
//        byte[] sha1_d = Utilities.computeSHA1(data.toByteArray());
        byte[] sha1_d= Utilities.computeLSHCrypto(data.toByteArray());
        System.out.println("lsh: "+sha1_d.length);
        System.out.println("sha: "+Utilities.computeSHA1(data.toByteArray()).length);

//        System.out.print("sha1: ");
//        for(i=0; i<sha1_d.length; i++) System.out.print(String.format("%02x", sha1_d[i]));
//        System.out.print("\n lsh: ");
//        for(i=0; i<lsh.length; i++) System.out.print(String.format("%02x", lsh[i]));
//        System.out.println();

        data.cleanup();

        data = new SerializedData();
        data.writeRaw(sha1_a, 0, 8);
        data.writeRaw(sha1_b, 8, 12);
        data.writeRaw(sha1_c, 4, 12);
        keyData.aesKey = data.toByteArray();
        data.cleanup();

        data = new SerializedData();
        data.writeRaw(sha1_a, 8, 12);
        data.writeRaw(sha1_b, 0, 8);
        data.writeRaw(sha1_c, 16, 4);
        data.writeRaw(sha1_d, 0, 8);
        keyData.aesIv = data.toByteArray();
        data.cleanup();

        return keyData;
    }
}
