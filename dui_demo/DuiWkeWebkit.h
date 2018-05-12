#pragma once
#include "wke.h"
namespace DuiEngine
{
	const char TM_TICKER = 1;
	class CDuiWkeWebkit : public CDuiWindow, protected wkeBufHandler
	{
		DUIOBJ_DECLARE_CLASS_NAME(CDuiWindow, "wkeWebkit")
	public:
		CDuiWkeWebkit(void);
		~CDuiWkeWebkit(void);
		static void WkeWebkit_Init();
		static void WkeWebkit_Shutdown();

		wkeWebView	GetWebView(){return m_pWebView;}
	protected:
		virtual void onBufUpdated (const HDC hdc,int x, int y, int cx, int cy);
	protected:
		int OnCreate(void *);
		void OnDestroy();
		void OnPaint(CDCHandle dc);
		void OnSize(UINT nType, CSize size);
		LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam,LPARAM lParam); 
		LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam,LPARAM lParam); 
		LRESULT OnKeyDown(UINT uMsg, WPARAM wParam,LPARAM lParam);
		LRESULT OnKeyUp(UINT uMsg, WPARAM wParam,LPARAM lParam);
		LRESULT OnChar(UINT uMsg, WPARAM wParam,LPARAM lParam);
		LRESULT OnImeStartComposition(UINT uMsg, WPARAM wParam,LPARAM lParam);
		void OnSetDuiFocus();
		void OnKillDuiFocus();
		void OnDuiTimer(char cTimerID);

		virtual BOOL OnDuiSetCursor(const CPoint &pt);
		BOOL OnAttrUrl(CDuiStringA strValue, BOOL bLoading);
	protected:
		DUIWIN_DECLARE_ATTRIBUTES_BEGIN()
			DUIWIN_CUSTOM_ATTRIBUTE("url",OnAttrUrl)
		DUIWIN_DECLARE_ATTRIBUTES_END()

		DUIWIN_BEGIN_MSG_MAP()
			MSG_WM_PAINT(OnPaint)
			MSG_WM_CREATE(OnCreate)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_SIZE(OnSize)
			MSG_WM_DUITIMER(OnDuiTimer)
			MSG_WM_SETFOCUS_EX(OnSetDuiFocus)
			MSG_WM_KILLFOCUS_EX(OnKillDuiFocus)
			MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST,0x209,OnMouseEvent)
			MESSAGE_HANDLER_EX(WM_MOUSEWHEEL,OnMouseWheel)
			MESSAGE_HANDLER_EX(WM_KEYDOWN,OnKeyDown)
			MESSAGE_HANDLER_EX(WM_KEYUP,OnKeyUp)
			MESSAGE_HANDLER_EX(WM_CHAR,OnChar)
			MESSAGE_HANDLER_EX(WM_IME_STARTCOMPOSITION,OnImeStartComposition)
		DUIWIN_END_MSG_MAP()

		wkeWebView m_pWebView;
		CDuiStringW m_strUrl;
	};
}
