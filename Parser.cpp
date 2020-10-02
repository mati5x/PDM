#pragma once
#include <cstdint>
#include <cinttypes>
#include <cfloat>
#include <climits>
#include <cstddef>
#include <string>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <cstring>

using namespace std;

#define NOT_VALID  -1

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || defined(_M_IX86))
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif

//=============================================================================================================================================================================
// Byte swap
//=============================================================================================================================================================================
#if defined(_MSC_VER)
static inline uint16_t xSwapBytes16(uint16_t Value) { return _byteswap_ushort(Value); }
static inline  int16_t xSwapBytes16(int16_t Value) { return _byteswap_ushort(Value); }
static inline uint32_t xSwapBytes32(uint32_t Value) { return _byteswap_ulong(Value); }
static inline  int32_t xSwapBytes32(int32_t Value) { return _byteswap_ulong(Value); }
static inline uint64_t xSwapBytes64(uint64_t Value) { return _byteswap_uint64(Value); }
static inline  int64_t xSwapBytes64(int64_t Value) { return _byteswap_uint64(Value); }
#elif defined (__GNUC__)
static inline uint16 xSwapBytes16(uint16 Value) { return __builtin_bswap16(Value); }
static inline  int16 xSwapBytes16(int16 Value) { return __builtin_bswap16(Value); }
static inline uint32 xSwapBytes32(uint32 Value) { return __builtin_bswap32(Value); }
static inline  int32 xSwapBytes32(int32 Value) { return __builtin_bswap32(Value); }
static inline uint64 xSwapBytes64(uint64 Value) { return __builtin_bswap64(Value); }
static inline  int64 xSwapBytes64(int64 Value) { return __builtin_bswap64(Value); }
#else
#error Unrecognized compiler
#endif

//=============================================================================================================================================================================

class xTS
{
public:
	static constexpr uint32_t TS_PacketLength = 188;
	static constexpr uint32_t TS_HeaderLength = 4;

	static constexpr uint32_t PES_HeaderLength = 6;

	static constexpr uint32_t BaseClockFrequency_Hz = 90000; //Hz
	static constexpr uint32_t ExtendedClockFrequency_Hz = 27000000; //Hz
	static constexpr uint32_t BaseClockFrequency_kHz = 90; //kHz
	static constexpr uint32_t ExtendedClockFrequency_kHz = 27000; //kHz
	static constexpr uint32_t BaseToExtendedClockMultiplier = 300;
};

//=============================================================================================================================================================================
// Klasa xTS_PacketHeader
//=============================================================================================================================================================================

class xTS_PacketHeader
{
protected:
	uint16_t SB;
	uint16_t E;
	uint16_t S;
	uint16_t T;
	uint16_t PID;
	uint16_t TSC;
	uint16_t AFC;
	uint16_t CC;

public:
	void     Reset();
	int32_t  Parse(const uint8_t* Input);
	void     Print() const;

public:
	uint16_t get_SB() const { return SB; }
	uint16_t get_E() const { return E; }
	uint16_t get_S() const { return S; }
	uint16_t get_T() const { return T; }
	uint16_t get_PID() const { return PID; }
	uint16_t get_TSC() const { return TSC; }
	uint16_t get_AFC() const { return AFC; }
	uint16_t get_CC() const { return CC; }

	void set_SB(uint16_t x) { SB = x; }
	void set_E(uint16_t x) { E = x; }
	void set_S(uint16_t x) { S = x; }
	void set_T(uint16_t x) { T = x; }
	void set_PID(uint16_t x) { PID = x; }
	void set_TSC(uint16_t x) { TSC = x; }
	void set_AFC(uint16_t x) { AFC = x; }
	void set_CC(uint16_t x) { CC = x; }

public:
	bool     hasAdaptationField() const {
		if (get_AFC() == 2 || get_AFC() == 3)
			return true;
		else
			return false;
	}

	bool     hasPayload() const { 
		if (get_AFC() != 2)
			return true;
		else
			return false;
	}
};

//=============================================================================================================================================================================
// Klasa xTS_AdaptationField
//=============================================================================================================================================================================

