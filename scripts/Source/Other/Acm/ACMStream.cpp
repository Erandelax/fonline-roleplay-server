/*
 * ACM decompression engine source code: ACM streaming
 *
 * Copyright (C) 2000 ANX Software
 * E-mail: anxsoftware@avn.mccme.ru
 *
 * Author: Alexander Belyakov (abel@krasu.ru)
 *
 * Adapted for ACMStream.DLL by Valery V. Anisimovsky (samael@avn.mccme.ru)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <memory.h>
#include <stdio.h>

#include <windows.h>

#include "ACMStreamUnpack.h"
// SomeBuff [PackAttr2, SomeSize]

char Table1 [27] =
		{0,  1,  2,   4,  5,  6,   8,  9, 10,
		16, 17, 18,  20, 21, 22,  24, 25, 26,
		32, 33, 34,  36, 37, 38,  40, 41, 42};
//Eng: in base-4 system it is:
//Rus: � ����������� ������� ��� �����:
//		000 001 002  010 011 012  020 021 022
//		100 101 102  110 111 112  120 121 122
//		200 201 202  210 211 212  220 221 222
short Table2 [125] =
		{ 0,  1,  2,  3,  4,   8,  9, 10, 11, 12,  16, 17, 18, 19, 20,  24, 25, 26, 27, 28,  32, 33, 34, 35, 36,
		 64, 65, 66, 67, 68,  72, 73, 74, 75, 76,  80, 81, 82, 83, 84,  88, 89, 90, 91, 92,  96, 97, 98, 99,100,
		128,129,130,131,132, 136,137,138,139,140, 144,145,146,147,148, 152,153,154,155,156, 160,161,162,163,164,
		192,193,194,195,196, 200,201,202,203,204, 208,209,210,211,212, 216,217,218,219,220, 224,225,226,227,228,
		256,257,258,259,260, 264,265,266,267,268, 272,273,274,275,276, 280,281,282,283,284, 288,289,290,291,292};
//Eng: in base-8 system:
//Rus: � ������������ �������:
//		000 001 002 003 004  010 011 012 013 014 ...
//		100 101 102 103 104 ...
//		200 ...
//		... 
unsigned char Table3 [121] =
		{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
		 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A,
		 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A,
		 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A,
		 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A,
		 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,
		 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A,
		 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A,
		 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A,
		 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A,
		 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};

FillerProc Fillers[32] = {
	&CACMUnpacker::ZeroFill,
	&CACMUnpacker::Return0,
	&CACMUnpacker::Return0,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::LinearFill,
	&CACMUnpacker::k1_3bits,
	&CACMUnpacker::k1_2bits,
	&CACMUnpacker::t1_5bits,
	&CACMUnpacker::k2_4bits,
	&CACMUnpacker::k2_3bits,
	&CACMUnpacker::t2_7bits,
	&CACMUnpacker::k3_5bits,
	&CACMUnpacker::k3_4bits,
	&CACMUnpacker::Return0,
	&CACMUnpacker::k4_5bits,
	&CACMUnpacker::k4_4bits,
	&CACMUnpacker::Return0,
	&CACMUnpacker::t3_7bits,
	&CACMUnpacker::Return0,
	&CACMUnpacker::Return0
};

short Amplitude_Buffer[0x10000];
short *Buffer_Middle;
unsigned char fileBuffPtr[ACM_BUFF_SIZE];

void Decode1 (short *decBuff, int *someBuff, int someSize, int blocks);
void Decode2 (int *decBuff, int *someBuff, int someSize, int blocks);

BOOL CACMUnpacker::init_unpacker() {
	Buffer_Middle = &Amplitude_Buffer[0x8000];

	if ((getBits (24) & 0xFFFFFF) != 0x032897) return FALSE;
	if ((getBits (8) & 0xFF) != 1) return FALSE;
	valsToGo = getBits (16) & 0xFFFF;
	valsToGo += (getBits (16) & 0xFFFF) << 16;
	acm_samples = valsToGo;
	acm_size = valsToGo*2;
	acm_channels = getBits (16) & 0xFFFF;
	acm_rate = getBits (16) & 0xFFFF;

	packAttrs = getBits (4) & 0xF;
	packAttrs2 = getBits (12) & 0xFFF;

	someSize = 1 << packAttrs;
	someSize2 = someSize * packAttrs2;

	int decBuf_size = 0;
	if (packAttrs)
		decBuf_size = 3*someSize / 2 - 2;
		
	blocks = 0x800 / someSize - 2;
	if (blocks < 1) blocks = 1;
	totBlSize = blocks * someSize;

	if (decBuf_size) {
		decompBuff = (int*)GlobalAlloc(GPTR, decBuf_size*sizeof(int));
		if (!decompBuff) return FALSE;
	}
		
	someBuff = (int*)GlobalAlloc(GMEM_FIXED, sizeof(int)*someSize2);
	if (!someBuff) return FALSE;
	valCnt = 0;

	return TRUE;
}

unsigned char CACMUnpacker::readNextPortion() {
	availBytes=readBuf(fileBuffPtr,bufferSize);
	if (!availBytes) {
		memset (fileBuffPtr, '\0', bufferSize);
		availBytes = bufferSize;
	}
	buffPos = fileBuffPtr+1;
	availBytes--;
	return *fileBuffPtr;
}
void CACMUnpacker::prepareBits (int bits) {
	while (bits > availBits) {
		int oneByte;
		availBytes--;
		if (availBytes >= 0) {
			oneByte = *buffPos;
			buffPos++;
		} else
			oneByte = readNextPortion();
		nextBits |= (oneByte << availBits);
		availBits += 8;
	}
}
int CACMUnpacker::getBits (int bits) {
	prepareBits (bits);
	int res = nextBits;
	availBits -= bits;
	nextBits >>= bits;
	return res;
}


int CACMUnpacker::createAmplitudeDictionary() {
	int pwr = getBits (4) & 0xF,
		val = getBits (16) & 0xFFFF,
		count = 1 << pwr,
		v = 0;

	for (int i=0; i<count; i++) {
		Buffer_Middle[i] = v;
		v += val;
	}
	v = -val;
	for (i=0; i<count; i++) {
		Buffer_Middle[-i-1] = v;
		v -= val;
	}

	// FillTables(). We have aleady done it, see definitions of Tables

	for (int pass=0; pass<someSize; pass++) {
		int ind = getBits (5) & 0x1F;
		if (! ((this->*Fillers[ind]) (pass, ind)) )
			return 0;
	}
	return 1;
}
void CACMUnpacker::unpackValues() {
	if (!packAttrs) return;
	
	int counter = packAttrs2;
	int* someBuffPtr = someBuff;

	while (counter > 0) {
		int* decBuffPtr = decompBuff;
		int loc_blocks = blocks;
		int loc_someSize = someSize / 2;

		if (loc_blocks > counter)
			loc_blocks = counter;

		loc_blocks *= 2;
		Decode1 ((short*)decBuffPtr, someBuffPtr, loc_someSize, loc_blocks);

		decBuffPtr += loc_someSize;

		for (int i=0; i<loc_blocks; i++)
			someBuffPtr [i*loc_someSize]++;

		loc_someSize /= 2;
		loc_blocks *= 2;

		while (loc_someSize != 0) {
			Decode2 (decBuffPtr, someBuffPtr, loc_someSize, loc_blocks);
			decBuffPtr += loc_someSize*2;

			loc_someSize /= 2;
			loc_blocks *= 2;
		}

		counter -= blocks;
		someBuffPtr += totBlSize;
	}
}
int CACMUnpacker::makeNewValues() {
	if (!createAmplitudeDictionary()) return 0;
	unpackValues();

	values = someBuff;
	valCnt = (someSize2 > valsToGo)? valsToGo: someSize2;
	valsToGo -= valCnt;

	return 1;
}

int CACMUnpacker::readAndDecompress (unsigned short* buff, int count) {
//Eng: takes new values (of type int) and writes them into output buffer (of type word),
//  the values are shifted by packAttrs bits
//Rus: ����� ����� �������� (���� ���) � ���������� � �������� ������ (���� ����),
// ��� ���� �������� ������� �� packAttrs ���
	int res = 0;
//	short fake;
	while (res < count) {
		if (!valCnt) {
			if (valsToGo == 0) break;
			if (!makeNewValues()) break;
		}
//		fake = (*values) >> (packAttrs-1);
//		if (fake < -0x8000) {
//			fake = -0x8000;
//		} else if (fake > 0x7fff) {
//			fake = 0x7fff;
//		}
//		*buff = fake;
		*buff = (*values) >> packAttrs;
		values++;
		buff++;
		res += 2;
		valCnt--;
	}
	return res;
}



// Filling functions:
// int CACMUnpacker::FillerProc (int pass, int ind)
int CACMUnpacker::Return0 (int pass, int ind) {
	return 0;
}
int CACMUnpacker::ZeroFill (int pass, int ind) {
//Eng: used when the whole column #pass is zero-filled
//Rus: ������������, ����� ���� ������� � ������� pass �������� ������

//	the meaning is following:

//	for (int i=0; i<packAttrs2; i++)
//		someBuff [i, pass] = 0;

//	speed-optimized version:
	int *sb_ptr = &someBuff [pass],
		step = someSize,
		i = packAttrs2;
	do {
		*sb_ptr = 0;
		sb_ptr += step;
	} while ((--i) != 0);
	return 1;
}
int CACMUnpacker::LinearFill (int pass, int ind) {
	int mask = (1 << ind) - 1;
	short* lb_ptr = &Buffer_Middle [(-1l) << (ind-1)];

	for (int i=0; i<packAttrs2; i++)
		someBuff [i*someSize + pass] = lb_ptr [getBits (ind) & mask];
	return 1;
}
int CACMUnpacker::k1_3bits (int pass, int ind) {
//Eng: column with number pass is filled with zeros, and also +/-1, zeros are repeated frequently
//Rus: c������ pass �������� ������, � ����� +/- 1, �� ���� ����� ���� ������
// efficiency (bits per value): 3-p0-2.5*p00, p00 - cnt of paired zeros, p0 - cnt of single zeros.
//Eng: it makes sense to use, when the freqnecy of paired zeros (p00) is greater than 2/3
//Rus: ����� ����� ������������, ����� ����������� ������ ����� (p00) ������ 2/3
	for (int i=0; i<packAttrs2; i++) {
		prepareBits (3);
		if ((nextBits & 1) == 0) {
			availBits--;
			nextBits >>= 1;
			someBuff [i*someSize + pass] = 0; if ((++i) == packAttrs2) break;
			someBuff [i*someSize + pass] = 0;
		} else if ((nextBits & 2) == 0) {
			availBits -= 2;
			nextBits >>= 2;
			someBuff [i*someSize + pass] = 0;
		} else {
			someBuff [i*someSize + pass] = (nextBits & 4)? Buffer_Middle[1]: Buffer_Middle[-1];
			availBits -= 3;
			nextBits >>= 3;
		}
	}
	return 1;
}
int CACMUnpacker::k1_2bits (int pass, int ind) {
//Eng: column is filled with zero and +/-1
//Rus: c������ pass �������� ������, � ����� +/- 1
// efficiency: 2-P0. P0 - cnt of any zero (P0 = p0 + p00)
//Eng: use it when P0 > 1/3
//Rus: ����� ����� ������������, ����� ����������� ���� ������ 1/3
	for (int i=0; i<packAttrs2; i++) {
		prepareBits (2);
		if ((nextBits & 1) == 0) {
			availBits--;
			nextBits >>= 1;
			someBuff [i*someSize + pass] = 0;
		} else {
			someBuff [i*someSize + pass] = (nextBits & 2)? Buffer_Middle[1]: Buffer_Middle[-1];
			availBits -= 2;
			nextBits >>= 2;
		}
	}
	return 1;
}
int CACMUnpacker::t1_5bits (int pass, int ind) {
//Eng: all the -1, 0, +1 triplets
//Rus: ��� ���������� ����� -1, 0, +1.
// efficiency: always 5/3 bits per value
// use it when P0 <= 1/3
	for (int i=0; i<packAttrs2; i++) {
		char bits = getBits (5) & 0x1f;
		bits = Table1 [bits];

		someBuff [i*someSize + pass] = Buffer_Middle[-1 + (bits & 3)];
			if ((++i) == packAttrs2) break;
			bits >>= 2;
		someBuff [i*someSize + pass] = Buffer_Middle[-1 + (bits & 3)];
			if ((++i) == packAttrs2) break;
			bits >>= 2;
		someBuff [i*someSize + pass] = Buffer_Middle[-1 + bits];
	}
	return 1;
}
int CACMUnpacker::k2_4bits (int pass, int ind) {
// -2, -1, 0, 1, 2, and repeating zeros
// efficiency: 4-2*p0-3.5*p00, p00 - cnt of paired zeros, p0 - cnt of single zeros.
//Eng: makes sense to use when p00>2/3
//Rus: ����� ����� ������������, ����� ����������� ������ ����� (p00) ������ 2/3
	for (int i=0; i<packAttrs2; i++) {
		prepareBits (4);
		if ((nextBits & 1) == 0) {
			availBits--;
			nextBits >>= 1;
			someBuff [i*someSize + pass] = 0; if ((++i) == packAttrs2) break;
			someBuff [i*someSize + pass] = 0;
		} else if ((nextBits & 2) == 0) {
			availBits -= 2;
			nextBits >>= 2;
			someBuff [i*someSize + pass] = 0;
		} else {
			someBuff [i*someSize + pass] =
				(nextBits & 8)?
					( (nextBits & 4)? Buffer_Middle[2]: Buffer_Middle[1] ):
					( (nextBits & 4)? Buffer_Middle[-1]: Buffer_Middle[-2] );
			availBits -= 4;
			nextBits >>= 4;
		}
	}
	return 1;
}
int CACMUnpacker::k2_3bits (int pass, int ind) {
// -2, -1, 0, 1, 2
// efficiency: 3-2*P0, P0 - cnt of any zero (P0 = p0 + p00)
//Eng: use when P0>1/3
//Rus: ����� ����� ������������, ����� ����������� ���� ������ 1/3
	for (int i=0; i<packAttrs2; i++) {
		prepareBits (3);
		if ((nextBits & 1) == 0) {
			availBits--;
			nextBits >>= 1;
			someBuff [i*someSize + pass] = 0;
		} else {
			someBuff [i*someSize + pass] =
				(nextBits & 4)?
					( (nextBits & 2)? Buffer_Middle[2]: Buffer_Middle[1] ):
					( (nextBits & 2)? Buffer_Middle[-1]: Buffer_Middle[-2] );
			availBits -= 3;
			nextBits >>= 3;
		}
	}
	return 1;
}
int CACMUnpacker::t2_7bits (int pass, int ind) {
//Eng: all the +/-2, +/-1, 0  triplets
// efficiency: always 7/3 bits per value
//Rus: ��� ���������� ����� -2, -1, 0, +1, 2.
// �������������: 7/3 ���� �� �������� - ������
// use it when p0 <= 1/3
	for (int i=0; i<packAttrs2; i++) {
		char bits = getBits (7) & 0x7f;
		short val = Table2 [bits];

		someBuff [i*someSize + pass] = Buffer_Middle[-2 + (val & 7)];
			if ((++i) == packAttrs2) break;
			val >>= 3;
		someBuff [i*someSize + pass] = Buffer_Middle[-2 + (val & 7)];
			if ((++i) == packAttrs2) break;
			val >>= 3;
		someBuff [i*someSize + pass] = Buffer_Middle[-2 + val];
	}
	return 1;
}
int CACMUnpacker::k3_5bits (int pass, int ind) {
// fills with values: -3, -2, -1, 0, 1, 2, 3, and double zeros
// efficiency: 5-3*p0-4.5*p00-p1, p00 - cnt of paired zeros, p0 - cnt of single zeros, p1 - cnt of +/- 1.
// can be used when frequency of paired zeros (p00) is greater than 2/3
	for (int i=0; i<packAttrs2; i++) {
		prepareBits (5);
		if ((nextBits & 1) == 0) {
			availBits--;
			nextBits >>= 1;
			someBuff [i*someSize + pass] = 0; if ((++i) == packAttrs2) break;
			someBuff [i*someSize + pass] = 0;
		} else if ((nextBits & 2) == 0) {
			availBits -= 2;
			nextBits >>= 2;
			someBuff [i*someSize + pass] = 0;
		} else if ((nextBits & 4) == 0) {
			someBuff [i*someSize + pass] = (nextBits & 8)? Buffer_Middle[1]: Buffer_Middle[-1];
			availBits -= 4;
			nextBits >>= 4;
		} else {
			availBits -= 5;
			int val = (nextBits & 0x18) >> 3;
			nextBits >>= 5;
			if (val >= 2) val += 3;
			someBuff [i*someSize + pass] = Buffer_Middle[-3 + val];
		}
	}
	return 1;
}
int CACMUnpacker::k3_4bits (int pass, int ind) {
// fills with values: -3, -2, -1, 0, 1, 2, 3.
// efficiency: 4-3*P0-p1, P0 - cnt of all zeros (P0 = p0 + p00), p1 - cnt of +/- 1.
	for (int i=0; i<packAttrs2; i++) {
		prepareBits (4);
		if ((nextBits & 1) == 0) {
			availBits--;
			nextBits >>= 1;
			someBuff [i*someSize + pass] = 0;
		} else if ((nextBits & 2) == 0) {
			availBits -= 3;
			someBuff [i*someSize + pass] = (nextBits & 4)? Buffer_Middle[1]: Buffer_Middle[-1];
			nextBits >>= 3;
		} else {
			int val = (nextBits &0xC) >> 2;
			availBits -= 4;
			nextBits >>= 4;
			if (val >= 2) val += 3;
			someBuff [i*someSize + pass] = Buffer_Middle[-3 + val];
		}
	}
	return 1;
}
int CACMUnpacker::k4_5bits (int pass, int ind) {
// fills with values: +/-4, +/-3, +/-2, +/-1, 0, and double zeros
// efficiency: 5-3*p0-4.5*p00, p00 - cnt of paired zeros, p0 - cnt of single zeros.
//Eng: makes sense to use when p00>2/3
//Rus: ����� ����� ������������, ����� ����������� ������ ����� (p00) ������ 2/3
	for (int i=0; i<packAttrs2; i++) {
		prepareBits (5);
		if ((nextBits & 1) == 0) {
			availBits--;
			nextBits >>= 1;
			someBuff [i*someSize + pass] = 0; if ((++i) == packAttrs2) break;
			someBuff [i*someSize + pass] = 0;
		} else if ((nextBits & 2) == 0) {
			availBits -= 2;
			nextBits >>= 2;
			someBuff [i*someSize + pass] = 0;
		} else {
			int val = (nextBits &0x1C) >> 2;
			if (val >= 4) val++;
			someBuff [i*someSize + pass] = Buffer_Middle[-4 + val];
			availBits -= 5;
			nextBits >>= 5;
		}
	}
	return 1;
}
int CACMUnpacker::k4_4bits (int pass, int ind) {
// fills with values: +/-4, +/-3, +/-2, +/-1, 0, and double zeros
// efficiency: 4-3*P0, P0 - cnt of all zeros (both single and paired).
	for (int i=0; i<packAttrs2; i++) {
		prepareBits (4);
		if ((nextBits & 1) == 0) {
			availBits--;
			nextBits >>= 1;
			someBuff [i*someSize + pass] = 0;
		} else {
			int val = (nextBits &0xE) >> 1;
			availBits -= 4;
			nextBits >>= 4;
			if (val >= 4) val++;
			someBuff [i*someSize + pass] = Buffer_Middle[-4 + val];
		}
	}
	return 1;
}
int CACMUnpacker::t3_7bits (int pass, int ind) {
//Eng: all the pairs of values from -5 to +5
// efficiency: 7/2 bits per value
//Rus: ��� ���������� ��� �� -5 �� +5
// �������������: 7/2 ���� �� �������� - ������
	for (int i=0; i<packAttrs2; i++) {
		char bits = getBits (7) & 0x7f;
		unsigned char val = Table3 [bits];

		someBuff [i*someSize + pass] = Buffer_Middle[-5 + (val & 0xF)];
			if ((++i) == packAttrs2) break;
			val >>= 4;
		someBuff [i*someSize + pass] = Buffer_Middle[-5 + val];
	}
	return 1;
}