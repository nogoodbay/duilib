#ifndef __UICONTROL_H__
#define __UICONTROL_H__

#pragma once

namespace DuiLib {

/////////////////////////////////////////////////////////////////////////////////////
//

typedef CControlUI* (CALLBACK* FINDCONTROLPROC)(CControlUI*, LPVOID);

class DUILIB_API CControlUI
{
public:
    CControlUI();
    virtual void Delete();

protected:
    virtual ~CControlUI();

public:
    virtual CDuiString GetName() const;
    virtual void SetName(LPCTSTR pstrName);

    virtual void SetCover(CControlUI* pControl);
    
    virtual UINT GetControlFlags() const;
	virtual HWND GetNativeWindow() const;
    virtual LPCTSTR GetClass() const;
    virtual LPVOID GetInterface(LPCTSTR pstrName);
    //virtual bool Activate();
    
    
    virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
    // virtual CControlUI* GetParent() const;
    DWORD GetBkColor2() const;

    virtual CControlUI* GetCover() const;
    //virtual void SetCover(CControlUI *pControl);

    
    // �ı����
    virtual CDuiString GetText() const;
    // virtual void SetText(LPCTSTR pstrText);
    virtual int GetHeight() const;

    virtual bool Activate();

    virtual CPaintManagerUI* GetManager() const;

    // λ�����
    //virtual const RECT& GetPos() const;
	virtual RECT GetRelativePos() const; // ���(���ؼ�)λ��
	virtual RECT GetClientPos() const; // �ͻ����򣨳�ȥscrollbar��inset��

    virtual CControlUI* GetParent() const;

	// ֻ�пؼ�Ϊfloat��ʱ���ⲿ����SetPos��Move������Ч�ģ�λ�ò�������Ը��ؼ���λ��
    virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
    virtual void SetText(wchar_t* pstrText);
	virtual void Move(SIZE szOffset, bool bNeedInvalidate = true);

    virtual int GetWidth() const;

    //virtual int GetHeight() const;
    virtual const RECT& GetPos() const;
    virtual int GetX() const;
    virtual int GetY() const;
    virtual RECT GetPadding() const;
    virtual void SetPadding(RECT rcPadding); // ������߾࣬���ϲ㴰�ڻ���
    virtual SIZE GetFixedXY() const;         // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
    virtual void SetFixedXY(SIZE szXY);      // ��floatΪtrueʱ��Ч
	virtual TPercentInfo GetFloatPercent() const;
	virtual void SetFloatPercent(TPercentInfo piFloatPercent);
    virtual int GetFixedWidth() const;       // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
    virtual void SetFixedWidth(int cx);      // Ԥ��Ĳο�ֵ
    virtual int GetFixedHeight() const;      // ʵ�ʴ�Сλ��ʹ��GetPos��ȡ������õ�����Ԥ��Ĳο�ֵ
    virtual void SetFixedHeight(int cy);     // Ԥ��Ĳο�ֵ
    virtual int GetMinWidth() const;
    virtual void SetMinWidth(int cx);
    virtual int GetMaxWidth() const;
    virtual void SetMaxWidth(int cx);
    virtual int GetMinHeight() const;
    virtual void SetMinHeight(int cy);
    virtual int GetMaxHeight() const;
    virtual void SetMaxHeight(int cy);

    // �����ʾ
    // virtual CDuiString GetToolTip() const;
    virtual void SetToolTip(LPCTSTR pstrText);
	virtual void SetToolTipWidth(int nWidth);
	virtual int	  GetToolTipWidth(void);	// ����ToolTip��������

    // ��ݼ�
    virtual TCHAR GetShortcut() const;
    virtual void SetShortcut(TCHAR ch);

    // �˵�
    virtual bool IsContextMenuUsed() const;
    virtual void SetContextMenuUsed(bool bMenuUsed);

    // �û�����
    virtual const CDuiString& GetUserData(); // �������������û�ʹ��
    virtual void SetUserData(LPCTSTR pstrText); // �������������û�ʹ��
    virtual UINT_PTR GetTag() const; // �������������û�ʹ��

    virtual CDuiString GetToolTip() const;

    virtual void SetTag(UINT_PTR pTag); // �������������û�ʹ��

    // һЩ��Ҫ������
    // virtual bool IsVisible() const;
    virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);



    virtual void SetVisible(bool bVisible = true);
    virtual void SetInternVisible(bool bVisible = true); // �����ڲ����ã���ЩUIӵ�д��ھ������Ҫ��д�˺���
    virtual bool IsEnabled() const;
    
    // virtual void SetEnabled(bool bEnable = true);
    virtual bool IsMouseEnabled() const;
    virtual void SetMouseEnabled(bool bEnable = true);
    virtual bool IsKeyboardEnabled() const;
    virtual void SetKeyboardEnabled(bool bEnable = true);
    // virtual bool IsFocused() const;
    //virtual void SetFocus();

    virtual bool IsFloat() const;
    virtual void SetFloat(bool bFloat = true);

