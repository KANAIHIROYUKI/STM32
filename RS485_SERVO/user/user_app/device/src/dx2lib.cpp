/*
  dx2lib.cpp
  Dynamixel Protocol V.2 Library for DXSHIELD

  Copyright (c) BestTechnology CO.,LTD. 2017
*/

#include "dx2lib.h"
#include <avr/pgmspace.h>

DX2LIB::DX2LIB (bool ss, HardwareSerial *hws, uint8_t rxpin, uint8_t txpin) {
  UseSoftSerial = ss;
  _HardSerial = hws;
  txp = txpin;
  rxp = rxpin;
}

void DX2LIB::begin (long baud) {
  if (UseSoftSerial) {
    _dxif = new SoftwareSerial (rxp, txp);
    _dxif->begin (baud);
    _dxif->setTimeout (20);
  } else {
    _HardSerial->begin (baud);
    _HardSerial->setTimeout (20);
  }
}

DX2LIB::~DX2LIB() {
  if (UseSoftSerial) {
    _dxif->end();
    _dxif = NULL;
  } else {
    _HardSerial->end();
  }
}

bool DX2LIB::active (void) {
  if (UseSoftSerial) {
    return (_dxif != NULL);
  } else {
    if (Serial) return true;
    else return false;
  }
}

void DX2LIB::PurgeRXBuff (void) {
  if (UseSoftSerial) {
    while (_dxif->available()) _dxif->read();
  } else {
    while (_HardSerial->available()) _HardSerial->read();
  }
}

void DX2LIB::puts (uint8_t *buf, int len) {
  if (UseSoftSerial) _dxif->write (buf, len);
  else _HardSerial->write (buf, len);
}

void DX2LIB::putch (uint8_t c) {
  if (UseSoftSerial) _dxif->write (c);
  else _HardSerial->write (c);
}

int DX2LIB::gets (uint8_t *buf, int len) {
  return (UseSoftSerial) ? _dxif->readBytes (buf, len) : _HardSerial->readBytes (buf, len);
}

void DX2LIB::flush (void) {
  if (UseSoftSerial) _dxif->flush();
  else _HardSerial->flush();
}

uint16_t DX2LIB::crc16 (uint16_t *crc, uint8_t *dat, uint16_t size) {
  const uint16_t crc16_lookuptable[256] PROGMEM = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
    0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
    0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
    0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
    0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
    0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
    0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
    0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
    0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
    0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
  };
  for (int16_t i = 0; i < size; i++) *crc = (*crc << 8) ^ crc16_lookuptable[ (((*crc >> 8) ^ dat[i]) & 0xff)];
  return *crc;
}

bool DX2LIB::TxPacket (uint8_t id, uint8_t inst, uint8_t *param, uint16_t len) {
  if (!active()) return false;
  if (id > 254) return false;
  uint16_t crc = 0x0e28;
  uint8_t buf[13] = {0xff, 0xff, 0xfd, 0, id,  0x00, 0x00,  inst};
  * (uint16_t *)&buf[5] = len + 3;
  crc16 (&crc, &buf[4], 4);
  PurgeRXBuff();
  puts (buf, 8);
  if (len > 0) {
    crc16 (&crc, param, len);
    puts (param, len);
  }
  * (uint16_t *)&buf[0] = crc;
  puts ((uint8_t *)&crc, 2);
  flush();
  return true;
}

bool DX2LIB::RxPacket (uint8_t *rdata, uint16_t rdatasize, uint8_t *rid, uint16_t *rlen, uint8_t *err) {
  if (!active()) return false;
  if (rdata == NULL) return false;
  if (rlen == NULL) return false;
  uint8_t buf[9];
  if (gets (buf, 9) == 9) {
    if ((* (uint32_t *)&buf[0] == 0x00fdffffUL) && (buf[7] == 0x55) && (* (uint16_t *)&buf[5] >= 4)) {
      if (rid != NULL) *rid = buf[4];
      if (err != NULL) *err = buf[8];
      uint16_t len = * (uint16_t *)&buf[5] - 4;
      uint16_t crc = 0;
      crc16 (&crc, buf, 9);
      if (len <= rdatasize) {
        if (gets (rdata, len) == len) {
          *rlen = len;
          crc16 (&crc, rdata, len);
          if (gets (buf, 2) == 2) return (crc == * (uint16_t *)&buf[0]);
        }
      }
    }
  }
  *rlen = 0;
  return false;
}

