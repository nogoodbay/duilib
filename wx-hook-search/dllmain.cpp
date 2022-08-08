// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <io.h>
#include <locale>
#include <codecvt>

// #define cout log

// ofstream log("C:\\Users\\gobills\\Documents\\duilib\\wx-hook\\output.txt");

//hook CDialogBuilder::Create
/*
CControlUI* CDialogBuilder::Create(IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent)
*/
VOID WINAPI CDialogBuilder_Create_Ret(CControlUI* pControlUI, u16ofstream& log)
{
    cout << "CDialogBuilder::Create Ret" << endl;
    // _setmode(log, _O_U16TEXT);
    // log.imbue(locale(log.getloc(), new codecvt_utf8<wchar_t, 0x10FFFFFF>));
    // _setmode(_fileno(stdout), _O_U8TEXT);

    if (pControlUI)
        enumContainer(pControlUI, log);
}

//遍历控件的信息
__declspec(noinline) void enumContainer(CControlUI* pControlUI, u16ofstream& log)
{
    // f_GetName getText = *reinterpret_cast<f_GetName*>(*addr + 0x48);
    // unsigned int* cduiStringText = getText();
    // string text = *reinterpret_cast<string*>(*cduiStringText + 0x54);
    // cout << text << endl;

    LPCSTR lpcszClass = pControlUI->GetClass();
    unsigned int* classPtr = reinterpret_cast<unsigned int*>(&lpcszClass);
    classPtr = reinterpret_cast<unsigned int*>(*classPtr);
    u16string className(reinterpret_cast<char16_t*>(classPtr));
    // CDuiString lpcszName = pControlUI->GetName();
    // CDuiString lpcszText = pControlUI->GetText();
    // pControlUI->SetVisible(false);
    //int length = sizeof(lpcszClass) * 2;
    //cout << "control class : lenght : " << length << "; name : ";
    //for (int i = 0; i < length; i++) {
    //    cout << lpcszClass[i];
    //}
    //cout << endl;

    log << "class: " << className << endl;

    unsigned int* control_addr = reinterpret_cast<unsigned int*>(pControlUI);
    unsigned int* name_addr = control_addr + 38;
    name_addr = reinterpret_cast<unsigned int*>(*name_addr);

    u16string name(reinterpret_cast<char16_t*>(name_addr));
    // const u16string name = u"文件";
    log << "name: " << name << endl;

    unsigned int* text_addr = control_addr + 84;
    text_addr = reinterpret_cast<unsigned int*>(*text_addr);
    u16string text(reinterpret_cast<char16_t*>(text_addr));
    log << "text: " << text << endl;

    //char16_t* text = reinterpret_cast<char16_t*>(text_addr);
    //log << "text: " << *text << endl;
    // MessageBox(NULL, name, "HOOK", NULL);
    // f_GetName getName = *reinterpret_cast<f_GetName*>(*addr + 0x14);
    // __asm push addr;
    // unsigned int* cduiStringName = getName();
    // string name = *reinterpret_cast<string*>(*cduiStringName + 0x54);
    // cout << name << endl;


    //    TRACE_LOG_INFO(_T("%s"), lpcszClass);
    //if (lpcszText) {
    //    cout << "control name: " << lpcszText << endl;
    //}
    //MessageBox(NULL, lpcszName, "Hook", NULL);
    //if (lpcszName) {
        // cout << "control text: " << lpcszName << endl;
    //}
    //    TRACE_LOG_INFO(_T("%s"), lpcszName);
    //if (lpcszText)
    //    TRACE_LOG_INFO(_T("%s"), lpcszText);
    //if (CDuiString(_T("session_list")) == pControlUI->GetName())
    //{
    //    g_session_list = pControlUI;
    //}
    //if (CDuiString(_T("contact_list")) == pControlUI->GetName())
    //{
    //    g_contact_list = pControlUI;
    //}

    IContainerUI* pContainerUI = static_cast<IContainerUI*>(pControlUI->GetInterface(DUI_CTR_ICONTAINER));
    if (pContainerUI)
    {
        int i = 0;
        CControlUI* pControlIter = NULL;
        while (pControlIter = pContainerUI->GetItemAt(i++))
        {
            enumContainer(pControlIter, log);
        }
    }


    //CListUI* pListUI = static_cast<CListUI*>(pControlUI->GetInterface(DUI_CTR_LIST));
    //if (pListUI)
    //{
    //    int i = 0;
    //    CControlUI* pControlIter = NULL;
    //    while (pControlIter = pListUI->GetItemAt(i++))
    //    {
    //        enumContainer(pControlIter, log);
    //    }
    //}

    //CTreeNodeUI* pContainerNode = static_cast<CTreeNodeUI*>(pControlUI->GetInterface(DUI_CTR_TREENODE));
    //if (pContainerNode)
    //{
    //    int i = 0;
    //    CControlUI* pControlIter = NULL;
    //    CContainerUI* treNode = pContainerNode->GetTreeNodeHoriznotal();
    //    while (pControlIter = treNode->GetItemAt(i++))
    //    {
    //        enumContainer(pControlIter, log);
    //    }
    //}

    //CVerticalLayoutUI* pVerticalUI = static_cast<CVerticalLayoutUI*>(pControlUI->GetInterface(DUI_CTR_VERTICALLAYOUT));
    //if (pVerticalUI)
    //{
    //    int i = 0;
    //    CControlUI* pControlIter = NULL;
    //    while (pControlIter = pVerticalUI->GetItemAt(i++))
    //    {
    //        enumContainer(pControlIter, log);
    //    }
    //}
}


