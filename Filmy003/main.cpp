#include <iostream>
#include <fstream>

using namespace std;

/* 
    struktura zawieraj�ca informacje o filmie:
    - start - czas rozpocz�cia w minutach
    - stop - czas zako�czenia w minutach
    - duration - czas trwania filmu w minutach
    - active - stan filmu - true - czyli taki, kt�ry nie zosta� jeszcze skre�lony i false, 
    gdy ju� nie powinien by� brany pod uwag� z powodu dadania do listy telewicza 
    lub z powodu kolizji z wybranym filmem

*/

struct Film {
    int start;
    int stop;
    int duration;
    bool active;

    Film() {
        active = true;
        start = -1;
        stop = -1;
        duration = -1;
    }
};

/*
* int hhmm2min(string hhmm)
* funkcja zamienia czas z formatu hh:mm na czas wyra�ony w minutach
* parametr string - czas w postaci hh:mm
* return int - czas w zapisie minutowym
*/

int hhmm2min(string hhmm) {
    int min = 0;
    min = (hhmm[0] - '0') * 10 + (hhmm[1] - '0');
    min *= 60;
    min += (hhmm[3] - '0') * 10 + (hhmm[4] - '0');
    return min;
}

/*
* string min2hhmm(int min)
* funkcja zamienia czas z zapisu minutowego na czas w formacie hh:mm
* parametr int - czas w zapisany w minutach
* return string - czas w w formacie hh:mm
*/
string min2hhmm(int min) {
    string hhmm = "";
    int tmp;
    min = min % (24 * 60);  // 1440 - ilo�� minut w 24h. Wazne przypadku gdy czas w minutach mo�e wskazywa� na nast�pn� dob�
    tmp = min / 600;        // informacja ile dziesi�tek godzin mie�ci si� w podanym czasie (minutach)
    hhmm += tmp + '0';      // dopisanie ilo�ci dziesi�tek godzin do ci�gu wynikowego. Dodane 'O' by np. 1 przesun�� do cz�ci cyfr w tabeli kod�w ASCII
    min -= tmp * 600;       // pomniejszenie czasu o minuty reprezentuj�ce dziesi�tki godzin
    tmp = min / 60;
    hhmm += tmp + '0';
    min -= tmp * 60;
    hhmm += ':';
    hhmm += min / 10 + '0';
    hhmm += min % 10 + '0';
    return hhmm;
}
/*
    W funkcjach wyszykuj�cych pocz�tkow� warto�ci� odnalezionego filmu jest -1. Powoduje to, �e funkcje te zwracaj� -1 
    gdy nie s� w stanie odnale�� filmu w zbiorze
    
    INT_FAST32_MAX - warto�� maksymalna dla 32bitowego inta

    wszystkie funkcje wyszukuj�ce posiadaj� zmienn� logiczn� czy, kt�ra odpowiada na pytanie, czy odnaleziono film 
    spe�niaj�cy warunki danej funkcji

    funkcje te zwracaj� indeks filmu, kt�ry jest zgodny z warunkami wyszukiwania lub -1 gdy nie uda�o si� 
    odnale�� takiego filmu

    funkcje wyszukuj�ce:
    - shortest
    - longest
    - earliestStart
    - earliestStop
*/


// znajdowanie aktywnego filmu, kt�ry trwa najkr�cej (i najwcze�niej ko�czy)

int shortest(Film filmy[], int n) {
    int id = -1; 
    int min = INT_FAST32_MAX;
    bool czy = false;

    for (int i = 0; i < n; i++) {
        czy = false;
        if (filmy[i].active == true && filmy[i].duration < min) czy = true;
        if (filmy[i].active == true && filmy[i].duration == min && filmy[i].stop < filmy[id].stop) czy = true;

        if (czy == true) {
            min = filmy[i].duration;
            id = i;
        }
    }
    return id;
}

// znajdowanie aktywnego filmu, kt�ry trwa najd�u�ej (i najwcze�niej ko�czy)

int longest(Film filmy[], int n) {
    int id = -1;
    int max = -1;
    bool czy = false;

    for (int i = 0; i < n; i++) {
        czy = false;
        if (filmy[i].active == true && filmy[i].duration > max) czy = true;
        if (filmy[i].active == true && filmy[i].duration == max && filmy[i].stop < filmy[id].stop) czy = true;

        if (czy == true) {
            max = filmy[i].duration;
            id = i;
        }
    }
    return id;
}

// znajdowanie aktywnego filmu, kt�ry zaczyna si� najwcze�niej (i najwcze�niej ko�czy)

int earliestStart(Film filmy[], int n) {
    int id = -1;
    int min = INT_FAST32_MAX;
    bool czy = false;

    for (int i = 0; i < n; i++) {
        czy = false;
        if (filmy[i].active == true && filmy[i].start < min) czy = true;
        if (filmy[i].active == true && filmy[i].start == min && filmy[i].stop < filmy[id].stop) czy = true;

        if (czy == true) {
            min = filmy[i].duration;
            id = i;
        }
    }
    return id;
}

// znajdowanie aktywnego filmu, kt�ry ko�czy si� najwcze�niej (i zaczyna najp�niej)

