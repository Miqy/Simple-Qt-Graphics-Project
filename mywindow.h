// Plik naglowkowy klasy MyWindow
// Obiekt tej klasy to glowne okno naszej aplikacji
// Szkielet tego pliku jest tworzony przez QtCreator
// Mozemy do niego dodac deklaracje wlasnych pol i metod

#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <iostream>
#include <vector>
#include <cmath>

#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QColor>

#include <mypainter.h>

namespace Ui {
    class MyWindow;
}

// MyWindow jest podklasa klasy QMainWindow.
class MyWindow : public QMainWindow
{

    Q_OBJECT

public:

    explicit MyWindow(QWidget *parent = 0);

    // Deklaracja destruktora
    ~MyWindow();






private:
    // QtCreator pozwala na tworzenie GUI za pomoca graficznego kreatora.
    // Skladniki interfejsu i ich wlasciwosci zapisane sa wowczas
    // w pliku XML "nazwa_klasy.ui"
    // Do poszczegolnych elementow GUI odwolujemy sie za pomoca zmiennej "ui"
    Ui::MyWindow *ui;

    //Obiekt typu MyPainter
    MyPainter rysownik;

    // Pole przechowujace obrazek
    QImage *MainImage;
    QImage *TmpImg;

    //Rodzaj aktualnego narzedzia do rysowania
    //zmienne pomocnicze do obslugi roznych narzedzi
    enum tool{LINE = 0, CIRCLE, ELLIPSE, POLYGON, BEZIERCURVE, BSPLINECURVE, FLOODFILL} tool;
    //zmienna przechowujaca ilosc wierzcholkow potrzebna do narysowania wielokata
    int iloscWierzcholkow;

    // Pola przechowujace szerokosc i wysokosc rysunku
    int szer;
    int wys;
    // oraz wspolrzedne jego lewego gornego naroznika
    int poczX;
    int poczY;
    //pola przechowujace poczatek odcinka przy pierwszym naicsnieciu muszy (OnMousePress)
    int odcX;
    int odcY;
    std::vector<QPoint> punkty;
    int PrzesuwanyPkt;
    int UsuwanyPkt;
    bool WyswietlPkty;




    // Deklaracje slotow, czyli funkcji wywolywanych
    // po wystapieniu zdarzen zwiazanych z GUI
    // np. klikniecie na przycisk, ruch myszka
private slots:
    void on_cleanButton_clicked();
    void on_exitButton_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent*);
    //Tools slots
    void setOdcinekTool();
    void setOkragTool();
    void setElipsaTool();
    void setWielokatTool();
    void setKrzyweTool();
    void setKrzywe2Tool();
    void setMalowanie();
    //Settery kolorow
    void setRed(int kolor);
    void setGreen(int kolor);
    void setBlue(int kolor);
    //setter dla wierzcholkow
    void setIloscWierzcholkow(int value);
    //setter do rysowania pktow i usuwania ich
    void setWyswietlPkty(bool value);
    void usunPkt();

};

#endif // MYWINDOW_H
