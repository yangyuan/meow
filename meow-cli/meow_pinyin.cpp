#include "meow.h"
#include "meow_pinyin.h"


static const UINT BLOB_PINYIN_FULL_LEN = 412;
static const WCHAR BLOB_PINYIN_FULL[412][7] = { L"a", L"ai", L"an", L"ang", L"ao", L"ba", L"bai", L"ban", L"bang", L"bao", L"bei", L"ben", L"beng", L"bi", L"bian", L"biao", L"bie", L"bin", L"bing", L"bo", L"bu", L"ca", L"cai", L"can", L"cang", L"cao", L"ce", L"cen", L"ceng", L"cha", L"chai", L"chan", L"chang", L"chao", L"che", L"chen", L"cheng", L"chi", L"chong", L"chou", L"chu", L"chua", L"chuai", L"chuan", L"chuang", L"chui", L"chun", L"chuo", L"ci", L"cong", L"cou", L"cu", L"cuan", L"cui", L"cun", L"cuo", L"da", L"dai", L"dan", L"dang", L"dao", L"de", L"den", L"dei", L"deng", L"di", L"dia", L"dian", L"diao", L"die", L"ding", L"diu", L"dong", L"dou", L"du", L"duan", L"dui", L"dun", L"duo", L"e", L"ei", L"en", L"eng", L"er", L"fa", L"fan", L"fang", L"fei", L"fen", L"feng", L"fiao", L"fo", L"fou", L"fu", L"ga", L"gai", L"gan", L"gang", L"gao", L"ge", L"gei", L"gen", L"geng", L"gong", L"gou", L"gu", L"gua", L"guai", L"guan", L"guang", L"gui", L"gun", L"guo", L"ha", L"hai", L"han", L"hang", L"hao", L"he", L"hei", L"hen", L"heng", L"hong", L"hou", L"hu", L"hua", L"huai", L"huan", L"huang", L"hui", L"hun", L"huo", L"ji", L"jia", L"jian", L"jiang", L"jiao", L"jie", L"jin", L"jing", L"jiong", L"jiu", L"ju", L"juan", L"jue", L"jun", L"ka", L"kai", L"kan", L"kang", L"kao", L"ke", L"kei", L"ken", L"keng", L"kong", L"kou", L"ku", L"kua", L"kuai", L"kuan", L"kuang", L"kui", L"kun", L"kuo", L"la", L"lai", L"lan", L"lang", L"lao", L"le", L"lei", L"leng", L"li", L"lia", L"lian", L"liang", L"liao", L"lie", L"lin", L"ling", L"liu", L"lo", L"long", L"lou", L"lu", L"lv", L"luan", L"lue", L"lve", L"lun", L"luo", L"ma", L"mai", L"man", L"mang", L"mao", L"me", L"mei", L"men", L"meng", L"mi", L"mian", L"miao", L"mie", L"min", L"ming", L"miu", L"mo", L"mou", L"mu", L"na", L"nai", L"nan", L"nang", L"nao", L"ne", L"nei", L"nen", L"neng", L"ni", L"nian", L"niang", L"niao", L"nie", L"nin", L"ning", L"niu", L"nong", L"nou", L"nu", L"nv", L"nuan", L"nve", L"nuo", L"nun", L"o", L"ou", L"pa", L"pai", L"pan", L"pang", L"pao", L"pei", L"pen", L"peng", L"pi", L"pian", L"piao", L"pie", L"pin", L"ping", L"po", L"pou", L"pu", L"qi", L"qia", L"qian", L"qiang", L"qiao", L"qie", L"qin", L"qing", L"qiong", L"qiu", L"qu", L"quan", L"que", L"qun", L"ran", L"rang", L"rao", L"re", L"ren", L"reng", L"ri", L"rong", L"rou", L"ru", L"ruan", L"rui", L"run", L"ruo", L"sa", L"sai", L"san", L"sang", L"sao", L"se", L"sen", L"seng", L"sha", L"shai", L"shan", L"shang", L"shao", L"she", L"shei", L"shen", L"sheng", L"shi", L"shou", L"shu", L"shua", L"shuai", L"shuan", L"shuang", L"shui", L"shun", L"shuo", L"si", L"song", L"sou", L"su", L"suan", L"sui", L"sun", L"suo", L"ta", L"tai", L"tan", L"tang", L"tao", L"te", L"teng", L"ti", L"tian", L"tiao", L"tie", L"ting", L"tong", L"tou", L"tu", L"tuan", L"tui", L"tun", L"tuo", L"wa", L"wai", L"wan", L"wang", L"wei", L"wen", L"weng", L"wo", L"wu", L"xi", L"xia", L"xian", L"xiang", L"xiao", L"xie", L"xin", L"xing", L"xiong", L"xiu", L"xu", L"xuan", L"xue", L"xun", L"ya", L"yan", L"yang", L"yao", L"ye", L"yi", L"yin", L"ying", L"yo", L"yong", L"you", L"yu", L"yuan", L"yue", L"yun", L"za", L"zai", L"zan", L"zang", L"zao", L"ze", L"zei", L"zen", L"zeng", L"zha", L"zhai", L"zhan", L"zhang", L"zhao", L"zhe", L"zhei", L"zhen", L"zheng", L"zhi", L"zhong", L"zhou", L"zhu", L"zhua", L"zhuai", L"zhuan", L"zhuang", L"zhui", L"zhun", L"zhuo", L"zi", L"zong", L"zou", L"zu", L"zuan", L"zui", L"zun", L"zuo" };
static const UINT  BLOB_PINYIN_ALL_LEN = 436;
static const WCHAR BLOB_PINYIN_ALL[436][7] = { L"a", L"ai", L"an", L"ang", L"ao", L"ba", L"bai", L"ban", L"bang", L"bao", L"bei", L"ben", L"beng", L"bi", L"bian", L"biao", L"bie", L"bin", L"bing", L"bo", L"bu", L"ca", L"cai", L"can", L"cang", L"cao", L"ce", L"cen", L"ceng", L"cha", L"chai", L"chan", L"chang", L"chao", L"che", L"chen", L"cheng", L"chi", L"chong", L"chou", L"chu", L"chua", L"chuai", L"chuan", L"chuang", L"chui", L"chun", L"chuo", L"ci", L"cong", L"cou", L"cu", L"cuan", L"cui", L"cun", L"cuo", L"da", L"dai", L"dan", L"dang", L"dao", L"de", L"den", L"dei", L"deng", L"di", L"dia", L"dian", L"diao", L"die", L"ding", L"diu", L"dong", L"dou", L"du", L"duan", L"dui", L"dun", L"duo", L"e", L"ei", L"en", L"eng", L"er", L"fa", L"fan", L"fang", L"fei", L"fen", L"feng", L"fiao", L"fo", L"fou", L"fu", L"ga", L"gai", L"gan", L"gang", L"gao", L"ge", L"gei", L"gen", L"geng", L"gong", L"gou", L"gu", L"gua", L"guai", L"guan", L"guang", L"gui", L"gun", L"guo", L"ha", L"hai", L"han", L"hang", L"hao", L"he", L"hei", L"hen", L"heng", L"hong", L"hou", L"hu", L"hua", L"huai", L"huan", L"huang", L"hui", L"hun", L"huo", L"ji", L"jia", L"jian", L"jiang", L"jiao", L"jie", L"jin", L"jing", L"jiong", L"jiu", L"ju", L"juan", L"jue", L"jun", L"ka", L"kai", L"kan", L"kang", L"kao", L"ke", L"kei", L"ken", L"keng", L"kong", L"kou", L"ku", L"kua", L"kuai", L"kuan", L"kuang", L"kui", L"kun", L"kuo", L"la", L"lai", L"lan", L"lang", L"lao", L"le", L"lei", L"leng", L"li", L"lia", L"lian", L"liang", L"liao", L"lie", L"lin", L"ling", L"liu", L"lo", L"long", L"lou", L"lu", L"lv", L"luan", L"lue", L"lve", L"lun", L"luo", L"ma", L"mai", L"man", L"mang", L"mao", L"me", L"mei", L"men", L"meng", L"mi", L"mian", L"miao", L"mie", L"min", L"ming", L"miu", L"mo", L"mou", L"mu", L"na", L"nai", L"nan", L"nang", L"nao", L"ne", L"nei", L"nen", L"neng", L"ni", L"nian", L"niang", L"niao", L"nie", L"nin", L"ning", L"niu", L"nong", L"nou", L"nu", L"nv", L"nuan", L"nve", L"nuo", L"nun", L"o", L"ou", L"pa", L"pai", L"pan", L"pang", L"pao", L"pei", L"pen", L"peng", L"pi", L"pian", L"piao", L"pie", L"pin", L"ping", L"po", L"pou", L"pu", L"qi", L"qia", L"qian", L"qiang", L"qiao", L"qie", L"qin", L"qing", L"qiong", L"qiu", L"qu", L"quan", L"que", L"qun", L"ran", L"rang", L"rao", L"re", L"ren", L"reng", L"ri", L"rong", L"rou", L"ru", L"ruan", L"rui", L"run", L"ruo", L"sa", L"sai", L"san", L"sang", L"sao", L"se", L"sen", L"seng", L"sha", L"shai", L"shan", L"shang", L"shao", L"she", L"shei", L"shen", L"sheng", L"shi", L"shou", L"shu", L"shua", L"shuai", L"shuan", L"shuang", L"shui", L"shun", L"shuo", L"si", L"song", L"sou", L"su", L"suan", L"sui", L"sun", L"suo", L"ta", L"tai", L"tan", L"tang", L"tao", L"te", L"teng", L"ti", L"tian", L"tiao", L"tie", L"ting", L"tong", L"tou", L"tu", L"tuan", L"tui", L"tun", L"tuo", L"wa", L"wai", L"wan", L"wang", L"wei", L"wen", L"weng", L"wo", L"wu", L"xi", L"xia", L"xian", L"xiang", L"xiao", L"xie", L"xin", L"xing", L"xiong", L"xiu", L"xu", L"xuan", L"xue", L"xun", L"ya", L"yan", L"yang", L"yao", L"ye", L"yi", L"yin", L"ying", L"yo", L"yong", L"you", L"yu", L"yuan", L"yue", L"yun", L"za", L"zai", L"zan", L"zang", L"zao", L"ze", L"zei", L"zen", L"zeng", L"zha", L"zhai", L"zhan", L"zhang", L"zhao", L"zhe", L"zhei", L"zhen", L"zheng", L"zhi", L"zhong", L"zhou", L"zhu", L"zhua", L"zhuai", L"zhuan", L"zhuang", L"zhui", L"zhun", L"zhuo", L"zi", L"zong", L"zou", L"zu", L"zuan", L"zui", L"zun", L"zuo", L"b", L"c", L"d", L"f", L"g", L"h", L"j", L"k", L"l", L"m", L"n", L"p", L"q", L"r", L"s", L"t", L"w", L"x", L"y", L"z", L"ch", L"sh", L"zh", L"ng" };
static const WORD  BLOB_PINYIN_ALL_SHORTCUT[436][2] = { { 0xFFFF, 0xFFFF }, { 0, 0xFFFF }, { 0, 0xFFFF }, { 0, 0xFFFF }, { 0, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 412, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 432 }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 413, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 414, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 79, 0xFFFF }, { 79, 0xFFFF }, { 79, 0xFFFF }, { 79, 0xFFFF }, { 415, 0xFFFF }, { 415, 0xFFFF }, { 415, 0xFFFF }, { 415, 0xFFFF }, { 415, 0xFFFF }, { 415, 0xFFFF }, { 415, 0xFFFF }, { 415, 0xFFFF }, { 415, 0xFFFF }, { 415, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 416, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 417, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 418, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 419, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 420, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 421, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 422, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 236, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 423, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 424, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 425, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 433 }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 426, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 427, 0xFFFF }, { 428, 0xFFFF }, { 428, 0xFFFF }, { 428, 0xFFFF }, { 428, 0xFFFF }, { 428, 0xFFFF }, { 428, 0xFFFF }, { 428, 0xFFFF }, { 428, 0xFFFF }, { 428, 0xFFFF }, { 428, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 429, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 430, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 434 }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 431, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF }, { 0xFFFF, 0xFFFF } };
static const UINT BLOB_PINYIN_SINGLE_LEN = 23;
static const WCHAR BLOB_PINYIN_SINGLE[23] = { L'a', L'b', L'c', L'd', L'e', L'f', L'g', L'h', L'j', L'k', L'l', L'm', L'n', L'o', L'p', L'q', L'r', L's', L't', L'w', L'x', L'y', L'z' };
static const UINT BLOB_PINYIN_SPLIA_LEN = 16;
static const WCHAR BLOB_PINYIN_SPLIA[16] = { L'b', L'c', L'd', L'f', L'j', L'k', L'l', L'm', L'p', L'q', L's', L't', L'w', L'x', L'y', L'z' };
static const UINT BLOB_PINYIN_SPLIB_LEN = 3; // g h r
static const WCHAR BLOB_PINYIN_SPLIB[3][5] = { L"gn", L"hzcs", L"re" };
static const UINT BLOB_PINYIN_SPLIC_LEN = 4; // a e o n
static const WCHAR * BLOB_PINYIN_SPLIC[4] = { L"aaeo", L"eaejoqx", L"oejoqvx", L"nbcdfghjklmnpqrstvwxyz" };
static const UINT BLOB_PINYIN_SPLID_LEN = 34; // i u v
static const WCHAR BLOB_PINYIN_SPLID[34][4] = { L"ib", L"ihc", L"ic", L"id", L"ij", L"il", L"im", L"in", L"ip", L"iq", L"ihs", L"is", L"it", L"ix", L"iy", L"ihz", L"ub", L"uc", L"ud", L"uf", L"ug", L"uj", L"uk", L"ul", L"un", L"uq", L"ur", L"us", L"ut", L"uw", L"uy", L"uz", L"vl", L"vn" };

