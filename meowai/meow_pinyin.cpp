#include "meow.h"
#include "meow_pinyin.h"

static const UINT PINYIN_FULL_LEN = 409;
static const WCHAR PINYIN_FULL[409][7] = { L"a", L"ai", L"an", L"ang", L"ao", L"ba", L"bai", L"ban", L"bang", L"bao", L"bei", L"ben", L"beng", L"bi", L"bian", L"biao", L"bie", L"bin", L"bing", L"bo", L"bu", L"ca", L"cai", L"can", L"cang", L"cao", L"ce", L"cen", L"ceng", L"cha", L"chai", L"chan", L"chang", L"chao", L"che", L"chen", L"cheng", L"chi", L"chong", L"chou", L"chu", L"chua", L"chuai", L"chuan", L"chuang", L"chui", L"chun", L"chuo", L"ci", L"cong", L"cou", L"cu", L"cuan", L"cui", L"cun", L"cuo", L"da", L"dai", L"dan", L"dang", L"dao", L"de", L"den", L"dei", L"deng", L"di", L"dia", L"dian", L"diao", L"die", L"ding", L"diu", L"dong", L"dou", L"du", L"duan", L"dui", L"dun", L"duo", L"e", L"ei", L"en", L"eng", L"er", L"fa", L"fan", L"fang", L"fei", L"fen", L"feng", L"fo", L"fou", L"fu", L"ga", L"gai", L"gan", L"gang", L"gao", L"ge", L"gei", L"gen", L"geng", L"gong", L"gou", L"gu", L"gua", L"guai", L"guan", L"guang", L"gui", L"gun", L"guo", L"ha", L"hai", L"han", L"hang", L"hao", L"he", L"hei", L"hen", L"heng", L"hong", L"hou", L"hu", L"hua", L"huai", L"huan", L"huang", L"hui", L"hun", L"huo", L"ji", L"jia", L"jian", L"jiang", L"jiao", L"jie", L"jin", L"jing", L"jiong", L"jiu", L"ju", L"juan", L"jue", L"jun", L"ka", L"kai", L"kan", L"kang", L"kao", L"ke", L"ken", L"keng", L"kong", L"kou", L"ku", L"kua", L"kuai", L"kuan", L"kuang", L"kui", L"kun", L"kuo", L"la", L"lai", L"lan", L"lang", L"lao", L"le", L"lei", L"leng", L"li", L"lia", L"lian", L"liang", L"liao", L"lie", L"lin", L"ling", L"liu", L"long", L"lou", L"lu", L"lv", L"luan", L"lue", L"lve", L"lun", L"luo", L"ma", L"mai", L"man", L"mang", L"mao", L"me", L"mei", L"men", L"meng", L"mi", L"mian", L"miao", L"mie", L"min", L"ming", L"miu", L"mo", L"mou", L"mu", L"na", L"nai", L"nan", L"nang", L"nao", L"ne", L"nei", L"nen", L"neng", L"ni", L"nian", L"niang", L"niao", L"nie", L"nin", L"ning", L"niu", L"nong", L"nou", L"nu", L"nv", L"nuan", L"nve", L"nuo", L"nun", L"o", L"ou", L"pa", L"pai", L"pan", L"pang", L"pao", L"pei", L"pen", L"peng", L"pi", L"pian", L"piao", L"pie", L"pin", L"ping", L"po", L"pou", L"pu", L"qi", L"qia", L"qian", L"qiang", L"qiao", L"qie", L"qin", L"qing", L"qiong", L"qiu", L"qu", L"quan", L"que", L"qun", L"ran", L"rang", L"rao", L"re", L"ren", L"reng", L"ri", L"rong", L"rou", L"ru", L"ruan", L"rui", L"run", L"ruo", L"sa", L"sai", L"san", L"sang", L"sao", L"se", L"sen", L"seng", L"sha", L"shai", L"shan", L"shang", L"shao", L"she", L"shei", L"shen", L"sheng", L"shi", L"shou", L"shu", L"shua", L"shuai", L"shuan", L"shuang", L"shui", L"shun", L"shuo", L"si", L"song", L"sou", L"su", L"suan", L"sui", L"sun", L"suo", L"ta", L"tai", L"tan", L"tang", L"tao", L"te", L"teng", L"ti", L"tian", L"tiao", L"tie", L"ting", L"tong", L"tou", L"tu", L"tuan", L"tui", L"tun", L"tuo", L"wa", L"wai", L"wan", L"wang", L"wei", L"wen", L"weng", L"wo", L"wu", L"xi", L"xia", L"xian", L"xiang", L"xiao", L"xie", L"xin", L"xing", L"xiong", L"xiu", L"xu", L"xuan", L"xue", L"xun", L"ya", L"yan", L"yang", L"yao", L"ye", L"yi", L"yin", L"ying", L"yo", L"yong", L"you", L"yu", L"yuan", L"yue", L"yun", L"za", L"zai", L"zan", L"zang", L"zao", L"ze", L"zei", L"zen", L"zeng", L"zha", L"zhai", L"zhan", L"zhang", L"zhao", L"zhe", L"zhei", L"zhen", L"zheng", L"zhi", L"zhong", L"zhou", L"zhu", L"zhua", L"zhuai", L"zhuan", L"zhuang", L"zhui", L"zhun", L"zhuo", L"zi", L"zong", L"zou", L"zu", L"zuan", L"zui", L"zun", L"zuo" };