//hook event notify
class CHookNotify : INotifyUI
{
public:
    typedef void(CHookNotify::* pfn_Notify)(TNotifyUI& msg);
    void Notify(TNotifyUI& msg)
    {
        //cout << "Notify..." << endl;
        u16ofstream log("C:\\Users\\gobills\\Documents\\duilib\\wx-hook\\output.txt", ios_base::app);
        //u16ofstream log("C:\\Users\\gobills\\Documents\\duilib\\wx-hook\\output.txt");
        CDialogBuilder_Create_Ret(msg.pSender, log);
        extern CHookNotify::pfn_Notify g_pfn_oldnotify;
        return (this->*g_pfn_oldnotify)(msg);
        //return g_pfn_oldnotify(msg);
    }
};

CHookNotify::pfn_Notify g_pfn_oldnotify = NULL;

__declspec(noinline) DWORD WINAPI test_thread(
    LPVOID lpThreadParameter
)
{
    // freopen("C:\\Users\\gobills\\Documents\\duilib\\wx-hook\\output.txt", "w", stdout);
    HWND hMainWnd = FindWindow("SearchResultWindow", NULL);
    if (!hMainWnd)
    {
        //log << "FindWindow Error ..." << endl;
        //TRACE_LOG_INFO(_T("%s"), _T("FindWindow Error ..."));
        return 1;
    }
    // log << "hMainWnd handle : " << hMainWnd << endl;
    CWindowWnd* pThis = reinterpret_cast<CWindowWnd*>(::GetWindowLongPtr(hMainWnd, GWLP_USERDATA));
    if (!pThis)
    {
        //log << "Get CWindowWnd Pointer Error ..." << endl;
        // cout << "Get CWindowWnd Pointer Error ..." << endl;
        //TRACE_LOG_INFO(_T("%s"), _T("Get CWindowWnd Pointer Error ..."));
        return 1;
    }
    HWND* phWnd = reinterpret_cast<HWND*>(((uintptr_t)pThis + sizeof(uintptr_t)));
    if (IsBadReadPtr(phWnd, sizeof(HWND)))
    {
        //log << "Pointer Invalid Error ..." << endl;
        // cout << "Pointer Invalid Error ..." << endl;
        //TRACE_LOG_INFO(_T("%s"), _T("Pointer Invalid Error ..."));
        return 1;
    }
    if (*phWnd != hMainWnd)
    {
        //log << "Invalid CWindowWnd Pointer Error ..." << endl;
        // cout << "Invalid CWindowWnd Pointer Error ..." << endl;
        //TRACE_LOG_INFO(_T("%s"), _T("Invalid CWindowWnd Pointer Error ..."));
        return 1;
    }
    //WindowImplBase * == CWindowWnd *
    // cout << "convert to notify" << endl;
    // WindowImplBase* pWinImplBase = reinterpret_cast<WindowImplBase*>(pThis);
    unsigned int* pWinImplBase = reinterpret_cast<unsigned int*>(pThis);
    // cout << "pWinImplBase addr: " << pWinImplBase << endl;
    // cout << "point to : " << *pWinImplBase << endl;
    unsigned int point_value = *pWinImplBase;
    unsigned int* point = reinterpret_cast<unsigned int*>(point_value);

    unsigned int temp;
    __asm push CHookNotify::Notify;
    __asm pop temp;

    DWORD dwOldProtect = 0;
    bool result = VirtualProtect(point, sizeof(void*) * 56, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    if (!result) {
        MessageBox(NULL, "VirtualProtect error", "HOOK", NULL);
        return 1;
    }
    g_pfn_oldnotify = *reinterpret_cast<CHookNotify::pfn_Notify*>(&point[55]);
    point[55] = temp;

    // cout << "point1 :" << *point << endl;
    // INotifyUI* pNotifyThis = static_cast<INotifyUI*>(pWinImplBase);
    // cout << "pINotifyUI addr: " << pNotifyThis << endl;

    // CHookNotify::pfn_Notify* p_pfn_oldnotify = *reinterpret_cast<CHookNotify::pfn_Notify**>(pNotifyThis);
    // cout << "pfn_oldnotify addr: " << p_pfn_oldnotify << endl;
    // cout << "pfn_oldnotify: " << *p_pfn_oldnotify << endl;

    // unsigned int* pvTable = (unsigned int*)pNotifyThis;
    // cout << "pvTable 1 addr: " << pvTable << endl;
    return 0;
    // pvTable = (unsigned int*)(*pvTable);
    //cout << "pvTable 2 addr : " << pvTable << endl;
    //cout << "virtal table addr : " << pvTable << endl;

    //pvTable = (unsigned long*) *pvTable;

    // cout << "start set virtual table hook" << endl;
    //set virtual table hook
    // g_pfn_oldnotify = *p_pfn_oldnotify;
    // CHookNotify::pfn_Notify temp;
    // unsigned int temp;
    // __asm push CHookNotify::Notify;
    // __asm pop temp;
    // DWORD dwOldProtect = 0;
    // cout << "set virtual protect" << endl;
    // VirtualProtect(pvTable, sizeof(void*), PAGE_EXECUTE_READWRITE, &dwOldProtect);
    // cout << "virtual protect" << endl;
    // g_pfn_oldnotify = *reinterpret_cast<CHookNotify::pfn_Notify*>(pvTable);
    // cout << "save old virtual table" << endl;
    // *pvTable = temp;
    //*p_pfn_oldnotify = temp;
    // return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        try {
            // clear file
            u16ofstream log("C:\\Users\\gobills\\Documents\\duilib\\wx-hook\\output.txt");
            log.close();
            int result = test_thread(NULL);

            if (result) {
                MessageBox(NULL, "注入失败", "HOOK", NULL);
                return FALSE;
            }
            return FALSE;
        }
        catch (exception e) {
            cout << e.what() << endl;
            MessageBox(NULL, e.what(), "注入", NULL);
            return FALSE;
        }

    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

