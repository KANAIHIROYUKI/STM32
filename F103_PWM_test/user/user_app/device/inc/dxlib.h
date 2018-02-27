#ifndef _INC_DXLIB_H_
#define _INC_DXLIB_H_

#include "system.h"

class DXLIB {
 public:
  void setup (USART &usartSet,GPIO &dirSet);
  bool TxPacket (uint8_t id, uint8_t inst, uint8_t *param, uint16_t len);
  bool RxPacket (uint8_t *rdata, uint16_t rdatasize, uint8_t *rid, uint16_t *rlen, uint8_t *err);

  bool ReadByteData (uint8_t id, uint16_t addr, uint8_t *data, uint8_t *err);
  bool ReadWordData (uint8_t id, uint16_t addr, uint16_t *data, uint8_t *err);
  bool ReadLongData (uint8_t id, uint16_t addr, uint32_t *data, uint8_t *err);
  bool WriteByteData (uint8_t id, uint16_t addr, uint8_t data, uint8_t *err);
  bool WriteWordData (uint8_t id, uint16_t addr, uint16_t data, uint8_t *err);
  bool WriteLongData (uint8_t id, uint16_t addr, uint32_t data, uint8_t *err);
  bool WriteSyncByteData (const uint8_t *pid, uint16_t addr, const uint8_t *pdata, int num);
  bool WriteSyncWordData (const uint8_t *pid, uint16_t addr, const uint16_t *pdata, int num);
  bool WriteSyncLongData (const uint8_t *pid, uint16_t addr, const uint32_t *pdata, int num);
  bool Ping (uint8_t id, uint8_t *err);
  bool Reset (uint8_t id, uint8_t *err);

  uint8_t txp, rxp;
  USART *_HardSerial;
  GPIO *dir;
  bool UseSoftSerial;

  void PurgeRXBuff (void);
  void puts (uint8_t *buf, int len);
  void putch (uint8_t c);
  int gets (uint8_t *buf, int len);
  void flush (void);
};
#endif
