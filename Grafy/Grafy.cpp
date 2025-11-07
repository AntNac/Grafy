#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <climits>
#include <cstring>

using namespace std;

class Queue {
private:
	int* tab;
	int size;
	int front, rear;

public:
	Queue(int n) : size(n), front(0), rear(0) {
		tab = new int[n];
	}

	void enqueue(int val) {
		tab[rear++] = val;
	}

	void dequeue() {
		front++;
	}

	int peek() {
		return tab[front];
	}

	bool isEmpty() {
		return front == rear;
	}

	Queue(const Queue& other) : size(other.size), front(other.front), rear(other.rear) {
		tab = new int[other.size];
		copy(other.tab, other.tab + other.size, tab);
	}

	Queue& operator=(const Queue& other) {
		if (this == &other) {
			return *this;
		}
		delete[] tab;
		size = other.size;
		front = other.front;
		rear = other.rear;
		tab = new int[other.size];
		copy(other.tab, other.tab + other.size, tab);
		return *this;
	}

	~Queue() {
		delete[] tab;
	}
};

class Wierzcholek {
	int stopien;
	int* sasiedzi;
	int kolor;

public:
	Wierzcholek() : stopien(0), sasiedzi(nullptr), kolor(0) {}

	explicit Wierzcholek(int s) : stopien(s), kolor(0) {
		sasiedzi = new int[s];
	}

	void DodajSasiada(int sasiad, int i) {
		sasiedzi[i] = sasiad;
	}

	int getStopien() const {
		return stopien;
	}

	int* getSasiedzi() const {
		return sasiedzi;
	}

	int getKolor() const {
		return kolor;
	}

	void setKolor(int barwa) {
		kolor = barwa;
	}

	Wierzcholek(const Wierzcholek& other) : stopien(other.stopien), kolor(other.kolor) {
		if (other.sasiedzi) {
			sasiedzi = new int[stopien];
			for (int i = 0; i < stopien; ++i) {
				sasiedzi[i] = other.sasiedzi[i];
			}
		}
		else {
			sasiedzi = nullptr;
		}
	}

	Wierzcholek& operator=(const Wierzcholek& other) {
		if (this != &other) {
			delete[] sasiedzi;

			stopien = other.stopien;
			kolor = other.kolor;

			if (other.sasiedzi) {
				sasiedzi = new int[stopien];
				for (int i = 0; i < stopien; ++i) {
					sasiedzi[i] = other.sasiedzi[i];
				}
			}
			else {
				sasiedzi = nullptr;
			}
		}
		return *this;
	}

	~Wierzcholek() {
		delete[] sasiedzi;
	}

};

class Graf {
public:
	int LiczbaWierzcholkow;
	int LiczbaKrawedzi;
	bool dwudzielnosc = true;
	int skladowaspojnosci = 0;
	Wierzcholek* wierzcholki;
	int* ciagStopniowy;
	int* posciagStopniowy;

	explicit Graf(int n) : LiczbaWierzcholkow(n), LiczbaKrawedzi(0) {
		wierzcholki = new Wierzcholek[LiczbaWierzcholkow];
		ciagStopniowy = new int[LiczbaWierzcholkow];
		posciagStopniowy = new int[LiczbaWierzcholkow];
	}

	Graf(const Graf& other) : LiczbaWierzcholkow(other.LiczbaWierzcholkow), LiczbaKrawedzi(other.LiczbaKrawedzi), dwudzielnosc(other.dwudzielnosc), skladowaspojnosci(other.skladowaspojnosci) {
		wierzcholki = new Wierzcholek[LiczbaWierzcholkow];
		ciagStopniowy = new int[LiczbaWierzcholkow];
		posciagStopniowy = new int[LiczbaWierzcholkow];
		for (int i = 0; i < LiczbaWierzcholkow; ++i) {
			wierzcholki[i] = other.wierzcholki[i];
			ciagStopniowy[i] = other.ciagStopniowy[i];
			posciagStopniowy[i] = other.ciagStopniowy[i];
		}
	}

