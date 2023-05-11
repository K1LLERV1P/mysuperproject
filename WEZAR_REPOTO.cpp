#define CURL_STATICLIB
#define UP 72
#define DOWN 80
#define ENTER 13

#include "include.h"
#include "functions.h"

static size_t getResponsetoString(void* contents, size_t size, size_t nmemb, void* userp) {
	((string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE); //дискриптор консоли

inline void consolXY(short x, short y)
{
	SetConsoleCursorPosition(hstdout, { x, y }); //перемещение курсора в другую точку
}

int menu()
{
	string menuSTR[] = { "Средняя погода на данный момент", "Средняя погода на завтра", "Выбрать дату" };
	int activeThemeMenu = 0;

	char ch;
	while (true) {
		int x = 38, y = 10;
		consolXY(x, y);
		for (int i = 0; i < size(menuSTR); i++) {
			if (i == activeThemeMenu) SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); //смена цвета
			else SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

			consolXY(x, y++);
			cout << "-----------------------------------";
			consolXY(x, y++);
			cout << "| " << setw(31) << left << menuSTR[i] << " |";
			consolXY(x, y++);
			cout << "-----------------------------------";
		}

		ch = _getch();
		if (ch == -32) ch = _getch(); //стрелочки

		switch (ch)
		{
		case UP:
			if (activeThemeMenu > 0) activeThemeMenu--;
			break;
		case DOWN:
			if (activeThemeMenu < size(menuSTR) - 1) activeThemeMenu++;	
			break;
		case ENTER:
			system("CLS");
			consolXY(x, y);
			SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN);
			switch (activeThemeMenu + 1)
			{
			case 1:
				return 1;
			case 2:
				return 2;
			case 3:
				return 3;
			}
			break;
		case 27:
			exit(0);
		}
	}
}

string curl_request_gis(string town)
{
	string town_gis;
	town_gis = "https://www.gismeteo.com/search/" + town + "/";

	CURL* curl2;
	CURLcode response2;
	string request_gis;

	curl2 = curl_easy_init();
	curl_easy_setopt(curl2, CURLOPT_URL, town_gis.c_str());
	curl_easy_setopt(curl2, CURLOPT_WRITEFUNCTION, getResponsetoString);
	curl_easy_setopt(curl2, CURLOPT_WRITEDATA, &request_gis);
	response2 = curl_easy_perform(curl2);
	curl_easy_cleanup(curl2);

	return request_gis;
}

string curl_request_gis2(string town, string town_gis_redirect)
{
	CURL* curl;
	CURLcode response;
	string request_gis2;

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, town_gis_redirect.c_str()); //"https://www.gismeteo.ru/weather-sochi-5233/tomorrow/"
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &request_gis2);
	response = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	return request_gis2;
}

string curl_request_wor(string town, int menuN)
{
	string town_wor;
	if (menuN == 3) {
		town_wor = "https://world-weather.ru/pogoda/russia/" + town + "/month/";
	}
	else { town_wor = "https://world-weather.ru/pogoda/russia/" + town + "/"; }

	CURL* curl3;
	CURLcode response3;
	string request_wor;

	curl3 = curl_easy_init();
	curl_easy_setopt(curl3, CURLOPT_URL, town_wor.c_str());
	curl_easy_setopt(curl3, CURLOPT_WRITEFUNCTION, getResponsetoString);
	curl_easy_setopt(curl3, CURLOPT_WRITEDATA, &request_wor);
	response3 = curl_easy_perform(curl3);
	curl_easy_cleanup(curl3);

	return request_wor;
}

string curl_request_mail(string town, int now, int date) 
{
	string town_mail;
	if (now == 1) {
		town_mail = "https://pogoda.mail.ru/prognoz/" + town + "/";
	}
	else if (now == 2) {
		town_mail = "https://pogoda.mail.ru/prognoz/" + town + "/" + to_string(date) + "-may/";
	}
	else {
		town_mail = "https://pogoda.mail.ru/prognoz/" + town + "/3dnya/";
	}
	
	//cout << town_mail << endl;

	CURL* curl4;
	CURLcode response4;
	string request_mail;

	curl4 = curl_easy_init();
	curl_easy_setopt(curl4, CURLOPT_URL, town_mail.c_str());
	curl_easy_setopt(curl4, CURLOPT_WRITEFUNCTION, getResponsetoString);
	curl_easy_setopt(curl4, CURLOPT_WRITEDATA, &request_mail);
	response4 = curl_easy_perform(curl4);
	curl_easy_cleanup(curl4);

	return request_mail;
}

