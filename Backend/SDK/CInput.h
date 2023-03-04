#pragma once
#include "vector3d.h"
#include "CUserCmd.h"

class CInput
{
public:
    char pad_0x00[0x0C]; // 0x00
    bool m_trackir_available; //0x0C
    bool m_mouse_initiated; //0x0D
    bool m_mouse_active; //0x0E
    char pad_0x0F[0xFBE];
    bool        m_fJoystickAdvancedInit;    //0x07
    void* m_pKeys;                    //0x34
    int            pad_0x41;
    int            pad_0x42;
    bool        m_fCameraInterceptingMouse;    //0x9C
    bool        m_fCameraMovingWithMouse;    //0x9E
    Vector        m_vecCameraOffset;            //0xA0
    bool        m_fCameraDistanceMove;        //0xAC
    int            m_nCameraOldX;                //0xB0
    int            m_nCameraOldY;                //0xB4
    int            m_nCameraX;                    //0xB8
    int            m_nCameraY;                    //0xBC
    bool        m_CameraIsOrthographic;        //0xC0
    Vector        m_angPreviousViewAngles;    //0xC4
    Vector        m_angPreviousViewAnglesTilt;//0xD0
    float        m_flLastForwardMove;        //0xDC
    int            m_nClearInputState;            //0xE0
    char                pad_0xE4[0x8];
    CUserCmd* m_pCommands;
    CVerifiedUserCmd* m_pVerifiedCommands;

    inline CUserCmd* GetUserCmd(int sequence_number);
    inline CUserCmd* GetUserCmd(int nSlot, int sequence_number);
    inline CVerifiedUserCmd* GetVerifiedCmd(int sequence_number);
};

CUserCmd* CInput::GetUserCmd(int sequence_number)
{
    using OriginalFn = CUserCmd * (__thiscall*)(void*, int, int);
    return vfunc<OriginalFn>(this, 8)(this, 0, sequence_number);
}

CUserCmd* CInput::GetUserCmd(int nSlot, int sequence_number)
{
    typedef CUserCmd* (__thiscall* GetUserCmd_t)(void*, int, int);
    return vfunc<GetUserCmd_t>(this, 8)(this, nSlot, sequence_number);
}

CVerifiedUserCmd* CInput::GetVerifiedCmd(int sequence_number)
{
    auto verifiedCommands = *(CVerifiedUserCmd**)(reinterpret_cast<uint32_t>(this) + 0xF8);
    return &verifiedCommands[sequence_number % 150];
}

