/*
File:   main_markdown.h
Author: Taylor Robbins
Date:   08\17\2024
*/

#ifndef _MAIN_MARKDOWN_H
#define _MAIN_MARKDOWN_H

struct FileStr_t
{
	Rangei_t range;
	MyStr_t str;
};

enum MdPieceType_t
{
	MdPieceType_None = 0,
	MdPieceType_Header,
	MdPieceType_KeyValue,
	MdPieceType_BulletPoint,
	MdPieceType_Comment,
	MdPieceType_EmptyLine,
	MdPieceType_Content, //All other blocks of text, separated by new-line chars
	MdPieceType_NumTypes,
};
const char* GetMdPieceTypeStr(MdPieceType_t enumValue)
{
	switch (enumValue)
	{
		case MdPieceType_None:        return "None";
		case MdPieceType_Header:      return "Header";
		case MdPieceType_KeyValue:    return "KeyValue";
		case MdPieceType_BulletPoint: return "BulletPoint";
		case MdPieceType_Comment:     return "Comment";
		case MdPieceType_EmptyLine:   return "EmptyLine";
		case MdPieceType_Content:     return "Content";
		default: return "Unknown";
	}
}

struct MdPiece_t
{
	MdPieceType_t type;
	
	FileStr_t prefix;
	FileStr_t editable;
	FileStr_t suffix;
	
	u64 headerLevel; //MdPieceType_Header
	FileStr_t keyStr; //MdPieceType_KeyValue
};

struct MdFile_t
{
	MemArena_t* allocArena;
	MyStr_t path;
	MyStr_t contents;
	LinkedList_t pieces; //MdPiece_t
};

#endif //  _MAIN_MARKDOWN_H