	// �Զ���(δ�����)����
	void AddCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr);
	LPCTSTR GetCustomAttribute(LPCTSTR pstrName) const;
	bool RemoveCustomAttribute(LPCTSTR pstrName);
	void RemoveAllCustomAttribute();

    virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

    void Invalidate();
    bool IsUpdateNeeded() const;
    void NeedUpdate();
    void NeedParentUpdate();
    DWORD GetAdjustColor(DWORD dwColor);

    virtual void Init();
    virtual void DoInit();

    virtual CDuiString GetAttribute(LPCTSTR pstrName);
    virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	virtual CDuiString GetAttributeList(bool bIgnoreDefault = true);
    virtual void SetAttributeList(LPCTSTR pstrList);

    virtual SIZE EstimateSize(SIZE szAvailable);

    
    virtual bool IsVisible() const;

	virtual bool Paint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl=NULL); // ����Ҫ��Ҫ��������
    
    // virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

    virtual void PaintBkColor(HDC hDC);
    virtual void PaintBkImage(HDC hDC);
    virtual void PaintStatusImage(HDC hDC);

    virtual void SetEnabled(bool bEnable = true);

    virtual void PaintText(HDC hDC);
    virtual void PaintBorder(HDC hDC);

    

    virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);

    //virtual void SetPlaceHolder2();
    virtual void SetPlaceHolder0();

	//���ⴰ�ڲ���
	void SetVirtualWnd(LPCTSTR pstrValue);
    virtual bool IsFocused() const;

    // ռλ����
    virtual void SetPlaceHolder();
    virtual void SetPlaceHolder1();

    virtual void SetFocus();

    
    virtual void SetPlaceHolder3();
    virtual void SetPlaceHolder4();
    virtual void SetPlaceHolder5();
    virtual void SetPlaceHolder6();
    virtual void SetPlaceHolder7();
    virtual void SetPlaceHolder8();
    virtual void SetPlaceHolder9();
    virtual void SetPlaceHolder10();
    virtual void SetPlaceHolder11();
    virtual void SetPlaceHolder12();
    virtual void SetPlaceHolder13();
    virtual void SetPlaceHolder14();
    virtual void SetPlaceHolder15();
    virtual void SetPlaceHolder16();

    virtual void DoEvent(TEventUI& event);
    virtual void Event(TEventUI& event);




    // ͼ�����
    DWORD GetBkColor() const;
    void SetBkColor(DWORD dwBackColor);
    // DWORD GetBkColor2() const;


    void SetBkColor2(DWORD dwBackColor);
    DWORD GetBkColor3() const;
    void SetBkColor3(DWORD dwBackColor);
    LPCTSTR GetBkImage();
    void SetBkImage(LPCTSTR pStrImage);
    DWORD GetFocusBorderColor() const;
    void SetFocusBorderColor(DWORD dwBorderColor);
    bool IsColorHSL() const;
    void SetColorHSL(bool bColorHSL);
    SIZE GetBorderRound() const;
    void SetBorderRound(SIZE cxyRound);
    bool DrawImage(HDC hDC, TDrawInfo& drawInfo);

    //�߿����
    DWORD GetBorderColor() const;
    void SetBorderColor(DWORD dwBorderColor);
    RECT GetBorderSize() const;
    void SetBorderSize(RECT rc);
    void SetBorderSize(int iSize);
    int GetBorderStyle() const;
    void SetBorderStyle(int nStyle);

	CDuiString GetVirtualWnd() const;

public:
    CEventSource OnInit;
    CEventSource OnDestroy;
    CEventSource OnSize;
    CEventSource OnEvent;
    CEventSource OnNotify;
	CEventSource OnPaint;
	CEventSource OnPostPaint;

protected:
    CPaintManagerUI* m_pManager;
    CControlUI* m_pParent;
    CControlUI* m_pCover;
	CDuiString m_sVirtualWnd;
    CDuiString m_sName;
    bool m_bUpdateNeeded;
    bool m_bMenuUsed;
	bool m_bAsyncNotify;
    RECT m_rcItem;
    RECT m_rcPadding;
    SIZE m_cXY;
    SIZE m_cxyFixed;
    SIZE m_cxyMin;
    SIZE m_cxyMax;
    bool m_bVisible;
    bool m_bInternVisible;
    bool m_bEnabled;
    bool m_bMouseEnabled;
	bool m_bKeyboardEnabled ;
    bool m_bFocused;
    bool m_bFloat;
	TPercentInfo m_piFloatPercent;
    bool m_bSetPos; // ��ֹSetPosѭ������

    CDuiString m_sText;
    CDuiString m_sToolTip;
    TCHAR m_chShortcut;
    CDuiString m_sUserData;
    UINT_PTR m_pTag;

    DWORD m_dwBackColor;
    DWORD m_dwBackColor2;
    DWORD m_dwBackColor3;
    TDrawInfo m_diBk;
	TDrawInfo m_diFore;
    DWORD m_dwBorderColor;
	DWORD m_dwFocusBorderColor;
    bool m_bColorHSL;
	int m_nBorderStyle;
	int m_nTooltipWidth;
    SIZE m_cxyBorderRound;
    RECT m_rcPaint;
	RECT m_rcBorderSize;
	CDuiStringPtrMap m_mCustomAttrHash;
};

} // namespace DuiLib

#endif // __UICONTROL_H__
