/*
File:   main.cpp
Author: Taylor Robbins
Date:   09\21\2024
Description: 
	** This is the main file that gets compiled into the WASM module.
	** All other files are #included from this one (including cpp files!) to keep compilation simple
*/

#include <math.h>
#include <orca.h>

#include "orca_aliases.h"
#include "orca_addons.h"

#include "build_config.h"

OC_Surface_t surface;
OC_CanvasRenderer_t renderer;
OC_CanvasContext_t canvasContext;
v2 mousePos;
v2 ScreenSize;
v2i ScreenSizei;
rec ScreenRec;
OC_Image_t pigTexture;

EXPORT void OC_OnInit()
{
	OC_Log_I("OC_OnInit!");
	
	OC_WindowSetTitle(NewStr(PROJECT_NAME_STR));
	
	renderer = OC_CanvasRendererCreate();
	surface = OC_CanvasSurfaceCreate(renderer);
	canvasContext = OC_CanvasContextCreate();
	
	pigTexture = OC_ImageCreateFromPath(renderer, NewStr("Resources/Textures/pig_invalid.png"), false);
	OC_Assert(!OC_ImageIsNil(pigTexture), "Failed to load pig_invalid.png!");
}

EXPORT void OC_OnResize(u32 width, u32 height)
{
	// OC_Log_I("OC_OnResize(%d, %d)!", width, height);
	ScreenSize = NewVec2((r32)width, (r32)height);
	ScreenSizei = NewVec2i((i32)width, (i32)height);
	ScreenRec = NewRec(0, 0, (r32)width, (r32)height);
}

EXPORT void OC_OnKeyDown(OC_ScanCode_t scan, OC_KeyCode_t key)
{
	//TODO: Implement me!
}

EXPORT void OC_OnKeyUp(OC_ScanCode_t scan, OC_KeyCode_t key)
{
	//TODO: Implement me!
}

EXPORT void OC_OnMouseMove(float x, float y, float dx, float dy)
{
	mousePos.x = x;
	mousePos.y = y;
}

EXPORT void OC_OnFrameRefresh()
{
	OC_ArenaScope_t scratch = OC_ScratchBegin();
	
	OC_CanvasContextSelect(canvasContext);
	OC_SetColorRgba(NewColorfHex(0xFFCC3B95));
	OC_Clear();
	
	OC_SetColorRgba(NewColorfHex(0xFF222222));
	// OC_ImageDraw(pigTexture, ScreenRec); //TODO: Color doesn't matter for OC_ImageDraw?
	OC_SetImage(pigTexture);
	OC_RectangleFill(ScreenRec);
	
	OC_SetColorRgba(NewColorfHex(0xFFF27CB1));
	OC_SetImage(pigTexture);
	OC_RoundedRectangleFill(NewRec(mousePos.x, mousePos.y, 100, 200), 25);
	OC_SetImage(OC_ImageNil());
	
	OC_CircleFill(mousePos.x - 50 + 25, mousePos.y - 50 + 25, 50);
	
	OC_SetColorRgba(NewColorfHex(0xFFF83333));
	OC_SetWidth(20);
	OC_MoveTo(100, 100);
	OC_CubicTo(100, 100, mousePos.x, mousePos.y, 200, 100);
	OC_Stroke();
	
	OC_CanvasRender(renderer, canvasContext, surface);
	OC_CanvasPresent(renderer, surface);
	
	OC_ScratchEnd(scratch);
}