	Graf& operator=(const Graf& other) {
		if (this != &other) {
			delete[] wierzcholki;
			delete[] ciagStopniowy;
			delete[] posciagStopniowy;
			LiczbaWierzcholkow = other.LiczbaWierzcholkow;
			LiczbaKrawedzi = other.LiczbaKrawedzi;
			dwudzielnosc = other.dwudzielnosc;
			skladowaspojnosci = other.skladowaspojnosci;

			wierzcholki = new Wierzcholek[LiczbaWierzcholkow];
			ciagStopniowy = new int[LiczbaWierzcholkow];
			posciagStopniowy = new int[LiczbaWierzcholkow];
			for (int i = 0; i < LiczbaWierzcholkow; ++i) {
				wierzcholki[i] = other.wierzcholki[i];
				ciagStopniowy[i] = other.ciagStopniowy[i];
				posciagStopniowy[i] = other.ciagStopniowy[i];
			}
		}
		return *this;
	}

	void DodajWierzcholek(const Wierzcholek& wierzcholek, int i) {
		wierzcholki[i] = wierzcholek;
	}

	void DodajKrawedzie(int k) {
		LiczbaKrawedzi += k;
	}

	Wierzcholek& getWierzcholek(int i) {
		return wierzcholki[i];
	}

	void quickSort(int* tab, int lewy, int prawy) {
		int i = lewy, j = prawy;
		int pivot = tab[(lewy + prawy) / 2];

		while (i <= j) {
			while (tab[i] > pivot) i++;
			while (tab[j] < pivot) j--;
			if (i <= j) {
				int tmp = tab[i];
				tab[i] = tab[j];
				tab[j] = tmp;
				i++;
				j--;
			}
		}
		if (lewy < j) quickSort(tab, lewy, j);
		if (i < prawy) quickSort(tab, i, prawy);
	}

	void static merge(int* tab, int* tab2, int lewy, int srodek, int prawy) {
		int n1 = srodek - lewy + 1;
		int n2 = prawy - srodek;

		int* L = new int[n1];
		int* R = new int[n2];
		int* L2 = new int[n1];
		int* R2 = new int[n2];

		for (int i = 0; i < n1; i++) {
			L[i] = tab[lewy + i];
			L2[i] = tab2[lewy + i];
		}
		for (int j = 0; j < n2; j++) {
			R[j] = tab[srodek + 1 + j];
			R2[j] = tab2[srodek + 1 + j];
		}

		int i = 0, j = 0, k = lewy;
		while (i < n1 && j < n2) {
			if (L[i] > R[j] || (L[i] == R[j] && L2[i] < R2[j])) {
				tab[k] = L[i];
				tab2[k] = L2[i];
				i++;
			}
			else {
				tab[k] = R[j];
				tab2[k] = R2[j];
				j++;
			}
			k++;
		}

		while (i < n1) {
			tab[k] = L[i];
			tab2[k] = L2[i];
			i++;
			k++;
		}

		while (j < n2) {
			tab[k] = R[j];
			tab2[k] = R2[j];
			j++;
			k++;
		}

		delete[] L;
		delete[] R;
		delete[] L2;
		delete[] R2;
	}

	void mergeSort(int* tab, int* tab2, int lewy, int prawy) {
		if (lewy < prawy) {
			int srodek = lewy + (prawy - lewy) / 2;
			mergeSort(tab, tab2, lewy, srodek);
			mergeSort(tab, tab2, srodek + 1, prawy);
			merge(tab, tab2, lewy, srodek, prawy);
		}
	}

