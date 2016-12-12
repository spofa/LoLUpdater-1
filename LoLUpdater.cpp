#define fileversion L"2.0.4.0"
#include "resource.h"
#include <Windows.h>
#include <fstream>
#include <Shlwapi.h>
#include <Shlobj.h>
#include <sstream>
#include <tlhelp32.h>
#include <codecvt>
#include <intrin.h>
#include <iostream>
#include <thread>
#define WIN32_LEAN_AND_MEAN
wchar_t gdx[2][MAX_PATH + 1];
wchar_t Reshade[2][MAX_PATH + 1];
wchar_t DXBUFFER[36][MAX_PATH + 1];
wchar_t Windows[MAX_PATH + 1];
wchar_t winpath[MAX_PATH + 1];
wchar_t enb[16][MAX_PATH + 1];
wchar_t client[5][MAX_PATH + 1]; // AIR, PATCH, GAME, BETA
wchar_t adobe[2][MAX_PATH + 1]; // Adobe AIR, Adobe FLASH
wchar_t cgbuf[3][MAX_PATH + 1]; // CG, GL, D3D9
wchar_t instdir[7][MAX_PATH + 1]; // REGULAR, GARENA, QQ, ADMIN, BETA, TENCENT
wchar_t buff[3][MAX_PATH + 1];
wchar_t DXSETUP[MAX_PATH + 1];
wchar_t temp[MAX_PATH + 1];
wchar_t Config[MAX_PATH + 1];
wchar_t tempo[MAX_PATH + 1];
wchar_t autoupdater[3][MAX_PATH + 1];
std::wstringstream wss;
wchar_t logbuffer[MAX_PATH + 1];
wchar_t svc[MAX_PATH + 1];
bool betaclient = false;
bool rclient = false;
wchar_t tbb[MAX_PATH + 1];
wchar_t loldir[MAX_PATH + 1];
const wchar_t *ComboBoxItems[] = { L"Fast TBB.dll", L"Precise TBB.dll", L"Strict TBB.dll" };
wchar_t files[11][MAX_PATH + 1] = { L"msvcp120.dll", L"msvcr120.dll", L"concrt140.dll", L"vcruntime140.dll", L"ucrtbase.dll", L"msvcp140.dll", L"cg.dll" , L"cgGL.dll", L"cgD3D9.dll", L"tbb.dll", L"NPSWF32.dll" };
wchar_t adobef[MAX_PATH + 1] = { L"Adobe AIR" };
wchar_t* cwd(_wgetcwd(nullptr, 0));
std::basic_string<wchar_t>line;
std::wifstream Myfile;
const std::wstring DLL = L".dll";
const std::wstring ini = L"LoLUpdater.ini";
const std::wstring rads = L"RADS";
const std::wstring proj = L"projects";
const std::wstring rel = L"releases";
const std::wstring dep = L"deploy";
const std::wstring wildcard = L"\\*.*\0";
WIN32_FIND_DATA data;
bool directx = false;
bool logs = false;
bool precise = false;
bool strict = false;
bool avx = false;
bool avx2 = false;

wchar_t *pValue;
size_t len;

SHELLEXECUTEINFO ei;
SHFILEOPSTRUCT stSHFileOpStruct;
WIN32_FIND_DATA fdFile;
HWND hwnd;

HWND hwndButton;
HWND hwndButton2;
HWND hwndButton3;
HWND hwndButton4;
HWND hWndComboBox;

