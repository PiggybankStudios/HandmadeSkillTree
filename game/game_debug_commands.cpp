/*
File:   game_debug_commands.cpp
Author: Taylor Robbins
Date:   06\17\2022
Description: 
	** Holds the GameHandleDebugCommand function which is where the game is
	** able to try processing a debug command input to the console.
	** See pig_debug_commands.cpp for engine level debug commands
*/

#pragma warning(push)
#pragma warning(disable: 4100) //'context': unreferenced formal parameter

// +--------------------------------------------------------------+
// |                       Command Helpers                        |
// +--------------------------------------------------------------+
ResourcePool_t* GameGetCurrentResourcePool() //pre-declared in game_main.h
{
	return nullptr; //TODO: Implement me!
}

// +--------------------------------------------------------------+
// |                             test                             |
// +--------------------------------------------------------------+
#define Debug_Test_Def  "void test()"
#define Debug_Test_Desc "Serves as a dedicated spot to place temporary test code"
void Debug_Test()
{
	MemArena_t* scratch = GetScratchArena();
	
	// Unimplemented(); //TODO: Implement me!
	
	//MdFile_t test
	#if 1
	{
		MyStr_t testFilePath = NewStr("Resources/Tree/handmade_hero.md");
		PlatFileContents_t testFile;
		bool readFileSuccess = plat->ReadFileContents(testFilePath, scratch, false, &testFile);
		Assert(readFileSuccess);
		MyStr_t testFileContents = NewStr(testFile.size, testFile.chars);
		
		ProcessLog_t parseLog;
		CreateDefaultProcessLog(&parseLog);
		SetProcessLogFilePath(&parseLog, testFilePath);
		
		MdFile_t mdFile;
		InitMdFile(&mdFile, mainHeap);
		MdFileSetPath(&mdFile, testFilePath);
		
		if (TryParseMdFile(testFileContents, &parseLog, &mdFile))
		{
			PrintLine_I("Parsed mdFile into %llu pieces", mdFile.pieces.count);
			MdPiece_t* piece = LinkedListFirst(&mdFile.pieces, MdPiece_t);
			u64 pIndex = 0;
			while (piece != nullptr)
			{
				PrintLine_D("Piece[%llu] %s \"%.*s\" (Prefix:\"%.*s\" Suffix:\"%.*s\")",
					pIndex,
					GetMdPieceTypeStr(piece->type),
					StrPrint(piece->editable.str),
					StrPrint(piece->prefix.str),
					StrPrint(piece->suffix.str)
				);
				piece = LinkedListNext(&mdFile.pieces, MdPiece_t, piece);
				pIndex++;
			}
		}
		else
		{
			PrintLine_E("Failed to parse mdFile: %s", GetResultStr((Result_t)parseLog.errorCode));
		}
		
		if (parseLog.hadErrors || parseLog.hadWarnings)
		{
			DumpProcessLog(&parseLog, "MdFile Parse Log", DbgLevel_Warning);
		}
		
		FreeProcessLog(&parseLog);
		FreeMdFile(&mdFile);
	}
	#endif
	
	FreeScratchArena(scratch);
}
EXPRESSION_FUNC_DEFINITION(Debug_Test_Glue) { Debug_Test(); return NewExpValueVoid(); }

// +--------------------------------------------------------------+
// |                         Registration                         |
// +--------------------------------------------------------------+
void GameAddDebugVarsToExpContext(ExpContext_t* context) //pre-declared in game_main.h
{
	const bool read = false;
	const bool write = true;
	UNUSED(read);
	UNUSED(write);
	
	// Constants
	AddExpConstantDef(context, "version_major", NewExpValueU8(GAME_VERSION_MAJOR));
	AddExpConstantDef(context, "version_minor", NewExpValueU8(GAME_VERSION_MINOR));
	AddExpConstantDef(context, "version_build", NewExpValueU16(GAME_VERSION_BUILD));
}

void GameAddDebugCommandsToExpContext(ExpContext_t* context) //pre-declared in game_main.h
{
	AddDebugCommandDef(context, Debug_Test_Def, Debug_Test_Glue, Debug_Test_Desc);
}

#pragma warning(pop)
