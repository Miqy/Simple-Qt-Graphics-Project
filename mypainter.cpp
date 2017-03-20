#include "mypainter.h"
#include <iostream>
#include <algorithm>

MyPainter::MyPainter()
{

}

MyPainter::MyPainter(QImage *image, QImage *image2, int w, int s)
{
    img = image;
    this->image = image2;
    szer = s;
    wys = w;
    red = green = blue = 0;

}



void MyPainter::setRed(int kolor)
{
    red = kolor;
}

void MyPainter::setGreen(int kolor)
{
    green = kolor;
}

void MyPainter::setBlue(int kolor)
{
    blue = kolor;
}

void MyPainter:: czysc(QImage *image, bool alpha)
{
    // Wskaznik za pomoca, ktorego bedziemy modyfikowac obraz
    unsigned char *ptr;

    // Funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
    ptr = image->bits();

    int i,j;

    // Przechodzimy po wszystkich wierszach obrazu
    for(i=0; i<wys; i++)
    {
        // Przechodzimy po pikselach danego wiersza
        // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
        for(j=0; j<szer; j++)
        {
            ptr[szer*4*i + 4*j]=255; // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = 255; // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = 255; // Skladowa RED
            if(alpha){
                ptr[szer*4*i + 4*j + 3] = 0; // Skladowa ALFA
            }
        }
    }
}


void MyPainter::zapalPiksel(int x, int y)
{
    if(x > 0 && y > 0 && x < szer && y < wys){
        unsigned char *ptr;
        ptr = img->bits();
        ptr[szer*4*y + 4*x] = blue;
        ptr[szer*4*y + 4*x + 1] = green;
        ptr[szer*4*y + 4*x + 2] = red;
        ptr[szer*4*y + 4*x + 3] = 255;
    }
}

void MyPainter::zapalPiksel2(int x, int y)
{
    if(x > 0 && y > 0 && x < szer && y < wys){
        unsigned char *ptr;
        ptr = image->bits();
        ptr[szer*4*y + 4*x] = blue;
        ptr[szer*4*y + 4*x + 1] = green;
        ptr[szer*4*y + 4*x + 2] = red;
    }
}

void MyPainter::zapal4Piksele(int x0, int y0, int x, int y)
{
    zapalPiksel(x0 +x,y0 + y);
    zapalPiksel(x0+x,y0-y);
    zapalPiksel(x0-x,y0+y);
    zapalPiksel(x0-x,y0-y);



}

void MyPainter::odcinek(int x0, int y0, int x1, int y1)
{
    float a, b;


    if(x0 == x1 || y0 == y1){
        a = 0;
        b = y1;
    }else{
        a = (float)(y1 - y0)/(x1 -x0);
        b = (float)(y0 - a * x0);
    }
    if(x0 < x1){
        if(abs(a) < 1){
            for(int x = x0; x <=x1; x++){
                int y = a * x + b;
                y = (int)floor(y + 0.5f);

                zapalPiksel(x,y);
            }
        }else if(abs(a) >= 1 && y0 <= y1 ){
            for(int y = y0; y <=y1; y++){
                int x = (y - b)/a;
                x = (int)floor(x + 0.5f);

                zapalPiksel(x,y);
            }
        }else{
            for(int y = y1; y <=y0; y++){
                int x = (y - b)/a;
                x = (int)floor(x + 0.5f);

                zapalPiksel(x,y);
            }
        }


    }else if(x0 > x1){
        if(abs(a) < 1){
            for(int x = x1; x <=x0; x++){
                int y = a * x + b;
                y = (int)floor(y + 0.5f);

                zapalPiksel(x,y);
            }
        }else if(abs(a) >= 1 && y1 <= y0){
            for(int y = y1; y <=y0; y++){
                int x = (y - b)/a;
                x = (int)floor(x + 0.5f);

                zapalPiksel(x,y);
            }

        }else{
            for(int y = y0; y <=y1; y++){
                int x = (y - b)/a;
                x = (int)floor(x + 0.5f);

                zapalPiksel(x,y);
            }
        }
    }else if(x0 == x1 && y0 < y1){
        for(int y = y0; y <=y1; y++){
            int x;
            x= x0;
            zapalPiksel(x,y);
        }
    }else{
        for(int y = y1; y <=y0; y++){
            int x;

            x= x0;
            zapalPiksel(x,y);
        }
    }
}

void MyPainter::okrag(int x0, int y0, int x1, int y1){
    //Obliczanie promienia na podstawie dwoch punktów
    float r = (float)sqrt((float)(x1 - x0)*(x1 - x0) + (float)(y1 - y0)*(y1 - y0));

    //Rysowanie gornej i dolnej czesci okregu
    for(int x = 0; x<= 0 + r; x++){
        int y = -sqrt(r * r - (float)(x) * (float)(x));
        y =  (int)floor(y + 0.5f);
        zapal4Piksele(x0,y0,x,y);




    }
    //Rysowanie prawej i lewej czesci okregu
    for(int y = 0; y<= 0 + r; y++){
        int x = sqrt(r * r - (float)(y) * (float)(y));
        x =  (int)floor(x + 0.5f);

        zapal4Piksele(x0,y0,x,y);

    }

}

