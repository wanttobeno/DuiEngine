//一个通用的下拉窗口模块
#pragma once

#include "duihostwnd.h"

namespace DuiEngine
{

	class CDuiDropDownWnd;
	class DUI_EXP IDuiDropDownOwner
	{
	public:
		virtual CDuiWindow * GetDropDownOwner() =0;
		virtual void OnDropDown(CDuiDropDownWnd *pDropDown) = 0;
		virtual void OnCloseUp(CDuiDropDownWnd *pDropDown,UINT uCode) = 0;
	};

	///////////////////////////////////////////////////////////////////////////////////////////

	class DUI_EXP CDuiDropDownWnd : public CDuiHostWnd , public CDuiMessageFilter 
	{
	public:
		CDuiDropDownWnd(IDuiDropDownOwner* pOwner);
		virtual ~CDuiDropDownWnd();

		virtual BOOL Create(LPCRECT lpRect,LPVOID lParam,DWORD dwStyle=WS_POPUP,DWORD dwExStyle=WS_EX_TOOLWINDOW|WS_EX_TOPMOST);
		void EndDropDown(UINT uCode=IDCANCEL);
	protected:
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnLButtonUp(UINT nFlags, CPoint point);
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnDestroy();
        void OnActivate(UINT nState, BOOL bMinimized, HWND wndOther){}//阻止窗口激活
		void OnActivateApp(BOOL bActive, DWORD dwThreadID);
		int OnMouseActivate(HWND wndTopLevel, UINT nHitTest, UINT message);
        void OnClose();

 		virtual void OnFinalMessage(HWND){delete this;}

		virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
		IDuiDropDownOwner* m_pOwner;
		BOOL				m_bClick;
		UINT				m_uExitCode;

		BEGIN_MSG_MAP_EX(CDuiDropDownWnd)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_KEYDOWN(OnKeyDown);
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_CLOSE(OnClose)
			MSG_WM_ACTIVATE(OnActivate)
			MSG_WM_ACTIVATEAPP(OnActivateApp)
			MSG_WM_MOUSEACTIVATE(OnMouseActivate)
			CHAIN_MSG_MAP(CDuiHostWnd)
		END_MSG_MAP()
	};

}//end of namespace DuiEngine