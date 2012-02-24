#ifndef __RAT_DS_PackedEvent__
#define __RAT_DS_PackedEvent__

#include <vector>
#include <string>
#include <TObject.h>
#include <iostream>
#include <cstring>

namespace SNOT {
using namespace std;

class PMTRecord_cccc {
	public:
		PMTRecord_cccc(): nhit_cccc(0),
			Crate(NULL), Card(NULL), Channel(NULL), Cell(NULL), CGT16(NULL),
			CGT24(NULL), ES16(NULL), Missed(NULL), NC(NULL), LGI(NULL) {}
		virtual ~PMTRecord_cccc() {
			if (Crate) delete [] Crate;
			if (Card) delete [] Card;
			if (Channel) delete [] Channel;
			if (Cell) delete [] Cell;
			if (CGT16) delete [] CGT16;
			if (CGT24) delete [] CGT24;
			if (ES16) delete [] ES16;
			if (Missed) delete [] Missed;
			if (NC) delete [] NC;
			if (LGI) delete [] LGI;
		}

		Int_t nhit_cccc;
		UChar_t* Crate;		//[nhit_cccc]
		UChar_t* Card;		//[nhit_cccc]
		UChar_t* Channel;	//[nhit_cccc]
		UChar_t* Cell;		//[nhit_cccc]
		Bool_t* CGT16;		//[nhit_cccc];
		Bool_t* CGT24;		//[nhit_cccc];
		Bool_t* ES16;		//[nhit_cccc];
		Bool_t* Missed;		//[nhit_cccc];
		Bool_t* NC;		//[nhit_cccc];
		Bool_t* LGI;		//[nhit_cccc];

		ClassDef(PMTRecord_cccc, 2)
};

class PMTRecord_qhl {
	public:
		PMTRecord_qhl(): nhit_qhl(0), QHL(NULL) {}
		virtual ~PMTRecord_qhl() {
			if (QHL) delete [] QHL;
		}

		Int_t nhit_qhl;
		UShort_t* QHL;		//[nhit_qhl]

		ClassDef(PMTRecord_qhl, 2)
};

class PMTRecord_qhs {
	public:
		PMTRecord_qhs(): nhit_qhs(0), QHS(NULL) {}
		virtual ~PMTRecord_qhs() {
			if (QHS) delete [] QHS;
		}

		Int_t nhit_qhs;
		UShort_t* QHS;		//[nhit_qhs]

		ClassDef(PMTRecord_qhs, 2)
};

class PMTRecord_qlx {
	public:
		PMTRecord_qlx(): nhit_qlx(0), QLX(NULL) {}
		virtual ~PMTRecord_qlx() {
			if (QLX) delete [] QLX;
		}

		Int_t nhit_qlx;
		UShort_t* QLX;		//[nhit_qlx]

		ClassDef(PMTRecord_qlx, 2)
};

class PMTRecord_tac {
	public:
		PMTRecord_tac(): nhit_tac(0), TAC(NULL) {}
		virtual ~PMTRecord_tac() {
			if (TAC) delete [] TAC;
		}

		Int_t nhit_tac;
		UShort_t* TAC;		//[nhit_tac]

		ClassDef(PMTRecord_tac, 2)
};

class CaenTrace {
	public:
		CaenTrace(): TraceLength(0), Waveform(NULL) {}
		virtual ~CaenTrace() { if (Waveform) delete [] Waveform; }
		virtual CaenTrace &operator=(const CaenTrace &rhs) { CopyObj(rhs); return *this; }

		Int_t TraceLength;
		UShort_t* Waveform; //[TraceLength]
		//baseline, risetime, ...

		ClassDef(CaenTrace, 2)

		virtual void CleanUp() {
			TraceLength = 0;
			if (Waveform) delete [] Waveform; Waveform = NULL;
		}

	protected:
		virtual void CopyObj(const CaenTrace &rhs) {
			TraceLength = rhs.TraceLength;
			if (Waveform) delete [] Waveform; Waveform = NULL;
			if (rhs.Waveform) {
				Waveform = new UShort_t[TraceLength];
				memcpy(Waveform, rhs.Waveform, TraceLength * sizeof(UShort_t));
			}
		}
};

class CaenHeader: public TObject {
	public:
		CaenHeader(): TObject(),
			ChannelMask(0), Pattern(0), EventCount(0), Clock(0) {}
		CaenHeader(const CaenHeader &rhs): TObject(),
			ChannelMask(rhs.ChannelMask), Pattern(rhs.Pattern),
			EventCount(rhs.EventCount), Clock(rhs.Clock) {}
		virtual ~CaenHeader() {}
		virtual CaenHeader &operator=(const CaenHeader &rhs) {CopyObj(rhs); return *this;}

