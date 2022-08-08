// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <io.h>
#include <locale>
#include <codecvt>
#include <sstream>


// #define cout log

// ofstream log("C:\\Users\\gobills\\Documents\\duilib\\wx-hook\\output.txt");

int totalSum = 8000;

static bool hasClick = false;
CControlUI* contactListControl;
int contackListIndex = 0;
u16string contactClassName= u"OptionUI";

bool hasEdit = false;
u16string editName = u"search_from_net";
u16string searchName = u"searchedit";

bool hasSelected = false;
u16string personClassName = u"ButtonUI";
u16string personControlName = u"send_message_button";
u16string personName = u"发消息";


bool hasSendMessage = false;
u16string sendClassName = u"RichEditUI";
u16string sendControlName = u"send_rich_edit";
wchar_t sendText[] = L"测试：小G电投壹自动化";


bool messageClick = false;
u16string messageButtonClass = u"ButtonUI";
u16string messageButtonControlName = u"send_message_button";
u16string messageButtonText = u"发送(S)";

//hook CDialogBuilder::Create
/*
CControlUI* CDialogBuilder::Create(IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent)
*/
VOID WINAPI CDialogBuilder_Create_Ret(CControlUI* pControlUI, u16ofstream& log)
{
    cout << "CDialogBuilder::Create Ret" << endl;
    
    if (pControlUI) {
        // CPaintManagerUI* manager = pControlUI->GetManager();
        // pControlUI = manager->GetRoot();
        enumContainer(pControlUI, log, u"");
    }
}

void testSendClick(CControlUI* pControlUI, u16string className,
    u16string controlName, u16string text) {
    if (className == messageButtonClass 
        && controlName == messageButtonControlName 
        && text == messageButtonText
        && pControlUI->IsVisible()) 
    {
        messageClick = true;
        keybd_event(VK_RETURN, 0x9C, 0, 0);
        keybd_event(VK_RETURN, 0x9C, KEYEVENTF_KEYUP, 0);
        // pControlUI->Activate();
    }
}

void testSendMessage(CControlUI* pControlUI, u16string className, u16string controlName) {
    if (className == sendClassName && controlName == sendControlName) {
        hasSendMessage = true;
        pControlUI->SetText(sendText);
    }
}

__declspec(noinline) void testSelectPerson(CControlUI* pControlUI, u16string className,
    u16string controlName, u16string text) {
    if (className == personClassName && controlName == personControlName && text == personName) {
        hasSelected = true;
        pControlUI->Activate();
    }
}
__declspec(noinline) void testEditInput(CControlUI* pControlUI, u16string name) {
    if (searchName == name && pControlUI->IsVisible()) {
        hasEdit = true;
        wchar_t replaceName[] = L"曾薇";
        pControlUI->Activate();
        pControlUI->SetText(replaceName);
    }
}

__declspec(noinline) void testContactButtonClick(CControlUI* pControlUI, u16ofstream& log, u16string prefix ) {
    CDuiString tip = pControlUI->GetToolTip();

    if (tip.IsEmpty()) {
        return;
    }

    const wchar_t* data = tip.GetData();
    unsigned int dataAddr = reinterpret_cast<unsigned int>(data);
    char16_t* wDataAddr = reinterpret_cast<char16_t*>(dataAddr);
    u16string udata(wDataAddr);

    log << prefix << " tool tip: " << udata << endl;

    u16string contact = u"通讯录";
    if (udata == contact && !hasClick) {
        hasClick = true;
        pControlUI->Activate();
    }
}

__declspec(noinline) void test_label(CControlUI* pControlUI, u16string text) {
    u16string fileAssist = u"曾薇";
    if (text != fileAssist) {
        return;
    }
    
    POINT pt = {};
    RECT pos = pControlUI->GetPos();
    pt.x = pos.left + 2;
    pt.y = pos.top + 2;

    //if (!hasClick) {
        hasClick = true;


        pControlUI = pControlUI->GetParent()->GetParent()->GetParent()->GetParent();
        CPaintManagerUI* manager = pControlUI->GetManager();
        //manager->SendNotify(pControlUI, L"setfocus", 0, 0, true);
        pControlUI->SetFocus();
        pControlUI->SetMouseEnabled(true);
        pControlUI->Activate();
        TEventUI event = { 0 };
        event.Type = UIEVENT_BUTTONDOWN;
        event.pSender = pControlUI;
        event.wParam = 0;
        event.lParam = 0;
        event.ptMouse = pt;
        //event.wKeyState = (WORD)wParam;
        event.dwTimestamp = ::GetTickCount();
        pControlUI->Event(event);
    //}
    
    //if (!pControlUI->IsFocused()) {
    //    pControlUI->SetFocus();
    //}
   
}

