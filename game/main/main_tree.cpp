/*
File:   main_tree.cpp
Author: Taylor Robbins
Date:   08\11\2024
Description: 
	** Holds all the functions that help us interact with Tree_t, TreeNode_t, etc.
	**
	** The "tree" is our main structure that backs the 2D visual that you explore
	** by panning around. Each node represents a project or library or etc. that
	** may or may not have some tasks that can help you get familiar with the project.
	** At a bare minimum each node has a name and some links to relavent sites.
	**
	** The tree is based on top of the MdFile_t abstraction, which helps us allow
	** the user to edit various bits of text directly in the UI, and each edit can
	** target a particular "piece" of the markdown file and only change that portion
*/

void FreeTreeNode(Tree_t* tree, TreeNode_t* node)
{
	NotNull3(tree, tree->allocArena, node);
	ClearPointer(node);
	FreeString(tree->allocArena, &node->name);
	FreeVarArray(&node->connections);
	ClearPointer(node);
}

void FreeTree(Tree_t* tree)
{
	NotNull(tree);
	if (tree->allocArena != nullptr)
	{
		VarArrayLoop(&tree->nodes, nIndex)
		{
			VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
			FreeTreeNode(tree, node);
		}
		FreeVarArray(&tree->nodes);
	}
	ClearPointer(tree);
}

void ClearTree(Tree_t* tree)
{
	NotNull(tree);
	VarArrayLoop(&tree->nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
		FreeTreeNode(tree, node);
	}
	VarArrayClear(&tree->nodes);
	tree->nextNodeId = 1;
}

void InitTree(Tree_t* tree, MemArena_t* memArena)
{
	NotNull2(tree, memArena);
	ClearPointer(tree);
	tree->allocArena = memArena;
	CreateVarArray(&tree->nodes, memArena, sizeof(TreeNode_t));
	tree->nextNodeId = 1;
}

TreeNode_t* FindTreeNodeById(Tree_t* tree, u64 id)
{
	NotNull(tree);
	VarArrayLoop(&tree->nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
		if (node->id == id) { return node; }
	}
	return nullptr;
}

TreeNode_t* AddTreeNode(Tree_t* tree, MyStr_t name, u64 numDependencies = 0, u64* dependencyIds = nullptr)
{
	NotNull2(tree, tree->allocArena);
	NotNullStr(&name);
	TreeNode_t* newNode = VarArrayAdd(&tree->nodes, TreeNode_t);
	NotNull(newNode);
	ClearPointer(newNode);
	newNode->id = tree->nextNodeId;
	tree->nextNodeId++;
	CreateVarArray(&newNode->connections, tree->allocArena, sizeof(TreeNodeConn_t));
	AssertIf(numDependencies > 0, dependencyIds != nullptr);
	for (u64 dIndex = 0; dIndex < numDependencies; dIndex++)
	{
		u64 dependencyId = dependencyIds[dIndex];
		TreeNodeConn_t* newConnection = VarArrayAdd(&newNode->connections, TreeNodeConn_t);
		NotNull(newConnection);
		ClearPointer(newConnection);
		newConnection->type = TreeNodeConnType_Dependency;
		newConnection->upId = dependencyId;
		newConnection->downId = newNode->id;
	}
	newNode->name = AllocString(tree->allocArena, &name);
	return newNode;
}

void CalculateAllDependents(Tree_t* tree)
{
	NotNull(tree);
	VarArrayLoop(&tree->nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
		node->numDependents = 0;
	}
	VarArrayLoop(&tree->nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
		VarArrayLoop(&node->connections, cIndex)
		{
			VarArrayLoopGet(TreeNodeConn_t, connection, &node->connections, cIndex);
			TreeNode_t* dependencyNode = FindTreeNodeById(tree, connection->upId);
			dependencyNode->numDependents++;
		}
	}
}

u64 CalculateTotalDepsForNode(Tree_t* tree, TreeNode_t* node)
{
	NotNull2(tree, node);
	
	if (node->totalDepsCount == 0 && node->connections.length > 0)
	{
		// We set the totalDepsCount to 1 here to make sure we don't find a loop in the graph that leads back to ourself and therefore recurse infinitely.
		// Loops in the graph are still bad for other reasons, but at least we won't crash
		node->totalDepsCount = 1;
		
		VarArrayLoop(&node->connections, cIndex)
		{
			VarArrayLoopGet(TreeNodeConn_t, connection, &node->connections, cIndex);
			Assert(connection->upId != node->id);
			if (connection->type == TreeNodeConnType_Dependency)
			{
				TreeNode_t* dependencyNode = FindTreeNodeById(tree, connection->upId);
				NotNull(dependencyNode);
				u64 dependencyTotalDepsCount = CalculateTotalDepsForNode(tree, dependencyNode);
				node->totalDepsCount = MaxU64(node->totalDepsCount, dependencyTotalDepsCount+1);
			}
		}
	}
	
	return node->totalDepsCount;
}

void CalculateTotalDepsForAll(Tree_t* tree)
{
	NotNull(tree);
	VarArrayLoop(&tree->nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
		node->totalDepsCount = 0;
	}
	VarArrayLoop(&tree->nodes, nIndex)
	{
		VarArrayLoopGet(TreeNode_t, node, &tree->nodes, nIndex);
		CalculateTotalDepsForNode(tree, node);
	}
}

void CreateTestTreeNodes(Tree_t* tree)
{
	NotNull2(tree, tree->allocArena);
	ClearTree(tree);
	
	#define AddNode(name, displayName) u64 name; do                      \
	{                                                                    \
		TreeNode_t* name##Pntr = AddTreeNode(tree, NewStr(displayName)); \
		name = name##Pntr->id;                                           \
	} while(0)
	#define AddNodeEx(name, displayName, ...) u64 name; do                                                   \
	{                                                                                                        \
		u64 name##Deps[] = { __VA_ARGS__ };                                                                  \
		TreeNode_t* name##Pntr = AddTreeNode(tree, NewStr(displayName), ArrayCount(name##Deps), name##Deps); \
		name = name##Pntr->id;                                                                               \
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
	
	CalculateAllDependents(tree);
}
