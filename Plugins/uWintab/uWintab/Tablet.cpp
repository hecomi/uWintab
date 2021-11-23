#include "Tablet.h"
#include "Wintab.h"

#define UWT_MAX_BUTTON_NUM 16



bool Tablet::IsAvailable()
{
    return Wintab::WTInfo(0, 0, NULL) != FALSE;
}


Tablet::Tablet()
{
    // Get version
    SHORT version;
    Wintab::WTInfo(WTI_INTERFACE, IFC_SPECVERSION, &version);
    const BYTE major = (version >> 8) & 0xFF;
    const BYTE minor = version & 0xFF;
    version_ = "V" + std::to_string(major) + "." + std::to_string(minor);

    // Get device name
    char deviceName[256];
    Wintab::WTInfo(WTI_DEVICES, DVC_NAME, deviceName);
    deviceName_ = deviceName;

    // Get device num
    Wintab::WTInfo(WTI_INTERFACE, IFC_NDEVICES, &deviceNum_);

    // Get device size
    Wintab::WTInfo(WTI_DEVICES, DVC_X, &coordX_);
    Wintab::WTInfo(WTI_DEVICES, DVC_Y, &coordY_);

    // Get min/max vlaues
    AXIS orientation[3];
    isPressureSupported_ = Wintab::WTInfo(WTI_DEVICES, DVC_NPRESSURE, &pressure_) > 0;
    isWheelSupported_ = Wintab::WTInfo(WTI_DEVICES, DVC_TPRESSURE, &wheel_) > 0;
    isOrientationSupported_ = Wintab::WTInfo(WTI_DEVICES, DVC_ORIENTATION, &orientation) > 0;
    azimuth_ = orientation[0];
    altitude_ = orientation[1];
    twist_ = orientation[2];

    // Check extensions
    UINT expKeyIndex;
    if (isExpKeysSupported_ = FindExtension(WTX_EXPKEYS2, expKeyIndex))
    {
        Wintab::WTInfo(WTI_EXTENSIONS + expKeyIndex, EXT_MASK, &expKeysMask_);
    }

    // Init buttons
    for (UINT i = 1; i <= UWT_MAX_BUTTON_NUM; ++i)
    {
        buttons_.emplace(i, ButtonState::Off);
    }
}


bool Tablet::FindExtension(UINT extension, UINT &index)
{
    for (UINT i = 0, tag = 0; Wintab::WTInfo(WTI_EXTENSIONS + i, EXT_TAG, &tag); ++i)
    {
        if (tag == extension)
        {
            index = i;
            return true;
        }
    }
    return false;
}


template <class T>
T Tablet::ExtGet(UINT extension, BYTE tabletId, BYTE controlId, BYTE functionId, WORD property)
{
    EXTPROPERTY prop;
    prop.version = 0;
    prop.tabletIndex = tabletId;
    prop.controlIndex = controlId;
    prop.functionIndex = functionId;
    prop.propertyID = property;
    prop.reserved = 0;
    prop.dataSize = sizeof(T);

    if (Wintab::WTExtGet(context_, extension, &prop))
    {
        return *reinterpret_cast<T *>(&prop.data[0]);
    }

    return T();
}


template <class T>
bool Tablet::ExtSet(UINT extension, BYTE tabletId, BYTE controlId, BYTE functionId, WORD property, T value)
{
    EXTPROPERTY prop;
    prop.version = 0;
    prop.tabletIndex = tabletId;
    prop.controlIndex = controlId;
    prop.functionIndex = functionId;
    prop.propertyID = property;
    prop.reserved = 0;
    prop.dataSize = sizeof(T);
    *reinterpret_cast<T *>(&prop.data[0]) = value;

    return Wintab::WTExtSet(context_, extension, &prop) >= S_OK;
}


