//
//  prefix.h
//  STEEForiOS
//
//  Created by xelz on 2017/7/19.
//  Copyright © 2017年 echonn. All rights reserved.
//

#ifndef dx_utils_crypto_prefix_h
#define dx_utils_crypto_prefix_h

#define print_buf_internal 												go0LL0IiIoiL00ilI01gLilOi9oi1iol
#define fflush_buf 														gO0oOL1o9I0OloOI900lo1oiLg9iIIoi
#define fill_buffer_filefunc 											iIg0gllIOL9O1IOoOgI19IigO1iIlI1o
#define fopen_buf_internal_func 										iiOggIl0llo99olIO0Oi0L9Lol1IoLIi
#define ftell_buf_internal_func 										l1lIO0g91gIo11igiogilOig0LiooLI9
#define fseek_buf_internal_func 										igg10li9Ii9oIllOlLO0Oli9lIgl9Oo0
#define ferror_buf_func 												g0oglL00L11l9o1li19OOoIglO990iIL
#define fopen_buf_func 													LLIlgggo0O90OI1O11Ol9oOl9LIil9g9
#define ftell_buf_func 													ggOo0og10IilILg99OOooi0O1I9li1IO
#define fopendisk_buf_func 												gOg1o1iOolOLl9Oli9gLLOIIOOiLo9il
#define fseek_buf_func 													gLig00LligLLoOiiiLg1IiIi9oL0LgLL
#define fwrite_buf_func 												ooloL9Oi0Li1L0iLiOIgo099ol0lI0lo
#define fclose_buf_func 												ILo9IO01g010io1191oii9giL9IIi99O
#define fread_buf_func 													g19Ol9oI1IgLg1O1LgoogOgg0lI9gOl9
#define fopen64_buf_func 												lILoligloOo9L1o1iI9LIL1L9LgL91O1
#define ftell64_buf_func 												iiI1LIL0LOoL9OIi0LOlO19OLIillLig
#define fopendisk64_buf_func 											LLg90g1Oo910i0LOg0IoiO9I0iggll0l
#define fseek64_buf_func 												Ogo11igoI1L0lO09OO00oiigOLogi999
#define fill_buffer_filefunc64 											go1gg0g0oiilggiioogOl19gL9ll9lll
#define fill_fopen_filefunc 											iggliig0999iiLil99Og00iLo9io00og
#define fill_fopen64_filefunc 											OigIiO91911g0L0Lgg1ii0og0oog0goO
#define call_zopen64 													ggL0lIiliI1I0o99g0L0oIoOooL1IgIl
#define call_ztell64 													glglogI00Io1g9g9gl9g91o1LloO01o0
#define call_zopendisk64 												g0Iio1Lil1i1igl191OiooLi1III9igL
#define call_zseek64 													iliilLgOiLoo9iO99Og0LLoLl000gi1l
#define fill_zlib_filefunc64_32_def_from_filefunc32 					I0IlIl0LII19L1o1gLgoo0g01IiiO1go
#define fill_memory_filefunc 											O1OlIL9glLL9giogg1lOIlo0Lli0iIlo
#define ferror_mem_func 												l0oIgoI9LllOLli19lL90LO0LI0010gl
#define fopen_mem_func 													OLoLoOILoL1lliLI9o0iolIL9OogLlIl
#define ftell_mem_func 													gL9igololloOL9i990goLoLiOOooilOg
#define fopendisk_mem_func 												LL09Llg10IiglLogoIg10Ii10llLlLlg
#define fseek_mem_func 													IgO0OIoI1I1gillgL19g0I9OIg1199lg
#define fwrite_mem_func 												IooLlii0ggi9Og11iOL1OOlglO11I1I9
#define fclose_mem_func 												lLOLiggig9091l9iOiLl0lo99Ioo1gLo
#define fread_mem_func 													L0Iol0oLi1loiooI90i9ILg9LLg0l01g

