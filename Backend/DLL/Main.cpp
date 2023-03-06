#include "../Include/GlobalIncludes.h"
#include "../Utility/Utilities.h"
#include "../Configuration/Settings.h"
#include "../Interfaces/Interfaces.h"
#include "../Configuration/Config.h"
#include "../Features/EventListener.h"
#include "../Security/Security.h"
#include "../Features/Visuals/EventLogger.h"
#include "../DLL/HWID/HWID.h"
#include "../lazyimports.h"
#include "../_fixes/anticrash.h"

#include "Engine.h"
#include <fstream>
#include <io.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <thread>
#include <chrono>
#include <string>

#include "Client/main.h"
#include "../../Other/fonts.h"

#include "../Security/base64.h"
#include "../Security/md5.h"
#include "../Hashing/xorstr.h"

#include "../Lua/CLua.h"
#include "../Features/skinchanger/cskin.h"
#include "../Features/Skinchanger/skinchanger2.h"

using namespace std::chrono_literals;

void __stdcall MainThread(LPVOID lpParam);
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved);

void quit(int) {
	Cheat::Settings->SocketConnection.Close();
	PostQuitMessage(0);
}

std::string ssprintf(const char* text, ...) {
	char bf[4096];

	va_list ap;
	va_start(ap, text);
	vsprintf(bf, text, ap);
	va_end(ap);

	return std::string(bf);
}

void __stdcall MainThread(LPVOID lpParam)
{
	//LI_FN(SetUnhandledExceptionFilter).get()(&My_UnhandledExceptionFilter);
	Cheat::Utilities->Console_Create("skeetsu");

	Cheat::Settings->LoadDefaults();
	c_config::get()->_load_defaults();
	c_config::get()->load_keys();

	std::string cmdln = LI_FN(GetCommandLineA).get()();
	if (cmdln.find("-d3d9ex") != std::string::npos) {
		LI_FN(MessageBoxA).get()(0, "Cannot run source with -d3d9ex parameter. Please, remove -d3d9ex parameter from CS:GO launch options and try again.", "Fatal error", MB_APPLMODAL | MB_OK | MB_ICONERROR);
		quit(0);
	}

	//CopyFile("C:\\source\\undefeated.ttf", "C:\\Windows\\Fonts\\undefeated.ttf", 0);
	AddFontResource("C:\\source\\undefeated.ttf");
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
	LoadLibraryA(xorstr_("mscorlib.dll"));

	Cheat::Settings->PanoramaMode = true;

	Interfaces::InterfaceLoader->Initialize();
	/*if (Cheat::Settings->ShowConsole)
		Cheat::Utilities->Console_Create(xorstr_("source BETA"));
	//LoadLibraryA(xorstr_("mscorlib.dll"));

	if (!LI_FN(GetModuleHandleA).get()(xorstr_("serverbrowser.dll")))
	{
		Cheat::Utilities->Console_Log(xorstr_("Waiting for CS:GO..."));

		while (!LI_FN(GetModuleHandleA).get()(xorstr_("serverbrowser.dll")))
			Sleep(1000);
	}

	Cheat::Utilities->Console_Log(xorstr_("source %i.%i build %i"),
		Cheat::Settings->VersionMajor,
		Cheat::Settings->VersionMinor,
		Cheat::Settings->VersionBuild);

	if (Cheat::Settings->UIDevMode)
		Cheat::Utilities->Console_Log(xorstr_("ImGui development mode! Only ImGUI was loaded..."));

	if (LI_FN(GetModuleHandleA).get()(xorstr_("client.dll")))
		Cheat::Settings->PanoramaMode = true;

	if (!Cheat::Settings->UIDevMode)
	{
		Interfaces::InterfaceLoader->Initialize();
	}*/
	CreateDirectory("C:/source/", NULL);
	CreateDirectory("C:/source/cfg/", NULL);
	CreateDirectory("C:/source/lua/", NULL);
	Cheat::wrapgameoverlay();
	Cheat::Initialize();

	c_lua::get()->initialize();

/*	Interfaces::Engine->ClientCmd_Unrestricted(xorstr_("clear"));
	LI_FN(Sleep).get()(500);
	Cheat::Utilities->Game_Msg((std::string(xorstr_("Welcome ")) + Cheat::Settings->UserName).c_str());*/

    //SkinChanger::setup_skins();
	//skins.skinChanger[WEAPON_KNIFE] = {};
	//auto knife_skin = &skins.skinChanger[WEAPON_KNIFE];
	//knife_skin->itemId = WEAPON_KNIFE;
	//knife_skin->definition_override_index = ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY;
	//knife_skin->paintKit = 418;
	//knife_skin->quality = 3;
	//knife_skin->wear = 0.01f;
	//knife_skin->stat_trak = 1337;
	//knife_skin->seed = 1;

	//auto glock_skin = &skins.skinChanger[WEAPON_GLOCK];
	//glock_skin->itemId = WEAPON_GLOCK;
	//glock_skin->paintKit = 353;
	//glock_skin->wear = 0.40f;
	//glock_skin->stat_trak = 228;
	//glock_skin->seed = 1;

	while (true)
	{
		if (Cheat::Settings->UnloadCheat)
		{
			bool _break = true;
			static size_t m_count = sizeof(Cheat::Settings->UnloadReadyTable) / sizeof(int);
			for (size_t i = 0; i < m_count; ++i)
			{
				int unload_state = ((int*)&Cheat::Settings->UnloadReadyTable)[i];
				bool can_unload = (unload_state == 0) || (((int)GetTickCount() - unload_state) >= 1000);
				if (!can_unload)
				{
					_break = false;
					break;
				}
			}
			if(_break)
				break;
		}
		LI_FN(Sleep).get()(1000);
	}

	Cheat::Unload();
	Cheat::unwrapgameoverlay();
	Interfaces::InterfaceLoader->Free();
	Cheat::Utilities->Console_Close();
	LI_FN(SetUnhandledExceptionFilter).get()(nullptr);
	LI_FN(FreeLibraryAndExitThread).get()((HMODULE)lpParam, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwCallReason, LPVOID lpReserved)
{
	if (dwCallReason == DLL_PROCESS_ATTACH) {
		LI_FN(CreateThread).get()(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL);
		return TRUE;
	}
	return FALSE;
}
