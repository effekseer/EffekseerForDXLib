
#include <Windows.h>
#include <stdio.h>

// 2D�̃T���v���B�{�̂�sample2D.cpp�ɋL�q����Ă���B
int sample2D();

// 3D�̃T���v���B�{�̂�sample3D.cpp�ɋL�q����Ă���B
int sample3D();

// �l�b�g���[�N�̃T���v���B�{�̂�network.cpp�ɋL�q����Ă���B
int network();

int main()
{
	printf("�T���v�������s���܂��B�{�^���������Ă��������B\n");
	printf("1 - 2D�T���v��\n");
	printf("2 - 3D�T���v��\n");
	printf("3 - �l�b�g���[�N\n");

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
