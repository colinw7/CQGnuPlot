#ifndef SELECT_pixmap_H
#define SELECT_pixmap_H

#include <CQPixmapCache.h>

class SELECT_pixmap {
 private:
  uchar data_[2680] = {
    0x3c,0x3f,0x78,0x6d,0x6c,0x20,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x3d,0x22,0x31,
    0x2e,0x30,0x22,0x20,0x65,0x6e,0x63,0x6f,0x64,0x69,0x6e,0x67,0x3d,0x22,0x55,0x54,
    0x46,0x2d,0x38,0x22,0x20,0x73,0x74,0x61,0x6e,0x64,0x61,0x6c,0x6f,0x6e,0x65,0x3d,
    0x22,0x6e,0x6f,0x22,0x3f,0x3e,0x0a,0x3c,0x21,0x2d,0x2d,0x20,0x43,0x72,0x65,0x61,
    0x74,0x65,0x64,0x20,0x77,0x69,0x74,0x68,0x20,0x49,0x6e,0x6b,0x73,0x63,0x61,0x70,
    0x65,0x20,0x28,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,0x77,0x77,0x2e,0x69,0x6e,
    0x6b,0x73,0x63,0x61,0x70,0x65,0x2e,0x6f,0x72,0x67,0x2f,0x29,0x20,0x2d,0x2d,0x3e,
    0x0a,0x0a,0x3c,0x73,0x76,0x67,0x0a,0x20,0x20,0x20,0x78,0x6d,0x6c,0x6e,0x73,0x3a,
    0x64,0x63,0x3d,0x22,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x70,0x75,0x72,0x6c,0x2e,
    0x6f,0x72,0x67,0x2f,0x64,0x63,0x2f,0x65,0x6c,0x65,0x6d,0x65,0x6e,0x74,0x73,0x2f,
    0x31,0x2e,0x31,0x2f,0x22,0x0a,0x20,0x20,0x20,0x78,0x6d,0x6c,0x6e,0x73,0x3a,0x63,
    0x63,0x3d,0x22,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x63,0x72,0x65,0x61,0x74,0x69,
    0x76,0x65,0x63,0x6f,0x6d,0x6d,0x6f,0x6e,0x73,0x2e,0x6f,0x72,0x67,0x2f,0x6e,0x73,
    0x23,0x22,0x0a,0x20,0x20,0x20,0x78,0x6d,0x6c,0x6e,0x73,0x3a,0x72,0x64,0x66,0x3d,
    0x22,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,0x77,0x77,0x2e,0x77,0x33,0x2e,0x6f,
    0x72,0x67,0x2f,0x31,0x39,0x39,0x39,0x2f,0x30,0x32,0x2f,0x32,0x32,0x2d,0x72,0x64,
    0x66,0x2d,0x73,0x79,0x6e,0x74,0x61,0x78,0x2d,0x6e,0x73,0x23,0x22,0x0a,0x20,0x20,
    0x20,0x78,0x6d,0x6c,0x6e,0x73,0x3a,0x73,0x76,0x67,0x3d,0x22,0x68,0x74,0x74,0x70,
    0x3a,0x2f,0x2f,0x77,0x77,0x77,0x2e,0x77,0x33,0x2e,0x6f,0x72,0x67,0x2f,0x32,0x30,
    0x30,0x30,0x2f,0x73,0x76,0x67,0x22,0x0a,0x20,0x20,0x20,0x78,0x6d,0x6c,0x6e,0x73,
    0x3d,0x22,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,0x77,0x77,0x2e,0x77,0x33,0x2e,
    0x6f,0x72,0x67,0x2f,0x32,0x30,0x30,0x30,0x2f,0x73,0x76,0x67,0x22,0x0a,0x20,0x20,
    0x20,0x78,0x6d,0x6c,0x6e,0x73,0x3a,0x73,0x6f,0x64,0x69,0x70,0x6f,0x64,0x69,0x3d,
    0x22,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x73,0x6f,0x64,0x69,0x70,0x6f,0x64,0x69,
    0x2e,0x73,0x6f,0x75,0x72,0x63,0x65,0x66,0x6f,0x72,0x67,0x65,0x2e,0x6e,0x65,0x74,
    0x2f,0x44,0x54,0x44,0x2f,0x73,0x6f,0x64,0x69,0x70,0x6f,0x64,0x69,0x2d,0x30,0x2e,
    0x64,0x74,0x64,0x22,0x0a,0x20,0x20,0x20,0x78,0x6d,0x6c,0x6e,0x73,0x3a,0x69,0x6e,
    0x6b,0x73,0x63,0x61,0x70,0x65,0x3d,0x22,0x68,0x74,0x74,0x70,0x3a,0x2f,0x2f,0x77,
    0x77,0x77,0x2e,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x2e,0x6f,0x72,0x67,0x2f,
    0x6e,0x61,0x6d,0x65,0x73,0x70,0x61,0x63,0x65,0x73,0x2f,0x69,0x6e,0x6b,0x73,0x63,
    0x61,0x70,0x65,0x22,0x0a,0x20,0x20,0x20,0x77,0x69,0x64,0x74,0x68,0x3d,0x22,0x34,
    0x35,0x30,0x2e,0x35,0x32,0x38,0x30,0x35,0x22,0x0a,0x20,0x20,0x20,0x68,0x65,0x69,
    0x67,0x68,0x74,0x3d,0x22,0x34,0x34,0x36,0x2e,0x34,0x38,0x37,0x34,0x33,0x22,0x0a,
    0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x73,0x76,0x67,0x32,0x22,0x0a,0x20,0x20,0x20,
    0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x3d,0x22,0x31,0x2e,0x31,0x22,0x0a,0x20,0x20,
    0x20,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x3a,0x76,0x65,0x72,0x73,0x69,0x6f,
    0x6e,0x3d,0x22,0x30,0x2e,0x34,0x38,0x2e,0x34,0x20,0x72,0x39,0x39,0x33,0x39,0x22,
    0x0a,0x20,0x20,0x20,0x73,0x6f,0x64,0x69,0x70,0x6f,0x64,0x69,0x3a,0x64,0x6f,0x63,
    0x6e,0x61,0x6d,0x65,0x3d,0x22,0x73,0x65,0x6c,0x65,0x63,0x74,0x2e,0x73,0x76,0x67,
    0x22,0x3e,0x0a,0x20,0x20,0x3c,0x64,0x65,0x66,0x73,0x0a,0x20,0x20,0x20,0x20,0x20,
    0x69,0x64,0x3d,0x22,0x64,0x65,0x66,0x73,0x34,0x22,0x20,0x2f,0x3e,0x0a,0x20,0x20,
    0x3c,0x73,0x6f,0x64,0x69,0x70,0x6f,0x64,0x69,0x3a,0x6e,0x61,0x6d,0x65,0x64,0x76,
    0x69,0x65,0x77,0x0a,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x62,0x61,0x73,
    0x65,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x70,0x61,0x67,0x65,0x63,0x6f,0x6c,0x6f,
    0x72,0x3d,0x22,0x23,0x66,0x66,0x66,0x66,0x66,0x66,0x22,0x0a,0x20,0x20,0x20,0x20,
    0x20,0x62,0x6f,0x72,0x64,0x65,0x72,0x63,0x6f,0x6c,0x6f,0x72,0x3d,0x22,0x23,0x36,
    0x36,0x36,0x36,0x36,0x36,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x62,0x6f,0x72,0x64,
    0x65,0x72,0x6f,0x70,0x61,0x63,0x69,0x74,0x79,0x3d,0x22,0x31,0x2e,0x30,0x22,0x0a,
    0x20,0x20,0x20,0x20,0x20,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x3a,0x70,0x61,
    0x67,0x65,0x6f,0x70,0x61,0x63,0x69,0x74,0x79,0x3d,0x22,0x30,0x2e,0x30,0x22,0x0a,
    0x20,0x20,0x20,0x20,0x20,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x3a,0x70,0x61,
    0x67,0x65,0x73,0x68,0x61,0x64,0x6f,0x77,0x3d,0x22,0x32,0x22,0x0a,0x20,0x20,0x20,
    0x20,0x20,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x3a,0x7a,0x6f,0x6f,0x6d,0x3d,
    0x22,0x30,0x2e,0x39,0x38,0x39,0x39,0x34,0x39,0x35,0x22,0x0a,0x20,0x20,0x20,0x20,
    0x20,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x3a,0x63,0x78,0x3d,0x22,0x36,0x39,
    0x2e,0x31,0x37,0x37,0x34,0x34,0x37,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x69,0x6e,
    0x6b,0x73,0x63,0x61,0x70,0x65,0x3a,0x63,0x79,0x3d,0x22,0x31,0x33,0x32,0x2e,0x36,
    0x35,0x39,0x35,0x33,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x69,0x6e,0x6b,0x73,0x63,
    0x61,0x70,0x65,0x3a,0x64,0x6f,0x63,0x75,0x6d,0x65,0x6e,0x74,0x2d,0x75,0x6e,0x69,
    0x74,0x73,0x3d,0x22,0x70,0x78,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x69,0x6e,0x6b,
    0x73,0x63,0x61,0x70,0x65,0x3a,0x63,0x75,0x72,0x72,0x65,0x6e,0x74,0x2d,0x6c,0x61,
    0x79,0x65,0x72,0x3d,0x22,0x6c,0x61,0x79,0x65,0x72,0x31,0x22,0x0a,0x20,0x20,0x20,
    0x20,0x20,0x73,0x68,0x6f,0x77,0x67,0x72,0x69,0x64,0x3d,0x22,0x66,0x61,0x6c,0x73,
    0x65,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,
    0x3a,0x77,0x69,0x6e,0x64,0x6f,0x77,0x2d,0x77,0x69,0x64,0x74,0x68,0x3d,0x22,0x31,
    0x34,0x36,0x30,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x69,0x6e,0x6b,0x73,0x63,0x61,
    0x70,0x65,0x3a,0x77,0x69,0x6e,0x64,0x6f,0x77,0x2d,0x68,0x65,0x69,0x67,0x68,0x74,
    0x3d,0x22,0x31,0x30,0x33,0x33,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x69,0x6e,0x6b,
    0x73,0x63,0x61,0x70,0x65,0x3a,0x77,0x69,0x6e,0x64,0x6f,0x77,0x2d,0x78,0x3d,0x22,
    0x35,0x32,0x33,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x69,0x6e,0x6b,0x73,0x63,0x61,
    0x70,0x65,0x3a,0x77,0x69,0x6e,0x64,0x6f,0x77,0x2d,0x79,0x3d,0x22,0x31,0x35,0x33,
    0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x3a,
    0x77,0x69,0x6e,0x64,0x6f,0x77,0x2d,0x6d,0x61,0x78,0x69,0x6d,0x69,0x7a,0x65,0x64,
    0x3d,0x22,0x30,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x66,0x69,0x74,0x2d,0x6d,0x61,
    0x72,0x67,0x69,0x6e,0x2d,0x74,0x6f,0x70,0x3d,0x22,0x30,0x22,0x0a,0x20,0x20,0x20,
    0x20,0x20,0x66,0x69,0x74,0x2d,0x6d,0x61,0x72,0x67,0x69,0x6e,0x2d,0x6c,0x65,0x66,
    0x74,0x3d,0x22,0x30,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x66,0x69,0x74,0x2d,0x6d,
    0x61,0x72,0x67,0x69,0x6e,0x2d,0x72,0x69,0x67,0x68,0x74,0x3d,0x22,0x30,0x22,0x0a,
    0x20,0x20,0x20,0x20,0x20,0x66,0x69,0x74,0x2d,0x6d,0x61,0x72,0x67,0x69,0x6e,0x2d,
    0x62,0x6f,0x74,0x74,0x6f,0x6d,0x3d,0x22,0x30,0x22,0x20,0x2f,0x3e,0x0a,0x20,0x20,
    0x3c,0x6d,0x65,0x74,0x61,0x64,0x61,0x74,0x61,0x0a,0x20,0x20,0x20,0x20,0x20,0x69,
    0x64,0x3d,0x22,0x6d,0x65,0x74,0x61,0x64,0x61,0x74,0x61,0x37,0x22,0x3e,0x0a,0x20,
    0x20,0x20,0x20,0x3c,0x72,0x64,0x66,0x3a,0x52,0x44,0x46,0x3e,0x0a,0x20,0x20,0x20,
    0x20,0x20,0x20,0x3c,0x63,0x63,0x3a,0x57,0x6f,0x72,0x6b,0x0a,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x72,0x64,0x66,0x3a,0x61,0x62,0x6f,0x75,0x74,0x3d,0x22,
    0x22,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x64,0x63,0x3a,0x66,
    0x6f,0x72,0x6d,0x61,0x74,0x3e,0x69,0x6d,0x61,0x67,0x65,0x2f,0x73,0x76,0x67,0x2b,
    0x78,0x6d,0x6c,0x3c,0x2f,0x64,0x63,0x3a,0x66,0x6f,0x72,0x6d,0x61,0x74,0x3e,0x0a,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x64,0x63,0x3a,0x74,0x79,0x70,0x65,
    0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x72,0x64,0x66,0x3a,
    0x72,0x65,0x73,0x6f,0x75,0x72,0x63,0x65,0x3d,0x22,0x68,0x74,0x74,0x70,0x3a,0x2f,
    0x2f,0x70,0x75,0x72,0x6c,0x2e,0x6f,0x72,0x67,0x2f,0x64,0x63,0x2f,0x64,0x63,0x6d,
    0x69,0x74,0x79,0x70,0x65,0x2f,0x53,0x74,0x69,0x6c,0x6c,0x49,0x6d,0x61,0x67,0x65,
    0x22,0x20,0x2f,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c,0x64,0x63,
    0x3a,0x74,0x69,0x74,0x6c,0x65,0x20,0x2f,0x3e,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,
    0x3c,0x2f,0x63,0x63,0x3a,0x57,0x6f,0x72,0x6b,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,
    0x2f,0x72,0x64,0x66,0x3a,0x52,0x44,0x46,0x3e,0x0a,0x20,0x20,0x3c,0x2f,0x6d,0x65,
    0x74,0x61,0x64,0x61,0x74,0x61,0x3e,0x0a,0x20,0x20,0x3c,0x67,0x0a,0x20,0x20,0x20,
    0x20,0x20,0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x3a,0x6c,0x61,0x62,0x65,0x6c,
    0x3d,0x22,0x4c,0x61,0x79,0x65,0x72,0x20,0x31,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,
    0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x3a,0x67,0x72,0x6f,0x75,0x70,0x6d,0x6f,
    0x64,0x65,0x3d,0x22,0x6c,0x61,0x79,0x65,0x72,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,
    0x69,0x64,0x3d,0x22,0x6c,0x61,0x79,0x65,0x72,0x31,0x22,0x0a,0x20,0x20,0x20,0x20,
    0x20,0x74,0x72,0x61,0x6e,0x73,0x66,0x6f,0x72,0x6d,0x3d,0x22,0x74,0x72,0x61,0x6e,
    0x73,0x6c,0x61,0x74,0x65,0x28,0x34,0x2e,0x37,0x38,0x36,0x36,0x38,0x39,0x36,0x2c,
    0x37,0x2e,0x35,0x33,0x33,0x36,0x33,0x32,0x33,0x29,0x22,0x3e,0x0a,0x20,0x20,0x20,
    0x20,0x3c,0x72,0x65,0x63,0x74,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x73,0x74,
    0x79,0x6c,0x65,0x3d,0x22,0x66,0x69,0x6c,0x6c,0x3a,0x6e,0x6f,0x6e,0x65,0x3b,0x73,
    0x74,0x72,0x6f,0x6b,0x65,0x3a,0x6e,0x6f,0x6e,0x65,0x22,0x0a,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x72,0x65,0x63,0x74,0x32,0x39,0x38,0x38,0x22,
    0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x77,0x69,0x64,0x74,0x68,0x3d,0x22,0x34,
    0x35,0x30,0x2e,0x35,0x32,0x38,0x30,0x35,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x68,0x65,0x69,0x67,0x68,0x74,0x3d,0x22,0x34,0x34,0x36,0x2e,0x34,0x38,0x37,
    0x34,0x33,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x78,0x3d,0x22,0x2d,0x32,
    0x30,0x2e,0x32,0x30,0x33,0x30,0x35,0x31,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x79,0x3d,0x22,0x2d,0x32,0x30,0x2e,0x36,0x38,0x37,0x37,0x36,0x31,0x22,0x0a,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x74,0x72,0x61,0x6e,0x73,0x66,0x6f,0x72,0x6d,
    0x3d,0x22,0x74,0x72,0x61,0x6e,0x73,0x6c,0x61,0x74,0x65,0x28,0x31,0x35,0x2e,0x34,
    0x31,0x36,0x33,0x36,0x31,0x2c,0x31,0x33,0x2e,0x31,0x35,0x34,0x31,0x32,0x39,0x29,
    0x22,0x20,0x2f,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,0x70,0x61,0x74,0x68,0x0a,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x69,0x6c,
    0x6c,0x3a,0x6e,0x6f,0x6e,0x65,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x3a,0x23,0x30,
    0x30,0x30,0x30,0x30,0x30,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x77,0x69,0x64,
    0x74,0x68,0x3a,0x34,0x30,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x6c,0x69,0x6e,
    0x65,0x63,0x61,0x70,0x3a,0x62,0x75,0x74,0x74,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,
    0x2d,0x6c,0x69,0x6e,0x65,0x6a,0x6f,0x69,0x6e,0x3a,0x6d,0x69,0x74,0x65,0x72,0x3b,
    0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x6d,0x69,0x74,0x65,0x72,0x6c,0x69,0x6d,0x69,
    0x74,0x3a,0x34,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x6f,0x70,0x61,0x63,0x69,
    0x74,0x79,0x3a,0x31,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x64,0x61,0x73,0x68,
    0x61,0x72,0x72,0x61,0x79,0x3a,0x6e,0x6f,0x6e,0x65,0x3b,0x6d,0x61,0x72,0x6b,0x65,
    0x72,0x2d,0x65,0x6e,0x64,0x3a,0x6e,0x6f,0x6e,0x65,0x22,0x0a,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x64,0x3d,0x22,0x4d,0x20,0x33,0x39,0x32,0x2e,0x39,0x34,0x39,0x33,
    0x34,0x2c,0x33,0x39,0x34,0x2e,0x39,0x36,0x39,0x36,0x35,0x20,0x37,0x35,0x2e,0x32,
    0x35,0x36,0x33,0x36,0x34,0x2c,0x37,0x36,0x2e,0x32,0x36,0x36,0x35,0x32,0x32,0x22,
    0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x70,0x61,0x74,0x68,
    0x32,0x39,0x39,0x31,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x6e,0x6b,
    0x73,0x63,0x61,0x70,0x65,0x3a,0x63,0x6f,0x6e,0x6e,0x65,0x63,0x74,0x6f,0x72,0x2d,
    0x63,0x75,0x72,0x76,0x61,0x74,0x75,0x72,0x65,0x3d,0x22,0x30,0x22,0x0a,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x74,0x72,0x61,0x6e,0x73,0x66,0x6f,0x72,0x6d,0x3d,0x22,
    0x74,0x72,0x61,0x6e,0x73,0x6c,0x61,0x74,0x65,0x28,0x2d,0x34,0x2e,0x37,0x38,0x36,
    0x36,0x38,0x39,0x36,0x2c,0x2d,0x37,0x2e,0x35,0x33,0x33,0x36,0x33,0x32,0x33,0x29,
    0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x73,0x6f,0x64,0x69,0x70,0x6f,0x64,
    0x69,0x3a,0x6e,0x6f,0x64,0x65,0x74,0x79,0x70,0x65,0x73,0x3d,0x22,0x63,0x63,0x22,
    0x20,0x2f,0x3e,0x0a,0x20,0x20,0x20,0x20,0x3c,0x70,0x61,0x74,0x68,0x0a,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x73,0x74,0x79,0x6c,0x65,0x3d,0x22,0x66,0x69,0x6c,0x6c,
    0x3a,0x23,0x30,0x30,0x30,0x30,0x30,0x30,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x3a,
    0x23,0x30,0x30,0x30,0x30,0x30,0x30,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x77,
    0x69,0x64,0x74,0x68,0x3a,0x31,0x70,0x78,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,
    0x6c,0x69,0x6e,0x65,0x63,0x61,0x70,0x3a,0x62,0x75,0x74,0x74,0x3b,0x73,0x74,0x72,
    0x6f,0x6b,0x65,0x2d,0x6c,0x69,0x6e,0x65,0x6a,0x6f,0x69,0x6e,0x3a,0x6d,0x69,0x74,
    0x65,0x72,0x3b,0x73,0x74,0x72,0x6f,0x6b,0x65,0x2d,0x6f,0x70,0x61,0x63,0x69,0x74,
    0x79,0x3a,0x31,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x64,0x3d,0x22,0x4d,
    0x20,0x35,0x33,0x2e,0x35,0x33,0x38,0x30,0x38,0x35,0x2c,0x35,0x33,0x2e,0x35,0x33,
    0x38,0x30,0x38,0x39,0x20,0x31,0x31,0x37,0x2e,0x31,0x37,0x37,0x36,0x39,0x2c,0x33,
    0x31,0x31,0x2e,0x31,0x32,0x36,0x39,0x38,0x20,0x31,0x36,0x30,0x2e,0x36,0x31,0x34,
    0x32,0x35,0x2c,0x31,0x36,0x30,0x2e,0x36,0x31,0x34,0x32,0x36,0x20,0x32,0x38,0x31,
    0x2e,0x38,0x33,0x32,0x35,0x36,0x2c,0x31,0x31,0x36,0x2e,0x31,0x36,0x37,0x35,0x35,
    0x20,0x7a,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x69,0x64,0x3d,0x22,0x70,
    0x61,0x74,0x68,0x33,0x39,0x34,0x31,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x69,0x6e,0x6b,0x73,0x63,0x61,0x70,0x65,0x3a,0x63,0x6f,0x6e,0x6e,0x65,0x63,0x74,
    0x6f,0x72,0x2d,0x63,0x75,0x72,0x76,0x61,0x74,0x75,0x72,0x65,0x3d,0x22,0x30,0x22,
    0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x74,0x72,0x61,0x6e,0x73,0x66,0x6f,0x72,
    0x6d,0x3d,0x22,0x74,0x72,0x61,0x6e,0x73,0x6c,0x61,0x74,0x65,0x28,0x2d,0x34,0x2e,
    0x37,0x38,0x36,0x36,0x38,0x39,0x36,0x2c,0x2d,0x37,0x2e,0x35,0x33,0x33,0x36,0x33,
    0x32,0x33,0x29,0x22,0x0a,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x73,0x6f,0x64,0x69,
    0x70,0x6f,0x64,0x69,0x3a,0x6e,0x6f,0x64,0x65,0x74,0x79,0x70,0x65,0x73,0x3d,0x22,
    0x63,0x63,0x63,0x63,0x63,0x22,0x20,0x2f,0x3e,0x0a,0x20,0x20,0x3c,0x2f,0x67,0x3e,
    0x0a,0x3c,0x2f,0x73,0x76,0x67,0x3e,0x0a,
  };

 public:
  SELECT_pixmap() {
    CQPixmapCache::instance()->addData("SELECT", data_, 2680);
  }
};

static SELECT_pixmap s_SELECT_pixmap;

#endif
