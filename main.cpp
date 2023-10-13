#include "main.h"

bool isPluginInitialized = false;
static unsigned int m_nLastPressedTime;

void __stdcall GameLoop() {
	static bool initialized = false;
	if (!initialized) {
		if (SAMP::pSAMP->LoadAPI()) {
			initialized = true;
			isPluginInitialized = true;
			SAMP::pSAMP->addMessageToChat(-1, "[sbiv] loaded. key: B | vk/@sinhxxx");
		}
	}
	if (initialized) {	//gameLoop
		CVehicle* pLocalVeh = FindPlayerVehicle(-1, false);
		if (GetAsyncKeyState(66) && // key B
			GetActiveWindow() == FindWindow(0, "AMAZING ONLINE") && // won't work outta gta
			CTimer::m_snTimeInMilliseconds > (m_nLastPressedTime + 1) && // cpu saver
			!pLocalVeh && // not in veh
			SAMP::pSAMP->getMiscInfo()->cursorMode != 3) // not in chat/dialog
		{
			m_nLastPressedTime = CTimer::m_snTimeInMilliseconds;
			BitStream bs;
			bs.Write<UINT16>(SAMP::pSAMP->getPlayers()->sLocalPlayerID);
			SAMP::pSAMP->getRakNet()->EmulRPC(RPC_ScrClearAnimations, &bs);
		}
	}
}

int __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH: {
			SAMP::Init();
			SAMP::CallBacks::pCallBackRegister->RegisterGameLoopCallback(GameLoop);//register gameloop hook
			break;
		}
		case DLL_PROCESS_DETACH: {
			SAMP::ShutDown();
			break;
		}
	}
	return true;
}