/*
File:   main_state.cpp
Author: Taylor Robbins
Date:   07\02\2024
Description: 
	** We don't use app states in this tool, so this is the main and only app state
*/

#include "ui/ui_scroll_view.cpp"
#include "ui/ui_divider.cpp"

#include "main/main_helpers.cpp"
#include "main/main_markdown.cpp"
#include "main/main_tree.cpp"
#include "main/main_tree_layout.cpp"

// +--------------------------------------------------------------+
// |                       Access Resources                       |
// +--------------------------------------------------------------+
void MainAppStateAccessResources()
{
}

// +--------------------------------------------------------------+
// |                            Start                             |
// +--------------------------------------------------------------+
void StartMainAppState(AppState_t oldAppState, bool initialize)
{
	UNUSED(oldAppState);
	NotNull(main);
	MemArena_t* scratch = GetScratchArena();
	
	if (initialize)
	{
		ClearPointer(main);
		
		InitTree(&main->tree, mainHeap);
		CreateTestTreeNodes(&main->tree);
		main->placementAlg = NodePlacementAlg_DepsRows;
		main->doPlacement = true;
		main->cameraPos = Vec2_Zero;
		main->cameraGoto = main->cameraPos;
		main->moveCameraToCenter = true;
		
		InitUiDivider(&main->rightDivider, true, true, 0.75f);
		InitUiDivider(&main->leftDivider, true, true, 0.25f);
		
		main->initialized = true;
	}
	
	MainAppStateAccessResources();
	
	FreeScratchArena(scratch);
}

// +--------------------------------------------------------------+
// |                             Stop                             |
// +--------------------------------------------------------------+
void StopMainAppState(AppState_t newAppState, bool deinitialize, bool shuttingDown)
{
	UNUSED(newAppState);
	NotNull(main);
	
	if (deinitialize && !shuttingDown)
	{
		FreeTree(&main->tree);
		ClearPointer(main);
	}
}

// +--------------------------------------------------------------+
// |                   Layout and CaptureMouse                    |
// +--------------------------------------------------------------+
void LayoutMainAppState()
{
	RcBindFont(&pig->resources.fonts->debug, SelectDefaultFontFace());
	
	main->rightDivider.mainRec = ScreenRec;
	LayoutUiDivider(&main->rightDivider);
	main->leftDivider.mainRec = ScreenRec;
	LayoutUiDivider(&main->leftDivider);
	
	if (main->sidebarAnim < 1.0f)
	{
		main->viewportRec = RecLerp(ScreenRec, main->rightDivider.leftRec, main->sidebarAnim);
		RecAlign(&main->viewportRec);
		main->sidebarRec = RecLerp(NewRec(ScreenSize.width, 0, 0, ScreenSize.height), main->rightDivider.rightRec, main->sidebarAnim);
		RecAlign(&main->sidebarRec);
		main->infoRec = NewRec(ScreenSize.width, 0, 0, ScreenSize.height);
		RecAlign(&main->infoRec);
	}
	else 
	{
		main->viewportRec = RecLerp(main->rightDivider.leftRec, NewRec(0, 0, 0, ScreenSize.height), main->transitionAnim);
		RecAlign(&main->viewportRec);
		main->sidebarRec = RecLerp(main->rightDivider.rightRec, main->leftDivider.leftRec, main->transitionAnim);
		RecAlign(&main->sidebarRec);
		main->infoRec = RecLerp(NewRec(ScreenSize.width, 0, 0, ScreenSize.height), main->leftDivider.rightRec, main->transitionAnim);
		RecAlign(&main->infoRec);
	}
	
	main->tree.nodeBounds = Rec_Zero;
	VarArrayLoop(&main->tree.nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &main->tree.nodes, nIndex);
		node->mainRec = NewRecCentered(node->currentPos, NewVec2(NODE_WIDTH, NODE_HEIGHT));
		node->fillRec = NewRec(0, 0, node->mainRec.size);
		node->nameMeasure = RcMeasureText(node->name, node->mainRec.width + NODE_MAX_NAME_WIDTH_SLOP);
		node->namePos = NewVec2(
			node->mainRec.width/2,
			-NODE_NAME_MARGIN - node->nameMeasure.size.height + node->nameMeasure.offset.y
		);
		if (main->tree.nodeBounds == Rec_Zero) { main->tree.nodeBounds = node->mainRec; }
		else { main->tree.nodeBounds = RecBoth(main->tree.nodeBounds, node->mainRec); }
	}
}