void MyPainter::elipsa(int x0, int y0, int x1, int y1)
{
    double a,b;
    int krokx, kroky;

    a = (double)x1 - (double)x0;
    b = (double)y1 - (double)y0;
    krokx = abs(x1 - x0);
    kroky = abs(y1 - y0);
    if(kroky > krokx){
        krokx = kroky;
    }

    for(double t = 0; t < 6.28318530718; t += 3.14/(4 * krokx)){
        int x = x0 + a * sin(t);
        int y = y0 + b * cos(t);
        x = floor(x + 0.5f);
        y = floor(y + 0.5f);
        zapalPiksel(x,y);


    }

}

void MyPainter::wielokat(int wierzcholki, int x0, int y0, int x1, int y1)
{
    double a,b;
    int x, y, oldX, oldY, startX, startY;


    a = (double)x1 - (double)x0;
    b = (double)y1 - (double)y0;
    x = x0 + a * sin(0);
    y = y0 + b * cos(0);
    x = floor(x + 0.5f);
    y = floor(y + 0.5f);

    //Potrzebne do zamkniecia wielokata
    startX = x;
    startY = y;


    for(double t = 0; t < 6.28318530718; t += 6.28318530718/(wierzcholki)){
        oldX = x;
        oldY = y;
        x = x0 + a * sin(t);
        y = y0 + b * cos(t);
        x = floor(x + 0.5f);
        y = floor(y + 0.5f);
        odcinek(oldX,oldY,x,y);
    }
    odcinek(x,y,startX,startY);


}

void MyPainter::krzywaBeziera(QPoint p0, QPoint p1, QPoint p2, QPoint p3)
{
    /*std::cout << "p0" << ":" << p0.x() << "," << p0.y() << std::endl;
    std::cout << "p1" << ":" << p1.x() << "," << p2.y() << std::endl;
    std::cout << "p2" << ":" << p2.x() << "," << p2.y() << std::endl;
    std::cout << "p3" << ":" << p3.x() << "," << p3.y() << std::endl;*/

    for(double t = 0; t < 1.0; t += 0.001){
        int x = pow(1-t,3) * p0.x() + 3*pow(1-t,2) * t * p1.x() + 3 * (1-t)  * pow(t,2) * p2.x() + pow(t,3) * p3.x();
        int y = pow(1-t,3) * p0.y() + 3*pow(1-t,2) * t * p1.y() + 3 * (1-t)  * pow(t,2) * p2.y() + pow(t,3) * p3.y();
        x = floor(x+0.5f);
        y = floor(y+0.5f);
        zapalPiksel(x,y);
    }


}

void MyPainter::krzywaBSpline(QPoint p0, QPoint p1, QPoint p2, QPoint p3)
{
    for(double t = 0; t < 1.0; t += 0.001){
        int x = (((-pow(t,3) + 3*pow(t,2) - 3*t + 1)/6) * p0.x()) + (((3*pow(t,3)- 6*pow(t,2) + 4)/6)*p1.x()) + (((-3*pow(t,3) + 3*pow(t,2) + 3*t + 1)/6) * p2.x()) + ((pow(t,3)/6)*p3.x());
        int y = (((-pow(t,3) + 3*pow(t,2) - 3*t + 1)/6) * p0.y()) + (((3*pow(t,3)- 6*pow(t,2) + 4)/6)*p1.y()) + (((-3*pow(t,3) + 3*pow(t,2) + 3*t + 1)/6) * p2.y()) + ((pow(t,3)/6)*p3.y());
        x = floor(x+0.5f);
        y = floor(y+0.5f);
        zapalPiksel(x,y);
    }
}

void MyPainter::floodFil(int x, int y, QColor zamien)
{
    unsigned char* ptr = image->bits();
    QColor Kol;
    QPoint p;

    std::stack<QPoint> stos;
    stos.push(QPoint(x,y));
    while(!stos.empty()){
        p = stos.top();
        stos.pop();
        int x,y;
        x = p.x();
        y = p.y();
        if(x < 0 || x > szer-1 || y < 0 || y > wys-1) continue;
        Kol.setBlue(ptr[szer*4*y + 4*x]);
        Kol.setGreen(ptr[szer*4*y + 4*x + 1]);
        Kol.setRed(ptr[szer*4*y + 4*x + 2]);
        Kol.setAlpha(255);

        if (Kol == zamien){
           zapalPiksel2(x, y);
           stos.push(QPoint(p.x()-1,p.y()));
           stos.push(QPoint(p.x()+1,p.y()));
           stos.push(QPoint(p.x(),p.y()-1));
           stos.push(QPoint(p.x(),p.y()+1));
        }
    }
    return;

}