void Tablet::Open(HWND hwnd)
{
    LOGCONTEXT context;
    Wintab::WTInfo(WTI_DEFCONTEXT, 0, &context);
    wsprintf(context.lcName, kLibraryName);
    context.lcOptions |= CXO_MESSAGES;
    context.lcPktData = PACKETDATA | expKeysMask_;
    context.lcPktMode = PACKETMODE;
    context.lcMoveMask = PACKETDATA;
    context.lcBtnUpMask = context.lcBtnDnMask;
    context.lcInOrgX = coordX_.axMin;
    context.lcInOrgY = coordY_.axMin;
    context.lcInExtX = coordX_.axMax;
    context.lcInExtY = coordY_.axMax;

    context_ = Wintab::WTOpen(hwnd, &context, TRUE);

    InitExpKeys();
}


void Tablet::Close()
{
    Wintab::WTClose(context_);
    context_ = nullptr;
}


void Tablet::InitExpKeys()
{
    for (UINT tabletId = 0; tabletId < deviceNum_; ++tabletId)
    {
        const auto controlCount = ExtGet<UINT>(WTX_EXPKEYS2, tabletId, 0, 0, TABLET_PROPERTY_CONTROLCOUNT);
        for (UINT controlId = 0; controlId < controlCount; ++controlId)
        {
            const auto funcCount = ExtGet<UINT>(WTX_EXPKEYS2, tabletId, controlId, 0, TABLET_PROPERTY_FUNCCOUNT);
            for (UINT funcId = 0; funcId < funcCount; ++funcId)
            {
                ExtSet(WTX_EXPKEYS2, tabletId, controlId, funcId, TABLET_PROPERTY_OVERRIDE, static_cast<BOOL>(TRUE));
            }
        }

        auto &keyMap = expKeys_[tabletId];
        for (UINT controlId = 0; controlId < controlCount; ++controlId)
        {
            keyMap.emplace(controlId, ExpKeyState::Off);
        }
    }
}


void Tablet::Update()
{
    if (!context_) return;
    UpdateButtonState();
    UpdateExpKeyState();
}


BOOL Tablet::ReceivePacket(LPARAM lp, WPARAM wp)
{
    return Wintab::WTPacket((HCTX)lp, (UINT)wp, &packet_);
}


BOOL Tablet::ReceivePacketExt(LPARAM lp, WPARAM wp)
{
    return Wintab::WTPacket((HCTX)lp, (UINT)wp, &packetExt_);
}


void Tablet::ReceiveProximity(LPARAM lp, WPARAM wp)
{
    proximity_ = HIWORD(lp) > 0;
}


void Tablet::Overwrap()
{
    if (!context_) return;
    Wintab::WTOverlap(context_, TRUE);
}


void Tablet::Enable(WPARAM wp)
{
    if (!context_) return;
    Wintab::WTEnable(context_, (BOOL)wp);
}


bool Tablet::IsValidButton(USHORT id) const
{
    return id >= 1 && id <= UWT_MAX_BUTTON_NUM;
}


void Tablet::UpdateButtonState()
{
    using State = ButtonState;

    // Update hold / release state
    for (auto &&pair : buttons_)
    {
        const auto id = pair.first;
        auto &state = pair.second;
        if (state == State::Pressed)
        {
            state = State::On;
        }
        else if (state == State::Released)
        {
            state = State::Off;
        }
    }

    // Update pressed / released button
    const auto id = LOWORD(packet_.pkButtons);
    if (!IsValidButton(id)) return;

    auto &state = buttons_.at(id);
    switch (HIWORD(packet_.pkButtons))
    {
    case 1: // off
    {
        state = State::Released;
        break;
    }
    case 2: // on
    {
        state = State::Pressed;
        break;
    }
    }
}


bool Tablet::IsValidExpKey(USHORT tabletId, USHORT controlId) const
{
    return
        (tabletId >= 0 && tabletId < deviceNum_) &&
        (controlId >= 0 && controlId < expKeys_.at(tabletId).size());
}


