/*
File:   main_state.cpp
Author: Taylor Robbins
Date:   07\02\2024
Description: 
	** We don't use app states in this tool, so this is the main and only app state
*/

#include "ui/ui_scroll_view.cpp"
#include "ui/ui_divider.cpp"

#include "main_menu/main_helpers.cpp"

void MainCreateNodes()
{
	if (main->nodes.allocArena == nullptr)
	{
		CreateVarArray(&main->nodes, mainHeap, sizeof(SkillNode_t));
	}
	else	
	{
		VarArrayLoop(&main->nodes, nIndex)
		{
			VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
			FreeSkillNode(node);
		}
	}
	VarArrayClear(&main->nodes);
	main->nextNodeId = 1;
	
	#define AddNode(name, displayName) u64 name; do                  \
	{                                                                \
		SkillNode_t* name##Pntr = AddSkillNode(NewStr(displayName)); \
		name = name##Pntr->id;                                       \
	} while(0)
	#define AddNodeEx(name, displayName, ...) u64 name; do                                               \
	{                                                                                                    \
		u64 name##Deps[] = { __VA_ARGS__ };                                                              \
		SkillNode_t* name##Pntr = AddSkillNode(NewStr(displayName), ArrayCount(name##Deps), name##Deps); \
		name = name##Pntr->id;                                                                           \
	} while(0)
	
	AddNode(nodeLlvm, "LLVM");
	AddNode(nodeCpp, "C/C++");
	AddNode(nodeGo, "Go");
	AddNode(nodeLua, "Lua");
	AddNode(nodeMp3, "mp3");
	AddNode(nodePython, "Python");
	AddNodeEx(nodeRust, "Rust", nodeLlvm);
	AddNodeEx(nodeOdin, "Odin", nodeLlvm);
	AddNodeEx(nodeZig, "Zig", nodeLlvm);
	AddNodeEx(nodeJai, "Jai", nodeLlvm);
	AddNodeEx(nodeCakeLisp, "CakeLisp", nodeLlvm);
	AddNodeEx(nodeC3, "C3", nodeLlvm);
	AddNode(nodeWin32, "Win32");
	AddNode(nodeWinAudio, "WinAudio");
	AddNode(nodeGlfw, "GLFW");
	AddNode(nodeWasm, "Web Assembly (WASM)");
	AddNodeEx(nodeImgui, "Dear ImGui", nodeCpp);
	AddNodeEx(nodeSdl, "SDL", nodeCpp);
	AddNodeEx(nodeUnityBuilds, "Unity/Batch Building", nodeCpp);
	AddNode(nodeOpenGL, "OpenGL");
	AddNode(nodeVulkan, "Vulkan");
	AddNode(nodeDirectX, "DirectX");
	AddNode(nodeMetal, "Metal");
	AddNode(nodeWebGL, "WebGL/WebGPU");
	AddNodeEx(nodeZigBuild, "Zig Build", nodeZig);
	AddNode(nodeSoftwareRasterizing, "Software Rasterizing");
	AddNodeEx(nodeStbRectPack, "Stb RectPack", nodeCpp);
	AddNodeEx(nodeStbTrueType, "Stb TrueType", nodeStbRectPack, nodeCpp);
	AddNodeEx(nodeStbImage, "Stb Image", nodeCpp);
	AddNodeEx(nodeHandmadeHero, "Handmade Hero", nodeCpp, nodeWin32, nodeStbTrueType, nodeStbImage, nodeSoftwareRasterizing, nodeWinAudio);
	AddNodeEx(nodePigEngine, "Pig Engine", nodeCpp, nodeWin32, nodeGlfw, nodeOpenGL, nodeStbTrueType, nodeStbImage);
	AddNodeEx(nodeMsfGif, "msf_gif.h", nodeCpp);
	AddNodeEx(node4Coder, "4Coder", nodeCpp);
	AddNodeEx(nodeSpall, "Spall", nodeWasm);
	AddNodeEx(nodeRemedyBG, "RemedyBG", nodeCpp, nodeImgui);
	AddNodeEx(nodeDiskVoyager, "Disk Voyager", nodeCpp);
	AddNodeEx(nodeWhiteBox, "WhiteBox", nodeCpp, nodeImgui);
	AddNode(nodeBEdit, "BEdit");
	AddNode(nodeCactusImageViewer, "Cactus Image Viewer");
	AddNodeEx(nodeOrca, "Orca", nodeCpp, nodeWasm);
	AddNodeEx(nodeRaylib, "raylib", nodeCpp);
	AddNode(nodeDarke, "Darke");
	AddNode(nodeCuik, "CiukC");
	AddNode(nodeEssence, "Essence");
	AddNode(nodeWcap, "wcap");
	AddNode(nodeAvRacer, "AV-Racer");
	AddNode(nodeHappenlance, "Happenlance");
	AddNode(nodeSilverNode, "SilverNode");
	AddNodeEx(nodeHandmadeHeroNotes, "Handmade Hero Notes", nodeHandmadeHero);
	AddNodeEx(nodeNCine, "nCine", nodeCpp, nodeLua);
	AddNodeEx(nodeInsobot, "insobot", nodeCpp);
	AddNode(nodeRiscyBusiness, "Riscy Business");
	AddNode(nodeMotionBox, "Motion Box");
	AddNode(nodeTodool, "Todool");
	AddNode(nodeBitwise, "Bitwise");
	AddNode(nodeMilton, "Milton");
	AddNode(nodeCuteExplorer, "Cute Explorer");
	AddNode(nodeMetadesk, "Metadesk");
	AddNodeEx(nodeMPlay3, "MPlay3", nodeMp3);
	AddNode(nodeMetaAssemblyLanguage, "Meta Assembly Language");
	AddNode(nodePepperEditor, "Pepper Editor");
	AddNode(nodeSyzygy, "Syzygy");
	AddNode(nodeFeohTheFitter, "Feoh the Fitter");
	AddNode(nodeCyborgEarthworm, "Cyborg Earthworm");
	AddNode(nodeEscher, "Escher");
	AddNode(nodeMiniGlut, "miniglut");
	AddNode(nodeLightTracerRender, "Light Tracer Render");
	AddNode(nodeMiniAudio, "miniaudio");
	AddNode(nodeBreakArcadeGamesOut, "Break Arcade Games Out");
	AddNode(nodeTilengine, "Tilengine");
	AddNode(nodeIndigrid, "Indigrid");
	AddNode(nodeLibrg, "librg");
	AddNode(nodeHandmadeMath, "Handmade Math");
	AddNode(nodeGoxel, "Goxel");
	AddNodeEx(nodeGbLibraries, "GB Libraries", nodeCpp);
	AddNode(nodeSoLoud, "SoLoud Audio Engine");
	
	CalculateAllDependents();
}

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
		
		MainCreateNodes();
		main->placementAlg = NodePlacementAlg_DepsRows;
		main->doPlacement = true;
		main->cameraPos = Vec2_Zero;
		main->cameraGoto = main->cameraPos;
		main->moveCameraToCenter = true;
		
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
		VarArrayLoop(&main->nodes, nIndex)
		{
			VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
			FreeSkillNode(node);
		}
		FreeVarArray(&main->nodes);
		ClearPointer(main);
	}
}

