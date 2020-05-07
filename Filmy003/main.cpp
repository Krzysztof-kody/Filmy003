#include <iostream>
#include <string>

using namespace std;


    int hhmm2min(string hhmm) {
        int min = 0;
        min = (hhmm[0] - '0') * 10 + (hhmm[1] - '0');
        min *= 60;
        min += (hhmm[3] - '0') * 10 + (hhmm[4] - '0');
        return min;
    }

    string min2hhmm(int min) {
        string hhmm = "";
        int tmp;
        min = min % (24 * 60); //1440
        tmp = min / 600;
        hhmm += tmp + '0';
        min -= tmp * 600;
        tmp = min / 60;
        hhmm += tmp + '0';
        min -= tmp * 60;
        hhmm += ':';
        hhmm += min / 10 + '0';
        hhmm += min % 10 + '0';
        return hhmm;
    }

    int main() {

        cout << "Podaj ilo�� film�w: ";
        int n ;
        cin >> n;
        int czasNK = 1440;
        int indNK = 0;

        for (int i = 1; i <= n; i++) {
            cout << "Podaj czas rozpocz�cia filmu (hh:mm): ";
            string czas;
            cin >> czas;
            
            cout << "Podaj d�ugo�� filmu (hh:mm): ";
            string dlugosc;
            cin >> dlugosc;
      
            if (hhmm2min(dlugosc) < czasNK) {
                czasNK = hhmm2min(dlugosc);
                indNK = i;
            }
            int tmp = hhmm2min(czas) + hhmm2min(dlugosc);
            cout << "Czas zako�czenia filmu: " << min2hhmm(tmp) <<endl;
        }
        cout << "Najkr�tszy[" << indNK << "]: " << czasNK << " (" << min2hhmm(czasNK) << ")";
    }

