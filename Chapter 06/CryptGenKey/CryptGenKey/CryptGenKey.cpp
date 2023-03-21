// CryptGenKey.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <wincrypt.h>

// Link with the Advapi32.lib file.
#pragma comment (lib, "advapi32")

int _tmain(int argc, _TCHAR* argv[])
{
	HCRYPTPROV hProv = NULL;
	HCRYPTKEY hKey = NULL;

	// Get handle to the Microsoft Enhanced Cryptographic Provider.
	if(!CryptAcquireContext(
        &hProv, 
        NULL, 
        MS_ENHANCED_PROV, 
        PROV_RSA_FULL, 
        0))
	{
		printf("Error %x during CryptAcquireContext!\n", GetLastError());
		goto exit;
	}
	
	// Create block cipher session key.
	if(!CryptGenKey(
		hProv, 
		CALG_RC2, 
		CRYPT_EXPORTABLE, 
		&hKey)) 
	{
		printf("Error %x during CryptGenKey!\n", GetLastError());
		goto exit;
	}

	// 'hKey' is used to perform something.

	
	exit:
	// Destroy session key.
	if(hKey != 0) CryptDestroyKey(hKey);
	// Release provider handle.
	if(hProv != 0) CryptReleaseContext(hProv, 0);
	return 0;
}