MeowPinyin::MeowPinyin()
{
	LoadDictionary();
}

MeowPinyin::~MeowPinyin()
{
}



unsigned int MeowPinyin::Solute(WCHAR * _input) {
	if (_input == NULL) return 0;
	unsigned int len = wcslen(_input);
	if (len < MEOW_MAXLENGTH) {
		wcsncpy_s(input, _input, len);
		inputlength = len;
	}
	else {
		wcsncpy_s(input, _input, MEOW_MAXLENGTH - 1);
		inputlength = MEOW_MAXLENGTH - 1;
		input[MEOW_MAXLENGTH - 1] = L'\0';
	}

	segments.clear();
	solution.clear();
	results.clear();

	DoSplitIntoSegments();
	ParseSegments();
	GenerateResult();
	return 1;
}

inline bool SegmentSolutionCompare(const PINYIN_SEGMENT_SOLUTION& s1, const PINYIN_SEGMENT_SOLUTION& s2)
{
	return s1.length < s2.length;
}

inline bool SegmentSolutionCompare2(const PINYIN_SEGMENT_SOLUTION& s1, const PINYIN_SEGMENT_SOLUTION& s2)
{
	return s1.rest_length < s2.rest_length;
}

bool MeowPinyin::RecursiveQuanpinResult(unsigned int index, unsigned int offset_pinyin, PINYIN_RESULT pr) {
	if (index >= segments.size()) {
		results.push_back(pr);
		return true;
	}
	PINYIN_SEGMENT * segment = &segments[index];
	size_t length = segment->solutions_quanpin.size();
	std::sort(segment->solutions_quanpin.begin(), segment->solutions_quanpin.end(), SegmentSolutionCompare);

	// foreah solutions_quanpin
	for (unsigned int i = 0; i < length; i++) {
		for (unsigned int j = 0; j < segment->solutions_quanpin[i].length; j++) {
			pr.pinyin.push_back(segment->solutions_quanpin[i].result[j]);
		}
		RecursiveQuanpinResult(index + 1, offset_pinyin, pr);
		for (unsigned int j = 0; j < segment->solutions_quanpin[i].length; j++) {
			pr.pinyin.pop_back();
		}
	}
	return false;
}


