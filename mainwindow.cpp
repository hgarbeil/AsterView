#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include "hdffile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this) ;
    char file [420] ;
    strcpy (file, "/Users/hg/workdir/data/AST_L1T_03107012021191132_20210803162931_21709.hdf") ;
    HDFFile *hf = new HDFFile (file) ;
    ui->imwidget->loadQImage_8 (hf->b1, hf->ns, hf->nl) ;

    float *ydat = new float [2048] ;
    for (int i=0; i<2048; i++) {
        *(ydat+i) = *(hf->b1+(3000 * hf->ns)+1500+i) ;
    }

    ui->plotwidget->setYData (0, ydat, 2048);
    ui->imwidget->makeColorMap(2., 10.) ;
    ui->imwidget->update() ;
    delete hf ;
    scaleval = 1. ;
    offval = 0 ;
    connect (ui->imwidget, SIGNAL(imClicked(int*)), this,SLOT(gotClick(int *) )) ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QFile qf =  QFileDialog::getOpenFileName (this, tr("Open Image")) ;
    qDebug () << qf.fileName() ;

}

void MainWindow::on_scale_slider_valueChanged(int value)
{
    float myValue = float(value) * .1 ;
    QString qs = QString("%1").arg(myValue,4,'g',2) ;
    scaleval = myValue ;
    ui->scaleLabel->setText (qs);
}

void MainWindow::gotClick (int *pt){

    qDebug () <<"x is : "<< pt[0] ;
    qDebug () <<"y is : "<< pt[1] ;
}

void MainWindow::on_off_slider_valueChanged(int value)
{
    QString qs = QString("%1").arg(value) ;
    ui->offLabel->setText (qs);
    offval = value ;
}


void MainWindow::on_updateButton_clicked()
{
    ui->imwidget->makeColorMap (scaleval, offval);
    ui->imwidget->update() ;

}

