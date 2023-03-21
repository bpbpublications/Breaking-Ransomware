// CryptImportKey.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <windows.h>
#include <wincrypt.h>

// Link with the Advapi32.lib file.
#pragma comment (lib, "advapi32")

//PlainTextBlob is 44 bytes where 12 bytes are blob header and reaming 32 bytes key length.
BYTE PlainTextBlob[] = {
	0x08,0x02,0x00,0x00,0x10,0x66,0x00,0x00,		// BLOB header, 8 bytes 
	0x20,0x00,0x00,0x00,							// key length, in bytes (0x20 = 32 bytes = 256 bit key length)
	0x65,0x4F,0xC2,0xC4,0x1B,0x95,0x1B,0x8C,0x70,	// CALG_AES_256 key
	0xC4,0xB7,0xCF,0x37,0x90,0xAD,0xFF,0x00,0x2D,
	0xC6,0x3A,0xED,0x4C,0xD4,0x7B,0x4F,0x9A,0xE4,
	0xED,0x14,0x17,0x0D,0xB9,0xFD,0xFD,0xFD,0xFD
    };

int _tmain(int argc, _TCHAR* argv[])
{
    // Declare and initialize local variables.
    HCRYPTPROV hCryptProv = NULL; 
    HCRYPTKEY hKey = NULL; 

	PBYTE pbKeyBlob = NULL; 
    DWORD dwKeyBlobLen; 
     
    // Get the handle to the default provider. 
    if(CryptAcquireContext(&hCryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0))
    {
        printf("A cryptographic provider has been acquired. \n");
    }
    else
    {
        printf("Error %x during CryptAcquireContext!\n", GetLastError());
        goto Exit;
    }

    // Import PLAINTEXTBLOB into CSP. 
   if (!CryptImportKey(hCryptProv, PlainTextBlob, sizeof(PlainTextBlob),0, CRYPT_EXPORTABLE, &hKey ))
   {
      printf("Error 0x%08x in importing the Des key \n",
         GetLastError());
   }

    // Determine size of the key BLOB, and allocate memory. 
    if(CryptExportKey(hKey, NULL, PLAINTEXTKEYBLOB, 0, NULL, &dwKeyBlobLen))
    {
        printf("The key BLOB is %d bytes long. \n", dwKeyBlobLen);
    }
    else
    {  
        printf("Error %x computing BLOB length! \n", GetLastError());
        goto Exit;
    }

    if(pbKeyBlob = (BYTE *)malloc(dwKeyBlobLen))
    { 
        printf("Memory is allocated for the key BLOB. \n");
    }
    else
    { 
        printf("Out of memory. \n"); 
        goto Exit;
    }

    // Encrypt and export the session key into a simple key BLOB. 
    if(CryptExportKey(hKey, NULL, PLAINTEXTKEYBLOB, 0, pbKeyBlob, &dwKeyBlobLen))
    {
        printf("The key has been exported. \n");
    } 
    else
    {
        printf("Error %x during CryptExportKey!\n", GetLastError());
        goto Exit;
    } 

	//Printing Key BLOB for verification
	DWORD Num = 0;
	printf("For verification printing Key BLOB: \n");
	for(Num=0; Num < dwKeyBlobLen;)
	{
		printf("%02x ",pbKeyBlob[Num]);
		Num ++;
	}

    // Free memory.
    free(pbKeyBlob);

Exit:
    // Release the session key. 
    if(hKey)
    {
        if(!(CryptDestroyKey(hKey)))
        {
            printf("Error %x during CryptDestroyKey!\n", GetLastError());
        }
    }

    // Release the provider handle. 
    if(hCryptProv)
    {
        if(!(CryptReleaseContext(hCryptProv, 0)))
        {
            printf("Error %x during CryptReleaseContext!\n", GetLastError());
        }
    }
    
	return 0;
}

