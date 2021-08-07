#ifndef CR
#define CR


extern "C" {
#include "nr.h"
#include "nrutil.h"
}



class CReg {
	public :
	
	int   npts, ns, nl, ns_out, nl_out, nbands ;
	float  maxlat, minlat, maxlon, minlon, a_x [6], a_y [6] ;
	double *lats, *lons ;
	float *samp, *line ;

	CReg () ;
	~CReg () ;
	void GetCoefs () ;
    //void  ReadTiepts (char *) ;
    // alloc mem for points
    void setTiepts (double *, double *, float *, float *, int);
    void setTiepts (float *, float *, float *, float *, int);
	void  RegData (unsigned char *indat, unsigned char *outdat) ;
	void  WriteHeader (char *) ;

	friend void fitfunct (float x, float p[], int np) ;
} ;

#endif