UINT16 MeowPinyin::GetPinyinIndexAll(const WCHAR * input, std::vector<unsigned short> * output, unsigned int offset, unsigned int length) {
	unsigned int max = -1;
	unsigned short max_result = 0;
	std::vector<WORD> results;
	for (unsigned int i = 0; i < BLOB_PINYIN_FULL_LEN; i++) {
		unsigned int tmp = MatchSubToSample(input + offset, length, BLOB_PINYIN_FULL[i]);
		if (tmp > max) {
			max = tmp;
			max_result = i;
		}
		if (tmp >= length) {
			output->push_back(i);
		}
	}

	if (output->size() == 0) {
		output->push_back(max_result);
	}

	return output->size();
}

// almost same to RecursiveQuanpinResult, but end-with a not finished (guessing) quanping
bool MeowPinyin::RecursiveQuanpinPendingResult(unsigned int index, unsigned int offset_pinyin, PINYIN_RESULT pr) {

	PINYIN_SEGMENT * segment = &segments[index];

	if (index == segments.size() - 1) { // this is last segment
		size_t length = segment->solutions_quanpin_prefix.size();
		std::sort(segment->solutions_quanpin_prefix.begin(), segment->solutions_quanpin_prefix.end(), SegmentSolutionCompare);

		// foreah solutions_quanpin
		for (unsigned int i = 0; i < length; i++) {
			for (unsigned int j = 0; j < segment->solutions_quanpin_prefix[i].length; j++) { // all full pinyin result
				pr.pinyin.push_back(segment->solutions_quanpin_prefix[i].result[j]);
			}
			std::vector<unsigned short> last_results;
			unsigned int count = GetPinyinIndexAll(input, &last_results, segment->solutions_quanpin_prefix[i].rest_offset, segment->solutions_quanpin_prefix[i].rest_length);

			for (unsigned int k = 0; k < count; k++) {
				pr.pinyin.push_back(last_results[k]);
				results.push_back(pr);
				pr.pinyin.pop_back();
			}


			for (unsigned int j = 0; j < segment->solutions_quanpin_prefix[i].length; j++) { // all full pinyin result
				pr.pinyin.pop_back();
			}
		}
		return true;
	}
	else {
		size_t length = segment->solutions_quanpin.size();
		std::sort(segment->solutions_quanpin.begin(), segment->solutions_quanpin.end(), SegmentSolutionCompare);

		// foreah solutions_quanpin
		for (unsigned int i = 0; i < length; i++) {
			for (unsigned int j = 0; j < segment->solutions_quanpin[i].length; j++) {
				pr.pinyin.push_back(segment->solutions_quanpin[i].result[j]);
			}
			RecursiveQuanpinPendingResult(index + 1, offset_pinyin, pr);
			for (unsigned int j = 0; j < segment->solutions_quanpin[i].length; j++) {
				pr.pinyin.pop_back();
			}
		}
	}
	return false;
}


