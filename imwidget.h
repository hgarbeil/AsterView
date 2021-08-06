#ifndef IMWIDGET_H
#define IMWIDGET_H

#include <QWidget>
#include <QImage>
#include <QVector>


class ImWidget : public QWidget
{
    Q_OBJECT
public:
    QImage *qim ;
    explicit ImWidget(QWidget *parent = nullptr);
    ~ImWidget() ;
    void loadQImage_8 (unsigned char *dat, int ns, int nl);
    void makeColorMap(float slope, float offset) ;
    void paintEvent(QPaintEvent *) ;
    QVector<QRgb> colors ;

signals:

};

#endif // IMWIDGET_H
