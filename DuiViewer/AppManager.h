#pragma once
#include "stdafx.h"
#include "Common/Singleton.h"
#include "UI/WindowBase.h"

// 窗口加载管理类，加载root.xml
class CAppManager : public CSingleton<CAppManager>
{
public:
	CAppManager();
	~CAppManager();
public:
	// 加载皮肤
	BOOL		LoadSkin(LPCTSTR lpcSkin);
	// 重新加载皮肤
	BOOL		RefreshSkin();
protected:
private:
	TString		m_strSkinPath;
	TString		m_strSkinName;
	CWindowBase	*m_WinBase;
	TString		m_AppPath;
};