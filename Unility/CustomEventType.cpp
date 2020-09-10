#include "CustomEventType.h"

CustomEventType::CustomEventType(QEvent::Type type, QString data, bool bState/* = true*/) :
    QEvent(type) {
    msg = data;
    mbState = bState;
};
