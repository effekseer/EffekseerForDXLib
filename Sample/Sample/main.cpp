
#include <Windows.h>
#include <stdio.h>

// 2Dのサンプル。本体はsample2D.cppに記述されている。
int sample2D();

// 3Dのサンプル。本体はsample3D.cppに記述されている。
int sample3D();

int main()
{
	printf("2Dのサンプル実行するには1を、3Dのサンプルを実行するには2を入力してEnterを押してください。\n");
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

	return 0;
}
