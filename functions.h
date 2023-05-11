#pragma once

inline void consolXY(short x, short y);

int menu();

string curl_request_gis(string town);

string curl_request_gis2(string town, string town_gis_redirect);

string curl_request_wor(string town, int menuN);

string curl_request_mail(string town, int now, int date);

int WEZAR();