//遍历控件的信息
__declspec(noinline) void enumContainer(CControlUI* pControlUI, u16ofstream& log, u16string prefix)
{
    u16string isVisiable = u"False";
    if (pControlUI->IsVisible()) {
        isVisiable = u"True";
    }
    log << prefix << "visiable: " << isVisiable << endl;
    // pControlUI->SetText("hello");
    // CDuiString tip = pControlUI->GetToolTip();

    LPCSTR lpcszClass = pControlUI->GetClass();
    unsigned int* classPtr = reinterpret_cast<unsigned int*>(&lpcszClass);
    classPtr = reinterpret_cast<unsigned int*>(*classPtr);
    u16string className(reinterpret_cast<char16_t*>(classPtr));
    
    unsigned int uipointAddr = reinterpret_cast<unsigned int>(pControlUI);
    wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;

    log << prefix << "ui addr:" << conv.from_bytes(to_string(uipointAddr)) << endl;

    CDuiString tip = pControlUI->GetToolTip();

    if (!tip.IsEmpty()) {
        const wchar_t* data = tip.GetData();
        unsigned int dataAddr = reinterpret_cast<unsigned int>(data);
        char16_t* wDataAddr = reinterpret_cast<char16_t*>(dataAddr);
        u16string udata(wDataAddr);

        log << prefix << " tool tip: " << udata << endl;
    }

    if (!className.empty()) {
        log << prefix << "class: " << className << endl;
        if (className == contactClassName) {
            testContactButtonClick(pControlUI, log, prefix);
        }
    }

    unsigned int* control_addr = reinterpret_cast<unsigned int*>(pControlUI);
    unsigned int* name_addr = control_addr + 38;
    name_addr = reinterpret_cast<unsigned int*>(*name_addr);
    
    u16string name(reinterpret_cast<char16_t*>(name_addr));
    // const u16string name = u"文件";
    if (!name.empty()) {
        log << prefix << "name: " << name << endl;
        if (hasClick && !hasEdit) {
            testEditInput(pControlUI, name);
        }
    }
    
    unsigned int* text_addr = control_addr + 84;
    text_addr = reinterpret_cast<unsigned int*>(*text_addr);
    u16string text(reinterpret_cast<char16_t*>(text_addr));
    if (!text.empty()) {
        log << prefix << "text: " << text << endl;

        if (hasEdit && !hasSelected) {
            testSelectPerson(pControlUI, className, name, text);
        }
    }

    if (hasSendMessage && !messageClick) {
        totalSum--;
        if (totalSum < 0) {
            testSendClick(pControlUI, className, name, text);
        }
    }

    if (hasSelected && !hasSendMessage) {
        testSendMessage(pControlUI, className, name);
    }


    u16string isFocus = u"False";
    if (pControlUI->IsFocused()) {
        isFocus = u"True";
    }
    log << prefix << "focus: " << isFocus << endl;

    log << endl;
    u16string containerName = u"IContainer";
    unsigned int* addr = *reinterpret_cast<unsigned int**>(&containerName);
    LPCTSTR lpContainerName = reinterpret_cast<LPCTSTR>(addr);

    IContainerUI* pContainerUI = static_cast<IContainerUI*>(pControlUI->GetInterface(lpContainerName));
    if (pContainerUI)
    {
        int i = 0;
        CControlUI* pControlIter = NULL;
        while (pControlIter = pContainerUI->GetItemAt(i++))
        {
            enumContainer(pControlIter, log, prefix + u"    ");
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
        //cout << "Notify..." << endl;
        u16ofstream log("C:\\Users\\gobills\\Documents\\duilib\\wx-hook\\output.txt", ios_base::app);
        //u16ofstream log("C:\\Users\\gobills\\Documents\\duilib\\wx-hook\\output.txt");
        
        CDuiString type = msg.sType;
        const wchar_t* data = type.GetData();
        unsigned int dataAddr = reinterpret_cast<unsigned int>(data);
        char16_t* wDataAddr = reinterpret_cast<char16_t*>(dataAddr);
        u16string udata(wDataAddr);
        log << "msg type: " << udata << endl;

        CControlUI* uipoint = msg.pSender;
        unsigned int uipointAddr = reinterpret_cast<unsigned int>(uipoint);

        wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, std::little_endian>, char16_t> conv;
        log << "ui addr: " << conv.from_bytes(to_string(uipointAddr)) << endl;
        CDialogBuilder_Create_Ret(msg.pSender, log);

        log.close();
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
    HWND hMainWnd = FindWindow("WeWorkWindow", NULL);
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
    // old origin point 55
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

BOOL APIENTRY DllMain( HMODULE hModule,
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
            return TRUE;
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

