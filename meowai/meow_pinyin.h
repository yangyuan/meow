#pragma once


typedef struct  {
	bool is_quanpin;
	bool is_quanpin_perfix;
	unsigned int length;
	unsigned int result_error[8][128];
} PinyinSegmentSolution;

// PinyinSegment
typedef struct  {
	bool has_quanpin;
	bool has_quanpin_perfix;
	bool has_xxx;
	bool has_yyy;

	unsigned int index;
	unsigned int length;

	// min length of the split result
	unsigned int length_min;
	// ordered 
	PinyinSegmentSolution * solutions_quanpin;
	PinyinSegmentSolution * solutions_short;
} PinyinSegment;







class MeowPinyin
{
public:
	MeowPinyin();
	~MeowPinyin();

	static void subsplit(WCHAR * input);
	static void split(WCHAR * input);
	static void printsplit(WCHAR * input);

	unsigned int Solute (WCHAR * input);

private:
	static const unsigned int maxlength = 128;
	void DoSplitIntoSegments();
	void PrintSegments();



	// what user input (original)
	unsigned int length;
	WCHAR input[128];

	CAtlArray<PinyinSegment> segments;
};