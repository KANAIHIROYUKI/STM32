#ifndef _INC_DX2LIB_H_
#define _INC_DX2LIB_H_


class DX2LIB {
 public:
  DX2LIB (bool ss, HardwareSerial *hws = &Serial, uint8_t rxpin = 8, uint8_t txpin = 9);
  ~DX2LIB();
  void begin (long baud);
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
 private:
  uint8_t txp, rxp;
  HardwareSerial *_HardSerial;
  bool UseSoftSerial;
  bool active (void);
  uint16_t crc16 (uint16_t *crc, uint8_t *dat, uint16_t size);

  SoftwareSerial *_dxif;
  void PurgeRXBuff (void);
  void puts (uint8_t *buf, int len);
  void putch (uint8_t c);
  int gets (uint8_t *buf, int len);
  void flush (void);
};
#endif