static const UINT PINYIN_ALL_LEN = 433;
static const WCHAR PINYIN_ALL[433][7] = { L"a", L"ai", L"an", L"ang", L"ao", L"b", L"ba", L"bai", L"ban", L"bang", L"bao", L"bei", L"ben", L"beng", L"bi", L"bian", L"biao", L"bie", L"bin", L"bing", L"bo", L"bu", L"c", L"ch", L"ca", L"cai", L"can", L"cang", L"cao", L"ce", L"cen", L"ceng", L"cha", L"chai", L"chan", L"chang", L"chao", L"che", L"chen", L"cheng", L"chi", L"chong", L"chou", L"chu", L"chua", L"chuai", L"chuan", L"chuang", L"chui", L"chun", L"chuo", L"ci", L"cong", L"cou", L"cu", L"cuan", L"cui", L"cun", L"cuo", L"d", L"da", L"dai", L"dan", L"dang", L"dao", L"de", L"den", L"dei", L"deng", L"di", L"dia", L"dian", L"diao", L"die", L"ding", L"diu", L"dong", L"dou", L"du", L"duan", L"dui", L"dun", L"duo", L"e", L"ei", L"en", L"eng", L"er", L"f", L"fa", L"fan", L"fang", L"fei", L"fen", L"feng", L"fo", L"fou", L"fu", L"g", L"ga", L"gai", L"gan", L"gang", L"gao", L"ge", L"gei", L"gen", L"geng", L"gong", L"gou", L"gu", L"gua", L"guai", L"guan", L"guang", L"gui", L"gun", L"guo", L"h", L"ha", L"hai", L"han", L"hang", L"hao", L"he", L"hei", L"hen", L"heng", L"hong", L"hou", L"hu", L"hua", L"huai", L"huan", L"huang", L"hui", L"hun", L"huo", L"j", L"ji", L"jia", L"jian", L"jiang", L"jiao", L"jie", L"jin", L"jing", L"jiong", L"jiu", L"ju", L"juan", L"jue", L"jun", L"k", L"ka", L"kai", L"kan", L"kang", L"kao", L"ke", L"ken", L"keng", L"kong", L"kou", L"ku", L"kua", L"kuai", L"kuan", L"kuang", L"kui", L"kun", L"kuo", L"l", L"la", L"lai", L"lan", L"lang", L"lao", L"le", L"lei", L"leng", L"li", L"lia", L"lian", L"liang", L"liao", L"lie", L"lin", L"ling", L"liu", L"long", L"lou", L"lu", L"lv", L"luan", L"lue", L"lve", L"lun", L"luo", L"m", L"ma", L"mai", L"man", L"mang", L"mao", L"me", L"mei", L"men", L"meng", L"mi", L"mian", L"miao", L"mie", L"min", L"ming", L"miu", L"mo", L"mou", L"mu", L"n", L"na", L"nai", L"nan", L"nang", L"nao", L"ne", L"nei", L"nen", L"neng", L"ng", L"ni", L"nian", L"niang", L"niao", L"nie", L"nin", L"ning", L"niu", L"nong", L"nou", L"nu", L"nv", L"nuan", L"nve", L"nuo", L"nun", L"o", L"ou", L"p", L"pa", L"pai", L"pan", L"pang", L"pao", L"pei", L"pen", L"peng", L"pi", L"pian", L"piao", L"pie", L"pin", L"ping", L"po", L"pou", L"pu", L"q", L"qi", L"qia", L"qian", L"qiang", L"qiao", L"qie", L"qin", L"qing", L"qiong", L"qiu", L"qu", L"quan", L"que", L"qun", L"r", L"ran", L"rang", L"rao", L"re", L"ren", L"reng", L"ri", L"rong", L"rou", L"ru", L"ruan", L"rui", L"run", L"ruo", L"s", L"sh", L"sa", L"sai", L"san", L"sang", L"sao", L"se", L"sen", L"seng", L"sha", L"shai", L"shan", L"shang", L"shao", L"she", L"shei", L"shen", L"sheng", L"shi", L"shou", L"shu", L"shua", L"shuai", L"shuan", L"shuang", L"shui", L"shun", L"shuo", L"si", L"song", L"sou", L"su", L"suan", L"sui", L"sun", L"suo", L"t", L"ta", L"tai", L"tan", L"tang", L"tao", L"te", L"teng", L"ti", L"tian", L"tiao", L"tie", L"ting", L"tong", L"tou", L"tu", L"tuan", L"tui", L"tun", L"tuo", L"w", L"wa", L"wai", L"wan", L"wang", L"wei", L"wen", L"weng", L"wo", L"wu", L"x", L"xi", L"xia", L"xian", L"xiang", L"xiao", L"xie", L"xin", L"xing", L"xiong", L"xiu", L"xu", L"xuan", L"xue", L"xun", L"y", L"ya", L"yan", L"yang", L"yao", L"ye", L"yi", L"yin", L"ying", L"yo", L"yong", L"you", L"yu", L"yuan", L"yue", L"yun", L"z", L"zh", L"za", L"zai", L"zan", L"zang", L"zao", L"ze", L"zei", L"zen", L"zeng", L"zha", L"zhai", L"zhan", L"zhang", L"zhao", L"zhe", L"zhei", L"zhen", L"zheng", L"zhi", L"zhong", L"zhou", L"zhu", L"zhua", L"zhuai", L"zhuan", L"zhuang", L"zhui", L"zhun", L"zhuo", L"zi", L"zong", L"zou", L"zu", L"zuan", L"zui", L"zun", L"zuo" };