#define xxtea_ubyte_encrypt												g910O00LIlolLIo111iOl1ilil1L9191
#define xxtea_uint_encrypt												g9iiLIL909IglOoL1li1o9I0oiog1I9g
#define xxtea_ubyte_decrypt												LIO9o9ogo09OoLlgio1o9iIIIOoggo0l
#define xxtea_uint_decrypt												L90Ilolo91iL0oggg0O0O0L9iIL1IiII
#define xxtea_encrypt_in												Ii91oIO01990g9iI99LlO1ggLI00g0go
#define xxtea_decrypt_in												o0ggogOOlgglloi0LIOOgoLo0OiL1Io9
#define SHA256Mini_Init													o9IoiLIl11101gi01l11OoI1Io01010L
#define SHA256Mini_Final												IiOli90I09L19i19gl00L0L91l0100gg
#define SHA256Mini														II9LOg1og90o0Oo01IL9oIi1LgiLIlIO
#define SHA256Mini_Update												g9gLl01oOL999ggiioI1L1L09i9II0Oo
#define mpiadd_mini														LiILolo1g9lLlIioOLg0g1iiI9I0OlgI
#define base64_encode_imp												OoO99iLlliOl1O1OL1i1iiiLOOLlI9O1
#define base64_decode_imp												I9Ooogi19Oo1i0ooLo09i9LgOLL9oog1
#define mpisub_mini														llOi91Ooo9I9ILl10O0OOiL9lO0iIi9L
#define sm3_encrypt														Io91iIoggIilL10I1l90i1i19IOoO1Oi
#define sm3_init														I1oioiilgIOl9oolIooll19oIO9OLIo0
#define sm3_compress													OoogiliOlo0gI0lLLo9iL99iLg0ggl1I
#define sm3_final														ILO00i9iIlg0Li1I9L9oILlL09IIi1Oo
#define sm3_update														ILiooOLLIILol1lgO0ioLlio9919oI1l
#define F25519_copy_mini												oo9iIgoilo9o9IIiioIiO9O0lOOlLII9
#define F25519_set_mini													O1OglOIi1O0oiLg9I9ggiILolg9L10og
#define F25519_get_mini													l9i01O01O0gg9LLilo10i1gO1IIio0Oo
#define F25519_equal_mini												Ollolol9O9L0Lio1i1io1L91gg01LIg0
#define F25519_setK_mini												g0oIg10Io0lgo1LiLi900Il11o0OLI90
#define F25519_cmp_mini_												ii9oiIg0lLOIoOI0L1IiILgLg9iIO0gO
#define F25519_sub_core_mini_											g1Llg1oI9lIgo0IoLILo9lgol9I9Lo0O
#define F25519_reduce_mini_												LiIIooOlgg90OolgLg10g0Oi111oligo
#define F25519_P_mini_													ILooOgO00o9L11l90lIlgolgIgoog1O0
#define F25519_sqr_mini													gO10og1ii1ililIL1O9oIi0L9og9i0gO
#define F25519_mulK_mini												OiL9oL0IOgO0gOI0II0OO91LIL1lgOLi
#define F25519_mul3_mini												ooLg1Igi00ILL1I1I0i0L91lIgLlI09o
#define F25519_add3_mini												oi1O9OlOL90IL91II11liI1oiOIlLOlo
#define Curve25519_mini													ol0l0gI0OOoo19iLl11o0010o9IooIgL
#define mpicmp_mini														iLli990loI11Il1gii9o1lo91OIigglo
#define mpisetval_mini													oII10gg0Olg0L1loLi01OO9OoLIolOO0
#define F25519_sub3_mini												i0gLLL0IL0LL11IgLlog9LiIi1L1OgLI
#define AESMini_Init_Encrypt											L1L09o1i990Oii99OOilI0Ii0LO111gO
#define AESMini_ECB_Encrypt												gig19g1liL09ig9I9IogiLoI0i0oi9gl
#define AES_ECB_encrypt													gO0ll1OLOLlIOiIIOOL0o91Oli1lL0gi
#define AES_ECB_decrypt													lggLIgi0g1gO99iO1OiI1I119iI0Io10
#define AES_CBC_encrypt_buffer											l101oggO0LLOOooiiOo0Ogg0LliI9Ilg
#define AES_CBC_decrypt_buffer											log9l191Ll9OI9i9iL9l0lL0gLiL9III
#define AES_CTR_xcrypt_buffer											LO0O9OOilIi9oLIggioLoo9L9l1Oo0g9
#define AESMini_Init													oOII1OLoIIOilooIIOOIl1goLogLggOi


