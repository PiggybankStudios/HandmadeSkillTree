/*
File:   main_tree_layout.cpp
Author: Taylor Robbins
Date:   08\19\2024
Description: 
	** Holds functions that lay out the tree nodes in a 2D space according to various algorithms
*/

void LayoutTreeNodes(Tree_t* tree, NodePlacementAlg_t algorithm, bool animate = false)
{
	NotNull(tree);
	switch (algorithm)
	{
		// +==============================+
		// |   NodePlacementAlg_Simple    |
		// +==============================+
		case NodePlacementAlg_Simple:
		{
			v2 currentPos = NewVec2(40, 40);
			r32 rowHeight = 0;
			VarArrayLoop(&tree->nodes, nIndex)
			{
				VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
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
			CalculateTotalDepsForAll(tree);
			
			u64 maxTotalDeps = 0;
			u64 numUnconnectedNodes = 0;
			VarArrayLoop(&tree->nodes, nIndex)
			{
				VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
				maxTotalDeps = MaxU64(maxTotalDeps, node->totalDepsCount);
				if (node->connections.length == 0 && node->numDependents == 0) { numUnconnectedNodes++; }
			}
			
			u64 numRows = maxTotalDeps+1;
			r32* rowHeights = AllocArray(scratch, r32, numRows);
			MyMemSet(rowHeights, 0x00, sizeof(r32) * numRows);
			VarArrayLoop(&tree->nodes, nIndex)
			{
				VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
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
			VarArrayLoop(&tree->nodes, nIndex)
			{
				VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
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
			
			VarArrayLoop(&tree->nodes, nIndex)
			{
				VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
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
