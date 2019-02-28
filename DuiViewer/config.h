#pragma once

// 定义全局通用自定义消息
#define WM_USER_BASE			WM_USER + 500


typedef enum _tagCloseMethod
{
	RTN_BTN_CLOSE = 100,			// 关闭按钮
	RTN_BTN_MENU_CLOSE = 101,		// 菜单关闭按钮
	RTN_EVENT_CLOSE = 102,			// 自定义关闭事件
} CCloseMethod;

// 定义标签事件，不区分大小写
#define UIEVENT_CUSTOM_ONCLICK				_T("onclick")

// 定义事件名称，不区分大小写
#define UIEVENT_CUSTOM_CLOSE			_T("close")
#define UIEVENT_CUSTOM_MIN				_T("min")
#define UIEVENT_CUSTOM_QUIT				_T("quit")
#define UIEVENT_CUSTOM_DIE				_T("die")
#define UIEVENT_CUSTOM_SHOW				_T("show")	// show('img_')
#define UIEVENT_CUSTOM_HIDE				_T("hide")	// hide('img_')
#define UIEVENT_CUSTOM_POP				_T("pop")	// pop('img_') 弹出控件，失去焦点时自动隐藏
#define UIEVENT_CUSTOM_SLEEP			_T("sleep")	// sleep(1000)
