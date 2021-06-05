//
//  prefix.h
//  STEEForiOS
//
//  Created by xelz on 2017/7/19.
//  Copyright © 2017年 echonn. All rights reserved.
//

#ifndef dx_utils_crypto_prefix_h
#define dx_utils_crypto_prefix_h

// aes
#define AES_init_ctx_iv             ol1io9lOlgl0ogoo1I101io0IgLl9L1g
#define AES_ctx_set_iv              iogilOi0oiL10O991OOOO1lo0olOgL0l
#define AES_ECB_encrypt             i1g0O1010loiOo1ggI1LO1LooIgIiLlo
#define Cipher                      L9OiLLIIl190O00oOOioIIg10Oio0g1L
#define AES_ECB_decrypt             gL1Og9L0o0oIlioL0LlOLioiiLOlL99i
#define InvCipher                   ig9LO0LlL9giI1oi1119OlLLgl19iO1I
#define AES_CBC_encrypt_buffer      OlLILoiLLioO0lglLL1OoOIgLiI09ILl
#define AES_CBC_decrypt_buffer      og1OigI0OgLiILi001ioOOLO0OoII11I
#define AES_CTR_xcrypt_buffer       oOO0LIOO9iiiioO9oiOI9g9gilLIiOLo

// aes_mini
#define AESMini_Init_Encrypt        lo0l1911gLoL90o9OlL90oio0OIiiooi
#define AESMini_Init_Decrypt        o0l09Il1gOI1i00oOii9i1lo0IOgoL90
#define SxBytesRot8                 i9gOO0LoO1LogLO0il99ILgiII1Ig0O1
#define SxBytes                     O1OIgoo191l0IoOO199oiIg9111og9L0
#define AESMini_ECB_Encrypt         LoIlgoIoi9oILgIggo0ii9oogOi9glii
#define Te0123                      IOlll9OL9IO1iLgoOi9gI9OO1IlOIgli
#define encLastRound                lILIL0lgigLLI1iiiogg0IoglOlO1011
#define AESMini_ECB_Decrypt         lgOog9gLioLl9I1OioLOlLlO99OLIii1
#define Td0123                      lo9g9oiOLOoool91loo9Ig0LL1Ool9li
#define decLastRound                i910L1o19L1iLoLIO19iI91Ol9l9010g
#define AESMini_Init                I99g1ig9OIlIigO19l0IioILl9LL1lo0

// base64
#define base64_encode_imp           L1LO9L9ogOl99gi1io0O900oLogLlOiO
#define base64_decode_imp           Li0l9g9g0gOOgioiI09oiLOIl9li10i9

// hmac_md5
#define hmac_md5                    iL1gI190L11ii9LO9OIi00OliOil1111

// md5
#define MD5Init                     ILgOO000000golgo9iLoLiLOo1lLioO9
#define MD5Update                   I0iIi0lO1O1LiO9i00lg1ILL1OOgg1i1
#define MD5Transform                iL1gI190L11ii9LO9OIi00OliOil101i
#define MD5Final                    lI9OLIlOlli11ILioioil0iL09I99loL
#define Encode                      io0ilI9L11iglll9i1l1LiII099lLLig
#define only_md5                     OlllgLl999o0Iilg0oi9gLgLoOl0iil0

// mtkey
#define kex2k                       ig1L0llI010I1IiLLO0IoIO9gLoLL901
#define kp_set_api                  Ooi0lo1O091I9I9Li90gLoOI0i9gILoO
#define kp_unset_api                i0iiL0g1igi0gO1g1IOLg11lil9I9OiO
#define kp_encode_base              LOlgI01OioOIIl9LLoLlLO9iL09O11g1
#define kp_decode_base              o01gLIOlLI19gili09oII1o1liILI0o0
#define kp_encode                   L9li1g9iL9iOoo91i9lIOLgi0L1O0o0L
#define kp_decode                   gLOl9OO1lO1g1gO9i9OLoilgiog0i9g0

// rc4
#define rc4_init                    gg190ilLloI0liiLg9OLoLoIl1g9l9o0
#define rc4_crypt                   L1II11l9gIOig0i9oIOiiI1l0liOi0oI

