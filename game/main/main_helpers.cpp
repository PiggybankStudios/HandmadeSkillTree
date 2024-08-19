/*
File:   main_helpers.cpp
Author: Taylor Robbins
Date:   07\19\2024
Description: 
	** Holds a variety of loose functions that help the Main AppState do it's job
*/

bool IsConnectionHoverConnected(const TreeNodeConn_t* connection)
{
	return (main->isNodeHovered && (main->hoveredNodeId == connection->upId || main->hoveredNodeId == connection->downId));
}

bool IsConnectionSelectionConnected(const TreeNodeConn_t* connection)
{
	return (main->isNodeSelected && (main->selectedNodeId == connection->upId || main->selectedNodeId == connection->downId));
}