class xTS_AdaptationField
{
protected:
	uint16_t AFL;
	uint16_t DC;
	uint16_t RA;
	uint16_t SP;
	uint16_t PR;
	uint16_t OR;
	uint16_t SPF;
	uint16_t TP;
	uint16_t EX;

public:
	void    Reset();
	int32_t Parse(const uint8_t* Input, uint8_t AdaptationFieldControl);
	void    Print() const;

public:
	void set_AFL(uint16_t x) { AFL = x; }
	void set_DC(uint16_t x) { DC = x; }
	void set_RA(uint16_t x) { RA = x; }
	void set_SP(uint16_t x) { SP = x; }
	void set_PR(uint16_t x) { PR = x; }
	void set_OR(uint16_t x) { OR = x; }
	void set_SPF(uint16_t x) { SPF = x; }
	void set_TP(uint16_t x) { TP = x; }
	void set_EX(uint16_t x) { EX = x; }

	uint16_t get_AFL() const { return AFL; }
	uint16_t get_DC() const { return DC; }
	uint16_t get_RA() const { return RA; }
	uint16_t get_SP() const { return SP; }
	uint16_t get_PR() const { return PR; }
	uint16_t get_OR() const { return OR; }
	uint16_t get_SPF() const { return SPF; }
	uint16_t get_TP() const { return TP; }
	uint16_t get_EX() const { return EX; }

	uint32_t    getNumBytes() const { return get_AFL() + 1; }
};

//=============================================================================================================================================================================
// Klasa xPES_PacketHeader
//=============================================================================================================================================================================

class xPES_PacketHeader
{
public:
	enum eStreamId : uint8_t
	{
		eStreamId_program_stream_map = 0xBC,
		eStreamId_padding_stream = 0xBE,
		eStreamId_private_stream_2 = 0xBF,
		eStreamId_ECM = 0xF0,
		eStreamId_EMM = 0xF1,
		eStreamId_program_stream_directory = 0xFF,
		eStreamId_DSMCC_stream = 0xF2,
		eStreamId_ITUT_H222_1_type_E = 0xF8,
	};

protected:
	uint32_t    m_PacketStartCodePrefix;
	uint16_t    m_StreamId;
	uint16_t    m_PacketLength;
	uint8_t     PES_HDL;
	bool        extension;

public:
	void        Reset();
	int32_t     Parse(const uint8_t* Input);
	void        Print() const;
	void        Print_headerL() const;

public:
	uint32_t    getPacketStartCodePrefix() const { return m_PacketStartCodePrefix; }
	uint16_t    getStreamId() const { return m_StreamId; }
	uint16_t    getPacketLength() const { return m_PacketLength; }
	uint8_t     getPES_HDL() const { return PES_HDL; }
	bool        getExtension() const { return extension; }

};

//=============================================================================================================================================================================
// Klasa xPES_Assembler
//=============================================================================================================================================================================

class xPES_Assembler
{
public:
	enum class eResult : int32_t
	{
		UnexpectedPID = 1,
		StreamPackedLost,
		AssemblingStarted,
		AssemblingContinue,
		AssemblingFinished,
	};

protected:

	int32_t             m_PID = 0;

	uint8_t*			m_Buffer;
	uint32_t            m_BufferSize;
	uint32_t            m_DataOffset;
	uint32_t            PES_Len;

	int16_t             counter;
	int16_t             check;
	int16_t             m_NextContinuityCounter;
	int16_t             m_LastContinuityCounter;
	bool                m_Started;
	xPES_PacketHeader   m_PESH;

public:
	xPES_Assembler()
	{
		m_Buffer = static_cast<uint8_t*>(malloc(188 * 100));
		m_BufferSize = 0;
		m_DataOffset = 0;
		PES_Len = 0;
		counter = 0;
		check = 0;
		m_NextContinuityCounter = 0;
		m_LastContinuityCounter = 0;
		m_Started = true;

		FILE* fileToWrite = fopen("out.mp2", "wb");
	};
	~xPES_Assembler() {};

	eResult                 AbsorbPacket(const uint8_t* TransportStreamPacket, const xTS_PacketHeader* PacketHeader, const xTS_AdaptationField* AdaptationField);

	void                    PrintPESH() const { m_PESH.Print(); }
	void                    PrintPES_HeaderLength() const { m_PESH.Print_headerL(); }
	xPES_PacketHeader       get_m_PESH() { return m_PESH; }
	uint8_t*				getPacket() { return m_Buffer; }
	int32_t                 getNumberPacketBytes() const { return m_DataOffset; }
	int32_t                 getPES_L() const { return PES_Len; }

	void                    set_Offset(int32_t offset) { m_DataOffset = offset; }

protected:
	void xBufferReset() { set_Offset(0); };
	void xBufferAppend(const uint8_t* Data, int32_t Size);

};

//=============================================================================================================================================================================
// xTS_PacketHeader - metody
//=============================================================================================================================================================================

