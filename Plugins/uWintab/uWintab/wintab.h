#pragma once

#include <Windows.h>
#include <Wintab/wintab.h>


class Wintab
{
public:
    static BOOL Load();
    static void Unload();
    static bool HasLoaded() { return library_ != nullptr; }

    static UINT WTInfo(UINT wCategory, UINT nIndex, LPVOID lpOutput);
    static HCTX WTOpen(HWND hWnd, LPLOGCONTEXT lpLogCtx, BOOL fEnable);
    static BOOL WTGet(HCTX hCtx, LPLOGCONTEXT lpLogCtx);
    static BOOL WTSet(HCTX hCtx, LPLOGCONTEXT lpLogCtx);
    static BOOL WTClose(HCTX hCtx);
    static BOOL WTEnable(HCTX hCtx, BOOL fEnable);
    static BOOL WTPacket(HCTX hCtx, UINT wSerial, LPVOID lpPkt);
    static BOOL WTOverlap(HCTX hCtx, BOOL fToTop);
    static BOOL WTSave(HCTX hCtx, LPVOID lpSaveInfo);
    static BOOL WTConfig(HCTX hCtx, HWND hWnd);
    static HCTX WTRestore(HWND hWnd, LPVOID lpSaveInfo, BOOL fEnable);
    static BOOL WTExtSet(HCTX hCtx, UINT wExt, LPVOID lpData);
    static BOOL WTExtGet(HCTX hCtx, UINT wExt, LPVOID lpData);
    static BOOL WTQueueSizeSet(HCTX hCtx, int nPkts);
    static int  WTDataPeek(HCTX hCtx, UINT wBegin, UINT wEnd, int cMaxPkts, LPVOID lpPkts, LPINT lpNPkts);
    static int  WTPacketsGet(HCTX hCtx, int cMaxPkts, LPVOID lpPkts);

private:
    template <class FuncType>
    static bool GetWintabProc(FuncType& pFunc, LPCSTR name)
    {
        pFunc = (FuncType)(GetProcAddress(library_, name));
        return pFunc != NULL;
    }

    using WTINFO          =  UINT (API *)(UINT, UINT, LPVOID);
    using WTOPEN          =  HCTX (API *)(HWND, LPLOGCONTEXT, BOOL);
    using WTGET           =  BOOL (API *)(HCTX, LPLOGCONTEXT);
    using WTSET           =  BOOL (API *)(HCTX, LPLOGCONTEXT);
    using WTCLOSE         =  BOOL (API *)(HCTX);
    using WTENABLE        =  BOOL (API *)(HCTX, BOOL);
    using WTPACKET        =  BOOL (API *)(HCTX, UINT, LPVOID);
    using WTOVERLAP       =  BOOL (API *)(HCTX, BOOL);
    using WTSAVE          =  BOOL (API *)(HCTX, LPVOID);
    using WTCONFIG        =  BOOL (API *)(HCTX, HWND);
    using WTRESTORE       =  HCTX (API *)(HWND, LPVOID, BOOL);
    using WTEXTSET        =  BOOL (API *)(HCTX, UINT, LPVOID);
    using WTEXTGET        =  BOOL (API *)(HCTX, UINT, LPVOID);
    using WTQUEUESIZESET  =  BOOL (API *)(HCTX, int);
    using WTDATAPEEK      =  int  (API *)(HCTX, UINT, UINT, int, LPVOID, LPINT );
    using WTPACKETSGET    =  int  (API *)(HCTX, int, LPVOID);

    static HINSTANCE      library_;
    static WTINFO         WTInfo_;
    static WTOPEN         WTOpen_;
    static WTGET          WTGet_;
    static WTSET          WTSet_;
    static WTCLOSE        WTClose_;
    static WTPACKET       WTPacket_;
    static WTENABLE       WTEnable_;
    static WTOVERLAP      WTOverlap_;
    static WTSAVE         WTSave_;
    static WTCONFIG       WTConfig_;
    static WTRESTORE      WTRestore_;
    static WTEXTSET       WTExtSet_;
    static WTEXTGET       WTExtGet_;
    static WTQUEUESIZESET WTQueueSizeSet_;
    static WTDATAPEEK     WTDataPeek_;
    static WTPACKETSGET   WTPacketsGet_;
};