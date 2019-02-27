#include "stdafx.h"
#include "AppManager.h"
#include "Common/Utils.h"
#include "UI/WindowBase.h"



template<> CAppManager* CSingleton<CAppManager>::ms_Singleton = NULL;

CAppManager::CAppManager()
	:m_WinBase(NULL)
{

}

CAppManager::~CAppManager()
{

}

BOOL CAppManager::LoadSkin(LPCTSTR lpcSkin)
{
	TString strSkin = lpcSkin;
	if (strSkin.empty()) return FALSE;
	size_t ps = strSkin.find_last_of(_T("\\"));
	if (ps == TString::npos) return FALSE;

	m_strSkinPath = strSkin.substr(0, ps + 1);
	m_strSkinName = strSkin.substr(ps + 1);

	if (m_strSkinPath.empty() || m_strSkinName.empty())
		return FALSE;

	CPaintManagerUI::SetResourcePath(m_strSkinPath.c_str());
	
	m_WinBase = new CWindowBase(_T("DuiViewer"), m_strSkinName.c_str());
	m_WinBase->Create(0, _T("DuiViewer"), UI_WNDSTYLE_FRAME^WS_MAXIMIZEBOX^WS_VISIBLE, UI_WNDSTYLE_EX_FRAME);
	m_WinBase->CenterWindow();
	m_WinBase->ShowWindow(true);

	return TRUE;
}

BOOL CAppManager::RefreshSkin()
{
	if (m_WinBase)
	{
		m_WinBase->Close(0);
		m_WinBase = NULL;
	}

	m_WinBase = new CWindowBase(_T("DuiViewer"), m_strSkinName.c_str());
	m_WinBase->Create(0, _T("DuiViewer"), UI_WNDSTYLE_FRAME^WS_MAXIMIZEBOX^WS_VISIBLE, UI_WNDSTYLE_EX_FRAME);
	m_WinBase->CenterWindow();
	m_WinBase->ShowWindow(true);

	return TRUE;
}