HDC hDC;
PAINTSTRUCT ps;
RECT localLabel = { 0, 0, 260, 20 };
RECT localLabel1 = { 20, 80, 260, 0 };


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ButtonProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ButtonProc2(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ButtonProc3(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ButtonProc4(HWND, UINT, WPARAM, LPARAM);


WNDPROC OldButtonProc;
WNDPROC OldButtonProc2;
WNDPROC OldButtonProc3;
WNDPROC OldButtonProc4;


PROCESSENTRY32 entry;
MSG Msg;
FILE* f;
FILE *f1;
VS_FIXEDFILEINFO test;


bool IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
	OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0,{ 0 }, 0, 0 };
	DWORDLONG        const dwlConditionMask = VerSetConditionMask(
		VerSetConditionMask(
			VerSetConditionMask(
				0, VER_MAJORVERSION, VER_GREATER_EQUAL),
			VER_MINORVERSION, VER_GREATER_EQUAL),
		VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

	osvi.dwMajorVersion = wMajorVersion;
	osvi.dwMinorVersion = wMinorVersion;
	osvi.wServicePackMajor = wServicePackMajor;

	return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
}




class LimitSingleInstance
{
protected:
	HANDLE Mutex;

public:
	explicit LimitSingleInstance(std::wstring const& strMutexName)
	{
		Mutex = CreateMutex(nullptr, 0, strMutexName.c_str());
	}

	~LimitSingleInstance()
	{
		if (Mutex)
		{
			CloseHandle(Mutex);
			Mutex = nullptr;
		}
	}

	static BOOL AnotherInstanceRunning()
	{
		return ERROR_ALREADY_EXISTS == GetLastError();
	}
};

struct Version
{
	explicit Version(std::wstring versionStr)
	{
		swscanf_s(versionStr.c_str(), L"%d.%d.%d.%d", &major, &minor, &revision, &build);
	}

	bool operator<(const Version &otherVersion) const
	{
		if (major < otherVersion.major)
			return true;
		if (minor < otherVersion.minor)
			return true;
		if (revision < otherVersion.revision)
			return true;
		if (build < otherVersion.build)
			return true;
		return false;
	}

	int major, minor, revision, build;
};

class ZoneIdentifier
{
public:
	explicit ZoneIdentifier(const wchar_t* fileName)
		: mFilename(fileName ? fileName : L"")
	{
		if (!fileExists(mFilename))
		{
			mFilename.clear();
		}
	}

	bool validFile() const
	{
		return !mFilename.empty();
	}

	static bool fileExists(const std::wstring& fileName)
	{
		auto attr = GetFileAttributesW(fileName.c_str());
		return INVALID_FILE_ATTRIBUTES != attr && FILE_ATTRIBUTE_DIRECTORY != attr;
	}

	bool hasZoneID() const
	{
		if (validFile())
		{
			auto file = mFilename;
			file.append(L":Zone.Identifier");
			return fileExists(file);
		}
		return false;
	}

	bool strip() const
	{
		if (validFile())
		{
			auto file = mFilename;
			file.append(L":Zone.Identifier");
			return !!DeleteFile(file.c_str());
		}
		return false;
	}

private:
	std::wstring mFilename;
};

class CallbackHandler : public IBindStatusCallback
{
	int m_percentLast;

public:
	virtual ~CallbackHandler()
	{
	}

	CallbackHandler() : m_percentLast(0)
	{
	}

	// IUnknown

	HRESULT STDMETHODCALLTYPE
		QueryInterface(REFIID riid, void** ppvObject) override
	{

		if (IsEqualIID(IID_IBindStatusCallback, riid)
			|| IsEqualIID(IID_IUnknown, riid))
		{
			*ppvObject = reinterpret_cast<void*>(this);
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	ULONG STDMETHODCALLTYPE
		AddRef() override
	{
		return 2UL;
	}

	ULONG STDMETHODCALLTYPE
		Release() override
	{
		return 1UL;
	}

	// IBindStatusCallback

	HRESULT STDMETHODCALLTYPE
		OnStartBinding(DWORD     /*dwReserved*/,
			IBinding* /*pib*/) override
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE
		GetPriority(LONG* /*pnPriority*/) override
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE
		OnLowResource(DWORD /*reserved*/) override
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE
		OnProgress(ULONG   ulProgress,
			ULONG   ulProgressMax,
			ULONG   ulStatusCode,
			LPCWSTR /*szStatusText*/) override
	{
		switch (ulStatusCode)
		{
		case BINDSTATUS_FINDINGRESOURCE:
			std::wcout << L"Finding resource..." << std::endl;
			break;
		case BINDSTATUS_CONNECTING:
			std::wcout << L"Connecting..." << std::endl;
			break;
		case BINDSTATUS_SENDINGREQUEST:
			std::wcout << L"Sending request..." << std::endl;
			break;
		case BINDSTATUS_MIMETYPEAVAILABLE:
			std::wcout << L"Mime type available" << std::endl;
			break;
		case BINDSTATUS_CACHEFILENAMEAVAILABLE:
			std::wcout << L"Cache filename available" << std::endl;
			break;
		case BINDSTATUS_BEGINDOWNLOADDATA:
			std::wcout << L"Begin download" << std::endl;
			break;
		case BINDSTATUS_DOWNLOADINGDATA:
		case BINDSTATUS_ENDDOWNLOADDATA:
		{
			auto percent = static_cast<int>(100.0 * static_cast<double>(ulProgress) / static_cast<double>(ulProgressMax));
			if (m_percentLast < percent)
			{


				m_percentLast = percent;
			}

			if (ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA)
			{
				std::wcout << std::endl << L"End download" << std::endl;
			}
		}
		break;

		default:
		{
			std::wcout << L"Status code : " << ulStatusCode << std::endl;
		}
		}
		// The download can be cancelled by returning E_ABORT here
		// of from any other of the methods.
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE
		OnStopBinding(HRESULT /*hresult*/,
			LPCWSTR /*szError*/) override
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE
		GetBindInfo(DWORD*    /*grfBINDF*/,
			BINDINFO* /*pbindinfo*/) override
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE
		OnDataAvailable(DWORD      /*grfBSCF*/,
			DWORD      /*dwSize*/,
			FORMATETC* /*pformatetc*/,
			STGMEDIUM* /*pstgmed*/) override
	{
		return E_NOTIMPL;
	}

	HRESULT STDMETHODCALLTYPE
		OnObjectAvailable(REFIID    /*riid*/,
			IUnknown* /*punk*/) override
	{
		return E_NOTIMPL;
	}
};



void PCombine(PWSTR pszPathOut, std::wstring const& pszPathIn, std::wstring const& pszMore)
{
	PathCombine(pszPathOut, pszPathIn.c_str(), pszMore.c_str());
}

void PAppend(PWSTR pszPath, std::wstring const& pszMore)
{
	PathAppend(pszPath, pszMore.c_str());
}

void FindLatest(PWSTR PATH)
{
	auto hFind = FindFirstFile(std::wstring(PATH + std::wstring(L"\\*.*")).c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(hFind, &data))
		{
		}

		PAppend(PATH, data.cFileName);
		FindClose(hFind);
	}
}

int del(PCWSTR DeleteFolderPath)
{
	int nFolderPathLen = wcslen(DeleteFolderPath);
	auto pszFrom = new wchar_t[nFolderPathLen + 2];
	wcscpy_s(pszFrom, nFolderPathLen + 2, DeleteFolderPath);
	pszFrom[nFolderPathLen] = 0;
	pszFrom[++nFolderPathLen] = 0;

	stSHFileOpStruct.wFunc = FO_DELETE;
	stSHFileOpStruct.pFrom = pszFrom;
	stSHFileOpStruct.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR | FOF_SILENT | FOF_FILESONLY;
	stSHFileOpStruct.fAnyOperationsAborted = false;
	auto nFileDeleteOprnRet = SHFileOperation(&stSHFileOpStruct);
	delete[]pszFrom;
	if (0 != nFileDeleteOprnRet)
	{
		return 1;
	}
	return 0;
}

void CheckAndKill(std::wstring const& processName)
{
	auto exists = false;
	entry.dwSize = sizeof entry;

	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_wcsicmp(entry.szExeFile, processName.c_str()))
				exists = true;

	CloseHandle(snapshot);

	if (exists)
	{
		entry.dwSize = sizeof entry;
		auto hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
		bool hRes = Process32First(hSnapShot, &entry);
		while (hRes)
		{
			if (_wcsicmp(entry.szExeFile, processName.c_str()) == 0)
			{
				auto hProcess = OpenProcess(PROCESS_TERMINATE, 0, static_cast<DWORD>(entry.th32ProcessID));
				if (hProcess != nullptr)
				{
					TerminateProcess(hProcess, 9);
					CloseHandle(hProcess);
				}
			}
			hRes = Process32Next(hSnapShot, &entry);
		}
		CloseHandle(hSnapShot);
	}
}

bool LoL()
{
	return (std::wifstream(instdir[4]).good() || std::wifstream(instdir[0]).good() || std::wifstream(instdir[3]).good());
}

bool LoLQQ()
{
	return (std::wifstream(instdir[1]).good() || std::wifstream(instdir[6]).good());
}


void LoLCheck()
{
	if (LoL())
	{
		if (MessageBox(nullptr, L"League of Legends will now close", L"LoLUpdater", MB_YESNO) == IDYES)
		{
			CheckAndKill(L"LoLClient.exe");
			CheckAndKill(L"LoLPatcher.exe");
			CheckAndKill(L"League of Legends.exe");
			CheckAndKill(L"LeagueClient.exe");
			CheckAndKill(L"lol.launcher.exe");
			CheckAndKill(L"lol.launcher.admin.exe");

		}
		else
		{
			exit(0);
		}
	}
	else
	{
		if (MessageBox(nullptr, L"League of Legends will now close", L"LoLUpdater", MB_YESNO) == IDYES)
		{
			CheckAndKill(L"LoLClient.exe");
			CheckAndKill(L"LoLLauncher.exe");
			CheckAndKill(L"League of Legends.exe");
			CheckAndKill(L"lol.exe");
			CheckAndKill(L"lolex.exe");
			CheckAndKill(L"Client.exe");
			CheckAndKill(L"lol.launcher_tencent.exe");
		}
		else
		{
			exit(0);
		}
	}
}

void UnblockFile(std::wstring const& filename)
{
	if (std::wifstream(filename).fail())
	{
		std::wcout << filename.c_str() << L" was not found, continuing" << std::endl;
	}
	else
	{
		ZoneIdentifier id(filename.c_str());

		if (id.hasZoneID() & std::wifstream(filename).good())
		{
			id.strip();
		}
	}
}


void ExtractResource(std::wstring const& RCDATAID, std::wstring const& filename)
{

	auto hRes = FindResource(nullptr, RCDATAID.c_str(), RT_RCDATA);
	_wfopen_s(&f, filename.c_str(), L"wb");
	fwrite(LockResource(LoadResource(nullptr, hRes)), SizeofResource(nullptr, hRes), 1, f);
	fclose(f);
	UnblockFile(filename);

}

bool IsDots(const std::wstring& strName)
{
	return strName == L"." || strName == L"..";
}

bool RmDir(const std::wstring& strDir)
{

	memset(&fdFile, 0, sizeof fdFile);

	auto strSearch = strDir + L"\\*.*";

	auto hFind = ::FindFirstFile(strSearch.data(), &fdFile);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	do
	{
		auto strDelete = strDir + L"\\" + fdFile.cFileName;

		if ((fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{

			if (IsDots(fdFile.cFileName))
			{
				continue;
			}

			RmDir(strDelete);
		}

		else
		{
			::DeleteFile(strDelete.data());
		}
	} while (::FindNextFile(hFind, &fdFile));

	FindClose(hFind);
	::RemoveDirectory(strDir.data());

	return true;
}


void msvccopy(std::wstring const& MSVCP, std::wstring const& MSVCR)
{

	*svc = '\0';
	PCombine(svc, client[2], files[0]);
	ExtractResource(MSVCP, svc);

	*svc = '\0';
	PCombine(svc, client[2], files[1]);
	ExtractResource(MSVCR, svc);

	if (LoL())
	{
		*svc = '\0';
		PCombine(svc, client[1], files[0]);
		ExtractResource(MSVCP, svc);

		*svc = '\0';
		PCombine(svc, client[1], files[1]);
		ExtractResource(MSVCR, svc);
	}
}

int instrset_detect(void) {

	static int iset = -1;                                  // remember value for next call
	if (iset >= 0) {
		return iset;                                       // called before
	}
	iset = 0;                                              // default value
	int abcd[4] = { 0,0,0,0 };                               // cpuid results
	__cpuid(abcd, 0);                                        // call cpuid function 0
	if (abcd[0] == 0) return iset;                         // no further cpuid function supported
	__cpuid(abcd, 1);                                        // call cpuid function 1 for feature flags


	if ((abcd[2] & (1 << 27)) == 0) return iset;           // no OSXSAVE
	if ((_xgetbv(0) & 6) != 6)       return iset;           // AVX not enabled in O.S.
	if ((abcd[2] & (1 << 28)) == 0) return iset;           // no AVX
	iset = 7;                                              // 7: AVX supported
	__cpuid(abcd, 7);                                        // call cpuid leaf 7 for feature flags
	if ((abcd[1] & (1 << 5)) == 0) return iset;           // no AVX2
	iset = 8;                                              // 8: AVX2 supported
	return iset;
}

void SIMDCheck(std::wstring const& AVX2, std::wstring const& AVX, std::wstring const& SSE2, std::wstring const& SAVX2, std::wstring const& SAVX, std::wstring const& SSSE2, std::wstring const& PAVX2, std::wstring const& PAVX, std::wstring const& PSSE2)
{
	avx2 = (instrset_detect() >= 8);
	avx = (instrset_detect() >= 7);

	
	if (strict)
	{
		if (avx2)
		{
			ExtractResource(SAVX2, tbb);
		}
		if (avx & !avx2)
		{

			ExtractResource(SAVX, tbb);
		}
		if(!avx & !avx2)
		{
			ExtractResource(SSSE2, tbb);
		}
	}
	if (precise)
	{
		if (avx2)
		{
			ExtractResource(PAVX2, tbb);
		}
		if (avx & !avx2)
		{

			ExtractResource(PAVX, tbb);
		}
		if (!avx & !avx2)
		{
			ExtractResource(PSSE2, tbb);
		}
	}
	if(!strict & !precise)
	{
		if (avx2)
		{
			ExtractResource(AVX2, tbb);
		}
		if (avx & !avx2)
		{
			ExtractResource(AVX, tbb);
		}
		if (!avx & !avx2)
		{
			ExtractResource(SSE2, tbb);
		}
	}


}

bool Is64BitWindows()
{
	typedef BOOL(WINAPI *WPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

	auto bIsWow64 = FALSE;

	auto fnIsWow64Process = reinterpret_cast<WPFN_ISWOW64PROCESS>(GetProcAddress(
		GetModuleHandle(L"kernel32"), "IsWow64Process"));

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			bIsWow64 = FALSE;
		}
	}

	return bIsWow64 != 0;

}


bool dirExists(const std::wstring& dirName_in)
{
	DWORD ftyp = GetFileAttributes(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

LRESULT CALLBACK ButtonProc(HWND, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		Msg = {};
		SendMessage(hwndButton, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(L"Patching..."));
		std::thread t1(LoLCheck);
		t1.join();

		switch (SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0))
		{
		case 0:
			precise = false;
			strict = false;
			break;
		case 1:
			precise = true;
			strict = false;
			break;
		case 2:
			strict = true;
			precise = false;
			break;
		}

		if (LoL())
		{
			PCombine(Config, loldir, L"Config");
			PCombine(tempo, loldir, L"Config");
			PAppend(Config, L"game.cfg");

			SetFileAttributes(Config, FILE_ATTRIBUTE_NORMAL);

			static std::wifstream file(Config);
			if (file.good())
			{
				std::wstring strReplace = L"EnableParticleOptimizations=0";
				std::wstring strNew = L"EnableParticleOptimizations=1";
				std::wifstream filein(Config); //File to read from

				PAppend(tempo, L"renametogame.cfg");
				std::wofstream fileout(tempo); //Temporary file
				std::wstring strTemp;

				while (filein >> strTemp)
				{
					if (strTemp == strReplace) {
						strTemp = strNew;
					}
					fileout << std::endl;
					fileout << strTemp;
				}
				fileout.close();
				filein.close();

				CopyFile(tempo, Config, false);
				DeleteFile(tempo);

				std::wstring contents((std::istreambuf_iterator<wchar_t>(file)), std::istreambuf_iterator<wchar_t>());
				std::wofstream out(Config, std::ios::app);
				Myfile.open(Config);
				std::wstring needle = L"DefaultParticleMultiThreading=1";

				if (contents.find(needle.c_str()) == std::wstring::npos)
				{
					out << std::endl;
					out << needle;
					out << std::endl;
				}

				Myfile.close();
			}


			msvccopy(L"X2", L"X3");
		}
		else
		{
			ExtractResource(L"D3D", gdx[1]);
			DeleteFile(gdx[0]);
		}




		if (!GetProcAddress(GetModuleHandle(L"ntdll.dll"), "wine_get_version"))
		{


			if (IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINTHRESHOLD), LOBYTE(_WIN32_WINNT_WINTHRESHOLD), 0))
			{
				SIMDCheck(L"x6", L"x9", L"x12", L"S3", L"S6", L"S9", L"P3", L"P6", L"P9");
			}

			if (IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), 0))
			{
				SIMDCheck(L"x5", L"x8", L"x11", L"S8", L"S5", L"S2", L"P8", L"P5", L"P2");
			}

			if (IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 0))
			{
				SIMDCheck(L"x4", L"x7", L"x10", L"S1", L"S4", L"S7", L"P1", L"P4", L"P7");
			}

			if (IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 0))
			{
				if (strict)
				{
					ExtractResource(L"S10", tbb);
				}
				if (precise)
				{
					ExtractResource(L"P10", tbb);
				}
				if(!strict & !precise)
				{
					ExtractResource(L"x13", tbb);
				}
			}

			if (IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 0))
			{
				if (strict)
				{
					ExtractResource(L"XPS", tbb);
				}
				if (precise)
				{
					ExtractResource(L"XPP", tbb);
				}
				if (!strict & !precise)
				{
					ExtractResource(L"XP", tbb);
				}
			}


		}
		else
		{
			std::wcout << L"Running on Wine" << std::endl;
			SIMDCheck(L"x4", L"x7", L"x10", L"S1", L"S4", L"S7", L"P1", L"P4", L"P7");
		}

		ExtractResource(L"xa1", cgbuf[0]);
		ExtractResource(L"xa2", cgbuf[1]);
		ExtractResource(L"xa3", cgbuf[2]);

		if (betaclient)
		{
			*svc = '\0';
			PCombine(svc, client[4], L"d3dcompiler_47.dll");
			UnblockFile(svc);

			*svc = '\0';
			PCombine(svc, client[4], files[2]);
			ExtractResource(L"CON", svc);

			*svc = '\0';
			PCombine(svc, client[4], files[3]);
			ExtractResource(L"CON2", svc);

			*svc = '\0';
			PCombine(svc, client[4], files[4]);
			ExtractResource(L"CON3", svc);

			*svc = '\0';
			PCombine(svc, client[4], files[5]);
			ExtractResource(L"CON4", svc);

			*svc = '\0';
			PCombine(svc, loldir, files[2]);
			ExtractResource(L"CON", svc);

			*svc = '\0';
			PCombine(svc, loldir, files[3]);
			ExtractResource(L"CON2", svc);

			*svc = '\0';
			PCombine(svc, loldir, files[4]);
			ExtractResource(L"CON3", svc);


			*svc = '\0';
			PCombine(svc, loldir, files[5]);
			ExtractResource(L"CON4", svc);
		}

		if (rclient)
		{

			*svc = '\0';
			PCombine(svc, loldir, files[0]);
			ExtractResource(L"x2", svc);

			*svc = '\0';
			PCombine(svc, loldir, files[1]);
			ExtractResource(L"x3", svc);

			ExtractResource(L"xAA", adobe[0]);
			ExtractResource(L"xBB", adobe[1]);
		}
		else if (!rclient)
		{
			ExtractResource(L"xAA", adobe[0]);
			ExtractResource(L"xBB", adobe[1]);
		}

		if (directx)
		{
			PCombine(temp, cwd, L"DirectXTemp");
			CreateDirectory(temp, nullptr);

			ExtractResource(L"DX0", temp + std::wstring(L"\\Apr2005_d3dx9_25_x86.cab"));
			ExtractResource(L"DX1", temp + std::wstring(L"\\Apr2006_d3dx9_30_x86.cab"));
			ExtractResource(L"DX2", temp + std::wstring(L"\\Apr2006_MDX1_x86.cab"));
			ExtractResource(L"DX3", temp + std::wstring(L"\\Apr2006_MDX1_x86_Archive.cab"));
			ExtractResource(L"DX4", temp + std::wstring(L"\\Apr2006_xinput_x86.cab"));
			ExtractResource(L"DX5", temp + std::wstring(L"\\Apr2006_XACT_x86.cab"));
			ExtractResource(L"DX6", temp + std::wstring(L"\\APR2007_d3dx9_33_x86.cab"));
			ExtractResource(L"DX7", temp + std::wstring(L"\\APR2007_d3dx10_33_x86.cab"));
			ExtractResource(L"DX8", temp + std::wstring(L"\\APR2007_XACT_x86.cab"));
			ExtractResource(L"DX9", temp + std::wstring(L"\\Aug2005_d3dx9_27_x86.cab"));
			ExtractResource(L"DX10", temp + std::wstring(L"\\AUG2006_XACT_x86.cab"));
			ExtractResource(L"DX11", temp + std::wstring(L"\\APR2007_xinput_x86.cab"));
			ExtractResource(L"DX12", temp + std::wstring(L"\\AUG2007_d3dx9_35_x86.cab"));
			ExtractResource(L"DX13", temp + std::wstring(L"\\AUG2006_xinput_x86.cab"));
			ExtractResource(L"DX14", temp + std::wstring(L"\\AUG2007_d3dx10_35_x86.cab"));
			ExtractResource(L"DX15", temp + std::wstring(L"\\AUG2007_XACT_x86.cab"));
			ExtractResource(L"DX16", temp + std::wstring(L"\\Aug2008_d3dx9_39_x86.cab"));
			ExtractResource(L"DX17", temp + std::wstring(L"\\Aug2008_d3dx10_39_x86.cab"));
			ExtractResource(L"DX18", temp + std::wstring(L"\\Aug2008_XACT_x86.cab"));
			ExtractResource(L"DX19", temp + std::wstring(L"\\Aug2008_XAudio_x86.cab"));
			ExtractResource(L"DX20", temp + std::wstring(L"\\Aug2009_D3DCompiler_42_x86.cab"));
			ExtractResource(L"DX21", temp + std::wstring(L"\\Aug2009_d3dcsx_42_x86.cab"));
			ExtractResource(L"DX22", temp + std::wstring(L"\\Aug2009_d3dx9_42_x86.cab"));
			ExtractResource(L"DX23", temp + std::wstring(L"\\Aug2009_d3dx10_42_x86.cab"));
			ExtractResource(L"DX24", temp + std::wstring(L"\\Aug2009_d3dx11_42_x86.cab"));
			ExtractResource(L"DX25", temp + std::wstring(L"\\Aug2009_XACT_x86.cab"));
			ExtractResource(L"DX26", temp + std::wstring(L"\\Aug2009_XAudio_x86.cab"));
			ExtractResource(L"DX27", temp + std::wstring(L"\\Dec2005_d3dx9_28_x86.cab"));
			ExtractResource(L"DX28", temp + std::wstring(L"\\DEC2006_d3dx9_32_x86.cab"));
			ExtractResource(L"DX29", temp + std::wstring(L"\\DEC2006_d3dx10_00_x86.cab"));
			ExtractResource(L"DX30", temp + std::wstring(L"\\DEC2006_XACT_x86.cab"));

			ExtractResource(L"DX31", temp + std::wstring(L"\\DSETUP.dll"));
			ExtractResource(L"DX32", temp + std::wstring(L"\\dsetup32.dll"));
			ExtractResource(L"DX33", temp + std::wstring(L"\\dxdllreg_x86.cab"));
			ExtractResource(L"DX34", temp + std::wstring(L"\\DXSETUP.exe"));
			ExtractResource(L"DX35", temp + std::wstring(L"\\dxupdate.cab"));

			ExtractResource(L"DX36", temp + std::wstring(L"\\Feb2005_d3dx9_24_x86.cab"));
			ExtractResource(L"DX37", temp + std::wstring(L"\\Feb2006_d3dx9_29_x86.cab"));
			ExtractResource(L"DX38", temp + std::wstring(L"\\Feb2006_XACT_x86.cab"));
			ExtractResource(L"DX39", temp + std::wstring(L"\\FEB2007_XACT_x86.cab"));
			ExtractResource(L"DX40", temp + std::wstring(L"\\Feb2010_X3DAudio_x86.cab"));
			ExtractResource(L"DX41", temp + std::wstring(L"\\Feb2010_XACT_x86.cab"));
			ExtractResource(L"DX42", temp + std::wstring(L"\\Feb2010_XAudio_x86.cab"));
			ExtractResource(L"DX43", temp + std::wstring(L"\\Jun2005_d3dx9_26_x86.cab"));
			ExtractResource(L"DX44", temp + std::wstring(L"\\JUN2006_XACT_x86.cab"));
			ExtractResource(L"DX45", temp + std::wstring(L"\\JUN2007_d3dx9_34_x86.cab"));
			ExtractResource(L"DX46", temp + std::wstring(L"\\JUN2007_d3dx10_34_x86.cab"));
			ExtractResource(L"DX47", temp + std::wstring(L"\\Jun2010_d3dcsx_43_x86.cab"));
			ExtractResource(L"DX48", temp + std::wstring(L"\\JUN2007_XACT_x86.cab"));
			ExtractResource(L"DX49", temp + std::wstring(L"\\JUN2008_d3dx9_38_x86.cab"));
			ExtractResource(L"DX50", temp + std::wstring(L"\\JUN2008_d3dx10_38_x86.cab"));
			ExtractResource(L"DX51", temp + std::wstring(L"\\JUN2008_X3DAudio_x86.cab"));
			ExtractResource(L"DX52", temp + std::wstring(L"\\JUN2008_XACT_x86.cab"));
			ExtractResource(L"DX53", temp + std::wstring(L"\\JUN2008_XAudio_x86.cab"));
			ExtractResource(L"DX54", temp + std::wstring(L"\\Jun2010_D3DCompiler_43_x86.cab"));
			ExtractResource(L"DX55", temp + std::wstring(L"\\Jun2010_XACT_x86.cab"));
			ExtractResource(L"DX56", temp + std::wstring(L"\\Jun2010_d3dx9_43_x86.cab"));
			ExtractResource(L"DX57", temp + std::wstring(L"\\Jun2010_d3dx11_43_x86.cab"));
			ExtractResource(L"DX58", temp + std::wstring(L"\\Jun2010_d3dx10_43_x86.cab"));
			ExtractResource(L"DX59", temp + std::wstring(L"\\Mar2008_d3dx9_37_x86.cab"));
			ExtractResource(L"DX60", temp + std::wstring(L"\\Jun2010_XAudio_x86.cab"));
			ExtractResource(L"DX61", temp + std::wstring(L"\\Mar2008_d3dx10_37_x86.cab"));
			ExtractResource(L"DX62", temp + std::wstring(L"\\Mar2008_X3DAudio_x86.cab"));
			ExtractResource(L"DX63", temp + std::wstring(L"\\Mar2008_XAudio_x86.cab"));
			ExtractResource(L"DX64", temp + std::wstring(L"\\Mar2008_XACT_x86.cab"));
			ExtractResource(L"DX65", temp + std::wstring(L"\\Mar2009_d3dx9_41_x86.cab"));
			ExtractResource(L"DX66", temp + std::wstring(L"\\Mar2009_d3dx10_41_x86.cab"));
			ExtractResource(L"DX67", temp + std::wstring(L"\\Mar2009_X3DAudio_x86.cab"));
			ExtractResource(L"DX68", temp + std::wstring(L"\\Mar2009_XACT_x86.cab"));
			ExtractResource(L"DX69", temp + std::wstring(L"\\Mar2009_XAudio_x86.cab"));
			ExtractResource(L"DX70", temp + std::wstring(L"\\Nov2007_d3dx9_36_x86.cab"));
			ExtractResource(L"DX71", temp + std::wstring(L"\\Nov2007_d3dx10_36_x86.cab"));
			ExtractResource(L"DX72", temp + std::wstring(L"\\NOV2007_X3DAudio_x86.cab"));
			ExtractResource(L"DX73", temp + std::wstring(L"\\NOV2007_XACT_x86.cab"));
			ExtractResource(L"DX74", temp + std::wstring(L"\\Nov2008_d3dx9_40_x86.cab"));
			ExtractResource(L"DX75", temp + std::wstring(L"\\Nov2008_d3dx10_40_x86.cab"));
			ExtractResource(L"DX76", temp + std::wstring(L"\\Nov2008_X3DAudio_x86.cab"));
			ExtractResource(L"DX77", temp + std::wstring(L"\\Nov2008_XACT_x86.cab"));
			ExtractResource(L"DX78", temp + std::wstring(L"\\Nov2008_XAudio_x86.cab"));
			ExtractResource(L"DX79", temp + std::wstring(L"\\Oct2005_xinput_x86.cab"));
			ExtractResource(L"DX80", temp + std::wstring(L"\\OCT2006_d3dx9_31_x86.cab"));
			ExtractResource(L"DX81", temp + std::wstring(L"\\OCT2006_XACT_x86.cab"));

			ei.cbSize = sizeof(SHELLEXECUTEINFO);
			ei.fMask = SEE_MASK_NOCLOSEPROCESS;
			ei.nShow = SW_SHOW;
			ei.lpVerb = L"runas";
			ei.lpParameters = L"/silent";


			PCombine(DXSETUP, cwd, temp);
			PAppend(DXSETUP, L"DXSETUP.exe");
			ei.lpFile = DXSETUP;

			ShellExecuteEx(&ei);
			if (ei.hProcess != nullptr)
			{
				WaitForSingleObject(ei.hProcess, INFINITE);
			}


			RmDir(temp);


			GetWindowsDirectory(winpath, MAX_PATH + 1);
			if (Is64BitWindows())
			{
				PCombine(Windows, winpath, L"SysWOW64");
			}
			else
			{
				PCombine(Windows, winpath, L"System32");
			}

			PCombine(DXBUFFER[0], Windows, L"d3d10.dll");
			PCombine(DXBUFFER[1], Windows, L"d3d10_1.dll");
			PCombine(DXBUFFER[2], Windows, L"d3d10_1core.dll");
			PCombine(DXBUFFER[3], Windows, L"d3d10core.dll");
			PCombine(DXBUFFER[4], Windows, L"d3d10level9.dll");
			PCombine(DXBUFFER[5], Windows, L"d3d10ref.dll");
			PCombine(DXBUFFER[6], Windows, L"d3d10sdklayers.dll");
			PCombine(DXBUFFER[7], Windows, L"d3d10warp.dll");
			PCombine(DXBUFFER[8], Windows, L"d3d11.dll");
			PCombine(DXBUFFER[9], Windows, L"d3d11_1sdklayers.dll");
			PCombine(DXBUFFER[10], Windows, L"d3d11_2sdklayers.dll");
			PCombine(DXBUFFER[11], Windows, L"d3d11ref.dll");
			PCombine(DXBUFFER[12], Windows, L"d3d11sdklayers.dll");
			PCombine(DXBUFFER[13], Windows, L"D3D12.dll");
			PCombine(DXBUFFER[14], Windows, L"d3dx10.dll");
			PCombine(DXBUFFER[15], Windows, L"d3dx10_33.dll");
			PCombine(DXBUFFER[16], Windows, L"d3dx10_34.dll");
			PCombine(DXBUFFER[17], Windows, L"d3dx10_35.dll");
			PCombine(DXBUFFER[18], Windows, L"d3dx10_36.dll");
			PCombine(DXBUFFER[19], Windows, L"d3dx10_37.dll");
			PCombine(DXBUFFER[20], Windows, L"d3dx10_38.dll");
			PCombine(DXBUFFER[21], Windows, L"d3dx10_39.dll");
			PCombine(DXBUFFER[22], Windows, L"d3dx10_40.dll");
			PCombine(DXBUFFER[23], Windows, L"d3dx10_41.dll");
			PCombine(DXBUFFER[24], Windows, L"d3dx10_42.dll");
			PCombine(DXBUFFER[25], Windows, L"d3dx10_43.dll");
			PCombine(DXBUFFER[26], Windows, L"d3dx11_42.dll");
			PCombine(DXBUFFER[27], Windows, L"d3dx11_43.dll");
			PCombine(DXBUFFER[28], Windows, L"XAudio2_8.dll");
			PCombine(DXBUFFER[29], Windows, L"XAudio2_9.dll");
			PCombine(DXBUFFER[30], Windows, L"XInput1_4.dll");
			PCombine(DXBUFFER[31], Windows, L"D3DCompiler_47.dll");
			PCombine(DXBUFFER[32], Windows, L"d2d1.dll");
			PCombine(DXBUFFER[33], Windows, L"XInput9_1_0.dll");
			PCombine(DXBUFFER[34], Windows, L"d3dref9.dll");
			PCombine(DXBUFFER[35], Windows, L"opengl32.dll");

			for (auto i = 0; i < 36; i++)
			{
				UnblockFile(DXBUFFER[i]);
			}
		}

		if (logs)
		{
			PCombine(logbuffer, loldir, L"Logs");
			RmDir(logbuffer);
		}
		if (betaclient)
		{
			ei = {};
			ei.cbSize = sizeof(SHELLEXECUTEINFO);
			ei.fMask = SEE_MASK_NOCLOSEPROCESS;
			ei.nShow = SW_SHOW;
			ei.lpFile = instdir[4];

			ShellExecuteEx(&ei);

			exit(0);
		}
		else if (rclient & LoL())
		{

			ei = {};
			ei.cbSize = sizeof(SHELLEXECUTEINFO);
			ei.fMask = SEE_MASK_NOCLOSEPROCESS;
			ei.nShow = SW_SHOW;
			if (std::wifstream(instdir[0]).fail())
			{

				ei.lpVerb = L"runas";
				ei.lpFile = instdir[3];
			}
			else
			{
				ei.lpFile = instdir[0];
			}

			ShellExecuteEx(&ei);

			exit(0);
		}
		EnableWindow(hwndButton, false);
		SendMessage(hwndButton, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(L"Patched!"));
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return CallWindowProc(OldButtonProc, hwndButton, msg, wp, lp);
}


