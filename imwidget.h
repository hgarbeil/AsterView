#ifndef IMWIDGET_H
#define IMWIDGET_H

#include <QWidget>
#include <QImage>
#include <QVector>
#include <QMouseEvent>


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
    void mousePressEvent (QMouseEvent *ev) ;
    QVector<QRgb> colors ;

signals:
    void imClicked (int *);
};

#endif // IMWIDGET_H