int earliestStop(Film filmy[], int n) {
    int id = -1;
    int min = INT_FAST32_MAX;
    bool czy = false;

    for (int i = 0; i < n; i++) {
        czy = false;
        if (filmy[i].active == true && filmy[i].stop < min) czy = true;
        if (filmy[i].active == true && filmy[i].stop == min && filmy[i].start > filmy[id].start) czy = true;

        if (czy == true) {
            min = filmy[i].duration;
            id = i;
        }
    }
    return id;
}

// funkcja ustawiaj�ca status active na false wszystkim koliduj�cym, z filmem o indeksie id, filmom
// wzi�te pod uwag� 3 mo�liwo�ci - film zaczyna si� w czasie projekcji filmu id, film ko�czy si� w czasie 
// projekcji filmu id, film zawiera w sobie czas projekcji filmu id

void colisions(Film filmy[], int n, int id) {

    for (int i = 0; i < n; i++) {
        if (filmy[i].active) {
            if (filmy[i].start >= filmy[id].start && filmy[i].start < filmy[id].stop) {
                filmy[i].active = false;
                continue;
            }
            if (filmy[i].stop > filmy[id].start && filmy[i].stop <= filmy[id].stop) {
                filmy[i].active = false;
                continue;
            }
            if (filmy[i].start <= filmy[id].start && filmy[i].stop >= filmy[id].stop) {
                filmy[i].active = false;
                continue;
            }
        }
    }
}

// funkcja przywracaj�ca status active wszystkim filmom na li�cie

void reset(Film filmy[], int n) {
    for (int i = 0; i < n; i++) {
        filmy[i].active = true;
    }
}

// funkcja realizuj�ca za�o�enia strategii A

void strategiaA(Film filmy[], int n) {
    int id = longest(filmy, n);
    while (id >= 0) {
        // wypisanie informacji o znaleziomy/wyznaczonym filmie
        cout << "[" << (id+1) << "]" << min2hhmm(filmy[id].start) << " " << min2hhmm(filmy[id].stop) << " " << min2hhmm(filmy[id].duration) << endl;
        // ustawienie statusy active na false wszystkim koliduj�cym filmom
        colisions(filmy, n, id);
        // wyszukanie kolejnego filmu
        id = longest(filmy, n);
    }
}

// funkcja realizuj�ca za�o�enia strategii B

void strategiaB(Film filmy[], int n) {
    int id = shortest(filmy, n);
    while (id >= 0) {
        // wypisanie informacji o znaleziomy/wyznaczonym filmie
        cout << "[" << (id+1) << "]" << min2hhmm(filmy[id].start) << " " << min2hhmm(filmy[id].stop) << " " << min2hhmm(filmy[id].duration) << endl;
        // ustawienie statusy active na false wszystkim koliduj�cym filmom
        colisions(filmy, n, id);
        // wyszukanie kolejnego filmu
        id = shortest(filmy, n);
    }
}

// funkcja realizuj�ca za�o�enia strategii C

void strategiaC(Film filmy[], int n) {
    int id = earliestStart(filmy, n);
    while (id >= 0) {
        cout << "[" << (id + 1) << "]" << min2hhmm(filmy[id].start) << " " << min2hhmm(filmy[id].stop) << " " << min2hhmm(filmy[id].duration) << endl;
        colisions(filmy, n, id);
        id = earliestStart(filmy, n);
    }
}

// funkcja realizuj�ca za�o�enia strategii D

void strategiaD(Film filmy[], int n) {
    int id = earliestStop(filmy, n);
    while (id >= 0) {
        cout << "[" << (id + 1) << "]" << min2hhmm(filmy[id].start) << " " << min2hhmm(filmy[id].stop) << " " << min2hhmm(filmy[id].duration) << endl;
        colisions(filmy, n, id);
        id = earliestStop(filmy, n);
    }
}

int main() {
    ifstream plik("dane.in");   // plik z danymi, by nie wpisyawa� ich za ka�dym razem
    if (plik.good()) {
        int n;
        plik >> n;                  // czytanie ilo�ci film�w
        int indNK = 0;              // zmienna wskazuj�ca, kt�y fim okaza� si� najkr�tszy
        Film* filmy = new Film[n]; // stworzenie n-elementowej tablicy film�w 
        string stmp;                // zmienna pomocnicza by odczyta� kolejne czasy jako stringi
        for (int i = 0; i < n; i++) {
            plik >> stmp;                       // odczyt czasu rozpocz�cia filmu (hh:mm) 
            filmy[i].start = hhmm2min(stmp);    // zmiana formatu czasu na minutowy i wstawienie go do tablicy dla i-tego filmu
            plik >> stmp;                       // odczyt czasu trwania filmu (hh:mm)
            filmy[i].duration = hhmm2min(stmp);// zmiana formatu czasu na minutowy i wstawienie go do tablicy dla i-tego filmu
            filmy[i].stop = filmy[i].start + filmy[i].duration; // wyznaczenie czasu zako�czenia filmu
        }
        strategiaA(filmy, n);
        reset(filmy, n);
        cout << "------" << endl;
        strategiaB(filmy, n);
        reset(filmy, n);
        cout << "------" << endl;
        strategiaC(filmy, n);
        reset(filmy, n);
        cout << "------" << endl;
        strategiaD(filmy, n);
    }
    else
        cout << "co� posz�o nie tak";
}

