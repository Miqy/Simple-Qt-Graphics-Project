// Dolaczamy plik naglowkowy naszej klasy MyWindow
#include "mywindow.h"
#include <cmath>

// Dolaczamy plik naglowkowy zawierajacy definicje GUI
// Plik ten jest generowany automatycznie
// z pliku XML "mywindow.ui"
#include "ui_mywindow.h"

// Definicja konstruktora, wywolujemy najpierw
// konstruktor klasy nadrzednej, nastepnie tworzymy
// obiekt klasy Ui_MyWindow reprezentujacy GUI
MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    // Wywolujemy funkcje tworzaca elementy GUI
    // Jej definicja znajduje sie w pliku "ui_mywindow.h"
    ui->setupUi(this);

    // Pobieramy wymiary i wspolrzedne lewego gornego naroznika ramki
    // i ustawiamy wartosci odpowiednich pol
    // Uwaga: ramke "rysujFrame" wykorzystujemy tylko do
    // wygodnego ustaiwenia tych wymiarow. Rysunek bedziemy wyswietlac
    // bezposrednio w glownym oknie aplikacji.
    szer = ui->rysujFrame->width();
    wys = ui->rysujFrame->height();
    poczX = ui->rysujFrame->x();
    poczY = ui->rysujFrame->y();
    //zerowanie pol
    tool = 0;
    iloscPkt = 0;
    iloscWierzcholkow = 3;
    odcX = 0;
    odcY = 0;
    PrzesuwanyPkt = -1;
    UsuwanyPkt = -1;
    WyswietlPkty = true;


    // Tworzymy obiekt klasy QImage, o odpowiedniej szerokosci
    // i wysokosci. Ustawiamy format bitmapy na 32 bitowe RGB
    // (0xffRRGGBB).
    img = new QImage(szer,wys,QImage::Format_RGB32);
    tmpImg = new QImage(szer,wys,QImage::Format_ARGB32);

    //incijalizacja rysownika ktory rysuje
    rysownik = MyPainter(tmpImg, img,szer,wys);

    //Czyszczenie na poczatku okna do rysowania(zmiana na kolor bialy)
    rysownik.czysc(img, false);
    rysownik.czysc(tmpImg, true);

    //definicja polaczen sygnalow ze slotami z narzedziami
    connect(ui->Odcinek,SIGNAL(clicked()),this,SLOT(setOdcinekTool()));
    connect(ui->Okrag,SIGNAL(clicked(bool)),this,SLOT(setOkragTool()));
    connect(ui->Elipsa,SIGNAL(clicked(bool)),this,SLOT(setElipsaTool()));
    connect(ui->Wielokat,SIGNAL(clicked(bool)),this,SLOT(setWielokatTool()));
    connect(ui->Krzywe,SIGNAL(clicked(bool)),this,SLOT(setKrzyweTool()));
    connect(ui->Krzywe2,SIGNAL(clicked(bool)),this,SLOT(setKrzywe2Tool()));
    connect(ui->Malowanie,SIGNAL(clicked(bool)),this,SLOT(setMalowanie()));

    //polaczenia do obslugi kolorkow
    connect(ui->RedSlider,SIGNAL(valueChanged(int)),this,SLOT(setRed(int)));
    connect(ui->GreenSlider,SIGNAL(valueChanged(int)),this,SLOT(setGreen(int)));
    connect(ui->BlueSlider,SIGNAL(valueChanged(int)),this,SLOT(setBlue(int)));

    //polaczenia do obslugi wielokatow
    connect(ui->WielSlider,SIGNAL(valueChanged(int)),this,SLOT(setIloscWierzcholkow(int)));
    //polaczenie do obslugi krzywych(Rysowania pktow)
    connect(ui->RysujPkty,SIGNAL(toggled(bool)),this,SLOT(setWyswietlPkty(bool)));
    connect(ui->UsunPkt,SIGNAL(clicked(bool)),this,SLOT(usunPkt()));
}

// Definicja destruktora
MyWindow::~MyWindow()
{
    delete ui;
}





// Funkcja (slot) wywolywana po nacisnieciu przycisku "Wyjscie" (exitButton)
// Uwaga: polaczenie tej funkcji z sygnalem "clicked"
// emitowanym przez przycisk jest realizowane
// za pomoca funkcji QMetaObject::connectSlotsByName(MyWindow)
// znajdujacej sie w automatycznie generowanym pliku "ui_mywindow.h"
// Nie musimy wiec sami wywolywac funkcji "connect"
void MyWindow::on_exitButton_clicked()
{
    // qApp to globalny wskaznik do obiektu reprezentujacego aplikacje
    // quit() to funkcja (slot) powodujaca zakonczenie aplikacji z kodem 0 (brak bledu)
    qApp->quit();
}


