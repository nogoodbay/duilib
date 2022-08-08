// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <string>
#include <atlstr.h>
#include <iostream>
#include <cstdio>

#pragma warning(disable : 4996)

//32 bit relative jump opcode is E9, takes 1 32 bit operand for jump offset
uint8_t jmpInstruction[5] = { 0xE9, 0x0, 0x0, 0x0, 0x0 };

uint8_t old_instrucction[12] = {  };
uint32_t old_jmp_addr;
LPVOID old_notify;
WindowImplBase* pWinImplBase;

//hook CDialogBuilder::Create
/*
CControlUI* CDialogBuilder::Create(IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent)
*/
VOID WINAPI CDialogBuilder_Create_Ret(CControlUI* pControlUI)
{
    //MessageBox(NULL, "CDialogBuilder::Create Ret ...", "Hook", NULL);
    cout << "CDialogBuilder::Create Ret" << endl;
    //TRACE_LOG_INFO(_T("CDialogBuilder::Create Ret ..."));
    if (pControlUI)
        enumContainer(pControlUI);
}
//遍历控件的信息
void enumContainer(CControlUI* pControlUI)
{
    MessageBox(NULL, "enum ..", "Hook", NULL);
    CDuiString lpcszClass = pControlUI->GetClass();
    //CDuiString lpcszName = pControlUI->GetName();
    //CDuiString lpcszText = pControlUI->GetText();
    if (lpcszClass)
    {
        cout << *lpcszClass << endl;
        //TRACE_LOG_INFO(_T("%s"), lpcszClass);
    }
    //if (lpcszName)
    //{
    //    cout << lpcszName << endl;
        //TRACE_LOG_INFO(_T("%s"), lpcszName);
    //}
    //if (lpcszText)
    //{
    //    cout << lpcszText << endl;
        //TRACE_LOG_INFO(_T("%s"), lpcszText);
    //}
    IContainerUI* pContainerUI = static_cast<IContainerUI*>(pControlUI->GetInterface("IContainer"));
    if (pContainerUI)
    {
        int i = 0;
        CControlUI* pControlIter = NULL;
        while (pControlIter = pContainerUI->GetItemAt(i++))
        {
            enumContainer(pControlIter);
        }
    }
}
//hook event notify
class CHookNotify : INotifyUI
{
public:
    typedef void(CHookNotify::* pfn_Notify)(TNotifyUI& msg);
    void Notify(TNotifyUI& msg)
    {
        try {
            cout << "Dispatch to pWinImplBase ...." << endl;
            //pWinImplBase->NotifyPump(msg);
            // cout << msg.pSender->GetText() << endl;
            // cout << "Hook method ... " << endl;
            CDialogBuilder_Create_Ret(msg.pSender);
        }
        catch (exception e) {
            cout << e.what() << endl;
        }
        __asm nop;
        __asm nop;
        __asm nop;
        __asm nop;
        __asm nop;
        
    }
};
CHookNotify::pfn_Notify g_pfn_oldnotify = NULL;