int32_t xTS_PacketHeader::Parse(const uint8_t* Input) {
	unsigned char tmp;

	int SB = Input[0];
	set_SB(SB);

	tmp = Input[1];
	tmp >>= 7;
	int E = tmp & 0x1;
	set_E(E);

	tmp = Input[1];
	tmp >>= 6;
	int S = tmp & 0x1;
	set_S(S);

	tmp = Input[1];
	tmp >>= 5;
	int T = tmp & 0x1;
	set_T(T);

	tmp = Input[1];
	int PID_H = tmp & 0x1F;
	int PID_L = Input[2];
	int PID = (PID_H * (0xFF + 1)) + PID_L;
	set_PID(PID);

	tmp = Input[3];
	tmp >>= 6;
	int TSC = tmp & 0x3;
	set_TSC(TSC);

	tmp = Input[3];
	tmp >>= 4;
	int AFC = tmp & 0x3;
	set_AFC(AFC);

	tmp = Input[3];
	int CC = tmp & 0xF;
	set_CC(CC);

	return 0;
};

void xTS_PacketHeader::Reset() {

	xTS_PacketHeader::set_SB(0x0);
	xTS_PacketHeader::set_E(0x0);
	xTS_PacketHeader::set_S(0x0);
	xTS_PacketHeader::set_T(0x0);
	xTS_PacketHeader::set_PID(0x0);
	xTS_PacketHeader::set_TSC(0x0);
	xTS_PacketHeader::set_AFC(0x0);
	xTS_PacketHeader::set_CC(0x0);

};

void xTS_PacketHeader::Print() const {

	cout << "TS: " << "SB= " << get_SB()
		<< " E= " << get_E()
		<< " S= " << get_S()
		<< " T= " << get_T()
		<< " PID= " << get_PID()
		<< " TSC= " << get_TSC()
		<< " AFC= " << get_AFC()
		<< " CC= " << get_CC();

};

//=============================================================================================================================================================================
// xTS_AdaptationField - metody
//=============================================================================================================================================================================

int32_t xTS_AdaptationField::Parse(const uint8_t* Input, uint8_t AdaptationFieldControl) {

	unsigned char tmp;
	if (AdaptationFieldControl == 2)
	{
		int afl = 0b10110111;
		set_AFL(afl);
	}
	else
	{
		tmp = Input[4];
		int afl = tmp & 0xFF;
		set_AFL(afl);
	}

	tmp = Input[5];
	tmp >>= 7;
	int dc = tmp & 0x1;
	set_DC(dc);

	tmp = Input[5];
	tmp >>= 6;
	int ra = tmp & 0x1;
	set_RA(ra);

	tmp = Input[5];
	tmp >>= 5;
	int sp = tmp & 0x1;
	set_SP(sp);

	tmp = Input[5];
	tmp >>= 4;
	int pr = tmp & 0x1;
	set_PR(pr);

	tmp = Input[5];
	tmp >>= 3;
	int or_ = tmp & 0x1;
	set_OR(or_);
	tmp = Input[5];
	tmp >>= 2;
	int spf = tmp & 0x1;
	set_SPF(spf);

	tmp = Input[5];
	tmp >>= 1;
	int tp = tmp & 0x1;
	set_TP(tp);

	tmp = Input[5];
	int ex = tmp & 0x1;
	set_EX(ex);

	return 0;
}

void xTS_AdaptationField::Print() const {

	cout << "  AF: " << "L= "
		<< get_AFL() << " DC= "
		<< get_DC() << " RA= "
		<< get_RA() << " SP= "
		<< get_SP() << " PR= "
		<< get_PR() << " OR= "
		<< get_OR() << " SP= "
		<< get_SPF() << " TP= "
		<< get_TP() << " EX="
		<< get_EX();

};

void xTS_AdaptationField::Reset() {

	xTS_AdaptationField::set_AFL(0x0);
	xTS_AdaptationField::set_DC(0x0);
	xTS_AdaptationField::set_RA(0x0);
	xTS_AdaptationField::set_SP(0x0);
	xTS_AdaptationField::set_PR(0x0);
	xTS_AdaptationField::set_OR(0x0);
	xTS_AdaptationField::set_SP(0x0);
	xTS_AdaptationField::set_TP(0x0);
	xTS_AdaptationField::set_EX(0x0);

};

//=============================================================================================================================================================================
// xPES_Assembler - metody
//=============================================================================================================================================================================