bool MeowPinyin::RecursiveQuanpinFirstResult(unsigned int index, unsigned int offset_pinyin, PINYIN_RESULT pr) {
	if (index >= segments.size()) {
		results.push_back(pr);
		return true;
	}
	PINYIN_SEGMENT * segment = &segments[index];
	size_t length = segment->solutions_quanpin.size();
	std::sort(segment->solutions_quanpin.begin(), segment->solutions_quanpin.end(), SegmentSolutionCompare);
	// foreah solutions_quanpin
	for (unsigned int i = 0; i < length; i++) {
		for (unsigned int j = 0; j < segment->solutions_quanpin[i].length; j++) {
			pr.pinyin.push_back(segment->solutions_quanpin[i].result[j]);
		}
		RecursiveQuanpinFirstResult(index + 1, offset_pinyin, pr);
		for (unsigned int j = 0; j < segment->solutions_quanpin[i].length; j++) {
			pr.pinyin.pop_back();
		}
	}
	if (length != 0) {
		return false;
	}
	length = segment->solutions_short.size();
	std::sort(segment->solutions_short.begin(), segment->solutions_short.end(), SegmentSolutionCompare);
	for (unsigned int i = 0; i < length; i++) {
		for (unsigned int j = 0; j < segment->solutions_short[i].length; j++) {
			pr.pinyin.push_back(segment->solutions_short[i].result[j]);
		}
		RecursiveQuanpinFirstResult(index + 1, offset_pinyin, pr);
		for (unsigned int j = 0; j < segment->solutions_short[i].length; j++) {
			pr.pinyin.pop_back();
		}
	}
	return false;
}

