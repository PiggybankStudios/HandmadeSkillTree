/*
File:   main_helpers.cpp
Author: Taylor Robbins
Date:   07\19\2024
Description: 
	** Holds a variety of loose functions that help the Main AppState do it's job
*/

void FreeSkillNode(SkillNode_t* node)
{
	NotNull(node);
	ClearPointer(node);
	FreeString(mainHeap, &node->name);
	FreeVarArray(&node->connections);
	ClearPointer(node);
}

SkillNode_t* FindSkillNodeById(u64 id)
{
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		if (node->id == id) { return node; }
	}
	return nullptr;
}

SkillNode_t* AddSkillNode(MyStr_t name, u64 numDependencies = 0, u64* dependencyIds = nullptr)
{
	NotNullStr(&name);
	SkillNode_t* newNode = VarArrayAdd(&main->nodes, SkillNode_t);
	NotNull(newNode);
	ClearPointer(newNode);
	newNode->id = main->nextNodeId;
	main->nextNodeId++;
	CreateVarArray(&newNode->connections, mainHeap, sizeof(SkillNodeConn_t));
	AssertIf(numDependencies > 0, dependencyIds != nullptr);
	for (u64 dIndex = 0; dIndex < numDependencies; dIndex++)
	{
		u64 dependencyId = dependencyIds[dIndex];
		SkillNodeConn_t* newConnection = VarArrayAdd(&newNode->connections, SkillNodeConn_t);
		NotNull(newConnection);
		ClearPointer(newConnection);
		newConnection->type = SkillNodeConnType_Dependency;
		newConnection->upId = dependencyId;
		newConnection->downId = newNode->id;
	}
	newNode->name = AllocString(mainHeap, &name);
	return newNode;
}

void CalculateAllDependents()
{
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		node->numDependents = 0;
	}
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		VarArrayLoop(&node->connections, cIndex)
		{
			VarArrayLoopGet(SkillNodeConn_t, connection, &node->connections, cIndex);
			SkillNode_t* dependencyNode = FindSkillNodeById(connection->upId);
			dependencyNode->numDependents++;
		}
	}
}

u64 CalculateTotalDepsForNode(SkillNode_t* node)
{
	NotNull(node);
	
	if (node->totalDepsCount == 0 && node->connections.length > 0)
	{
		// We set the totalDepsCount to 1 here to make sure we don't find a loop in the graph that leads back to ourself and therefore recurse infinitely.
		// Loops in the graph are still bad for other reasons, but at least we won't crash
		node->totalDepsCount = 1;
		
		VarArrayLoop(&node->connections, cIndex)
		{
			VarArrayLoopGet(SkillNodeConn_t, connection, &node->connections, cIndex);
			Assert(connection->upId != node->id);
			if (connection->type == SkillNodeConnType_Dependency)
			{
				SkillNode_t* dependencyNode = FindSkillNodeById(connection->upId);
				NotNull(dependencyNode);
				u64 dependencyTotalDepsCount = CalculateTotalDepsForNode(dependencyNode);
				node->totalDepsCount = MaxU64(node->totalDepsCount, dependencyTotalDepsCount+1);
			}
		}
	}
	
	return node->totalDepsCount;
}

void CalculateTotalDepsForAll()
{
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		node->totalDepsCount = 0;
	}
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		CalculateTotalDepsForNode(node);
	}
}

void PlaceSkillNodes(NodePlacementAlg_t algorithm, bool animate = false)
{
	switch (algorithm)
	{
		// +==============================+
		// |   NodePlacementAlg_Simple    |
		// +==============================+
		case NodePlacementAlg_Simple:
		{
			v2 currentPos = NewVec2(40, 40);
			r32 rowHeight = 0;
			VarArrayLoop(&main->nodes, nIndex)
			{
				VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
				if (currentPos.x + node->mainRec.width/2 >= ScreenSize.width)
				{
					currentPos.x = 40;
					currentPos.y += rowHeight;
					rowHeight = 0;
				}
				node->targetPos = currentPos;
				if (!animate) { node->currentPos = node->targetPos; }
				currentPos.x += node->mainRec.width + NODE_HORI_MARGIN;
				rowHeight = MaxR32(rowHeight, node->mainRec.height);
			}
		} break;
		
		// +==============================+
		// |  NodePlacementAlg_DepsRows   |
		// +==============================+
		case NodePlacementAlg_DepsRows:
		{
			MemArena_t* scratch = GetScratchArena();
			RcBindFont(&pig->resources.fonts->debug, SelectDefaultFontFace());
			CalculateTotalDepsForAll();
			
			u64 maxTotalDeps = 0;
			u64 numUnconnectedNodes = 0;
			VarArrayLoop(&main->nodes, nIndex)
			{
				VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
				maxTotalDeps = MaxU64(maxTotalDeps, node->totalDepsCount);
				if (node->connections.length == 0 && node->numDependents == 0) { numUnconnectedNodes++; }
			}
			
			u64 numRows = maxTotalDeps+1;
			r32* rowHeights = AllocArray(scratch, r32, numRows);
			MyMemSet(rowHeights, 0x00, sizeof(r32) * numRows);
			VarArrayLoop(&main->nodes, nIndex)
			{
				VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
				if (node->connections.length > 0 || node->numDependents > 0)
				{
					r32* rowHeight = &rowHeights[node->totalDepsCount];
					*rowHeight = MaxR32(*rowHeight, node->mainRec.height);
				}
			}
			for (u64 rIndex = 0; rIndex < numRows; rIndex++)
			{
				if (rIndex > 0) { rowHeights[rIndex] += rowHeights[rIndex-1] + NODE_VERT_MARGIN + RcGetLineHeight() + NODE_NAME_MARGIN; }
			}
			
			r32 maxRowWidth = 0;
			r32* rowWidths = AllocArray(scratch, r32, numRows);
			MyMemSet(rowWidths, 0x00, sizeof(r32) * numRows);
			VarArrayLoop(&main->nodes, nIndex)
			{
				VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
				if (node->connections.length > 0 || node->numDependents > 0)
				{
					node->targetPos.y = 40 + rowHeights[node->totalDepsCount];
					node->targetPos.x = 40 + rowWidths[node->totalDepsCount];
					rowWidths[node->totalDepsCount] += node->mainRec.width;
					maxRowWidth = MaxR32(maxRowWidth, rowWidths[node->totalDepsCount]);
					rowWidths[node->totalDepsCount] += NODE_HORI_MARGIN;
				}
				else
				{
					//TODO: Place the unconnected nodes in a nicer shape, and choose a better center location in general
					node->targetPos = NewVec2(
						-150 + GetRandR32(&pig->random, -150, 150),
						-150 + GetRandR32(&pig->random, -150, 150)
					);
				}
			}
			
			VarArrayLoop(&main->nodes, nIndex)
			{
				VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
				if (node->connections.length > 0 || node->numDependents > 0)
				{
					r32 rowWidth = rowWidths[node->totalDepsCount];
					r32 widthDiff = maxRowWidth - rowWidth;
					if (widthDiff > 0)
					{
						node->targetPos.x += widthDiff/2;
					}
				}
				
				if (!animate) { node->currentPos = node->targetPos; }
			}
			
			FreeScratchArena(scratch);
		} break;
		
		default: Unimplemented(); break;
	}
}

bool IsConnectionHoverConnected(const SkillNodeConn_t* connection)
{
	return (main->isNodeHovered && (main->hoveredNodeId == connection->upId || main->hoveredNodeId == connection->downId));
}
