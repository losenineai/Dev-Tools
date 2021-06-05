#include "wbox.h"

/* round 1 */

/* round 1 / subround 0 */
#define r000(i) thiz->a[0*256+i]
#define r001(i) thiz->a[1*256+i]
#define r002(i) thiz->a[2*256+i]
#define r003(i) thiz->a[3*256+i]

/* round 1 / subround 1 */
#define r010(i) thiz->a[4*256+i]
#define r011(i) thiz->a[5*256+i]
#define r012(i) thiz->a[6*256+i]
#define r013(i) thiz->a[7*256+i]

/* round 1 / subround 2 */
#define r020(i) thiz->a[8*256+i]
#define r021(i) thiz->a[9*256+i]
#define r022(i) thiz->a[10*256+i]
#define r023(i) thiz->a[11*256+i]

/* round 1 / subround 3 */
#define r030(i) thiz->a[12*256+i]
#define r031(i) thiz->a[13*256+i]
#define r032(i) thiz->a[14*256+i]
#define r033(i) thiz->a[15*256+i]

/* round 1 / subround 4 */
#define r040(i) thiz->a[16*256+i]
#define r041(i) thiz->a[17*256+i]
#define r042(i) thiz->a[18*256+i]
#define r043(i) thiz->a[19*256+i]

/* round 1 / subround 5 */
#define r050(i) thiz->a[20*256+i]
#define r051(i) thiz->a[21*256+i]
#define r052(i) thiz->a[22*256+i]
#define r053(i) thiz->a[23*256+i]

/* round 1 / subround 6 */
#define r060(i) thiz->a[24*256+i]
#define r061(i) thiz->a[25*256+i]
#define r062(i) thiz->a[26*256+i]
#define r063(i) thiz->a[27*256+i]

/* round 1 / subround 7 */
#define r070(i) thiz->a[28*256+i]
#define r071(i) thiz->a[29*256+i]
#define r072(i) thiz->a[30*256+i]
#define r073(i) thiz->a[31*256+i]

/* round 1 / subround 8 */
#define r080(i) thiz->a[32*256+i]
#define r081(i) thiz->a[33*256+i]
#define r082(i) thiz->a[34*256+i]
#define r083(i) thiz->a[35*256+i]

/* round 1 / subround 9 */
#define r090(i) thiz->a[36*256+i]
#define r091(i) thiz->a[37*256+i]
#define r092(i) thiz->a[38*256+i]
#define r093(i) thiz->a[39*256+i]

/* round 1 / subround 10 */
#define r0a0(i) thiz->a[40*256+i]
#define r0a1(i) thiz->a[41*256+i]
#define r0a2(i) thiz->a[42*256+i]
#define r0a3(i) thiz->a[43*256+i]

/* round 1 / subround 11 */
#define r0b0(i) thiz->a[44*256+i]
#define r0b1(i) thiz->a[45*256+i]
#define r0b2(i) thiz->a[46*256+i]
#define r0b3(i) thiz->a[47*256+i]

/* round 1 / subround 12 */
#define r0c0(i) thiz->a[48*256+i]
#define r0c1(i) thiz->a[49*256+i]
#define r0c2(i) thiz->a[50*256+i]
#define r0c3(i) thiz->a[51*256+i]

/* round 1 / subround 13 */
#define r0d0(i) thiz->a[52*256+i]
#define r0d1(i) thiz->a[53*256+i]
#define r0d2(i) thiz->a[54*256+i]
#define r0d3(i) thiz->a[55*256+i]

/* round 1 / subround 14 */
#define r0e0(i) thiz->a[56*256+i]
#define r0e1(i) thiz->a[57*256+i]
#define r0e2(i) thiz->a[58*256+i]
#define r0e3(i) thiz->a[59*256+i]

/* round 1 / subround 15 */
#define r0f0(i) thiz->a[60*256+i]
#define r0f1(i) thiz->a[61*256+i]
#define r0f2(i) thiz->a[62*256+i]
#define r0f3(i) thiz->a[63*256+i]

/* round 2 */

/* round 2 / subround 0 */
#define r200(i) thiz->a[64*256+i]
#define r201(i) thiz->a[65*256+i]
#define r202(i) thiz->a[66*256+i]
#define r203(i) thiz->a[67*256+i]

/* round 2 / subround 1 */
#define r210(i) thiz->a[68*256+i]
#define r211(i) thiz->a[69*256+i]
#define r212(i) thiz->a[70*256+i]
#define r213(i) thiz->a[71*256+i]

/* round 2 / subround 2 */
#define r220(i) thiz->a[72*256+i]
#define r221(i) thiz->a[73*256+i]
#define r222(i) thiz->a[74*256+i]
#define r223(i) thiz->a[75*256+i]

/* round 2 / subround 3 */
#define r230(i) thiz->a[76*256+i]
#define r231(i) thiz->a[77*256+i]
#define r232(i) thiz->a[78*256+i]
#define r233(i) thiz->a[79*256+i]

/* round 2 / subround 4 */
#define r240(i) thiz->a[80*256+i]
#define r241(i) thiz->a[81*256+i]
#define r242(i) thiz->a[82*256+i]
#define r243(i) thiz->a[83*256+i]

/* round 2 / subround 5 */
#define r250(i) thiz->a[84*256+i]
#define r251(i) thiz->a[85*256+i]
#define r252(i) thiz->a[86*256+i]
#define r253(i) thiz->a[87*256+i]

/* round 2 / subround 6 */
#define r260(i) thiz->a[88*256+i]
#define r261(i) thiz->a[89*256+i]
#define r262(i) thiz->a[90*256+i]
#define r263(i) thiz->a[91*256+i]

/* round 2 / subround 7 */
#define r270(i) thiz->a[92*256+i]
#define r271(i) thiz->a[93*256+i]
#define r272(i) thiz->a[94*256+i]
#define r273(i) thiz->a[95*256+i]

/* round 2 / subround 8 */
#define r280(i) thiz->a[96*256+i]
#define r281(i) thiz->a[97*256+i]
#define r282(i) thiz->a[98*256+i]
#define r283(i) thiz->a[99*256+i]

/* round 2 / subround 9 */
#define r290(i) thiz->a[100*256+i]
#define r291(i) thiz->a[101*256+i]
#define r292(i) thiz->a[102*256+i]
#define r293(i) thiz->a[103*256+i]

/* round 2 / subround 10 */
#define r2a0(i) thiz->a[104*256+i]
#define r2a1(i) thiz->a[105*256+i]
#define r2a2(i) thiz->a[106*256+i]
#define r2a3(i) thiz->a[107*256+i]

/* round 2 / subround 11 */
#define r2b0(i) thiz->a[108*256+i]
#define r2b1(i) thiz->a[109*256+i]
#define r2b2(i) thiz->a[110*256+i]
#define r2b3(i) thiz->a[111*256+i]

/* round 2 / subround 12 */
#define r2c0(i) thiz->a[112*256+i]
#define r2c1(i) thiz->a[113*256+i]
#define r2c2(i) thiz->a[114*256+i]
#define r2c3(i) thiz->a[115*256+i]

/* round 2 / subround 13 */
#define r2d0(i) thiz->a[116*256+i]
#define r2d1(i) thiz->a[117*256+i]
#define r2d2(i) thiz->a[118*256+i]
#define r2d3(i) thiz->a[119*256+i]

