//
// Created by 구자은 on 2015. 7. 17..
//

#include "com_lsh_lshlibrary_MainActivity.h"
#include <android/log.h>

//JNIEXPORT void Java_org_telegram_messenger_Utilities_aesIgeEncryption(JNIEnv *env, jclass class, jobject buffer, jbyteArray key, jbyteArray iv, jboolean encrypt, int offset, int length) {
//    jbyte *what = (*env)->GetDirectBufferAddress(env, buffer) + offset;
//    unsigned char *keyBuff = (unsigned char *)(*env)->GetByteArrayElements(env, key, NULL);
//    unsigned char *ivBuff = (unsigned char *)(*env)->GetByteArrayElements(env, iv, NULL);
//
//    AES_KEY akey;
//    if (!encrypt) {
//        AES_set_decrypt_key(keyBuff, 32 * 8, &akey);
//        AES_ige_encrypt(what, what, length, &akey, ivBuff, AES_DECRYPT);
//    } else {
//        AES_set_encrypt_key(keyBuff, 32 * 8, &akey);
//        AES_ige_encrypt(what, what, length, &akey, ivBuff, AES_ENCRYPT);
//    }
//    (*env)->ReleaseByteArrayElements(env, key, keyBuff, JNI_ABORT);
//    (*env)->ReleaseByteArrayElements(env, iv, ivBuff, 0);
//}

JNIEXPORT jbyteArray JNICALL Java_com_lsh_lshlibrary_MainActivity_LSH256 (JNIEnv *env, jobject obj, jbyteArray data, jint databitlen){
//    size_t len= (unsigned int)(*env)->GetByteArrayElements(env, data, 0);
//    unsigned char *inputData= (unsigned char*)malloc((int)databitlen/8);
    unsigned char *outputData= (unsigned char *)malloc(32*sizeof(unsigned char));
    unsigned char *inputData= (unsigned char *)(*env)->GetByteArrayElements(env, data, NULL);
    jbyteArray hashval= (*env)->NewByteArray(env, 32);

    // strncpy(inputData, (const unsigned char*)data, (int)databitlen/8);
//    (*env)->GetByteArrayRegion(env, data, 0, 32, inputData);
    hash256(256, inputData, (int)databitlen, outputData);
//    (*env)->SetByteArrayRegion(env, hashval, 0, 32, (jbyte*)outputData);
    (*env)->ReleaseByteArrayElements(env, hashval, (jbyte*)outputData, 0);
    free(inputData);
    free(outputData);
    return hashval;
}