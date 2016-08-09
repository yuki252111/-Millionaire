#ifndef __UDEFINE_H__
#define __UDEFINE_H__
//常量
#define ONE_SECOND 1.0f
#define WINDOW_LENGTH 800
#define WINDOW_HEIGHT 480
#define WINDOW_NAME "Zillionare"
#define FPS 1.0f/60
#define MAP_COUNT  3
//音效
#define BG_MUSIC "background_music"
#define EFFECT_MUSIC "effect_music"
//用户相关
#define PLAYER_1_TAG 1
#define PLAYER_2_TAG 2
#define PLAYER_1_NAME "player1"
#define PLAYER_2_NAME "player2"
#define PLAYER1_ANIM_PLIST "player1_anim.plist"
#define PLAYER1_ANIM_PNG "player1_anim.png"
#define PLAYER2_ANIM_PLIST "player2_anim.plist"
#define PLAYER2_ANIM_PNG "player2_anim.png"
//地图
#define PATH_MARK_1 "Mark1.png"
#define PATH_MARK_2 "Mark2.png"
#define PATH_MARK_3 "Mark3.png"
#define PATH_MARK_4 "Mark4.png"
#define PATH_MARK_5 "Mark5.png"
#define PATH_MARK_6 "Mark6.png"
//色子
#define DICE_ANIM_PLIST "dice.plist"
#define DICE_ANIM_PNG "dice.png"
#define ROUND_PLIST "digital_round.plist"
#define ROUND_PNG "digital_round.png"
//对话框
#define  DIALOG_BG "dialog_bg.png"
#define  BUTTON_BG1 "button_bg1.png"
#define  BUTTON_BG2 "button_bg2.png"
#define  BUTTON_BG3 "button_bg3.png"
#define DIALOG_TITLE  "Alert Message"
#define DIALOG_CONTENT "Do you want to quit ?"
//字符串
#define OK "Ok"
#define CANCEL "Cancel"
#define MSG_GO "msg_go"
#define MSG_BUY "msg_buy"
#define MSG_MONEY "msg_money"
#define MSG_STRENGTH "msg_strength"
#define MSG_PAY_TOLLS "msg_pay_tolls"
#define MSG_CONTROLLER "msg_controller"
#define  BUY_LAND_MSG "Do you want to buy the land ? need $  ";
#define MSG_RANDOM_ASK_EVENT "random_ask_event"
#define MSG_GAME_OVER "game_over"
//--------------------------------------------------------------------------------------------
//随机事件
#define  TAX_REBATE  "The government encourages investment, tax rebates $5000"
#define  TAX_PAY "Government scrutiny of accounts, pay taxes $5000"
#define  STRENGTH_LOSS "To drink alcohol, diarrhea, loss strength Y 40"
#define  STRENGTH_GET "Eat rice and recover strength Y 60"
#define  INVESTMENT_PROFIT "Investment profits, get $10000"
#define  INVESTMENT_LOSS "Investment failure, loss $10000"

#define STRENGTH_UP_30 "Strength up +30"
#define STRENGTH_UP_50 "Strength up +50"
#define STRENGTH_UP_80 "Strength up +80"
//-----------------------------------------------------------------------------------------------------
#define P1_DEYIDEYITIAN  "sound/byelottery.wav"//拜拜 祝您中奖
#define P1_need1000  "sound/need1000.wav"//只要1000元
#define P1_select_lottery  "sound/select_lottery.wav"//请圈选你要购买的彩票
#define P1_meirendejiang "sound/meirendejiang.wav"//sorry 本月份没有人得奖
#define P1_xiwangshini  "sound/xiwangshini.wav"//希望下次得奖者就是您
#define P1_Speaking_00181  "sound/Speaking_00181.wav"//投资获利
#define P1_Speaking_00182  "sound/Speaking_00182.wav"//投资失败
#define PARTICLE_EFFECT "sound/particle.mp3"
#define P1_STOCK "sound/stock.wav"
#define P1_tilihuisheng "sound/youismine.wav"