/* round 2 / subround 14 */
#define r2e0(i) thiz->a[120*256+i]
#define r2e1(i) thiz->a[121*256+i]
#define r2e2(i) thiz->a[122*256+i]
#define r2e3(i) thiz->a[123*256+i]

/* round 2 / subround 15 */
#define r2f0(i) thiz->a[124*256+i]
#define r2f1(i) thiz->a[125*256+i]
#define r2f2(i) thiz->a[126*256+i]
#define r2f3(i) thiz->a[127*256+i]

/* round 3 */

/* round 3 / subround 0 */
#define r300(i) thiz->a[128*256+i]
#define r301(i) thiz->a[129*256+i]
#define r302(i) thiz->a[130*256+i]
#define r303(i) thiz->a[131*256+i]

/* round 3 / subround 1 */
#define r310(i) thiz->a[132*256+i]
#define r311(i) thiz->a[133*256+i]
#define r312(i) thiz->a[134*256+i]
#define r313(i) thiz->a[135*256+i]

/* round 3 / subround 2 */
#define r320(i) thiz->a[136*256+i]
#define r321(i) thiz->a[137*256+i]
#define r322(i) thiz->a[138*256+i]
#define r323(i) thiz->a[139*256+i]

/* round 3 / subround 3 */
#define r330(i) thiz->a[140*256+i]
#define r331(i) thiz->a[141*256+i]
#define r332(i) thiz->a[142*256+i]
#define r333(i) thiz->a[143*256+i]

/* round 3 / subround 4 */
#define r340(i) thiz->a[144*256+i]
#define r341(i) thiz->a[145*256+i]
#define r342(i) thiz->a[146*256+i]
#define r343(i) thiz->a[147*256+i]

/* round 3 / subround 5 */
#define r350(i) thiz->a[148*256+i]
#define r351(i) thiz->a[149*256+i]
#define r352(i) thiz->a[150*256+i]
#define r353(i) thiz->a[151*256+i]

/* round 3 / subround 6 */
#define r360(i) thiz->a[152*256+i]
#define r361(i) thiz->a[153*256+i]
#define r362(i) thiz->a[154*256+i]
#define r363(i) thiz->a[155*256+i]

/* round 3 / subround 7 */
#define r370(i) thiz->a[156*256+i]
#define r371(i) thiz->a[157*256+i]
#define r372(i) thiz->a[158*256+i]
#define r373(i) thiz->a[159*256+i]

/* round 3 / subround 8 */
#define r380(i) thiz->a[160*256+i]
#define r381(i) thiz->a[161*256+i]
#define r382(i) thiz->a[162*256+i]
#define r383(i) thiz->a[163*256+i]

/* round 3 / subround 9 */
#define r390(i) thiz->a[164*256+i]
#define r391(i) thiz->a[165*256+i]
#define r392(i) thiz->a[166*256+i]
#define r393(i) thiz->a[167*256+i]

/* round 3 / subround 10 */
#define r3a0(i) thiz->a[168*256+i]
#define r3a1(i) thiz->a[169*256+i]
#define r3a2(i) thiz->a[170*256+i]
#define r3a3(i) thiz->a[171*256+i]

/* round 3 / subround 11 */
#define r3b0(i) thiz->a[172*256+i]
#define r3b1(i) thiz->a[173*256+i]
#define r3b2(i) thiz->a[174*256+i]
#define r3b3(i) thiz->a[175*256+i]

/* round 3 / subround 12 */
#define r3c0(i) thiz->a[176*256+i]
#define r3c1(i) thiz->a[177*256+i]
#define r3c2(i) thiz->a[178*256+i]
#define r3c3(i) thiz->a[179*256+i]

/* round 3 / subround 13 */
#define r3d0(i) thiz->a[180*256+i]
#define r3d1(i) thiz->a[181*256+i]
#define r3d2(i) thiz->a[182*256+i]
#define r3d3(i) thiz->a[183*256+i]

/* round 3 / subround 14 */
#define r3e0(i) thiz->a[184*256+i]
#define r3e1(i) thiz->a[185*256+i]
#define r3e2(i) thiz->a[186*256+i]
#define r3e3(i) thiz->a[187*256+i]

/* round 3 / subround 15 */
#define r3f0(i) thiz->a[188*256+i]
#define r3f1(i) thiz->a[189*256+i]
#define r3f2(i) thiz->a[190*256+i]
#define r3f3(i) thiz->a[191*256+i]

/* round 4 */

/* round 4 / subround 0 */
#define r400(i) thiz->a[192*256+i]
#define r401(i) thiz->a[193*256+i]
#define r402(i) thiz->a[194*256+i]
#define r403(i) thiz->a[195*256+i]

/* round 4 / subround 1 */
#define r410(i) thiz->a[196*256+i]
#define r411(i) thiz->a[197*256+i]
#define r412(i) thiz->a[198*256+i]
#define r413(i) thiz->a[199*256+i]

/* round 4 / subround 2 */
#define r420(i) thiz->a[200*256+i]
#define r421(i) thiz->a[201*256+i]
#define r422(i) thiz->a[202*256+i]
#define r423(i) thiz->a[203*256+i]

/* round 4 / subround 3 */
#define r430(i) thiz->a[204*256+i]
#define r431(i) thiz->a[205*256+i]
#define r432(i) thiz->a[206*256+i]
#define r433(i) thiz->a[207*256+i]

/* round 4 / subround 4 */
#define r440(i) thiz->a[208*256+i]
#define r441(i) thiz->a[209*256+i]
#define r442(i) thiz->a[210*256+i]
#define r443(i) thiz->a[211*256+i]

/* round 4 / subround 5 */
#define r450(i) thiz->a[212*256+i]
#define r451(i) thiz->a[213*256+i]
#define r452(i) thiz->a[214*256+i]
#define r453(i) thiz->a[215*256+i]

/* round 4 / subround 6 */
#define r460(i) thiz->a[216*256+i]
#define r461(i) thiz->a[217*256+i]
#define r462(i) thiz->a[218*256+i]
#define r463(i) thiz->a[219*256+i]

/* round 4 / subround 7 */
#define r470(i) thiz->a[220*256+i]
#define r471(i) thiz->a[221*256+i]
#define r472(i) thiz->a[222*256+i]
#define r473(i) thiz->a[223*256+i]

/* round 4 / subround 8 */
#define r480(i) thiz->a[224*256+i]
#define r481(i) thiz->a[225*256+i]
#define r482(i) thiz->a[226*256+i]
#define r483(i) thiz->a[227*256+i]

/* round 4 / subround 9 */
#define r490(i) thiz->a[228*256+i]
#define r491(i) thiz->a[229*256+i]
#define r492(i) thiz->a[230*256+i]
#define r493(i) thiz->a[231*256+i]

/* round 4 / subround 10 */
#define r4a0(i) thiz->a[232*256+i]
#define r4a1(i) thiz->a[233*256+i]
#define r4a2(i) thiz->a[234*256+i]
#define r4a3(i) thiz->a[235*256+i]

/* round 4 / subround 11 */
#define r4b0(i) thiz->a[236*256+i]
#define r4b1(i) thiz->a[237*256+i]
#define r4b2(i) thiz->a[238*256+i]
#define r4b3(i) thiz->a[239*256+i]

/* round 4 / subround 12 */
#define r4c0(i) thiz->a[240*256+i]
#define r4c1(i) thiz->a[241*256+i]
#define r4c2(i) thiz->a[242*256+i]
#define r4c3(i) thiz->a[243*256+i]

