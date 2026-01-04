#ifndef IR_TX_CORE_H
#define IR_TX_CORE_H

#include "ir_frame.h"

void irTxInit();
void irSendFrame(const IRFrame &frame);

#endif