	void bfs(int start, int* odleglosc, int& max, bool* odwiedzone) {
		memset(odwiedzone, 0, LiczbaWierzcholkow * sizeof(bool));
		Queue kolejka(LiczbaWierzcholkow);
		odwiedzone[start] = true;
		kolejka.enqueue(start);
		odleglosc[start] = 0;
		max = 0;
		int ileOdwiedzil = 1;

		while (!kolejka.isEmpty() && ileOdwiedzil < LiczbaWierzcholkow) {
			int obecny = kolejka.peek();
			kolejka.dequeue();
			for (int i = 0; i < wierzcholki[obecny].getStopien(); i++) {
				int sasiadObecnego = wierzcholki[obecny].getSasiedzi()[i];
				if (!odwiedzone[sasiadObecnego]) {
					odwiedzone[sasiadObecnego] = true;
					odleglosc[sasiadObecnego] = odleglosc[obecny] + 1;
					kolejka.enqueue(sasiadObecnego);
					ileOdwiedzil++;
					if (odleglosc[sasiadObecnego] > max)
						max = odleglosc[sasiadObecnego];
				}
			}
		}
	}

	void acentrycznosc() {
		int* ciag = new int[LiczbaWierzcholkow];
		int* odleglosc = new int[LiczbaWierzcholkow];
		bool* odwiedzone = new bool[LiczbaWierzcholkow];
		int max = 0;

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			if (wierzcholki[i].getStopien() == 0) {
				ciag[i] = 0;
				continue;
			}

			memset(odleglosc, 0, LiczbaWierzcholkow * sizeof(int));
			bfs(i, odleglosc, max, odwiedzone);

			ciag[i] = max;
		}

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			cout << ciag[i] << " ";
		}
		cout << endl;

		delete[] ciag;
		delete[] odleglosc;
		delete[] odwiedzone;
	}

	void koloruj(int nr, int kolor, bool* pokolorowane) {
		if (pokolorowane[nr]) {
			if (wierzcholki[nr].getKolor() != kolor) {
				dwudzielnosc = false;
			}
			return;
		}
		else {
			wierzcholki[nr].setKolor(kolor);
			pokolorowane[nr] = true;
		}

		for (int i = 0; i < wierzcholki[nr].getStopien(); ++i) {
			int sasiad = wierzcholki[nr].getSasiedzi()[i];
			koloruj(sasiad, 3 - kolor, pokolorowane);
		}
	}

	bool dwudzielnoscGrafu() {
		bool* pokolorowane = new bool[LiczbaWierzcholkow]();
		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			pokolorowane[i] = false;
		}
		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			if (dwudzielnosc && !pokolorowane[i]) {
				koloruj(i, 1, pokolorowane);
			}
		}
		delete[] pokolorowane;
		return dwudzielnosc;
	}

	void CiagStopniowy() {
		int* ciag = new int[LiczbaWierzcholkow];
		int* kolejnoscStopniowa = new int[LiczbaWierzcholkow];
		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			ciag[i] = ciagStopniowy[i];
			kolejnoscStopniowa[i] = i;
		}
		mergeSort(ciag, kolejnoscStopniowa, 0, LiczbaWierzcholkow - 1);

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			posciagStopniowy[i] = kolejnoscStopniowa[i];
			cout << ciag[i] << " ";
		}
		cout << endl;

		delete[] kolejnoscStopniowa;
		delete[] ciag;
	}

	void dfs(int nr, bool* odwiedzone, int& nieodwiedzone) { // Zmieniono na referencję
		odwiedzone[nr] = true;
		nieodwiedzone--;
		for (int i = 0; i < wierzcholki[nr].getStopien(); i++) {
			if (nieodwiedzone > 0 && !odwiedzone[wierzcholki[nr].getSasiedzi()[i]]) {
				dfs(wierzcholki[nr].getSasiedzi()[i], odwiedzone, nieodwiedzone);
			}
		}
	}

	void skladoweSpojnosci() {
		int liczba = 0;
		bool* odwiedzone = new bool[LiczbaWierzcholkow];
		int niodwiedzone = LiczbaWierzcholkow;
		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			odwiedzone[i] = false;
		}

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			if (niodwiedzone > 0) {
				if (!odwiedzone[i]) {
					dfs(i, odwiedzone, niodwiedzone);
					liczba++;
				}
			}
			else {
				break;
			}
		}
		delete[] odwiedzone;
		skladowaspojnosci = liczba;
		cout << liczba << endl;
	}

	void kolorowaniezachlanne() {
		int* kolory = new int[LiczbaWierzcholkow];
		bool* kolorUzyty = new bool[LiczbaWierzcholkow];

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			kolory[i] = -1;
			kolorUzyty[i] = false;
		}

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			kolorUzyty[0] = true;

			int najw = 1;
			for (int j = 0; j < wierzcholki[i].getStopien(); j++) {
				int sasiad = wierzcholki[i].getSasiedzi()[j];
				if (kolory[sasiad] != -1) {
					kolorUzyty[kolory[sasiad]] = true;
					if (kolory[sasiad] > najw) {
						najw = kolory[sasiad];
					}
				}
			}

			int kolordlawierz = 1;
			bool pierwszy = true;
			for (int j = 0; j <= najw + 1; j++) {
				if (!kolorUzyty[j] && pierwszy) {
					kolordlawierz = j;
					pierwszy = false;
				}
				kolorUzyty[j] = false;
			}

			kolory[i] = kolordlawierz;
		}

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			cout << kolory[i] << " ";
		}
		cout << endl;

		delete[] kolory;
		delete[] kolorUzyty;
	}

	void LF() {
		int* kolory = new int[LiczbaWierzcholkow];
		bool* kolorUzyty = new bool[LiczbaWierzcholkow];

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			kolory[i] = -1;
			kolorUzyty[i] = false;
		}

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			kolorUzyty[0] = true;

			int najw = 1;
			for (int j = 0; j < wierzcholki[posciagStopniowy[i]].getStopien(); j++) {
				int sasiad = wierzcholki[posciagStopniowy[i]].getSasiedzi()[j];
				if (kolory[sasiad] != -1) {
					kolorUzyty[kolory[sasiad]] = true;
					if (kolory[sasiad] > najw) {
						najw = kolory[sasiad];
					}
				}
			}

			int kolordlawierz = 1;
			bool pierwszy = true;
			for (int j = 0; j <= najw + 1; j++) {
				if (!kolorUzyty[j] && pierwszy) {
					kolordlawierz = j;
					pierwszy = false;
				}
				kolorUzyty[j] = false;
			}

			kolory[posciagStopniowy[i]] = kolordlawierz;

		}

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			cout << kolory[i] << " ";
		}
		cout << endl;

		delete[] kolory;
		delete[] kolorUzyty;
	}


	void SLF() {
		int* kolory = new int[LiczbaWierzcholkow];
		int* nasycenie = new int[LiczbaWierzcholkow];
		bool* unikatoweKolory = new bool[LiczbaWierzcholkow];

		memset(kolory, -1, LiczbaWierzcholkow * sizeof(int));
		memset(nasycenie, 0, LiczbaWierzcholkow * sizeof(int));
		// Alokacja pamięci dla unikatoweKolory - brakowało tego
		unikatoweKolory = new bool[LiczbaWierzcholkow + 2]; // Bezpieczny rozmiar

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			int obecny = -1;
			for (int j = 0; j < LiczbaWierzcholkow; j++) {
				if (kolory[j] == -1 && (obecny == -1 || nasycenie[j] > nasycenie[obecny] ||
					(nasycenie[j] == nasycenie[obecny] && ciagStopniowy[j] > ciagStopniowy[obecny]))) {
					obecny = j;
				}
			}

			if (obecny == -1) continue; // Na wszelki wypadek

			int obecnyStopien = wierzcholki[obecny].getStopien();

			int kolorObec = 1;
			bool found = false;
			while (!found) {
				found = true;
				for (int j = 0; j < obecnyStopien; j++) {
					int neighbor = wierzcholki[obecny].getSasiedzi()[j];
					if (kolory[neighbor] == kolorObec) {
						kolorObec++;
						found = false;
						break;
					}
				}
			}
			kolory[obecny] = kolorObec;

			for (int j = 0; j < obecnyStopien; j++) {
				int sasiad = wierzcholki[obecny].getSasiedzi()[j];
				if (kolory[sasiad] == -1) {
					int uniqueCount = 0;
					// Używamy LiczbaWierzcholkow jako bezpiecznego rozmiaru tablicy
					memset(unikatoweKolory, false, (LiczbaWierzcholkow + 2) * sizeof(bool));
					int stopienSasiada = wierzcholki[sasiad].getStopien();
					for (int k = 0; k < stopienSasiada; k++) {
						int neighborNeighbor = wierzcholki[sasiad].getSasiedzi()[k];
						if (kolory[neighborNeighbor] != -1 && !unikatoweKolory[kolory[neighborNeighbor]]) {
							unikatoweKolory[kolory[neighborNeighbor]] = true;
							uniqueCount++;
						}
					}
					nasycenie[sasiad] = uniqueCount; // +1 było błędem, samo znalezienie koloru jest nasyceniem
				}
			}
		}

		for (int i = 0; i < LiczbaWierzcholkow; i++) {
			cout << kolory[i] << " ";
		}
		cout << endl;

		delete[] kolory;
		delete[] nasycenie;
		delete[] unikatoweKolory;
	}


	void dopelnienieGrafu() const {
		long long liczbaWierzcholkowLL = static_cast<long long>(LiczbaWierzcholkow);
		long long liczbaKrawedziLL = static_cast<long long>(LiczbaKrawedzi);
		cout << (liczbaWierzcholkowLL * (liczbaWierzcholkowLL - 1) / 2) - liczbaKrawedziLL / 2 << endl;
	}

	~Graf() {
		delete[] wierzcholki;
		delete[] ciagStopniowy;
		delete[] posciagStopniowy;
	}
};