/* round 4 / subround 13 */
#define r4d0(i) thiz->a[244*256+i]
#define r4d1(i) thiz->a[245*256+i]
#define r4d2(i) thiz->a[246*256+i]
#define r4d3(i) thiz->a[247*256+i]

/* round 4 / subround 14 */
#define r4e0(i) thiz->a[248*256+i]
#define r4e1(i) thiz->a[249*256+i]
#define r4e2(i) thiz->a[250*256+i]
#define r4e3(i) thiz->a[251*256+i]

/* round 4 / subround 15 */
#define r4f0(i) thiz->a[252*256+i]
#define r4f1(i) thiz->a[253*256+i]
#define r4f2(i) thiz->a[254*256+i]
#define r4f3(i) thiz->a[255*256+i]

/* round 5 */

/* round 5 / subround 0 */
#define r500(i) thiz->a[256*256+i]
#define r501(i) thiz->a[257*256+i]
#define r502(i) thiz->a[258*256+i]
#define r503(i) thiz->a[259*256+i]

/* round 5 / subround 1 */
#define r510(i) thiz->a[260*256+i]
#define r511(i) thiz->a[261*256+i]
#define r512(i) thiz->a[262*256+i]
#define r513(i) thiz->a[263*256+i]

/* round 5 / subround 2 */
#define r520(i) thiz->a[264*256+i]
#define r521(i) thiz->a[265*256+i]
#define r522(i) thiz->a[266*256+i]
#define r523(i) thiz->a[267*256+i]

/* round 5 / subround 3 */
#define r530(i) thiz->a[268*256+i]
#define r531(i) thiz->a[269*256+i]
#define r532(i) thiz->a[270*256+i]
#define r533(i) thiz->a[271*256+i]

/* round 5 / subround 4 */
#define r540(i) thiz->a[272*256+i]
#define r541(i) thiz->a[273*256+i]
#define r542(i) thiz->a[274*256+i]
#define r543(i) thiz->a[275*256+i]

/* round 5 / subround 5 */
#define r550(i) thiz->a[276*256+i]
#define r551(i) thiz->a[277*256+i]
#define r552(i) thiz->a[278*256+i]
#define r553(i) thiz->a[279*256+i]

/* round 5 / subround 6 */
#define r560(i) thiz->a[280*256+i]
#define r561(i) thiz->a[281*256+i]
#define r562(i) thiz->a[282*256+i]
#define r563(i) thiz->a[283*256+i]

/* round 5 / subround 7 */
#define r570(i) thiz->a[284*256+i]
#define r571(i) thiz->a[285*256+i]
#define r572(i) thiz->a[286*256+i]
#define r573(i) thiz->a[287*256+i]

/* round 5 / subround 8 */
#define r580(i) thiz->a[288*256+i]
#define r581(i) thiz->a[289*256+i]
#define r582(i) thiz->a[290*256+i]
#define r583(i) thiz->a[291*256+i]

/* round 5 / subround 9 */
#define r590(i) thiz->a[292*256+i]
#define r591(i) thiz->a[293*256+i]
#define r592(i) thiz->a[294*256+i]
#define r593(i) thiz->a[295*256+i]

/* round 5 / subround 10 */
#define r5a0(i) thiz->a[296*256+i]
#define r5a1(i) thiz->a[297*256+i]
#define r5a2(i) thiz->a[298*256+i]
#define r5a3(i) thiz->a[299*256+i]

/* round 5 / subround 11 */
#define r5b0(i) thiz->a[300*256+i]
#define r5b1(i) thiz->a[301*256+i]
#define r5b2(i) thiz->a[302*256+i]
#define r5b3(i) thiz->a[303*256+i]

/* round 5 / subround 12 */
#define r5c0(i) thiz->a[304*256+i]
#define r5c1(i) thiz->a[305*256+i]
#define r5c2(i) thiz->a[306*256+i]
#define r5c3(i) thiz->a[307*256+i]

/* round 5 / subround 13 */
#define r5d0(i) thiz->a[308*256+i]
#define r5d1(i) thiz->a[309*256+i]
#define r5d2(i) thiz->a[310*256+i]
#define r5d3(i) thiz->a[311*256+i]

/* round 5 / subround 14 */
#define r5e0(i) thiz->a[312*256+i]
#define r5e1(i) thiz->a[313*256+i]
#define r5e2(i) thiz->a[314*256+i]
#define r5e3(i) thiz->a[315*256+i]

/* round 5 / subround 15 */
#define r5f0(i) thiz->a[316*256+i]
#define r5f1(i) thiz->a[317*256+i]
#define r5f2(i) thiz->a[318*256+i]
#define r5f3(i) thiz->a[319*256+i]

/* round 6 */

/* round 6 / subround 0 */
#define r600(i) thiz->a[320*256+i]
#define r601(i) thiz->a[321*256+i]
#define r602(i) thiz->a[322*256+i]
#define r603(i) thiz->a[323*256+i]

/* round 6 / subround 1 */
#define r610(i) thiz->a[324*256+i]
#define r611(i) thiz->a[325*256+i]
#define r612(i) thiz->a[326*256+i]
#define r613(i) thiz->a[327*256+i]

/* round 6 / subround 2 */
#define r620(i) thiz->a[328*256+i]
#define r621(i) thiz->a[329*256+i]
#define r622(i) thiz->a[330*256+i]
#define r623(i) thiz->a[331*256+i]

/* round 6 / subround 3 */
#define r630(i) thiz->a[332*256+i]
#define r631(i) thiz->a[333*256+i]
#define r632(i) thiz->a[334*256+i]
#define r633(i) thiz->a[335*256+i]

/* round 6 / subround 4 */
#define r640(i) thiz->a[336*256+i]
#define r641(i) thiz->a[337*256+i]
#define r642(i) thiz->a[338*256+i]
#define r643(i) thiz->a[339*256+i]

/* round 6 / subround 5 */
#define r650(i) thiz->a[340*256+i]
#define r651(i) thiz->a[341*256+i]
#define r652(i) thiz->a[342*256+i]
#define r653(i) thiz->a[343*256+i]

/* round 6 / subround 6 */
#define r660(i) thiz->a[344*256+i]
#define r661(i) thiz->a[345*256+i]
#define r662(i) thiz->a[346*256+i]
#define r663(i) thiz->a[347*256+i]

/* round 6 / subround 7 */
#define r670(i) thiz->a[348*256+i]
#define r671(i) thiz->a[349*256+i]
#define r672(i) thiz->a[350*256+i]
#define r673(i) thiz->a[351*256+i]

/* round 6 / subround 8 */
#define r680(i) thiz->a[352*256+i]
#define r681(i) thiz->a[353*256+i]
#define r682(i) thiz->a[354*256+i]
#define r683(i) thiz->a[355*256+i]

/* round 6 / subround 9 */
#define r690(i) thiz->a[356*256+i]
#define r691(i) thiz->a[357*256+i]
#define r692(i) thiz->a[358*256+i]
#define r693(i) thiz->a[359*256+i]

/* round 6 / subround 10 */
#define r6a0(i) thiz->a[360*256+i]
#define r6a1(i) thiz->a[361*256+i]
#define r6a2(i) thiz->a[362*256+i]
#define r6a3(i) thiz->a[363*256+i]

