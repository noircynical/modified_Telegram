package org.telegram.messenger;

import java.util.Arrays;

public class LSHCrypto {
    public final static int nCycle= 26;
    public final static int sc256[][]= {
            { 0x917caf90, 0x6c1b10a2, 0x6f352943, 0xcf778243, 0x2ceb7472, 0x29e96ff2, 0x8a9ba428, 0x2eeb2642 },
            { 0x0e2c4021, 0x872bb30e, 0xa45e6cb2, 0x46f9c612, 0x185fe69e, 0x1359621b, 0x263fccb2, 0x1a116870 },
            { 0x3a6c612f, 0xb2dec195, 0x02cb1f56, 0x40bfd858, 0x784684b6, 0x6cbb7d2e, 0x660c7ed8, 0x2b79d88a },
            { 0xa6cd9069, 0x91a05747, 0xcdea7558, 0x00983098, 0xbecb3b2e, 0x2838ab9a, 0x728b573e, 0xa55262b5 },
            { 0x745dfa0f, 0x31f79ed8, 0xb85fce25, 0x98c8c898, 0x8a0669ec, 0x60e445c2, 0xfde295b0, 0xf7b5185a },
            { 0xd2580983, 0x29967709, 0x182df3dd, 0x61916130, 0x90705676, 0x452a0822, 0xe07846ad, 0xaccd7351 },
            { 0x2a618d55, 0xc00d8032, 0x4621d0f5, 0xf2f29191, 0x00c6cd06, 0x6f322a67, 0x58bef48d, 0x7a40c4fd },
            { 0x8beee27f, 0xcd8db2f2, 0x67f2c63b, 0xe5842383, 0xc793d306, 0xa15c91d6, 0x17b381e5, 0xbb05c277 },
            { 0x7ad1620a, 0x5b40a5bf, 0x5ab901a2, 0x69a7a768, 0x5b66d9cd, 0xfdee6877, 0xcb3566fc, 0xc0c83a32 },
            { 0x4c336c84, 0x9be6651a, 0x13baa3fc, 0x114f0fd1, 0xc240a728, 0xec56e074, 0x009c63c7, 0x89026cf2 },
            { 0x7f9ff0d0, 0x824b7fb5, 0xce5ea00f, 0x605ee0e2, 0x02e7cfea, 0x43375560, 0x9d002ac7, 0x8b6f5f7b },
            { 0x1f90c14f, 0xcdcb3537, 0x2cfeafdd, 0xbf3fc342, 0xeab7b9ec, 0x7a8cb5a3, 0x9d2af264, 0xfacedb06 },
            { 0xb052106e, 0x99006d04, 0x2bae8d09, 0xff030601, 0xa271a6d6, 0x0742591d, 0xc81d5701, 0xc9a9e200 },
            { 0x02627f1e, 0x996d719d, 0xda3b9634, 0x02090800, 0x14187d78, 0x499b7624, 0xe57458c9, 0x738be2c9 },
            { 0x64e19d20, 0x06df0f36, 0x15d1cb0e, 0x0b110802, 0x2c95f58c, 0xe5119a6d, 0x59cd22ae, 0xff6eac3c },
            { 0x467ebd84, 0xe5ee453c, 0xe79cd923, 0x1c190a0d, 0xc28b81b8, 0xf6ac0852, 0x26efd107, 0x6e1ae93b },
            { 0xc53c41ca, 0xd4338221, 0x8475fd0a, 0x35231729, 0x4e0d3a7a, 0xa2b45b48, 0x16c0d82d, 0x890424a9 },
            { 0x017e0c8f, 0x07b5a3f5, 0xfa73078e, 0x583a405e, 0x5b47b4c8, 0x570fa3ea, 0xd7990543, 0x8d28ce32 },
            { 0x7f8a9b90, 0xbd5998fc, 0x6d7a9688, 0x927a9eb6, 0xa2fc7d23, 0x66b38e41, 0x709e491a, 0xb5f700bf },
            { 0x0a262c0f, 0x16f295b9, 0xe8111ef5, 0x0d195548, 0x9f79a0c5, 0x1a41cfa7, 0x0ee7638a, 0xacf7c074 },
            { 0x30523b19, 0x09884ecf, 0xf93014dd, 0x266e9d55, 0x191a6664, 0x5c1176c1, 0xf64aed98, 0xa4b83520 },
            { 0x828d5449, 0x91d71dd8, 0x2944f2d6, 0x950bf27b, 0x3380ca7d, 0x6d88381d, 0x4138868e, 0x5ced55c4 },
            { 0x0fe19dcb, 0x68f4f669, 0x6e37c8ff, 0xa0fe6e10, 0xb44b47b0, 0xf5c0558a, 0x79bf14cf, 0x4a431a20 },
            { 0xf17f68da, 0x5deb5fd1, 0xa600c86d, 0x9f6c7eb0, 0xff92f864, 0xb615e07f, 0x38d3e448, 0x8d5d3a6a },
            { 0x70e843cb, 0x494b312e, 0xa6c93613, 0x0beb2f4f, 0x928b5d63, 0xcbf66035, 0x0cb82c80, 0xea97a4f7 },
            { 0x592c0f3b, 0x947c5f77, 0x6fff49b9, 0xf71a7e5a, 0x1de8c0f5, 0xc2569600, 0xc4e4ac8c, 0x823c9ce1 }
    };
    public final static int IV256[] = {
            0x46a10f1f, 0xfddce486, 0xb41443a8, 0x198e6b9d, 0x3304388d, 0xb0f5a3c7, 0xb36061c4, 0x7adbd553,
            0x105d5378, 0x2f74de54, 0x5c2f2d95, 0xf2553fbe, 0x8051357a, 0x138668c8, 0x47aa4484, 0xe01afb41
    };
    public final static int gamma256[] = { 0, 8, 16, 24, 24, 16, 8, 0 };

