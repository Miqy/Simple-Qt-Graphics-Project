// Dolaczamy plik naglowkowy naszej klasy MyWindow
#include "mywindow.h"


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
    tool = LINE;
    iloscWierzcholkow = 3;
    odcX = 0;
    odcY = 0;
    PrzesuwanyPkt = -1;
    UsuwanyPkt = -1;
    WyswietlPkty = true;


    // Tworzymy obiekt klasy QImage, o odpowiedniej szerokosci
    // i wysokosci. Ustawiamy format bitmapy na 32 bitowe RGB
    // (0xffRRGGBB).
    MainImage = new QImage(szer,wys,QImage::Format_RGB32);
    TmpImg = new QImage(szer,wys,QImage::Format_ARGB32);

    //incijalizacja rysownika ktory rysuje
    rysownik = MyPainter(TmpImg, MainImage,szer,wys);

    //Czyszczenie na poczatku okna do rysowania(zmiana na kolor bialy)
    rysownik.czysc(MainImage, false);
    rysownik.czysc(TmpImg, true);

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
    p.drawImage(poczX,poczY,*MainImage);
    p.drawImage(poczX,poczY,*TmpImg);
    QImage *img = new QImage(8,8,QImage::Format_RGB32);
    img->fill(QColor(200,100,5));
    if(WyswietlPkty){
        for(int i = 0; i < punkty.size(); i++){
            p.drawImage(punkty[i].x(),punkty[i].y(),*img);
        }
    }
}

void MyWindow::setOdcinekTool()
{
    tool = LINE;
    ui->Tool->setText(QString("Odcinek"));
}

void MyWindow::setOkragTool()
{
    tool = CIRCLE;
    ui->Tool->setText(QString("Okrag"));
}

void MyWindow::setElipsaTool()
{
    tool = ELLIPSE;
    ui->Tool->setText(QString("Elipsa"));
}

void MyWindow::setWielokatTool()
{
    tool = POLYGON;
    ui->Tool->setText(QString("Wielokat"));
}


void MyWindow::setKrzyweTool()
{
    tool = BEZIERCURVE;
    ui->Tool->setText(QString("Krzywe"));
}

void MyWindow::setKrzywe2Tool()
{
    tool = BSPLINECURVE;
    ui->Tool->setText(QString("BSpline"));
}

void MyWindow::setMalowanie()
{
    tool = FLOODFILL;
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

//funkcja usuwajaca ostatnio wybrany punkt
void MyWindow::usunPkt()
{
    if(UsuwanyPkt == -1) return;
    punkty.erase(punkty.begin() + UsuwanyPkt);
    UsuwanyPkt = -1;
    rysownik.czysc(MainImage,false);
    if(tool == 4){
        if(punkty.size() >= 4){
            for(int i = 3; i < punkty.size(); i+= 3){
                rysownik.krzywaBeziera(punkty[i-3],punkty[i-2],punkty[i-1],punkty[i]);
            }
        }
    }else if(tool == 5){
        if(punkty.size() >= 4){
            for(int i = 3; i < punkty.size(); i++){
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
    rysownik.czysc(MainImage,false);
    rysownik.czysc(TmpImg, true);
    punkty.resize(0);
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
    if(tool != BEZIERCURVE && event->buttons() == Qt::LeftButton){
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

    if(tool == BEZIERCURVE){
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

                if( ((punkty.size() == 4) || ((punkty.size() > 3) && ((punkty.size()-1) % 3 == 0)))){
                    punkty[punkty.size() - 1] = QPoint(x,y);
                    rysownik.krzywaBeziera(punkty[punkty.size()-4],punkty[punkty.size()-3],punkty[punkty.size()-2],punkty[punkty.size()-1]);
                }
            }
        }
    }else if(tool == BSPLINECURVE){
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

                if(punkty.size() >= 4){
                    punkty[punkty.size() - 1] = QPoint(x,y);
                    rysownik.krzywaBSpline(punkty[punkty.size()-4],punkty[punkty.size()-3],punkty[punkty.size()-2],punkty[punkty.size()-1]);
                }
            }
        }
    }else if(tool == FLOODFILL){
        if((x>=0)&&(y>=0)&&(x<szer)&&(y<wys)){
            unsigned char* ptr = MainImage->bits();
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
        rysownik.czysc(TmpImg, true);
        if(tool == LINE){
            rysownik.odcinek(odcX, odcY, x, y);
        }else if(tool == CIRCLE){
            rysownik.okrag(odcX,odcY,x,y);
        }else if(tool == ELLIPSE){
            rysownik.elipsa(odcX,odcY,x,y);
        }else if(tool == POLYGON){
            rysownik.wielokat(iloscWierzcholkow,odcX,odcY,x,y);
        }else if(tool == BEZIERCURVE){    //Bardzo rozbudowane narzedzie do rysowania krzywych Beziera
            if(event->buttons() == Qt::LeftButton){
                if(punkty.size() >= 4){
                    //odtwarzanie krzywej Beziera po przesunieciu pktu
                    if(PrzesuwanyPkt != -1){
                        punkty[PrzesuwanyPkt] = QPoint(x,y);
                        rysownik.czysc(MainImage,false);
                        if(punkty.size() >= 4){
                            for(int i = 3; i < punkty.size(); i+= 3){
                                rysownik.krzywaBeziera(punkty[i-3],punkty[i-2],punkty[i-1],punkty[i]);
                            }
                        }
                    }
                }
            }else{
                if(((punkty.size() == 4) || ((punkty.size() > 3) && ((punkty.size()-1) % 3 == 0)))){
                    punkty[punkty.size() - 1] = QPoint(x,y);
                    rysownik.krzywaBeziera(punkty[punkty.size()-4],punkty[punkty.size()-3],punkty[punkty.size()-2],punkty[punkty.size()-1]);
                }
            }

        }else if(tool == BSPLINECURVE){
            if(event->buttons() == Qt::LeftButton){
                if(punkty.size() >= 4){
                    //odtwarzanie krzywej Bspline po przesunieciu pktu
                    if(PrzesuwanyPkt != -1){
                        punkty[PrzesuwanyPkt] = QPoint(x,y);
                        rysownik.czysc(MainImage,false);
                        if(punkty.size() >= 4){
                            for(int i = 3; i < punkty.size(); i++){
                                rysownik.krzywaBSpline(punkty[i-3],punkty[i-2],punkty[i-1],punkty[i]);
                            }
                        }
                    }
                }
            }else{
                if(punkty.size() >= 4){
                    punkty[punkty.size() - 1] = QPoint(x,y);
                    rysownik.krzywaBSpline(punkty[punkty.size()-4],punkty[punkty.size()-3],punkty[punkty.size()-2],punkty[punkty.size()-1]);
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


        //Doklejanie tymczasowego obrazu z tmpImg na glowny obraz MainImage
        QPainter painter(MainImage);
        QRectF target(0.0, 0.0, szer, wys);
        QRectF source(0.0, 0.0, szer, wys);
        painter.drawImage(target, *TmpImg, source);
        rysownik.czysc(TmpImg, true);
    }
    //ustawianie przesuwanego pktu na zaden z punktow
    PrzesuwanyPkt = -1;

    // Odswiezamy komponent
    update();

}




