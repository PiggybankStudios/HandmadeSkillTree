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
		
		//TODO: Implement me!
		
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
		//TODO: Implement me!
		ClearPointer(main);
	}
}

// +--------------------------------------------------------------+
// |                   Layout and CaptureMouse                    |
// +--------------------------------------------------------------+
void LayoutMainAppState()
{
	RcBindFont(&pig->resources.fonts->debug, SelectDefaultFontFace());
	
}

void CaptureMouseMainAppState()
{
	
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
	CaptureMouseMainAppState();
	
	//TODO: Implement me!
	
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
	
	RcBegin(pig->currentWindow, renderBuffer, &pig->resources.shaders->main2D, Black);
	
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
	}
	
	FreeScratchArena(scratch);
}
