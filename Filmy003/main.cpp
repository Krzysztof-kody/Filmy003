#include <iostream>
#include <fstream>

using namespace std;

/* 
    struktura zawieraj¹ca informacje o filmie:
    - start - czas rozpoczêcia w minutach
    - stop - czas zakoñczenia w minutach
    - duration - czas trwania filmu w minutach
    - active - stan filmu - true - czyli taki, który nie zosta³ jeszcze skreœlony i false, 
    gdy ju¿ nie powinien byæ brany pod uwagê z powodu dadania do listy telewicza 
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
* funkcja zamienia czas z formatu hh:mm na czas wyra¿ony w minutach
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
    min = min % (24 * 60);  // 1440 - iloœæ minut w 24h. Wazne przypadku gdy czas w minutach mo¿e wskazywaæ na nastêpn¹ dobê
    tmp = min / 600;        // informacja ile dziesi¹tek godzin mieœci siê w podanym czasie (minutach)
    hhmm += tmp + '0';      // dopisanie iloœci dziesi¹tek godzin do ci¹gu wynikowego. Dodane 'O' by np. 1 przesun¹æ do czêœci cyfr w tabeli kodów ASCII
    min -= tmp * 600;       // pomniejszenie czasu o minuty reprezentuj¹ce dziesi¹tki godzin
    tmp = min / 60;
    hhmm += tmp + '0';
    min -= tmp * 60;
    hhmm += ':';
    hhmm += min / 10 + '0';
    hhmm += min % 10 + '0';
    return hhmm;
}
/*
    W funkcjach wyszykuj¹cych pocz¹tkow¹ wartoœci¹ odnalezionego filmu jest -1. Powoduje to, ¿e funkcje te zwracaj¹ -1 
    gdy nie s¹ w stanie odnaleŸæ filmu w zbiorze
    
    INT_FAST32_MAX - wartoœæ maksymalna dla 32bitowego inta

    wszystkie funkcje wyszukuj¹ce posiadaj¹ zmienn¹ logiczn¹ czy, która odpowiada na pytanie, czy odnaleziono film 
    spe³niaj¹cy warunki danej funkcji

    funkcje te zwracaj¹ indeks filmu, który jest zgodny z warunkami wyszukiwania lub -1 gdy nie uda³o siê 
    odnaleŸæ takiego filmu

    funkcje wyszukuj¹ce:
    - shortest
    - longest
    - earliestStart
    - earliestStop
*/


// znajdowanie aktywnego filmu, który trwa najkrócej (i najwczeœniej koñczy)

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

// znajdowanie aktywnego filmu, który trwa najd³u¿ej (i najwczeœniej koñczy)

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

// znajdowanie aktywnego filmu, który zaczyna siê najwczeœniej (i najwczeœniej koñczy)

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

// znajdowanie aktywnego filmu, który koñczy siê najwczeœniej (i zaczyna najpóŸniej)

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

// funkcja ustawiaj¹ca status active na false wszystkim koliduj¹cym, z filmem o indeksie id, filmom
// wziête pod uwagê 3 mo¿liwoœci - film zaczyna siê w czasie projekcji filmu id, film koñczy siê w czasie 
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

// funkcja przywracaj¹ca status active wszystkim filmom na liœcie

void reset(Film filmy[], int n) {
    for (int i = 0; i < n; i++) {
        filmy[i].active = true;
    }
}

// funkcja realizuj¹ca za³o¿enia strategii A

void strategiaA(Film filmy[], int n) {
    int id = longest(filmy, n);
    while (id >= 0) {
        // wypisanie informacji o znaleziomy/wyznaczonym filmie
        cout << "[" << (id+1) << "]" << min2hhmm(filmy[id].start) << " " << min2hhmm(filmy[id].stop) << " " << min2hhmm(filmy[id].duration) << endl;
        // ustawienie statusy active na false wszystkim koliduj¹cym filmom
        colisions(filmy, n, id);
        // wyszukanie kolejnego filmu
        id = longest(filmy, n);
    }
}

// funkcja realizuj¹ca za³o¿enia strategii B

void strategiaB(Film filmy[], int n) {
    int id = shortest(filmy, n);
    while (id >= 0) {
        // wypisanie informacji o znaleziomy/wyznaczonym filmie
        cout << "[" << (id+1) << "]" << min2hhmm(filmy[id].start) << " " << min2hhmm(filmy[id].stop) << " " << min2hhmm(filmy[id].duration) << endl;
        // ustawienie statusy active na false wszystkim koliduj¹cym filmom
        colisions(filmy, n, id);
        // wyszukanie kolejnego filmu
        id = shortest(filmy, n);
    }
}

// funkcja realizuj¹ca za³o¿enia strategii C

void strategiaC(Film filmy[], int n) {
    int id = earliestStart(filmy, n);
    while (id >= 0) {
        cout << "[" << (id + 1) << "]" << min2hhmm(filmy[id].start) << " " << min2hhmm(filmy[id].stop) << " " << min2hhmm(filmy[id].duration) << endl;
        colisions(filmy, n, id);
        id = earliestStart(filmy, n);
    }
}

// funkcja realizuj¹ca za³o¿enia strategii D

void strategiaD(Film filmy[], int n) {
    int id = earliestStop(filmy, n);
    while (id >= 0) {
        cout << "[" << (id + 1) << "]" << min2hhmm(filmy[id].start) << " " << min2hhmm(filmy[id].stop) << " " << min2hhmm(filmy[id].duration) << endl;
        colisions(filmy, n, id);
        id = earliestStop(filmy, n);
    }
}

int main() {
    ifstream plik("dane.in");   // plik z danymi, by nie wpisyawaæ ich za ka¿dym razem
    if (plik.good()) {
        int n;
        plik >> n;                  // czytanie iloœci filmów
        int indNK = 0;              // zmienna wskazuj¹ca, któy fim okaza³ siê najkrótszy
        Film* filmy = new Film[n]; // stworzenie n-elementowej tablicy filmów 
        string stmp;                // zmienna pomocnicza by odczytaæ kolejne czasy jako stringi
        for (int i = 0; i < n; i++) {
            plik >> stmp;                       // odczyt czasu rozpoczêcia filmu (hh:mm) 
            filmy[i].start = hhmm2min(stmp);    // zmiana formatu czasu na minutowy i wstawienie go do tablicy dla i-tego filmu
            plik >> stmp;                       // odczyt czasu trwania filmu (hh:mm)
            filmy[i].duration = hhmm2min(stmp);// zmiana formatu czasu na minutowy i wstawienie go do tablicy dla i-tego filmu
            filmy[i].stop = filmy[i].start + filmy[i].duration; // wyznaczenie czasu zakoñczenia filmu
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
        cout << "coœ posz³o nie tak";
}

