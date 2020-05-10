#include <iostream>
#include <fstream>

using namespace std;

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

int shortest(Film filmy[], int n) {
    int id = -1;
    int min = INT_FAST32_MAX;
    bool czy = false;

    for (int i = 0; i < n; i++) {
        czy = false;
        if (filmy[i].duration < min) czy = true;
        if (filmy[i].duration == min && filmy[i].stop < filmy[id].stop) czy = true;

        if (czy == true) {
            min = filmy[i].duration;
            id = i;
        }
    }
    return id;
}

int longest(Film filmy[], int n) {
    int id = -1;
    int max = -1;
    bool czy = false;

    for (int i = 0; i < n; i++) {
        czy = false;
        if (filmy[i].duration > max) czy = true;
        if (filmy[i].duration == max && filmy[i].stop < filmy[id].stop) czy = true;

        if (czy == true) {
            max = filmy[i].duration;
            id = i;
        }
    }
    return id;
}

int earliestStart(Film filmy[], int n) {
    int id = -1;
    int min = INT_FAST32_MAX;
    bool czy = false;

    for (int i = 0; i < n; i++) {
        czy = false;
        if (filmy[i].start < min) czy = true;
        if (filmy[i].start == min && filmy[i].stop < filmy[id].stop) czy = true;

        if (czy == true) {
            min = filmy[i].duration;
            id = i;
        }
    }
    return id;
}

int earliestStop(Film filmy[], int n) {
    int id = -1;
    int min = INT_FAST32_MAX;
    bool czy = false;

    for (int i = 0; i < n; i++) {
        czy = false;
        if (filmy[i].stop < min) czy = true;
        if (filmy[i].stop == min && filmy[i].start > filmy[id].start) czy = true;

        if (czy == true) {
            min = filmy[i].duration;
            id = i;
        }
    }
    return id;
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

        indNK = shortest(filmy, n);
        cout << "Najkrotszy[" << indNK << "]: " << filmy[indNK].duration << " (" << min2hhmm(filmy[indNK].duration) << ")" << endl;
        indNK = longest(filmy, n);
        cout << "Najdluzszy[" << indNK << "]: " << filmy[indNK].duration << " (" << min2hhmm(filmy[indNK].duration) << ")" << endl;
        indNK = earliestStart(filmy, n);
        cout << "NajwczesniejZaczyna[" << indNK << "]: " << filmy[indNK].start << " (" << min2hhmm(filmy[indNK].start) << ")" << endl;
        indNK = earliestStop(filmy, n);
        cout << "NajwczesniejKonczy[" << indNK << "]: " << filmy[indNK].start << " (" << min2hhmm(filmy[indNK].start) << ")" << endl;
    }
    else
        cout << "co� posz�o nie tak";
}

