#include "CReg.h"
#include <fstream.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>






CReg::CReg()
{
	lats = NULL ;
	lons = NULL ;
	samp = NULL ;
	line = NULL ;
	minlat=1.E9 ;
	maxlat=-1.E9 ;
	minlon=1.E9 ;
	maxlon=-1.E9 ;
	ns = 4100 ;
	nl = 4200 ;
	nbands = 3 ;

}

CReg::~CReg()
{
	if (lats)
	delete [] lats ;
	if (lons)
	delete [] lons ;
	if (samp)
	delete [] samp ;
	if (line)
	delete [] line ;
}

void CReg::ReadTiepts (char *infile) 
{
	char fline [240] ;
	int  i ;

	npts = 0 ;
	ifstream ifil (infile, ios::in) ;

	// count number of tiepoints
	while (ifil.good() && !ifil.eof())
	{
		ifil.getline (fline, 240, '\n') ;
		npts++ ;
	}
	npts-- ;

	ifil.close () ;


	// alloc mem for points 
	lats = new double [npts] ;
	lons = new double [npts] ;
	samp = new float [npts] ;
	line = new float [npts] ;

	ifil.open (infile, ios::in) ;
	for ( i=0; i<npts; i++) ifil >> lons [i] >> lats [i] >> samp [i] >>
		line [i] ;
	for (i=0; i<npts; i++) {
		if (lats[i] > maxlat) maxlat = lats[i] ;
		if (lons[i] > maxlon) maxlon = lons[i] ;
		if (lons[i] < minlon) minlon = lons[i] ;
		if (lats[i] < minlat) minlat = lats[i] ;
	}
	

		
	for (i=0; i<npts; i++) {
		lats[i] = (maxlat - lats [i]) / .00015 ;
		lons[i] = (lons [i] - minlon) / .00015 ;
	}

	// get the number of samples and lines
	ns_out = (maxlon - minlon) / .00015 + 1 ;
	nl_out = (maxlat - minlat) / .00015 + 1 ;
		
	cout << "Bounds are ..." << endl ;
	cout << "Upperleft    " << maxlat << "   " << minlon << endl ; 
	cout << "Lowerright    " << minlat << "   " << maxlon << endl ; 
	cout << "ns : " << ns_out << endl ;
	cout << "nl : " << nl_out << endl ;

	ifil.close () ;
	return ;
}
		

void CReg::GetCoefs ()
{
	int i, ncoefs ;
	int  ia [6] ;
	float *ind, **u, **v, *w, **covar, chisq=0. ;
	float *sig, *a ;


	ncoefs = 6;
	ind = new float [npts] ;

	float *yf = new float [npts] ;
	sig  = new float [npts] ;
	for (i=0; i<6; i++) {
		a_x[i] = 0. ;
		a_y[i] = 0. ;
		ia[i] = 1 ;
	}

	for (i=0; i<npts; i++) {
		ind [i] = i ;
		sig [i] = 1. ;
	}


	u = matrix (1, npts, 1 , ncoefs) ;
	covar = matrix (1, ncoefs, 1 , ncoefs) ;
	v = matrix (1, ncoefs, 1, ncoefs) ;
	w = vector (long (1), long (ncoefs) ) ;

	a = &a_y [0] ;
	//svdfit (ind - 1, line - 1, sig-1, npts, a - 1, ncoefs, u, v,w, &chisq, fitfunct) ; 
	
	lfit (ind - 1, line - 1, sig-1, npts, a - 1, ia -1, ncoefs, covar, &chisq, fitfunct) ; 
	for (i=0; i<npts; i++) 
	{
		yf [i] = a [0] + a[1] * lons [i] + a[2] * lats [i] +
			a[3] * lons [i] * lats [i]  + a[4] * lons[i] * lons[i] + 
			a [5] * lats[i] * lats [i] ;  
	//	cout << line [i] << "   " << yf [i] << endl ;
	}
	a = &a_x [0] ;
	lfit (ind - 1, samp - 1, sig-1, npts, a - 1, ia -1, ncoefs, covar, &chisq, fitfunct) ; 

	for (i=0; i<npts; i++) 
	{
		yf [i] = a [0] + a[1] * lons [i] + a[2] * lats [i] +
			a[3] * lons [i] * lats [i]  + a[4] * lons[i] * lons[i] + 
			a [5] * lats[i] * lats [i] ;  
	//	cout << samp [i] << "   " << yf [i] << endl ;
	}


	free_matrix (u, 1, npts, 1, ncoefs) ;
	free_matrix (covar, 1, ncoefs, 1, ncoefs) ;
	free_matrix (v, 1, ncoefs, 1, ncoefs) ;
	free_vector (w, 1, ncoefs) ; 
	delete [] sig ;
	delete [] ind ;
}