#define STORM_SPEAKING  "sound/storm_speaking.wav" //龙卷风侵袭 摧毁房屋一栋
#define LOTTERY_PUBLISH_WAV  "sound/lottery_publish.wav"
#define LOTTERY_NUM "sound/lottery_num.wav" //请圈选您的幸运号码
#define MASHANGKAIJIANG  "sound/mashangkaijiang.wav" //现在马上开出这期的号码
//交过路费声音
#define P1_Speaking_00435  "sound/Speaking_00435.wav"//oh 哈利路亚
#define P1_Speaking_00461  "sound/Speaking_00461.wav"//oh 我的血汗钱
#define P1_Speaking_00475  "sound/Speaking_00475.wav"//算了算了 老子有的是钱
#define P1_Speaking_01060  "sound/Speaking_01060.wav"//老本都快没了
#define P1_Speaking_001062  "sound/Speaking_001062.wav"//拿去了不用找了
//抢夺别人地块
#define P1_Speaking_00429  "sound/Speaking_00429.wav"//让我把他据为己有
//房屋被抢夺
#define P1_Speaking_00430  "sound/Speaking_00430.wav"//黄金地段 让给你
#define P1_Speaking_00464  "sound/Speaking_00464.wav"//太不给面子了
#define P1_Speaking_00469  "sound/Speaking_00469.wav"//你皮子痒啊
#define P1_Speaking_00470  "sound/Speaking_00470.wav"//竟敢在太岁头上动土
#define P1_Speaking_00476  "sound/Speaking_00476.wav"//算你狠
//房屋被摧毁
#define P1_Speaking_00462  "sound/Speaking_00462.wav"//好大的胆子
#define P1_Speaking_00463  "sound/Speaking_00463.wav"//谁敢动我的地
#define P1_Speaking_00466  "sound/Speaking_00466.wav"//竟敢破坏我的好事
#define P1_Speaking_00468  "sound/Speaking_00468.wav"//拆的还真干净
#define P1_Speaking_00474  "sound/Speaking_00474.wav"//你有没有搞错啊
#define P1_Speaking_001061  "sound/Speaking_001061.wav"//真没良心
//摧毁别人房屋
#define P1_Speaking_00433  "sound/Speaking_00433.wav"//不必谢我
#define P1_Speaking_00437  "sound/Speaking_00437.wav"//全部夷为平地
//掉进陷阱
#define P1_Speaking_00449  "sound/Speaking_00449.wav"//快来帮我把
#define P1_Speaking_01054  "sound/Speaking_01054.wav"//我惨了
#define P1_Speaking_01055  "sound/Speaking_01055.wav"//哎呦喂啊
#define P1_Speaking_001071  "sound/Speaking_001071.wav"//我不要打针
//看到别人住院
#define P1_Speaking_001073  "sound/Speaking_001073.wav"//别闹了
//收取过路费
#define P1_Speaking_00453  "sound/Speaking_00453.wav"//小本经营 概不赊欠
#define P1_Speaking_01059  "sound/Speaking_01059.wav"//蝇头小利
#define P1_Speaking_01057  "sound/Speaking_01057.wav"//这是我应得的
//升级房子
#define P1_Speaking_01051  "sound/Speaking_01051.wav"//别嫉妒我
//不交过路费
#define P1_Speaking_00446  "sound/Speaking_00446.wav"//有钱也不给你
#define P1_Speaking_00477  "sound/Speaking_00477.wav"//可别想占我便宜啊
//买地
#define P1_Speaking_00458  "sound/Speaking_00458.wav"//盖什么好呢

#define P1_Speaking_01052  "sound/Speaking_01052.wav"//鸿运当头
#define P1_Speaking_001063  "sound/Speaking_001063.wav"//上帝保佑
//对方被罚收税
#define P1_Speaking_00452  "sound/Speaking_00452.wav"//别想偷漏税


