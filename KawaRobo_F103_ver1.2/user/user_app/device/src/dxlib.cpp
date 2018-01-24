
#include "dxlib.h"



void DXLIB::setup (USART &usartSet,GPIO &dirSet) {
	this->_HardSerial = &usartSet;
	this->dir = &dirSet;
}


void DXLIB::PurgeRXBuff (void) {
	while (_HardSerial->available()) _HardSerial->read();
}

void DXLIB::puts (uint8_t *buf, int len) {
	dir->write(1);
	for(int i=0;i<100;i++);
	for(int i=0;i<len;i++){
		_HardSerial->send(buf[len]);
	}
	dir->write(0);
}

void DXLIB::putch (uint8_t c) {
	dir->write(1);
	for(int i=0;i<100;i++);
	_HardSerial->send(c);
	dir->write(0);
}

int DXLIB::gets (uint8_t *buf, int len) {
	int getAve = _HardSerial->available();
	for(int i=0;i<_HardSerial->available();i++){
		buf[i] = _HardSerial->read();
	}
  return getAve;
}

void DXLIB::flush (void) {
	_HardSerial->flush();
}

bool DXLIB::TxPacket (uint8_t id, uint8_t inst, uint8_t *param, uint16_t len) {

  if (id > 254) return false;
  uint8_t sum = 0, buf[5] = {0xff, 0xff, id, (uint8_t)(len + 2), inst};
  PurgeRXBuff();
  for (int i = 2; i < 5; i++) sum += buf[i];
  puts (buf, 5);
  if (len > 0) {
    for (int i = 0; i < len; i++) sum += param[i];
    puts (param, len);
  }
  sum = ~sum;
  puts (&sum, 1);
  flush();
  return true;
}

bool DXLIB::RxPacket (uint8_t *rdata, uint16_t rdatasize, uint8_t *rid, uint16_t *rlen, uint8_t *err) {
  if (rdata == NULL) return false;
  if (rlen == NULL) return false;
  uint8_t buf[5];
  if (gets (buf, 5) == 5) {
    if ((* (uint16_t *)&buf[0] == 0xffffUL) && (buf[2] <= 0xfd) && (buf[3] >= 2)) {
      if (rid != NULL) *rid = buf[2];
      if (err != NULL) *err = buf[4];
      uint8_t len = buf[3] - 2;
      uint8_t sum = 0;
      if (len <= rdatasize) {
        if (gets (rdata, len) == len) {
          *rlen = len;
          for (int i = 0; i < len; i++) sum += rdata[i];
          if (gets (buf, 1) == 1) return (sum = ~buf[0]);
        }
      }
    }
  }
  *rlen = 0;
  return false;
}

// read byte
bool DXLIB::ReadByteData (uint8_t id, uint16_t addr, uint8_t *data, uint8_t *err) {
  uint8_t buf[2] = {(uint8_t)(addr), 1};
  if (TxPacket (id, 0x02, buf, 2)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr)) {
      if (err != NULL) *err = rerr;
      if ((len == 1) && (rid == id) && ((rerr & 0x5a) == 0)) {
        *data = buf[0];
        return true;
      }
    }
  }
  return false;
}

// read word
bool DXLIB::ReadWordData (uint8_t id, uint16_t addr, uint16_t *data, uint8_t *err) {
  uint8_t buf[2] = {(uint8_t)(addr), 2};
  if (TxPacket (id, 0x02, buf, 2)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr)) {
      if (err != NULL) *err = rerr;
      if ((len == 2) && (rid == id) && ((rerr & 0x5a) == 0)) {
        *data = * (uint16_t *)&buf[0];
        return true;
      }
    }
  }
  return false;
}

// read long
bool DXLIB::ReadLongData (uint8_t id, uint16_t addr, uint32_t *data, uint8_t *err) {
  uint8_t buf[4] = {(uint8_t)(addr), 4};
  if (TxPacket (id, 0x02, buf, 2)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr)) {
      if (err != NULL) *err = rerr;
      if ((len == 4) && (rid == id) && ((rerr & 0x5a) == 0)) {
        *data = * (uint32_t *)&buf[0];
        return true;
      }
    }
  }
  return false;
}

