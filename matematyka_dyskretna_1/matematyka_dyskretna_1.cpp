#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <string>
#include <cctype>
#include <cmath>

#define DBG 0

using namespace std;

const string nazwa_pliku = "badanie_zdań_logicznych.txt";
std::string wejscie;
int tryb;
bool** tabelaprawdy;
short wym_W, wym_K,przypadek = 10,rodz_litera,ilosc_P, ilosc_Q, ilosc_R, varsCount;
bool tryb_zaawansowany = false;

int calculateDimention()
{
	return static_cast<int>(pow(2.0, double(varsCount)));
}

int calculateCase()
{
	if (varsCount == 1)
		return 0;
	if (varsCount == 3)
		return 10;
	if (ilosc_P && ilosc_Q)
		return 1;
	if (ilosc_P && ilosc_R)
		return 2;
	if (ilosc_Q && ilosc_R)
		return 3;
}

bool pobierz_dane_plik(void)
{
	std::fstream plik(nazwa_pliku, ios::in);
	if (!plik.good())
	{
		return false;
	}

	getline(plik, wejscie);
	plik >> tryb_zaawansowany;

	return true;
}

void utworz_plik(void)
{
	fstream plik_wejsciowy(nazwa_pliku,ios::out);
	plik_wejsciowy << "pcq" << endl << "0" <<endl;
	plik_wejsciowy << "==============================================================================================================" << endl;
	plik_wejsciowy << "Instrukcja wprowadzania danych do porgramu:" << endl;
	plik_wejsciowy << "   W pierwszej linijce należy wprwadzić zdanie logiczne zgodnie z konwencją (np. pCq) ,wielkosc liter nie ma znaczneia "<< endl;
	plik_wejsciowy << "   Czyli wymagana jest konstrukcja wprowadzonego zdania zmienna-dzialanie-zmienna-dzialanie-zmienna ..." << endl;
	plik_wejsciowy << "   Każda zmienna może być poprzedzona negacją (np. NpCNq) . Program wykonuje działania w kolejności od lewej do prawej. " << endl;
	plik_wejsciowy << "			LEGENDA działań: " << endl;
	plik_wejsciowy << "   N (negation - negacja)" << endl;
	plik_wejsciowy << "   D (disjunction - alternatywa) " << endl;
	plik_wejsciowy << "   C (conjunction – koniunkcja)" << endl;
	plik_wejsciowy << "   I (implication – implikacja)" << endl;
	plik_wejsciowy << "   E (equivalence – równoważnośd)" << endl << endl;
	plik_wejsciowy << "W drugiej linijce liczba decyduje o włączeniu trybu zawansowanego, powoduje to wyswietlenie tabeli prawdy" << endl;
	plik_wejsciowy << "   0 (wyłączony tryb zaawansowany - domyślny)" << endl;
	plik_wejsciowy << "   1 (włączony tryb zaawansowany)" << endl;
	plik_wejsciowy.close();
	return;
}

void podglad_tabeli_dbg(bool** &tabelaprawdy)
{
	cout << endl << endl;
	if (wym_W == 2) { cout << "P  "<<endl; }
	else if (wym_W == 8) { cout << "P  Q  R" << endl; }
	else
	{
		if (przypadek == 1) { cout << "P  Q" << endl; }
		else if (przypadek == 2) { cout << "P  R" << endl; }
		else if (przypadek == 3) { cout << "Q  R" << endl; }
	}
	for (int i = 0; i < wym_W; i++)
	{
		for (int k = 0; k < wym_K; k++)
		{
			cout << tabelaprawdy[i][k] << "  ";
		}
		cout << endl;
	}
	cout << endl << endl;
}