xPES_Assembler::eResult xPES_Assembler::AbsorbPacket(const uint8_t* TransportStreamPacket, const xTS_PacketHeader* PacketHeader, const xTS_AdaptationField* AdaptationField) {

	eResult wynik = xPES_Assembler::eResult::StreamPackedLost;
	const uint8_t* dane;
	int32_t rozmiar;

	if (PacketHeader->hasAdaptationField() == true)
	{
		dane = TransportStreamPacket + (4 + AdaptationField->getNumBytes());
		rozmiar = 188 - (4 + AdaptationField->getNumBytes());
	}
	else
	{
		dane = TransportStreamPacket + 4;
		rozmiar = 184;
	}

	if (PacketHeader->hasPayload() && PacketHeader->get_S() == 1 && PacketHeader->get_PID() == 136)
	{
		wynik = xPES_Assembler::eResult::AssemblingStarted;
		m_PESH.Parse(dane);
		m_NextContinuityCounter = PacketHeader->get_CC() + 1;
		int16_t CC_check = ((m_PESH.getPacketLength() - rozmiar) / 184) + 1;
		xPES_Assembler::xBufferAppend(dane, rozmiar);

		check = CC_check;
		counter = 1;
		m_LastContinuityCounter = static_cast<int16_t>((PacketHeader->get_CC() + CC_check) % 16);
	}
	if (PacketHeader->get_CC() == m_LastContinuityCounter && PacketHeader->get_PID() == 136 && check == counter)
	{
		wynik = xPES_Assembler::eResult::AssemblingFinished;
		xPES_Assembler::xBufferAppend(dane, rozmiar);
		m_NextContinuityCounter = 0;
	}
	if (PacketHeader->get_S() == 0 && PacketHeader->get_CC() == m_NextContinuityCounter && PacketHeader->get_PID() == 136 && wynik != xPES_Assembler::eResult::AssemblingFinished)
	{
		wynik = xPES_Assembler::eResult::AssemblingContinue;
		xPES_Assembler::xBufferAppend(dane, rozmiar);

		m_NextContinuityCounter++;
		if (m_NextContinuityCounter == 16)
			m_NextContinuityCounter = 0;
		counter++;
	}
	else if (PacketHeader->get_CC() != m_NextContinuityCounter && wynik != xPES_Assembler::eResult::AssemblingStarted && wynik != xPES_Assembler::eResult::AssemblingFinished) {
		wynik = xPES_Assembler::eResult::StreamPackedLost;
	}

	if (PacketHeader->get_S() == 0 && (PacketHeader->get_AFC() == 3))
	{
		FILE* fileToWrite = fopen("out.mp2", "ab");
		fseek(fileToWrite, 0, SEEK_END);
		int written = fwrite(getPacket(), 1, getNumberPacketBytes(), fileToWrite);
		fclose(fileToWrite);
		PES_Len = getNumberPacketBytes();
		xBufferReset();
	}
	return wynik;
}

void xPES_Assembler::xBufferAppend(const uint8_t* Data, int32_t Size)
{
	uint8_t* bufor = getPacket();
	int32_t offset = getNumberPacketBytes();
	for (int i = 0; i < Size; i++)
	{
		bufor[offset + i] = Data[i];
	}
	offset += Size;
	set_Offset(offset);
}

//=============================================================================================================================================================================
// xPES_PacketHeader - metody
//=============================================================================================================================================================================