LRESULT CALLBACK ButtonProc2(HWND, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		Msg = {};
		SendMessage(hwndButton2, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(L"Restoring..."));
		std::thread t2(LoLCheck);
		t2.join();
		CheckAndKill(L"ENBInjector.exe");

		if (rclient)
		{
			ExtractResource(L"x101", adobe[0]);
			ExtractResource(L"x701", adobe[1]);

			*svc = '\0';
			PCombine(svc, loldir, files[0]);
			ExtractResource(L"x501", svc);

			*svc = '\0';
			PCombine(svc, loldir, files[1]);
			ExtractResource(L"x601", svc);
		}

		ExtractResource(L"xu1", cgbuf[0]);
		ExtractResource(L"xu2", cgbuf[1]);
		ExtractResource(L"xu3", cgbuf[2]);
		ExtractResource(L"xu4", tbb);


		PCombine(buff[0], client[2], L"d3d9.dll");
		PCombine(buff[1], client[2], L"reshade-shaders");
		PCombine(buff[2], client[2], L"d3d9.ini");
		if (std::wifstream(buff[0]).good())
		{
			DeleteFile(buff[0]);
		}
		if (dirExists(buff[1]))
		{
			RmDir(buff[1]);
		}


		if (std::wifstream(buff[2]).good())
		{
			DeleteFile(buff[2]);
		}


		if (betaclient)
		{

			*svc = '\0';
			PCombine(svc, client[4], files[2]);
			ExtractResource(L"CONx", svc);
			*svc = '\0';
			PCombine(svc, client[4], files[3]);
			ExtractResource(L"CONx2", svc);

			*svc = '\0';
			PCombine(svc, client[4], files[4]);
			ExtractResource(L"CONx3", svc);

			*svc = '\0';
			PCombine(svc, client[4], files[5]);
			ExtractResource(L"XU6", svc);


			*svc = '\0';
			PCombine(svc, loldir, files[2]);
			ExtractResource(L"CONx", svc);
			*svc = '\0';
			PCombine(svc, loldir, files[3]);
			ExtractResource(L"CONx2", svc);

			*svc = '\0';
			PCombine(svc, loldir, files[4]);
			ExtractResource(L"CONx3", svc);

			*svc = '\0';
			PCombine(svc, loldir, files[5]);
			ExtractResource(L"XU6", svc);
		}

		if (!LoL())
		{
			ExtractResource(L"xu5", gdx[0]);
			DeleteFile(gdx[1]);
		}



		DeleteFile(enb[0]);
		DeleteFile(enb[1]);
		DeleteFile(enb[2]);
		DeleteFile(enb[3]);
		DeleteFile(enb[4]);
		DeleteFile(enb[5]);
		DeleteFile(enb[6]);
		DeleteFile(enb[7]);
		DeleteFile(enb[8]);
		DeleteFile(enb[9]);
		DeleteFile(enb[10]);
		RmDir(enb[11]);
		RmDir(enb[12]);
		RmDir(enb[13]);
		RmDir(enb[14]);
		RmDir(enb[15]);


		EnableWindow(hwndButton2, false);
		SendMessage(hwndButton2, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(L"Restored!"));
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return CallWindowProc(OldButtonProc2, hwndButton2, msg, wp, lp);
}

