#ifndef HDFFILE_H
#define HDFFILE_H
#include "mfhdf.h"

class HDFFile
{
public:
    unsigned char *b1 ;
    int ns, nl, ns_ll, nl_ll ;
    HDFFile(char *ifile);
    void LL_Fit() ;
};

#endif // HDFFILE_H
