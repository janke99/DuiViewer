#pragma once

class CDialogBuilderCallbackEx: public IDialogBuilderCallback
{
public:
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		return NULL;
	}
};