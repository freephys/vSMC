//============================================================================
// vSMC/example/rng/src/rng_ars_validation.cpp
//----------------------------------------------------------------------------
//                         vSMC: Scalable Monte Carlo
//----------------------------------------------------------------------------
// Copyright (c) 2013-2015, Yan Zhou
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#include "rng_validation.hpp"
#include <vsmc/rng/ars.hpp>

int main()
{
    std::vector<unsigned long long> ARS_1x32Result = {796127361ULL,
        1626711678ULL, 3589784708ULL, 2622921265ULL, 760398377ULL,
        2304767162ULL, 1559788848ULL, 3908080103ULL, 2971437870ULL,
        38358525ULL, 729719251ULL, 213650164ULL, 1825114697ULL, 585061349ULL,
        3621828443ULL, 3711622123ULL, 1246808021ULL, 1595493378ULL,
        881987222ULL, 1117914193ULL, 2626122435ULL, 579883476ULL,
        1931525198ULL, 4044461532ULL, 3273721905ULL, 3575678374ULL,
        3164496350ULL, 3927885071ULL, 3143026182ULL, 2333816398ULL,
        2738864050ULL, 744839911ULL, 2131383844ULL, 3894350015ULL,
        1455037725ULL, 254237405ULL, 1416033138ULL, 1408035017ULL,
        3275890010ULL, 64870272ULL, 1203169913ULL, 3219848914ULL,
        3562485125ULL, 3839807324ULL, 3738438179ULL, 4107646450ULL,
        2034716230ULL, 2388102770ULL, 2949994800ULL, 2229307724ULL,
        361576060ULL, 836181710ULL, 3535320762ULL, 1759903786ULL,
        4097998713ULL, 3143632161ULL, 3351571794ULL, 3406708424ULL,
        1280975264ULL, 451179313ULL, 2331632315ULL, 3938548920ULL,
        4219194363ULL, 1457619074ULL, 4283063402ULL, 3652951458ULL,
        3426163109ULL, 4285873557ULL, 1900180677ULL, 4027559286ULL,
        3095311675ULL, 1440105327ULL, 4178782292ULL, 1309693355ULL,
        978101075ULL, 4216491039ULL, 1979069466ULL, 2826005653ULL,
        3105258647ULL, 1376778200ULL, 2249921551ULL, 2871561047ULL,
        834198938ULL, 331171955ULL, 916885012ULL, 1305118162ULL,
        4092836087ULL, 3160950143ULL, 662550367ULL, 1347103458ULL,
        587514770ULL, 4186838558ULL, 1382635580ULL, 621763216ULL,
        4000865830ULL, 3370148887ULL, 716106018ULL, 2948151585ULL,
        2306165114ULL, 2565643371ULL, 37ULL};
    rng_validation<vsmc::ARS_1x32>(ARS_1x32Result, "ARS_1x32");

    std::vector<unsigned long long> ARS_2x32Result = {1506401005ULL,
        1566930312ULL, 3968849347ULL, 868414330ULL, 864859197ULL,
        3614870904ULL, 1092093126ULL, 4199618827ULL, 937540971ULL,
        1652772473ULL, 859442176ULL, 560119613ULL, 2110866132ULL,
        3149457632ULL, 1759796229ULL, 1722033677ULL, 3974644070ULL,
        1835900443ULL, 3061905127ULL, 1480172411ULL, 58115972ULL, 65514580ULL,
        3387839417ULL, 1866185824ULL, 1890001072ULL, 406473227ULL,
        4225085627ULL, 3474519432ULL, 3470686628ULL, 653628432ULL,
        3183313425ULL, 3101700313ULL, 1180983610ULL, 81973836ULL,
        3374940972ULL, 2025737334ULL, 3224984896ULL, 3705088712ULL,
        4038102249ULL, 3836796976ULL, 3014914718ULL, 167498511ULL,
        2921301210ULL, 3707020648ULL, 3644506709ULL, 1075696384ULL,
        2886224212ULL, 1986260920ULL, 1497879829ULL, 3298856737ULL,
        2845420298ULL, 656128063ULL, 1910005398ULL, 1768072174ULL,
        4119629537ULL, 2785940143ULL, 1092831933ULL, 2914488089ULL,
        4173440366ULL, 58838488ULL, 2868386626ULL, 2655183041ULL,
        3678439516ULL, 776431926ULL, 3582928394ULL, 304448614ULL,
        1958017664ULL, 1781011066ULL, 1408354806ULL, 659082921ULL,
        701248598ULL, 4181186469ULL, 3758410321ULL, 2625899026ULL,
        3840604415ULL, 345880411ULL, 244475133ULL, 3662560368ULL,
        1166346977ULL, 2501681875ULL, 141024365ULL, 2569387004ULL,
        1082831329ULL, 1448013316ULL, 2338480763ULL, 784655723ULL,
        1408758071ULL, 406225150ULL, 1647999336ULL, 2083778512ULL,
        3154377994ULL, 2331643778ULL, 3797434671ULL, 580601859ULL,
        1517264564ULL, 2621330ULL, 2384645041ULL, 235873275ULL, 3246836495ULL,
        1076923897ULL, 68ULL};
    rng_validation<vsmc::ARS_2x32>(ARS_2x32Result, "ARS_2x32");

    std::vector<unsigned long long> ARS_4x32Result = {968508700ULL,
        707936791ULL, 3161631372ULL, 2580719561ULL, 1706521342ULL,
        3042400620ULL, 1597618311ULL, 387899364ULL, 2464168596ULL,
        3859099870ULL, 1145603706ULL, 3624763188ULL, 2236197454ULL,
        1997247559ULL, 585086256ULL, 2857406289ULL, 1479062212ULL,
        884306268ULL, 1549002861ULL, 4201497302ULL, 1617541751ULL, 3874503ULL,
        467261452ULL, 1267984312ULL, 124640380ULL, 880037478ULL,
        2227060856ULL, 5431372ULL, 331820446ULL, 3720249381ULL, 4273392319ULL,
        3612508498ULL, 929199115ULL, 440500546ULL, 1629636029ULL,
        458334912ULL, 4248058720ULL, 1319319375ULL, 2710625922ULL,
        4178453209ULL, 2333533333ULL, 34488990ULL, 2670813119ULL,
        295218821ULL, 4071777778ULL, 2684404724ULL, 1794978929ULL,
        2826072410ULL, 3140967576ULL, 808177955ULL, 1938732503ULL,
        1082240565ULL, 3437968544ULL, 1843838621ULL, 3393399892ULL,
        1602677731ULL, 3767901083ULL, 3289607443ULL, 3704321307ULL,
        3124739907ULL, 3119180280ULL, 4013105362ULL, 3879264619ULL,
        437745040ULL, 1537609947ULL, 3592512626ULL, 260364102ULL,
        4220626429ULL, 4235875254ULL, 234844369ULL, 2305015364ULL,
        2225670627ULL, 2283016334ULL, 3677666670ULL, 300572989ULL,
        4212235381ULL, 955962174ULL, 3776360964ULL, 4241371054ULL,
        3220846649ULL, 3348139499ULL, 3903057570ULL, 3279654432ULL,
        2691412314ULL, 213311666ULL, 332245734ULL, 2043859053ULL,
        452742092ULL, 4166587657ULL, 2192774165ULL, 2355485558ULL,
        2668612791ULL, 2366335778ULL, 4209733055ULL, 115110763ULL,
        3242852075ULL, 2303693576ULL, 3454226431ULL, 2270044267ULL,
        3951378588ULL, 26ULL};
    rng_validation<vsmc::ARS_4x32>(ARS_4x32Result, "ARS_4x32");

    std::vector<unsigned long long> ARS_8x32Result = {2446274506ULL,
        133501369ULL, 1202595740ULL, 819545423ULL, 3512234747ULL,
        3351125661ULL, 4102137845ULL, 1852021548ULL, 1875557326ULL,
        2985158329ULL, 2052575654ULL, 1371407915ULL, 1464514730ULL,
        2109892726ULL, 109957215ULL, 3639367848ULL, 3874290711ULL,
        3307013579ULL, 3224170249ULL, 1381672899ULL, 2030124854ULL,
        2946869765ULL, 1444921763ULL, 343962719ULL, 677336442ULL,
        3678160800ULL, 3024601112ULL, 2650702996ULL, 90697313ULL,
        3434528274ULL, 2932755910ULL, 2185518059ULL, 1387380003ULL,
        1105395701ULL, 2814342550ULL, 1379895492ULL, 3572972366ULL,
        2722489814ULL, 3288076409ULL, 1063009184ULL, 2312492951ULL,
        1759467243ULL, 245084815ULL, 1431045673ULL, 304035409ULL,
        1000830112ULL, 2555668527ULL, 88560149ULL, 3832418118ULL,
        2967067595ULL, 2816072325ULL, 3616438708ULL, 2341332189ULL,
        1153160841ULL, 1503324190ULL, 2022794513ULL, 2903531028ULL,
        2116636753ULL, 406511774ULL, 226487284ULL, 1654717444ULL,
        1107092733ULL, 3681063381ULL, 2703242221ULL, 3592400058ULL,
        61447738ULL, 1635528429ULL, 1555245155ULL, 2483511608ULL,
        2109039859ULL, 2945346256ULL, 767158063ULL, 18713800ULL,
        2792385211ULL, 3310330852ULL, 1001284280ULL, 428542089ULL,
        873660946ULL, 3388051135ULL, 2457791830ULL, 3555772283ULL,
        960667167ULL, 3486979536ULL, 2029384214ULL, 2594535219ULL,
        2535775285ULL, 16229456ULL, 3772134829ULL, 310679290ULL,
        1725431670ULL, 2529826086ULL, 2004688916ULL, 4089119264ULL,
        1736492334ULL, 2581181268ULL, 1266415523ULL, 128875792ULL,
        184088490ULL, 2003669117ULL, 1705169842ULL, 95ULL};
    rng_validation<vsmc::ARS_8x32>(ARS_8x32Result, "ARS_8x32");

    std::vector<unsigned long long> ARS_1x64Result = {16229323214839758881ULL,
        13563030803324504954ULL, 1240872178739021830ULL,
        2950613756740326187ULL, 6770852898833197211ULL,
        15864460101854243896ULL, 303846497084258108ULL,
        6355903770177269161ULL, 12737242811135977078ULL,
        5355992873849910746ULL, 2072728430819746974ULL,
        16307450330777583165ULL, 8258597156617899207ULL,
        2841212012417803492ULL, 13782716504705785336ULL,
        6572852146355387713ULL, 6752306954526247558ULL,
        15725393172941506581ULL, 17327776427048697760ULL,
        10347731340373982746ULL, 4933898792256985552ULL,
        8983561836991337920ULL, 15359987272990856610ULL,
        9849715034405362720ULL, 245351778789284520ULL,
        13186708506934631719ULL, 7834430246484426596ULL,
        3330287897104352015ULL, 15930770884283545099ULL,
        17013039194912647182ULL, 7333096571965021251ULL,
        9670555301653754652ULL, 9188846394350683700ULL,
        15029952665237128162ULL, 11098138813662366064ULL,
        8794851048820183702ULL, 2277276095419463621ULL,
        11325207083756286455ULL, 17685606936584743593ULL,
        4389849686898753851ULL, 85948425532752743ULL, 18418708296196392868ULL,
        12632749615857751637ULL, 12589394054997494079ULL,
        1527224945421579497ULL, 305273856375617651ULL, 2579819522538971254ULL,
        18373768505362587150ULL, 8610201686575498286ULL,
        6165167248128329293ULL, 16785950063193593704ULL,
        11941098663499280934ULL, 10138317522536075180ULL,
        13720327717192791971ULL, 303839823981511594ULL,
        13080200505595733700ULL, 4564423144631288240ULL,
        14472687730112144027ULL, 9256731734594833122ULL,
        12814070280864448743ULL, 4102768587669147796ULL,
        15982814616021848946ULL, 3352113425425106356ULL,
        11544887835202315239ULL, 7054223831146589208ULL,
        4175129243584477823ULL, 9421811630887066797ULL,
        9331860670245388186ULL, 2053624971824263526ULL,
        11062215365178609892ULL, 2842876263779664045ULL,
        13087945687744096531ULL, 672349020097127055ULL,
        12180974550944390225ULL, 1467869741153079464ULL,
        2458432810391925750ULL, 5991155138247896839ULL,
        2926458957651690571ULL, 4436876832264488530ULL,
        16246903076826137080ULL, 14060893538056702301ULL,
        5327354760677352705ULL, 8698625928409720668ULL,
        13105709383251734894ULL, 2075461333528576538ULL,
        13555963267222774400ULL, 8363858076568934849ULL,
        191073807150877489ULL, 14818753024194256525ULL,
        17120487170031646424ULL, 8025200248637141834ULL,
        3378952841769122404ULL, 2218454443998120080ULL,
        17768301160744122533ULL, 182955778416768784ULL,
        16157733909211096573ULL, 11301303264372629514ULL,
        2387880429689743572ULL, 5004085329040803030ULL,
        7028562077673584513ULL, 16ULL};
    rng_validation<vsmc::ARS_1x64>(ARS_1x64Result, "ARS_1x64");

    std::vector<unsigned long long> ARS_2x64Result = {13344282735961089769ULL,
        11051429143314858410ULL, 5891087496938012467ULL,
        3485794132210490024ULL, 11574588018780778635ULL,
        6538297211646685089ULL, 13710368974552466577ULL,
        4976628926753383415ULL, 16094792924853702535ULL,
        12804536247037479146ULL, 16070756711876179943ULL,
        12429679744269758857ULL, 11482075053156095862ULL,
        17190536218928753979ULL, 12897427911052679426ULL,
        17251189473095536688ULL, 8887352455052562115ULL,
        13362137323687493215ULL, 9435783606676184013ULL,
        11358440021326825302ULL, 9408651781467816789ULL,
        4040596736130451809ULL, 5432357394592748887ULL,
        3525672725025997312ULL, 11860813077867513741ULL,
        11222803289830620511ULL, 2255775404333435860ULL,
        4110890953847580930ULL, 821926735745307746ULL,
        13970901295459596743ULL, 6904373322315115531ULL,
        7092998467379112335ULL, 13199925814030332806ULL,
        17596657645197853396ULL, 13348782965975486250ULL,
        353371976971893351ULL, 8287425601961662964ULL,
        11219762899462754917ULL, 5873041580670829223ULL,
        14011514596726157704ULL, 13709344630488595727ULL,
        1078322611445506394ULL, 2181112096870813022ULL,
        10701677666646985577ULL, 10449443567295408126ULL,
        9530830826471739297ULL, 14424050422758243098ULL,
        7842756877296006261ULL, 11737862943421904386ULL,
        7056361778655928828ULL, 14087952828603567031ULL,
        2539017255333933824ULL, 1772746132331269744ULL,
        6415105183302828286ULL, 9715486486055295443ULL,
        9375632216405266626ULL, 7175237378857747670ULL,
        15272704509664655701ULL, 2716266903141104978ULL,
        1133080724832290795ULL, 4410378244332956230ULL,
        4609706796334117474ULL, 17645361552433836497ULL,
        11561360527753868044ULL, 15291015481995411938ULL,
        17875460363851647555ULL, 13092476893297916624ULL,
        7650335272259376427ULL, 9714531821695425555ULL,
        16504174706257435071ULL, 7272734726539376735ULL,
        2990447108660061316ULL, 14279425966523789666ULL,
        11737107766094632023ULL, 4840288336957650199ULL,
        2717751966027160755ULL, 1883379119058472048ULL,
        9104820656927863477ULL, 7643464893054368583ULL,
        11380476579405066939ULL, 16261438988647147564ULL,
        325041414695061914ULL, 6846812586511440979ULL, 6007895814970187528ULL,
        4548269247803674324ULL, 14125450422330061346ULL,
        314504108208929041ULL, 13886857233235436328ULL,
        10417276815905641004ULL, 286085858246032742ULL,
        8050106981992559758ULL, 10716903316654062126ULL, 36911369259396032ULL,
        2672752110806807704ULL, 5364408532359965351ULL,
        1721561039084675562ULL, 852060490346622853ULL,
        10529232396430324525ULL, 5716056847800209847ULL,
        12717456429138762872ULL, 100ULL};
    rng_validation<vsmc::ARS_2x64>(ARS_2x64Result, "ARS_2x64");

    std::vector<unsigned long long> ARS_4x64Result = {503690066854205580ULL,
        18064608408436295890ULL, 16378342681065640112ULL,
        1840070065881033855ULL, 2640302039001605818ULL,
        10053757176866153237ULL, 57636577532130014ULL,
        14380874222221648384ULL, 2060502946189963501ULL,
        10524199109505498579ULL, 2150326218441645028ULL,
        4862966703108773331ULL, 3309181532860528875ULL,
        7200016544088045844ULL, 3765466904116291ULL, 6481677041609121060ULL,
        2349743786833090768ULL, 18104134040501063890ULL,
        12418098900250596130ULL, 11943125234793006431ULL,
        3247335176665906184ULL, 13026568145145368459ULL,
        6618342709422175591ULL, 5634059337471730331ULL,
        4424645420833918853ULL, 6027124122124544597ULL,
        4203033546356070261ULL, 13107942912338467591ULL,
        12648291207145827102ULL, 3508103966277869999ULL,
        15212048322198628810ULL, 5054690685150340885ULL,
        9078069814861533778ULL, 7268955131569896348ULL, 840926743599025911ULL,
        4432369079365529163ULL, 2604847491822545835ULL,
        4992344983518012776ULL, 6979694563567112147ULL,
        12350053474172614355ULL, 13593751319817953700ULL,
        2709450594403108848ULL, 12297464222957979299ULL,
        14951411698291919992ULL, 13409326892749259647ULL,
        6247631032048430381ULL, 14745713749908202021ULL,
        17900626603342481000ULL, 7633774472227710432ULL,
        6332889082031156775ULL, 1173544426167561136ULL,
        7127197135751200791ULL, 3794630985178253428ULL,
        8599483122919723000ULL, 16529691743829043616ULL,
        5243137989649212452ULL, 17713889946904227919ULL,
        8760087386498306772ULL, 14973426116958656677ULL,
        2108043499608607810ULL, 18396050504236558870ULL,
        9335037586297390427ULL, 6235848529150406852ULL,
        10897850621516938893ULL, 49535068102055637ULL, 9958543313702810098ULL,
        10880259203789114112ULL, 6539122160013078153ULL,
        5453328419590745670ULL, 9307248640611611579ULL,
        1641694407205931455ULL, 3100462164169339928ULL,
        15421806046621203078ULL, 812223836509910384ULL, 357458483694773576ULL,
        7615016984522068970ULL, 7084574473108747696ULL,
        4724923210066720250ULL, 7502639287647505111ULL,
        1524978981301806653ULL, 6184012953220301469ULL,
        9896697251420471129ULL, 16960638341731847271ULL,
        3932528181641558045ULL, 9717379221466058427ULL, 335656264017715152ULL,
        7171881306146597741ULL, 13149785357157856843ULL,
        1666754878522689601ULL, 3391935601484229494ULL,
        3909999977977034538ULL, 17944655166631385244ULL,
        12349555834074568297ULL, 13278892425203203337ULL,
        1141724664896345746ULL, 15641611448191158297ULL,
        12277216884079854725ULL, 2668994936448063489ULL,
        5384194566366042042ULL, 12171582474523146557ULL, 89ULL};
    rng_validation<vsmc::ARS_4x64>(ARS_4x64Result, "ARS_4x64");

    std::vector<unsigned long long> ARS_8x64Result = {13300393051842062417ULL,
        15829204577734186096ULL, 12916216784412139746ULL,
        10866598331985900489ULL, 7592936610729180561ULL,
        15768484264076900209ULL, 2451053756790455507ULL,
        14203632501433974751ULL, 7615707596571806822ULL,
        110722280073870609ULL, 11305726846999933910ULL,
        13638869854020466058ULL, 1088282446920346665ULL,
        18410240571745966686ULL, 5792788262751919264ULL,
        1010285996142555713ULL, 2403953648682057869ULL,
        15185972729525601292ULL, 13860520892115414863ULL,
        901536029741996840ULL, 17117785360804756231ULL,
        6464174742244371043ULL, 4280004043842527112ULL,
        2088265397512460874ULL, 7182595697777920175ULL,
        6464994680095996624ULL, 17896207072751247480ULL,
        3567139893124151245ULL, 8833581268536696494ULL,
        6885626824738093893ULL, 7776363138006381668ULL,
        17132806398312074606ULL, 330303100006942327ULL, 727255385129880494ULL,
        698962089755139063ULL, 17236036049938971496ULL,
        6817514279684916442ULL, 4497837016970305863ULL,
        14351561599905937973ULL, 16278013777331928866ULL,
        6865991217900574998ULL, 5203772416380945418ULL,
        10333703665666121718ULL, 1560005595811138962ULL,
        4642680921027930504ULL, 10443709023995512214ULL,
        12360009119144801772ULL, 5142791954665661727ULL,
        13979567036600091014ULL, 8062071104473279322ULL,
        5877064618375133598ULL, 8556376656760505406ULL,
        1336876523508461138ULL, 26680799503052978ULL, 15685939740977860043ULL,
        14592584072821547845ULL, 4639226010252054570ULL,
        9387720824568086132ULL, 10837802302105023695ULL,
        14996088671094951006ULL, 11401652616307170567ULL,
        12796430960276183321ULL, 14446290016465191198ULL,
        14140701370875874112ULL, 13928191165495452466ULL,
        10255337405633900693ULL, 12701567664692689488ULL,
        10254602777512803294ULL, 15797806001575304619ULL,
        15049186002604815605ULL, 17141528278208670438ULL,
        8589121530148333358ULL, 5236447510202965414ULL, 698088159358904064ULL,
        5294980228499962103ULL, 17012162214127664000ULL,
        10343872407585139794ULL, 10797568011773812382ULL,
        4569827555929520353ULL, 2411912216201111127ULL,
        16338021611287700688ULL, 1517593821667190459ULL,
        4276597613116677857ULL, 2255295170564884137ULL,
        16931540802316922320ULL, 9535475709301632858ULL,
        2718594747251999768ULL, 11733385169154196241ULL,
        754663789768916262ULL, 1260872009098050219ULL, 7232021358799435554ULL,
        14370478072141893680ULL, 6175690913731886028ULL,
        16318222017731305895ULL, 7931817192016995312ULL,
        5764749839677016152ULL, 17860865663168359789ULL,
        14522552708558412038ULL, 9837632710000709317ULL,
        9003194998672112778ULL, 33ULL};
    rng_validation<vsmc::ARS_8x64>(ARS_8x64Result, "ARS_8x64");

    return 0;
}