#include "includes.h"

BOOL WINAPI DllMain( HMODULE, DWORD reason, LPVOID ) {
    if( reason == DLL_PROCESS_ATTACH ) {
        // The worker starts after the loader releases its lock. Never wait here.
        HANDLE thread = CreateThread( nullptr, 0, Client::init, nullptr, 0, nullptr );
        if( !thread )
            return FALSE;

        CloseHandle( thread );
    }

    return TRUE;
}
