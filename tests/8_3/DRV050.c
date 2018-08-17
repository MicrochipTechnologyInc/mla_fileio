/*******************************************************************************
Copyright 2018 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "emulated_disk.h"

#ifdef SECTOR_SIZE
#error "Sector size already defined."
#endif

#define SECTOR_SIZE 512

static const uint8_t sector0[] = { 0x33, 0xC0, 0x8E, 0xD0, 0xBC, 0x00, 0x7C, 0xFB, 0x50, 0x07, 0x50, 0x1F, 0xFC, 0xBE, 0x1B, 0x7C, 0xBF, 0x1B, 0x06, 0x50, 0x57, 0xB9, 0xE5, 0x01, 0xF3, 0xA4, 0xCB, 0xBD, 0xBE, 0x07, 0xB1, 0x04, 0x38, 0x6E, 0x00, 0x7C, 0x09, 0x75, 0x13, 0x83, 0xC5, 0x10, 0xE2, 0xF4, 0xCD, 0x18, 0x8B, 0xF5, 0x83, 0xC6, 0x10, 0x49, 0x74, 0x19, 0x38, 0x2C, 0x74, 0xF6, 0xA0, 0xB5, 0x07, 0xB4, 0x07, 0x8B, 0xF0, 0xAC, 0x3C, 0x00, 0x74, 0xFC, 0xBB, 0x07, 0x00, 0xB4, 0x0E, 0xCD, 0x10, 0xEB, 0xF2, 0x88, 0x4E, 0x10, 0xE8, 0x46, 0x00, 0x73, 0x2A, 0xFE, 0x46, 0x10, 0x80, 0x7E, 0x04, 0x0B, 0x74, 0x0B, 0x80, 0x7E, 0x04, 0x0C, 0x74, 0x05, 0xA0, 0xB6, 0x07, 0x75, 0xD2, 0x80, 0x46, 0x02, 0x06, 0x83, 0x46, 0x08, 0x06, 0x83, 0x56, 0x0A, 0x00, 0xE8, 0x21, 0x00, 0x73, 0x05, 0xA0, 0xB6, 0x07, 0xEB, 0xBC, 0x81, 0x3E, 0xFE, 0x7D, 0x55, 0xAA, 0x74, 0x0B, 0x80, 0x7E, 0x10, 0x00, 0x74, 0xC8, 0xA0, 0xB7, 0x07, 0xEB, 0xA9, 0x8B, 0xFC, 0x1E, 0x57, 0x8B, 0xF5, 0xCB, 0xBF, 0x05, 0x00, 0x8A, 0x56, 0x00, 0xB4, 0x08, 0xCD, 0x13, 0x72, 0x23, 0x8A, 0xC1, 0x24, 0x3F, 0x98, 0x8A, 0xDE, 0x8A, 0xFC, 0x43, 0xF7, 0xE3, 0x8B, 0xD1, 0x86, 0xD6, 0xB1, 0x06, 0xD2, 0xEE, 0x42, 0xF7, 0xE2, 0x39, 0x56, 0x0A, 0x77, 0x23, 0x72, 0x05, 0x39, 0x46, 0x08, 0x73, 0x1C, 0xB8, 0x01, 0x02, 0xBB, 0x00, 0x7C, 0x8B, 0x4E, 0x02, 0x8B, 0x56, 0x00, 0xCD, 0x13, 0x73, 0x51, 0x4F, 0x74, 0x4E, 0x32, 0xE4, 0x8A, 0x56, 0x00, 0xCD, 0x13, 0xEB, 0xE4, 0x8A, 0x56, 0x00, 0x60, 0xBB, 0xAA, 0x55, 0xB4, 0x41, 0xCD, 0x13, 0x72, 0x36, 0x81, 0xFB, 0x55, 0xAA, 0x75, 0x30, 0xF6, 0xC1, 0x01, 0x74, 0x2B, 0x61, 0x60, 0x6A, 0x00, 0x6A, 0x00, 0xFF, 0x76, 0x0A, 0xFF, 0x76, 0x08, 0x6A, 0x00, 0x68, 0x00, 0x7C, 0x6A, 0x01, 0x6A, 0x10, 0xB4, 0x42, 0x8B, 0xF4, 0xCD, 0x13, 0x61, 0x61, 0x73, 0x0E, 0x4F, 0x74, 0x0B, 0x32, 0xE4, 0x8A, 0x56, 0x00, 0xCD, 0x13, 0xEB, 0xD6, 0x61, 0xF9, 0xC3, 0x49, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20, 0x70, 0x61, 0x72, 0x74, 0x69, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x00, 0x45, 0x72, 0x72, 0x6F, 0x72, 0x20, 0x6C, 0x6F, 0x61, 0x64, 0x69, 0x6E, 0x67, 0x20, 0x6F, 0x70, 0x65, 0x72, 0x61, 0x74, 0x69, 0x6E, 0x67, 0x20, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x00, 0x4D, 0x69, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x20, 0x6F, 0x70, 0x65, 0x72, 0x61, 0x74, 0x69, 0x6E, 0x67, 0x20, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x44, 0x63, 0x18, 0x2E, 0x07, 0xC3, 0x00, 0x00, 0x80, 0x01, 0x01, 0x00, 0x06, 0x10, 0x20, 0xF9, 0x20, 0x00, 0x00, 0x00, 0xE0, 0xBF, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA, };
static const uint8_t sector512[] = { 0xE5, 0x52, 0x49, 0x54, 0x45, 0x20, 0x20, 0x20, 0x54, 0x58, 0x54, 0x20, 0x00, 0x00, 0x00, 0x00, 0xAE, 0x4A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAE, 0x4A, 0x03, 0x00, 0x09, 0x00, 0x00, 0x00, 0xE5, 0x49, 0x4F, 0x4E, 0x20, 0x20, 0x20, 0x20, 0x4D, 0x50, 0x33, 0x20, 0x00, 0x00, 0x38, 0x24, 0x0F, 0x37, 0x88, 0x4B, 0x00, 0x00, 0xD3, 0x24, 0x0F, 0x37, 0xDA, 0x00, 0x37, 0x96, 0x68, 0x00, 0xE5, 0x69, 0x00, 0x6D, 0x00, 0x61, 0x00, 0x67, 0x00, 0x65, 0x00, 0x0F, 0x00, 0xCE, 0x5F, 0x00, 0x6D, 0x00, 0x6F, 0x00, 0x75, 0x00, 0x73, 0x00, 0x65, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x50, 0x00, 0xE5, 0x4D, 0x41, 0x47, 0x45, 0x5F, 0x7E, 0x31, 0x48, 0x45, 0x58, 0x20, 0x00, 0xA8, 0x84, 0x8A, 0x43, 0x3A, 0x43, 0x3A, 0x00, 0x00, 0x92, 0x82, 0x6C, 0x39, 0x02, 0x00, 0xBF, 0xD1, 0x00, 0x00, 0xE5, 0x4D, 0x41, 0x47, 0x45, 0x20, 0x20, 0x20, 0x48, 0x45, 0x58, 0x20, 0x18, 0xA8, 0x84, 0x8A, 0x43, 0x3A, 0x43, 0x3A, 0x00, 0x00, 0x92, 0x82, 0x6C, 0x39, 0x02, 0x00, 0xBF, 0xD1, 0x00, 0x00, 0xE5, 0x30, 0x00, 0x20, 0x00, 0x50, 0x00, 0x49, 0x00, 0x4D, 0x00, 0x0F, 0x00, 0xCE, 0x2E, 0x00, 0x68, 0x00, 0x65, 0x00, 0x78, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE5, 0x49, 0x00, 0x43, 0x00, 0x32, 0x00, 0x34, 0x00, 0x46, 0x00, 0x0F, 0x00, 0xCE, 0x4A, 0x00, 0x32, 0x00, 0x35, 0x00, 0x36, 0x00, 0x47, 0x00, 0x42, 0x00, 0x00, 0x00, 0x31, 0x00, 0x31, 0x00, 0xE5, 0x69, 0x00, 0x6D, 0x00, 0x61, 0x00, 0x67, 0x00, 0x65, 0x00, 0x0F, 0x00, 0xCE, 0x5F, 0x00, 0x6D, 0x00, 0x6F, 0x00, 0x75, 0x00, 0x73, 0x00, 0x65, 0x00, 0x00, 0x00, 0x5F, 0x00, 0x50, 0x00, 0xE5, 0x4D, 0x41, 0x47, 0x45, 0x5F, 0x7E, 0x31, 0x48, 0x45, 0x58, 0x20, 0x00, 0x68, 0x1B, 0x6E, 0x46, 0x3A, 0x46, 0x3A, 0x00, 0x00, 0x92, 0x82, 0x6C, 0x39, 0x02, 0x00, 0xBF, 0xD1, 0x00, 0x00, 0xE5, 0x4D, 0x41, 0x47, 0x45, 0x20, 0x20, 0x20, 0x48, 0x45, 0x58, 0x20, 0x18, 0x68, 0x1B, 0x6E, 0x46, 0x3A, 0x46, 0x3A, 0x00, 0x00, 0x92, 0x82, 0x6C, 0x39, 0x02, 0x00, 0xBF, 0xD1, 0x00, 0x00, 0xE5, 0x6C, 0x00, 0x65, 0x00, 0x2E, 0x00, 0x6D, 0x00, 0x70, 0x00, 0x0F, 0x00, 0xCE, 0x33, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE5, 0x31, 0x00, 0x32, 0x00, 0x20, 0x00, 0x2D, 0x00, 0x20, 0x00, 0x0F, 0x00, 0xCE, 0x54, 0x00, 0x68, 0x00, 0x65, 0x00, 0x20, 0x00, 0x42, 0x00, 0x61, 0x00, 0x00, 0x00, 0x74, 0x00, 0x74, 0x00, 0xE5, 0x65, 0x00, 0x20, 0x00, 0x57, 0x00, 0x61, 0x00, 0x72, 0x00, 0x0F, 0x00, 0xCE, 0x64, 0x00, 0x72, 0x00, 0x6F, 0x00, 0x62, 0x00, 0x65, 0x00, 0x20, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x20, 0x00, 0xE5, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x74, 0x00, 0x63, 0x00, 0x0F, 0x00, 0xCE, 0x68, 0x00, 0x20, 0x00, 0x41, 0x00, 0x6E, 0x00, 0x64, 0x00, 0x20, 0x00, 0x00, 0x00, 0x54, 0x00, 0x68, 0x00, 0xE5, 0x54, 0x00, 0x68, 0x00, 0x65, 0x00, 0x20, 0x00, 0x4C, 0x00, 0x0F, 0x00, 0xCE, 0x69, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x2C, 0x00, 0x20, 0x00, 0x54, 0x00, 0x00, 0x00, 0x68, 0x00, 0x65, 0x00, 0xE5, 0x48, 0x45, 0x4C, 0x49, 0x4F, 0x7E, 0x31, 0x4D, 0x50, 0x33, 0x20, 0x00, 0x5C, 0x01, 0x61, 0x49, 0x3A, 0x49, 0x3A, 0x00, 0x00, 0x68, 0x4B, 0x2A, 0x34, 0x08, 0x00, 0x37, 0x96, 0x68, 0x00, };
static const uint8_t sector513[] = { 0xE5, 0x6C, 0x00, 0x65, 0x00, 0x2E, 0x00, 0x6D, 0x00, 0x70, 0x00, 0x0F, 0x00, 0xCE, 0x33, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE5, 0x31, 0x00, 0x32, 0x00, 0x20, 0x00, 0x2D, 0x00, 0x20, 0x00, 0x0F, 0x00, 0xCE, 0x54, 0x00, 0x68, 0x00, 0x65, 0x00, 0x20, 0x00, 0x42, 0x00, 0x61, 0x00, 0x00, 0x00, 0x74, 0x00, 0x74, 0x00, 0xE5, 0x65, 0x00, 0x20, 0x00, 0x57, 0x00, 0x61, 0x00, 0x72, 0x00, 0x0F, 0x00, 0xCE, 0x64, 0x00, 0x72, 0x00, 0x6F, 0x00, 0x62, 0x00, 0x65, 0x00, 0x20, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x20, 0x00, 0xE5, 0x20, 0x00, 0x57, 0x00, 0x69, 0x00, 0x74, 0x00, 0x63, 0x00, 0x0F, 0x00, 0xCE, 0x68, 0x00, 0x20, 0x00, 0x41, 0x00, 0x6E, 0x00, 0x64, 0x00, 0x20, 0x00, 0x00, 0x00, 0x54, 0x00, 0x68, 0x00, 0xE5, 0x54, 0x00, 0x68, 0x00, 0x65, 0x00, 0x20, 0x00, 0x4C, 0x00, 0x0F, 0x00, 0xCE, 0x69, 0x00, 0x6F, 0x00, 0x6E, 0x00, 0x2C, 0x00, 0x20, 0x00, 0x54, 0x00, 0x00, 0x00, 0x68, 0x00, 0x65, 0x00, 0xE5, 0x48, 0x45, 0x4C, 0x49, 0x4F, 0x7E, 0x31, 0x4D, 0x50, 0x33, 0x20, 0x00, 0x5C, 0x01, 0x61, 0x49, 0x3A, 0x88, 0x4B, 0x00, 0x00, 0x68, 0x4B, 0x2A, 0x34, 0x08, 0x00, 0x37, 0x96, 0x68, 0x00, 0x44, 0x52, 0x56, 0x30, 0x35, 0x30, 0x20, 0x20, 0x20, 0x20, 0x20, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x52, 0x88, 0x4B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0x45, 0x41, 0x44, 0x20, 0x20, 0x20, 0x20, 0x54, 0x58, 0x54, 0x20, 0x00, 0x13, 0x78, 0x52, 0x88, 0x4B, 0x93, 0x4B, 0x00, 0x00, 0xD0, 0x80, 0x87, 0x4B, 0x02, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static const uint8_t sector32[] = { 0xEB, 0x3C, 0x90, 0x4D, 0x53, 0x57, 0x49, 0x4E, 0x34, 0x2E, 0x31, 0x00, 0x02, 0x40, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0xF8, 0xEF, 0x00, 0x20, 0x00, 0x10, 0x00, 0x20, 0x00, 0x00, 0x00, 0xE0, 0xBF, 0x3B, 0x00, 0x80, 0x01, 0x29, 0x00, 0x00, 0x00, 0x00, 0x4E, 0x4F, 0x20, 0x4E, 0x41, 0x4D, 0x45, 0x20, 0x20, 0x20, 0x20, 0x46, 0x41, 0x54, 0x31, 0x36, 0x20, 0x20, 0x20, 0x33, 0xC9, 0x8E, 0xD1, 0xBC, 0xFC, 0x7B, 0x16, 0x07, 0xBD, 0x78, 0x00, 0xC5, 0x76, 0x00, 0x1E, 0x56, 0x16, 0x55, 0xBF, 0x22, 0x05, 0x89, 0x7E, 0x00, 0x89, 0x4E, 0x02, 0xB1, 0x0B, 0xFC, 0xF3, 0xA4, 0x06, 0x1F, 0xBD, 0x00, 0x7C, 0xC6, 0x45, 0xFE, 0x0F, 0x38, 0x4E, 0x24, 0x7D, 0x20, 0x8B, 0xC1, 0x99, 0xE8, 0x7E, 0x01, 0x83, 0xEB, 0x3A, 0x66, 0xA1, 0x1C, 0x7C, 0x66, 0x3B, 0x07, 0x8A, 0x57, 0xFC, 0x75, 0x06, 0x80, 0xCA, 0x02, 0x88, 0x56, 0x02, 0x80, 0xC3, 0x10, 0x73, 0xED, 0x33, 0xC9, 0xFE, 0x06, 0xD8, 0x7D, 0x8A, 0x46, 0x10, 0x98, 0xF7, 0x66, 0x16, 0x03, 0x46, 0x1C, 0x13, 0x56, 0x1E, 0x03, 0x46, 0x0E, 0x13, 0xD1, 0x8B, 0x76, 0x11, 0x60, 0x89, 0x46, 0xFC, 0x89, 0x56, 0xFE, 0xB8, 0x20, 0x00, 0xF7, 0xE6, 0x8B, 0x5E, 0x0B, 0x03, 0xC3, 0x48, 0xF7, 0xF3, 0x01, 0x46, 0xFC, 0x11, 0x4E, 0xFE, 0x61, 0xBF, 0x00, 0x07, 0xE8, 0x28, 0x01, 0x72, 0x3E, 0x38, 0x2D, 0x74, 0x17, 0x60, 0xB1, 0x0B, 0xBE, 0xD8, 0x7D, 0xF3, 0xA6, 0x61, 0x74, 0x3D, 0x4E, 0x74, 0x09, 0x83, 0xC7, 0x20, 0x3B, 0xFB, 0x72, 0xE7, 0xEB, 0xDD, 0xFE, 0x0E, 0xD8, 0x7D, 0x7B, 0xA7, 0xBE, 0x7F, 0x7D, 0xAC, 0x98, 0x03, 0xF0, 0xAC, 0x98, 0x40, 0x74, 0x0C, 0x48, 0x74, 0x13, 0xB4, 0x0E, 0xBB, 0x07, 0x00, 0xCD, 0x10, 0xEB, 0xEF, 0xBE, 0x82, 0x7D, 0xEB, 0xE6, 0xBE, 0x80, 0x7D, 0xEB, 0xE1, 0xCD, 0x16, 0x5E, 0x1F, 0x66, 0x8F, 0x04, 0xCD, 0x19, 0xBE, 0x81, 0x7D, 0x8B, 0x7D, 0x1A, 0x8D, 0x45, 0xFE, 0x8A, 0x4E, 0x0D, 0xF7, 0xE1, 0x03, 0x46, 0xFC, 0x13, 0x56, 0xFE, 0xB1, 0x04, 0xE8, 0xC2, 0x00, 0x72, 0xD7, 0xEA, 0x00, 0x02, 0x70, 0x00, 0x52, 0x50, 0x06, 0x53, 0x6A, 0x01, 0x6A, 0x10, 0x91, 0x8B, 0x46, 0x18, 0xA2, 0x26, 0x05, 0x96, 0x92, 0x33, 0xD2, 0xF7, 0xF6, 0x91, 0xF7, 0xF6, 0x42, 0x87, 0xCA, 0xF7, 0x76, 0x1A, 0x8A, 0xF2, 0x8A, 0xE8, 0xC0, 0xCC, 0x02, 0x0A, 0xCC, 0xB8, 0x01, 0x02, 0x80, 0x7E, 0x02, 0x0E, 0x75, 0x04, 0xB4, 0x42, 0x8B, 0xF4, 0x8A, 0x56, 0x24, 0xCD, 0x13, 0x61, 0x61, 0x72, 0x0A, 0x40, 0x75, 0x01, 0x42, 0x03, 0x5E, 0x0B, 0x49, 0x75, 0x77, 0xC3, 0x03, 0x18, 0x01, 0x27, 0x0D, 0x0A, 0x49, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x20, 0x64, 0x69, 0x73, 0x6B, 0xFF, 0x0D, 0x0A, 0x44, 0x69, 0x73, 0x6B, 0x20, 0x49, 0x2F, 0x4F, 0x20, 0x65, 0x72, 0x72, 0x6F, 0x72, 0xFF, 0x0D, 0x0A, 0x52, 0x65, 0x70, 0x6C, 0x61, 0x63, 0x65, 0x20, 0x74, 0x68, 0x65, 0x20, 0x64, 0x69, 0x73, 0x6B, 0x2C, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x20, 0x70, 0x72, 0x65, 0x73, 0x73, 0x20, 0x61, 0x6E, 0x79, 0x20, 0x6B, 0x65, 0x79, 0x0D, 0x0A, 0x00, 0x00, 0x49, 0x4F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x53, 0x59, 0x53, 0x4D, 0x53, 0x44, 0x4F, 0x53, 0x20, 0x20, 0x20, 0x53, 0x59, 0x53, 0x7F, 0x01, 0x00, 0x41, 0xBB, 0x00, 0x07, 0x60, 0x66, 0x6A, 0x00, 0xE9, 0x3B, 0xFF, 0x00, 0x00, 0x55, 0xAA, };
static const uint8_t sector544[] = { 0x61, 0x73, 0x64, 0x66, 0x6A, 0x6B, 0x6C, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static const uint8_t sector33[] = { 0x31, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x31, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x86, 0x5E, 0xC1, 0x14, 0xE1, 0x2D, 0xC0, 0x80, 0xE7, 0x64, 0xC1, 0x34, 0xE1, 0x2D, 0xC0, 0x9D, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x2F, 0x00, 0x2D, 0xC0, 0x38, 0xE1, 0x2D, 0xC0, 0x5C, 0xF2, 0x64, 0xC1, 0x4C, 0xE1, 0x2D, 0xC0, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x29, 0x2D, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x73, 0x64, 0xC1, 0xC4, 0xE1, 0x2D, 0xC0, 0x01, 0x00, 0x64, 0xC1, 0x58, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x78, 0xE1, 0x2D, 0xC0, 0x64, 0x29, 0x2D, 0xC0, 0x90, 0x73, 0x64, 0xC1, 0xE0, 0xE1, 0x2D, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x20, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x90, 0xE1, 0x2D, 0xC0, 0x80, 0xE7, 0x64, 0xC1, 0xB0, 0xE1, 0x2D, 0xC0, 0x9C, 0xE1, 0x2D, 0xC0, 0x80, 0xE7, 0x64, 0xC1, 0xBC, 0xE1, 0x2D, 0xC0, 0x9D, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x2F, 0x00, 0x2D, 0xC0, 0xC0, 0xE1, 0x2D, 0xC0, 0x5C, 0xF2, 0x64, 0xC1, 0xD4, 0xE1, 0x2D, 0xC0, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x29, 0x2D, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x73, 0x64, 0xC1, 0x4C, 0xE2, 0x2D, 0xC0, 0x01, 0x00, 0x64, 0xC1, 0x58, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0xE2, 0x2D, 0xC0, 0x64, 0x29, 0x2D, 0xC0, 0x90, 0x73, 0x64, 0xC1, 0x68, 0xE2, 0x2D, 0xC0, 0x01, 0x00, 0x2D, 0xC0, 0x58, 0x00, 0x00, 0x00, 0x02, 0x00, 0x2D, 0xC0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x20, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x18, 0xE2, 0x2D, 0xC0, 0x80, 0xE7, 0x64, 0xC1, 0x38, 0xE2, 0x2D, 0xC0, 0x9D, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3C, 0xE2, 0x2D, 0xC0, 0xF0, 0xF2, 0x64, 0xC1, 0x40, 0xE2, 0x2D, 0xC0, 0xC0, 0xE8, 0x64, 0xC1, 0x64, 0xE2, 0x2D, 0xC0, 0xA0, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x50, 0x15, 0x5A, 0xC1, 0x04, 0x00, 0x00, 0x00, 0x64, 0x29, 0x2D, 0xC0, 0x4C, 0x7E, 0x64, 0xC1, 0x7C, 0xE2, 0x2D, 0xC0, 0x7C, 0xE2, 0x2D, 0xC0, 0x74, 0xE2, 0x2D, 0xC0, 0x78, 0x74, 0x64, 0xC1, 0x84, 0xE2, 0x2D, 0xC0, 0x64, 0x29, 0x2D, 0xC0, 0x84, 0xE2, 0x2D, 0xC0, 0x6C, 0xE6, 0x64, 0xC1, 0xA4, 0xE2, 0x2D, 0xC0, 0x9A, 0x00, 0x01, 0x00, 0xD8, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xCC, 0xE2, 0x2D, 0xC0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x63, 0x60, 0xC1, 0x40, 0xE3, 0x2D, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x77, 0x07, 0x00, 0x00, 0xFE, 0xFF, 0xFE, 0xFF, 0x04, 0x00, 0x00, 0x00, 0x68, 0x3F, 0xC0, 0x48, 0x68, 0x3F, 0xC0, 0x48, 0x68, 0x3F, 0xC0, 0x48, 0xCC, 0xE2, 0x2D, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x2F, 0x64, 0x64, 0x2F, 0x73, 0x4E, 0x75, 0x6C, 0x6C, 0x4F, 0x75, 0x74, 0x31, 0x30, 0x5F, 0x43, 0x54, 0x48, 0x32, 0x41, 0x32, 0x30, 0x30, 0x36, 0x32, 0x31, 0x38, 0x33, };
static const uint8_t sector34[] = { 0x00, 0x00, 0xFF, 0x7F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };
static const uint8_t sector272[] = { 0x31, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x31, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x86, 0x5E, 0xC1, 0x14, 0xE1, 0x2D, 0xC0, 0x80, 0xE7, 0x64, 0xC1, 0x34, 0xE1, 0x2D, 0xC0, 0x9D, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x2F, 0x00, 0x2D, 0xC0, 0x38, 0xE1, 0x2D, 0xC0, 0x5C, 0xF2, 0x64, 0xC1, 0x4C, 0xE1, 0x2D, 0xC0, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x29, 0x2D, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x73, 0x64, 0xC1, 0xC4, 0xE1, 0x2D, 0xC0, 0x01, 0x00, 0x64, 0xC1, 0x58, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x78, 0xE1, 0x2D, 0xC0, 0x64, 0x29, 0x2D, 0xC0, 0x90, 0x73, 0x64, 0xC1, 0xE0, 0xE1, 0x2D, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x20, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x90, 0xE1, 0x2D, 0xC0, 0x80, 0xE7, 0x64, 0xC1, 0xB0, 0xE1, 0x2D, 0xC0, 0x9C, 0xE1, 0x2D, 0xC0, 0x80, 0xE7, 0x64, 0xC1, 0xBC, 0xE1, 0x2D, 0xC0, 0x9D, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x2F, 0x00, 0x2D, 0xC0, 0xC0, 0xE1, 0x2D, 0xC0, 0x5C, 0xF2, 0x64, 0xC1, 0xD4, 0xE1, 0x2D, 0xC0, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x29, 0x2D, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x73, 0x64, 0xC1, 0x4C, 0xE2, 0x2D, 0xC0, 0x01, 0x00, 0x64, 0xC1, 0x58, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0xE2, 0x2D, 0xC0, 0x64, 0x29, 0x2D, 0xC0, 0x90, 0x73, 0x64, 0xC1, 0x68, 0xE2, 0x2D, 0xC0, 0x01, 0x00, 0x2D, 0xC0, 0x58, 0x00, 0x00, 0x00, 0x02, 0x00, 0x2D, 0xC0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x20, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x18, 0xE2, 0x2D, 0xC0, 0x80, 0xE7, 0x64, 0xC1, 0x38, 0xE2, 0x2D, 0xC0, 0x9D, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3C, 0xE2, 0x2D, 0xC0, 0xF0, 0xF2, 0x64, 0xC1, 0x40, 0xE2, 0x2D, 0xC0, 0xC0, 0xE8, 0x64, 0xC1, 0x64, 0xE2, 0x2D, 0xC0, 0xA0, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x50, 0x15, 0x5A, 0xC1, 0x04, 0x00, 0x00, 0x00, 0x64, 0x29, 0x2D, 0xC0, 0x4C, 0x7E, 0x64, 0xC1, 0x7C, 0xE2, 0x2D, 0xC0, 0x7C, 0xE2, 0x2D, 0xC0, 0x74, 0xE2, 0x2D, 0xC0, 0x78, 0x74, 0x64, 0xC1, 0x84, 0xE2, 0x2D, 0xC0, 0x64, 0x29, 0x2D, 0xC0, 0x84, 0xE2, 0x2D, 0xC0, 0x6C, 0xE6, 0x64, 0xC1, 0xA4, 0xE2, 0x2D, 0xC0, 0x9A, 0x00, 0x01, 0x00, 0xD8, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xCC, 0xE2, 0x2D, 0xC0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x63, 0x60, 0xC1, 0x40, 0xE3, 0x2D, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x77, 0x07, 0x00, 0x00, 0xFE, 0xFF, 0xFE, 0xFF, 0x04, 0x00, 0x00, 0x00, 0x68, 0x3F, 0xC0, 0x48, 0x68, 0x3F, 0xC0, 0x48, 0x68, 0x3F, 0xC0, 0x48, 0xCC, 0xE2, 0x2D, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x2F, 0x64, 0x64, 0x2F, 0x73, 0x4E, 0x75, 0x6C, 0x6C, 0x4F, 0x75, 0x74, 0x31, 0x30, 0x5F, 0x43, 0x54, 0x48, 0x32, 0x41, 0x32, 0x30, 0x30, 0x36, 0x32, 0x31, 0x38, 0x33, };
static const uint8_t sector273[] = { 0x00, 0x00, 0xFF, 0x7F, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

static struct EMULATED_DISK disk = {0, NULL};

void DRV050_writeSector(const uint8_t *buff, uint32_t lba, size_t count)
{
    size_t i;
    
    for(i=0; i<count; i++){
        EmulatedDiskSectorWrite(&disk, (uint32_t)(lba + i), (uint8_t*)&buff[SECTOR_SIZE*i], true);
    }
    
    
}

void DRV050_readSector(	uint8_t *buff, uint32_t lba, size_t count)
{
    size_t i;
    
    for(i=0; i<count; i++){
        EmulatedDiskSectorRead(&disk, (uint32_t)(lba + i), (uint8_t*)&buff[SECTOR_SIZE*i]);
    }
    
    
}

void DRV050_initialize(void)
{    
    EmulatedDiskCreate(&disk, SECTOR_SIZE);
    
    DRV050_writeSector((const uint8_t*)sector0, 0, 1);
    DRV050_writeSector((const uint8_t*)sector512, 512, 1);
    DRV050_writeSector((const uint8_t*)sector513, 513, 1);
    DRV050_writeSector((const uint8_t*)sector32, 32, 1);
    DRV050_writeSector((const uint8_t*)sector544, 544, 1);
    DRV050_writeSector((const uint8_t*)sector33, 33, 1);
    DRV050_writeSector((const uint8_t*)sector34, 34, 1);
    DRV050_writeSector((const uint8_t*)sector272, 272, 1);
    DRV050_writeSector((const uint8_t*)sector273, 273, 1);

}

void DRV050_print(void)
{
    EmulatedDiskPrint(&disk);
}

static const char id[] = "DRV050";
const char* DRV050_id(void)
{
    return id;
}

struct EMULATED_DRIVE DRV050 = { &DRV050_initialize, &DRV050_print, &DRV050_id};