		UInt_t ChannelMask;
		UInt_t Pattern;
		UInt_t EventCount;
		UInt_t Clock;

		ClassDef(CaenHeader, 2)

		virtual void CleanUp() {
			ChannelMask = 0;
			Pattern = 0;
			EventCount = 0;
			Clock = 0;
		}

	protected:
		virtual void CopyObj(const CaenHeader &rhs) {
			ChannelMask = rhs.ChannelMask;
			Pattern = rhs.Pattern;
			EventCount = rhs.EventCount;
			Clock = rhs.Clock;
		}
};

class MTCRecord : public TObject { 
	public:
		MTCRecord(): TObject(),
			RunID(0), SubRunID(0), NHits(0), Cnt10Mhz(0), Cnt50Mhz(0), GTId(0),
			GTMask(0), MissTrg(false) {}
		virtual ~MTCRecord() {}
		virtual MTCRecord &operator=(const MTCRecord &rhs) { CopyObj(rhs); return *this; }

		UInt_t RunID;
		UInt_t SubRunID;
		UInt_t NHits;
		ULong64_t Cnt10Mhz;
		ULong64_t Cnt50Mhz;
		UInt_t GTId;
		UInt_t GTMask;
		Bool_t MissTrg;
 
		ClassDef(MTCRecord, 2)

		virtual void CleanUp() {
			RunID = 0;
			SubRunID = 0;
			NHits = 0;
			Cnt10Mhz = 0;
			Cnt50Mhz = 0;
			GTId = 0;
			GTMask = 0;
			MissTrg = 0;
		}
	protected:
		virtual void CopyObj(const MTCRecord &rhs) {
			RunID = rhs.RunID;
			SubRunID = rhs.SubRunID;
			NHits = rhs.NHits;
			Cnt10Mhz = rhs.Cnt10Mhz;
			Cnt50Mhz = rhs.Cnt50Mhz;
			GTId = rhs.GTId;
			GTMask = rhs.GTMask;
			MissTrg = rhs.MissTrg;
		}
};

struct pack {
	MTCRecord mtc;
	PMTRecord_cccc pmt_cccc;
	PMTRecord_qhl pmt_qhl;
	PMTRecord_qhs pmt_qhs;
	PMTRecord_qlx pmt_qlx;
	PMTRecord_tac pmt_tac;
	CaenHeader caen_header;
	CaenTrace caen_trace0;
	CaenTrace caen_trace1;
	CaenTrace caen_trace2;
	CaenTrace caen_trace3;
	CaenTrace caen_trace4;
	CaenTrace caen_trace5;
	CaenTrace caen_trace6;
	CaenTrace caen_trace7;
};

class CMOSRate: public TObject {
	public:
		CMOSRate(): TObject() {}
		virtual ~CMOSRate() {}

 		int crate;
		unsigned int slotmask;
		unsigned int channelmask[16];
		unsigned int delay;
		int errorflags;
		float rates[8*32];
		ClassDef(CMOSRate, 1);
};


} // namespace SNOT
#endif

#ifdef __MAKECINT__
	#pragma link C++ nestedclass;
	#pragma link C++ class SNOT::PMTRecord_cccc+;
	#pragma link C++ class SNOT::PMTRecord_qhl+;
	#pragma link C++ class SNOT::PMTRecord_qhs+;
	#pragma link C++ class SNOT::PMTRecord_qlx+;
	#pragma link C++ class SNOT::PMTRecord_tac+;
	#pragma link C++ class SNOT::CaenTrace+;
	#pragma link C++ class vector<SNOT::CaenTrace>;
	#pragma link C++ class SNOT::CaenHeader+;
	#pragma link C++ class vector<SNOT::CaenHeader>;
	#pragma link C++ class SNOT::MTCRecord+;
	#pragma link C++ class vector<SNOT::MTCRecord>;
	#pragma link C++ struct SNOT::pack;
	#pragma link C++ struct vector<SNOT::pack>;
	#pragma link C++ class SNOT::CMOSRate;
	#pragma link C++ class vector<SNOT::CMOSRate>;
#endif //__MAKECINT__

