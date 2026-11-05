#include <Windows.h>
#include "core/Client.h"
#include "utils/Logger.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        Client::instance().startThread();
        break;
    }
    case DLL_PROCESS_DETACH:
    {
        Client::instance().shutdown();
        break;
    }
    }
    return TRUE;
}
