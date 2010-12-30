#ifndef GSMMASTER
#define GSMMASTER
#include "gsmSMS.h"
#include "gsmGPRS.h"

class gsmMASTER : public gsmSMS,  public gsmGPRS 
{

	public:
	gsmMASTER(Serial& _telit, uint32_t (*_millis)(), Serial* _debug);
	bool init(uint16_t);

};


#endif
