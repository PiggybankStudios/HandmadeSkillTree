/*
File:   main_tree.h
Author: Taylor Robbins
Date:   08\19\2024
*/

#ifndef _MAIN_TREE_H
#define _MAIN_TREE_H

enum TreeNodeConnType_t
{
	TreeNodeConnType_None = 0,
	TreeNodeConnType_Dependency,
	TreeNodeConnType_Sibling,
	TreeNodeConnType_NumTypes,
};
const char* GetTreeNodeConnTypeStr(TreeNodeConnType_t enumValue)
{
	switch (enumValue)
	{
		case TreeNodeConnType_None:       return "None";
		case TreeNodeConnType_Dependency: return "Dependency";
		case TreeNodeConnType_Sibling:    return "Sibling";
		default: return "Unknown";
	}
}

struct TreeNodeConn_t
{
	TreeNodeConnType_t type;
	u64 upId; //this is the library/dependency
	u64 downId; //this is the thing that uses that library/dependency
};

struct TreeNode_t
{
	u64 id;
	MyStr_t name;
	VarArray_t connections; //TreeNodeConn_t
	u64 numDependents;
	
	u64 totalDepsCount;
	
	v2 currentPos;
	v2 targetPos;
	
	bool leftClickStartedInside;
	bool nodeIsBeingDragged;
	v2 leftClickOffset;
	
	rec mainRec;
	//These are all relative to mainRec
	rec fillRec;
	TextMeasure_t nameMeasure; 
	v2 namePos;
};

struct Tree_t
{
	MemArena_t* allocArena;
	u64 nextNodeId;
	VarArray_t nodes; //SkillNode_t
	
	rec nodeBounds;
};

#endif //  _MAIN_TREE_H