int32_t xPES_PacketHeader::Parse(const uint8_t* Input)
{
	unsigned char tmp;

	tmp = Input[0];
	int PSCP_H = tmp & 0xFF;
	tmp = Input[1];
	int PSCP_M = tmp & 0xFF;
	int PSCP_L = Input[2] & 0x1;
	int PSCP = ((PSCP_H * (0xFFFF + 1)) + (PSCP_M * (0xFF + 1)) + PSCP_L);
	m_PacketStartCodePrefix = PSCP;

	tmp = Input[3];
	int SID = tmp & 0xFF;
	m_StreamId = SID;

	tmp = Input[4];
	int PL_H = tmp & 0xFF;
	int PL_L = Input[5] & 0xFF;
	int PL = (PL_H * (0xFF + 1) + PL_L);
	m_PacketLength = PL;

	if (m_StreamId != eStreamId_program_stream_map
		&& m_StreamId != eStreamId_padding_stream
		&& m_StreamId != eStreamId_private_stream_2
		&& m_StreamId != eStreamId_ECM
		&& m_StreamId != eStreamId_EMM
		&& m_StreamId != eStreamId_program_stream_directory
		&& m_StreamId != eStreamId_DSMCC_stream
		&& m_StreamId != eStreamId_ITUT_H222_1_type_E)
	{
		tmp = Input[6];
		tmp >>= 6;
		int constant = tmp & 0x3;

		tmp = Input[6];
		tmp >>= 4;
		int PES_SC = tmp & 0x3;

		tmp = Input[6];
		tmp >>= 3;
		int PES_priority = tmp & 0x1;

		tmp = Input[6];
		tmp >>= 2;
		int data_alignment_indicator = tmp & 0x1;

		tmp = Input[6];
		tmp >>= 1;
		int copyright = tmp & 0x1;

		int originals_or_copy = Input[6] & 0x1;

		tmp = Input[7];
		tmp >>= 6;
		int PTS_DTS_flags = tmp & 0x3;

		tmp = Input[7];
		tmp >>= 5;
		int ESCR_flag = tmp & 0x1;

		tmp = Input[7];
		tmp >>= 4;
		int ES_rate_flag = tmp & 0x1;

		tmp = Input[7];
		tmp >>= 3;
		int DSM_trick_mode_flag = tmp & 0x1;

		tmp = Input[7];
		tmp >>= 2;
		int additional_copy_info_flag = tmp & 0x1;

		tmp = Input[7];
		tmp >>= 1;
		int PES_CRC_flag = tmp & 0x1;

		int PES_extension_flag = Input[7] & 0x1;

		int PES_header_data_length = Input[8] & 0xFF;
		PES_HDL = PES_header_data_length;
		extension = true;
	}
	return 0;
}

void xPES_PacketHeader::Reset()
{
	m_PacketStartCodePrefix = 0;
	m_StreamId = 0;
	m_PacketLength = 0;
}

void xPES_PacketHeader::Print() const {
	cout << " PES: PSCP=" << getPacketStartCodePrefix() << " SID=" << m_StreamId << " L=" << m_PacketLength;
}

void xPES_PacketHeader::Print_headerL() const {
	if (extension)
	{
		cout << " HeadLength=" << (PES_HDL + 9);
	}
	else
	{
		cout << " HeadLength=" << 6;
	}
}

//=============================================================================================================================================================================
// Main
//=============================================================================================================================================================================

int main(int argc, char* argv[], char* envp[])
{

	xTS_PacketHeader      TS_PacketHeader;
	xTS_AdaptationField   TS_AdaptationField;

	const int32_t         numberOfElements = 188;
	uint8_t               TS_PacketBuffer[numberOfElements];
	xPES_Assembler        PES_Assembler;

	int32_t TS_PacketId = 0;

	FILE* file = fopen("example_new.ts", "rb");
	if (file != NULL) {
		cout << "Wczytano plik" << endl;
	}

	while (!feof(file))
	{
		int ile = fread(TS_PacketBuffer, sizeof(uint8_t), sizeof(uint8_t) * 188, file);
		if (ile != numberOfElements)
		{
			cout << endl << "Zakonczono" << endl;
			break;
		}

		TS_PacketHeader.Reset();
		TS_PacketHeader.Parse(TS_PacketBuffer);
		TS_AdaptationField.Reset();

		if (TS_PacketHeader.get_SB() == 'G' && TS_PacketHeader.get_PID() == 136)
		{
			if (TS_PacketHeader.hasAdaptationField())
			{
				TS_AdaptationField.Parse(TS_PacketBuffer, TS_PacketHeader.get_AFC());
			}

			printf("%010d ", TS_PacketId);

			TS_PacketHeader.Print();
			if (TS_PacketHeader.hasAdaptationField())
				TS_AdaptationField.Print();

			xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(TS_PacketBuffer, &TS_PacketHeader, &TS_AdaptationField);
			switch (Result)
			{
			case xPES_Assembler::eResult::StreamPackedLost:
				cout << " Packet Lost";
				break;
			case xPES_Assembler::eResult::AssemblingStarted:
				cout << " Started: ";
				PES_Assembler.PrintPESH();
				break;
			case xPES_Assembler::eResult::AssemblingContinue:
				cout << " Continue ";
				break;
			case xPES_Assembler::eResult::AssemblingFinished:
				cout << " Finished: " << "PES: Len= " << PES_Assembler.getPES_L();
				PES_Assembler.PrintPES_HeaderLength();
				if (PES_Assembler.get_m_PESH().getExtension())
				{
					cout << " DataLength=" << PES_Assembler.getPES_L() - (PES_Assembler.get_m_PESH().getPES_HDL() + 9) << endl;;
				}
				else
				{
					cout << " DataLength=" << PES_Assembler.getPES_L() - 6 << endl;;
				}
			}
			printf("\n");

		}
		TS_PacketId++;
	}
}