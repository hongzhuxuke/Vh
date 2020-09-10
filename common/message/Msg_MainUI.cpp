#include "Msg_MainUI.h"

STRU_MAINUI_WIDGET_SHOW::STRU_MAINUI_WIDGET_SHOW() {
   memset(this, 0, sizeof(STRU_MAINUI_WIDGET_SHOW));
}

STRU_VHALL_ACTIVE_EVENT::STRU_VHALL_ACTIVE_EVENT() {
   memset(this, 0, sizeof(STRU_VHALL_ACTIVE_EVENT));
}

STRU_MAINUI_CLICK_CONTROL::STRU_MAINUI_CLICK_CONTROL() {
   memset(this, 0, sizeof(STRU_MAINUI_CLICK_CONTROL));
   m_dwExtraData = -1;
   m_bIsRoomDisConnect = false;
   m_eReason = reason_none;
}

STRU_MAINUI_LIST_CHANGE::STRU_MAINUI_LIST_CHANGE() {
   memset(this, 0, sizeof(STRU_MAINUI_LIST_CHANGE));
   m_nIndex = -1;
}

STRU_MAINUI_VOLUME_CHANGE::STRU_MAINUI_VOLUME_CHANGE() {
   memset(this, 0, sizeof(STRU_MAINUI_VOLUME_CHANGE));
}

STRU_MAINUI_MUTE::STRU_MAINUI_MUTE() {
   memset(this, 0, sizeof(STRU_MAINUI_MUTE));
}

STRU_MAINUI_PLAY_CLICK::STRU_MAINUI_PLAY_CLICK() {
   memset(this, 0, sizeof(STRU_MAINUI_PLAY_CLICK));
}

STRU_MAINUI_CAMERACTRL::STRU_MAINUI_CAMERACTRL() {
   memset(this, 0, sizeof(STRU_MAINUI_CAMERACTRL));
}

//STRU_MAINUI_CAMERA_OPTION_SAVE::STRU_MAINUI_CAMERA_OPTION_SAVE() {
//   memset(this, 0, sizeof(STRU_MAINUI_CAMERA_OPTION_SAVE));
//}

STRU_MAINUI_DEVICE_CHANGE::STRU_MAINUI_DEVICE_CHANGE() {
   memset(this, 0, sizeof(STRU_MAINUI_DEVICE_CHANGE));
}

STRU_MAINUI_DEVICE_MODIFY::STRU_MAINUI_DEVICE_MODIFY()
{
   memset(this, 0, sizeof(STRU_MAINUI_DEVICE_MODIFY));
}


STRU_MAINUI_DEVICE_FULL::STRU_MAINUI_DEVICE_FULL() {
   memset(this, 0, sizeof(STRU_MAINUI_DEVICE_FULL));
}

STRU_MAINUI_CHECKSTATUS::STRU_MAINUI_CHECKSTATUS() {
   memset(this, 0, sizeof(STRU_MAINUI_CHECKSTATUS));
}

STRU_MAINUI_DELETECAMERA::STRU_MAINUI_DELETECAMERA(){
   memset(this, 0, sizeof(STRU_MAINUI_DELETECAMERA));
}