static const UINT PINYIN_SINGLE_LEN = 23;
static const WCHAR PINYIN_SINGLE[23] = { L'a', L'b', L'c', L'd', L'e', L'f', L'g', L'h', L'j', L'k', L'l', L'm', L'n', L'o', L'p', L'q', L'r', L's', L't', L'w', L'x', L'y', L'z' };

// 16
static const UINT PINYIN_SPLIA_LEN = 16;
static const WCHAR PINYIN_SPLIA[16] = { L'b', L'c', L'd', L'f', L'j', L'k', L'l', L'm', L'p', L'q', L's', L't', L'w', L'x', L'y', L'z' };

// ghr
static const UINT PINYIN_SPLIB_LEN = 3;
static const WCHAR PINYIN_SPLIB[3][5] = { L"gn", L"hzcs", L"re" };
// aeon
static const UINT PINYIN_SPLIC_LEN = 4;
static const WCHAR * PINYIN_SPLIC[4] = { L"aaeo", L"eaejoqx", L"oejoqvx", L"nbcdfghjklmnpqrstvwxyz" };

// i u v
static const UINT PINYIN_SPLID_LEN = 34;
static const WCHAR PINYIN_SPLID[34][4] = { L"ib", L"ihc", L"ic", L"id", L"ij", L"il", L"im", L"in", L"ip", L"iq", L"ihs", L"is", L"it", L"ix", L"iy", L"ihz", L"ub", L"uc", L"ud", L"uf", L"ug", L"uj", L"uk", L"ul", L"un", L"uq", L"ur", L"us", L"ut", L"uw", L"uy", L"uz", L"vl", L"vn" };

MeowPinyin::MeowPinyin()
{
}

MeowPinyin::~MeowPinyin()
{
}


