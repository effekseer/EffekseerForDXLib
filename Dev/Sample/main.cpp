
#include <Windows.h>
#include <stdio.h>

// 2Dのサンプル。本体はsample2D.cppに記述されている。
int sample2D();

// 3Dのサンプル。本体はsample3D.cppに記述されている。
int sample3D();

// ネットワークのサンプル。本体はnetwork.cppに記述されている。
int network();

int main()
{
	printf("サンプルを実行します。ボタンを押してください。\n");
	printf("1 - 2Dサンプル\n");
	printf("2 - 3Dサンプル\n");
	printf("3 - ネットワーク\n");

	int num = 0;
	scanf_s("%d", &num);

	if(num == 1)
	{
		sample2D();
	}
	else if(num == 2)
	{
		sample3D();
	}
	else if (num == 3)
	{
		network();
	}

	return 0;
}