#define mpimul_mini														ol9ogOIliO90OgL9LiO1Lloo110Og0iI
#define mpi_mulrow_mini_												LLl11LOi9i1lO1I1o1Ll0gOILiIoIIlO
#define aes_wb_decryptor												gIO1I1IL19iOO9llLo0L9oiIOL1IOlOO
#define rc4_crypt														ILiiOIiiOogigoLI1O0io1O0iOiogoLI
#define rc4_init														gol0il1O00go9l1l0Ogg0Igiioig1Ig9
#define set_key														 	l0gIOIl9O0iOio0gOggIOg9oi90iLli1
#define search_s_box													ogoLO10iOi0LLLiigg9olgLL0lgL1gI9
#define sm4_encrypt														iil9oo1g1Oi9lLo1oi19og1LoOLlo9LI
#define sm4_decrypt														ILLI1ol01ig9oo1110O99II919iL1OLg
#define sm4_crypt														oi9goILooLL9goliii0i01gLo1Lg99gl
#define sm4Lt														 	g1iogiiIiog1goLIi0I1I9g1oOL0Oiil
#define cal_ci_rk														LiI0iLgOlO11iLLioilLoL9Igi0900Il
#define sm_4f														 	iiLogl9g0gg1l9olL9o1liLogol09goI
#define one_round														lo0lOO0lo1g0OOloOolO0g1LlOLi9L1O
#define FK														 		LIoigo1Iligl11LooO99l9loIllL0li0
#define CK														 		LLOLglI1O1gllOIIo19IO1oloLiO9Ili
#define S_BOX_TABLE														g1lOOLl90OL1oI19oiiIgl0i9li10OgL
#define json11														 	o0L0ol0lglg1gLL1liIlil1OL19LOlo1
#define JsonParse														oioL1l9O10glg0gOL11gIIglOgIglli9
#define Json														 	o10Io9LO9glgO9o10lIILLoII00lOg9O
#define JsonValue														OgLi990gl0lOL01LLlOOOI9ggIO091lo
#define sm3_hmac														gogIgoI199IL9Io9OLOOOogL0ogIglo0
#define sm3_hmac_init													OigIoILlg0Io1ii0llIlO9LI1loo1iI9
#define sm3_hmac_update													oo10g9LLLlLoi0gi0Lg1L0l100oili0l
#define sm3_hmac_final													Ool19go010ligggO9lIg0OO99o19l0gl
#define hmac_sha256														l9O1g99oL9L0O0ooLl9o90gLoloog9go
#define parity														 	ig10o11gO1OoLoOIl9I990oi9oLggO99
#define sha1_init														O0gO9L9g1l1go9lOg1lLoOoglgLLLI9O
#define sha1_nextBlock													l0gl1Lloiog01l9oLil01Oo9gL0109iO
#define sha1_lastBlock													iL0IOlOIiI9g1oOL9lgiO9Olig9o1911
#define maj														 		loLIII11I0iiog0oll0i1L0l90gLglOI
#define sha1_ctx2hash													i09iI9i1g90LlI9g0I0gg191igliO0OI
#define ch														 		gO1iiiogl90oIoI9O90gglI0og09glI1
#define change_endian32													g00L1o0gIL1i1Lg0glg00LIigiil9oO1
#define rotl32														 	o00l1gL10i999I9o9L90L1ioI00Ii090
#define sha1														 	O19igg0IlIIgIOI1g0iI0li1Og0iLoi1

/** 已补全 **/

#define MD5Init														 	i111OgLgilg1ooLL0gliI9LoglLg19io
#define MD5Final														gILO9OiOLlgI0Li1o90i10OO990L1lIo
#define MD5Update														gIO1iI1LgLIgO1gIgIIolIgIloglg1lO
#define MD5Transform													LI9L0I0I10L1loO1Oo0IIilLL99gLg0O
#define md5														 		Lilg9Og019iI91iII9OgIOI0Ool9LLIi
#define hmac_sha1														o0I19L0O1090lgILigoII9liil990glL
#define hmac_md5														ig990IgILiO1OOOoOog0goI9gLIiog0i
#define Encode															oL0loloo9l9g91i0o91990oioi0O110g

