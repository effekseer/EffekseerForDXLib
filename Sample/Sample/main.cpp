
#include <Windows.h>
#include <stdio.h>

// 2D�̃T���v���B�{�̂�sample2D.cpp�ɋL�q����Ă���B
int sample2D();

// 3D�̃T���v���B�{�̂�sample3D.cpp�ɋL�q����Ă���B
int sample3D();

int main()
{
	printf("2D�̃T���v�����s����ɂ�1���A3D�̃T���v�������s����ɂ�2����͂���Enter�������Ă��������B\n");
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
