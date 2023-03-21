// CryptExportKey.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <wincrypt.h>

// Link with the Advapi32.lib file.
#pragma comment (lib, "advapi32")

#define KEYLENGTH  0x00800000		//2048 bits
#define ENCRYPT_ALGORITHM CALG_RC4 

int _tmain(int argc, _TCHAR* argv[])
{
    //---------------------------------------------------------------
    // Declare and initialize local variables.
    HCRYPTPROV hCryptProv = NULL; 
    HCRYPTKEY hKey = NULL; 
    HCRYPTKEY hXchgKey = NULL; 

    PBYTE pbKeyBlob = NULL; 
    DWORD dwKeyBlobLen; 
     
    //---------------------------------------------------------------
    // Get the handle to the default provider. 
    if(CryptAcquireContext(
        &hCryptProv, 
        NULL, 
        MS_ENH_RSA_AES_PROV, 
        PROV_RSA_AES, 
        0))
    {
        printf("A cryptographic provider has been acquired. \n");
    }
    else
    {
        printf("Error %x during CryptAcquireContext!\n", GetLastError());
        goto Exit;
    }

    // Create a random session key. 
    if(CryptGenKey(
        hCryptProv, 
        CALG_AES_256, 
        CRYPT_EXPORTABLE, 
        &hKey))
    {
        printf("A session key has been created. \n");
    } 
    else
    {
        printf("Error %x during CryptGenKey. \n", GetLastError()); 
        goto Exit;
    }

    //-----------------------------------------------------------
    // Get the handle to the exchange public key. 
    if(CryptGetUserKey(
        hCryptProv, 
        AT_KEYEXCHANGE, 
        &hXchgKey))
    {
        printf("The user public key has been retrieved. \n");
    }
    else
    { 
        if(NTE_NO_KEY == GetLastError())
        {
            // No exchange key exists. Try to create one.
            if(!CryptGenKey(
                hCryptProv, 
                AT_KEYEXCHANGE, 
                CRYPT_EXPORTABLE, 
                &hXchgKey))
            {
                printf("Error %x Could not create a user public key.\n", GetLastError()); 
                goto Exit;
            }
        }
        else
        {
            printf("Error %x User public key is not available and may not exist.\n", GetLastError()); 
            goto Exit;
        }
    }

    //-----------------------------------------------------------
    // Determine size of the key BLOB, and allocate memory. 
    if(CryptExportKey(
        hKey, 
        hXchgKey, 
        SIMPLEBLOB, 
        0, 
        NULL, 
        &dwKeyBlobLen))
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

    //-----------------------------------------------------------
    // Encrypt and export the session key into a simple key BLOB. 
    if(CryptExportKey(
        hKey, 
        hXchgKey, 
        SIMPLEBLOB, 
        0, 
        pbKeyBlob, 
        &dwKeyBlobLen))
    {
        printf("The key has been exported. \n");
    } 
    else
    {
        printf("Error %x during CryptExportKey!\n", GetLastError());
        goto Exit;
    } 
         
    //-----------------------------------------------------------
    // Release the key exchange key handle. 
    if(hXchgKey)
    {
        if(!(CryptDestroyKey(hXchgKey)))
        {
            printf("Error %x during CryptDestroyKey.\n", GetLastError()); 
            goto Exit;
        }
      
        hXchgKey = 0;
    }
     
    // Free memory.
    free(pbKeyBlob);



Exit:
    //---------------------------------------------------------------
    // Release the session key. 
    if(hKey)
    {
        if(!(CryptDestroyKey(hKey)))
        {
            printf("Error %x during CryptDestroyKey!\n", GetLastError());
        }
    }

    //---------------------------------------------------------------
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