#define zipCloseFileInZipRaw											iOlOoOIo00OIloglo09Il000O1glLglO
#define zip_copyright													il0olO1L00g1lg9lIiigl109ilOO0Ogg
#define zipOpenNewFileInZip												gLLOgLgLlIgIiI9O9o9gilli1oO9Li1O
#define zipWriteInFileInZip												I999gO00l9OoLIgLi90g0Lg0goIlLigi
#define zipCloseFileInZip												OolgIoiiO10I9Oo1liOglL0009Oll90g
#define zipOpen														 	o09ilgoOoli1Oo0o10gIgi1Il911Lg1O
#define zipClose														LoOIgIgl0IgLiogi0I9i01ogilioiOOi
#define zipOpenNewFileInZip4											IL0lO9I0OIg1l19i09lo19oL9i1iLOli
#define zipOpen4														lgg01oOL91LLO19Il99OoO9Oiligi0I9
#define zipCloseFileInZipRaw64											IIO09Oo9o011o1I1oo0lLLigii10lo0O
#define zipOpenNewFileInZip64											illgOi9olLLg999IIo190L9o19oi01oi
#define zipOpen64														IgiIIiILi01g9ILL0Io190190ILl0g19
#define zipOpenNewFileInZip4_64											Li19iL99L9Lo1LI9119li0Ig1liliI19
#define zipOpenNewFileInZip3_64											OlOO9IL9go00Ol9gLogILl1OLOi1ol1o
#define zipOpen3_64														L1oLoL1IiiI9Ig0illI9O0iI9Lio1L9L
#define zipOpenNewFileInZip2_64											Ig0i0901g9Lg9Olg9l10Ioi0io0o0LII
#define zipOpen2_64														ogOoL9o1goi0OIgLLLo1iO9iiOLI909i
#define zipOpenNewFileInZip3											iLoOO01IL1oII911iliL0OIg1L0g0Io1
#define zipOpen3														Oolo19o0I1OLOi0O0golOIOLg9L0I9g0
#define zipOpenNewFileInZip2											Ol09I9l10l0LIO1oILIi1L1o09ooiL9L
#define zipOpen2														IL1oig9gil9l0ILOg1LO010g91l1LO01

#define init_keys														L901g09oiLiOlOL0I0I10oggLlLiloll
#define add_data_in_datablock											l0g1gL0g01OOlL19ili0ooOl99lOo90g
#define update_keys														I1IOig01Lll0il0LOo1iglLIlIOoIIo1
#define zip64FlushWriteBuffer											I1iL0gL1OOL10ILgi1giIlLL0Lg9L0lO
#define zip64local_getLong												iOo99gOlI9iigioioOg1LLloLiloOI1g
#define zip64local_getLong64											ogOg0lOi11Ogg9ggg9iOl1iO9igoIOi9
#define zip64local_getShort												g0Lg0Oi9giLiiiOo9OILi1iogo1OLiIo
#define zip64local_putValue												Ogoig9oIliLIli0o0oIg999OIi9olLgO
#define zipGoToNextDisk													lIi1golOgl1O1gOioOO1LiIiIIgO9LI0
#define zipGoToSpecificDisk												lgIOiIiILOl91I99LOgoLooO099o9go0

