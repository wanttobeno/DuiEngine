#pragma once

#include "duiwnd.h"
#include "wtl.mini/duicoll.h"
namespace DuiEngine
{
#define DUIHDI_WIDTH               0x0001
#define DUIHDI_TEXT                0x0002
#define DUIHDI_SORTFLAG			   0x0004
#define DUIHDI_LPARAM              0x0008
#define DUIHDI_ORDER			   0x0010

	typedef enum _DUIHDSORTFLAG{
		ST_NULL=0,
		ST_UP,
		ST_DOWN,
	}DUIHDSORTFLAG;

	typedef struct _DUIHDITEM {
		UINT    mask; 
		int     cx; 
		LPTSTR  pszText; 
		int     cchTextMax; 
		DUIHDSORTFLAG stFlag;
		LPARAM  lParam; 
		UINT   state;
		int		iOrder;
	} DUIHDITEM,  *LPDUIHDITEM;


	class DUI_EXP CDuiHeaderCtrl: public CDuiWindow
	{
		DUIOBJ_DECLARE_CLASS_NAME(CDuiHeaderCtrl, "header")
	public:
		CDuiHeaderCtrl(void);
		~CDuiHeaderCtrl(void);

		int InsertItem( int iItem,LPCTSTR pszText,int nWidth, DUIHDSORTFLAG stFlag,LPARAM lParam );
		BOOL GetItem(int iItem,DUIHDITEM *pItem);

		int GetItemCount(){return m_arrItems.GetCount();}
		int GetTotalWidth();
		int GetItemWidth(int iItem);

		BOOL DeleteItem(int iItem);
		void DeleteAllItems();

		DUIWIN_DECLARE_ATTRIBUTES_BEGIN()
			DUIWIN_SKIN_ATTRIBUTE("itemSkin",m_pSkinItem,FALSE)
			DUIWIN_SKIN_ATTRIBUTE("sortSkin",m_pSkinSort,FALSE)
			DUIWIN_INT_ATTRIBUTE("fixWidth",m_bFixWidth,FALSE)
			DUIWIN_INT_ATTRIBUTE("itemSwapEnable",m_bItemSwapEnable,FALSE)
			DUIWIN_INT_ATTRIBUTE("sortHeader",m_bSortHeader,FALSE)
		DUIWIN_DECLARE_ATTRIBUTES_END()
	protected:
		virtual BOOL LoadChildren(pugi::xml_node xmlNode);
		virtual BOOL OnDuiSetCursor(const CPoint &pt);

		void OnPaint(CDCHandle dc);
		virtual void DrawItem(CDCHandle dc,CRect rcItem,const LPDUIHDITEM pItem);
		CRect	GetItemRect(int iItem);
		//鼠标位置分析，返回一个DWORD，LOWORD为前面的项，HIWORD为后面的项，相同时代表在一个项的中间
		DWORD  HitTest(CPoint pt);
		void RedrawItem(int iItem);

		void DrawDraggingState(DWORD dwDragTo);

		BOOL IsItemHover(DWORD dwHitTest)
		{
			return dwHitTest!=-1 && LOWORD(dwHitTest)==HIWORD(dwHitTest);
		}

		HBITMAP CreateDragImage(int iItem);

		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();
		void OnDestroy();
		
		DUIWIN_BEGIN_MSG_MAP()
			MSG_WM_PAINT(OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_DESTROY(OnDestroy)
		DUIWIN_END_MSG_MAP()

		CDuiSkinBase *	m_pSkinItem;
		CDuiSkinBase *	m_pSkinSort;
		BOOL			m_bSortHeader;		//表头可以点击排序
		BOOL			m_bFixWidth;		//表项宽度固定开关
		BOOL			m_bItemSwapEnable;//允许拖动调整位置开关

		BOOL			m_bDragging;
		HBITMAP			m_hDragImg;
		CPoint			m_ptClick;
		DWORD			m_dwHitTest;
		DWORD			m_dwDragTo;
		
		int				m_nAdjItemOldWidth;//保存被拖动项的原始宽度
		CDuiArray<DUIHDITEM> m_arrItems;
	};
}//end of namespace DuiEngine