void MeowPinyin::GenerateResult() {
	PINYIN_RESULT pr;
	pr.length = 0;
	pr.power = 0;
	RecursiveQuanpinResult(0, 0, pr);

	if (results.size() == 0) {
		RecursiveQuanpinPendingResult(0, 0, pr);
	}

	if (results.size() == 0) {
		RecursiveQuanpinFirstResult(0, 0, pr);
	}
}





void MeowPinyin::DoSplitIntoSegments() {
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	unsigned int result[MEOW_MAXLENGTH] {0};

	bool added = false;
	result[0] = 1; // 0
	for (i = 1; i < inputlength; i++) { // start with 1
		added = false;
		for (j = 0; j < BLOB_PINYIN_SPLIA_LEN; j++) {
			if (input[i] == BLOB_PINYIN_SPLIA[j]) {
				result[i] = 1;
				added = true;
				break;
			}
		}
		if (added) continue;
		for (j = 0; j < BLOB_PINYIN_SPLIB_LEN; j++) {
			if (input[i] == BLOB_PINYIN_SPLIB[j][0]) {
				for (unsigned int offset = 1; offset < 5; offset++) {
					if (i - 1 < 0) break;
					if (BLOB_PINYIN_SPLIB[j][offset] == L'\0') {
						result[i] = 1;
						added = true;
						break;
					}
					if (BLOB_PINYIN_SPLIB[j][offset] == input[i - 1]) {
						break;
					}
				}
				break;
			}
		}
		if (added) continue;
		for (j = 0; j < BLOB_PINYIN_SPLIC_LEN; j++) {
			if (input[i] == BLOB_PINYIN_SPLIC[j][0]) {
				for (unsigned int offset = 1; offset < MEOW_MAXLENGTH; offset++) {
					if (i - 1 < 0) break;
					if (BLOB_PINYIN_SPLIC[j][offset] == L'\0') {
						break;
					}
					if (BLOB_PINYIN_SPLIC[j][offset] == input[i - 1]) {
						result[i] = 1;
						added = true;
						break;
					}
				}
				break;
			}
		}
		if (added) continue;
		for (j = 0; j < BLOB_PINYIN_SPLID_LEN; j++) {
			bool match = false;
			for (unsigned int offset = 0; offset < 4; offset++) {
				if (i - offset < 0) break;
				if (BLOB_PINYIN_SPLID[j][offset] == L'\0') {
					match = true;
					result[i - offset + 1] = 1;
					break;
				}
				if (BLOB_PINYIN_SPLID[j][offset] != input[i - offset]) {
					break;
				}
			}
			if (match) break;
		}
	}
	// s  h  u  r  u  f  a  0
	//[1, 0, 0, 1, 0, 1, 0, 0] to [3, 0, 0, 2, 0, 2, 0, 0]
	unsigned int last = 0;
	for (i = 1; i <= inputlength; i++) { // start from 1
		if (i == inputlength) { // last one or 1

			PINYIN_SEGMENT seg;
			seg.index = last;
			seg.length = inputlength - last;
			segments.push_back(seg);

			result[last] = inputlength - last;
			result[inputlength] = 0;
		}
		else if (result[i] == 1) {

			PINYIN_SEGMENT seg;
			seg.index = last;
			seg.length = i - last;
			segments.push_back(seg);

			result[last] = i - last;
			last = i;
		}
	}
	return;
}


