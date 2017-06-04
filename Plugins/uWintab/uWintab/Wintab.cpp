#include "Wintab.h"



namespace
{
    constexpr const char* kDllName = "Wintab32.dll";
}


decltype(Wintab::library_)        Wintab::library_;
decltype(Wintab::WTInfo_)         Wintab::WTInfo_;
decltype(Wintab::WTOpen_)         Wintab::WTOpen_;
decltype(Wintab::WTGet_)          Wintab::WTGet_;
decltype(Wintab::WTSet_)          Wintab::WTSet_;
decltype(Wintab::WTClose_)        Wintab::WTClose_;
decltype(Wintab::WTPacket_)       Wintab::WTPacket_;
decltype(Wintab::WTEnable_)       Wintab::WTEnable_;
decltype(Wintab::WTOverlap_)      Wintab::WTOverlap_;
decltype(Wintab::WTSave_)         Wintab::WTSave_;
decltype(Wintab::WTConfig_)       Wintab::WTConfig_;
decltype(Wintab::WTRestore_)      Wintab::WTRestore_;
decltype(Wintab::WTExtSet_)       Wintab::WTExtSet_;
decltype(Wintab::WTExtGet_)       Wintab::WTExtGet_;
decltype(Wintab::WTQueueSizeSet_) Wintab::WTQueueSizeSet_;
decltype(Wintab::WTDataPeek_)     Wintab::WTDataPeek_;
decltype(Wintab::WTPacketsGet_)   Wintab::WTPacketsGet_;


BOOL Wintab::Load()
{
    if (HasLoaded()) return FALSE;

	library_ = LoadLibrary(kDllName);
	if (!library_)
	{
		const auto err = GetLastError();
        // TODO: Output error
		return FALSE;
	}

#define UWT_TO_STR2(Name) #Name
#define UWT_TO_STR(Name) UWT_TO_STR(Name)
#define UWT_LOADFUNC(Type) \
    if (!Wintab::GetWintabProc(Type##_, UWT_TO_STR2(Type))) return FALSE;

    UWT_LOADFUNC(WTInfo)
    UWT_LOADFUNC(WTOpen)
    UWT_LOADFUNC(WTGet)
    UWT_LOADFUNC(WTSet)
    UWT_LOADFUNC(WTClose)
    UWT_LOADFUNC(WTPacket)
    UWT_LOADFUNC(WTEnable)
    UWT_LOADFUNC(WTOverlap)
    UWT_LOADFUNC(WTSave)
    UWT_LOADFUNC(WTConfig)
    UWT_LOADFUNC(WTRestore)
    UWT_LOADFUNC(WTExtSet)
    UWT_LOADFUNC(WTExtGet)
    UWT_LOADFUNC(WTQueueSizeSet)
    UWT_LOADFUNC(WTDataPeek)
    UWT_LOADFUNC(WTPacketsGet)

    return TRUE;
}


void Wintab::Unload()
{
    if (!HasLoaded()) return;

    FreeLibrary(library_);
	library_ = nullptr;

    WTInfo_ = nullptr;
    WTOpen_ = nullptr;
    WTGet_ = nullptr;
    WTSet_ = nullptr;
    WTClose_ = nullptr;
    WTPacket_ = nullptr;
    WTEnable_ = nullptr;
    WTOverlap_ = nullptr;
    WTSave_ = nullptr;
    WTConfig_ = nullptr;
    WTRestore_ = nullptr;
    WTExtSet_ = nullptr;
    WTExtGet_ = nullptr;
    WTQueueSizeSet_ = nullptr;
    WTDataPeek_ = nullptr;
    WTPacketsGet_ = nullptr;
}


UINT Wintab::WTInfo(UINT wCategory, UINT nIndex, LPVOID lpOutput)
{
    if (!HasLoaded()) return -1;
    return WTInfo_(wCategory, nIndex, lpOutput);
}


HCTX Wintab::WTOpen(HWND hWnd, LPLOGCONTEXT lpLogCtx, BOOL fEnable)
{
    if (!HasLoaded()) return NULL;
    return WTOpen_(hWnd, lpLogCtx, fEnable);
}


BOOL Wintab::WTGet(HCTX hCtx, LPLOGCONTEXT lpLogCtx)
{
    if (!HasLoaded()) return FALSE;
    return WTGet_(hCtx, lpLogCtx);
}


BOOL Wintab::WTSet(HCTX hCtx, LPLOGCONTEXT lpLogCtx)
{
    if (!HasLoaded()) return FALSE;
    return WTSet_(hCtx, lpLogCtx);
}


BOOL Wintab::WTClose(HCTX hCtx)
{
    if (!HasLoaded()) return FALSE;
    return WTClose_(hCtx);
}


BOOL Wintab::WTEnable(HCTX hCtx, BOOL fEnable)
{
    if (!HasLoaded()) return FALSE;
    return WTEnable_(hCtx, fEnable);
}


BOOL Wintab::WTPacket(HCTX hCtx, UINT wSerial, LPVOID lpPkt)
{
    if (!HasLoaded()) return FALSE;
    return WTPacket_(hCtx, wSerial, lpPkt);
}


BOOL Wintab::WTOverlap(HCTX hCtx, BOOL fToTop)
{
    if (!HasLoaded()) return FALSE;
    return WTOverlap_(hCtx, fToTop);
}


BOOL Wintab::WTSave(HCTX hCtx, LPVOID lpSaveInfo)
{
    if (!HasLoaded()) return FALSE;
    return WTSave_(hCtx, lpSaveInfo);
}


BOOL Wintab::WTConfig(HCTX hCtx, HWND hWnd)
{
    if (!HasLoaded()) return FALSE;
    return WTConfig_(hCtx, hWnd);
}


HCTX Wintab::WTRestore(HWND hWnd, LPVOID lpSaveInfo, BOOL fEnable)
{
    if (!HasLoaded()) return NULL;
    return WTRestore_(hWnd, lpSaveInfo, fEnable);
}


BOOL Wintab::WTExtSet(HCTX hCtx, UINT wExt, LPVOID lpData)
{
    if (!HasLoaded()) return FALSE;
    return WTExtSet_(hCtx, wExt, lpData);
}


BOOL Wintab::WTExtGet(HCTX hCtx, UINT wExt, LPVOID lpData)
{
    if (!HasLoaded()) return FALSE;
    return WTExtGet_(hCtx, wExt, lpData);
}


BOOL Wintab::WTQueueSizeSet(HCTX hCtx, int nPkts)
{
    if (!HasLoaded()) return FALSE;
    return WTQueueSizeSet_(hCtx, nPkts);
}


int Wintab::WTDataPeek(HCTX hCtx, UINT wBegin, UINT wEnd, int cMaxPkts, LPVOID lpPkts, LPINT lpNPkts)
{
    if (!HasLoaded()) return -1;
    return WTDataPeek_(hCtx, wBegin, wEnd, cMaxPkts, lpPkts, lpNPkts);
}


int Wintab::WTPacketsGet(HCTX hCtx, int cMaxPkts, LPVOID lpPkts)
{
    if (!HasLoaded()) return -1;
    return WTPacketsGet_(hCtx, cMaxPkts, lpPkts);
}