//交过路费声音
#define P2_SPEAKING01 "sound/p2_Speaking01.wav" //违章建筑怎么没人管哪
#define P2_QISIWOLE     "sound/p2_qisiwole.wav" //气死我了
#define P2_XINHAOKONGA  "sound/p2_xinhaokonga.wav" //我心好痛啊
#define P2_BUHUIBA  "sound/p2_buhuiba.wav" //不会吧
#define P2_PAYHIGH  "sound/p2_payhigh.wav" //不觉得这条路的收费太贵了吗
#define P2_QIANGQIANA  "sound/p2_qiangqiana.wav" //抢钱啊
#define P2_HEBAOCHUXIE  "sound/p2_hebaochuxie.wav" //荷包大出血
//抢夺别人地块
#define P2_BIEGUAIWO  "sound/p2_bieguaiwo.wav" //别怪我
#define P2_SPEAKING02 "sound/p2_Speaking02.wav" //想不到有这一招吧
#define P2_TIGER  "sound/p2_tiger.wav" // 老虎不发威 把我当病猫
#define P2_NIDEJIUSHODE "sound/p2_nidejiushode.wav" //你的就是我的 我的还是我的
//房屋被抢夺
#define P2_ZHENMIANMU  "sound/p2_zhenmianmu.wav" //终于认清你的真面目了
#define P2_WODEDIQI  "sound/p2_wodediqi.wav" //谁偷了我的地契
#define P2_HAOQIFU "sound/p2_haoqifu.wav" //别以为老娘好欺负
#define P2_WANGFA "sound/p2_wangfa.wav" // 这还有王法吗
//摧毁别人房屋
#define P2_NIGAIWOCHAI  "sound/p2_nigaiwochai.wav" //你盖我就拆
#define P2_KANWODE "sound/p2_kanwode.wav" //看我的
#define P2_HAIRENLE "sound/p2_hairenle.wav" //我又要害人了
#define P2_BAOCHOU "sound/p2_baochou.wav" //报仇的时候到了
//房屋被摧毁
#define P2_WODEYANGFANG  "sound/p2_wodeyangfang.wav" //我的洋房
#define P2_QIFURENJIA  "sound/p2_qifurenjia.wav" //欺负人家
#define P2_SHAQIANDAO  "sound/p2_shaqiandao.wav" //你这个杀千刀的
#define P2_LIANXIANGXIYU  "sound/p2_lianxiangxiyu.wav" //你不会怜香惜玉啊
#define P2_HAOJIUGAIHAO "p2_haojiugaihao.wav" //人家花了好久才盖好的
//掉进陷阱
#define P2_YUNQICHA "sound/p2_yunqicha.wav" //运气太差了
#define P2_HAIRENJING "sound/p2_hairenjing.wav" //害人精
#define P2_XIAOHUANG "sound/p2_xiaohuang.wav" //小黄快来帮我
#define P2_YISHENG "sound/p2_yisheng.wav" //医生在哪里
//看到别人住院
#define P2_DUOXIUXI "sound/p2_duoxiuxi.wav" //多休息一会儿吧
#define P2_XIUXIJITIAN "sound/p2_xiuxijitian.wav" //休息几天吧

//收取过路费
#define P2_RENBUWEIJI  "sound/p2_renbuweiji.wav" //人不为己天诛地灭
#define P2_XIAOQI  "sound/p2_xiaoqi.wav" //小气吧啦
#define P2_RONGXING "sound/p2_rongxing.wav" //这是你们的荣幸
#define P2_MANYI "sound/p2_manyi.wav" //恩 我很满意
#define P2_XIAOFUPO "sound/p2_xiaofupo.wav" //我是小富婆
#define P2_DUOGEI "sound/p2_duogei.wav" //怎么不多给一点啊
//升级房子
#define P2_HIGHER       "sound/p2_higher.wav" //盖的越高越好
#define P2_WANZHANGGAOLOU "sound/p2_wanzhanggaolou.wav" //万丈高楼平地起
//不交过路费
#define P2_NOTPAY       "sound/p2_notpay.wav"      //不付也是应该的
#define P2_YIMAOBUGEI  "sound/p2_yimaobugei.wav" //一毛都不给你
//买地
#define P2_BUYIT        "sound/p2_buyit.wav" //买了它
#define P2_HAODEKAISHI "sound/p2_haodekaishi.wav" //好的开始是成功的一半
#define P2_RANGNIZHU "sound/p2_rangnizhu.wav" //我的漂亮房子让你住
#define P2_MAIWOBA  "sound/p2_maiwoba.wav" //这块地卖我吧

#define P2_DEYIDEYITIAN "sound/p2_deyideyitian.wav"//得意的一天
#define P2_HENGCAI "sound/p2_hengcai.wav " //人无横财不富
//对方被罚收税
#define P2_TOUSHUI  "sound/p2_toushui.wav" //看你一副偷漏税的样子
#define P2_FALVZHICAI "sound/p2_falvzhicai.wav" //接受法律的制裁吧
#define P2_GUOKU "sound/p2_guoku.wav" //交钱给国库吧
#define P2_NASHUI "sound/p2_nashui.wav" //纳税是国民应尽的义务

//背景音乐
#define BG01_MP3 "sound/bg01.mp3" 
#define BG02_MP3 "sound/bg02.mp3" 
#define BG03_MP3 "sound/bg03.mp3" 
//-------------------------------------------------------------------
const int PLAYER1_POS = 23;
const int PLAYER2_POS = 23;
//-----------------------------------------------------------------
const int MSG_GAME_OVER_TAG = 0;
//-----------------------------------------------------------------
//传递消息信号的变量
const int MSG_GO_HIDE_TAG = 1;
const int MSG_GO_SHOW_TAG = 2;
const int MSG_GO_SHOW_MESSAGE_TAG = 3;