/* round 6 / subround 11 */
#define r6b0(i) thiz->a[364*256+i]
#define r6b1(i) thiz->a[365*256+i]
#define r6b2(i) thiz->a[366*256+i]
#define r6b3(i) thiz->a[367*256+i]

/* round 6 / subround 12 */
#define r6c0(i) thiz->a[368*256+i]
#define r6c1(i) thiz->a[369*256+i]
#define r6c2(i) thiz->a[370*256+i]
#define r6c3(i) thiz->a[371*256+i]

/* round 6 / subround 13 */
#define r6d0(i) thiz->a[372*256+i]
#define r6d1(i) thiz->a[373*256+i]
#define r6d2(i) thiz->a[374*256+i]
#define r6d3(i) thiz->a[375*256+i]

/* round 6 / subround 14 */
#define r6e0(i) thiz->a[376*256+i]
#define r6e1(i) thiz->a[377*256+i]
#define r6e2(i) thiz->a[378*256+i]
#define r6e3(i) thiz->a[379*256+i]

/* round 6 / subround 15 */
#define r6f0(i) thiz->a[380*256+i]
#define r6f1(i) thiz->a[381*256+i]
#define r6f2(i) thiz->a[382*256+i]
#define r6f3(i) thiz->a[383*256+i]

/* round 7 */

/* round 7 / subround 0 */
#define r700(i) thiz->a[384*256+i]
#define r701(i) thiz->a[385*256+i]
#define r702(i) thiz->a[386*256+i]
#define r703(i) thiz->a[387*256+i]

/* round 7 / subround 1 */
#define r710(i) thiz->a[388*256+i]
#define r711(i) thiz->a[389*256+i]
#define r712(i) thiz->a[390*256+i]
#define r713(i) thiz->a[391*256+i]

/* round 7 / subround 2 */
#define r720(i) thiz->a[392*256+i]
#define r721(i) thiz->a[393*256+i]
#define r722(i) thiz->a[394*256+i]
#define r723(i) thiz->a[395*256+i]

/* round 7 / subround 3 */
#define r730(i) thiz->a[396*256+i]
#define r731(i) thiz->a[397*256+i]
#define r732(i) thiz->a[398*256+i]
#define r733(i) thiz->a[399*256+i]

/* round 7 / subround 4 */
#define r740(i) thiz->a[400*256+i]
#define r741(i) thiz->a[401*256+i]
#define r742(i) thiz->a[402*256+i]
#define r743(i) thiz->a[403*256+i]

/* round 7 / subround 5 */
#define r750(i) thiz->a[404*256+i]
#define r751(i) thiz->a[405*256+i]
#define r752(i) thiz->a[406*256+i]
#define r753(i) thiz->a[407*256+i]

/* round 7 / subround 6 */
#define r760(i) thiz->a[408*256+i]
#define r761(i) thiz->a[409*256+i]
#define r762(i) thiz->a[410*256+i]
#define r763(i) thiz->a[411*256+i]

/* round 7 / subround 7 */
#define r770(i) thiz->a[412*256+i]
#define r771(i) thiz->a[413*256+i]
#define r772(i) thiz->a[414*256+i]
#define r773(i) thiz->a[415*256+i]

/* round 7 / subround 8 */
#define r780(i) thiz->a[416*256+i]
#define r781(i) thiz->a[417*256+i]
#define r782(i) thiz->a[418*256+i]
#define r783(i) thiz->a[419*256+i]

/* round 7 / subround 9 */
#define r790(i) thiz->a[420*256+i]
#define r791(i) thiz->a[421*256+i]
#define r792(i) thiz->a[422*256+i]
#define r793(i) thiz->a[423*256+i]

/* round 7 / subround 10 */
#define r7a0(i) thiz->a[424*256+i]
#define r7a1(i) thiz->a[425*256+i]
#define r7a2(i) thiz->a[426*256+i]
#define r7a3(i) thiz->a[427*256+i]

/* round 7 / subround 11 */
#define r7b0(i) thiz->a[428*256+i]
#define r7b1(i) thiz->a[429*256+i]
#define r7b2(i) thiz->a[430*256+i]
#define r7b3(i) thiz->a[431*256+i]

/* round 7 / subround 12 */
#define r7c0(i) thiz->a[432*256+i]
#define r7c1(i) thiz->a[433*256+i]
#define r7c2(i) thiz->a[434*256+i]
#define r7c3(i) thiz->a[435*256+i]

/* round 7 / subround 13 */
#define r7d0(i) thiz->a[436*256+i]
#define r7d1(i) thiz->a[437*256+i]
#define r7d2(i) thiz->a[438*256+i]
#define r7d3(i) thiz->a[439*256+i]

/* round 7 / subround 14 */
#define r7e0(i) thiz->a[440*256+i]
#define r7e1(i) thiz->a[441*256+i]
#define r7e2(i) thiz->a[442*256+i]
#define r7e3(i) thiz->a[443*256+i]

/* round 7 / subround 15 */
#define r7f0(i) thiz->a[444*256+i]
#define r7f1(i) thiz->a[445*256+i]
#define r7f2(i) thiz->a[446*256+i]
#define r7f3(i) thiz->a[447*256+i]

/* round 8 */

/* round 8 / subround 0 */
#define r800(i) thiz->a[448*256+i]
#define r801(i) thiz->a[449*256+i]
#define r802(i) thiz->a[450*256+i]
#define r803(i) thiz->a[451*256+i]

/* round 8 / subround 1 */
#define r810(i) thiz->a[452*256+i]
#define r811(i) thiz->a[453*256+i]
#define r812(i) thiz->a[454*256+i]
#define r813(i) thiz->a[455*256+i]

/* round 8 / subround 2 */
#define r820(i) thiz->a[456*256+i]
#define r821(i) thiz->a[457*256+i]
#define r822(i) thiz->a[458*256+i]
#define r823(i) thiz->a[459*256+i]

/* round 8 / subround 3 */
#define r830(i) thiz->a[460*256+i]
#define r831(i) thiz->a[461*256+i]
#define r832(i) thiz->a[462*256+i]
#define r833(i) thiz->a[463*256+i]

/* round 8 / subround 4 */
#define r840(i) thiz->a[464*256+i]
#define r841(i) thiz->a[465*256+i]
#define r842(i) thiz->a[466*256+i]
#define r843(i) thiz->a[467*256+i]

/* round 8 / subround 5 */
#define r850(i) thiz->a[468*256+i]
#define r851(i) thiz->a[469*256+i]
#define r852(i) thiz->a[470*256+i]
#define r853(i) thiz->a[471*256+i]

/* round 8 / subround 6 */
#define r860(i) thiz->a[472*256+i]
#define r861(i) thiz->a[473*256+i]
#define r862(i) thiz->a[474*256+i]
#define r863(i) thiz->a[475*256+i]

/* round 8 / subround 7 */
#define r870(i) thiz->a[476*256+i]
#define r871(i) thiz->a[477*256+i]
#define r872(i) thiz->a[478*256+i]
#define r873(i) thiz->a[479*256+i]

/* round 8 / subround 8 */
#define r880(i) thiz->a[480*256+i]
#define r881(i) thiz->a[481*256+i]
#define r882(i) thiz->a[482*256+i]
#define r883(i) thiz->a[483*256+i]

/* round 8 / subround 9 */
#define r890(i) thiz->a[484*256+i]
#define r891(i) thiz->a[485*256+i]
#define r892(i) thiz->a[486*256+i]
#define r893(i) thiz->a[487*256+i]

