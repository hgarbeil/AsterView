#include "imwidget.h"
#include <QPainter>
#include <QVector>
#include <QRgb>

int pointClicked [2] ;
ImWidget::ImWidget(QWidget *parent) : QWidget(parent)
{
    qim = 0l ;
    colors.clear() ;
}

ImWidget::~ImWidget(){
    if (qim) delete qim ;
}


void ImWidget::loadQImage_8 (unsigned char *dat, int ns, int nl){
    unsigned char uval ;
    int ival ;
    qim = new QImage (dat, ns, nl, ns, QImage::Format_Indexed8) ;
    colors.clear();
    for (unsigned int i = 0; i < 256; ++i) {
            ival = i * 1.5 ;
            if (ival <0) ival = 0 ;
            if (ival > 255)ival = 255 ;
            uval = ival ;
            colors.append (qRgb (uval/2, uval/2, uval)) ;
            //colors.append (qr) ;
            //colors.push_front(color.rgba());
   }
    qim->setColorTable(colors);
    this->update();

}

void ImWidget::paintEvent (QPaintEvent *qp) {
    int w, h ;
    w = this->width() ;
    h = this->height() ;

    QPainter p (this) ;
    //p.drawLine (0, 0, w, h) ;
    if (qim)
        p.drawImage (QRect(0, 0, w,h), *qim, QRect(3072,3072,w,h)) ;

}


void ImWidget::makeColorMap(float slope, float offset){

    unsigned char uval ;
    float val ;
    colors.clear() ;
    for (unsigned int i=0; i<256;i++){
        val = (i - offset) * slope ;
        if (val < 0) val=0. ;
        if (val > 255)val=255. ;
        uval = val ;
        colors.append(qRgb(uval, uval, uval));
    }
    qim->setColorTable(colors);


}


void ImWidget::mousePressEvent (QMouseEvent *ev){
    pointClicked[0] = ev->pos().x() ;
    pointClicked[1] = ev->pos().y() ;
    emit(imClicked(pointClicked)) ;

}