// Funkcja "odmalowujaca" komponent
void MyWindow::paintEvent(QPaintEvent*)
{
    // Obiekt klasy QPainter pozwala nam rysowac na komponentach
    QPainter p(this);

    // Rysuje obrazek "img" w punkcie (poczX,poczY)
    // (tu bedzie lewy gorny naroznik)
    p.drawImage(poczX,poczY,*img);
    p.drawImage(poczX,poczY,*tmpImg);
    QImage *img = new QImage(8,8,QImage::Format_RGB32);
    img->fill(QColor(255,0,0));
    if(WyswietlPkty){
        for(int i = 0; i < punkty.size(); i++){
            // p.drawImage(punkty[i].x(),punkty[i].y(),QImage(8,8,QImage::Format_RGB32));
            p.drawImage(punkty[i].x(),punkty[i].y(),*img);
        }
    }
}

void MyWindow::setOdcinekTool()
{
    tool = 0;
    ui->Tool->setText(QString("Odcinek"));
}

void MyWindow::setOkragTool()
{
    tool = 1;
    ui->Tool->setText(QString("Okrag"));
}

void MyWindow::setElipsaTool()
{
    tool = 2;
    ui->Tool->setText(QString("Elipsa"));
}

void MyWindow::setWielokatTool()
{
    tool = 3;
    ui->Tool->setText(QString("Wielokat"));
}


void MyWindow::setKrzyweTool()
{
    tool = 4;
    ui->Tool->setText(QString("Krzywe"));
}

void MyWindow::setKrzywe2Tool()
{
    tool = 5;
    ui->Tool->setText(QString("BSpline"));
}

void MyWindow::setMalowanie()
{
    tool = 6;
    ui->Tool->setText(QString("Malowanie"));
}

void MyWindow::setRed(int kolor)
{
    rysownik.setRed(kolor);
}

void MyWindow::setGreen(int kolor)
{
    rysownik.setGreen(kolor);
}

void MyWindow::setBlue(int kolor)
{
    rysownik.setBlue(kolor);
}

void MyWindow::setIloscWierzcholkow(int value)
{
    iloscWierzcholkow = value;
}

void MyWindow::setWyswietlPkty(bool value)
{
    WyswietlPkty = value;
    update();
}

void MyWindow::usunPkt()
{
    if(UsuwanyPkt == -1) return;
    punkty.erase(punkty.begin() + UsuwanyPkt);
    UsuwanyPkt = -1;
    iloscPkt--;
    rysownik.czysc(img,false);
    if(tool == 4){
        if(iloscPkt >= 4){
            for(int i = 3; i < iloscPkt; i+= 3){
                rysownik.krzywaBeziera(punkty[i-3],punkty[i-2],punkty[i-1],punkty[i]);
            }
        }
    }else if(tool == 5){
        if(iloscPkt >= 4){
            for(int i = 3; i < iloscPkt; i++){
                rysownik.krzywaBSpline(punkty[i-3],punkty[i-2],punkty[i-1],punkty[i]);
            }
        }
    }
    update();
}


// Funkcja (slot) wywolywana po nacisnieciu przycisku "Czysc" (cleanButton)
void MyWindow::on_cleanButton_clicked()
{
    // Funkcja czysci (zamalowuje na bialo) obszar rysowania
    // definicja znajduje sie ponizej
    rysownik.czysc(img,false);
    rysownik.czysc(tmpImg, true);
    punkty.resize(0);
    iloscPkt = 0;
    PrzesuwanyPkt = -1;
    UsuwanyPkt = -1;

    // Funkcja "update()" powoduje ponowne "namalowanie" calego komponentu
    // Wywoluje funkcje "paintEvent"
    update();
}


// Funkcja (slot) wywolywana po nacisnieciu przycisku myszy (w glownym oknie)
void MyWindow::mousePressEvent(QMouseEvent *event)
{
    // Pobieramy wspolrzedne punktu klikniecia
    int x = event->x();
    int y = event->y();

    // Sa to wspolrzedne wzgledem glownego okna,
    // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku
    if(tool !=4 && event->buttons() == Qt::LeftButton){
        x -= poczX;
        y -= poczY;
        if ((x>=0)&&(y>=0)&&(x<szer)&&(y<wys)){
            odcX = x;
            odcY = y;
        }else{
            odcX = 0;
            odcY = 0;
        }
    }





    // Sprawdzamy ktory przycisk myszy zostal klkniety
    if(tool == 4){
        if(event->buttons() == Qt::LeftButton){
            for(int i = 0; i < punkty.size(); i++){
                if(abs(x - punkty[i].x()) < 8 && abs(y - punkty[i].y()) < 8){
                    PrzesuwanyPkt = i;
                    UsuwanyPkt = i;
                }
            }
        }
        else
        {
            if((x>=0)&&(y>=0)&&(x<szer)&&(y<wys)){

                punkty.push_back(QPoint(x,y));
                iloscPkt++;

                if( ((punkty.size() == 4) || ((punkty.size() > 3) && ((iloscPkt-1) % 3 == 0)))){
                    punkty[iloscPkt - 1] = QPoint(x,y);
                    rysownik.krzywaBeziera(punkty[iloscPkt-4],punkty[iloscPkt-3],punkty[iloscPkt-2],punkty[iloscPkt-1]);
                }
            }
        }
    }else if(tool == 5){
        if(event->buttons() == Qt::LeftButton){
            for(int i = 0; i < punkty.size(); i++){
                if(abs(x - punkty[i].x()) < 8 && abs(y - punkty[i].y()) < 8){
                    PrzesuwanyPkt = i;
                    UsuwanyPkt = i;
                }
            }
        }
        else
        {
            if((x>=0)&&(y>=0)&&(x<szer)&&(y<wys)){

                punkty.push_back(QPoint(x,y));
                iloscPkt++;

                if(punkty.size() >= 4){
                    punkty[iloscPkt - 1] = QPoint(x,y);
                    rysownik.krzywaBSpline(punkty[iloscPkt-4],punkty[iloscPkt-3],punkty[iloscPkt-2],punkty[iloscPkt-1]);
                }
            }
        }
    }else if(tool == 6){
        if((x>=0)&&(y>=0)&&(x<szer)&&(y<wys)){
            unsigned char* ptr = img->bits();
            QColor zamieniany;
            zamieniany.setBlue(ptr[szer*4*y + 4*x]);
            zamieniany.setGreen(ptr[szer*4*y + 4*x + 1]);
            zamieniany.setRed(ptr[szer*4*y + 4*x + 2]);
            rysownik.floodFil(x,y,zamieniany);
        }
    }


    // Odswiezamy komponent
    update();

}