inline void negacja(short &a,int &przebieg,short &przy,int &litera, bool** &tabelaprawdy) //liczba wierszy,ktore dzialanie z kolei, przypadek dla 2 litera, odniesienie do litery
{
	if (a == 2) {
		tabelaprawdy[0][przebieg] = !tabelaprawdy[0][0];
		tabelaprawdy[1][przebieg] = !tabelaprawdy[1][0];
	}
	else if (a == 4) {
		if (przy == 1 || przy == 2) { // PQ , PR
			if (litera == 80 || litera == 112) {
				for (short i = 0; i < 4; i++) tabelaprawdy[i][przebieg] = !tabelaprawdy[i][0];
			}
			else {
				for (short i = 0; i < 4; i++) tabelaprawdy[i][przebieg] = !tabelaprawdy[i][1];
			}
		}
		if (przy == 3) { // QR
			if (litera == 81 || litera == 113) {
				for (short i = 0; i < 4; i++) tabelaprawdy[i][przebieg] = !tabelaprawdy[i][0];
			}
			else {
				for (short i = 0; i < 4; i++) tabelaprawdy[i][przebieg] = !tabelaprawdy[i][1];
			}
		}
	}
	else {
		if (litera == 80 || litera == 112) {//P
			for (short i = 0; i < 8; i++) tabelaprawdy[i][przebieg] = !tabelaprawdy[i][0];
		}
		else if (litera == 81 || litera == 113) {//Q
			for (short i = 0; i < 8; i++) tabelaprawdy[i][przebieg] = !tabelaprawdy[i][1];
		}
		else {//R
			for (short i = 0; i < 8; i++) tabelaprawdy[i][przebieg] = !tabelaprawdy[i][2];
		}
	}
	przebieg += 1;
}

bool** utworz_tablice() //tworzy dynamiczna tablice o odpowiednym wymiarze, uzupelniona
{
	int aski;
	short ilosc_dzialan = 0;

	for (auto character : wejscie)
	{
		switch (std::tolower(character))
		{
			case 'p':
				if (!ilosc_P)
				{
					varsCount++;
				}
				ilosc_P++;
				break;
			case 'q':
				if (!ilosc_Q)
				{
					varsCount++;
				}
				ilosc_Q++;
				break;
			case 'r':
				if (!ilosc_R)
				{
					varsCount++;
				}
				ilosc_R++;
				break;
			case 'n': case 'd': case 'c': case 'i': case 'e':
				ilosc_dzialan++;
				break;
		}
	}

	if (ilosc_dzialan == 0) { return NULL; }

#if DBG == 1
	cout << ilosc_dzialan << endl;
#endif

	wym_W = calculateDimention();
	przypadek = calculateCase();

	wym_K = ilosc_dzialan + varsCount;

	if (wejscie[0] != 'n' && wejscie[0] != 'N') { wym_K += 1; }

	bool** tabelaprawdy;
	tabelaprawdy = new bool*[wym_K];
	for (int i = 0; i < wym_W; i++)  tabelaprawdy[i] = new bool[wym_W]; 

	if (wym_W == 2) {
		tabelaprawdy[0][0] = 1; tabelaprawdy[1][0] = 0; rodz_litera = 0;
	}
	else if (wym_W == 4) {
		tabelaprawdy[0][0] = 0; tabelaprawdy[1][0] = 0; tabelaprawdy[2][0] = 1; tabelaprawdy[3][0] = 1;
		tabelaprawdy[0][1] = 0; tabelaprawdy[1][1] = 1; tabelaprawdy[2][1] = 0; tabelaprawdy[3][1] = 1;
	}
	else {
		tabelaprawdy[0][0] = 0; tabelaprawdy[1][0] = 0; tabelaprawdy[2][0] = 1; tabelaprawdy[3][0] = 1; tabelaprawdy[4][0] = 0; tabelaprawdy[5][0] = 0; tabelaprawdy[6][0] = 1; tabelaprawdy[7][0] = 1;
		tabelaprawdy[0][1] = 0; tabelaprawdy[1][1] = 1; tabelaprawdy[2][1] = 0; tabelaprawdy[3][1] = 1; tabelaprawdy[4][1] = 0; tabelaprawdy[5][1] = 1; tabelaprawdy[6][1] = 0; tabelaprawdy[7][1] = 1;
		tabelaprawdy[0][2] = 0; tabelaprawdy[1][2] = 0; tabelaprawdy[2][2] = 0; tabelaprawdy[3][2] = 0; tabelaprawdy[4][2] = 1; tabelaprawdy[5][2] = 1; tabelaprawdy[6][2] = 1; tabelaprawdy[7][2] = 1;
	}

	return tabelaprawdy;
}