// write byte
bool DXLIB::WriteByteData (uint8_t id, uint16_t addr, uint8_t data, uint8_t *err) {
  uint8_t buf[2] = {(uint8_t)(addr), data};
  bool result = false;
  if (TxPacket (id, 0x03, buf, 2)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len,  &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x5a) == 0);
    if (err != NULL) *err = rerr;
  }
  return result;
}

// write word
bool DXLIB::WriteWordData (uint8_t id, uint16_t addr, uint16_t data, uint8_t *err) {
  uint8_t buf[3] = {(uint8_t)(addr), (uint8_t)(data & 0xff), (uint8_t)(data >> 8)};
  bool result = false;
  if (TxPacket (id, 0x03, buf, 3)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x5a) == 0);
    if (err != NULL) *err = rerr;
  }
  return result;
}

// write long
bool DXLIB::WriteLongData (uint8_t id, uint16_t addr, uint32_t data, uint8_t *err) {
  uint8_t buf[5] = {(uint8_t)(addr)};
  bool result = false;
  * (uint32_t *)&buf[1] = data;
  if (TxPacket (id, 0x03, buf, 5)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x5a) == 0);
    if (err != NULL) *err = rerr;
  }
  return result;
}

// sync write byte
bool DXLIB::WriteSyncByteData (const uint8_t *pid, uint16_t addr, const uint8_t *pdata, int num) {
  uint8_t buf[7] = {0xff, 0xff, 0xfe, (uint8_t) ((num * 2 + 4) & 0xff), 0x83, (uint8_t)(addr), 1};
  uint8_t sum = 0;
  for (int i = 2; i < 7; i++) sum += buf[i];
  PurgeRXBuff();
  puts (buf, 7);
  for (int i = 0; i < num; i++) {
    sum += pid[i];
    putch (pid[i]);
    sum += pdata[i];
    putch (pdata[i]);
  }
  putch (~sum);
  flush();
  return true;
}

// sync write word
bool DXLIB::WriteSyncWordData (const uint8_t *pid, uint16_t addr, const uint16_t *pdata, int num) {
  uint8_t buf[7] = {0xff, 0xff, 0xfe, (uint8_t) ((num * 3 + 4) & 0xff), 0x83, (uint8_t)(addr), 2};
  uint8_t sum = 0;
  for (int i = 2; i < 7; i++) sum += buf[i];
  PurgeRXBuff();
  puts (buf, 7);
  for (int i = 0; i < num; i++) {
    sum += pid[i];
    putch (pid[i]);
    sum += (pdata[i] & 0xff);
    putch ((pdata[i] & 0xff));
    sum += ((pdata[i] >> 8) & 0xff);
    putch ((pdata[i] >> 8) & 0xff);
  }
  putch (~sum);
  flush();
  return true;
}

// sync write long
bool DXLIB::WriteSyncLongData (const uint8_t *pid, uint16_t addr, const uint32_t *pdata, int num) {
  uint8_t buf[7] = {0xff, 0xff, 0xfe, (uint8_t) ((num * 5 + 4) & 0xff), 0x83, (uint8_t)(addr), 4};
  uint8_t sum = 0;
  for (int i = 2; i < 7; i++) sum += buf[i];
  PurgeRXBuff();
  puts (buf, 7);
  for (int i = 0; i < num; i++) {
    sum += pid[i];
    putch (pid[i]);
    sum += (pdata[i] & 0xff);
    putch ((pdata[i] & 0xff));
    sum += ((pdata[i] >> 8) & 0xff);
    putch ((pdata[i] >> 8) & 0xff);
    sum += ((pdata[i] >> 16) & 0xff);
    putch ((pdata[i] >> 16) & 0xff);
    sum += ((pdata[i] >> 24) & 0xff);
    putch ((pdata[i] >> 24) & 0xff);
  }
  putch (~sum);
  flush();
  return true;
}

// ping
bool DXLIB::Ping (uint8_t id, uint8_t *err) {
  if (id > 253) return false;
  uint8_t buf[5];
  bool result = false;
  if (TxPacket (id, 0x01, NULL, 0)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x5a) == 0);
    if (err != NULL) *err = rerr;
  }
  return result;
}

// factory reset
bool DXLIB::Reset (uint8_t id, uint8_t *err) {
  if (id >= 254) return false;
  uint8_t buf[5];
  bool result = false;
  if (TxPacket (id, 0x06, NULL, 0)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x5a) == 0);
    if (err != NULL) *err = rerr;
  }
  return result;
}