void MyWindow::mouseMoveEvent(QMouseEvent *event)
{
    // Pobieramy wspolrzedne punktu klikniecia
    int x = event->x();
    int y = event->y();

    // Sa to wspolrzedne wzgledem glownego okna,
    // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku

    x -= poczX;
    y -= poczY;

    //Definicja wszystkich narzedzi
    if ((x>=0)&&(y>=0)&&(x<szer)&&(y<wys))
    {
        rysownik.czysc(tmpImg, true);
        if(tool == 0){
            rysownik.odcinek(odcX, odcY, x, y);
        }else if(tool == 1){
            rysownik.okrag(odcX,odcY,x,y);
        }else if(tool == 2){
            rysownik.elipsa(odcX,odcY,x,y);
        }else if(tool == 3){
            rysownik.wielokat(iloscWierzcholkow,odcX,odcY,x,y);
        }else if(tool == 4){    //Bardzo rozbudowane narzedzie do rysowania krzywych Beziera
            if(event->buttons() == Qt::LeftButton){
                if(punkty.size() >= 4){
                    //odtwarzanie krzywej Beziera po przesunieciu pktu
                    if(PrzesuwanyPkt != -1){
                        punkty[PrzesuwanyPkt] = QPoint(x,y);
                        rysownik.czysc(img,false);
                        if(iloscPkt >= 4){
                            for(int i = 3; i < iloscPkt; i+= 3){
                                rysownik.krzywaBeziera(punkty[i-3],punkty[i-2],punkty[i-1],punkty[i]);
                            }
                        }
                    }
                }
            }else{
                if(((punkty.size() == 4) || ((punkty.size() > 3) && ((iloscPkt-1) % 3 == 0)))){
                    punkty[iloscPkt - 1] = QPoint(x,y);
                    rysownik.krzywaBeziera(punkty[iloscPkt-4],punkty[iloscPkt-3],punkty[iloscPkt-2],punkty[iloscPkt-1]);
                }
            }

        }else if(tool == 5){
            if(event->buttons() == Qt::LeftButton){
                if(punkty.size() >= 4){
                    //odtwarzanie krzywej Bspline po przesunieciu pktu
                    if(PrzesuwanyPkt != -1){
                        punkty[PrzesuwanyPkt] = QPoint(x,y);
                        rysownik.czysc(img,false);
                        if(iloscPkt >= 4){
                            for(int i = 3; i < iloscPkt; i++){
                                rysownik.krzywaBSpline(punkty[i-3],punkty[i-2],punkty[i-1],punkty[i]);
                            }
                        }
                    }
                }
            }else{
                if(punkty.size() >= 4){
                    punkty[iloscPkt - 1] = QPoint(x,y);
                    rysownik.krzywaBSpline(punkty[iloscPkt-4],punkty[iloscPkt-3],punkty[iloscPkt-2],punkty[iloscPkt-1]);
                }
            }
        }

        update();
    }

    // Odswiezamy komponent
    update();

}


void MyWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // Pobieramy wspolrzedne punktu klikniecia
    int x = event->x();
    int y = event->y();

    // Sa to wspolrzedne wzgledem glownego okna,
    // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku

    x -= poczX;
    y -= poczY;

    // Sprawdzamy czy klikniecie nastapilo w granicach rysunku
    if ((x>=0)&&(y>=0)&&(x<szer)&&(y<wys))
    {


        //Doklejanie tymczasowego odcinka z tmpImg na glowny obraz img
        QPainter painter(img);
        QRectF target(0.0, 0.0, szer, wys);
        QRectF source(0.0, 0.0, szer, wys);
        painter.drawImage(target, *tmpImg, source);
        rysownik.czysc(tmpImg, true);

    }
    PrzesuwanyPkt = -1;

    // Odswiezamy komponent
    update();

}