void CaptureMouseMainAppState()
{
	main->isNodeHovered = false;
	if (IsMouseInsideRec(main->viewportRec))
	{
		v2 nodesOffset = main->viewportRec.topLeft + main->viewportRec.size/2 - main->cameraPos;
		VarArrayLoop(&main->tree.nodes, nIndex)
		{
			VarArrayLoopGet(TreeNode_t, node, &main->tree.nodes, nIndex);
			if (node->nodeIsBeingDragged)
			{
				MouseHitPrint("ViewportTreeNode%llu", node->id);
				main->isNodeHovered = true;
				main->hoveredNodeId = node->id;
			}
		}
		for (u64 nIndex = main->tree.nodes.length; nIndex > 0; nIndex--)
		{
			VarArrayLoopGet(TreeNode_t, node, &main->tree.nodes, nIndex-1);
			rec mainRec = node->mainRec + nodesOffset;
			if (MouseHitRecPrint(mainRec, "ViewportTreeNode%llu", node->id))
			{
				main->isNodeHovered = true;
				main->hoveredNodeId = node->id;
			}
		}
		MouseHitNamed("Viewport");
	}
	MouseHitRecNamed(main->sidebarRec, "Sidebar");
	MouseHitRecNamed(main->infoRec, "Info");
}

// +--------------------------------------------------------------+
// |                            Update                            |
// +--------------------------------------------------------------+
void UpdateMainAppState()
{
	NotNull(main);
	MemArena_t* scratch = GetScratchArena();
	MainAppStateAccessResources();
	LayoutMainAppState();
	if (main->doPlacement)
	{
		LayoutTreeNodes(&main->tree, main->placementAlg);
		LayoutMainAppState();
		main->doPlacement = false;
	}
	
	// +==============================+
	// |     I Toggles Info Panel     |
	// +==============================+
	if (KeyPressed(Key_I))
	{
		HandleKeyExtended(Key_I);
		main->isInfoOpened = !main->isInfoOpened;
	}
	
	// +==============================+
	// |  Update Divider Animations   |
	// +==============================+
	if (main->isInfoOpened)
	{
		UpdateAnimationUp(&main->transitionAnim, TRANSITION_ANIM_TIME);
	}
	else
	{
		UpdateAnimationDown(&main->transitionAnim, TRANSITION_ANIM_TIME);
	}
	if (main->isNodeSelected)
	{
		UpdateAnimationUp(&main->sidebarAnim, SIDEBAR_ANIM_TIME);
	}
	else
	{
		UpdateAnimationDown(&main->sidebarAnim, SIDEBAR_ANIM_TIME);
	}
	
	// +==============================+
	// |      WASD Moves Camera       |
	// +==============================+
	if (!main->isNodeSelected || !main->isInfoOpened)
	{
		r32 cameraSpeed = (r32)(KeyDownRaw(Key_Shift) ? CAMERA_MOVE_SPEED_FAST : CAMERA_MOVE_SPEED_SLOW);
		if (KeyDown(Key_W)) { HandleKey(Key_W); main->cameraGoto.y -= cameraSpeed * (r32)TimeScale; }
		if (KeyDown(Key_A)) { HandleKey(Key_A); main->cameraGoto.x -= cameraSpeed * (r32)TimeScale; }
		if (KeyDown(Key_S)) { HandleKey(Key_S); main->cameraGoto.y += cameraSpeed * (r32)TimeScale; }
		if (KeyDown(Key_D)) { HandleKey(Key_D); main->cameraGoto.x += cameraSpeed * (r32)TimeScale; }
	}
	
	// +==============================+
	// |  Handle moveCameraToCenter   |
	// +==============================+
	if (main->moveCameraToCenter)
	{
		main->cameraGoto = main->tree.nodeBounds.topLeft + main->tree.nodeBounds.size/2;
		main->cameraPos = main->cameraGoto;
		main->moveCameraToCenter = false;
	}
	
	// +==============================+
	// |  Middle Mouse Moves Camera   |
	// +==============================+
	if (main->isMiddleDragging)
	{
		HandleMouse(MouseBtn_Middle);
		main->cameraGoto = main->middleMouseDragPos - (MousePos - main->viewportRec.topLeft) + main->viewportRec.size/2;
		main->cameraPos = main->cameraPos;
		if (!MouseDownRaw(MouseBtn_Middle)) { main->isMiddleDragging = false; }
	}
	else if (MousePressed(MouseBtn_Middle) && IsMouseInsideRec(main->viewportRec))
	{
		HandleMouse(MouseBtn_Middle);
		main->isMiddleDragging = true;
		main->middleMouseDragPos = main->cameraPos - main->viewportRec.size/2 + (MousePos - main->viewportRec.topLeft);
	}
	
	// +==============================+
	// |        Update Camera         |
	// +==============================+
	v2 cameraDelta = main->cameraGoto - main->cameraPos;
	if (Vec2LengthSquared(cameraDelta) > 1) { main->cameraPos += cameraDelta / CAMERA_LAG; }
	else { main->cameraPos = main->cameraGoto; }
	v2 viewportSize = main->viewportRec.size;
	main->cameraGoto.x = MinR32(main->cameraGoto.x, main->tree.nodeBounds.x + main->tree.nodeBounds.width + viewportSize.width/2 - CAMERA_LIMIT_BORDER_THICKNESS);
	main->cameraGoto.x = MaxR32(main->cameraGoto.x, main->tree.nodeBounds.x - viewportSize.width/2 + CAMERA_LIMIT_BORDER_THICKNESS);
	main->cameraGoto.y = MinR32(main->cameraGoto.y, main->tree.nodeBounds.y + main->tree.nodeBounds.height + viewportSize.height/2 - CAMERA_LIMIT_BORDER_THICKNESS);
	main->cameraGoto.y = MaxR32(main->cameraGoto.y, main->tree.nodeBounds.y - viewportSize.height/2 + CAMERA_LIMIT_BORDER_THICKNESS);
	
	// +====================================+
	// | Capture Mouse After View Movement  |
	// +====================================+
	CaptureMouseMainAppState();
	
	// +===============================+
	// | P Changes Placement Algorithm |
	// +===============================+
	if (KeyPressed(Key_P))
	{
		HandleKeyExtended(Key_P);
		main->placementAlg = (NodePlacementAlg_t)(((u64)main->placementAlg + 1) % NodePlacementAlg_NumAlgs);
		if (main->placementAlg == NodePlacementAlg_None) { main->placementAlg = (NodePlacementAlg_t)(((u64)main->placementAlg + 1) % NodePlacementAlg_NumAlgs); }
		PrintLine_D("Placement algorithm = %s", GetNodePlacementAlgStr(main->placementAlg));
	}
	
	// +==============================+
	// |    F5 Triggers Placement     |
	// +==============================+
	if (KeyPressed(Key_F5))
	{
		HandleKeyExtended(Key_F5);
		LayoutTreeNodes(&main->tree, main->placementAlg, true);
	}
	// +==============================+
	// |     F6 Regenerates Nodes     |
	// +==============================+
	if (KeyPressed(Key_F6))
	{
		HandleKeyExtended(Key_F6);
		CreateTestTreeNodes(&main->tree);
		LayoutTreeNodes(&main->tree, main->placementAlg, false);
		LayoutMainAppState();
		LayoutTreeNodes(&main->tree, main->placementAlg, true);
	}
	
	// +==============================+
	// |      Update Tree Nodes       |
	// +==============================+
	if (!main->isNodeSelected || !main->isInfoOpened)
	{
		v2 nodesOffset = main->viewportRec.topLeft + main->viewportRec.size/2 - main->cameraPos;
		v2 mouseWorldPos = (MousePos - main->viewportRec.topLeft) - main->viewportRec.size/2 + main->cameraPos;
		VarArrayLoop(&main->tree.nodes, nIndex)
		{
			VarArrayLoopGet(TreeNode_t, node, &main->tree.nodes, nIndex);
			bool isHovered = IsMouseOverPrint("ViewportTreeNode%llu", node->id);
			
			v2 offset = node->targetPos - node->currentPos;
			if (Vec2LengthSquared(offset) > 1) { node->currentPos += offset / NODE_MOVE_LAG; }
			else { node->currentPos = node->targetPos; }
			
			if (MousePressedRaw(MouseBtn_Left)) { node->leftClickStartedInside = false; }
			
			if (isHovered)
			{
				if (MousePressed(MouseBtn_Left))
				{
					HandleMouse(MouseBtn_Left);
					node->leftClickStartedInside = true;
					node->leftClickOffset = mouseWorldPos - node->mainRec.topLeft;
				}
				if (MouseReleased(MouseBtn_Left) && node->leftClickStartedInside && !node->nodeIsBeingDragged)
				{
					HandleMouse(MouseBtn_Left);
					main->isNodeSelected = true;
					main->selectedNodeId = node->id;
				}
			}
			if (MouseDownRaw(MouseBtn_Left) && node->leftClickStartedInside && !node->nodeIsBeingDragged)
			{
				v2 moveOffset = mouseWorldPos - (node->mainRec.topLeft + node->leftClickOffset);
				if (Vec2LengthSquared(moveOffset) >= Square(NODE_MOVE_START_DIST))
				{
					node->nodeIsBeingDragged = true;
				}
			}
			if (node->nodeIsBeingDragged)
			{
				HandleMouse(MouseBtn_Left);
				node->targetPos = mouseWorldPos - node->leftClickOffset + node->mainRec.size/2;
				if (MouseReleasedRaw(MouseBtn_Left)) { node->nodeIsBeingDragged = false; }
			}
		}
	}
	
	// +======================================+
	// | Left Click in Empty Space Deselects  |
	// +======================================+
	if (IsMouseOverNamed("Viewport"))
	{
		if (MouseDown(MouseBtn_Left)) { HandleMouse(MouseBtn_Left); }
		else if (MouseReleased(MouseBtn_Left))
		{
			HandleMouse(MouseBtn_Left);
			main->isNodeSelected = false;
		}
	}
	
	FreeScratchArena(scratch);
}

