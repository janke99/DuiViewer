// DuiViewer.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DuiViewer.h"
#include "AppManager.h"

void HelpOut()
{
	::MessageBox(0, _T("请拖放将(*.xml)文件拖放到工具打开！"), _T(""), MB_OK);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	HRESULT Hr = ::CoInitialize(0);
	if (FAILED(Hr)) return -1;

	CPaintManagerUI::SetInstance(hInstance);
	CAppManager *pApp = new CAppManager;

	if (_tcslen(lpCmdLine) != 0)
	{
		if (!pApp->LoadSkin(lpCmdLine))
		{
			HelpOut();
			return -1;
		}
	}else
		pApp->LoadDefaultSkin();

	CPaintManagerUI::MessageLoop();
	delete pApp;
	return 0;

}