    // lsh-256-224, lsh-256, lsh-512-224, lsh-512-256, lsh-512-384, lsh-512
    // lsh-maxhashbitlen-hashbitlen
    public static byte[] LSH(byte[] convertme, int offset, int hashlen){
        if(convertme == null) return null;
        hash256 hash= new hash256();
        if(init(hash,hashlen)) return null;
        update(hash, convertme);
        return finalProcess(hash);
    }

    private static boolean init(hash256 hash, int hashlen){
        int i, j, k, vl, vr;
        int[] T= new int[16];
        if(hashlen<0 || hashlen>256) return true;
        else if(hashlen == 256) System.arraycopy(IV256, 0, hash.cv256, 0, IV256.length);
        else{
            Arrays.fill(hash.cv256, 0);
            hash.cv256[0]= 32;
            hash.cv256[1]= hashlen;

            for(j=0; j<nCycle/2; j++){
                k= j<<1;
                for(i=0; i<8; i++){
                    vl= hash.cv256[i];
                    vr= hash.cv256[i+8];
                    vl= ((((vl+vr)<<29)|((vl+vr)>>3)))^sc256[k][i];
                    vr= (((vl+vr)<<1)|((vl+vr)>>31));
                    T[i]= vr+vl;
                    T[i+8]= ((vr<<gamma256[i])|(vr>>gamma256[i]));
                }
                hash.cv256[ 0]= T[ 6];hash.cv256[ 8]= T[ 2];
                hash.cv256[ 1]= T[ 4];hash.cv256[ 9]= T[ 0];
                hash.cv256[ 2]= T[ 5];hash.cv256[10]= T[ 1];
                hash.cv256[ 3]= T[ 7];hash.cv256[11]= T[ 3];
                hash.cv256[ 4]= T[12];hash.cv256[12]= T[ 8];
                hash.cv256[ 5]= T[15];hash.cv256[13]= T[11];
                hash.cv256[ 6]= T[14];hash.cv256[14]= T[10];
                hash.cv256[ 7]= T[13];hash.cv256[15]= T[ 9];

                k= 1+(j<<1);
                for(i=0; i<8; i++){
                    vl= hash.cv256[i];
                    vr= hash.cv256[i+8];
                    vl= (((vl+vr)<<5)|((vl+vr)>>27))^sc256[k][i];
                    vr= (((vl+vr)<<17)|((vl+vr)>>15));
                    T[i]= vr+vl;
                    T[i+8]= ((vr<<gamma256[i])|(vr>>gamma256[i]));
                }
                hash.cv256[ 0]= T[ 6];hash.cv256[ 8]= T[ 2];
                hash.cv256[ 1]= T[ 4];hash.cv256[ 9]= T[ 0];
                hash.cv256[ 2]= T[ 5];hash.cv256[10]= T[ 1];
                hash.cv256[ 3]= T[ 7];hash.cv256[11]= T[ 3];
                hash.cv256[ 4]= T[12];hash.cv256[12]= T[ 8];
                hash.cv256[ 5]= T[15];hash.cv256[13]= T[11];
                hash.cv256[ 6]= T[14];hash.cv256[14]= T[10];
                hash.cv256[ 7]= T[13];hash.cv256[15]= T[ 9];
            }
        }
        hash.hashbitlen= hashlen;
        return false;
    }
    private static void update(hash256 hash, byte[] convertme){
        int numBlocks, temp, pos1, pos2, i, databitlen= convertme.length;
        numBlocks= (databitlen>>10)+1;
        for(i=0; i<numBlocks-1; i++){
            compress(hash);
        }

        if((databitlen&0x3ff) != 0){
            temp= (numBlocks-1)<<7;
            pos1= (databitlen>>3)-temp;
            pos2= databitlen&0x7;

            System.arraycopy(convertme, 0, hash.last256, 0, pos1);
            if(pos2 != 0) hash.last256[pos1]= (byte)((convertme[pos1]&(0xff<<(8-pos2)))^(1<<(7-pos2)));
            else hash.last256[pos1]= (byte)0x80;
            if(pos1 != 127) Arrays.fill(hash.last256, pos1+1, pos1+1+(127-pos1), (byte)0x0);
        }else{

        }
    }
    private static byte[] finalProcess(hash256 hash){
        int pos1, pos2, l;
        int[] H= new int[8];
        byte[] output= new byte[hash.hashbitlen>>3];

        pos1= hash.last256.length>>3;
        pos2= hash.last256.length&0x7;
        if(pos2 != 0) hash.last256[pos1] |= (1<<(7-pos2));
        else hash.last256[pos1]= (byte)0x80;

        compress(hash);
        for(l= 0; l<8; l++) H[l]= hash.cv256[l]^hash.cv256[l+8];
        for(l= 0; l<(hash.hashbitlen>>3); l++) output[l]= (byte)(H[l>>2]>>((l<<3)&0x1f));
        return output;
    }
    private static void compress(hash256 hash){
        int j, l, k, vl, vr;
        int[] m= new int[(nCycle+1)<<4];
        int[] T= new int[16];

        for(l=0; l<32; l++) m[l]=(int)hash.last256[l];
        for(j= 2; j<= nCycle; j++){
            k= j<<4;
            m[k+ 0]=m[k-16]+m[k-29];
            m[k+ 1]=m[k-15]+m[k-30];
            m[k+ 2]=m[k-14]+m[k-32];
            m[k+ 3]=m[k-13]+m[k-31];
            m[k+ 4]=m[k-12]+m[k-25];
            m[k+ 5]=m[k-11]+m[k-28];
            m[k+ 6]=m[k-10]+m[k-27];
            m[k+ 7]=m[k- 9]+m[k-26];
            m[k+ 8]=m[k- 8]+m[k-21];
            m[k+ 9]=m[k- 7]+m[k-22];
            m[k+10]=m[k- 6]+m[k-24];
            m[k+11]=m[k- 5]+m[k-23];
            m[k+12]=m[k- 4]+m[k-17];
            m[k+13]=m[k- 3]+m[k-20];
            m[k+14]=m[k- 2]+m[k-19];
            m[k+15]=m[k- 1]+m[k-18];
        }

        for(j= 0; j<(nCycle>>1); j++){
            k= j<<1;
            for(l= 0; l<8; l++){
                vl= hash.cv256[l]^m[(k<<4)+l];
                vr= hash.cv256[l+8]^m[(k<<4)+l+8];
                vl= ((((vl+vr)<<29)|((vl+vr)>>3)))^sc256[k][l];
                vr= (((vl+vr)<<1)|((vl+vr)>>31));
                T[l]= vr+vl;
                T[l+8]= ((vr<<gamma256[l])|(vr>>gamma256[l]));
            }
            hash.cv256[ 0]= T[ 6];hash.cv256[ 8]= T[ 2];
            hash.cv256[ 1]= T[ 4];hash.cv256[ 9]= T[ 0];
            hash.cv256[ 2]= T[ 5];hash.cv256[10]= T[ 1];
            hash.cv256[ 3]= T[ 7];hash.cv256[11]= T[ 3];
            hash.cv256[ 4]= T[12];hash.cv256[12]= T[ 8];
            hash.cv256[ 5]= T[15];hash.cv256[13]= T[11];
            hash.cv256[ 6]= T[14];hash.cv256[14]= T[10];
            hash.cv256[ 7]= T[13];hash.cv256[15]= T[ 9];

            k= (j<<1)+1;
            for(l= 0; l<8; l++){
                vl= hash.cv256[l]^m[(k<<4)+l];
                vr= hash.cv256[l+8]^m[(k<<4)+l+8];
                vl= ((((vl+vr)<<5)|((vl+vr)>>27)))^sc256[k][l];
                vr= (((vl+vr)<<1)|((vl+vr)>>31));
                T[l]= vr+vl;
                T[l+8]= ((vr<<gamma256[l])|(vr>>gamma256[l]));
            }
            hash.cv256[ 0]= T[ 6];hash.cv256[ 8]= T[ 2];
            hash.cv256[ 1]= T[ 4];hash.cv256[ 9]= T[ 0];
            hash.cv256[ 2]= T[ 5];hash.cv256[10]= T[ 1];
            hash.cv256[ 3]= T[ 7];hash.cv256[11]= T[ 3];
            hash.cv256[ 4]= T[12];hash.cv256[12]= T[ 8];
            hash.cv256[ 5]= T[15];hash.cv256[13]= T[11];
            hash.cv256[ 6]= T[14];hash.cv256[14]= T[10];
            hash.cv256[ 7]= T[13];hash.cv256[15]= T[ 9];
        }
        for(l= 0; l<16; l++) hash.cv256[l]^=m[(nCycle<<4)+l];
    }

    static class hash256{
        int hashbitlen= 0;
        byte[] last256= null;
        int[] cv256= null;

        hash256(){
            last256= new byte[128];
            cv256= new int[16];
            Arrays.fill(last256, (byte)0);
        }
    }
}