#ifndef MYPAINTER_H
#define MYPAINTER_H


#include <QImage>
#include <QColor>

#include <cmath>
#include <stack>
#include <iostream>
#include <algorithm>

class MyPainter
{

private:
    QImage *TmpImg;
    QImage *MainImage;
    int szer, wys;

    //Kolory
    short red;
    short green;
    short blue;
public:
    MyPainter();
    MyPainter(QImage*, QImage*, int w, int s);

    //Deklaracje funkcji edytujacych obrazek
    void czysc(QImage *image,bool alpha);
    void zapalPiksel(int x, int y);
    void zapalPiksel2(int x, int y);
    void zapal4Piksele(int x0, int y0, int x, int y);
    QColor kolorPiksela(int x, int y);
    void odcinek(int x0, int y0,int x1, int y1);
    void okrag(int x0,int y0, int x1, int y1);
    void elipsa(int x0, int y0, int x1,int y1);
    void wielokat(int wierzcholki, int x0, int y0, int x1,int y1);
    void krzywaBeziera(QPoint p0, QPoint p1, QPoint p2, QPoint p3);
    void krzywaBSpline(QPoint p0, QPoint p1, QPoint p2, QPoint p3);
    void floodFil(int x, int y, QColor zamien);
public slots:
    //Settery kolorow
    void setRed(int kolor);
    void setGreen(int kolor);
    void setBlue(int kolor);
};

#endif // MYPAINTER_H
