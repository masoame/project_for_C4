#pragma once
#include<stdint.h>

#pragma pack(1)

struct zip_header
{
	uint32_t local_file_header_signature = 0x04034b50;
	uint16_t v0, v1, v2, v3, v4;
	uint32_t CRC, after, before;
	uint16_t name, words;
};
#pragma pack() 