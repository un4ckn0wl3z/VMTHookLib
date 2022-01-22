#include <cstdio>
#include "VMTHook.h"

// https://github.com/gfreivasc/VMTHook

class Sum
{
public:
	virtual int operate(int a, int b) { return a + b; }
};

typedef int(__thiscall* operate_t)(Sum*, int, int);
operate_t oOperate;


int __stdcall operate_no_orig(int a, int b)
{
	printf("Hooked! Original output not accessible.\n");
	return a - b;
}

int __fastcall operate(Sum* that, DWORD _EDX, int a, int b)  // _EDX can be ignored

{
	printf("Hooked! Original output: %d\n", oOperate(that, a, b));
	return a * b;
}

int main()
{
	Sum* sum = new Sum();

	printf("  4\n+ 5\n---\n%3d\n", sum->operate(4, 5));
	oOperate = (operate_t)HookMethod((LPVOID)sum, (PVOID)operate, NULL);
	printf("  4\n* 5\n---\n%3d\n", sum->operate(4, 5));

	HookMethod((LPVOID)sum, (PVOID)operate_no_orig, NULL);
	printf("  4\n- 5\n---\n%3d\n", sum->operate(4, 5));

	system("Pause");
	delete sum;
	return 0;
}