// +--------------------------------------------------------------+
// |                            Render                            |
// +--------------------------------------------------------------+
void RenderMainAppState(FrameBuffer_t* renderBuffer, bool bottomLayer)
{
	UNUSED(bottomLayer);
	NotNull(main);
	MemArena_t* scratch = GetScratchArena();
	MainAppStateAccessResources();
	LayoutMainAppState();
	
	RcBegin(pig->currentWindow, renderBuffer, &pig->resources.shaders->main2D, BACKGROUND_COLOR);
	
	// +==============================+
	// |      Render Tree Nodes       |
	// +==============================+
	RcSetViewport(main->viewportRec);
	v2 nodesOffset = main->viewportRec.topLeft + main->viewportRec.size/2 - main->cameraPos;
	//non-highlighted connections
	VarArrayLoop(&main->tree.nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &main->tree.nodes, nIndex);
		v2 nodePos = node->currentPos + nodesOffset;
		VarArrayLoop(&node->connections, cIndex)
		{
			VarArrayLoopGet(TreeNodeConn_t, connection, &node->connections, cIndex);
			if (!IsConnectionHoverConnected(connection) && !IsConnectionSelectionConnected(connection))
			{
				TreeNode_t* upNode = FindTreeNodeById(&main->tree, connection->upId);
				v2 upNodePos = upNode->currentPos + nodesOffset;
				RcDrawLine(nodePos, upNodePos, CONNECTION_THICKNESS, CONNECTION_COLOR);
			}
		}
	}
	//highlighted connections
	VarArrayLoop(&main->tree.nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &main->tree.nodes, nIndex);
		v2 nodePos = node->currentPos + nodesOffset;
		VarArrayLoop(&node->connections, cIndex)
		{
			VarArrayLoopGet(TreeNodeConn_t, connection, &node->connections, cIndex);
			if (IsConnectionHoverConnected(connection) || IsConnectionSelectionConnected(connection))
			{
				TreeNode_t* upNode = FindTreeNodeById(&main->tree, connection->upId);
				v2 upNodePos = upNode->currentPos + nodesOffset;
				Color_t connectionColor = IsConnectionSelectionConnected(connection) ? CONNECTION_SELECTED_COLOR : CONNECTION_HIGHLIGHT_COLOR;
				RcDrawLine(nodePos, upNodePos, CONNECTION_THICKNESS, connectionColor);
			}
		}
	}
	//shadows
	VarArrayLoop(&main->tree.nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &main->tree.nodes, nIndex);
		v2 nodePos = node->currentPos + nodesOffset;
		rec mainRec = node->mainRec + nodesOffset;
		rec shadowRec = RecInflate(mainRec, NODE_SHADOW_SIZE, NODE_SHADOW_SIZE);
		r32 shadowIncrement = NODE_SHADOW_SIZE / 8;
		RcDrawRectangle(RecDeflate(shadowRec, 0*shadowIncrement, 0*shadowIncrement), ColorTransparent(BACKGROUND_COLOR, 0.25f));
		RcDrawRectangle(RecDeflate(shadowRec, 1*shadowIncrement, 1*shadowIncrement), ColorTransparent(BACKGROUND_COLOR, 0.25f));
		RcDrawRectangle(RecDeflate(shadowRec, 2*shadowIncrement, 2*shadowIncrement), ColorTransparent(BACKGROUND_COLOR, 0.25f));
		RcDrawRectangle(RecDeflate(shadowRec, 3*shadowIncrement, 3*shadowIncrement), ColorTransparent(BACKGROUND_COLOR, 0.25f));
		RcDrawRectangle(RecDeflate(shadowRec, 4*shadowIncrement, 4*shadowIncrement), ColorTransparent(BACKGROUND_COLOR, 0.25f));
		RcDrawRectangle(RecDeflate(shadowRec, 5*shadowIncrement, 5*shadowIncrement), ColorTransparent(BACKGROUND_COLOR, 0.25f));
		RcDrawRectangle(RecDeflate(shadowRec, 6*shadowIncrement, 6*shadowIncrement), ColorTransparent(BACKGROUND_COLOR, 0.25f));
		RcDrawRectangle(RecDeflate(shadowRec, 7*shadowIncrement, 7*shadowIncrement), ColorTransparent(BACKGROUND_COLOR, 0.25f));
		//Since we drew a shadow on top of all connections, we want to redraw a portion of our connection lines on top to make them look unshadowed
		VarArrayLoop(&node->connections, cIndex)
		{
			VarArrayLoopGet(TreeNodeConn_t, connection, &node->connections, cIndex);
			TreeNode_t* upNode = FindTreeNodeById(&main->tree, connection->upId);
			v2 upNodePos = upNode->currentPos + nodesOffset;
			v2 upNodeDirVec = Vec2Normalize(upNodePos - nodePos);
			bool eitherNodeHovered = IsConnectionHoverConnected(connection);
			bool eitherNodeSelected = IsConnectionSelectionConnected(connection);
			Color_t connectionColor = eitherNodeSelected ? CONNECTION_SELECTED_COLOR : (eitherNodeHovered ? CONNECTION_HIGHLIGHT_COLOR : CONNECTION_COLOR);
			RcDrawLine(nodePos, nodePos + upNodeDirVec * Vec2Length(shadowRec.size/2), CONNECTION_THICKNESS, connectionColor);
		}
		VarArrayLoop(&main->tree.nodes, nIndex2)
		{
			VarArrayLoopGet(TreeNode_t, node2, &main->tree.nodes, nIndex2);
			if (node->id != node2->id)
			{
				v2 node2Pos = node2->currentPos + nodesOffset;
				VarArrayLoop(&node2->connections, cIndex)
				{
					VarArrayLoopGet(TreeNodeConn_t, connection, &node2->connections, cIndex);
					if (connection->upId == node->id)
					{
						v2 downNodeDirVec = Vec2Normalize(node2Pos - nodePos);
						bool eitherNodeHovered = IsConnectionHoverConnected(connection);
						bool eitherNodeSelected = IsConnectionSelectionConnected(connection);
						Color_t connectionColor = eitherNodeSelected ? CONNECTION_SELECTED_COLOR : (eitherNodeHovered ? CONNECTION_HIGHLIGHT_COLOR : CONNECTION_COLOR);
						RcDrawLine(nodePos, nodePos + downNodeDirVec * Vec2Length(shadowRec.size/2), CONNECTION_THICKNESS, connectionColor);
					}
				}
			}
		}
	}
	//nodes
	VarArrayLoop(&main->tree.nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &main->tree.nodes, nIndex);
		rec mainRec = node->mainRec + nodesOffset;
		bool isSelected = (main->isNodeSelected && node->id == main->selectedNodeId);
		bool isHovered = IsMouseOverPrint("ViewportTreeNode%llu", node->id);
		Color_t nodeColor = GetPredefPalColorByIndex(nIndex);
		Color_t borderColor = nodeColor;
		if (isSelected) { borderColor = MonokaiGreen; }
		else if (isHovered) { borderColor = MonokaiWhite; }
		RcDrawRoundedRectangleWithBorder(mainRec, NODE_CORNER_RADIUS, NODE_BORDER_THICKNESS, nodeColor, borderColor);
	}
	//names
	RcBindFont(&pig->resources.fonts->debug, SelectDefaultFontFace());
	VarArrayLoop(&main->tree.nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &main->tree.nodes, nIndex);
		rec mainRec = node->mainRec + nodesOffset;
		RcDrawText(node->name, node->namePos + mainRec.topLeft, NODE_NAME_COLOR, TextAlignment_Center, mainRec.width + NODE_MAX_NAME_WIDTH_SLOP);
	}
	RcSetViewport(ScreenRec);
	
	// +==============================+
	// |        Render Sidebar        |
	// +==============================+
	RcDrawRectangle(main->sidebarRec, MonokaiGray2);
	
	// +==============================+
	// | Render Assertion Status Text |
	// +==============================+
	if (false)
	{
		RcBindFont(&pig->resources.fonts->debug, SelectDefaultFontFace());
		v2 textPos = NewVec2(10, ScreenSize.height - 10 - RcGetMaxDescend());
		Vec2Align(&textPos);
		
		if (pig->debugOverlay.enabled || DEVELOPER_BUILD)
		{
			if (GYLIB_ASSERTIONS_ENABLED)
			{
				RcDrawTextPrint(textPos, MonokaiWhite, "Assertions \b\fenabled\f\b %s \b\f%s\f\b", pig->dontExitOnAssert ? "but" : "and", pig->dontExitOnAssert ? "won't exit" : "will exit");
				textPos.y -= RcGetLineHeight();
			}
			else
			{
				RcDrawText("Assertions \b\fdisabled\f\b", textPos, MonokaiWhite);
				textPos.y -= RcGetLineHeight();
			}
		}
		if (DEVELOPER_BUILD)
		{
			RcDrawText("DEVELOPER_BUILD", textPos, MonokaiWhite);
			textPos.y -= RcGetLineHeight();
		}
		if (DEBUG_BUILD)
		{
			RcDrawText("DEBUG_BUILD", textPos, MonokaiWhite);
			textPos.y -= RcGetLineHeight();
		}
		if (DEMO_BUILD)
		{
			RcDrawText("DEMO_BUILD", textPos, MonokaiWhite);
			textPos.y -= RcGetLineHeight();
		}
		#if STEAM_BUILD
		{
			if (pig->debugOverlay.enabled || DEVELOPER_BUILD)
			{
				RcDrawTextPrint(textPos, MonokaiWhite, "STEAM_BUILD (%llu)", platInfo->steamAppId);
			}
			else
			{
				RcDrawText("STEAM_BUILD", textPos, MonokaiWhite);
			}
			textPos.y -= RcGetLineHeight();
		}
		#endif
		
		RcDrawTextPrint(textPos, MonokaiWhite, "Platform v%u.%02u(%03u)", platInfo->version.major, platInfo->version.minor, platInfo->version.build);
		textPos.y -= RcGetLineHeight();
		RcDrawTextPrint(textPos, MonokaiWhite, "Engine   v%llu.%02llu(%03llu)", (u64)ENGINE_VERSION_MAJOR, (u64)ENGINE_VERSION_MINOR, (u64)ENGINE_VERSION_BUILD);
		textPos.y -= RcGetLineHeight();
		if (pig->reloadIndex > 1)
		{
			RcDrawTextPrint(textPos, MonokaiWhite, "Game     v%llu.%02llu(%03llu) (reload %llu)", (u64)GAME_VERSION_MAJOR, (u64)GAME_VERSION_MINOR, (u64)GAME_VERSION_BUILD, pig->reloadIndex);
			textPos.y -= RcGetLineHeight();
		}
		else
		{
			RcDrawTextPrint(textPos, MonokaiWhite, "Game     v%llu.%02llu(%03llu)", (u64)GAME_VERSION_MAJOR, (u64)GAME_VERSION_MINOR, (u64)GAME_VERSION_BUILD);
			textPos.y -= RcGetLineHeight();
		}
		
		RcDrawTextPrint(textPos, MonokaiWhite, "Camera (%g, %g)", main->cameraPos.x, main->cameraPos.y);
		textPos.y -= RcGetLineHeight();
		RcDrawTextPrint(textPos, MonokaiWhite, "Selected: %s %llu", main->isNodeSelected ? "Yes" : "No", main->selectedNodeId);
		textPos.y -= RcGetLineHeight();
		RcDrawTextPrint(textPos, MonokaiWhite, "Hovered: %s %llu", main->isNodeHovered ? "Yes" : "No", main->hoveredNodeId);
		textPos.y -= RcGetLineHeight();
	}
	
	FreeScratchArena(scratch);
}
