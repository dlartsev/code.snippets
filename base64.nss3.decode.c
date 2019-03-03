typedef char*(*PL_Base64Decode_p)(
        const char *src, 
        PRUint32 srclen,
        char *dest
);

PL_Base64Decode_p fPL_Base64Decode = NULL;

int Base64Decode(char *cryptData, char **decodeData, int *decodeLen)
{
	int len = strlen(cryptData);
	int adjust = 0;
	if (cryptData[len - 1] == '=')
	{
		adjust++;
		if (cryptData[len - 2] == '=')
			adjust++;
	}
	*decodeData = (char *)(*fPL_Base64Decode)(cryptData, len, NULL);
	if (*decodeData == NULL)
	{
		return 0;
	}
	*decodeLen = (len * 3) / 4 - adjust;
	return 1;
}

BOOL nss3() {
	NSS3 = LoadLibraryA("nss3.dll");
	if (NSS3 != NULL) {
		fPL_Base64Decode = (PL_Base64Decode_p)GetProcAddress(NSS3, "PL_Base64Decode");
		if (fPL_Base64Decode != NULL) {
			return TRUE;
		}
		else return FALSE;
	}
	else return FALSE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    int decodeLen = 0;
	  char *decodeData = NULL;
	  Base64Decode(strCryptData, &decodeData, &decodeLen);
}
