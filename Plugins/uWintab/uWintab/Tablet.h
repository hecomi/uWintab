#pragma once

#include <string>
#include <map>
#include <Windows.h>
#include <Wintab/wintab.h>
#include "Common.h"


class Tablet
{
public:
    static bool IsAvailable();

    enum class CursorType
    {
        Invalid = -1,
        // ---
        Cursor = 0,
        Pen = 1,
        TailSwitch = 2,
        // ---
        Num,
    };

    enum class ButtonState
    {
        Off,
        Pressed,
        On,
        Released,
    };

    enum class ExpKeyState
    {
        Off,
        Pressed,
        On,
        Released,
    };

public:
    Tablet();

    void Open(HWND hwnd);
    void Close();
    void Update();
    BOOL ReceivePacket(LPARAM lp, WPARAM wp);
    BOOL ReceivePacketExt(LPARAM lp, WPARAM wp);
    void ReceiveProximity(LPARAM lp, WPARAM wp);
    const std::string & GetDeviceName() const { return deviceName_; }

    void Overwrap();
    void Enable(WPARAM wp);

    bool IsPressureSupported() const { return isPressureSupported_; }
    bool IsWheelSupported() const { return isWheelSupported_; }
    bool IsOrientationSupported() const { return isOrientationSupported_; }
    bool IsExpKeysSupported() const { return isExpKeysSupported_; }

    const std::string& GetVersion() const { return version_; }
    UINT GetDeviceNum() const { return deviceNum_; }
    UINT GetExpKeyNum(UINT tabletId) const;
    float GetX() const;
    float GetY() const;
    float GetPressure() const;
    float GetWheel() const;
    float GetAzimuth() const;
    float GetAltitude() const;
    float GetTwist() const;
    UINT GetPenId() const;
    CursorType GetCursor() const;
    DWORD GetTime() const;
    bool GetProximity() const;
    bool GetButton(USHORT id) const;
    bool GetButtonDown(USHORT id) const;
    bool GetButtonUp(USHORT id) const;
    bool GetExpKey(USHORT tabletId, USHORT controlId) const;
    bool GetExpKeyDown(USHORT tabletId, USHORT controlId) const;
    bool GetExpKeyUp(USHORT tabletId, USHORT controlId) const;

private:
    bool FindExtension(UINT extension, UINT &index);
    template <class T>
    T ExtGet(UINT extension, BYTE tabletId, BYTE controlId, BYTE functionId, WORD property);
    template <class T>
    bool ExtSet(UINT extension, BYTE tabletId, BYTE controlId, BYTE functionId, WORD property, T value);
    void InitExpKeys();
    bool IsValidButton(USHORT id) const;
    void UpdateButtonState();
    bool IsValidExpKey(USHORT tabletId, USHORT controlId) const;
    void UpdateExpKeyState();

    HCTX context_;
    std::string version_;
    std::string deviceName_;
    PACKET packet_;
    PACKETEXT packetExt_;

    bool isPressureSupported_ = false;
    bool isWheelSupported_ = false;
    bool isOrientationSupported_ = false;
    bool isExpKeysSupported_ = false;

    WTPKT expKeysMask_ = 0;
    UINT deviceNum_ = 0;

    AXIS coordX_;
    AXIS coordY_;
    AXIS pressure_;
    AXIS wheel_;
    AXIS azimuth_;
    AXIS altitude_;
    AXIS twist_;
    bool proximity_ = false;

    std::map<USHORT, ButtonState> buttons_;
    std::map<USHORT, std::map<USHORT, ExpKeyState>> expKeys_;
};