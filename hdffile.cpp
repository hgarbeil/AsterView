#include "hdffile.h"


#include <QDebug>

using namespace std ;
HDFFile::HDFFile(char *infile)
{

    b1 = 0;
    nl = 5113 ;
    ns =  5731 ;
    char name[240] ;
    int32 startarr[2]={0,0};
    int32 stride[2]={1,1};
    int32 endarr[2]={nl,ns};
    int32 endarr_ll[2]={11,11} ;
    double *lat, *lon ;
    int32 i, rank, sds_id, sd_id, n_datasets, n_fileattrs, dim_sizes[3], attributes, num_type, status, status_32 ;
    sd_id = SDstart (infile, DFACC_RDONLY) ;

            if (sd_id <= 0){
                    qDebug() << "Problem with : " << infile  ;

            }
            qDebug() << "opened " << infile << " with id : " << sd_id  ;
            n_datasets = 0 ;
            n_fileattrs = 0 ;
            SDfileinfo (sd_id, &n_datasets, &n_fileattrs) ;
            qDebug() << "num datasets " << n_datasets << " file atts : " <<
                    n_fileattrs  ;


            for (i=0; i<n_datasets; i++) {
                sds_id = SDselect (sd_id, i) ;
                SDgetinfo (sds_id, name, &rank, dim_sizes, &num_type, &attributes) ;
                qDebug() << sds_id << "  " << name << "  " << attributes ;
                if (sds_id==262144) {
                    nl_ll = dim_sizes[0] ;
                    ns_ll = dim_sizes[1] ;
                    endarr_ll[0] = nl_ll ;
                    endarr_ll[1] = ns_ll ;
                    lat = new double [ns_ll * ns_ll] ;
                    status = SDreaddata (sds_id,startarr,stride,endarr_ll,lat ) ;
                }
                if (sds_id==262145) {

                    lon = new double [ns_ll * ns_ll] ;
                    status = SDreaddata (sds_id,startarr,stride,endarr_ll,lon ) ;
                    qDebug() << nl_ll << " " << ns_ll << " " <<num_type  ;
                }
                if (sds_id==262148){
                    nl = dim_sizes[0] ;
                    ns = dim_sizes[1] ;

                    b1 = new unsigned char [ns * nl] ;
                    status = SDreaddata (sds_id, startarr, stride, endarr, b1) ;


                }

            }
            SDendaccess (sd_id) ;
}

void HDFFile::LL_Fit(doube *lat, double *lon ){
    int i, j, inum,xstep, ystep ;
    double *xvals, *yvals, *zvals, *zvals_1, xloc, yloc ;
    xstep = 498 ;
    ystep = 420 ;
    xvals = new double [121] ;
    yvals = new double [121] ;
    zvals = new double [121] ;
    zvals_1 = new double [121] ;

    // first fit x y to lat
    for (i=0; i<11; i++){
        yloc = i * ystep ;
        for (j=0; j<11; j++) {
            inum = i * 11 + j ;
            xloc = j * xstep ;
            xvals [inum] = xloc ;
            yvals [inum] = yloc ;
            zvals [inum] = lat[inum] ;
            zvals_1[inum]= lon[inum] ;
        }
    }


}