// +--------------------------------------------------------------+
// |                   Layout and CaptureMouse                    |
// +--------------------------------------------------------------+
void LayoutMainAppState()
{
	RcBindFont(&pig->resources.fonts->debug, SelectDefaultFontFace());
	main->nodeBounds = Rec_Zero;
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		node->mainRec = NewRecCentered(node->currentPos, NewVec2(NODE_WIDTH, NODE_HEIGHT));
		node->fillRec = NewRec(0, 0, node->mainRec.size);
		node->nameMeasure = RcMeasureText(node->name, node->mainRec.width + NODE_MAX_NAME_WIDTH_SLOP);
		node->namePos = NewVec2(
			node->mainRec.width/2,
			-NODE_NAME_MARGIN - node->nameMeasure.size.height + node->nameMeasure.offset.y
		);
		if (main->nodeBounds == Rec_Zero) { main->nodeBounds = node->mainRec; }
		else { main->nodeBounds = RecBoth(main->nodeBounds, node->mainRec); }
	}
}

void CaptureMouseMainAppState()
{
	main->isNodeHovered = false;
	v2 nodesOffset = ScreenSize/2 - main->cameraPos;
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		if (node->nodeIsBeingDragged)
		{
			MouseHitPrint("SkillNode%llu", node->id);
			main->isNodeHovered = true;
			main->hoveredNodeId = node->id;
		}
	}
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		rec mainRec = node->mainRec + nodesOffset;
		if (MouseHitRecPrint(mainRec, "SkillNode%llu", node->id))
		{
			main->isNodeHovered = true;
			main->hoveredNodeId = node->id;
		}
	}
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
		PlaceSkillNodes(main->placementAlg);
		LayoutMainAppState();
		main->doPlacement = false;
	}
	
	// +==============================+
	// |      WASD Moves Camera       |
	// +==============================+
	r32 cameraSpeed = (r32)(KeyDownRaw(Key_Shift) ? CAMERA_MOVE_SPEED_FAST : CAMERA_MOVE_SPEED_SLOW);
	if (KeyDown(Key_W)) { HandleKey(Key_W); main->cameraGoto.y -= cameraSpeed * (r32)TimeScale; }
	if (KeyDown(Key_A)) { HandleKey(Key_A); main->cameraGoto.x -= cameraSpeed * (r32)TimeScale; }
	if (KeyDown(Key_S)) { HandleKey(Key_S); main->cameraGoto.y += cameraSpeed * (r32)TimeScale; }
	if (KeyDown(Key_D)) { HandleKey(Key_D); main->cameraGoto.x += cameraSpeed * (r32)TimeScale; }
	
	// +==============================+
	// |  Handle moveCameraToCenter   |
	// +==============================+
	if (main->moveCameraToCenter)
	{
		main->cameraGoto = main->nodeBounds.topLeft + main->nodeBounds.size/2;
		main->cameraPos = main->cameraGoto;
		main->moveCameraToCenter = false;
	}
	
	// +==============================+
	// |  Middle Mouse Moves Camera   |
	// +==============================+
	if (main->isMiddleDragging)
	{
		HandleMouse(MouseBtn_Middle);
		main->cameraGoto = main->middleMouseDragPos - MousePos + ScreenSize/2;
		main->cameraPos = main->cameraPos;
		if (!MouseDownRaw(MouseBtn_Middle)) { main->isMiddleDragging = false; }
	}
	else if (MousePressed(MouseBtn_Middle))
	{
		HandleMouse(MouseBtn_Middle);
		main->isMiddleDragging = true;
		main->middleMouseDragPos = main->cameraPos - ScreenSize/2 + MousePos;
	}
	
	// +==============================+
	// |        Update Camera         |
	// +==============================+
	v2 cameraDelta = main->cameraGoto - main->cameraPos;
	if (Vec2LengthSquared(cameraDelta) > 1) { main->cameraPos += cameraDelta / CAMERA_LAG; }
	else { main->cameraPos = main->cameraGoto; }
	main->cameraGoto.x = MinR32(main->cameraGoto.x, main->nodeBounds.x + main->nodeBounds.width + ScreenSize.width/2 - CAMERA_LIMIT_BORDER_THICKNESS);
	main->cameraGoto.x = MaxR32(main->cameraGoto.x, main->nodeBounds.x - ScreenSize.width/2 + CAMERA_LIMIT_BORDER_THICKNESS);
	main->cameraGoto.y = MinR32(main->cameraGoto.y, main->nodeBounds.y + main->nodeBounds.height + ScreenSize.height/2 - CAMERA_LIMIT_BORDER_THICKNESS);
	main->cameraGoto.y = MaxR32(main->cameraGoto.y, main->nodeBounds.y - ScreenSize.height/2 + CAMERA_LIMIT_BORDER_THICKNESS);
	
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
		PlaceSkillNodes(main->placementAlg, true);
	}
	// +==============================+
	// |     F6 Regenerates Nodes     |
	// +==============================+
	if (KeyPressed(Key_F6))
	{
		HandleKeyExtended(Key_F6);
		MainCreateNodes();
		PlaceSkillNodes(main->placementAlg, false);
		LayoutMainAppState();
		PlaceSkillNodes(main->placementAlg, true);
	}
	
	// +==============================+
	// |      Update Skill Nodes      |
	// +==============================+
	v2 nodesOffset = ScreenSize/2 - main->cameraPos;
	v2 mouseWorldPos = MousePos - ScreenSize/2 + main->cameraPos;
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		bool isHovered = IsMouseOverPrint("SkillNode%llu", node->id);
		
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
	// |      Render Skill Nodes      |
	// +==============================+
	v2 nodesOffset = ScreenSize/2 - main->cameraPos;
	//non-highlighted connections
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		v2 nodePos = node->currentPos + nodesOffset;
		VarArrayLoop(&node->connections, cIndex)
		{
			VarArrayLoopGet(SkillNodeConn_t, connection, &node->connections, cIndex);
			if (!IsConnectionHoverConnected(connection))
			{
				SkillNode_t* upNode = FindSkillNodeById(connection->upId);
				v2 upNodePos = upNode->currentPos + nodesOffset;
				RcDrawLine(nodePos, upNodePos, CONNECTION_THICKNESS, CONNECTION_COLOR);
			}
		}
	}
	//highlighted connections
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		v2 nodePos = node->currentPos + nodesOffset;
		VarArrayLoop(&node->connections, cIndex)
		{
			VarArrayLoopGet(SkillNodeConn_t, connection, &node->connections, cIndex);
			if (IsConnectionHoverConnected(connection))
			{
				SkillNode_t* upNode = FindSkillNodeById(connection->upId);
				v2 upNodePos = upNode->currentPos + nodesOffset;
				bool eitherNodeHovered = IsConnectionHoverConnected(connection);
				Color_t connectionColor = eitherNodeHovered ? CONNECTION_HIGHLIGHT_COLOR : CONNECTION_COLOR;
				RcDrawLine(nodePos, upNodePos, CONNECTION_THICKNESS, CONNECTION_HIGHLIGHT_COLOR);
			}
		}
	}
	//shadows
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
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
			VarArrayLoopGet(SkillNodeConn_t, connection, &node->connections, cIndex);
			SkillNode_t* upNode = FindSkillNodeById(connection->upId);
			v2 upNodePos = upNode->currentPos + nodesOffset;
			v2 upNodeDirVec = Vec2Normalize(upNodePos - nodePos);
			bool eitherNodeHovered = IsConnectionHoverConnected(connection);
			Color_t connectionColor = eitherNodeHovered ? CONNECTION_HIGHLIGHT_COLOR : CONNECTION_COLOR;
			RcDrawLine(nodePos, nodePos + upNodeDirVec * Vec2Length(shadowRec.size/2), CONNECTION_THICKNESS, connectionColor);
		}
		VarArrayLoop(&main->nodes, nIndex2)
		{
			VarArrayLoopGet(SkillNode_t, node2, &main->nodes, nIndex2);
			if (node->id != node2->id)
			{
				v2 node2Pos = node2->currentPos + nodesOffset;
				VarArrayLoop(&node2->connections, cIndex)
				{
					VarArrayLoopGet(SkillNodeConn_t, connection, &node2->connections, cIndex);
					if (connection->upId == node->id)
					{
						v2 downNodeDirVec = Vec2Normalize(node2Pos - nodePos);
						bool eitherNodeHovered = IsConnectionHoverConnected(connection);
						Color_t connectionColor = eitherNodeHovered ? CONNECTION_HIGHLIGHT_COLOR : CONNECTION_COLOR;
						RcDrawLine(nodePos, nodePos + downNodeDirVec * Vec2Length(shadowRec.size/2), CONNECTION_THICKNESS, connectionColor);
					}
				}
			}
		}
	}
	//nodes
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		rec mainRec = node->mainRec + nodesOffset;
		bool isSelected = (main->isNodeSelected && node->id == main->selectedNodeId);
		bool isHovered = IsMouseOverPrint("SkillNode%llu", node->id);
		Color_t nodeColor = GetPredefPalColorByIndex(nIndex);
		Color_t borderColor = nodeColor;
		if (isSelected) { borderColor = MonokaiGreen; }
		else if (isHovered) { borderColor = MonokaiWhite; }
		RcDrawRoundedRectangleWithBorder(mainRec, NODE_CORNER_RADIUS, NODE_BORDER_THICKNESS, nodeColor, borderColor);
	}
	//names
	RcBindFont(&pig->resources.fonts->debug, SelectDefaultFontFace());
	VarArrayLoop(&main->nodes, nIndex)
	{
		VarArrayLoopGet(SkillNode_t, node, &main->nodes, nIndex);
		rec mainRec = node->mainRec + nodesOffset;
		RcDrawText(node->name, node->namePos + mainRec.topLeft, NODE_NAME_COLOR, TextAlignment_Center, mainRec.width + NODE_MAX_NAME_WIDTH_SLOP);
	}
	
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