/* round 8 / subround 10 */
#define r8a0(i) thiz->a[488*256+i]
#define r8a1(i) thiz->a[489*256+i]
#define r8a2(i) thiz->a[490*256+i]
#define r8a3(i) thiz->a[491*256+i]

/* round 8 / subround 11 */
#define r8b0(i) thiz->a[492*256+i]
#define r8b1(i) thiz->a[493*256+i]
#define r8b2(i) thiz->a[494*256+i]
#define r8b3(i) thiz->a[495*256+i]

/* round 8 / subround 12 */
#define r8c0(i) thiz->a[496*256+i]
#define r8c1(i) thiz->a[497*256+i]
#define r8c2(i) thiz->a[498*256+i]
#define r8c3(i) thiz->a[499*256+i]

/* round 8 / subround 13 */
#define r8d0(i) thiz->a[500*256+i]
#define r8d1(i) thiz->a[501*256+i]
#define r8d2(i) thiz->a[502*256+i]
#define r8d3(i) thiz->a[503*256+i]

/* round 8 / subround 14 */
#define r8e0(i) thiz->a[504*256+i]
#define r8e1(i) thiz->a[505*256+i]
#define r8e2(i) thiz->a[506*256+i]
#define r8e3(i) thiz->a[507*256+i]

/* round 8 / subround 15 */
#define r8f0(i) thiz->a[508*256+i]
#define r8f1(i) thiz->a[509*256+i]
#define r8f2(i) thiz->a[510*256+i]
#define r8f3(i) thiz->a[511*256+i]

/* round 9 & 10 */

/* round 9 / subround 0 */
#define r900(i) thiz->a[512*256+i]
#define r901(i) thiz->a[513*256+i]
#define r902(i) thiz->a[514*256+i]
#define r903(i) thiz->a[515*256+i]

/* round 10 / subround 0 */
#define ra00(i) thiz->a[516*256+i]

/* round 9 / subround 1 */
#define r910(i) thiz->a[517*256+i]
#define r911(i) thiz->a[518*256+i]
#define r912(i) thiz->a[519*256+i]
#define r913(i) thiz->a[520*256+i]

/* round 10 / subround 1 */
#define ra10(i) thiz->a[521*256+i]

/* round 9 / subround 2 */
#define r920(i) thiz->a[522*256+i]
#define r921(i) thiz->a[523*256+i]
#define r922(i) thiz->a[524*256+i]
#define r923(i) thiz->a[525*256+i]

/* round 10 / subround 2 */
#define ra20(i) thiz->a[526*256+i]

/* round 9 / subround 3 */
#define r930(i) thiz->a[527*256+i]
#define r931(i) thiz->a[528*256+i]
#define r932(i) thiz->a[529*256+i]
#define r933(i) thiz->a[530*256+i]

/* round 10 / subround 3 */
#define ra30(i) thiz->a[531*256+i]

/* round 9 / subround 4 */
#define r940(i) thiz->a[532*256+i]
#define r941(i) thiz->a[533*256+i]
#define r942(i) thiz->a[534*256+i]
#define r943(i) thiz->a[535*256+i]

/* round 10 / subround 4 */
#define ra40(i) thiz->a[536*256+i]

/* round 9 / subround 5 */
#define r950(i) thiz->a[537*256+i]
#define r951(i) thiz->a[538*256+i]
#define r952(i) thiz->a[539*256+i]
#define r953(i) thiz->a[540*256+i]

/* round 10 / subround 5 */
#define ra50(i) thiz->a[541*256+i]

/* round 9 / subround 6 */
#define r960(i) thiz->a[542*256+i]
#define r961(i) thiz->a[543*256+i]
#define r962(i) thiz->a[544*256+i]
#define r963(i) thiz->a[545*256+i]

/* round 10 / subround 6 */
#define ra60(i) thiz->a[546*256+i]

/* round 9 / subround 7 */
#define r970(i) thiz->a[547*256+i]
#define r971(i) thiz->a[548*256+i]
#define r972(i) thiz->a[549*256+i]
#define r973(i) thiz->a[550*256+i]

/* round 10 / subround 7 */
#define ra70(i) thiz->a[551*256+i]

/* round 9 / subround 8 */
#define r980(i) thiz->a[552*256+i]
#define r981(i) thiz->a[553*256+i]
#define r982(i) thiz->a[554*256+i]
#define r983(i) thiz->a[555*256+i]

/* round 10 / subround 8 */
#define ra80(i) thiz->a[556*256+i]

/* round 9 / subround 9 */
#define r990(i) thiz->a[557*256+i]
#define r991(i) thiz->a[558*256+i]
#define r992(i) thiz->a[559*256+i]
#define r993(i) thiz->a[560*256+i]

/* round 10 / subround 9 */
#define ra90(i) thiz->a[561*256+i]

/* round 9 / subround 10 */
#define r9a0(i) thiz->a[562*256+i]
#define r9a1(i) thiz->a[563*256+i]
#define r9a2(i) thiz->a[564*256+i]
#define r9a3(i) thiz->a[565*256+i]

/* round 10 / subround 10 */
#define raa0(i) thiz->a[566*256+i]

/* round 9 / subround 11 */
#define r9b0(i) thiz->a[567*256+i]
#define r9b1(i) thiz->a[568*256+i]
#define r9b2(i) thiz->a[569*256+i]
#define r9b3(i) thiz->a[570*256+i]

/* round 10 / subround 11 */
#define rab0(i) thiz->a[571*256+i]

/* round 9 / subround 12 */
#define r9c0(i) thiz->a[572*256+i]
#define r9c1(i) thiz->a[573*256+i]
#define r9c2(i) thiz->a[574*256+i]
#define r9c3(i) thiz->a[575*256+i]

/* round 10 / subround 12 */
#define rac0(i) thiz->a[576*256+i]

/* round 9 / subround 13 */
#define r9d0(i) thiz->a[577*256+i]
#define r9d1(i) thiz->a[578*256+i]
#define r9d2(i) thiz->a[579*256+i]
#define r9d3(i) thiz->a[580*256+i]

/* round 10 / subround 13 */
#define rad0(i) thiz->a[581*256+i]

/* round 9 / subround 14 */
#define r9e0(i) thiz->a[582*256+i]
#define r9e1(i) thiz->a[583*256+i]
#define r9e2(i) thiz->a[584*256+i]
#define r9e3(i) thiz->a[585*256+i]

/* round 10 / subround 14 */
#define rae0(i) thiz->a[586*256+i]

/* round 9 / subround 15 */
#define r9f0(i) thiz->a[587*256+i]
#define r9f1(i) thiz->a[588*256+i]
#define r9f2(i) thiz->a[589*256+i]
#define r9f3(i) thiz->a[590*256+i]

/* round 10 / subround 15 */
#define raf0(i) thiz->a[591*256+i]

