#include "stdafx.h"
#include "WindowBase.h"
#include "../Common/Utils.h"
#include "../AppManager.h"
#include "DialogBuilderEx.h"
#include "DialogBuilderCallbackEx.h"


CWindowBase::CWindowBase(LPCTSTR pszClass, LPCTSTR pszXML)
	: m_strClassName(pszClass), m_strXML(pszXML), m_popControl(NULL)
{

}

CWindowBase::~CWindowBase()
{
}

DuiLib::CDuiString CWindowBase::GetSkinFile()
{
	return m_strXML;
}

LPCTSTR CWindowBase::GetWindowClassName(void) const
{
	return m_strClassName;
}

LRESULT CWindowBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:  		lRes = OnCreate(wParam, lParam, bHandled);				break;
	case WM_CLOSE:  		lRes = OnClose(wParam, lParam, bHandled);				break;
	case WM_DESTROY:		lRes = OnDestroy(wParam, lParam, bHandled);		break;

	case WM_NCACTIVATE:     lRes = OnNcActivate(wParam, lParam, bHandled);	break;
	case WM_NCCALCSIZE:     lRes = OnNcCalcSize(wParam, lParam, bHandled);	break;
	case WM_NCPAINT:        lRes = OnNcPaint(wParam, lParam, bHandled);		break;
	case WM_NCHITTEST:      lRes = OnNcHitTest(wParam, lParam, bHandled);		break;
	case WM_GETMINMAXINFO:  lRes = OnGetMinMaxInfo(wParam, lParam, bHandled); break;
	case WM_SIZE:			lRes = OnSize(wParam, lParam, bHandled);				break;
	case WM_SYSCOMMAND:     lRes = OnSysCommand(wParam, lParam, bHandled);	break;
	case WM_TIMER:			lRes = OnTimer(wParam, lParam, bHandled);				break;

	case WM_KEYDOWN:        lRes = OnKeyDown(wParam, lParam, bHandled);		break;
	case WM_KILLFOCUS:      lRes = OnKillFocus(wParam, lParam, bHandled);		break;
	case WM_SETFOCUS:       lRes = OnSetFocus(wParam, lParam, bHandled);		break;
	case WM_LBUTTONUP:      lRes = OnLButtonUp(wParam, lParam, bHandled);		break;
	case WM_LBUTTONDOWN:    lRes = OnLButtonDown(wParam, lParam, bHandled);	break;
	case WM_DROPFILES:		lRes = OnDropfiles(wParam, lParam, bHandled);	break;

	default:
		bHandled = FALSE;
	}

	if (bHandled) return lRes;

	lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);

	if (bHandled) return lRes;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWindowBase::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;

	return 0;
}

LRESULT CWindowBase::OnCreate(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	lStyle = (lStyle ^ WS_CAPTION);
	lStyle = (lStyle ^ WS_THICKFRAME);
	SetWindowLong(m_hWnd, GWL_STYLE, lStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_PaintManager.Init(m_hWnd);

	TCHAR szSkin[MAX_PATH] = { 0 };
	lstrcpyn(szSkin, m_strXML, MAX_PATH);

	//CDialogBuilder builder;
	CDialogBuilder builder;
	//CDialogBuilderCallbackEx cb;
	
	CControlUI* pRoot = builder.Create(szSkin, 0, NULL, &m_PaintManager);

	if (!pRoot)
		return -1;

	m_PaintManager.AttachDialog(pRoot);
	m_PaintManager.AddNotifier(this);

	ShowWindow(true, true);

	return 0;
}

LRESULT CWindowBase::OnDestroy(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CWindowBase::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right;
	lpMMI->ptMaxSize.y = rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CWindowBase::OnNcActivate(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CWindowBase::OnNcCalcSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CWindowBase::OnNcPaint(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CWindowBase::OnNcHitTest(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
		if (pControl)
		{
			LPCTSTR pszClass = pControl->GetClass();
			if (_tcsicmp(pszClass, _T("VerticalLayoutUI")) == 0 
				|| _tcsicmp(pszClass, _T("HorizontalLayoutUI")) == 0)
			{
				return HTCAPTION;
			}
		}
	}
	return HTCLIENT;
}

LRESULT CWindowBase::OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CWindowBase::OnClose(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CWindowBase::OnSysCommand(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)
	{
		bHandled = TRUE;
		PostMessage(WM_NCDESTROY);
		::PostQuitMessage(0);
		return 0;
	}

	return CWindowWnd::HandleMessage(WM_SYSCOMMAND, wParam, lParam);
}

LRESULT CWindowBase::OnTimer(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CWindowBase::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			return ResponseDefaultKeyEvent(wParam);
		default:
			break;
		}
	}
	return FALSE;
}

void CWindowBase::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT)
		OnWindowInit();
	else if (msg.sType == DUI_MSGTYPE_CLICK)
		OnClick(msg);
	else if (msg.sType == DUI_MSGTYPE_KILLFOCUS)
		OnKillFocus(msg);
}

void CWindowBase::OnWindowInit()
{
	
}

void CWindowBase::OnClick(TNotifyUI& msg)
{
	// 处理点击事件标签
	LPCTSTR lpEvents = msg.pSender->GetCustomAttribute(UIEVENT_CUSTOM_ONCLICK);
	if (lpEvents) DispatchCustomEvents(msg.pSender, lpEvents);
}

void CWindowBase::OnKillFocus(TNotifyUI& msg)
{
	// 处理弹出菜单按钮焦点问题
	if (msg.pSender == m_popControl)
	{
		RECT rc = msg.pSender->GetClientPos();
		POINT pt = msg.ptMouse;
		if (!::PtInRect(&rc, pt))
		{
			msg.pSender->SetVisible(false);
			m_popControl = NULL;
		}
	}
}