// read byte
bool DX2LIB::ReadByteData (uint8_t id, uint16_t addr, uint8_t *data, uint8_t *err) {
  uint8_t buf[10] = {addr & 0xff, addr >> 8, 1, 0};
  if (TxPacket (id, 0x02, buf, 4)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr)) {
      if (err != NULL) *err = rerr;
      if ((len == 1) && (rid == id) && ((rerr & 0x7f) == 0)) {
        *data = buf[0];
        return true;
      }
    }
  }
  return false;
}

// read word
bool DX2LIB::ReadWordData (uint8_t id, uint16_t addr, uint16_t *data, uint8_t *err) {
  uint8_t buf[10] = {addr & 0xff, addr >> 8, 2, 0};
  if (TxPacket (id, 0x02, buf, 4)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, err)) {
      if (err != NULL) *err = rerr;
      if ((len == 2) && (rid == id) && ((rerr & 0x7f) == 0)) {
        *data = * (uint16_t *)&buf[0];
        return true;
      }
    }
  }
  return false;
}

// read long
bool DX2LIB::ReadLongData (uint8_t id, uint16_t addr, uint32_t *data, uint8_t *err) {
  uint8_t buf[10] = {addr & 0xff, addr >> 8, 4, 0};
  if (TxPacket (id, 0x02, buf, 4)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, err)) {
      if (err != NULL) *err = rerr;
      if ((rid == id) && ((rerr & 0x7f) == 0)) {
        if (len == 4) {
          *data = * (uint32_t *)&buf[0];
          return true;
        } else if (len == 5) {
          if (* (uint32_t *)&buf[0] == 0xfdfdffffUL) {
            *data = 0xfdffff00UL | (uint32_t)buf[4];
            return true;
          } else if (* (uint32_t *)&buf[1] == 0xfdfdffffUL) {
            *data = 0x00fdffffUL | ((uint32_t)buf[0] << 24);
            return true;
          }
        }
      }
    }
  }
  return false;
}

// write byte
bool DX2LIB::WriteByteData (uint8_t id, uint16_t addr, uint8_t data, uint8_t *err) {
  uint8_t buf[10] = {addr & 0xff, addr >> 8, data};
  bool result = false;
  if (TxPacket (id, 0x03, buf, 3)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x7f) == 0);
    if (err != NULL) *err = rerr;
  }
  return result;
}

// write word
bool DX2LIB::WriteWordData (uint8_t id, uint16_t addr, uint16_t data, uint8_t *err) {
  uint8_t buf[10] = {addr & 0xff, addr >> 8, data & 0xff, data >> 8};
  bool result = false;
  if (TxPacket (id, 0x03, buf, 4)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr)) {
      if (err != NULL) *err = rerr;
      result = (rid == id) && (len == 0) && ((rerr & 0x7f) == 0);
    }
    if (err != NULL) *err = rerr;
  }
  return result;
}

// write long
bool DX2LIB::WriteLongData (uint8_t id, uint16_t addr, uint32_t data, uint8_t *err) {
  uint8_t buf[10] = {addr & 0xff, addr >> 8};
  bool result = false;
  uint16_t wl;
  if ((data & 0x00ffffffUL) == 0xfdffffUL) {
    * (uint32_t *)&buf[2] = 0xfdfdffffUL;
    buf[6] = (data >> 24);
    wl = 7;
  } else if ((data & 0xffffff00UL) == 0xfdffff00UL) {
    * (uint32_t *)&buf[3] = 0xfdfdffffUL;
    buf[2] = data & 0xff;
    wl = 7;
  } else {
    * (uint32_t *)&buf[2] = data;
    wl = 6;
  }
  if (TxPacket (id, 0x03, buf, wl)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x7f) == 0);
    if (err != NULL) *err = rerr;
  }
  return result;
}

