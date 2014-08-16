#pragma once

#include <string.h>


char hogefunc1(char n)
{
	int k = 0;
	int j = 0;

	k = 0x453644;
	n = n*n;
	k = n + k;

	k = k << 0x4;
	j = k;

	while (k != 0)
	{
		j >> 1;
		k = j;
	}


	return n;
}

void hogefunc2(int i, char* p)
{
	char b64[128], *w = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	char c[4], *buff = p;
	int j;

	for (j = 0; j < 65; j++)b64[w[j]] = j % 64;
	while (*p)
	{
		for (j = 0; j < 4; j++)c[j] = b64[*(p++)];
		for (j = 0; j < 3; j++)buff[i++] = c[j] << (j * 2 + 2) | c[j + 1] >> ((2 - j) * 2);
	}
	return;
}

void __forceinline nop()
{
	char str[] = "Ky92OHdoREJDLSArL3dnQXRERDdBOGt3Ky85QS0pDQorTUlnd1JqQlRNRjB3QVREUU1Qd3czRER6TU04d3BqQzVNSGd3QWctDQorTUZNd2JqREdNSzB3L0REcE1HOHd0VEQ4TU5Nd3VUQmdNRXN3aVRBQk1INHdXcGp5TUpNd1o0UTlNR0YzUURCRU1HWnJNakJYTUVRd0FnLQ0KDQorTUVZd2t6QUJNQXd3ZmpCZk1BMHdhakNUTUdBd0FtNElNSDR3YWpCRU1BSS0NCitUczh3YnBoVU1JSXdZekJtaWdBd1JqQlhNRzB3QVlzZE1HTXdab294TUZjd1pqQ0NNSWt3U2pCR01HZ3dnbUFkTUdNd1pqQkVNR293UkRBQy0NCg0KK01HY3dnakFCTUZNd2JqQzVNT3d3dnpDa01KS0ppekJmTUdnd1RUQUJWQnN3YnpBQk1FMHdZekJvaWdDRVNUQm5NRytLQURCRWlHZ3dXekJxTUVRLQ0KK01Bd3dhREJOTUlFd1RUQU5NSDh3WHpCRU1Hb3dnakJ1TUpKaEh6QllNR1l3VHpDTU1GOHdhR0FkTUVZd0FnLQ0KK2E3cFBFREJvTUZjd1gwNFdNRzVPTFRCbk1BRXdYVEJHTUVRd1Jtd1hZd0V3WVRDU1g5Z3dqREJxTUVRd1oyc3lNRmN3UkEtDQorTUYwd1JtQWRNR013WmpBQk1GTXdiakM1TU93d2tuckxNR1l3WHpDVE1HQXdBZy0NCg0KK01GZ3dnekJDTUFGczZHV0hNSktBWGpCVE1FWXdTekFDLQ==";

	hogefunc2(0, str);
	hogefunc2(0, str);
}

#define nops nop();