unsigned int MeowPinyin::Solute(WCHAR * _input) {
	if (input == NULL) return 0;
	unsigned int len = wcslen(input);
	if (len < maxlength) {
		wcsncpy_s(input, _input, len);
		length = len;
	}
	else {
		wcsncpy_s(input, _input, maxlength-1);
		length = maxlength-1;
		input[maxlength-1] = L'\0';
	}

	DoSplitIntoSegments();
	return 1;
}
void MeowPinyin::DoSplitIntoSegments() {
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	unsigned int result[maxlength] {0};

	bool added = false;
	result[0] = 1; // 0
	for (i = 1; i < length; i++) { // start with 1
		added = false;
		for (j = 0; j < PINYIN_SPLIA_LEN; j++) {
			if (input[i] == PINYIN_SPLIA[j]) {
				result[i] = 1;
				added = true;
				break;
			}
		}
		if (added) continue;
		for (j = 0; j < PINYIN_SPLIB_LEN; j++) {
			if (input[i] == PINYIN_SPLIB[j][0]) {
				for (unsigned int offset = 1; offset < 5; offset++) {
					if (i - 1 < 0) break;
					if (PINYIN_SPLIB[j][offset] == L'\0') {
						result[i] = 1;
						added = true;
						break;
					}
					if (PINYIN_SPLIB[j][offset] == input[i - 1]) {
						break;
					}
				}
				break;
			}
		}
		if (added) continue;
		for (j = 0; j < PINYIN_SPLIC_LEN; j++) {
			if (input[i] == PINYIN_SPLIC[j][0]) {
				for (unsigned int offset = 1; offset < maxlength; offset++) {
					if (i - 1 < 0) break;
					if (PINYIN_SPLIC[j][offset] == L'\0') {
						break;
					}
					if (PINYIN_SPLIC[j][offset] == input[i - 1]) {
						result[i] = 1;
						added = true;
						break;
					}
				}
				break;
			}
		}
		if (added) continue;
		for (j = 0; j < PINYIN_SPLID_LEN; j++) {
			bool match = false;
			for (unsigned int offset = 0; offset < 4; offset++) {
				if (i - offset < 0) break;
				if (PINYIN_SPLID[j][offset] == L'\0') {
					match = true;
					result[i - offset + 1] = 1;
					break;
				}
				if (PINYIN_SPLID[j][offset] != input[i - offset]) {
					break;
				}
			}
			if (match) break;
		}
	}
	// s  h  u  r  u  f  a  0
	//[1, 0, 0, 1, 0, 1, 0, 0] to [3, 0, 0, 2, 0, 2, 0, 0]
	unsigned int last = 0;
	for (i = 1; i <= length; i++) { // start from 1
		if (i == length) { // last one or 1
			result[last] = length - last;
			result[length] = 0;
		}
		else if (result[i] == 1) {
			result[last] = i - last;
			last = i;

			PinyinSegment seg;
			seg.index = last;
			seg.length = i - last;
			segments.Add(seg);
		}
	}
	return 1;
}

bool match(WCHAR * input, CONST WCHAR * target) {
	unsigned int i = 0;
	for (i = 0; i < 128; i++) {
		if (target[i] == L'\0') {
			return true;
		}
		else if (target[i] != input[i]) {
			return false;
		}
	}
	return false;
}

unsigned int result[128] = { 0 };

unsigned int result_final_count = 0;
unsigned int result_final[8][128] = { 0 };

unsigned int result_error_count = 0;
unsigned int result_error[8][128] = { 0 }; 

bool find_full(WCHAR * input, unsigned int index, unsigned int length, unsigned int level) {
	unsigned int i = 0;
	if (index == length) {
		/*
		for (i = 0; i < level; i++) {
			wprintf(L" %s", PINYIN_FULL[result[i]]);
		}
		wprintf(L"\n");
		*/

		for (i = 0; i < level; i++) {
			// wprintf(L" %s", PINYIN_FULL[result[i]]);
			result_final[result_final_count][i + 1] = result[i];
		}
		result_final[result_final_count][0] = level;
		result_final_count++;
		return true;
	}
	bool found = false;
	for (i = 0; i < PINYIN_FULL_LEN; i++) {
		if (match(input + index, PINYIN_FULL[i])) {
			result[level] = i;
			if (find_full(input, index + wcslen(PINYIN_FULL[i]), length, level+1)) {
				found = true;
			}
		}
	}
	if (!found) {
		for (i = 0; i < level; i++) {
			// wprintf(L" %s", PINYIN_FULL[result[i]]);
			result_error[result_error_count][i+1] = result[i];
		}
		result_error[result_error_count][0] = (0 - level);
		result_error_count ++;
	}
	return found;
}

