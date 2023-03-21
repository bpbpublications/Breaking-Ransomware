// CryptAcquireContext.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <wincrypt.h>

// Link with the Advapi32.lib file.
#pragma comment (lib, "advapi32")

int _tmain(int argc, _TCHAR* argv[])
{
	HCRYPTPROV hProv = NULL;

	// Get handle to the Microsoft Enhanced Cryptographic Provider.
	if(!CryptAcquireContext(
        &hProv, 
        NULL, 
        MS_ENHANCED_PROV, 
        PROV_RSA_FULL, 
        0))
	{
		printf("Error %x during CryptAcquireContext!\n", GetLastError());
		return 0;
	}
	// Add cryptographic operation code.
	
	// Release provider handle.
	if(!CryptReleaseContext(hProv, 0)) 
	{
		printf("Error %x during CryptReleaseContext!\n", GetLastError());
		return 0;
	}
	return 0;
}

