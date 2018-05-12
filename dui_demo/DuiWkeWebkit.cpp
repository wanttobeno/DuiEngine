#include "StdAfx.h"
#include "DuiWkeWebkit.h"
#include <Imm.h>
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"wke.lib")

namespace DuiEngine
{

	void CDuiWkeWebkit::WkeWebkit_Init()
	{
		wkeInit();
	}

	void CDuiWkeWebkit::WkeWebkit_Shutdown()
	{
		wkeShutdown();
	}

	CDuiWkeWebkit::CDuiWkeWebkit(void):m_pWebView(NULL)
	{
	}

	CDuiWkeWebkit::~CDuiWkeWebkit(void)
	{
	}

	void CDuiWkeWebkit::OnPaint( CDCHandle dc )
	{
		CRect rcClip;
		dc.GetClipBox(&rcClip);
		CRect rcClient;
		GetClient(&rcClient);
		CRect rcInvalid;
		rcInvalid.IntersectRect(&rcClip,&rcClient);

		m_pWebView->paint(dc,rcInvalid.left,rcInvalid.top,rcInvalid.Width(),rcInvalid.Height(),rcInvalid.left-rcClient.left,rcInvalid.top-rcClient.top,true);
	}

	void CDuiWkeWebkit::OnSize( UINT nType, CSize size )
	{
		m_pWebView->resize(size.cx,size.cy);
	}

	int CDuiWkeWebkit::OnCreate( void * )
	{
		m_pWebView = wkeCreateWebView();
		if(!m_pWebView) return 1;
		m_pWebView->setBufHandler(this);
		m_pWebView->loadURL(m_strUrl);
		SetDuiTimer(TM_TICKER,10);
		return 0;
	}

	void CDuiWkeWebkit::OnDestroy()
	{
		if(m_pWebView) wkeDestroyWebView(m_pWebView);
	}

	LRESULT CDuiWkeWebkit::OnMouseEvent( UINT message, WPARAM wParam,LPARAM lParam)
	{
		if (message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_RBUTTONDOWN)
		{
			SetDuiFocus();
			SetDuiCapture();
		}
		else if (message == WM_LBUTTONUP || message == WM_MBUTTONUP || message == WM_RBUTTONUP)
		{
			ReleaseDuiCapture();
		}

		CRect rcClient;
		GetClient(&rcClient);

		int x = GET_X_LPARAM(lParam)-rcClient.left;
		int y = GET_Y_LPARAM(lParam)-rcClient.top;

		unsigned int flags = 0;

		if (wParam & MK_CONTROL)
			flags |= WKE_CONTROL;
		if (wParam & MK_SHIFT)
			flags |= WKE_SHIFT;

		if (wParam & MK_LBUTTON)
			flags |= WKE_LBUTTON;
		if (wParam & MK_MBUTTON)
			flags |= WKE_MBUTTON;
		if (wParam & MK_RBUTTON)
			flags |= WKE_RBUTTON;

		bool bHandled = m_pWebView->mouseEvent(message, x, y, flags);
		SetMsgHandled(bHandled);
		return 0;
	}

	LRESULT CDuiWkeWebkit::OnKeyDown( UINT uMsg, WPARAM wParam,LPARAM lParam )
	{
		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)
			flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)
			flags |= WKE_EXTENDED;

		SetMsgHandled(m_pWebView->keyDown(wParam, flags, false));
		return 0;
	}

	LRESULT CDuiWkeWebkit::OnKeyUp( UINT uMsg, WPARAM wParam,LPARAM lParam )
	{
		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)
			flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)
			flags |= WKE_EXTENDED;

		SetMsgHandled(m_pWebView->keyUp(wParam, flags, false));
		return 0;
	}

	LRESULT CDuiWkeWebkit::OnMouseWheel( UINT uMsg, WPARAM wParam,LPARAM lParam )
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		int delta = GET_WHEEL_DELTA_WPARAM(wParam);

		unsigned int flags = 0;

		if (wParam & MK_CONTROL)
			flags |= WKE_CONTROL;
		if (wParam & MK_SHIFT)
			flags |= WKE_SHIFT;

		if (wParam & MK_LBUTTON)
			flags |= WKE_LBUTTON;
		if (wParam & MK_MBUTTON)
			flags |= WKE_MBUTTON;
		if (wParam & MK_RBUTTON)
			flags |= WKE_RBUTTON;

		//flags = wParam;

		BOOL handled = m_pWebView->mouseWheel(pt.x, pt.y, delta, flags);
		SetMsgHandled(handled);

		return handled;
	}

	LRESULT CDuiWkeWebkit::OnChar( UINT uMsg, WPARAM wParam,LPARAM lParam )
	{
		unsigned int charCode = wParam;
		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)
			flags |= WKE_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)
			flags |= WKE_EXTENDED;

		//flags = HIWORD(lParam);

		SetMsgHandled(m_pWebView->keyPress(charCode, flags, false));
		return 0;
	}

	LRESULT CDuiWkeWebkit::OnImeStartComposition( UINT uMsg, WPARAM wParam,LPARAM lParam )
	{
		wkeRect caret = m_pWebView->getCaret();

		CRect rcClient;
		GetClient(&rcClient);

		CANDIDATEFORM form;
		form.dwIndex = 0;
		form.dwStyle = CFS_EXCLUDE;
		form.ptCurrentPos.x = caret.x + rcClient.left;
		form.ptCurrentPos.y = caret.y + caret.h + rcClient.top;
		form.rcArea.top = caret.y + rcClient.top;
		form.rcArea.bottom = caret.y + caret.h + rcClient.top;
		form.rcArea.left = caret.x + rcClient.left;
		form.rcArea.right = caret.x + caret.w + rcClient.left;
		COMPOSITIONFORM compForm;
		compForm.ptCurrentPos=form.ptCurrentPos;
		compForm.rcArea=form.rcArea;
		compForm.dwStyle=CFS_POINT;

		HWND hWnd=GetContainer()->GetHostHwnd();
		HIMC hIMC = ImmGetContext(hWnd);
		ImmSetCandidateWindow(hIMC, &form);
		ImmSetCompositionWindow(hIMC,&compForm);
		ImmReleaseContext(hWnd, hIMC);
		return 0;
	}

	void CDuiWkeWebkit::OnSetDuiFocus()
	{
		m_pWebView->focus();
	}

	void CDuiWkeWebkit::OnKillDuiFocus()
	{
		m_pWebView->unfocus();
	}

	void CDuiWkeWebkit::OnDuiTimer( char cTimerID )
	{
		if(cTimerID==TM_TICKER)
		{
			m_pWebView->tick();
		}
	}

	void CDuiWkeWebkit::onBufUpdated( const HDC hdc,int x, int y, int cx, int cy )
	{
		CRect rcClient;
		GetClient(&rcClient);
		CRect rcInvalid(CPoint(x,y),CSize(cx,cy));
		rcInvalid.OffsetRect(rcClient.TopLeft());
		NotifyInvalidateRect(rcInvalid);
	}

	BOOL CDuiWkeWebkit::OnDuiSetCursor( const CPoint &pt )
	{
		return TRUE;
	}

	BOOL CDuiWkeWebkit::OnAttrUrl( CDuiStringA strValue, BOOL bLoading )
	{
		m_strUrl=DUI_CA2W(strValue);
		if(!bLoading) m_pWebView->loadURL(m_strUrl);
		return !bLoading;
	}
}