void CReg::RegData (unsigned char *data, unsigned char *outdata) {
	
	int 	i, j, int_x, int_y ;
	unsigned char *outptr, *inptr ;
	double 	is, js, calc_x, calc_y, tmpval ;
	double  frac_x, frac_y ;

	for (i=0; i<nl_out; i++) {
		is  = i ;

		for (j=0; j<ns_out; j++) {
			outptr = outdata + long(i) * ns_out + j ;
			js = j ;
			calc_x = a_x [0] + a_x [1] * js + a_x [2] * is + a_x [3] *
				is * js + a_x [4] * js * js + a_x [5] * is * is ;
			calc_y = a_y [0] + a_y [1] * js + a_y [2] * is + a_y [3] *
				is * js + a_y [4] * js * js + a_y [5] * is * is ;

			// bilinear interpolate 
			// check if out of bounds of input array
			if (calc_x < 0 || calc_x >= ns-1 ||
			 	calc_y < 0 || calc_y >= nl-1) {
						*outptr = 0 ; 
						continue ;
				}

			int_x = int (calc_x) ;	
			int_y = int (calc_y) ;	
			frac_x = 1. - (calc_x - int_x) ;
			frac_y = 1. - (calc_y - int_y) ;
			
			inptr = data + long (int_y) * ns + int_x ;

			tmpval = frac_x * frac_y * *inptr + 
				frac_y * (1. - frac_x) * *(inptr+1) +
				(1-frac_y) * (1. - frac_x) * *(inptr+ns+1) +
				(1-frac_y) * frac_x * *(inptr+ns) ;
	
			if (tmpval <0) *outptr = 0 ;
			else 
			{
				if (tmpval >255) *outptr = 255 ;
				else *outptr = (unsigned char) tmpval ;
			}
		}
	}

	return ;
}

void CReg::WriteHeader (char *outfile) 
{
	char outfilehdr [320] ;
	strcpy (outfilehdr, outfile) ;
	strcat (outfilehdr, ".hdr") ;

	FILE *hdrout = fopen (outfilehdr, "w") ;
	
	fprintf (hdrout, "ENVI\ndescription = {\nRegistration Result }\n") ;
	fprintf (hdrout, "samples    = %5d\n",ns_out) ;
	fprintf (hdrout, "lines      = %5d\n",nl_out) ;
	fprintf (hdrout, "bands      = %5d\n",nbands) ;
	fprintf (hdrout, "header offset = 0 \n") ;
	fprintf (hdrout, "file type = ENVI Standard \n") ;
	fprintf (hdrout, "data type = 1 \n") ;
	fprintf (hdrout, "interleave = bsq \n") ;
	fprintf (hdrout, "sensor type = ASTER \n") ;
	fprintf (hdrout, "byte order = 0\n") ;
	fprintf (hdrout, "map info = {Geographic Lat/Lon, 1.0000, 1.0000, %12.8f, %12.8f,",
			minlon, maxlat) ;
	fprintf (hdrout, ".00015000, .00015000, WGS-84, units=Degrees}\n") ;

	fclose (hdrout) ;
}
	

	

	

void fitfunct (float x, float p [], int np) 
{
	int j ;
	extern CReg *crg ;
	j = int (x) ;
	p [1] = 1. ;
	p [2] = float(crg->lons [j]) ;
	p [3] = float (crg->lats [j]) ;
	p [4] = float (crg->lons [j] * crg->lats [j]) ;
	p [5] = float (crg->lons [j] * crg->lons [j]) ;
	p [6] = float (crg->lats [j] * crg->lats [j]) ;
	return ;
	
}
	

