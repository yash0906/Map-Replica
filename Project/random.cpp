#include<bits/stdc++.h>
using namespace std;
char *random_key(int stringLength)
{
	// string key = "";
	string letters = "";
	char *key = (char *)malloc(sizeof(char) * stringLength + 1);
	for (char i = 'a'; i <= 'z'; i++)
		letters += i;
	for (char i = 'A'; i <= 'Z'; i++)
		letters += i;
	for (int i = 0; i < stringLength; i++)
		key[i] = letters[rand() % 52];
	key[stringLength] = '\0';

	return key;
}
char *random_value(int stringLength)
{
	char *value = (char *)malloc(sizeof(char) * stringLength + 1);

	// string value = "";
	string letters = "";
	for (int i = 0; i <= 255; i++)
		letters += char(i);

	for (int i = 0; i < stringLength; i++)
		value[i] = letters[rand() % 256];
	value[stringLength] = '\0';
	return value;
}
int main()
{
	int n;
	// cin >> n;
	n=10000000;
	cout << n << endl;
	for(int i=0;i<n;i++)
	{
		int k = rand()%64 +1;
		int v = rand()%256 +1;
		cout << random_key(k) << " " << random_value(v) << endl;
	}
}