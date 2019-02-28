#include "stdafx.h"
#include <shlwapi.h>
#include "AppManager.h"
#include "Common/Utils.h"
#include "UI/WindowBase.h"

#pragma comment(lib, "Shlwapi.lib")


template<> CAppManager* CSingleton<CAppManager>::ms_Singleton = NULL;

#pragma warning(disable:4996)
CAppManager::CAppManager()
	:m_WinBase(NULL), m_lpResourceZIPBuffer(NULL)
{
	TCHAR szCurrentDir[MAX_PATH + 1] = { 0 };
	GetCurrentDirectory(MAX_PATH, szCurrentDir);
	LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
	_tcscpy(lpInsertPos, _T("\\"));
	m_AppPath = szCurrentDir;
}

CAppManager::~CAppManager()
{
	if (m_lpResourceZIPBuffer) delete m_lpResourceZIPBuffer;
}

BOOL CAppManager::LoadSkin(LPCTSTR lpcSkin)
{
	if (!lpcSkin || _tcslen(lpcSkin) == 0) return FALSE;
	TCHAR szBuf[MAX_PATH] = {0};
	::PathCombine(szBuf, m_AppPath.c_str(), lpcSkin);
	TString strSkin = szBuf;
	if (strSkin.empty()) return FALSE;
	
	size_t ps = strSkin.find_last_of(_T("\\"));
	if (ps == TString::npos) return FALSE;

	m_strSkinPath = strSkin.substr(0, ps + 1);
	m_strSkinName = strSkin.substr(ps + 1);

	if (m_strSkinPath.empty() || m_strSkinName.empty())
		return FALSE;

	CPaintManagerUI::SetResourceClear();
	//CPaintManagerUI::SetResourceType(UILIB_FILE);
	CPaintManagerUI::SetResourcePath(m_strSkinPath.c_str());
	//CPaintManagerUI::ReloadSkin();
	
	CWindowBase *pWin = new CWindowBase(_T("DuiViewer"), m_strSkinName.c_str());
	HWND h = pWin->Create(0, _T("DuiViewer"), UI_WNDSTYLE_FRAME^WS_MAXIMIZEBOX^WS_VISIBLE,
		UI_WNDSTYLE_EX_FRAME | WS_EX_ACCEPTFILES);

	if (!::IsWindow(h))
	{
		::MessageBox(0, _T("Æ¤·ô¼ÓÔØÊ§°Ü£¡"), _T("´íÎó"), MB_OK);
		delete pWin;
		return FALSE;
	}

	if (m_WinBase)
		m_WinBase->Close(0);
	m_WinBase = pWin;
	m_WinBase->CenterWindow();
	m_WinBase->ShowWindow(true);

	return TRUE;
}

BOOL CAppManager::LoadDefaultSkin()
{
	//´Ó×ÊÔ´ÖÐ¼ÓÔØzip
	HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), MAKEINTRESOURCE(IDR_ZIP1), _T("ZIP"));
	if (hResource == NULL)
		return FALSE;
	DWORD dwSize = 0;
	HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
	if (hGlobal == NULL)
	{
		::FreeResource(hResource);
		return FALSE;
	}
	dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
	if (dwSize == 0)
		return FALSE;
	m_lpResourceZIPBuffer = new BYTE[dwSize];
	if (m_lpResourceZIPBuffer != NULL)
	{
		::CopyMemory(m_lpResourceZIPBuffer, (LPBYTE)::LockResource(hGlobal), dwSize);
	}
	::FreeResource(hResource);
	CPaintManagerUI::SetResourceZip(m_lpResourceZIPBuffer, dwSize);

	m_WinBase = new CWindowBase(_T("DuiViewer"), _T("skin.xml"));
	m_WinBase->Create(0, _T("DuiViewer"), UI_WNDSTYLE_FRAME^WS_MAXIMIZEBOX^WS_VISIBLE, 
		UI_WNDSTYLE_EX_FRAME| WS_EX_ACCEPTFILES);
	m_WinBase->CenterWindow();
	m_WinBase->ShowWindow(true);

	return TRUE;
}

BOOL CAppManager::RefreshSkin()
{
	if (m_strSkinName.empty())
		return FALSE;

	CWindowBase *pWin = new CWindowBase(_T("DuiViewer"), m_strSkinName.c_str());
	HWND h = pWin->Create(0, _T("DuiViewer"), UI_WNDSTYLE_FRAME^WS_MAXIMIZEBOX^WS_VISIBLE, 
		UI_WNDSTYLE_EX_FRAME| WS_EX_ACCEPTFILES);
	
	if (!::IsWindow(h))
	{
		::MessageBox(0, _T("Æ¤·ô¼ÓÔØÊ§°Ü£¡"), _T("´íÎó"), MB_OK);
		delete pWin;
		return FALSE;
	}

	if (m_WinBase)
		m_WinBase->Close(0);
	m_WinBase = pWin;
	m_WinBase->CenterWindow();
	m_WinBase->ShowWindow(true);

	return TRUE;
}
