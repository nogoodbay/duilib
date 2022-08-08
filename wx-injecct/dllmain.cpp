// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include <iostream>
#include <cstdio>

#pragma warning(disable : 4996)

typedef  CControlUI* (*f_GetRoot)(CPaintManagerUI*);
typedef CPaintManagerUI* (*f_GetPainManager)();

LPCSTR export_GetPaintManager = "?GetPaintManager@WindowImplBase@DuiLib@@UAEPAVCPaintManagerUI@2@XZ";
LPCSTR export_GetRoot = "?GetRoot@CPaintManagerUI@DuiLib@@QBEPAVCControlUI@2@XZ";


//遍历控件的信息
void enumContainer(CControlUI* pControlUI)
{
    LPCTSTR lpcszClass = pControlUI->GetClass();
    //CDuiString lpcszName = pControlUI->GetName();
    //CDuiString lpcszText = pControlUI->GetText();
    //if (lpcszClass)
    //{
    //    cout << lpcszClass << endl;
        //TRACE_LOG_INFO(_T("%s"), lpcszClass);
    //}
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
    //IContainerUI* pContainerUI = static_cast<IContainerUI*>(pControlUI->GetInterface("IContainer"));
    IContainerUI* pContainerUI = static_cast<IContainerUI*>(pControlUI->GetInterface("IContainer"));
    cout << "enum container ... " << pContainerUI << endl;
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

//hook event notify
class CHookNotify : INotifyUI
{
public:
    CHookNotify() {};
    void Notify(TNotifyUI& msg)
    {
        cout << "Notify..." << endl;
        CDialogBuilder_Create_Ret(msg.pSender);
    }
};

void test_hook() {
    HWND hMainWnd = FindWindow(_T("WXWorkLocalLoginWindow"), NULL);
    if (!hMainWnd)
    {
        cout << "FindWindow Error ..." << endl;
        //TRACE_LOG_INFO(_T("%s"), _T("FindWindow Error ..."));
        return;
    }
    CWindowWnd* pThis = reinterpret_cast<CWindowWnd*>(::GetWindowLongPtr(hMainWnd, GWLP_USERDATA));
    if (!pThis)
    {
        cout << "Get CWindowWnd Pointer Error ..." << endl;
        //TRACE_LOG_INFO(_T("%s"), _T("Get CWindowWnd Pointer Error ..."));
        return;
    }
    HWND* phWnd = reinterpret_cast<HWND*>(((uintptr_t)pThis + sizeof(uintptr_t)));
    if (IsBadReadPtr(phWnd, sizeof(HWND)))
    {
        cout << "Pointer Invalid Error ..." << endl;
        //TRACE_LOG_INFO(_T("%s"), _T("Pointer Invalid Error ..."));
        return;
    }
    if (*phWnd != hMainWnd)
    {
        cout << "Invalid CWindowWnd Pointer Error ..." << endl;
        //TRACE_LOG_INFO(_T("%s"), _T("Invalid CWindowWnd Pointer Error ..."));
        return;
    }
    //WindowImplBase * == CWindowWnd *
    cout << "convert to notify" << endl;
    WindowImplBase* pWinImplBase = reinterpret_cast<WindowImplBase*>(pThis);

    __asm mov ecx,pWinImplBase;
    cout << "pWinImplBase addr: " << pWinImplBase << endl;

    cout << "load DuiLib.dll" << endl;
    HINSTANCE hGetProcIDDLL = LoadLibrary("DuiLib.dll");
    if (!hGetProcIDDLL) {
        cout << "could not load the dynamic library" << endl;
        return;
    }

    cout << "load GetPaintManager method" << endl;
    // resolve function address here
    f_GetPainManager f_painManager = (f_GetPainManager)GetProcAddress(hGetProcIDDLL, export_GetPaintManager);
    if (!f_painManager) {
        cout << "could not load the function" << endl;
        return;
    }
    CPaintManagerUI* manager = f_painManager();
    
    cout << "manager addr: " << manager << endl;
    cout << "add self notify" << endl;

    // manager->AddNotifier(new CHookNotify());

    //cout << "invoke manager.GetRoot()" << endl;
    //CControlUI* root_c = manager->GetRoot();
    //cout << root_c << endl;

    //cout << "load GetRoot method" << endl;
    //f_GetRoot f_getRoot = (f_GetRoot)GetProcAddress(hGetProcIDDLL, export_GetRoot);
    //if (!f_getRoot) {
    //    cout << "could not load method" << endl;
    //    return;
    //}
    //__asm mov ecx,pWinImplBase;
    //CControlUI* root = f_getRoot(manager);
    //cout << "root addr " << root << endl;
    //CDialogBuilder_Create_Ret(root);
    //return;
    //CPaintManagerUI* manager = CPaintManagerUI::GetPaintManager(NULL);
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
        freopen("C:\\Users\\gobills\\Documents\\duilib\\wx-hook\\output.txt", "w", stdout);
        try {
            MessageBox(NULL, "Start Hook ...", "HOOK", NULL);
            test_hook();
        }
        catch (exception e) {
            MessageBox(NULL, e.what(), "HOOK", NULL);
        }
        
        return FALSE;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

