#ifndef __CMOSRates__
#define __CMOSRates__

#include <TObject.h>

class CMOSRates : public TObject {
public:
  int errorflags;
  int crate;
  float rates[16*32];

  ClassDef(CMOSRates, 1);
};

#endif

