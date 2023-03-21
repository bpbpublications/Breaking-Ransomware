// CryptGetUserKey.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <wincrypt.h>

// Link with the Advapi32.lib file.
#pragma comment (lib, "advapi32")

int _tmain(int argc, _TCHAR* argv[])
{
	HCRYPTPROV hProv = NULL;
	HCRYPTKEY hXchgKey = NULL;

	// Get handle to user default provider.
	if(!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0)) 
	{
		printf("Error %x during CryptAcquireContext!\n", GetLastError());
		goto exit;
	}
	// Get handle to key exchange key.
	if(!CryptGetUserKey(hProv, AT_KEYEXCHANGE, &hXchgKey)) 
	{
		printf("Error %x during AT_KEYEXCHANGE CryptGetUserKey!\n", GetLastError());
		goto exit;
	}
	
	// 'hXchgKey' to be used here

	exit:
	// Destroy key exchange key handle.
	if(hXchgKey != 0) CryptDestroyKey(hXchgKey);
	// Release provider handle.
	if(hProv != 0) CryptReleaseContext(hProv, 0);
	return 0;
}