int WEZAR() {
	string wr;
	cin >> wr;
	LPCWSTR saund = L"везер-репорт.wav";
	//string saund = L"C:\\Users\\Давид\\source\\repos\\ConsoleApplication1\\ConsoleApplication1\\везер-репорт.wav";
	if (wr == "WEZAR_REPOTO") { PlaySound(saund, NULL, SND_FILENAME); }
	else return 0;
}

int main()
{
	setlocale(0, "Rus");

	if (WEZAR() == 0) return 0;

	int menuN = menu();

	system("CLS");

	cout << "Введите город: ";
	string town;
	cin >> town;
	cout << endl;

	if (menuN == 1) {
		int date = 0;
		string str_redirect;
		str_redirect = curl_request_gis(town);

		int Find_Gis2_loc;
		int Find_Gis2;
		Find_Gis2_loc = str_redirect.find(">Localities");
		//cout << Find_Gis2_loc << endl;
		Find_Gis2 = str_redirect.find("catalog-item-link", Find_Gis2_loc + 15);
		//cout << Find_Gis2 << endl << endl;
		//cout << str_redirect[Find_Gis2 + 28] << str_redirect[Find_Gis2 + 29] << str_redirect[Find_Gis2 + 30] << str_redirect[Find_Gis2 + 31] << str_redirect[Find_Gis2 + 32] << str_redirect[Find_Gis2 + 33] << endl;
		//cout << str_redirect[Find_Gis2 + 37] << str_redirect[Find_Gis2 + 38] << str_redirect[Find_Gis2 + 39] << str_redirect[Find_Gis2 + 40] << str_redirect[Find_Gis2 + 41] << str_redirect[Find_Gis2 + 42] << endl;
		int k; k = 0;
		int i; i = 29;
		string town_gis_redirect = "https://www.gismeteo.ru/";
		while (k < 1) {
			if (str_redirect[Find_Gis2 + i] == '/') {
				k++;
			}
			else {
				town_gis_redirect = town_gis_redirect + str_redirect[Find_Gis2 + i];
				i++;
			}
		}
		town_gis_redirect = town_gis_redirect + "/";
		//cout << town_gis_redirect;
		cout << endl << endl;

		string str_response;
		str_response = curl_request_gis2(town, town_gis_redirect);

		int Find_Gis;
		int Find_Gis_N;
		int Find_Gis_N2;
		int Find_Giss;
		int Find_Gisss;

		Find_Gis_N = str_response.find("Сейчас");
		//cout << "date date  --  " << Find_Gis_D << endl;
		Find_Gis_N2 = str_response.find("temperature_c", Find_Gis_N + 15);
		Find_Gis = str_response.find("</span>", Find_Gis_N2 + 13);
		//Find_Giss = str_response.find("style=\"top:", Find_Gis + 13); //unit unit_temperature_c
		//Find_Gisss = str_response.find("_c", Find_Giss + 13);
		//cout << Find_Gisss << endl << endl;
		//cout << str_response[Find_Gis+5] << str_response[Find_Gis + 6] << str_response[Find_Gis + 7] << str_response[Find_Gis + 8] << str_response[Find_Gis + 9] << str_response[Find_Gis + 10] << str_response[Find_Gis + 11] << str_response[Find_Gis + 12] << str_response[Find_Gis + 13];
		
		string res_gis;
		if (str_response[Find_Gis + 7] == '&') {
			cout << '-' << str_response[Find_Gis + 13];
			res_gis = res_gis + "-" + str_response[Find_Gis + 13];
			if (str_response[Find_Gis + 14] == '<') {}
			else {
				cout << str_response[Find_Gis + 14];
				res_gis = res_gis + str_response[Find_Gis + 14];
			}
		}
		else if (str_response[Find_Gis + 6] == '>') {
			cout << '+' << str_response[Find_Gis + 7];  res_gis = res_gis + '+' + str_response[Find_Gis + 7];
			if (str_response[Find_Gis + 8] == '<') {}
			else { cout << str_response[Find_Gis + 8]; res_gis = res_gis + str_response[Find_Gis + 8]; }
		}
		/*else if (str_response[Find_Gis + 5] == '<') {
			cout << str_response[Find_Gis + 4]; res_gis = res_gis + str_response[Find_Gis + 4];
		}*/
		//else { cout << str_response[Find_Gis + 4] << str_response[Find_Gis + 5]; res_gis = res_gis + str_response[Find_Gis + 4] + str_response[Find_Gis + 5]; }
		cout << endl << res_gis;
		cout << endl << endl << endl << endl << endl;

		//---------------------------------------------------------/

		string str_response3;
		str_response3 = curl_request_wor(town, menuN);

		int Find_wor;
		int Find_wor_N;
		int Find_wor_N2;

		Find_wor_N = str_response3.find("Сейчас");
		//cout << "day-week  --  " << Find_wor_D << endl;
		Find_wor = str_response3.find("number\">", Find_wor_N + 15);
		//Find_wor = str_response3.find("rise", Find_wor_N2 + 15);
		//Find_worr = str_response.find("style=\"top:", Find_wor + 13); //unit unit_temperature_c
		//Find_worrr = str_response.find("_c", Find_worr + 13);
		//cout << Find_wor << endl << endl;
		//cout << str_response3[Find_wor + 7] << str_response3[Find_wor + 8] << str_response3[Find_wor + 9] << str_response3[Find_wor + 10]; // << str_response3[Find_wor + 18] << str_response3[Find_wor + 19] << str_response3[Find_wor + 20] << str_response3[Find_wor + 21] << str_response3[Find_wor + 22];
		
		string res_wor;
		if (str_response3[Find_wor + 8] == '-') {
			res_wor = res_wor + '-' + str_response3[Find_wor + 9];
			cout << '-' << str_response3[Find_wor + 9];
			if (str_response3[Find_wor + 10] == '<') {}
			else {
				res_wor = res_wor + str_response3[Find_wor + 10];
				cout << str_response3[Find_wor + 10];
			}
		}
		else if (str_response3[Find_wor + 8] == '+') {
			res_wor = res_wor + '+' + str_response3[Find_wor + 9];
			cout << '+' << str_response3[Find_wor + 9];
			if (str_response3[Find_wor + 10] == '<') {}
			else {
				res_wor = res_wor + str_response3[Find_wor + 10];
				cout << str_response3[Find_wor + 10];
			}
		}
		else if (str_response3[Find_wor + 9] == '<') {
			res_wor = res_wor + str_response3[Find_wor + 8];
			cout << str_response3[Find_wor + 8];
		}

		cout << endl << res_wor;
		cout << endl << endl << endl << endl << endl;

		//---------------------------------------------------------/
		int now = 1;
		string str_response4;
		str_response4 = curl_request_mail(town, now, date);

		int Find_mail_D;
		int Find_mail_C;
		int Find_mail_C2;
		int Find_mail_C3;
		int Find_mail_B;

		Find_mail_D = str_response4.find("information__content");
		//cout << "day2  --  " << Find_mail_D << endl;
		Find_mail_C = str_response4.find("content__temperature", Find_mail_D + 15);
		//cout << "c  --  " << Find_mail_C << endl;
		Find_mail_B = str_response4.find("</span>", Find_mail_C + 20);
		//cout << "c  --  " << Find_mail_C2 << endl;
		//Find_mail_C3 = str_response4.find("column_percent-16", Find_mail_C2 + 20);
		//cout << "c  --  " << Find_mail_C3 << endl;
		//Find_mail_B = str_response4.find("um margin_bottom_10", Find_mail_C3 + 20); //21
		//cout << Find_wor << endl << endl;
		//cout << str_response4[Find_mail_B + 7] << str_response4[Find_mail_B + 8] << str_response4[Find_mail_B + 9] << str_response4[Find_mail_B + 10] << str_response4[Find_mail_B + 11]; //<< str_response3[Find_wor + 19] << str_response3[Find_wor + 20] << str_response3[Find_wor + 21] << str_response3[Find_wor + 22];

		string res_mail;
		if (str_response4[Find_mail_B + 7] == '-') {
			res_mail = res_mail + '-' + str_response4[Find_mail_B + 8];
			cout << '-' << str_response4[Find_mail_B + 8];
			if (str_response4[Find_mail_B + 9] == '&') {}
			else {
				res_mail = res_mail + str_response4[Find_mail_B + 9];
				cout << str_response4[Find_mail_B + 9];
			}
		}
		else if (str_response4[Find_mail_B + 7] == '+') {
			res_mail = res_mail + '+' + str_response4[Find_mail_B + 8];
			cout << '+' << str_response4[Find_mail_B + 8];
			if (str_response4[Find_mail_B + 9] == '&') {}
			else {
				res_mail = res_mail + str_response4[Find_mail_B + 9];
				cout << str_response4[Find_mail_B + 9];
			}
		}
		else if (str_response4[Find_mail_B + 8] == '&') {
			res_mail = res_mail + str_response4[Find_mail_B + 7];
			cout << str_response4[Find_mail_B + 7];
		}

		cout << endl << res_mail;
		cout << endl << endl << endl << endl << endl;

		//---------------------------------------------------------/

		int res_gis_int = stoi(res_gis);
		int res_wor_int = stoi(res_wor);
		int res_mail_int = stoi(res_mail);

		double sr = (res_gis_int + res_wor_int + res_mail_int) / 3.0;
		cout << "sr  =  " << sr;

	}
	else if (menuN == 2) {
	int date = 0;
		string str_redirect;
		str_redirect = curl_request_gis(town);

		int Find_Gis2_loc;
		int Find_Gis2;
		Find_Gis2_loc = str_redirect.find(">Localities");
		//cout << Find_Gis2_loc << endl;
		Find_Gis2 = str_redirect.find("catalog-item-link", Find_Gis2_loc + 15);
		//cout << Find_Gis2 << endl << endl;
		//cout << str_redirect[Find_Gis2 + 28] << str_redirect[Find_Gis2 + 29] << str_redirect[Find_Gis2 + 30] << str_redirect[Find_Gis2 + 31] << str_redirect[Find_Gis2 + 32] << str_redirect[Find_Gis2 + 33] << endl;
		//cout << str_redirect[Find_Gis2 + 37] << str_redirect[Find_Gis2 + 38] << str_redirect[Find_Gis2 + 39] << str_redirect[Find_Gis2 + 40] << str_redirect[Find_Gis2 + 41] << str_redirect[Find_Gis2 + 42] << endl;
		int k; k = 0;
		int i; i = 29;
		string town_gis_redirect = "https://www.gismeteo.ru/";
		while (k < 1) {
			if (str_redirect[Find_Gis2 + i] == '/') {
				k++;
			}
			else {
				town_gis_redirect = town_gis_redirect + str_redirect[Find_Gis2 + i];
				i++;
			}
		}
		town_gis_redirect = town_gis_redirect + "/tomorrow/";
		//cout << town_gis_redirect;
		cout << endl << endl;


		string str_response;
		str_response = curl_request_gis2(town, town_gis_redirect);

		int Find_Gis;
		int Find_Gis_D;
		int Find_Gis_D2;
		int Find_Giss;
		int Find_Gisss;

		Find_Gis_D = str_response.find("date date");
		//cout << "date date  --  " << Find_Gis_D << endl;
		Find_Gis_D2 = str_response.find("date date", Find_Gis_D + 15);

		Find_Gis = str_response.find("style=\"top:", Find_Gis_D2 + 13);
		Find_Giss = str_response.find("style=\"top:", Find_Gis + 13); //unit unit_temperature_c
		Find_Gisss = str_response.find("_c", Find_Giss + 13);
		//cout << Find_Gisss << endl << endl;
		//cout << str_response[Find_Gis+68] << str_response[Find_Gis + 69] << str_response[Find_Gis + 70] << str_response[Find_Gis + 71] << str_response[Find_Gis + 72] << str_response[Find_Gis + 73] << str_response[Find_Gis + 74] << str_response[Find_Gis + 75] << str_response[Find_Gis + 76];

		string res_gis;
		if (str_response[Find_Gisss + 4] == '&') {
			cout << '-' << str_response[Find_Gisss + 11];
			res_gis = res_gis + "-" + str_response[Find_Gisss + 11];
			if (str_response[Find_Gisss + 12] == '<') {}
			else {
				cout << str_response[Find_Gisss + 12];
				res_gis = res_gis + str_response[Find_Gisss + 12];
			}
		}
		else if (str_response[Find_Gisss + 4] == '+') {
			cout << '+' << str_response[Find_Gisss + 5];  res_gis = res_gis + '+' + str_response[Find_Gisss + 5];
			if (str_response[Find_Gisss + 6] == '<') {}
			else { cout << str_response[Find_Gisss + 6]; res_gis = res_gis + str_response[Find_Gisss + 6]; }
		}
		else if (str_response[Find_Gisss + 5] == '<') {
			cout << str_response[Find_Gisss + 4]; res_gis = res_gis + str_response[Find_Gisss + 4];
		}
		else { cout << str_response[Find_Gisss + 4] << str_response[Find_Gisss + 5]; res_gis = res_gis + str_response[Find_Gisss + 4] + str_response[Find_Gisss + 5]; }
		cout << endl << res_gis;
		cout << endl << endl << endl << endl << endl;


		//-------------------------------------------------------------------------------------------------------------------\\
		

		string str_response3;
		str_response3 = curl_request_wor(town, menuN);

		int Find_wor;
		int Find_wor_D;
		int Find_wor_D2;

		Find_wor_D = str_response3.find("day-week");
		//cout << "day-week  --  " << Find_wor_D << endl;
		Find_wor_D2 = str_response3.find("day-week", Find_wor_D + 15);
		Find_wor = str_response3.find("rise", Find_wor_D2 + 15);
		//Find_worr = str_response.find("style=\"top:", Find_wor + 13); //unit unit_temperature_c
		//Find_worrr = str_response.find("_c", Find_worr + 13);
		//cout << Find_wor << endl << endl;
		//cout << str_response3[Find_wor + 12] << str_response3[Find_wor + 13] << str_response3[Find_wor + 14] << str_response3[Find_wor + 15]; // << str_response3[Find_wor + 18] << str_response3[Find_wor + 19] << str_response3[Find_wor + 20] << str_response3[Find_wor + 21] << str_response3[Find_wor + 22];

		string res_wor;
		if (str_response3[Find_wor + 13] == '-') {
			res_wor = res_wor + '-' + str_response3[Find_wor + 14];
			cout << '-' << str_response3[Find_wor + 14];
			if (str_response3[Find_wor + 17] == '<') {}
			else {
				res_wor = res_wor + str_response3[Find_wor + 15];
				cout << str_response3[Find_wor + 15];
			}
		}
		else if (str_response3[Find_wor + 13] == '+') {
			res_wor = res_wor + '+' + str_response3[Find_wor + 14];
			cout << '+' << str_response3[Find_wor + 14];
			if (str_response3[Find_wor + 17] == '<') {}
			else {
				res_wor = res_wor + str_response3[Find_wor + 15];
				cout << str_response3[Find_wor + 15];
			}
		}
		else if (str_response3[Find_wor + 16] == '<') {
			res_wor = res_wor + str_response3[Find_wor + 13];
			cout << str_response3[Find_wor + 13];
		}

		cout << endl << res_wor;
		cout << endl << endl << endl << endl << endl;


		//-------------------------------------------------------------------------------------------------------------------\\

		int now = 0;
		string str_response4;
		str_response4 = curl_request_mail(town, now, date);

		int Find_mail_D;
		int Find_mail_C;
		int Find_mail_C2;
		int Find_mail_C3;
		int Find_mail_B;

		Find_mail_D = str_response4.find("id=\"day2\"");
		//cout << "day2  --  " << Find_mail_D << endl;
		Find_mail_C = str_response4.find("column_percent-16", Find_mail_D + 15);
		//cout << "c  --  " << Find_mail_C << endl;
		Find_mail_C2 = str_response4.find("column_percent-16", Find_mail_C + 20);
		//cout << "c  --  " << Find_mail_C2 << endl;
		Find_mail_C3 = str_response4.find("column_percent-16", Find_mail_C2 + 20);
		//cout << "c  --  " << Find_mail_C3 << endl;
		Find_mail_B = str_response4.find("um margin_bottom_10", Find_mail_C3 + 20); //21
		//cout << Find_wor << endl << endl;
		//cout << str_response4[Find_mail_B + 20] << str_response4[Find_mail_B + 21] << str_response4[Find_mail_B + 22] << str_response4[Find_mail_B + 23] << str_response4[Find_mail_B + 24]; //<< str_response3[Find_wor + 19] << str_response3[Find_wor + 20] << str_response3[Find_wor + 21] << str_response3[Find_wor + 22];

		string res_mail;
		if (str_response4[Find_mail_B + 21] == '-') {
			res_mail = res_mail + '-' + str_response4[Find_mail_B + 22];
			cout << '-' << str_response4[Find_mail_B + 22];
			if (str_response4[Find_mail_B + 23] == '&') {}
			else {
				res_mail = res_mail + str_response4[Find_mail_B + 23];
				cout << str_response4[Find_mail_B + 23];
			}
		}
		else if (str_response4[Find_mail_B + 21] == '+') {
			res_mail = res_mail + '+' + str_response4[Find_mail_B + 22];
			cout << '+' << str_response4[Find_mail_B + 22];
			if (str_response4[Find_mail_B + 23] == '&') {}
			else {
				res_mail = res_mail + str_response4[Find_mail_B + 23];
				cout << str_response4[Find_mail_B + 23];
			}
		}
		else if (str_response4[Find_mail_B + 22] == '&') {
			res_mail = res_mail + str_response4[Find_mail_B + 21];
			cout << str_response4[Find_mail_B + 21];
		}

		cout << endl << res_mail;
		cout << endl << endl << endl << endl << endl;


		//-------------------------------------------------------------------------------------------------------------------\\


		int res_gis_int = stoi(res_gis);
		int res_wor_int = stoi(res_wor);
		int res_mail_int = stoi(res_mail);

		double sr = (res_gis_int + res_wor_int + res_mail_int) / 3.0;
		cout << "sr  =  " << sr;

	}
	else if (menuN == 3) {
		int date;
		cout << "Введите дату: ";
		cin >> date;
		//https://www.gismeteo.com/weather-taganrog-5106/month/
		//https://world-weather.ru/pogoda/russia/taganrog/month/
		//https://pogoda.mail.ru/prognoz/taganrog/1-june/


		string str_redirect;
		str_redirect = curl_request_gis(town);

		int Find_Gis2_loc;
		int Find_Gis2;
		Find_Gis2_loc = str_redirect.find(">Localities");
		//cout << Find_Gis2_loc << endl;
		Find_Gis2 = str_redirect.find("catalog-item-link", Find_Gis2_loc + 15);
		//cout << Find_Gis2 << endl << endl;
		//cout << str_redirect[Find_Gis2 + 28] << str_redirect[Find_Gis2 + 29] << str_redirect[Find_Gis2 + 30] << str_redirect[Find_Gis2 + 31] << str_redirect[Find_Gis2 + 32] << str_redirect[Find_Gis2 + 33] << endl;
		//cout << str_redirect[Find_Gis2 + 37] << str_redirect[Find_Gis2 + 38] << str_redirect[Find_Gis2 + 39] << str_redirect[Find_Gis2 + 40] << str_redirect[Find_Gis2 + 41] << str_redirect[Find_Gis2 + 42] << endl;
		int k; k = 0;
		int i; i = 29;
		string town_gis_redirect = "https://www.gismeteo.ru/";
		while (k < 1) {
			if (str_redirect[Find_Gis2 + i] == '/') {
				k++;
			}
			else {
				town_gis_redirect = town_gis_redirect + str_redirect[Find_Gis2 + i];
				i++;
			}
		}
		town_gis_redirect = town_gis_redirect + "/month/";
		//cout << town_gis_redirect;
		cout << endl << endl;


		string str_response;
		str_response = curl_request_gis2(town, town_gis_redirect);

		string date_Gis = "\">" + to_string(date) + "</div>";
		int Find_Gis;
		int Find_Gis_D;
		int Find_Gis_D2;
		int Find_Giss;
		int Find_Gisss;

		Find_Gis_D = str_response.find("widget-body"); //<div class="date item-day-3">31</div>
		//cout << "date date  --  " << Find_Gis_D << endl;
		Find_Gis_D2 = str_response.find(date_Gis, Find_Gis_D + 15);
		Find_Gis = str_response.find("temperature_c\">", Find_Gis_D2 + 15);
		//Find_Giss = str_response.find("style=\"top:", Find_Gis + 13); //unit unit_temperature_c
		Find_Gisss = str_response.find("an>", Find_Gis);
		//cout << Find_Gisss << endl << endl;
		//cout << str_response[Find_Gisss] << str_response[Find_Gisss + 1] << str_response[Find_Gisss + 2] << str_response[Find_Gisss + 3] << str_response[Find_Gisss + 4] << str_response[Find_Gisss + 5] << str_response[Find_Gisss + 6] << str_response[Find_Gisss + 7] << str_response[Find_Gisss + 8] << str_response[Find_Gisss + 9];

		string res_gis;
		if (str_response[Find_Gisss + 3] == '&') {
			cout << '-' << str_response[Find_Gisss + 9];
			res_gis = res_gis + "-" + str_response[Find_Gisss + 9];
			if (str_response[Find_Gisss + 10] == '<') {}
			else {
				cout << str_response[Find_Gisss + 10];
				res_gis = res_gis + str_response[Find_Gisss + 10];
			}
		}
		else if (str_response[Find_Gisss + 3] != '&') {
			cout << '+' << str_response[Find_Gisss + 3];  res_gis = res_gis + '+' + str_response[Find_Gisss + 3];
			if (str_response[Find_Gisss + 4] == '<') {}
			else { cout << str_response[Find_Gisss + 4]; res_gis = res_gis + str_response[Find_Gisss + 4]; }
		}
		/*else if (str_response[Find_Gisss + 5] == '<') {
			cout << str_response[Find_Gisss + 4]; res_gis = res_gis + str_response[Find_Gisss + 4];
		}
		else { cout << str_response[Find_Gisss + 4] << str_response[Find_Gisss + 5]; res_gis = res_gis + str_response[Find_Gisss + 4] + str_response[Find_Gisss + 5]; }*/
		cout << endl << res_gis;
		cout << endl << endl << endl << endl << endl;


		//-------------------------------------------------------------------------------------------------------------------\\

		
		string str_response3;
		str_response3 = curl_request_wor(town, menuN);

		int Find_wor;
		int Find_wor_N;
		int Find_wor_N2;
		string date_wor = "<div>" + to_string(date) + "</div>";

		Find_wor_N = str_response3.find("ww-month");
		//cout << "day-week  --  " << Find_wor_D << endl;
		Find_wor_N2 = str_response3.find(date_wor, Find_wor_N + 15); // <span>+14°< / span>
		Find_wor = str_response3.find("an>", Find_wor_N2 + 15);
		//Find_worr = str_response.find("style=\"top:", Find_wor + 13); //unit unit_temperature_c
		//Find_worrr = str_response.find("_c", Find_worr + 13);
		//cout << Find_wor << endl << endl;
		//cout << str_response3[Find_wor] << str_response3[Find_wor + 1] << str_response3[Find_wor + 2] << str_response3[Find_wor + 3] << str_response3[Find_wor + 4] << str_response3[Find_wor + 5] << str_response3[Find_wor + 6] << str_response3[Find_wor + 7] << str_response3[Find_wor + 8];
		
		string res_wor;
		if (str_response3[Find_wor + 3] == '-') {
			res_wor = res_wor + '-' + str_response3[Find_wor + 4];
			cout << '-' << str_response3[Find_wor + 4];
			if (str_response3[Find_wor + 7] == '<') {}
			else {
				res_wor = res_wor + str_response3[Find_wor + 5];
				cout << str_response3[Find_wor + 5];
			}
		}
		else if (str_response3[Find_wor + 3] == '+') {
			res_wor = res_wor + '+' + str_response3[Find_wor + 4];
			cout << '+' << str_response3[Find_wor + 4];
			if (str_response3[Find_wor + 7] == '<') {}
			else {
				res_wor = res_wor + str_response3[Find_wor + 5];
				cout << str_response3[Find_wor + 5];
			}
		}
		else if (str_response3[Find_wor + 6] == '<') {
			res_wor = res_wor + str_response3[Find_wor + 3];
			cout << str_response3[Find_wor + 3];
		}

		cout << endl << res_wor;
		cout << endl << endl << endl << endl << endl;


		//-------------------------------------------------------------------------------------------------------------------\\


		int now = 2;
		string str_response4;
		str_response4 = curl_request_mail(town, now, date);

		int Find_mail_D;
		int Find_mail_C;
		int Find_mail_C2;
		int Find_mail_C3;
		int Find_mail_B;

		Find_mail_D = str_response4.find("day day_period");
		//cout << "day2  --  " << Find_mail_D << endl;
		Find_mail_C = str_response4.find("day day_period", Find_mail_D + 15); // <div class="day__temperature ">+19°</div>
		//cout << "c  --  " << Find_mail_C << endl;
		Find_mail_C2 = str_response4.find("day day_period", Find_mail_C + 20);
		//cout << "c  --  " << Find_mail_C2 << endl;
		Find_mail_B = str_response4.find("day__temperature", Find_mail_C2 + 20);
		//cout << "c  --  " << Find_mail_C3 << endl;
		//Find_mail_B = str_response4.find("um margin_bottom_10", Find_mail_C3 + 20); //21
		//cout << Find_wor << endl << endl;
		//cout << str_response4[Find_mail_B + 16] << str_response4[Find_mail_B + 17] << str_response4[Find_mail_B + 18] << str_response4[Find_mail_B + 19] << str_response4[Find_mail_B + 20] << str_response4[Find_mail_B + 21] << str_response4[Find_mail_B + 22] << str_response4[Find_mail_B + 23] << str_response4[Find_mail_B + 24];
		
		string res_mail;
		if (str_response4[Find_mail_B + 19] == '-') {
			res_mail = res_mail + '-' + str_response4[Find_mail_B + 20];
			cout << '-' << str_response4[Find_mail_B + 20];
			if (str_response4[Find_mail_B + 21] == '&') {}
			else {
				res_mail = res_mail + str_response4[Find_mail_B + 21];
				cout << str_response4[Find_mail_B + 21];
			}
		}
		else if (str_response4[Find_mail_B + 19] == '+') {
			res_mail = res_mail + '+' + str_response4[Find_mail_B + 20];
			cout << '+' << str_response4[Find_mail_B + 20];
			if (str_response4[Find_mail_B + 21] == '&') {}
			else {
				res_mail = res_mail + str_response4[Find_mail_B + 21];
				cout << str_response4[Find_mail_B + 21];
			}
		}
		else if (str_response4[Find_mail_B + 20] == '&') {
			res_mail = res_mail + str_response4[Find_mail_B + 19];
			cout << str_response4[Find_mail_B + 19];
		}

		cout << endl << res_mail;
		cout << endl << endl << endl << endl << endl;


		//-------------------------------------------------------------------------------------------------------------------\\


		int res_gis_int = stoi(res_gis);
		int res_wor_int = stoi(res_wor);
		int res_mail_int = stoi(res_mail);

		double sr = (res_gis_int + res_wor_int + res_mail_int) / 3.0;
		cout << "sr  =  " << sr;
	}
}
