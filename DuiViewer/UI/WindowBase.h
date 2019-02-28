#pragma once

class CWindowBase : public CWindowWnd, public INotifyUI
{
public:
	// 强制定义类名，弹窗拦截规则相关
	explicit CWindowBase(LPCTSTR pszClass, LPCTSTR pszXML);
	~CWindowBase();

	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual HWND Create(HWND hwndParent, LPCTSTR pstrName);
public:
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcActivate(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcCalcSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcPaint(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcHitTest(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDropfiles(WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual void	Notify(TNotifyUI& msg);
	virtual void	OnWindowInit();
	virtual void	OnClick(TNotifyUI& msg);
	virtual void	OnKillFocus(TNotifyUI& msg);
protected:
	virtual void	OnFinalMessage(HWND hWnd);
protected:
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	// 根据名称调用事件，批量处理与单条处理，";"符号分割
	virtual LRESULT DispatchCustomEvents(CControlUI* pSender, LPCTSTR pszEvents);
	virtual LRESULT DispatchCustomEvent(CControlUI* pSender, LPCTSTR pszEvent);
	// 子类化回调
protected:
	CPaintManagerUI		m_PaintManager;
	CDuiString			m_strXML;
	CDuiString			m_strClassName;

	CControlUI*			m_popControl;		// 记录置顶弹出控件
private:
};