int main() {
	int iloscGrafow;
	cout << "Podaj ilosc grafow do przetworzenia: ";
	scanf("%d", &iloscGrafow);

	for (int i = 0; i < iloscGrafow; i++) {
		int ileLinijek;
		cout << "\n===== GRAF " << (i + 1) << " =====" << endl;
		cout << "Podaj liczbe wierzcholkow: ";
		scanf("%d", &ileLinijek);
		Graf graf(ileLinijek);

		cout << "Wprowadz " << ileLinijek << " linii (format: <stopien> <sasiad1> <sasiad2> ...):" << endl;

		for (int j = 0; j < ileLinijek; j++) {
			int s;
			scanf("%d", &s);
			Wierzcholek w(s);
			graf.ciagStopniowy[j] = s;
			graf.DodajWierzcholek(w, j);
			graf.DodajKrawedzie(s);
			for (int k = 0; k < s; k++) {
				int l;
				scanf("%d", &l);
				graf.getWierzcholek(j).DodajSasiada(l - 1, k);
			}
		}

		cout << "\n--- WYNIKI DLA GRAFU " << (i + 1) << " ---" << endl;

		cout << "Ciag stopniowy (posortowany): ";
		graf.CiagStopniowy();

		cout << "Liczba skladowych spojnosci: ";
		graf.skladoweSpojnosci();

		cout << "Czy graf jest dwudzielny (T/F): ";
		if (graf.dwudzielnoscGrafu()) {
			cout << "T" << endl;
		}
		else {
			cout << "F" << endl;
		}

		cout << "Acentrycznosc wierzcholkow: ";
		graf.acentrycznosc();

		cout << "Kolorowanie zachlanne (kolejnosc 1..N): ";
		graf.kolorowaniezachlanne();

		cout << "Kolorowanie LF (Largest First): ";
		graf.LF();

		cout << "Kolorowanie SLF (Saturation Largest First): ";
		graf.SLF();

		cout << "Liczba krawedzi w dopelnieniu grafu: ";
		graf.dopelnienieGrafu();
	}
	return 0;
}