DWORD WINAPI test_thread(
    LPVOID lpThreadParameter
)
{
    HWND hMainWnd = FindWindow("WXWorkLocalLoginWindow", NULL);
    if (!hMainWnd)
    {
        MessageBox(NULL, "FindWindow Error ..", "Hook", NULL);
        return 0;
    }
    CWindowWnd* pThis = reinterpret_cast<CWindowWnd*>(::GetWindowLongPtr(hMainWnd, GWLP_USERDATA));
    if (!pThis)
    {
        MessageBox(NULL, "Get CWindowWnd Pointer Error ...", "Hook", NULL);
        //TRACE_LOG_INFO(_T("%s"), _T("Get CWindowWnd Pointer Error ..."));
        return 0;
    }
    HWND* phWnd = reinterpret_cast<HWND*>(((uintptr_t)pThis + sizeof(uintptr_t)));
    if (IsBadReadPtr(phWnd, sizeof(HWND)))
    {
        MessageBox(NULL, "Pointer Invalid Error ...", "Hook", NULL);
        //TRACE_LOG_INFO(_T("%s"), _T("Pointer Invalid Error ..."));
        return 0;
    }
    if (*phWnd != hMainWnd)
    {
        MessageBox(NULL, "Invalid CWindowWnd Pointer Error ...", "Hook", NULL);
        //TRACE_LOG_INFO(_T("%s"), _T("Invalid CWindowWnd Pointer Error ..."));
        return 0;
    }
    //WindowImplBase * == CWindowWnd *
    pWinImplBase = reinterpret_cast<WindowImplBase*>(pThis);
    //INotifyUI* pNotifyThis = static_cast<INotifyUI*>(pWinImplBase);
 
    //CHookNotify::pfn_Notify* p_pfn_oldnotify = *reinterpret_cast<CHookNotify::pfn_Notify**>(pNotifyThis);
    __asm push WindowImplBase::Notify;
    __asm pop old_notify;
    CString t;
    t.Format(_T("test_thread result : %d"), old_notify);
    MessageBox(NULL, t, "Hook", NULL);
    // unsigned int* old_notify_pt = (unsigned int*)old_notify;
    // old_notify = *old_notify_pt + 7;
    // t.Format(_T("test_thread result : %d"), old_notify);
    // MessageBox(NULL, t, "Hook", NULL);
    //set virtual table hook
    try {
        //to fill out the last 4 bytes of jmpInstruction, we need the offset between 
        //the payload function and the instruction immediately AFTER the jmp instruction
       uint32_t temp;
        MessageBox(NULL, "1", "Hook", NULL);
        freopen("C:\\Users\\gobills\\Documents\\duilib\\wx-hook\\output.txt", "w", stdout);
        __asm push CHookNotify::Notify;
        __asm pop temp;
        const uint32_t relAddr = temp - ((uint32_t)old_notify + sizeof(jmpInstruction) + 7);
        uint32_t old_jmp = 0x0000335f;
        old_jmp_addr = relAddr - old_jmp + 50;
        t.Format(_T("test_thread result : %d"), relAddr);
        MessageBox(NULL, t, "Hook", NULL);
        memcpy(jmpInstruction + 1, &relAddr, 4);
        MessageBox(NULL, "2", "Hook", NULL);
        DWORD dwOldProtect = 0;
        BOOL success = VirtualProtect(old_notify, 12, PAGE_EXECUTE_READWRITE, &dwOldProtect);
        if (!success) {
            MessageBox(NULL, "VirtualProtect invoke error!", "Hook", NULL);
            return 0;
        }

        MessageBox(NULL, "3", "Hook", NULL);
        memcpy(old_instrucction, old_notify, 12);
        memcpy(old_instrucction + 8, &relAddr, 4);
        memcpy(old_notify, old_instrucction, sizeof(old_instrucction));
        MessageBox(NULL, "开始注入0-3", "Hook", NULL);

        //g_pfn_oldnotify = *p_pfn_oldnotify;
        //CHookNotify::pfn_Notify temp;
        //MessageBox(NULL, "1", "Hook", NULL);
        //__asm push CHookNotify::Notify;
        //__asm pop temp;
        //MessageBox(NULL, "2", "Hook", NULL);
        //DWORD dwOldProtect = 0;
        //VirtualProtect(p_pfn_oldnotify, sizeof(void*), PAGE_EXECUTE_READWRITE, &dwOldProtect);
        //MessageBox(NULL, "3", "Hook", NULL);
        //*p_pfn_oldnotify = temp;
        return 1;
    }
    catch (exception e) {
        MessageBox(NULL, e.what(), "注入", NULL);
        return 0;
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        MessageBox(NULL, "start inject..", "Hook", NULL);
        int result = test_thread(NULL);
        CString t;
        t.Format(_T("test_thread result : %d"), result);
        MessageBox(NULL, t, "注入", NULL);
        if (!result) {
            return FALSE;
        }
        MessageBox(NULL, "注入成功", "注入", NULL);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