// write sync byte
bool DX2LIB::WriteSyncByteData (const uint8_t *pid, uint16_t addr, const uint8_t *pdata, int num) {
  if (!active()) return false;
  uint16_t crc = 0x2a20;
  uint16_t l = num * 2;
  uint8_t buf[12] = {0xff, 0xff, 0xfd, 0, 0xfe, (7 + l) & 0xff, (7 + l) >> 8,  0x83,  addr & 0xff, addr >> 8,  0x01, 0};
  PurgeRXBuff();
  crc16 (&crc, &buf[5], 7);
  puts (buf, 12);
  for (int i = 0; i < num; i++) {
    buf[0] = pid[i];
    buf[1] = pdata[i];
    puts (buf, 2);
    crc16 (&crc, buf, 2);
  }
  * (uint16_t *)&buf[0] = crc;
  puts (buf, 2);
  flush();
  return true;
}

// write sync word
bool DX2LIB::WriteSyncWordData (const uint8_t *pid, uint16_t addr, const uint16_t *pdata, int num) {
  if (!active()) return false;
  uint16_t crc = 0x2a20;
  uint16_t l = num * 3;
  uint8_t buf[12] = {0xff, 0xff, 0xfd, 0, 0xfe, (7 + l) & 0xff, (7 + l) >> 8,  0x83,  addr & 0xff, addr >> 8,  0x02, 0};
  PurgeRXBuff();
  crc16 (&crc, &buf[5], 8);
  puts (buf, 12);
  for (int i = 0; i < num; i++) {
    buf[0] = pid[i];
    * (uint16_t *)&buf[1] = pdata[i];
    puts (buf, 3);
    crc16 (&crc, buf, 4);
  }
  * (uint16_t *)&buf[0] = crc;
  puts (buf, 2);
  flush();
  return true;
}

// write sync long
bool DX2LIB::WriteSyncLongData (const uint8_t *pid, uint16_t addr, const uint32_t *pdata, int num) {
  if (!active()) return false;
  uint16_t crc = 0x2a20;
  uint16_t l = num * 5;
  uint16_t suffix = 0;
  uint8_t buf[12] = {0xff, 0xff, 0xfd, 0, 0xfe, (7 + l) & 0xff, (7 + l) >> 8,  0x83,  addr & 0xff, addr >> 8,  0x04, 0};
  PurgeRXBuff();
  for (int i = 0; i < num; i++) {
    if ((pdata[i] & 0x00ffffffUL) == 0xfdffffUL) suffix++;
    else if ((pdata[i] & 0xffffff00UL) == 0xfdffff00UL) suffix++;
  }
  * (uint16_t *)&buf[5] = 7 + l + suffix;
  crc16 (&crc, &buf[5], 7);
  puts (buf, 12);
  for (int i = 0; i < num; i++) {
    buf[0] = pid[i];
    if ((pdata[i] & 0x00ffffffUL) == 0x00fdffffUL) {
      * (uint32_t *)&buf[1] = 0xfdfdffffUL;
      buf[5] = (pdata[i] >> 24);
      puts (buf, 6);
      crc16 (&crc, buf, 6);
    } else if ((pdata[i] & 0xffffff00UL) == 0xfdffff00UL) {
      * (uint32_t *)&buf[2] = 0xfdfdffffUL;
      buf[1] = pdata[i] & 0xff;
      puts (buf, 6);
      crc16 (&crc, buf, 6);
    } else {
      * (uint32_t *)&buf[1] = pdata[i];
      puts (buf, 5);
      crc16 (&crc, buf, 5);
    }
  }
  * (uint16_t *)&buf[0] = crc;
  puts (buf, 2);
  flush();
  return true;
}

// ping
bool DX2LIB::Ping (uint8_t id, uint8_t *err) {
  if (id > 252) return false;
  uint8_t buf[10];
  bool result = false;
  if (TxPacket (id, 0x01, NULL, 0)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 3) && ((rerr & 0x7f) == 0);
    if (err != NULL) *err = rerr;
  }
  return result;
}

// factory reset
bool DX2LIB::Reset (uint8_t id, uint8_t *err) {
  if (id > 252) return false;
  uint8_t buf[10] = {0xff};
  bool result = false;
  if (TxPacket (id, 0x06, buf, 1)) {
    uint8_t rid, rerr;
    uint16_t len;
    if (RxPacket (buf, sizeof (buf), &rid, &len, &rerr))
      result = (rid == id) && (len == 0) && ((rerr & 0x7f) == 0);
    if (err != NULL) *err = rerr;
  }
  return result;
}