#define unzGetGlobalComment 											oOlL9LLlilOlol1oI9LgiL91IiII1iog
#define unz_copyright 													o9giO9OO1Og1090I0OiOo0l9ggO1O00i
#define unzSetOffset 													iIOig01L0gI10oigOOl1IiilLgiiLoll
#define unzGetOffset 													goLI0gOilioogLOiOiIIL1OOl11g991I
#define unzGetFilePos 													iiog1ooLOO0gi1i0LllLgi9i1oiOIg0l
#define unzGoToFilePos 													lggIOl1i0gioOl00Ig90glol1oi00i90
#define unzGetGlobalInfo 												i9og1LLgOlLogllLolLiO0OOiLo11OO0
#define unzGetCurrentFileInfo 											lL0LIlOIogOOggILg99L1ii0OiIo0L0o
#define unzOpen 														OLIIiLiI01i09oiio0090L9919iIl9iL
#define unztell 														L9II1g0IO11oIgo9g9LggILOl9l1l1O9
#define unzeof 															igOg0O9lI1191ILi1gLLl091OO9i0O9o
#define unzClose 														O1oi9goOi0LgL1i0gglLig9i10oo0li9
#define unzGoToNextFile 												oLIIIoOi10Og0LgilIggiOLLoliIi9oO
#define unzGoToFirstFile 												o9gll1OLLiI9IlLOiI000glIlLLOii1g
#define unzOpenCurrentFile 												Lol9g9I9I19llooooiOgLOiglLlII1ig
#define unzCloseCurrentFile 											LOLL1IIOO0o001oi00LO99Il911oOl00
#define unzReadCurrentFile 												igiiL9io9gi1OIiigolil9oI9L099O99
#define unzLocateFile 													O0IiIOIgiOIoLoLiL190oLgI1gL0i1io
#define unzOpenCurrentFilePassword 										oIL011L1lLIoOg90Oo0lILo9iIOIgoi1
#define unzGetLocalExtrafield 											ogLlIgII0I90IL1L1O0gI0oo9I99gLL9
#define unzSetOffset64 													iOi1g1i10011l0ilO1l9Ooi0iiO91LOl
#define unzGetOffset64 													Olol9oli101Lg1I1ollgl1LoLLgiII10
#define unzGetCurrentFileZStreamPos64 									lII9iOLi9LlO9oOL1lgo0l9III00oioL
#define unzGetFilePos64 												OLOIiilI1olI0Io119ilioL09lLg1Lo0
#define unzGoToFilePos64 												LLL1iL1iOl9LL0oLiL0lg9Ig9IIlLlIo
#define unzGetGlobalInfo64 												l99goI1g00Ii9Oo1ol0IL109I1oI0ilL
#define unzGetCurrentFileInfo64 										g1Il1OL0ioOgl90oioI0ii0lg0Llo1O0
#define unzOpen64 														loIOio0l9IO1i00olOogiOO9LogIOLgg
#define unztell64 														LIII0OLIIoOIi1ig9lgi0OlilILIlL01
#define unzOpen2_64 													oglIoOgloO1IIoLi9Og1o1OlIl00Ogi9
#define unzOpenCurrentFile3 											ogglgLLloL9gIiIl1Ioo9LoOO9lOoooO
#define unzOpen2 														gOg10IogO0lLgIOL9gIl0LloO0oILg11
#define unzGoToNextFile2 												iLlL10O9iL901l9Ilooo0oII90gl9Olo
#define unzGoToFirstFile2 												g0OII0lLL1Ol0lllL9Oig0LIiL1io01O
#define unzOpenCurrentFile2 											lO09i0I0l9101Ol1I091oiI0i9LlLoli
#define unzOpenInternal 												gO1ilLoo0glll1g9l01Oli0O19I91LLi
#define unzGoToNextDisk		 											oLlg1O9III9LIiig0Igo1oiloL0Igi1i
#define unz64local_getLong		 										l9Ol9l91Lo0ol1LgOoiL01O1o9Looo99
#define unz64local_getShort		 										illloIiL0iloooil0L9l11LIO00iIIOo
#define unz64local_getLong64		 									OgOLii1oI1l90olg9gO9OOl9ILgoLlgi
#define unz64local_GetCurrentFileInfoInternal                           iiLg1IIoLgol1iLOL0IOiOgLoLgoI1I1

#define fopen_file_func 												oiL0iOl0LlolO091000g9Il1Lo01OO1o
#define fopendisk_file_func 											i0og10gO0O9LOoi0OL99iLl1oI0OOllI
#define fread_file_func 												OoL9ig1oi10O0gl1Og9L1OIlil01lIoL
#define fwrite_file_func 												OI90lo1Li0O9iOl1Iol091l9190loOLg
#define ftell_file_func 												oig0OlLiglO0i00i1O1Ooo9IiIi9iiIO
#define fseek_file_func 												L0liil1Ig9oLl09iIl9IiOLLgo09i9Il
#define fclose_file_func 												ogloilgL00IoLgO9iO9I0giIll90o9l0
#define ferror_file_func 												i0LIlgIg01l1Og9o091l1o9LoLIo0lL9
#define fopen64_file_func 												LLlOoL0logII0g1oogoO1oLgi9lIgg11
#define fopendisk64_file_func 											goig110L001igg0OiiilliLo01go9191
#define ftell64_file_func												i9L9o1i1I1ioi1I9lg1iOiLIg09IiO90
#define fseek64_file_func 												g10gi009ig91gllOlO0Ii90iO0100i1o
#define file_build_ioposix 												ilIgLoiOil1iLo0LOL11I99LigIO190I



