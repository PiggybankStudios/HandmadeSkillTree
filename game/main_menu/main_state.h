/*
File:   main_state.h
Author: Taylor Robbins
Date:   07\02\2024
*/

#ifndef _MAIN_STATE_H
#define _MAIN_STATE_H

#include "ui/ui_scroll_view.h"
#include "ui/ui_divider.h"

#define BACKGROUND_COLOR              MonokaiBack
#define NODE_WIDTH                    64 //px
#define NODE_HEIGHT                   64 //px
#define NODE_CORNER_RADIUS            10 //px
#define NODE_BORDER_THICKNESS         3 //px
#define NODE_NAME_MARGIN              5 //px
#define NODE_HORI_MARGIN              96 //px
#define NODE_VERT_MARGIN              50 //px
#define NODE_MAX_NAME_WIDTH_SLOP      96 //px
#define NODE_SHADOW_SIZE              12 //px
#define NODE_NAME_COLOR               MonokaiWhite
#define CONNECTION_COLOR              MonokaiGray2
#define CONNECTION_HIGHLIGHT_COLOR    MonokaiGray1
#define CONNECTION_THICKNESS          4 //px
#define CAMERA_MOVE_SPEED_FAST        25 //px/frame
#define CAMERA_MOVE_SPEED_SLOW        10 //px/frame
#define CAMERA_LAG                    4 //divisor
#define CAMERA_LIMIT_BORDER_THICKNESS 64 //px

enum NodePlacementAlg_t
{
	NodePlacementAlg_None = 0,
	NodePlacementAlg_Simple,
	NodePlacementAlg_DepsRows,
	NodePlacementAlg_NumAlgs,
};
const char* GetNodePlacementAlgStr(NodePlacementAlg_t enumValue)
{
	switch (enumValue)
	{
		case NodePlacementAlg_None:     return "None";
		case NodePlacementAlg_Simple:   return "Simple";
		case NodePlacementAlg_DepsRows: return "DepsRows";
		default: return "Unknown";
	}
}

enum SkillNodeConnType_t
{
	SkillNodeConnType_None = 0,
	SkillNodeConnType_Dependency,
	SkillNodeConnType_Sibling,
	SkillNodeConnType_NumTypes,
};
const char* GetSkillNodeConnTypeStr(SkillNodeConnType_t enumValue)
{
	switch (enumValue)
	{
		case SkillNodeConnType_None:       return "None";
		case SkillNodeConnType_Dependency: return "Dependency";
		case SkillNodeConnType_Sibling:    return "Sibling";
		default: return "Unknown";
	}
}

struct SkillNodeConn_t
{
	SkillNodeConnType_t type;
	u64 upId; //this is the library/dependency
	u64 downId; //this is the thing that uses that library/dependency
};

struct SkillNode_t
{
	u64 id;
	MyStr_t name;
	VarArray_t connections; //SkillNodeConn_t
	u64 numDependents;
	
	u64 totalDepsCount;
	
	v2 currentPos;
	v2 targetPos;
	
	rec mainRec;
	//These are all relative to mainRec
	rec fillRec;
	TextMeasure_t nameMeasure; 
	v2 namePos;
};

struct MainAppState_t
{
	bool initialized;
	
	bool doPlacement;
	NodePlacementAlg_t placementAlg;
	bool isNodeSelected;
	u64 selectedNodeId;
	bool isNodeHovered;
	u64 hoveredNodeId;
	
	u64 nextNodeId;
	rec nodeBounds;
	VarArray_t nodes; //SkillNode_t
	
	bool moveCameraToCenter;
	v2 cameraPos;
	v2 cameraGoto;
	bool isMiddleDragging;
	v2 middleMouseDragPos;
};

#endif //  _MAIN_STATE_H