LRESULT CALLBACK ButtonProc3(HWND, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		Msg = {};


		ExtractResource(L"RES1", Reshade[0]);
		ExtractResource(L"RES0", Reshade[1]);

		ShellExecute(nullptr, nullptr, Reshade[0], nullptr, nullptr, SW_SHOW);

		ei.cbSize = sizeof(SHELLEXECUTEINFO);
		ei.fMask = SEE_MASK_NOCLOSEPROCESS;
		ei.nShow = SW_SHOW;

		ei.lpFile = Reshade[1];

		ShellExecuteEx(&ei);
		if (ei.hProcess != nullptr)
		{
			WaitForSingleObject(ei.hProcess, INFINITE);
		}

		DeleteFile(Reshade[0]);
		DeleteFile(Reshade[1]);
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	default: break;
	}
	return CallWindowProc(OldButtonProc3, hwndButton3, msg, wp, lp);
}

LRESULT CALLBACK ButtonProc4(HWND, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		Msg = {};

		SendMessage(hwndButton4, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(L"Installing..."));
		CheckAndKill(L"ENBInjector.exe");
		ExtractResource(L"ENB0", enb[0]);
		ExtractResource(L"ENB1", enb[1]);
		ExtractResource(L"ENB2", enb[2]);
		ExtractResource(L"ENB3", enb[3]);
		ExtractResource(L"ENB4", enb[4]);
		ExtractResource(L"ENB6", enb[5]);
		ExtractResource(L"ENB7", enb[6]);
		ExtractResource(L"ENB8", enb[7]);
		ExtractResource(L"ENB9", enb[8]);
		ExtractResource(L"ENB10", enb[9]);
		ExtractResource(L"ENB5", enb[10]);
		CreateDirectory(enb[11], nullptr);
		CreateDirectory(enb[12], nullptr);
		CreateDirectory(enb[13], nullptr);
		CreateDirectory(enb[14], nullptr);
		CreateDirectory(enb[15], nullptr);
		ExtractResource(L"ENB11", enb[15] + std::wstring(L"\\psh2DF967C6.txt"));
		ExtractResource(L"ENB12", enb[15] + std::wstring(L"\\psh8DB4CDB2.txt"));
		ExtractResource(L"ENB13", enb[15] + std::wstring(L"\\psh323E9BB8.txt"));
		ExtractResource(L"ENB14", enb[15] + std::wstring(L"\\pshF5256B40.txt"));
		ExtractResource(L"ENB18", enb[11] + std::wstring(L"\\enbeffect.fx"));
		ExtractResource(L"ENB17", enb[12] + std::wstring(L"\\enbeffect.fx"));
		ExtractResource(L"ENB16", enb[13] + std::wstring(L"\\enbeffect.fx"));
		ExtractResource(L"ENB15", enb[14] + std::wstring(L"\\enbeffect.fx"));

		ei.cbSize = sizeof(SHELLEXECUTEINFO);
		ei.fMask = SEE_MASK_NOCLOSEPROCESS;
		ei.nShow = SW_SHOW;

		ei.lpFile = enb[5];

		ShellExecuteEx(&ei);

		EnableWindow(hwndButton4, false);
		SendMessage(hwndButton4, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(L"ENB Installed"));
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	default: break;
	}
	return CallWindowProc(OldButtonProc4, hwndButton4, msg, wp, lp);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &ps);
		SetTextColor(hDC, RGB(0, 0, 0));
		SetBkMode(hDC, TRANSPARENT);
		if (LoL())
		{
			DrawText(hDC, L"Erase all LoL-Logs (FPS+)", -1, &localLabel, DT_CENTER | DT_NOCLIP);
		}

		DrawText(hDC, L"Unblock DirectX for LoL (FPS+)", -1, &localLabel1, DT_CENTER | DT_NOCLIP);
		EndPaint(hwnd, &ps);
		break;

	case WM_CREATE:


		hwndButton = CreateWindowEx(
			NULL,
			L"BUTTON",
			L"Install",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			20,
			20,
			100,
			50,
			hwnd,
			nullptr,
			nullptr,
			nullptr
		);

		OldButtonProc = reinterpret_cast<WNDPROC>(SetWindowLong(hwndButton, GWL_WNDPROC, reinterpret_cast<LONG>(ButtonProc)));


		hwndButton2 = CreateWindowEx(
			NULL,
			L"BUTTON",
			L"Restore",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			140,
			20,
			100,
			50,
			hwnd,
			nullptr,
			nullptr,
			nullptr
		);

		OldButtonProc2 = reinterpret_cast<WNDPROC>(SetWindowLong(hwndButton2, GWL_WNDPROC, reinterpret_cast<LONG>(ButtonProc2)));

		hwndButton3 = CreateWindowEx(
			NULL,
			L"BUTTON",
			L"ReShade",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			260,
			40,
			100,
			50,
			hwnd,
			nullptr,
			nullptr,
			nullptr
		);

		OldButtonProc3 = reinterpret_cast<WNDPROC>(SetWindowLong(hwndButton3, GWL_WNDPROC, reinterpret_cast<LONG>(ButtonProc3)));


		hwndButton4 = CreateWindowEx(
			NULL,
			L"BUTTON",
			L"ENB",
			WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			380,
			40,
			100,
			50,
			hwnd,
			nullptr,
			nullptr,
			nullptr
		);

		OldButtonProc4 = reinterpret_cast<WNDPROC>(SetWindowLong(hwndButton4, GWL_WNDPROC, reinterpret_cast<LONG>(ButtonProc4)));





		hWndComboBox = CreateWindowEx(NULL, L"COMBOBOX",
			NULL,
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWN,
			250, 0, 150, 200,
			hwnd,
			nullptr,
			nullptr,
			nullptr);

		SendMessage(hWndComboBox,
			CB_ADDSTRING,
			0,
			reinterpret_cast<LPARAM>((PCWSTR)ComboBoxItems[0]));
		SendMessage(hWndComboBox,
			CB_ADDSTRING,
			0,
			reinterpret_cast<LPARAM>((PCWSTR)ComboBoxItems[1]));
		SendMessage(hWndComboBox,
			CB_ADDSTRING,
			0,
			reinterpret_cast<LPARAM>((PCWSTR)ComboBoxItems[2]));



		if (std::wifstream(ini).good())
		{
			std::wifstream wif(ini);
			wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			wss << wif.rdbuf();

			wcsncat_s(loldir, _countof(loldir), wss.str().c_str(), _TRUNCATE);
		}
		else
		{
			BROWSEINFO bi{};
			bi.lpszTitle = L"Select your League of Legends installation directory: (C:\Riot Games\League of Legends)";
			bi.ulFlags = BIF_USENEWUI;

			auto pidl = SHBrowseForFolder(&bi);

			if (pidl == nullptr)
			{
				exit(0);
			}

			SHGetPathFromIDList(pidl, loldir);

			std::wofstream outputFile(ini);
			outputFile << loldir;
			outputFile.close();
		}
		PCombine(instdir[0], loldir, L"lol.launcher.exe");

		PCombine(instdir[1], loldir, L"TCLS");
		PAppend(instdir[1], L"Client.exe");

		PCombine(instdir[6], loldir, L"lol.launcher_tencent.exe");



		PCombine(instdir[2], loldir, L"lol.exe");

		PCombine(instdir[5], loldir, L"lolex.exe");


		PCombine(instdir[3], loldir, L"lol.launcher.admin.exe");
		PCombine(instdir[4], loldir, L"LeagueClient.exe");



		CreateWindowEx(NULL, L"button", L"directx", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 15, 80, 15, 15, hwnd, reinterpret_cast<HMENU>(CHECKBOX2), (reinterpret_cast<LPCREATESTRUCT>(lp))->hInstance, NULL);


		if (LoL())
		{
			CreateWindowEx(NULL, L"button", L"Delete Logs", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 15, 0, 15, 15, hwnd, reinterpret_cast<HMENU>(CHECKBOX1), (reinterpret_cast<LPCREATESTRUCT>(lp))->hInstance, NULL);
		}

		break;

	case WM_COMMAND:
	{
		if ((HIWORD(wp) == BN_CLICKED))
		{
			switch (LOWORD(wp))
			{
			case CHECKBOX1:
			{
				if (IsDlgButtonChecked(hwnd, CHECKBOX1))
				{
					CheckDlgButton(hwnd, CHECKBOX1, BST_UNCHECKED);
					logs = false;
				}
				else
				{
					CheckDlgButton(hwnd, CHECKBOX1, BST_CHECKED);
					logs = true;
				}
			}
			break;

			case CHECKBOX2:
			{

				if (IsDlgButtonChecked(hwnd, CHECKBOX2))
				{
					CheckDlgButton(hwnd, CHECKBOX2, BST_UNCHECKED);
					directx = false;
				}
				else
				{
					CheckDlgButton(hwnd, CHECKBOX2, BST_CHECKED);
					directx = true;
				}
			}
			break;
			}
		}
		break;
	}




	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		exit(0);
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	LimitSingleInstance GUID(L"Global\\{101UPD473R-BYL0GG4N08@G17HUB-V3RYR4ND0M4NDR4R3MUCHW0W}");
	if (GUID.AnotherInstanceRunning())
		return 0;

	Msg = {};
	AllocConsole();
	AttachConsole(GetCurrentProcessId());

	_wfreopen_s(&f1, L"CONIN$", L"r", stdin);
	_wfreopen_s(&f1, L"CONOUT$", L"w", stdout);
	std::wcin.clear();
	std::wcout.clear();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	SetConsoleTitle(L"LoLUpdater-Autoupdater");

	PCombine(autoupdater[0], cwd, L"version.txt");
	if (URLDownloadToFile(nullptr, L"https://lolupdater.com/downloads/version.txt", autoupdater[0], 0, nullptr) == S_OK)
	{
		std::wifstream in(autoupdater[0]);
		std::wstringstream buffer;
		buffer << in.rdbuf();
		Version latest(buffer.str());

		Version current(fileversion);


		PCombine(autoupdater[2], cwd, L"LoLUpdaterUpdated.zip");
		if (current < latest)
		{

			PCombine(autoupdater[1], cwd, L"Changelog.txt");
			if (URLDownloadToFile(nullptr, L"https://lolupdater.com/downloads/changelog.txt", autoupdater[1], 0, nullptr) == S_OK)
			{
				std::wifstream file(autoupdater[1]);
				if (file.good())
				{
					std::wstringstream bufferz;

					bufferz << file.rdbuf();

					file.close();

					std::wcout << bufferz.str().c_str() << std::endl;
				}
			}
			DeleteFile(autoupdater[1]);
			std::wstring dialoge;
			dialoge += L"Download Update v";
			dialoge += buffer.str();
			dialoge += L"?";
			if (MessageBox(nullptr, dialoge.c_str(), L"LoLUpdater", MB_YESNO) == IDYES)
			{
				CallbackHandler callbackHandler;
				IBindStatusCallback* pBindStatusCallback = nullptr;
				callbackHandler.QueryInterface(IID_IBindStatusCallback,
					reinterpret_cast<void**>(&pBindStatusCallback));

				if (URLDownloadToFile(nullptr, L"https://lolupdater.com/downloads/LoLUpdater.zip", autoupdater[2], 0, pBindStatusCallback) == S_OK)
				{
					std::wstring test = L"Update saved to: ";
					test += autoupdater[2];
					UnblockFile(autoupdater[2]);
					MessageBox(nullptr, test.c_str(), L"LoLUpdater", MB_OK);
				}
			}


		}
		else
		{
			std::wstring uptodate;
			uptodate += L"LoLUpdater is up to date: v";
			uptodate += fileversion;
			std::wcout << uptodate.c_str() << std::endl;
		}
	}


	DeleteFile(autoupdater[0]);
	WNDCLASSEX wc{ sizeof(WNDCLASSEX), CS_DROPSHADOW | CS_PARENTDC, WndProc, 0, 0, hInstance, LoadIcon(hInstance, MAKEINTRESOURCE(MAINICON)), nullptr, nullptr, nullptr, L"mainwindow", LoadIcon(hInstance, MAKEINTRESOURCE(MAINICON)) };

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"mainwindow", L"LoLUpdater", WS_TILEDWINDOW ^ WS_THICKFRAME | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 525, 150, nullptr, nullptr, hInstance, nullptr);


	DWORD dwStyle = GetWindowLongPtr(hwnd, GWL_STYLE);
	dwStyle &= ~WS_MAXIMIZEBOX;
	SetWindowLongPtr(hwnd, GWL_STYLE, dwStyle);




	if (LoL())
	{
		PCombine(client[0], loldir, rads.c_str());
		PAppend(client[0], proj.c_str());
		PAppend(client[0], L"lol_air_client");
		if (dirExists(client[0]))
		{
			rclient = true;
			PAppend(client[0], rel.c_str());
			del(std::wstring(client[0] + wildcard).c_str());

			FindLatest(client[0]);
			PAppend(client[0], dep.c_str());

			PCombine(client[3], client[0], adobef);
			PAppend(client[3], L"Versions");
			PAppend(client[3], L"1.0");

			PCombine(adobe[0], client[3], adobef);
			wcsncat_s(adobe[0], _countof(adobe[0]), DLL.c_str(), _TRUNCATE);

			PCombine(adobe[1], client[3], L"Resources");
			PAppend(adobe[1], files[10]);
		}

		PCombine(client[1], loldir, rads.c_str());
		PAppend(client[1], proj.c_str());
		PAppend(client[1], L"lol_patcher");
		PAppend(client[1], rel.c_str());
		del(std::wstring(client[1] + wildcard).c_str());
		FindLatest(client[1]);
		PAppend(client[1], dep.c_str());

		PCombine(client[2], loldir, rads.c_str());
		PAppend(client[2], L"solutions");
		PAppend(client[2], L"lol_game_client_sln");
		PAppend(client[2], rel.c_str());
		del(std::wstring(client[2] + wildcard).c_str());
		FindLatest(client[2]);
		PAppend(client[2], dep.c_str());

		PCombine(client[4], loldir, rads.c_str());
		PAppend(client[4], proj.c_str());
		PAppend(client[4], L"league_client");
		PAppend(client[4], rel.c_str());
		if (dirExists(client[4]))
		{


			betaclient = true;
			*svc = '\0';
			PCombine(svc, client[4], L"installer");
			del(std::wstring(svc).c_str());
			del(std::wstring(client[4] + wildcard).c_str());

			FindLatest(client[4]);
			PAppend(client[4], dep.c_str());


		}
	}
	else
	{
		PCombine(client[2], loldir, L"Game");
		PCombine(client[0], loldir, L"Air");

		PCombine(client[3], client[0], adobef);
		PAppend(client[3], L"Versions");
		PAppend(client[3], L"1.0");



		PCombine(adobe[0], client[3], adobef);
		wcsncat_s(adobe[0], _countof(adobe[0]), DLL.c_str(), _TRUNCATE);

		PCombine(adobe[1], client[3], L"Resources");
		PAppend(adobe[1], files[10]);

		PCombine(gdx[0], client[2], L"D3DX9_39.dll");
		PCombine(gdx[1], client[2], L"D3DX9_43.dll");
	}

	PCombine(Reshade[0], cwd, L"Instructions.txt");
	PCombine(Reshade[1], cwd, L"ReShade_Setup_3.0.5.exe");
	PCombine(cgbuf[0], client[2], files[6]);
	PCombine(cgbuf[1], client[2], files[7]);
	PCombine(cgbuf[2], client[2], files[8]);
	PCombine(tbb, client[2], files[9]);

	PCombine(enb[0], client[2], L"key_codes.txt");
	PCombine(enb[1], client[2], L"enbseries.ini");
	PCombine(enb[2], client[2], L"enbseries.dll");
	PCombine(enb[3], client[2], L"enbmoonbump.tga");
	PCombine(enb[4], client[2], L"enbmoon.tga");
	PCombine(enb[5], client[2], L"ENBInjector.exe");
	PCombine(enb[6], client[2], L"enbeffect.fx");
	PCombine(enb[7], client[2], L"enbdetail.dds");
	PCombine(enb[8], client[2], L"enbclouds.fx");
	PCombine(enb[9], client[2], L"enbbloom.fx");
	PCombine(enb[10], client[2], L"enbinjector.ini");
	PCombine(enb[11], client[2], L"effectv1");
	PCombine(enb[12], client[2], L"effectv1gamecolors");
	PCombine(enb[13], client[2], L"effectv2");
	PCombine(enb[14], client[2], L"effectv2gamecolors");
	PCombine(enb[15], client[2], L"shaderinput");

	ShowWindow(hwnd, SW_SHOW);


	SendMessage(hWndComboBox, CB_SETCURSEL, 0, 0);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	FreeConsole();
	return static_cast<int>(Msg.wParam);
}