STEE
void aes_wb_decryptor(struct Wbox *thiz, const uint8_t in[16],
                      uint8_t out[16]) {
  uint8_t a00, a01, a02, a03, a10, a11, a12, a13;
  uint8_t a20, a21, a22, a23, a30, a31, a32, a33;

  /* round 1 */

  /* round 1 / subround 0 */
  uint8_t b00 = r000(in[0]) ^ r001(in[13]) ^ r002(in[10]) ^ r003(in[7]);

  /* round 1 / subround 1 */
  uint8_t b01 = r010(in[0]) ^ r011(in[13]) ^ r012(in[10]) ^ r013(in[7]);

  /* round 1 / subround 2 */
  uint8_t b02 = r020(in[0]) ^ r021(in[13]) ^ r022(in[10]) ^ r023(in[7]);

  /* round 1 / subround 3 */
  uint8_t b03 = r030(in[0]) ^ r031(in[13]) ^ r032(in[10]) ^ r033(in[7]);

  /* round 1 / subround 4 */
  uint8_t b10 = r040(in[4]) ^ r041(in[1]) ^ r042(in[14]) ^ r043(in[11]);

  /* round 1 / subround 5 */
  uint8_t b11 = r050(in[4]) ^ r051(in[1]) ^ r052(in[14]) ^ r053(in[11]);

  /* round 1 / subround 6 */
  uint8_t b12 = r060(in[4]) ^ r061(in[1]) ^ r062(in[14]) ^ r063(in[11]);

  /* round 1 / subround 7 */
  uint8_t b13 = r070(in[4]) ^ r071(in[1]) ^ r072(in[14]) ^ r073(in[11]);

  /* round 1 / subround 8 */
  uint8_t b20 = r080(in[8]) ^ r081(in[5]) ^ r082(in[2]) ^ r083(in[15]);

  /* round 1 / subround 9 */
  uint8_t b21 = r090(in[8]) ^ r091(in[5]) ^ r092(in[2]) ^ r093(in[15]);

  /* round 1 / subround 10 */
  uint8_t b22 = r0a0(in[8]) ^ r0a1(in[5]) ^ r0a2(in[2]) ^ r0a3(in[15]);

  /* round 1 / subround 11 */
  uint8_t b23 = r0b0(in[8]) ^ r0b1(in[5]) ^ r0b2(in[2]) ^ r0b3(in[15]);

  /* round 1 / subround 12 */
  uint8_t b30 = r0c0(in[12]) ^ r0c1(in[9]) ^ r0c2(in[6]) ^ r0c3(in[3]);

  /* round 1 / subround 13 */
  uint8_t b31 = r0d0(in[12]) ^ r0d1(in[9]) ^ r0d2(in[6]) ^ r0d3(in[3]);

  /* round 1 / subround 14 */
  uint8_t b32 = r0e0(in[12]) ^ r0e1(in[9]) ^ r0e2(in[6]) ^ r0e3(in[3]);

  /* round 1 / subround 15 */
  uint8_t b33 = r0f0(in[12]) ^ r0f1(in[9]) ^ r0f2(in[6]) ^ r0f3(in[3]);

  /* round 2 */

  /* round 2 / subround 0 */
  a00 = r200(b00) ^ r201(b31) ^ r202(b22) ^ r203(b13);

  /* round 2 / subround 1 */
  a01 = r210(b00) ^ r211(b31) ^ r212(b22) ^ r213(b13);

  /* round 2 / subround 2 */
  a02 = r220(b00) ^ r221(b31) ^ r222(b22) ^ r223(b13);

  /* round 2 / subround 3 */
  a03 = r230(b00) ^ r231(b31) ^ r232(b22) ^ r233(b13);

  /* round 2 / subround 4 */
  a10 = r240(b10) ^ r241(b01) ^ r242(b32) ^ r243(b23);

  /* round 2 / subround 5 */
  a11 = r250(b10) ^ r251(b01) ^ r252(b32) ^ r253(b23);

  /* round 2 / subround 6 */
  a12 = r260(b10) ^ r261(b01) ^ r262(b32) ^ r263(b23);

  /* round 2 / subround 7 */
  a13 = r270(b10) ^ r271(b01) ^ r272(b32) ^ r273(b23);

  /* round 2 / subround 8 */
  a20 = r280(b20) ^ r281(b11) ^ r282(b02) ^ r283(b33);

  /* round 2 / subround 9 */
  a21 = r290(b20) ^ r291(b11) ^ r292(b02) ^ r293(b33);

  /* round 2 / subround 10 */
  a22 = r2a0(b20) ^ r2a1(b11) ^ r2a2(b02) ^ r2a3(b33);

  /* round 2 / subround 11 */
  a23 = r2b0(b20) ^ r2b1(b11) ^ r2b2(b02) ^ r2b3(b33);

  /* round 2 / subround 12 */
  a30 = r2c0(b30) ^ r2c1(b21) ^ r2c2(b12) ^ r2c3(b03);

  /* round 2 / subround 13 */
  a31 = r2d0(b30) ^ r2d1(b21) ^ r2d2(b12) ^ r2d3(b03);

  /* round 2 / subround 14 */
  a32 = r2e0(b30) ^ r2e1(b21) ^ r2e2(b12) ^ r2e3(b03);

  /* round 2 / subround 15 */
  a33 = r2f0(b30) ^ r2f1(b21) ^ r2f2(b12) ^ r2f3(b03);

  /* round 3 */

  /* round 3 / subround 0 */
  b00 = r300(a00) ^ r301(a31) ^ r302(a22) ^ r303(a13);

  /* round 3 / subround 1 */
  b01 = r310(a00) ^ r311(a31) ^ r312(a22) ^ r313(a13);

  /* round 3 / subround 2 */
  b02 = r320(a00) ^ r321(a31) ^ r322(a22) ^ r323(a13);

  /* round 3 / subround 3 */
  b03 = r330(a00) ^ r331(a31) ^ r332(a22) ^ r333(a13);

  /* round 3 / subround 4 */
  b10 = r340(a10) ^ r341(a01) ^ r342(a32) ^ r343(a23);

  /* round 3 / subround 5 */
  b11 = r350(a10) ^ r351(a01) ^ r352(a32) ^ r353(a23);

  /* round 3 / subround 6 */
  b12 = r360(a10) ^ r361(a01) ^ r362(a32) ^ r363(a23);

  /* round 3 / subround 7 */
  b13 = r370(a10) ^ r371(a01) ^ r372(a32) ^ r373(a23);

  /* round 3 / subround 8 */
  b20 = r380(a20) ^ r381(a11) ^ r382(a02) ^ r383(a33);

  /* round 3 / subround 9 */
  b21 = r390(a20) ^ r391(a11) ^ r392(a02) ^ r393(a33);

  /* round 3 / subround 10 */
  b22 = r3a0(a20) ^ r3a1(a11) ^ r3a2(a02) ^ r3a3(a33);

  /* round 3 / subround 11 */
  b23 = r3b0(a20) ^ r3b1(a11) ^ r3b2(a02) ^ r3b3(a33);

  /* round 3 / subround 12 */
  b30 = r3c0(a30) ^ r3c1(a21) ^ r3c2(a12) ^ r3c3(a03);

  /* round 3 / subround 13 */
  b31 = r3d0(a30) ^ r3d1(a21) ^ r3d2(a12) ^ r3d3(a03);

  /* round 3 / subround 14 */
  b32 = r3e0(a30) ^ r3e1(a21) ^ r3e2(a12) ^ r3e3(a03);

  /* round 3 / subround 15 */
  b33 = r3f0(a30) ^ r3f1(a21) ^ r3f2(a12) ^ r3f3(a03);

  /* round 4 */

  /* round 4 / subround 0 */
  a00 = r400(b00) ^ r401(b31) ^ r402(b22) ^ r403(b13);

  /* round 4 / subround 1 */
  a01 = r410(b00) ^ r411(b31) ^ r412(b22) ^ r413(b13);

  /* round 4 / subround 2 */
  a02 = r420(b00) ^ r421(b31) ^ r422(b22) ^ r423(b13);

  /* round 4 / subround 3 */
  a03 = r430(b00) ^ r431(b31) ^ r432(b22) ^ r433(b13);

  /* round 4 / subround 4 */
  a10 = r440(b10) ^ r441(b01) ^ r442(b32) ^ r443(b23);

  /* round 4 / subround 5 */
  a11 = r450(b10) ^ r451(b01) ^ r452(b32) ^ r453(b23);

  /* round 4 / subround 6 */
  a12 = r460(b10) ^ r461(b01) ^ r462(b32) ^ r463(b23);

  /* round 4 / subround 7 */
  a13 = r470(b10) ^ r471(b01) ^ r472(b32) ^ r473(b23);

  /* round 4 / subround 8 */
  a20 = r480(b20) ^ r481(b11) ^ r482(b02) ^ r483(b33);

  /* round 4 / subround 9 */
  a21 = r490(b20) ^ r491(b11) ^ r492(b02) ^ r493(b33);

  /* round 4 / subround 10 */
  a22 = r4a0(b20) ^ r4a1(b11) ^ r4a2(b02) ^ r4a3(b33);

  /* round 4 / subround 11 */
  a23 = r4b0(b20) ^ r4b1(b11) ^ r4b2(b02) ^ r4b3(b33);

  /* round 4 / subround 12 */
  a30 = r4c0(b30) ^ r4c1(b21) ^ r4c2(b12) ^ r4c3(b03);

  /* round 4 / subround 13 */
  a31 = r4d0(b30) ^ r4d1(b21) ^ r4d2(b12) ^ r4d3(b03);

  /* round 4 / subround 14 */
  a32 = r4e0(b30) ^ r4e1(b21) ^ r4e2(b12) ^ r4e3(b03);

  /* round 4 / subround 15 */
  a33 = r4f0(b30) ^ r4f1(b21) ^ r4f2(b12) ^ r4f3(b03);

  /* round 5 */

  /* round 5 / subround 0 */
  b00 = r500(a00) ^ r501(a31) ^ r502(a22) ^ r503(a13);

  /* round 5 / subround 1 */
  b01 = r510(a00) ^ r511(a31) ^ r512(a22) ^ r513(a13);

  /* round 5 / subround 2 */
  b02 = r520(a00) ^ r521(a31) ^ r522(a22) ^ r523(a13);

  /* round 5 / subround 3 */
  b03 = r530(a00) ^ r531(a31) ^ r532(a22) ^ r533(a13);

  /* round 5 / subround 4 */
  b10 = r540(a10) ^ r541(a01) ^ r542(a32) ^ r543(a23);

  /* round 5 / subround 5 */
  b11 = r550(a10) ^ r551(a01) ^ r552(a32) ^ r553(a23);

  /* round 5 / subround 6 */
  b12 = r560(a10) ^ r561(a01) ^ r562(a32) ^ r563(a23);

  /* round 5 / subround 7 */
  b13 = r570(a10) ^ r571(a01) ^ r572(a32) ^ r573(a23);

  /* round 5 / subround 8 */
  b20 = r580(a20) ^ r581(a11) ^ r582(a02) ^ r583(a33);

  /* round 5 / subround 9 */
  b21 = r590(a20) ^ r591(a11) ^ r592(a02) ^ r593(a33);

  /* round 5 / subround 10 */
  b22 = r5a0(a20) ^ r5a1(a11) ^ r5a2(a02) ^ r5a3(a33);

  /* round 5 / subround 11 */
  b23 = r5b0(a20) ^ r5b1(a11) ^ r5b2(a02) ^ r5b3(a33);

  /* round 5 / subround 12 */
  b30 = r5c0(a30) ^ r5c1(a21) ^ r5c2(a12) ^ r5c3(a03);

  /* round 5 / subround 13 */
  b31 = r5d0(a30) ^ r5d1(a21) ^ r5d2(a12) ^ r5d3(a03);

  /* round 5 / subround 14 */
  b32 = r5e0(a30) ^ r5e1(a21) ^ r5e2(a12) ^ r5e3(a03);

  /* round 5 / subround 15 */
  b33 = r5f0(a30) ^ r5f1(a21) ^ r5f2(a12) ^ r5f3(a03);

  /* round 6 */

  /* round 6 / subround 0 */
  a00 = r600(b00) ^ r601(b31) ^ r602(b22) ^ r603(b13);

  /* round 6 / subround 1 */
  a01 = r610(b00) ^ r611(b31) ^ r612(b22) ^ r613(b13);

  /* round 6 / subround 2 */
  a02 = r620(b00) ^ r621(b31) ^ r622(b22) ^ r623(b13);

  /* round 6 / subround 3 */
  a03 = r630(b00) ^ r631(b31) ^ r632(b22) ^ r633(b13);

  /* round 6 / subround 4 */
  a10 = r640(b10) ^ r641(b01) ^ r642(b32) ^ r643(b23);

  /* round 6 / subround 5 */
  a11 = r650(b10) ^ r651(b01) ^ r652(b32) ^ r653(b23);

  /* round 6 / subround 6 */
  a12 = r660(b10) ^ r661(b01) ^ r662(b32) ^ r663(b23);

  /* round 6 / subround 7 */
  a13 = r670(b10) ^ r671(b01) ^ r672(b32) ^ r673(b23);

  /* round 6 / subround 8 */
  a20 = r680(b20) ^ r681(b11) ^ r682(b02) ^ r683(b33);

  /* round 6 / subround 9 */
  a21 = r690(b20) ^ r691(b11) ^ r692(b02) ^ r693(b33);

  /* round 6 / subround 10 */
  a22 = r6a0(b20) ^ r6a1(b11) ^ r6a2(b02) ^ r6a3(b33);

  /* round 6 / subround 11 */
  a23 = r6b0(b20) ^ r6b1(b11) ^ r6b2(b02) ^ r6b3(b33);

  /* round 6 / subround 12 */
  a30 = r6c0(b30) ^ r6c1(b21) ^ r6c2(b12) ^ r6c3(b03);

  /* round 6 / subround 13 */
  a31 = r6d0(b30) ^ r6d1(b21) ^ r6d2(b12) ^ r6d3(b03);

  /* round 6 / subround 14 */
  a32 = r6e0(b30) ^ r6e1(b21) ^ r6e2(b12) ^ r6e3(b03);

  /* round 6 / subround 15 */
  a33 = r6f0(b30) ^ r6f1(b21) ^ r6f2(b12) ^ r6f3(b03);

  /* round 7 */

  /* round 7 / subround 0 */
  b00 = r700(a00) ^ r701(a31) ^ r702(a22) ^ r703(a13);

  /* round 7 / subround 1 */
  b01 = r710(a00) ^ r711(a31) ^ r712(a22) ^ r713(a13);

  /* round 7 / subround 2 */
  b02 = r720(a00) ^ r721(a31) ^ r722(a22) ^ r723(a13);

  /* round 7 / subround 3 */
  b03 = r730(a00) ^ r731(a31) ^ r732(a22) ^ r733(a13);

  /* round 7 / subround 4 */
  b10 = r740(a10) ^ r741(a01) ^ r742(a32) ^ r743(a23);

  /* round 7 / subround 5 */
  b11 = r750(a10) ^ r751(a01) ^ r752(a32) ^ r753(a23);

  /* round 7 / subround 6 */
  b12 = r760(a10) ^ r761(a01) ^ r762(a32) ^ r763(a23);

  /* round 7 / subround 7 */
  b13 = r770(a10) ^ r771(a01) ^ r772(a32) ^ r773(a23);

  /* round 7 / subround 8 */
  b20 = r780(a20) ^ r781(a11) ^ r782(a02) ^ r783(a33);

  /* round 7 / subround 9 */
  b21 = r790(a20) ^ r791(a11) ^ r792(a02) ^ r793(a33);

  /* round 7 / subround 10 */
  b22 = r7a0(a20) ^ r7a1(a11) ^ r7a2(a02) ^ r7a3(a33);

  /* round 7 / subround 11 */
  b23 = r7b0(a20) ^ r7b1(a11) ^ r7b2(a02) ^ r7b3(a33);

  /* round 7 / subround 12 */
  b30 = r7c0(a30) ^ r7c1(a21) ^ r7c2(a12) ^ r7c3(a03);

  /* round 7 / subround 13 */
  b31 = r7d0(a30) ^ r7d1(a21) ^ r7d2(a12) ^ r7d3(a03);

  /* round 7 / subround 14 */
  b32 = r7e0(a30) ^ r7e1(a21) ^ r7e2(a12) ^ r7e3(a03);

  /* round 7 / subround 15 */
  b33 = r7f0(a30) ^ r7f1(a21) ^ r7f2(a12) ^ r7f3(a03);

  /* round 8 */

  /* round 8 / subround 0 */
  a00 = r800(b00) ^ r801(b31) ^ r802(b22) ^ r803(b13);

  /* round 8 / subround 1 */
  a01 = r810(b00) ^ r811(b31) ^ r812(b22) ^ r813(b13);

  /* round 8 / subround 2 */
  a02 = r820(b00) ^ r821(b31) ^ r822(b22) ^ r823(b13);

  /* round 8 / subround 3 */
  a03 = r830(b00) ^ r831(b31) ^ r832(b22) ^ r833(b13);

  /* round 8 / subround 4 */
  a10 = r840(b10) ^ r841(b01) ^ r842(b32) ^ r843(b23);

  /* round 8 / subround 5 */
  a11 = r850(b10) ^ r851(b01) ^ r852(b32) ^ r853(b23);

  /* round 8 / subround 6 */
  a12 = r860(b10) ^ r861(b01) ^ r862(b32) ^ r863(b23);

  /* round 8 / subround 7 */
  a13 = r870(b10) ^ r871(b01) ^ r872(b32) ^ r873(b23);

  /* round 8 / subround 8 */
  a20 = r880(b20) ^ r881(b11) ^ r882(b02) ^ r883(b33);

  /* round 8 / subround 9 */
  a21 = r890(b20) ^ r891(b11) ^ r892(b02) ^ r893(b33);

  /* round 8 / subround 10 */
  a22 = r8a0(b20) ^ r8a1(b11) ^ r8a2(b02) ^ r8a3(b33);

  /* round 8 / subround 11 */
  a23 = r8b0(b20) ^ r8b1(b11) ^ r8b2(b02) ^ r8b3(b33);

  /* round 8 / subround 12 */
  a30 = r8c0(b30) ^ r8c1(b21) ^ r8c2(b12) ^ r8c3(b03);

  /* round 8 / subround 13 */
  a31 = r8d0(b30) ^ r8d1(b21) ^ r8d2(b12) ^ r8d3(b03);

  /* round 8 / subround 14 */
  a32 = r8e0(b30) ^ r8e1(b21) ^ r8e2(b12) ^ r8e3(b03);

  /* round 8 / subround 15 */
  a33 = r8f0(b30) ^ r8f1(b21) ^ r8f2(b12) ^ r8f3(b03);

  /* round 9 & 10 */

  /* round 9 / subround 0 */
  b00 = r900(a00) ^ r901(a31) ^ r902(a22) ^ r903(a13);

  /* round 10 / subround 0 */
  out[0] = ra00(b00);

  /* round 9 / subround 1 */
  b01 = r910(a00) ^ r911(a31) ^ r912(a22) ^ r913(a13);

  /* round 10 / subround 1 */
  out[5] = ra10(b01);

  /* round 9 / subround 2 */
  b02 = r920(a00) ^ r921(a31) ^ r922(a22) ^ r923(a13);

  /* round 10 / subround 2 */
  out[10] = ra20(b02);

  /* round 9 / subround 3 */
  b03 = r930(a00) ^ r931(a31) ^ r932(a22) ^ r933(a13);

  /* round 10 / subround 3 */
  out[15] = ra30(b03);

  /* round 9 / subround 4 */
  b10 = r940(a10) ^ r941(a01) ^ r942(a32) ^ r943(a23);

  /* round 10 / subround 4 */
  out[4] = ra40(b10);

  /* round 9 / subround 5 */
  b11 = r950(a10) ^ r951(a01) ^ r952(a32) ^ r953(a23);

  /* round 10 / subround 5 */
  out[9] = ra50(b11);

  /* round 9 / subround 6 */
  b12 = r960(a10) ^ r961(a01) ^ r962(a32) ^ r963(a23);

  /* round 10 / subround 6 */
  out[14] = ra60(b12);

  /* round 9 / subround 7 */
  b13 = r970(a10) ^ r971(a01) ^ r972(a32) ^ r973(a23);

  /* round 10 / subround 7 */
  out[3] = ra70(b13);

  /* round 9 / subround 8 */
  b20 = r980(a20) ^ r981(a11) ^ r982(a02) ^ r983(a33);

  /* round 10 / subround 8 */
  out[8] = ra80(b20);

  /* round 9 / subround 9 */
  b21 = r990(a20) ^ r991(a11) ^ r992(a02) ^ r993(a33);

  /* round 10 / subround 9 */
  out[13] = ra90(b21);

  /* round 9 / subround 10 */
  b22 = r9a0(a20) ^ r9a1(a11) ^ r9a2(a02) ^ r9a3(a33);

  /* round 10 / subround 10 */
  out[2] = raa0(b22);

  /* round 9 / subround 11 */
  b23 = r9b0(a20) ^ r9b1(a11) ^ r9b2(a02) ^ r9b3(a33);

  /* round 10 / subround 11 */
  out[7] = rab0(b23);

  /* round 9 / subround 12 */
  b30 = r9c0(a30) ^ r9c1(a21) ^ r9c2(a12) ^ r9c3(a03);

  /* round 10 / subround 12 */
  out[12] = rac0(b30);

  /* round 9 / subround 13 */
  b31 = r9d0(a30) ^ r9d1(a21) ^ r9d2(a12) ^ r9d3(a03);

  /* round 10 / subround 13 */
  out[1] = rad0(b31);

  /* round 9 / subround 14 */
  b32 = r9e0(a30) ^ r9e1(a21) ^ r9e2(a12) ^ r9e3(a03);

  /* round 10 / subround 14 */
  out[6] = rae0(b32);

  /* round 9 / subround 15 */
  b33 = r9f0(a30) ^ r9f1(a21) ^ r9f2(a12) ^ r9f3(a03);

  /* round 10 / subround 15 */
  out[11] = raf0(b33);
}

/*EOF*/
