/*
File:   main_tree.cpp
Author: Taylor Robbins
Date:   08\11\2024
Description: 
	** Holds the MainCreateNodes function which fills out a test tree of SkillNodes
*/

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