// zero false
unsigned int MeowPinyin::MatchToSample(CONST WCHAR * input, unsigned int length, CONST WCHAR * sample) {
	unsigned int i = 0;
	for (i = 0; i < length; i++) {
		if (sample[i] < L'a' || sample[i] > L'z') {
			return i;
		}
		else if (sample[i] != input[i]) {
			return 0;
		}
	}
	if (sample[length] < L'a' || sample[length] > L'z') {
		return length;
	}
	else {
		return 0;
	}
}

unsigned int MeowPinyin::MatchSubToSample(CONST WCHAR * input, unsigned int length, CONST WCHAR * sample) {
	unsigned int i = 0;
	for (i = 0; i < length; i++) {
		if (sample[i] < L'a' || sample[i] > L'z') {
			return i;
		}
		else if (sample[i] != input[i]) {
			return i;
		}
	}
	if (sample[length] < L'a' || sample[length] > L'z') { // sample ended
		return MAXWORD; // end and fit
	}
	else {
		return length;
	}
}

// zero false
unsigned int MeowPinyin::Match(CONST WCHAR * a, CONST WCHAR * b) {
	unsigned int i = 0;
	while (true) {
		if ((a[i] < L'a' || a[i] > L'z') && (b[i] < L'a' || b[i] > L'z')) {
			return i;
		}
		if (a[i] != b[i]) return 0;
		i++;
	}
}

bool MeowPinyin::RecursiveQuanpinParser(PINYIN_SEGMENT * segment, unsigned int index, unsigned int length, unsigned int level) {
	unsigned int i = 0;
	if (length <= 0) {
		solutioncache.length = level;
		segment->solutions_quanpin.push_back(solutioncache);
		return true;
	}
	bool found = false;
	unsigned int eaten;
	for (i = 0; i < BLOB_PINYIN_FULL_LEN; i++) {
		eaten = MatchToSample(input + index, length, BLOB_PINYIN_FULL[i]);
		if (eaten > 0) {
			solutioncache.result[level] = i;
			if (RecursiveQuanpinParser(segment, index + eaten, length - eaten, level + 1)) {
				found = true;
			}
		}
	}
	if (!found) { // found a no-full result
		solutioncache.length = level;
		solutioncache.rest_offset = index;
		solutioncache.rest_length = length;
		segment->solutions_quanpin_prefix.push_back(solutioncache);
		RecursiveAllParser(segment, index, length, level);
	}
	return found;
}

bool MeowPinyin::RecursiveAllParser(PINYIN_SEGMENT * segment, unsigned int index, unsigned int length, unsigned int level) {
	unsigned int i = 0;
	if (length <= 0) {
		solutioncache.length = level;
		segment->solutions_short.push_back(solutioncache);
		return true;
	}
	bool found = false;
	unsigned int eaten;
	for (i = 0; i < BLOB_PINYIN_ALL_LEN; i++) {
		eaten = MatchToSample(input + index, length, BLOB_PINYIN_ALL[i]);
		if (eaten > 0) {
			solutioncache.result[level] = i;
			if (RecursiveAllParser(segment, index + eaten, length - eaten, level + 1)) {
				found = true;
			}
		}
	}
	return found;
}

void MeowPinyin::ParseSegments() {
	size_t length = segments.size();
	for (unsigned int i = 0; i < length; i++) {
		RecursiveQuanpinParser(&segments[i], segments[i].index, segments[i].length, 0);
	}
}