bool rachunek_logiczny(bool** &tabelaprawdy)
{
	short rodz_litera = 10,litera = 0;
	int aski1, askiL, askiP;
	if (wejscie.length() < 1) { return false; }

	int przebieg = !!ilosc_P + !!ilosc_Q + !!ilosc_R;

	if (wejscie[0] != 78 && wejscie[0] != 110) // N
	{ 
		przebieg += 1;
		if (przypadek == 0) { for (short j = 0; j < wym_W; j++) tabelaprawdy[j][1] = tabelaprawdy[j][0]; }
		else if (przypadek == 1 || przypadek == 2) {
			if (wejscie[0] == 80 || wejscie[0] == 112) { for (short j = 0; j < wym_W; j++) tabelaprawdy[j][2] = tabelaprawdy[j][0]; }
			else { for (short j = 0; j < wym_W; j++) tabelaprawdy[j][2] = tabelaprawdy[j][1]; }
		}
		else if (przypadek == 3)
		{
			if (wejscie[0] == 81 || wejscie[0] == 113) { for (short j = 0; j < wym_W; j++) tabelaprawdy[j][2] = tabelaprawdy[j][0]; }
			else { for (short j = 0; j < wym_W; j++) tabelaprawdy[j][2] = tabelaprawdy[j][1]; }
		}
		else
		{
			if (wejscie[0] == 80 || wejscie[0] == 112) { for (short j = 0; j < wym_W; j++) tabelaprawdy[j][3] = tabelaprawdy[j][0]; }
			else if (wejscie[0] == 81 || wejscie[0] == 113) { for (short j = 0; j < wym_W; j++) tabelaprawdy[j][3] = tabelaprawdy[j][1]; }
			else { for (short j = 0; j < wym_W; j++) tabelaprawdy[j][3] = tabelaprawdy[j][2]; }
		}
	}

#if DBG==1 
	podglad_tabeli_dbg(tabelaprawdy);
#endif

	for (int i = 0; i < wejscie.length()-1; i++)
	{
		aski1 = wejscie[i];

		switch (aski1)
		{
		case 78:
		case 110: //N ,~ - negacja
			askiP = wejscie[i + 1];
			negacja(wym_W,przebieg,przypadek, askiP,tabelaprawdy);
			break;
		case 68:
		case 100: //D ,v - alternatywa
			if (i == 0) { askiL = wejscie[i - 1]; }
			else { askiL = 0; }
			askiP = wejscie[i + 1];
			
			if (askiP == 78 || askiP == 110) {
				negacja(wym_W,przebieg,przypadek, askiP,tabelaprawdy);
				for (short m = 0; m < wym_W; m++)
				{
					if ((tabelaprawdy[m][przebieg - 2] == 0) && (tabelaprawdy[m][przebieg - 1] == 0)) { tabelaprawdy[m][przebieg] = 0; }
					else { tabelaprawdy[m][przebieg] = 1; }
				}
				i++;
			}
			else
			{
				if (rodz_litera != 0) 
				{
					if (przypadek == 1 || przypadek == 2) {
						if (askiP == 80 || askiP == 112) { litera = 0; }
						else { litera = 1; }
					}
					else if (przypadek == 3)
					{
						if (askiP == 81 || askiP == 113) { litera = 0; }
						else { litera = 1; }
					}
					else 
					{
						if (askiP == 820 || askiP == 112) { litera = 0; }
						else if (askiP == 81 || askiP == 113) { litera = 1; }
						else { litera = 2; }
					}
				}
				for (short j = 0; j < wym_W; j++)
				{
					if ((tabelaprawdy[j][przebieg - 1] == 0) && (tabelaprawdy[j][litera] == 0)) { tabelaprawdy[j][przebieg] = 0; }
					else { tabelaprawdy[j][przebieg] = 1; }
				}
			}
			
			przebieg += 1;
			break;
		case 67:
		case 99: //C ,^ - koniukcja
			if (i == 0) { askiL = wejscie[i - 1]; }
			else { askiL = 0; }
			askiP = wejscie[i + 1];

			if (askiP == 78 || askiP == 110) {
				negacja(wym_W, przebieg, przypadek, askiP, tabelaprawdy);
				for (short m = 0; m < wym_W; m++)
				{
					if (tabelaprawdy[m][przebieg - 2] == 1 && tabelaprawdy[m][przebieg - 1] == 1) { tabelaprawdy[m][przebieg] = 1; }
					else { tabelaprawdy[m][przebieg] = 0; }
				}
				i++;
			}
			else
			{
				if (rodz_litera != 0) {
					if (przypadek == 1 || przypadek == 2) {
						if (askiP == 80 || askiP == 112) { litera = 0; }
						else { litera = 1; }
					}
					else if (przypadek == 3)
					{
						if (askiP == 81 || askiP == 113) { litera = 0; }
						else { litera = 1; }
					}
					else
					{
						if (askiP == 80 || askiP == 112) { litera = 0; }
						else if (askiP == 81 || askiP == 113) { litera = 1; }
						else { litera = 2; }
					}
				}
				for (short j = 0; j < wym_W; j++)
				{
					if (tabelaprawdy[j][przebieg - 1] == 1 && tabelaprawdy[j][litera] == 1) { tabelaprawdy[j][przebieg] = 1; }
					else { tabelaprawdy[j][przebieg] = 0; }
				}
			}
			przebieg += 1;
			break;
		case 73:
		case 105: //I ,=> - implikacja
			if (i == 0) { askiL = wejscie[i - 1]; }
			else { askiL = 0; }
			askiP = wejscie[i + 1];


			if (askiP == 78 || askiP == 110) {
				negacja(wym_W, przebieg, przypadek, askiP, tabelaprawdy);
				for (short m = 0; m < wym_W; m++)
				{
					if (tabelaprawdy[m][przebieg - 2] == 1 && tabelaprawdy[m][przebieg - 1] == 0) { tabelaprawdy[m][przebieg] = 0; }
					else { tabelaprawdy[m][przebieg] = 1; }
				}
				i++;
			}
			else
			{
				if (rodz_litera != 0)
				{
					if (przypadek == 1 || przypadek == 2) {
						if (askiP == 80 || askiP == 112) { litera = 0; }
						else { litera = 1; }
					}
					else if (przypadek == 3)
					{
						if (askiP == 81 || askiP == 113) { litera = 0; }
						else { litera = 1; }
					}
					else
					{
						if (askiP == 80 || askiP == 112) { litera = 0; }
						else if (askiP == 81 || askiP == 113) { litera = 1; }
						else { litera = 2; }
					}
				}
				for (short j = 0; j < wym_W; j++)
				{
					if (tabelaprawdy[j][przebieg - 1] == 1 && tabelaprawdy[j][litera] == 0) { tabelaprawdy[j][przebieg] = 0; }
					else { tabelaprawdy[j][przebieg] = 1; }
				}
			}

			przebieg += 1;
			break;
		case 69:
		case 101: //E, <=> - równoważnosc
			if (i == 0) { askiL = wejscie[i - 1]; }
			else { askiL = 0; }
			askiP = wejscie[i + 1];

			if (askiP == 78 || askiP == 110) {
				negacja(wym_W, przebieg, przypadek, askiP, tabelaprawdy);
				for (short m = 0; m < wym_W; m++)
				{
					if ((tabelaprawdy[m][przebieg - 2] == 1 && tabelaprawdy[m][przebieg - 1] == 1) || (tabelaprawdy[m][przebieg - 2] == 0 && tabelaprawdy[m][przebieg - 1] == 0)) { tabelaprawdy[m][przebieg] = 1; }
					else { tabelaprawdy[m][przebieg] = 0; }
				}
				i++;
			}
			else
			{
				if (rodz_litera != 0) {
					if (przypadek == 1 || przypadek == 2) {
						if (askiP == 80 || askiP == 112) { litera = 0; }
						else { litera = 1; }
					}
					else if (przypadek == 3)
					{
						if (askiP == 81 || askiP == 113) { litera = 0; }
						else { litera = 1; }
					}
					else
					{
						if (askiP == 80 || askiP == 112) { litera = 0; }
						else if (askiP == 81 || askiP == 113) { litera = 1; }
						else { litera = 2; }
					}
				}
				for (short j = 0; j < wym_W; j++)
				{
					if ((tabelaprawdy[j][przebieg - 1] == 0 && tabelaprawdy[j][litera] == 0) || (tabelaprawdy[j][przebieg - 1] == 1 && tabelaprawdy[j][litera] == 1)) { tabelaprawdy[j][przebieg] = 1; }
					else { tabelaprawdy[j][przebieg] = 0; }
				}
			}
			przebieg += 1;
			break;
		default:
			break;
		}

#if DBG==1 
		if (i%2 == 1)podglad_tabeli_dbg(tabelaprawdy);
#endif
	}

	short ilosc_zmiennych = !!ilosc_P + !!ilosc_Q + !!ilosc_R;
	for (short j = 0; j < wym_W; j++)
	{
		if (tabelaprawdy[j][wym_K - 1] == 0) { break; }
		if (j == wym_W - 1) { return true; }
	}
	return false;
}