void CWindowBase::OnFinalMessage(HWND hWnd)
{
	delete this;
}

LRESULT CWindowBase::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		Close();
		return TRUE;
	}

	return FALSE;
}

LRESULT CWindowBase::DispatchCustomEvents(CControlUI* pSender, LPCTSTR pszEvents)
{
	if (!pszEvents) return -1;
	LRESULT lr = 0;
	LPTSTR lpEvent = _tcsdup(pszEvents);
	
	LPTSTR seps = _T(";");
	LPTSTR nxtoken = NULL;
	LPTSTR token = _tcstok_s(lpEvent, seps, &nxtoken);
	while (token != NULL)
	{
		lr = DispatchCustomEvent(pSender, token);
		if (FAILED(lr)) goto LBC;
		token = _tcstok_s(NULL, seps, &nxtoken);
	}
LBC:
	free(lpEvent);
	return lr;
}

#pragma warning(disable : 4996)
LRESULT CWindowBase::DispatchCustomEvent(CControlUI* pSender, LPCTSTR pszEvent)
{
	TCHAR szName[64], szArg[64];
	if (_tcsicmp(pszEvent, UIEVENT_CUSTOM_CLOSE) == 0)
		Close(RTN_EVENT_CLOSE);
	else if (_tcsicmp(pszEvent, UIEVENT_CUSTOM_MIN) == 0)
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	else if (_tcsicmp(pszEvent, UIEVENT_CUSTOM_QUIT) == 0)
		PostMessage(WM_QUIT, 0, 0);
	else if (_tcsicmp(pszEvent, UIEVENT_CUSTOM_DIE) == 0)
		ExitProcess(RTN_EVENT_CLOSE);
	else if (_tcsicmp(pszEvent, UIEVENT_CUSTOM_HIDE) == 0)
		ShowWindow(false, false);
	else if (_tcsicmp(pszEvent, UIEVENT_CUSTOM_SHOW) == 0)
		ShowWindow(true, true);
	// 为什么_stscanf_s会出错？
	// else if (_stscanf_s(pszEvent, _T("%[^(]('%[^']')"), &szName, sizeof(szName), &szArg, sizeof(szArg)) > 0)
	else if (_stscanf(pszEvent, _T("%[^(]('%[^']')"), &szName, &szArg) > 0)
	{
		// eg: show('menu')
		if (_tcsicmp(szName, UIEVENT_CUSTOM_SHOW) == 0)
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(szArg));
			if (pControl) pControl->SetVisible(true);
		}
		else if (_tcsicmp(szName, UIEVENT_CUSTOM_HIDE) == 0)
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(szArg));
			if (pControl) pControl->SetVisible(false);
		}
		else if (_tcsicmp(szName, UIEVENT_CUSTOM_POP) == 0)
		{
			if (m_popControl) m_popControl->SetVisible(false);
			m_popControl = static_cast<CControlUI*>(m_PaintManager.FindControl(szArg));
			if (m_popControl) {
				m_popControl->SetVisible(!m_popControl->IsVisible());
				if (m_popControl->IsVisible()) m_popControl->SetFocus();
			}
		}
		else if (_tcsicmp(szName, UIEVENT_CUSTOM_RETRY) == 0)
		{
			// 浏览器定义刷新
			if (pSender && _tcsicmp(pSender->GetClass(), DUI_CTR_WEBBROWSER) == 0) {
				CWebBrowserUI* pWeb = reinterpret_cast<CWebBrowserUI*>(pSender);
				LPCTSTR lpRetryedTime = pWeb->GetCustomAttribute(_T("_RetryedTime"));
				if (!lpRetryedTime) {
					lpRetryedTime = _T("0");
					pWeb->AddCustomAttribute(_T("_RetryedTime"), lpRetryedTime);
				}
				LPTSTR lpTmp = NULL;
				LONG lRetry = _tcstol(szArg, &lpTmp, 10);
				LONG lRetryed = _tcstol(lpRetryedTime, &lpTmp, 10);
				if (lRetryed < lRetry)
				{
					lRetryed++;
					TCHAR buf[4];
					_ltot(lRetryed, buf, 10);
					pWeb->RemoveCustomAttribute(_T("_RetryedTime"));
					pWeb->AddCustomAttribute(_T("_RetryedTime"), buf);
					pWeb->NavigateHomePage();
				}
				else {
					// die
					DispatchCustomEvent(pSender, UIEVENT_CUSTOM_DIE);
				}
			}
		}
		else if (_tcsicmp(szName, UIEVENT_CUSTOM_SLEEP) == 0)
		{
			LPTSTR pstr = NULL;
			::Sleep(_tcstol(szArg, &pstr, 10));
		}
		else
			return -1;
	}
	else {
		return -1;
	}

	return 0;
}

LRESULT CWindowBase::OnSetFocus(WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CWindowBase::OnLButtonDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CWindowBase::OnLButtonUp(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CWindowBase::OnDropfiles(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TCHAR szPath[_MAX_PATH];
	HDROP hDrop = (HDROP)wParam;
	UINT uCount = ::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	if (uCount > 0)
	{
		// 只取第一个
		::DragQueryFile(hDrop, 0, szPath, _MAX_PATH);
		CAppManager::getSingleton().LoadSkin(szPath);
	}
	::DragFinish(hDrop);
	return 0;
}

LRESULT CWindowBase::OnKeyDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == VK_DELETE)
	{
		PostQuitMessage(0);
	}
	else if (wParam == VK_F5)
	{
		CAppManager::getSingleton().RefreshSkin();
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CWindowBase::OnKillFocus(WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (m_popControl && m_popControl->IsVisible()) {
		m_popControl->SetVisible(false);
		m_popControl = NULL;
	}
	bHandled = FALSE;
	return 0;
}