UINT16 MeowPinyin::GetPinyinIndex(CONST WCHAR * input, unsigned int * length) {
	bool found = false;
	unsigned int eaten;
	for (unsigned int i = 0; i < BLOB_PINYIN_FULL_LEN; i++) {
		eaten = Match(input, BLOB_PINYIN_FULL[i]);
		if (eaten != 0) {
			*length = eaten;
			return i;
		}
	}
	return 0xFFFF;
}


UINT16 pinyinbuff[6];
void MeowPinyin::LoadDictChars() {
	HANDLE hfile = CreateFile(L"X:\\Workspaces\\git\\meow\\Tools\\character.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD size = GetFileSize(hfile, NULL);
	VOID * cache = HeapAlloc(GetProcessHeap(), NULL, size);
	DWORD sizeread;
	ReadFile(hfile, cache, size, &sizeread, NULL);
	if (sizeread != size) { return; }
	if (size % 2 != 0) { return; }

	WCHAR * offset = ((WCHAR *)cache + 1);
	DWORD length = size / 2 - 1;

	WCHAR tmp[128] = { 0 };

	unsigned int index = 0;
	unsigned int status = 0;
	WCHAR c_now;
	unsigned int pinyinc = 0;

	table_pinyin.push_back(0);
	table_pinyin.push_back(0);

	ZeroMemory(&table_character, sizeof(MEOW_CHARACTER_NODE) * 0xFFFF);


	for (unsigned int i = 0; i < length; i++) {
		switch (status) {
		case 0: // beigin char
		{
			c_now = *(offset + index);
			table_character[c_now].character = c_now;
			table_character[c_now].pinyin = -1;
			pinyinc = 0;
			status = 1;
		}
		break;
		case 1: // in pinyin
		{
			if (offset[i] == L' ') {
				unsigned int length;
				pinyinbuff[pinyinc] = GetPinyinIndex(offset + i + 1, &length);
				i += length;
				if (pinyinbuff[pinyinc] == MAXUINT) {
					int xxxx = 0;
				}
				pinyinc++;
			}
			else if (offset[i] == L'\n') {
				status = 0;
				if (pinyinc == 1) {
					table_character[c_now].pinyin = pinyinbuff[0];
				}
				else {
					table_character[c_now].pinyin = -2 - table_pinyin.size();
					table_pinyin.push_back(pinyinc);
					for (unsigned int t = 0; t < pinyinc; t++) {
						table_pinyin.push_back(pinyinbuff[t]);
					}
				}
				index = i + 1;
			}
		}
		break;
		}
	}
	HeapFree(GetProcessHeap(), NULL, cache);
	CloseHandle(hfile);
}


UINT8 MeowPinyin::GetCharTreeID(WCHAR c) {
	if (table_character[c].character == L'\0') {
		return 0xFF;
	}
	if (table_character[c].pinyin >= 0) {
		return BLOB_PINYIN_ALL[table_character[c].pinyin][0] - L'a';
	}
	if (table_character[c].pinyin < -1) {
		unsigned int offset = -table_character[c].pinyin - 2;
		return BLOB_PINYIN_ALL[table_pinyin[offset + 1]][0] - L'a';
	}
	return 0xFF;
}

UINT32 MeowPinyin::MakeTreeNode(UINT32 _parent, UINT8 * next, UINT8 length, UINT8 depth) {
	if (length <= 0 || depth <= 0) {
		return _parent;
	}

	if (*next >= 26) {
		return 0;
	}

	// warning: not thread safe, parent become N/A when tree_phrase changed
	MEOW_PHRASETREE_NODE * parent = &tree_phrase[_parent];

	// children should not be 0 because 0 is root
	if (parent->children == 0) {
		MEOW_PHRASETREE_NODE cache;
		cache.children = 0;
		parent->children = tree_phrase.size();
		for (unsigned int i = 0; i < 26; i++) {
			tree_phrase.push_back(cache);
		}
		parent = &tree_phrase[_parent];
	}
	return MakeTreeNode(parent->children + *next, next + 1, length - 1, depth - 1);
}
void MeowPinyin::LoadDictBasic() {
	{
		HANDLE hfile = CreateFile(L"X:\\Workspaces\\git\\meow\\Tools\\phrase.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD size = GetFileSize(hfile, NULL);
		blob_phrase = HeapAlloc(GetProcessHeap(), NULL, size);
		DWORD tmp;
		ReadFile(hfile, blob_phrase, size, &tmp, NULL);
		if (tmp != size) { return; }
		WCHAR * offset = ((WCHAR *)blob_phrase + 1);
		DWORD length = size / 2 - 1;
		table_phrase.clear();
		table_phrase.reserve(445494);
		unsigned int index = 0;
		for (unsigned int i = 0; i < length; i++) {
			if (offset[i] == L'\n') {
				offset[i] = L'\0';
				MEOW_PHRASE_NODE mpn;
				mpn.length = i - index;
				mpn.phrase = offset + index;
				table_phrase.push_back(mpn);
				index = i + 1;
			}
		}
		//HeapFree(GetProcessHeap(), NULL, blob_phrase);
		CloseHandle(hfile);
	}

	MEOW_PHRASETREE_NODE root;
	root.children = 0;
	tree_phrase.reserve(311073);
	tree_phrase.push_back(root);


	UINT32 length = table_phrase.size();
	for (unsigned int i = 0; i < length; i++) {
		UINT8 path[4] = { 0 };
		for (unsigned int j = 0; j < 4; j++) {
			if (j < table_phrase[i].length) {
				path[j] = GetCharTreeID(table_phrase[i].phrase[j]);
			}
			else {
				break;
			}
		}
		UINT32 node = MakeTreeNode(0, path, table_phrase[i].length, 4);
		tree_phrase[node].phrases.push_back(i);
	}


	length = tree_phrase.size();
	UINT32 max = 0;
	for (unsigned int i = 0; i < length; i++) {
		UINT32 tmp = tree_phrase[i].phrases.size();
		if (tmp > max) {
			max = tmp;
		}
	}
}

bool MeowPinyin::MatchPinyinToPinyin(WORD _py, WORD py) {
	if (_py == py) return true;
	if (BLOB_PINYIN_ALL_SHORTCUT[_py][0] == py) return true;
	if (BLOB_PINYIN_ALL_SHORTCUT[_py][1] == py) return true;
	return false;
}
// can this char prounce in this way?
bool MeowPinyin::MatchCharToPinyin(WCHAR c, WORD py) {
	if (table_character[c].character == L'\0') return false;
	if (MatchPinyinToPinyin(table_character[c].pinyin, py)) return true;
	if (table_character[c].pinyin < -1) {
		unsigned int offset = -table_character[c].pinyin - 2;
		unsigned int length_py = table_pinyin[offset];
		for (unsigned int k = 0; k < length_py; k++) {
			if (MatchPinyinToPinyin(table_pinyin[offset + 1 + k], py)) return true;
		}
	}
	return false;
}


void MeowPinyin::LoadDictionary() {
	LoadDictChars();
	LoadDictBasic();
}


UINT8 MeowPinyin::GetPinyinTreeID(UINT16 p) {
	return BLOB_PINYIN_ALL[p][0] - L'a';
}

UINT32 MeowPinyin::FindTreeNode(UINT32 _parent, UINT8 * next, UINT8 length, UINT8 depth) {
	if (length <= 0 || depth <= 0) {
		return _parent;
	}
	if (*next >= 26) {
		return 0;
	}
	MEOW_PHRASETREE_NODE * parent = &tree_phrase[_parent];
	if (parent->children == 0) {
		return 0;
	}
	return FindTreeNode(parent->children + *next, next + 1, length - 1, depth - 1);
}

void MeowPinyin::PrintCandidate() {
	wprintf(L"%s -->", input);
	UINT8 path[4] = { 0 };
	unsigned rlen = results.size();
	for (unsigned int rt = 0; rt < rlen; rt++)
	{
		unsigned rsize = results[rt].pinyin.size();
		for (unsigned int i = 0; i < 4; i++) {
			if (i < rsize) {
				path[i] = GetPinyinTreeID(results[rt].pinyin[i]);
			}
			else {
				break;
			}
		}
		UINT32 node = FindTreeNode(0, path, rsize, 4);
		MEOW_PHRASETREE_NODE * treenode = &tree_phrase[node];
		unsigned int casize = treenode->phrases.size();
		unsigned int count = 0;
		for (unsigned int i = 0; i < casize; i++)
		{
			bool match = true;
			MEOW_PHRASE_NODE * wn = &table_phrase[treenode->phrases[i]];
			if (wn->length < rsize) continue;
			for (unsigned int j = 0; j < rsize; j++) {
				if (!MatchCharToPinyin(wn->phrase[j], results[rt].pinyin[j])) {
					match = false;
					break;
				}
			}
			if (match) {
				_setmode(_fileno(stdout), _O_U16TEXT);
				count++;
				wprintf(L" %ls", wn->phrase);
				if (count < 75) {

				}
				else {
				}
			}
		}
	}


	wprintf(L"\n");
}
