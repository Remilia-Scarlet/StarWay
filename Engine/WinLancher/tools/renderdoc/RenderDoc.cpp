#include "WinLancher/precomp.h"
#include <Windows.h>
#include "RenderDoc.h"
#include "Ash/FileSystem/fs_include.h"
#include "TinyEngine/Engine/EngineDefs.h"


static const char* RENDER_DOC_DLL_NAME = "renderdoc.dll";
static const char* CAPTURE_FOLDER = "game:renderdoc_captures";
static const char* CAPTURE_NAME = "capture_";

RenderDoc* RenderDoc::instance()
{
	TinyAssert(s_instance);
	return s_instance;
}

bool RenderDoc::createInstance()
{
	RenderDoc* instance = new RenderDoc;
	if(!instance || !instance->init())
	{
		TINY_SAFE_DELETE(instance);
		DebugString("Render doc init failed!");
		return false;
	}
	s_instance = instance;
	return true;
}

RenderDoc* RenderDoc::s_instance = nullptr;

bool RenderDoc::init()
{
	do
	{
		const HMODULE dllModule = LoadLibraryA(RENDER_DOC_DLL_NAME);
		TINY_BREAK_IF(!dllModule);

		pRENDERDOC_GetAPI renderdocGetApiAddrs = reinterpret_cast<pRENDERDOC_GetAPI>(GetProcAddress(dllModule, "RENDERDOC_GetAPI"));
		TINY_BREAK_IF(!renderdocGetApiAddrs);

		TINY_BREAK_IF(!renderdocGetApiAddrs(eRENDERDOC_API_Version_1_1_1, reinterpret_cast<void**>(&_renderDocApi)));

		_renderDocApi->SetCaptureOptionU32(eRENDERDOC_Option_CaptureCallstacks, 1);
		_renderDocApi->SetCaptureOptionU32(eRENDERDOC_Option_APIValidation, 1);
		_renderDocApi->SetCaptureOptionU32(eRENDERDOC_Option_DebugOutputMute, 0);
		
		Path capturePath(CAPTURE_FOLDER);
		if(!capturePath.isDirectory())
		{
			capturePath.createDirectory();
		}
		_renderDocApi->SetLogFilePathTemplate((capturePath.getRelativePath() + "\\" + CAPTURE_NAME).c_str());

		RENDERDOC_InputButton prtScrKey = eRENDERDOC_Key_PrtScrn;
		_renderDocApi->SetCaptureKeys(&prtScrKey, 1);

		return true;
	} while (false);
	return false;
}

void RenderDoc::clean()
{
	//nothing to do
}

void RenderDoc::destroy()
{
	if(s_instance)
	{
		s_instance->clean();
		TINY_SAFE_DELETE(s_instance);
	}
}