const int MSG_GO_TAG = 4;
const int MSG_GO_CON_TAG = 5;
const int MSG_GO_PASS_TAG = 6;
const int MSG_GO_END_TAG = 7;
const int MSG_GO_TRAP_TAG = 8;
const int MSG_GO_REST_TAG = 9;

const int MSG_CONTROLLER_PASS_TAG =10;
const int MSG_CONTROLLER_END_TAG = 11;
const int MSG_CONTROLLER_AROUND_TAG = 12;
//-----------------------------------------
const int MSG_BUY_LAND_TAG = 13;
const int MSG_BUY_BLANK_TAG = 14;
const int MSG_BUY_LAND_1_TAG = 15;
const int MSG_BUY_LAND_2_TAG = 16;

const int MSG_BUY_BLANK_DIALOG = 17;
const int MSG_BUY_LAND_1_DIALOG = 18;
const int MSG_BUY_LAND_2_DIALOG = 19;

const int MSG_BUY_LOTTERY_TAG = 20;
const int MSG_BUY_LOTTERY_NUMBER_TAG = 21;
const int MSG_BUY_LOTTERY_PUBLISH_TAG = 22;
const int MSG_BUY_LOTTERY_PUBLISH_NUMBER_TAG = 56;
const int MSG_BUY_LOTTERY_WIN_TAG = 23;

const int MSG_BUY_STOCK_TAG = 24;
const int MSG_BUY_STOCK_DISMISS_TAG = 25;
const int MSG_BUY_STOCK_CON_TAG = 26;
const int MSG_BUY_PLAYER1_STOCK_TAG = 27;
const int MSG_BUY_PLAYER2_STOCK_TAG = 28;

const int MSG_BUY_SHOP_ITEM_TAG = 29;
const int MSG_BUY_SHOP_ITEM_NUMBER_TAG = 30;
const int MSG_PACKAGE_TAG = 31;
const int MSG_PACKAGE_USE_TAG = 32;
const int MSG_BUY_NOTHING_TAG = 33;
const int MSG_BUY_GOD_TAG = 34;

const int MSG_PAY_TOLLS_1_DIALOG = 35;
const int MSG_PAY_TOLLS_2_DIALOG = 36;
const int MSG_PAY_TOLLS_3_DIALOG = 37;

const int MSG_PAY_TOLLS_1_TAG = 38;
const int MSG_PAY_TOLLS_2_TAG = 39;
const int MSG_PAY_TOLLS_3_TAG = 40;

const int MSG_MONEY_PLAYER1_TAG = 41;
const int MSG_MONEY_PLAYER2_TAG = 42;

const int MSG_STRENGTH_PLAYER1_TAG = 43;
const int MSG_STRENGTH_PLAYER2_TAG = 44;

const int MSG_STRENGTH_UP30_TAG = 45;
const int MSG_STRENGTH_UP50_TAG = 46;
const int MSG_STRENGTH_UP80_TAG = 47;
//-----------------------
const int TAX_REBATE_TAG = 48;
const int TAX_PAY_TAG = 49;

const int STRENGTH_LOSS_TAG = 50;
const int STRENGTH_GET_TAG = 51;

const int INVESTMENT_PROFIT_TAG = 52;
const int INVESTMENT_LOSS_TAG = 53;

const int MSG_RANDOM_ASK_EVENT_TAG = 54;
const int MSG_RANDOM_DO_ASK_EVENT_TAG = 55;
//--------------------------------------------------
//游戏数据
const int LAND_BLANK_MONEY = 1000;
const int LAND_LEVEL_1_MONEY = 2000;
const int LAND_LEVEL_2_MONEY = 3000;

const int BUY_LOTTERY_MONEY = 3000;
const int LOTTERY_WIN_MONEY = 20000;
const int LOTTERY_TIME_INTERVAL = 7;//
const int BUY_SHOP_ITEM_MONEY = 1000;
const int TRAP_DAY = 1;


const int EACH_STRENGTH = 20;
const int STRENGTH_LEVEL_1 = 30;
const int STRENGTH_LEVEL_2 = 50;
const int STRENGTH_LEVEL_3 = 80;


const int stockCellWidth = 100;
const int stockCellHeight = 30;
const int buy_button = 801;
const int sell_button = 802;
const int back_button = 803;
//--------------------------------------------------------------------------------------------
#define HOST "127.0.0.1"
#define PORT 7788
#define MAX_LEN 512

#endif