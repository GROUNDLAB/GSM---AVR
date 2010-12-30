#include "gsmMaster.h"

gsmMASTER::gsmMASTER(Serial& _telit, uint32_t (*_millis)(), Serial* _debug)
:GSMbase(_telit, _millis, _debug), gsmSMS(_telit ,_millis, _debug), 
gsmGPRS(_telit, _millis, _debug)
{}

bool gsmMASTER::init(uint16_t band){
	if(GSMbase::init( band) && gsmSMS::smsInit()) return 1;
return 0;	
}