void Tablet::UpdateExpKeyState()
{
    using State = ExpKeyState;

    const auto tabletId = packetExt_.pkExpKeys.nTablet;
    const auto controlId = packetExt_.pkExpKeys.nControl;
    if (!IsValidExpKey(tabletId, controlId)) return;

    auto &state = expKeys_.at(tabletId).at(controlId);
    switch (packetExt_.pkExpKeys.nState)
    {
    case 0: // off
    {
        if (state == State::Released)
        {
            state = State::Off;
        }
        else if (state != State::Off)
        {
            state = State::Released;
        }
        break;
    }
    case 1: // on
    {
        if (state == State::Pressed)
        {
            state = State::On;
        }
        else if (state != State::On)
        {
            state = State::Pressed;
        }
        break;
    }
    }
}


UINT Tablet::GetExpKeyNum(UINT tabletId) const
{
    if (tabletId >= 0 && tabletId < expKeys_.size())
    {
        return static_cast<UINT>(expKeys_.at(tabletId).size());
    }
    return 0;
}


float Tablet::GetX() const
{
    return static_cast<float>(packet_.pkX) / (coordX_.axMax - coordX_.axMin);
}


float Tablet::GetY() const
{
    return static_cast<float>(packet_.pkY) / (coordY_.axMax - coordY_.axMin);
}


float Tablet::GetPressure() const
{
    return static_cast<float>(packet_.pkNormalPressure) / (pressure_.axMax - pressure_.axMin);
}


float Tablet::GetWheel() const
{
    return -1.f; // TODO: implement
}


float Tablet::GetAzimuth() const
{
    return static_cast<float>(packet_.pkOrientation.orAzimuth) / (azimuth_.axMax - azimuth_.axMin);
}


float Tablet::GetAltitude() const
{
    return static_cast<float>(packet_.pkOrientation.orAltitude) / (altitude_.axMax - altitude_.axMin);
}


float Tablet::GetTwist() const
{
    return static_cast<float>(packet_.pkOrientation.orTwist) / (twist_.axMax - twist_.axMin);
}


UINT Tablet::GetPenId() const
{
    return packet_.pkCursor / static_cast<int>(CursorType::Num);
}


Tablet::CursorType Tablet::GetCursor() const
{
    return static_cast<CursorType>(packet_.pkCursor % static_cast<int>(CursorType::Num));
}


DWORD Tablet::GetTime() const
{
    return packet_.pkTime;
}


bool Tablet::GetProximity() const
{
    return proximity_;
}


bool Tablet::GetButton(USHORT id) const
{
    if (!IsValidButton(id)) return false;

    using State = ButtonState;
    const auto &state = buttons_.at(id);
    return state == State::Pressed || state == State::On;
}


bool Tablet::GetButtonDown(USHORT id) const
{
    if (!IsValidButton(id)) return false;

    using State = ButtonState;
    const auto &state = buttons_.at(id);
    return state == State::Pressed;
}


bool Tablet::GetButtonUp(USHORT id) const
{
    if (!IsValidButton(id)) return false;

    using State = ButtonState;
    const auto &state = buttons_.at(id);
    return state == State::Released;
}


bool Tablet::GetExpKey(USHORT tabletId, USHORT controlId) const
{
    if (!IsValidExpKey(tabletId, controlId)) return false;

    using State = ExpKeyState;
    const auto &state = expKeys_.at(tabletId).at(controlId);
    return state == State::Pressed || state == State::On;
}


bool Tablet::GetExpKeyDown(USHORT tabletId, USHORT controlId) const
{
    if (!IsValidExpKey(tabletId, controlId)) return false;

    using State = ExpKeyState;
    const auto &state = expKeys_.at(tabletId).at(controlId);
    return state == State::Pressed;
}


bool Tablet::GetExpKeyUp(USHORT tabletId, USHORT controlId) const
{
    if (!IsValidExpKey(tabletId, controlId)) return false;

    using State = ExpKeyState;
    const auto &state = expKeys_.at(tabletId).at(controlId);
    return state == State::Released;
}