int main()
{
	setlocale(LC_CTYPE, ".1250");

	if (pobierz_dane_plik() == false) 
	{ 
		utworz_plik();
		return 0;
	}

#if DBG == 1
	cout << wejscie << endl;
#endif

	bool** temp = utworz_tablice();

	if (tryb_zaawansowany) { podglad_tabeli_dbg(temp); }


	if (rachunek_logiczny(temp) == true) 
	{
		cout << endl;
		cout << " Podane zdanie logiczne jest tautologią !" << endl;
		cout << endl;
	}
	else
	{
		cout << "Podane zdanie logiczne nie jest tatuologią dla: " << endl;
		for (short j = 0; j < wym_W; j++)
		{
			if (temp[j][wym_K - 1] == 0)
			{
				if (wym_W == 2) { cout << "P= " << temp[j][0] << endl; }
				else if (wym_W == 4) 
				{ 
					if (przypadek == 1) { cout << "P= " << temp[j][0] << ", Q= " << temp[j][1] << endl; }
					else if (przypadek == 2) { cout << "P= " << temp[j][0] << ", R= " << temp[j][1] << endl; }
					else { cout << "Q= " << temp[j][0] << ", R= " << temp[j][1] << endl; }
				}
				else { cout << "P= " << temp[j][0] << ", Q= " << temp[j][1] << ", R= " << temp[j][2] << endl; }
			}
		}
	}

	if (tryb_zaawansowany) { podglad_tabeli_dbg(temp); }

	for (int i = 0; i < wym_W; i++) delete[] temp[i];

	if (tryb_zaawansowany) { podglad_tabeli_dbg(temp); }

	delete[] temp;

	cout << "Program autorstwa Grzegorz Szwarc" << endl;
	system("pause");
	return 0;
}
