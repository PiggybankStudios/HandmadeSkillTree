@echo off

:: TODO: Do we need this?
setlocal enabledelayedexpansion

python --version 2>NUL
if %ERRORLEVEL% NEQ 0 (
	echo WARNING: Python isn't installed on this computer. Defines cannot be extracted from build_config.h!
	exit /B %ERRORLEVEL%
)

:: +--------------------------------------------------------------+
:: |                      Paths and Options                       |
:: +--------------------------------------------------------------+
for /f %%i in ('orca sdk-path') do set OrcaSdkPath=%%i

set SourceFolder=../src
set DataFolder=../data
set LibFolder=../lib
set AppIconPath=%DataFolder%/icon.png
set OutputWasmModulePath=module.wasm
set MainSourcePath=%SourceFolder%/main.cpp
set BuildConfigPath=%SourceFolder%/build_config.h
set ExtractDefineScriptPath=%LibFolder%/include/gylib/ExtractDefine.py

for /f "delims=" %%i in ('python %ExtractDefineScriptPath% %BuildConfigPath% PROJECT_NAME')      do set PROJECT_NAME=%%i
for /f "delims=" %%i in ('python %ExtractDefineScriptPath% %BuildConfigPath% PROJECT_NAME_SAFE') do set PROJECT_NAME_SAFE=%%i
for /f "delims=" %%i in ('python %ExtractDefineScriptPath% %BuildConfigPath% DEBUG_BUILD')       do set DEBUG_BUILD=%%i

:: +--------------------------------------------------------------+
:: |                  Compiler and Linker Flags                   |
:: +--------------------------------------------------------------+
:: Main Flags
:: --target=wasm32 = Output to wasm32 binary
:: -mbuild-memory = Something about bulk memory operations like memset being opt-in instructions in wasm, this is how we make those functions compile to single instructions
:: -std=c++11 = ?
:: -D__DEFINED_wchar_t = TODO: Why is this needed?? When compiling in C++ mode, we run into an error:
::                             alltypes.h:116:13: error: cannot combine with previous 'int' declaration specifier
set CompilerFlags=--target=wasm32 -mbulk-memory -std=c++11 -D__DEFINED_wchar_t
:: Linker Flags
:: -Wl,--no-entry = Allow no entry point in this compilation (Orca will act as the entry point, and it will use our wasm binary when we do orca bundle)
:: -Wl,--export-dynamic = ?
set CompilerFlags=%CompilerFlags% -Wl,--no-entry -Wl,--export-dynamic
:: Include Directories
:: --sysroot = Set the include directory for standard library headers (like stdint.h)
:: -I = Add an include directory so search when resolving #include "..." lines
set CompilerFlags=%CompilerFlags% --sysroot %OrcaSdkPath%/orca-libc -I%OrcaSdkPath%/src -I%OrcaSdkPath%/src/ext -I%LibFolder%/include/my_orca
:: Linker Flags
:: -L = Add a lib include folder
:: -lorca_wasm = This is the precompiled binary that we compile with in order to get all the orca API functions exposed to us
set LinkerFlags=-L %OrcaSdkPath%/bin -lorca_wasm

if "%DEBUG_BUILD%"=="1" (
	REM -g = Generate debug symbols
	REM -O2 = Optimization level 2
	set CompilerFlags=%CompilerFlags% -g -O0
) else (
	REM -O2 = Optimization level 2
	set CompilerFlags=%CompilerFlags% -O2
)

:: +--------------------------------------------------------------+
:: |                     Compile Wasm Module                      |
:: +--------------------------------------------------------------+
echo [Compiling WASM Module...]

clang %CompilerFlags% %LinkerFlags% -o %OutputWasmModulePath% %MainSourcePath%

IF %ERRORLEVEL% NEQ 0 (
	echo [Compilation Failed!]
	EXIT /B %ERRORLEVEL%
) ELSE (
	echo [Compilation Finished!]
)

:: +--------------------------------------------------------------+
:: |                   Bundle Orca Application                    |
:: +--------------------------------------------------------------+
echo [Bunding Orca App...]

orca bundle --name %PROJECT_NAME_SAFE% --icon %AppIconPath% --resource-dir %DataFolder% %OutputWasmModulePath%

echo [Finished Bundling!]
