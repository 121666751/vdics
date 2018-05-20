#ifndef _ZBITMASK_H
#define _ZBITMASK_H

#include <numbers/vbitmask.hpp>
#include <math.h>

class ZBitmask : public VBitmask
{
public:
	ZBitmask( VUINT nBits = 0 ) : VBitmask(nBits) {;}
	
	VUINT SetBit( VUINT nBit )
		{
			VASSERT( nBit < 31 );
			double dMask = pow(2, nBit);
			VUINT nBits = (VUINT)(dMask + 0.001);
			Append( nBits );
	
			return m_nBits;	
		}
		
	VUINT RemoveBit( VUINT nBit )
		{
			VASSERT( nBit < 31 );
			double dMask = pow(2, nBit);
			VUINT nBits = (VUINT)(dMask + 0.001);
			Remove( nBits );
	
			return m_nBits;	
		}
		
	VBOOL IsBitSet( VUINT nBit ) 
		{
			VASSERT( nBit < 31 );
			double dMask = pow(2, nBit);
			VUINT nBits = (VUINT)(dMask + 0.001);
	
			return IsSet( nBits );
		}
};

#endif //!_ZBITMASK_H
