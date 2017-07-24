#pragma once
// this file is used for fix the problem of vXXX_xp toolsets
// there some difference between the last SDK for xp (7.1) and new Win8 SDK
#if (WINVER <= 0x0601)
// GUID_TFCAT_TIPCAP_IMMERSIVESUPPORT {13A016DF-560B-46CD-947A-4C3AF1E0E35D}
static const GUID GUID_TFCAT_TIPCAP_IMMERSIVESUPPORT =
{ 0x13A016DF, 0x560B, 0x46CD, { 0x94, 0x7A, 0x4C, 0x3A, 0xF1, 0xE0, 0xE3, 0x5D } };
// GUID_TFCAT_TIPCAP_SYSTRAYSUPPORT {25504FB4-7BAB-4BC1-9C69-CF81890F0EF5}
static const GUID GUID_TFCAT_TIPCAP_SYSTRAYSUPPORT =
{ 0x25504FB4, 0x7BAB, 0x4BC1, { 0x9C, 0x69, 0xCF, 0x81, 0x89, 0x0F, 0x0E, 0xF5 } };
#endif

// 
static const IID IID_ITfDisplayAttributeCollectionProvider =
{ 0x3977526D, 0x1A0A, 0x435A, { 0x8D, 0x06, 0xEC, 0xC9, 0x51, 0x6B, 0x48, 0x4F } };