#include <iostream>
#include <string>

using namespace std;

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

int main() {

    cout << "Podaj iloœæ filmów: ";
    int n;
    cin >> n;
    int czasNK = 1440;      // zmienna potrzebna do wyznaczenia najkrótszego filmu. Przyj¹³em, ¿e podane filmy bêd¹ krótsze ni¿ 24h
    int indNK = 0;          // zmienna wskazuj¹ca, któy fim okaza³ siê najkrótszy

    for (int i = 1; i <= n; i++) {
        cout << "Podaj czas rozpoczêcia filmu (hh:mm): ";
        string czas;
        cin >> czas;

        cout << "Podaj d³ugoœæ filmu (hh:mm): ";
        string dlugosc;
        cin >> dlugosc;

        if (hhmm2min(dlugosc) < czasNK) {   // sprawdzenie czy podana w³aœnie d³ugoœæ filmu jest mniejsza ni¿ aktualnie najkrótszego
            czasNK = hhmm2min(dlugosc);     // ustalenie aktalnie najmniejszej d³ugoœci filmu
            indNK = i;                      // zapamiêtanie, który film okaza³ siê aktualnie najkrótszy
        }
        int tmp = hhmm2min(czas) + hhmm2min(dlugosc);
        cout << "Czas zakoñczenia filmu: " << min2hhmm(tmp) << endl;
    }
    cout << "Najkrótszy[" << indNK << "]: " << czasNK << " (" << min2hhmm(czasNK) << ")";
}

