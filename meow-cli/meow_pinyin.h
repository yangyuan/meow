#pragma once


// dont worry too much about this length
// memory is re-used and user cant input that long for pinyin
#define MEOW_MAXLENGTH 64

// UINT8
// UINT32, which is used to index tree_phrase
// UINT16, 16 bit unsgined integer, 0 - 0xFFFF, which is used to index pinyin & character
// WCHAR, UCS-2 LE

/*
typedef struct _MEOW_PINYIN_NODE {
UINT16 me;
UINT16 shortcuts[2];
} MEOW_PINYIN_NODE;
*/

typedef struct _MEOW_CHARACTER_NODE {
	WCHAR character; // L'\0' = N/A
	SHORT pinyin;
} MEOW_CHARACTER_NODE;

typedef struct _MEOW_PHRASE_NODE {
	WCHAR * phrase;
	UINT8 length;
} MEOW_PHRASE_NODE;

// BIGTREE is used to index all phrases, cooperate with MEOW_PHRASE_NODE
typedef struct _MEOW_PHRASETREE_NODE {
	std::vector<UINT32> phrases;
	// children has only 2 possible size: 0 or index to a 26 length MEOW_PHRASETREE_NODE
	UINT32 children;
} MEOW_PHRASETREE_NODE;

typedef struct {
	unsigned int length;
	unsigned int rest_offset;
	unsigned int rest_length;
	unsigned int result[MEOW_MAXLENGTH];
} PINYIN_SEGMENT_SOLUTION;

// PINYIN_SEGMENT
typedef struct _PINYIN_SEGMENT {
	unsigned int index;
	unsigned int length;
	// we used different array for different result 
	std::vector<PINYIN_SEGMENT_SOLUTION> solutions_quanpin;
	std::vector<PINYIN_SEGMENT_SOLUTION> solutions_quanpin_prefix;
	std::vector<PINYIN_SEGMENT_SOLUTION> solutions_short;
} PINYIN_SEGMENT;

typedef struct _PINYIN_RESULT {
	unsigned int power;
	unsigned int length;
	std::vector<WORD> pinyin;
} PINYIN_RESULT;

class MeowPinyin
{
public:
	MeowPinyin();
	~MeowPinyin();

	unsigned int Solute(WCHAR * input);
	void PrintCandidate();


	void LoadDictionary();
private:
	void DoSplitIntoSegments();
	void ParseSegments();
	void GenerateResult();

	bool RecursiveQuanpinParser(PINYIN_SEGMENT * segment, unsigned int index, unsigned int length, unsigned int level);
	bool RecursiveAllParser(PINYIN_SEGMENT * segment, unsigned int index, unsigned int length, unsigned int level);

	bool RecursiveQuanpinResult(unsigned int index, unsigned int offset_pinyin, PINYIN_RESULT pr);
	bool RecursiveQuanpinPendingResult(unsigned int index, unsigned int offset_pinyin, PINYIN_RESULT pr);
	bool RecursiveQuanpinFirstResult(unsigned int index, unsigned int offset_pinyin, PINYIN_RESULT pr);

	inline unsigned int MatchToSample(const WCHAR * input, unsigned int length, const WCHAR * sample);
	inline unsigned int MatchSubToSample(const WCHAR * input, unsigned int length, const WCHAR * sample);
	inline unsigned int Match(const WCHAR * input, const WCHAR * sample);

	UINT16 GetPinyinIndexAll(const WCHAR * input, std::vector<unsigned short> * output, unsigned int offset, unsigned int length);
	UINT16 GetPinyinIndex(const WCHAR * input, unsigned int * length);


	void LoadDictChars();
	void LoadDictBasic();



	// what user input (original)
	WCHAR input[MEOW_MAXLENGTH];
	unsigned int inputlength;

	PINYIN_SEGMENT_SOLUTION solutioncache;
	PINYIN_RESULT pinyincache;

	std::vector<PINYIN_SEGMENT> segments;
	std::vector<PINYIN_SEGMENT_SOLUTION> solution;
	std::vector<PINYIN_RESULT> results;


	inline bool MatchPinyinToPinyin(WORD _py, WORD py);
	inline bool MatchCharToPinyin(WCHAR c, WORD py);
	UINT8 GetPinyinTreeID(UINT16 p);
	UINT8 GetCharTreeID(WCHAR c);
	UINT32 FindTreeNode(UINT32 _parent, UINT8 * next, UINT8 length, UINT8 depth);
	UINT32 MakeTreeNode(UINT32 _parent, UINT8 * next, UINT8 length, UINT8 depth);


	// blob, table & tree
	// `blob` are memory data, hard-coded or directlly loaded from file.
	// `table` is used to make index small, index usually has an UINT32 offset and UINT8 length of table
	// `tree` is usually a tree index of data 
	std::vector<UINT16>               table_pinyin;
	MEOW_CHARACTER_NODE               table_character[0xFFFF];
	void *                            blob_phrase;
	std::vector<MEOW_PHRASE_NODE>     table_phrase; // 445494
	std::vector<MEOW_PHRASETREE_NODE> tree_phrase;
};


class MeowPinyinDictionary
{
public:
	void LoadSource() {};
	void LoadBinary() {};
	void DumpBinary() {};
};