// pkcs7
#define pkcs7 															Oog1o1I1L119LLLgg0LO9gOlIOIIgo10
#define open_file 														i0LoLO1o91goLlLioL99I01OOI00oL9I
#define get_content 													Loi901g9OlOl9OLo0ig0O0LI1g1I1Ll9
#define parse_pkcs7 													oLgIloL10g1iI9LiL09lioIloOiI01O1
#define get_from_apk 													oo1g10g9ioL10OOoi9LOIo10oio0oigl
#define len_num 														I1100OL90L1Olg1L9lo9O1o0o99Li9O9
#define num_from_len 													LLOooi19llOgLOIii9lgiIlO19l1l09l
#define tag_offset 														oOgOII19lgI9LLll0oLii11LLIL00L9l
#define get_length 														iiooLig0l9o1go1oLL0Oog10Ol0il1OL
#define create_element 													L1909go9Ool9o0IligLLoO1O9oIoOo0i
#define parse_certificate 												IgIogOl909l11II9II19gLlIL90lOool
#define parse_signerInfo 												lllLLlio109iOogLolO0g9oLi01g09Oo
#define parse_content 													iliILOIIgL10o1oOgOoil001ILggoog1
#define get_count 														LIOlO1L9gLL900g99900L1i90OIlgII1
#define get_MD5_hex 													iOll19IL1l0i1LoLI0iLLLii0I0OoI0L


// google

#define google 														LLli11iIl1g09ilgg9IoiiigLLOiill1


// api
#define CryptUtils 												    o99O10O1O0IILOiOo0I1IiiLool9l90L
#define JSonUtils 												    iiii1gogi19O1I0II91LlOoLLii0ogll
#define ZipFile 												    g11illoIiL1ooil1Oglo9i91io9iOg0o
#define XXTeaEncrypt 												LoOLOggOL9liOog1LLg1ilIll09iI9gi
#define XXTeaDecrypt 												ioo0oOi9i00Og90l01l9O9IlggL0Ol19
#define Base64Encode 												ooo1igli0g9O1gILOiLOL11io1Io1ggi
#define Base64Decode 												ioOlO00L1lI0iLiLlig11iOLg1ILILoo
#define MD5 												        O0go91loi9g0o0LL99goLL1LgO9000Io
#define MD5WithKey 												    o9I9iiol9li0g11lg9igOL9g9O90O0Io
#define SHA1 												        oL9lLiloiLi0lIIi9o900l9gg1Lo9oOO
#define SHA1WithKey 												lO0oII11oig0L0o01oIILgg9O9OlLlgI
#define SHA256 												        ggiio0IOg9190L0Ili1II9oIIIioi1oO
#define SHA256WithKey 												ogI9ii0LloOllO11glO0llgO9oo1OooO
#define HexStringToChar 											OLiloi0O1loi9gIigOloLi9l109Li99g
#define AesCbcDecrypt 												LILO91go9Igii9oOIlig0oLIgo9IglI1
#define CharToHexString 											lg1Oolg9I10L1LO1L9l9i9OogiOLOOlg
#define JSon2string 												gli9O91IoOgl0I1gOoLOiogg9OL01OOL
#define string2JSon 												gggIo0lgOo1Lo10i101lI1ILiO0L900O
#define Compress 												    o1il1Oi1lIlg1g9iOig9LO00ilOLl909
#define UnCompress 												    O0iOg110llL10oOI01loilO9loilL99i
#define CreateUZipperFromFile 										L0g99ioOLiI9llI0l0iigl010illIL01
#define CreateUZipperFromData 										i0O0LglolIgo1loOiloLOg1l1O1i90ig
#define CreateZipperFromData 										OiOoilOOI19iL019I9Oo1i991i1l0o99
#define CheckIsExist 												ioLlO9ioOLgOgLO11lo9gOlLlL9g90g9
#define GetFileData 												l19goI909ol09oOOloOlOogo9g9o1IO9
#define AddFileData 												gLoiIilggoo9OLig0IOooI1O1O1OigiL
#define GetZipFileData 												OgL9gO1glllLoioIgLg9Ol0giiLL0gO0
#define Close 												        Oo100O9iOioO1lIOgillgoioli0OLgO1
#define TraversalZipFile 											Ooo0igL1I09ooi0O0IoLo109o01igO1I
#define isValidKeyByAES 											o1LLO9g09L0Oglio1Og1IOI91i911L0L
#define isValidKeyByWBAES 											IO900199ioOlL909LlOL19g0O9o909i1
#define isValidKeyBySM4 											OO9gogLlo91LoILO0o90g9ilioO9i01O
#define MTKeyEncrypt 												o11O0g10lgiLioiIlg09IoIl09iOg099
#define MTKeyDecrypt 												g0Ll19llIO0iIL19io99gOiO9Og11ii9









#endif /* dx_utils_crypto_prefix_h */