// sha1
#define sha1_init                   gIo90glo999io9LolggliO0gOoigl9il
#define rotl32                      g01iILlIgoLLILIoiiLo91lolgOOilOg
#define change_endian32             l01I9l19gL09ooi9lI0oil99I19OloL9
#define ch                          OLiLl1iigLOLIii1IlIlg9IlI10Oo9L0
#define maj                         IoO1L1I0I9gil01ilOoLOIL10iO99go9
#define parity                      LI9IlL1lOl0LOo0oloL1olOL09iO9Ill
#define sha1_nextBlock              iooiI9L99IlOLoO9g0li1i1oiIo1I9lo
#define sha1_lastBlock              oggoL11oiO9LglOoOgLO191LlOI19lil
#define sha1_ctx2hash               g9O099Ilgi1lilggigiOI90IL9l099Lo
#define sha1                        iollgL9LO0LlII9Oil1oIoiiooIioo0O

// hmac_sha1
#define hmac_sha1                   oioog9IOl9l9gl0oo0I1gi0o0oLL91i1

// hmac_sha256
#define hmac_sha256                 iL11LLg19OILLLoLglOi9LIiig1o9i9o

// sha256
#define SHA256Mini_Init             ggoiI1IlLO0lOIIig10gigg1lOogLOiO
#define SHA256Mini_Update           lL9oI11oOOIo0L1I1oggOg1oOlLL90ii
#define SHA256Mini_compress         ogLiLL11giolIIIOI9OIgLIgO9Li099I
#define SHA256Mini_Final            iL0oo9oIIOl9lLI0i11O9III11L1O99l
#define SHA256Mini                  Oo19llIIg1iIg9IiLOl1lIoig1Oo9g1g
#define put_BE32                    O09o10gOoLO00loiIlgLlO9g0LOO0LgO
#define get_BE32                    IIiiO9gLl919lILoO0IgO1iLo1g90gIO

// hmac sm3
#define sm3_hmac_init               IILiIg0O0I1LiL0OOIo990OIILOO0lil
#define sm3_hmac_update             O99l1l00oI0LILg01l0ii0oI9LgOoIl0
#define sm3_hmac_final              I19o1g0l0090oIo99gIo9Oogll1lI99g
#define sm3_hmac                    l9i1L1O0Oioil9oOOLo919l9gilliLiL

// sm3
#define sm3_init                    g0I0gLgiooooiOIoIi01LIi0o919Li0g
#define sm3_compress                oiOLOO19li0iggOo9oOIgggL0lILLO0g
#define sm3_update                  giOOlooilO00OOl9g1oOoi9I9II1gOgg
#define sm3_final                   gil1L1oooOLi019lgi0o1Ll0glIoo09i
#define sm3_encrypt                 io0OlliL10O910lg19Iol1oI0l1I9li0

// sm4
#define search_s_box                l10g00OOILIOiLIO1g1ogI00oi199l91
#define cal_ci_rk                   loglgi091l0Ilg9o0LIo0liOiigl9g9i
#define set_key                     iioogoOli19LOg0O19goiIlgOiOL01Ig
#define sm4Lt                       g1OoO1llooL9iL1liOllo11L1gIlgg1o
#define sm_4f                       OL1oII0l00ollOllli99gl90O1l0goO0
#define one_round                   il9LiOi9l9l9llOio11OlOoOlL999IL1
#define sm4_crypt                   lLLol9i9IiLOgOg1o0l09gL9goL0I1IL
#define sm4_encrypt                 oOl9i01o0l1OlOoL1gLLOIggIiLL1O1o
#define sm4_decrypt                 liIllL9iLili90OO100ggi90og0ooiI1

// wbox
#define aes_wb_decryptor            OIgilo1Lg9ol1o0LlOI9LgiL1il9I9gg

// xxtea
#define xxtea_encrypt_in            lIOI9i1lIlLi1OI1iiLlg1OoI09L9Lgi
#define xxtea_decrypt_in            gI1I0iLO1iLi0lOgLi19Lo0gOI1LgIg0
#define xxtea_ubyte_encrypt         i9gglgLo101l9oOi1gO99I9iOoOiio9I
#define xxtea_ubyte_decrypt         gIO9I1IO19L09oll0Lgio0100LLil909
#define xxtea_to_uint_array         OOiiO9OIgIolLg0oIOoo1I0lllglOo10
#define xxtea_to_ubyte_array        i9LIgO9il1i1Lil90iIOLlgiL9L90oI9
#define xxtea_uint_encrypt          iO1lg9l1g0iI0OgiL0LLo091l0Igo0gO
#define xxtea_uint_decrypt          oLLIl99gIliIgl90olllgioIgIogiL0o


// tools
#define DX_HexStringToChar          OI1ILo90gloilO0Lo1goOlooogglg0Oi
#define DX_CharToHexString          I1lL1I1g0gI1199Io1iiO190LooOLLL0

#endif /* dx_utils_crypto_prefix_h */
