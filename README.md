# WEZAR REPOTO
## О проекте
#### Идея
Долго думал над тем, что представить в проекте, хотелось придумать что-то новое, хотя бы не заезженное. И в один прекрасный день я для чего-то посмотрел погоду на двух разных сайтах, разница температур была слишком большой. В тот момент я понял, какой у меня будет проект.
#### Цель проекта
Создать программу, которая будет собирать данные о погоде с 3-х разных сайтов и выдавать пользователю среднее арифметическое температуры
#### Задачи
1. Выбрать сайты с наиболее подходящим форматом ссылок
2. Выбрать способ парсинга сайтов на с++
3. Написать программу
4. Сделать вид, что мне понравилось парсить на с++
## О программе
Из трех вариантов парсинга сайтов, которые я нашел, самым удачным оказался libcurl (про остальные два не спрашивайте, я уже и не помню, что там было). Libcurl - это библиотека, с помощью которой я сохранял код html страницы в переменную string, после чего совершал с ней все необходимые действия.

Чтобы подключить libcurl пришлось потратить около двух дней танцев с бубном и консолью. Скорее всего, это из-за того, что я начинающий программист, но все же процесс подключения явно непростой. И, наконец, у меня было разблокировано:

```c++
#include <curl/curl.h> 
``` 

<img width="300px" src="могучий враг повержен.png"></img>

#### Запись html кода в string
Далее я выбрал 3 сайта погоды, с которых получал данные (gismeteo.com, world-weather.ru, pogoda.mail.ru (для удобства ниже будет представлен код только для гисметео, так как на всех сайтах он +- одинаковый)). А получал я эти данные при помощи

```
  CURL* curl;
  CURLcode response;
  string request_gis2;

  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, town_gis_redirect.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getResponsetoString);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &request_gis2);
  response = curl_easy_perform(curl);
  curl_easy_cleanup(curl); 
  // gismeteo.com
```
где ```request_gis2``` и есть та переменная, в которую записывался html код. Почему ```gis2```? Потому что gismeteo имел ужасную привычку писать в своих ссылках вместо нормального ```/taganrog/```, например, ```/weather-taganrog-5106/```, вследствие чего ```gis1``` пришлось отдать странице с редиректом (про редирект ниже).

Кстати функция ```getResponsetoString``` тоже была необходима для записи хтмл в переменную

```
static size_t getResponsetoString(void* contents, size_t size, size_t nmemb, void* userp) {
	((string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
```

#### Извлечение из string чего-нибудь полезного
Как вы понимаете, на этом только начиналась работа со стрингом.
Недолго думая (а думал я реально недолго), я взял ```.find``` и начал искать нужные куски htmlевского кода и постепенно перемещаться к температуре воздуха, а конкретно на сайте gismeteo к ссылке для редиректа.

Поиск температуры воздуха нормального человека:

```
	Find_Gis_D = str_response.find("date date");
	Find_Gis_D2 = str_response.find("date date", Find_Gis_D + 15);
	Find_Gis = str_response.find("style=\"top:", Find_Gis_D2 + 13);
	Find_Giss = str_response.find("style=\"top:", Find_Gis + 13);
	Find_Gisss = str_response.find("_c", Find_Giss + 13);
```

Поиск температуры воздуха gismeteo:

```
	Find_Gis2_loc = str_redirect.find(">Localities");
	Find_Gis2 = str_redirect.find("catalog-item-link", Find_Gis2_loc + 15);
    
	int k = 0;
	int i = 29;
	string town_gis_redirect = "https://www.gismeteo.ru/";
	while (k < 1) { 
    //Сбор ссылки для редиректа
		if (str_redirect[Find_Gis2 + i] == '/') {
			k++;
		}
		else {
			town_gis_redirect = town_gis_redirect + str_redirect[Find_Gis2 + i];
			i++;
		}
	}
	town_gis_redirect = town_gis_redirect + "/tomorrow/";
	cout << endl << endl;

	Find_Gis_D = str_response.find("date date");
	Find_Gis_D2 = str_response.find("date date", Find_Gis_D + 15);
	Find_Gis = str_response.find("style=\"top:", Find_Gis_D2 + 13);
	Find_Giss = str_response.find("style=\"top:", Find_Gis + 13); 
	Find_Gisss = str_response.find("_c", Find_Giss + 13);
```

Теперь самое главное. После того, как программа нашла место в коде с нужной мне температурой, я должен был вытащить только ее

```
// Извлечение температуры воздуха
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
```

#### Из string в int
А теперь ```Запись html кода в string; Извлечение из string чего-нибудь полезного``` повторяем еще два раза, но с другими сайтами. Далее нужно было преобразовать полученные данные из string в int для вычисления средней температуры
```
	int res_gis_int = stoi(res_gis);
	int res_wor_int = stoi(res_wor);
	int res_mail_int = stoi(res_mail);

	double sr = (res_gis_int + res_wor_int + res_mail_int) / 3.0;
	cout << "sr  =  " << sr;
```
#### Добавление меню выбора действия
После всех страда.. Кхм, после всей работы я решил сделать меню с возможностью выбрать день, прогноз погоды на который показать (код меню можно найти в главном файле с всей программой, потому что он не маленький).

![image](меню.gif)

## Итоги
В заключение хотелось бы сказать, что, создавая проект, я узнал много нового. Например, познакомился с основами парсинга и стал лучше ориентироваться в html коде. В будущем я планирую перевести проект на библиотеку для парсинга html.

Спасибо за внимание
