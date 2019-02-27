#pragma once
#include "stdafx.h"
#include "Common/Singleton.h"
#include "UI/WindowBase.h"

// ���ڼ��ع����࣬����root.xml
class CAppManager : public CSingleton<CAppManager>
{
public:
	CAppManager();
	~CAppManager();
public:
	// ����Ƥ��
	BOOL		LoadSkin(LPCTSTR lpcSkin);
	// ���¼���Ƥ��
	BOOL		RefreshSkin();
protected:
private:
	TString		m_strSkinPath;
	TString		m_strSkinName;
	CWindowBase	*m_WinBase;
	TString		m_AppPath;
};