void MeowPinyin::subsplit(WCHAR * input) {
	if (input == NULL) return;

	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	unsigned int len = wcslen(input);
	unsigned int output[128] {0};
	if (len > 128) {
		return;
	}

	find_full(input, 0, len, 0);
}


void MeowPinyin::printsplit(WCHAR * input) {
	if (input == NULL) return;
	wprintf(L"-- %s:", input);
	unsigned int i, j = 0;

	for (i = 0; i < result_final_count; i++) {
		wprintf(L" {");
		for (j = 1; j <= result_final[i][0]; j++) {
			wprintf(L" %s", PINYIN_FULL[result_final[i][j]]);
		}
		wprintf(L" } ");
	}
	wprintf(L"\n");
}


void MeowPinyin::split(WCHAR * input) {
	if (input == NULL) return;

	wprintf(L"========\n%s\n", input);

	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int k = 0;
	unsigned int len = wcslen(input);
	unsigned int output[128] {0};
	if (len > 128) {
		return;
	}

	bool added = false;
	output[0] = 1; // 0
	for (i = 1; i < len; i++) { // start with 1
		added = false;
		for (j = 0; j < PINYIN_SPLIA_LEN; j++) {
			if (input[i] == PINYIN_SPLIA[j]) {
				output[i] = 1;
				added = true;
				break;
			}
		}
		if (added) continue;
		for (j = 0; j < PINYIN_SPLIB_LEN; j++) {
			if (input[i] == PINYIN_SPLIB[j][0]) {
				for (unsigned int offset = 1; offset < 5; offset++) {
					if (i - 1 < 0) break;
					if (PINYIN_SPLIB[j][offset] == L'\0') {
						output[i] = 1;
						added = true;
						break;
					}
					if (PINYIN_SPLIB[j][offset] == input[i - 1]) {
						break;
					}
				}
				break;
			}
		}
		if (added) continue;
		for (j = 0; j < PINYIN_SPLIC_LEN; j++) {
			if (input[i] == PINYIN_SPLIC[j][0]) {
				for (unsigned int offset = 1; offset < 128; offset++) {
					if (i - 1 < 0) break;
					if (PINYIN_SPLIC[j][offset] == L'\0') {
						break;
					}
					if (PINYIN_SPLIC[j][offset] == input[i - 1]) {
						output[i] = 1;
						added = true;
						break;
					}
				}
				break;
			}
		}
		if (added) continue;
		for (j = 0; j < PINYIN_SPLID_LEN; j++) {
			bool match = false;
			for (unsigned int offset = 0; offset < 4; offset++) {
				if (i - offset < 0) break;
				if (PINYIN_SPLID[j][offset] == L'\0') {
					match = true;
					output[i - offset + 1] = 1;
					break;
				}
				if (PINYIN_SPLID[j][offset] != input[i - offset]) {
					break;
				}
			}
			if (match) break;
		}
	}
	// s  h  u  r  u  f  a  0
	//[1, 0, 0, 1, 0, 1, 0, 0] to [3, 0, 0, 2, 0, 2, 0, 0]
	unsigned int last = 0;
	for (i = 1; i <= len; i++) { // start from 1
		if (i == len) { // last one or 1
			output[last] = len - last;
			output[len] = 0;
		}
		else if (output[i] == 1) {
			output[last] = i - last;
			last = i;
		}
	}

	WCHAR cache[128] = { 0 };
	wprintf(L"->", cache);
	unsigned int next = 0;
	while (true) {
		if (output[next] == 0) break;
		wcsncpy_s(cache, input + next, output[next]);
		wprintf(L" %s", cache);
		next += output[next];
	}
	wprintf(L"\n");

	next = 0;
	while (true) {
		if (output[next] == 0) break;
		wcsncpy_s(cache, input + next, output[next]);
		subsplit(cache);
		printsplit(cache);
		result_final_count = 0;
		result_error_count = 0;
		next += output[next];
	}
	wprintf(L"\n");
}
