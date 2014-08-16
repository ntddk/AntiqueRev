#include <stdio.h>
#include <Windows.h>
#include <winternl.h>
#include "nops.h"

typedef int(__stdcall *msgbox)(HWND, LPCSTR, LPCSTR, UINT);
typedef int(__stdcall *loadstr)(HINSTANCE, UINT, long, int);
typedef int(__stdcall *getfileattr)(LPSTR);
typedef int(__stdcall *getstartupinfo)(LPSTARTUPINFOA);
typedef int(__stdcall *exitprocess)(UINT);
typedef int(__stdcall *ntqueryinformationprocess)(long, long, long, long, long);

#define obf(c)	(c) - 0x1CE
#define decodeobf(str)      do{char *p = str; while (*p) *p++ += 0x1CE;} while(0);

#define flagobf(c)  (c) + 0x22
#define decodeflagobf(str)      do{char *p = str; while (*p) *p++ -= 0x22;} while(0);

#define goma_GetProcAddress(a,b) (long)GetProcAddress(a,b) + 0x1CE;

int cookie = 8852-493;
int key = 10;
char user32[100];
long loadstring;
long goma_msgbox;

long __forceinline goma_GetMessageBox()
{
	char dll[] = { obf('U'), obf('S'), obf('E'), obf('R'), obf('3'), obf('2'), obf('.'), obf('D'), obf('L'), obf('L'), 0 };

	decodeobf(dll);
	return goma_GetProcAddress(GetModuleHandle(dll), user32) + 0xBEEF;
}

void __forceinline goma_MessageBox(long l, char* str, char* title)
{
	((msgbox)(l - 0xC0BD))(0, str, title, 0);
}

void __forceinline randloop()
{
	__asm{
	hoge:
		mov     ecx, [eax + 0x0C]
		mov     esi, [ebp + 8]
		push    edi
		mov[ebp], esi
		mov     eax, [ecx + 0x0C]
		mov     ebx, [eax + 4]
		mov     eax, [ecx + 8]
		mov     ecx, esi
		mov     eax, [eax + 4]
		mov[ebp], eax
		lea     eax, [ebp]
		push    eax
		call    ebp
		test    eax, eax
		popad
		push    esi
		push    edi
		mov     edi, ecx
		push    55h
		pop     edx
		and     dword ptr[edi + 4], 0
		lea     esi, [edi + 0Ch]
		mov     ecx, esi
		mov     dword ptr[edi], 1
		sub  eax,ebp
		jnz     hoge
		int 3
		test    eax, eax
		push    esi
		push    edi
		mov     edi, ecx
		push    55h
		pop     edx
		call esp
		nop
		push    edi
		mov     edi, ecx
		push    55h
		pop     edx
		and     dword ptr[edi + 4], 0
		lea     esi, [edi + 0Ch]
		mov     ecx, esi
		mov     dword ptr[edi], 1
		pushad
	}
}

int isVM()
{
	DWORD sysret1, sysret2;
	long ggfaa;
	char kernel32[] = { obf('k'), obf('e'), obf('r'), obf('n'), obf('e'), obf('l'), obf('3'), obf('2'), obf('.'), obf('d'), obf('l'), obf('l'), 0 };
	char gfaa[] = { obf('G'), obf('e'), obf('t'), obf('F'), obf('i'), obf('l'), obf('e'), obf('A'), obf('t'), obf('t'), obf('r'), obf('i'), obf('b'), obf('u'), obf('t'), obf('e'), obf('s'), obf('A'), 0 };

	decodeobf(kernel32);
	decodeobf(gfaa);

	ggfaa = goma_GetProcAddress(GetModuleHandle(kernel32), gfaa);

	sysret1 = ((getfileattr)(ggfaa - 0x1ce))("C:\\WINDOWS\\system32\\drivers\\vmhgfs.sys");
	sysret2 = ((getfileattr)(ggfaa - 0x1ce))("C:\\WINDOWS\\system32\\drivers\\vmmouse.sys");

	if (sysret1 == INVALID_FILE_ATTRIBUTES || sysret2 == INVALID_FILE_ATTRIBUTES || cookie != 8359)
		randloop();

	cookie = 0x319;
	key -= 4;
	return 0;
}

int __forceinline isPEB()
{
	int i = 0, j = 0, k = 0;
	PEB *peb = 0;
	char str2[20] = { 0 }, str[30] = { 0 };
	long messagebox;

	loadstring = (long)GetProcAddress(LoadLibrary("user32.dll"), "LoadStringA") + 0x31c5;

	messagebox = LoadLibrary(NULL);

	_asm{
		xor eax, eax
	}
	messagebox += 0x10;
	_asm{
		mov eax, fs:[0x30]
		mov ecx, 0x30
			shl ecx, 2
		mov[peb], eax
	}

	j = ((loadstr)(loadstring - 0x31c5))(messagebox - 0x10, 102, user32, 100);
	__asm{nop}
	k = ((loadstr)(loadstring - 0x31c5))(messagebox - 0x10, 103, str2, 20);

	__asm {
		xor ecx,ecx

		jnz jj
		nop
		nop
	//	int 03
	jj:
		xor ecx,ecx
	}

	for (i = 0; i < max(j,k); i++)
	{
		int n = user32[i];
		int m = str2[i];

		_asm{
			mov edx, eax;
			mov eax, ebx;
		}
		if (i < j)
			n += 4;
		_asm
		{
			add eax, ecx;
			xor edx, eax;
		}
		if (i < k)
			m += 11;
		_asm
		{
			add edx,1;
		}
		user32[i] = n;
		str2[i] = m;
	}

	j = ((loadstr)(loadstring - 0x31c5))(messagebox - 0x10, 111, str, 108);
	for (i = 0; i < j; i++)
	{
		int n = str[i];

		_asm{
			mov edx, eax;
			mov eax, ebx;
		}
		n -= key;
		_asm
		{
			add eax, ecx;
			xor edx, eax;
		}
		_asm
		{
			add edx, 1;
		}
		str[i] = n;
	}

	messagebox = (long)(long)GetProcAddress(GetModuleHandle(user32), str2) + 0xe9fb;

	if (peb->BeingDebugged && cookie != 0x319)
	{
		messagebox += 0xc1e8;
		((msgbox)(messagebox-0x1abe3))(0, str, 0, 0);
	}
	else
	{
		cookie = 0x0202;
		key - 4;
		return;
	}
	randloop();

	return;
}

int __forceinline ChangeSizeOfImage()
{
	__asm
	{
		mov eax, fs:[0x30]							// PEB
			mov eax, [eax + 0x0c]					// PEB_LDR_DATA
			mov eax, [eax + 0x0c]					// InOrderModuleList
			mov dword ptr[eax + 0x20], 0x100000000	// SizeOfImage
	}
	nops;

	return;
}

FARPROC __forceinline GetFuncAddress(LPSTR dllname, LPSTR funcname)
{
	int i;
	byte* module;
	IMAGE_NT_HEADERS32* header;
	IMAGE_IMPORT_DESCRIPTOR* iid;
	IMAGE_IMPORT_BY_NAME* iibn;
	IMAGE_THUNK_DATA *oitd, *itd;

	nops;
	module = GetModuleHandle(NULL);
	nops;
	header = ((IMAGE_NT_HEADERS*)(((IMAGE_DOS_HEADER*)module)->e_lfanew + module));
	nops;
	iid = header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress + module;
	nops;

	oitd = 0x0;
	nops;

	for (i = 0; iid[i].Name != NULL; i++)
	{
		if (strcmp((iid[i].Name + module), dllname) >= 0)
		{
			oitd = iid[i].OriginalFirstThunk + module;
			nops;
			itd = iid[i].FirstThunk + module;
			nops;
			break;
		}
	}

	if (oitd != 0x0)
	{
		for (i = 0; oitd[i].u1.AddressOfData != 0x0; i++)
		{
			iibn = (IMAGE_IMPORT_BY_NAME*)(oitd[i].u1.AddressOfData + module);
			nops;
			if (strcmp(iibn->Name, funcname) >= 0)
			{
				nops;
				return itd[i].u1.Function;
			}
		}
	}
	nops;
	return 0;
}

int __forceinline isNtQIP()
{
	char str[100] = { 0 }, str2[100] = { 0 };
	int i,j;
	HMODULE hmod;
	long _NtQueryInformationProcess;
	PVOID retVal = 0;
	char ntdll[] = { obf('n'), obf('t'), obf('d'), obf('l'), obf('l'), obf('.'), obf('d'), obf('l'), obf('l'), 0 };
	char ntquery[] = { obf('N'), obf('t'), obf('Q'), obf('u'), obf('e'), obf('r'), obf('y'), obf('I'), obf('n'), obf('f'), obf('o'), obf('r'), obf('m'), obf('a'), obf('t'), obf('i'), obf('o'), obf('n'), obf('P'), obf('r'), obf('o'), obf('c'), obf('e'), obf('s'), obf('s'), 0 };


	_asm{
		mov eax,0x30
		nop
		mov ebx,eax
	}

	j = ((loadstr)(loadstring - 0x31c5))(GetModuleHandle(NULL),109,str,100);
	for (i = 0; i < j; i++)
		str[i] += 2;

	j = ((loadstr)(loadstring - 0x31c5))(GetModuleHandle(NULL), 110, str2, 100);
	for (i = 0; i < j; i++)
		str2[i] += 2;


	decodeobf(ntdll);
	decodeobf(ntquery);

	hmod = LoadLibrary(ntdll);
	_NtQueryInformationProcess = goma_GetProcAddress(hmod, ntquery);

	((FARPROC)(_NtQueryInformationProcess - 0x1CE))(-1, 0x07, &retVal, 4, NULL);

	if (retVal != 0 && cookie != 0x0202)
	{
		GetFuncAddress(str, str2)(0);
		randloop();
	}

	cookie = 19720924;
	return;
}

void __forceinline Flag()
{
	char window[] = { obf('F'), obf('L'), obf('A'), obf('G'), obf('I'), obf('S'), 0 };
	char flag[] = { flagobf('G'), flagobf('A'), flagobf('N'), flagobf('B'), flagobf('A'), flagobf('R'), flagobf('U'), flagobf('Z'), flagobf('O'), flagobf('I'), flagobf('!'), 0 };
	long gmb = goma_GetMessageBox();

	decodeflagobf(flag);
	decodeobf(window);

	const char *title = window, *msg = flag;

	if (cookie == 19720924)
		goma_MessageBox(gmb, msg, title);

	nops;
}

int main(int argc, char* argv[])
{
	puts("進捗どうですか？");

	if (isVM() == 0 && isPEB() == 0 && ChangeSizeOfImage() == 0 && isNtQIP() == 0)
		Flag();

	return;
}


/*

これは、セキュリティ・キャンプ全国大会2014で行われたCTFのために書かれました。

たくさんの助言を寄せてくれた@mimura1133と、
問題をレビューしてくれた@potetisensei, @6f70に感謝します。

以下ネタバレです。

44Gy44Gp44GE44Kz44O844OJ44KS5pu444GE44Gm44GK44GE44Gm5L2V44Gn44GZ44GM44CB5ZWP
6aGM44Gu5oSP5Zuz44Go44GX44Gm44Gv44CBDQrjg7vlrp/nv5LjgafnlKjjgYTjgZ9WTXdhcmUg
UGxheWVy44Gu44KC44Go44Gn5YuV44GL44GX44Gm44KC44KJ44GG44GT44GoDQrjg7vlrp/nv5Lj
gaflrabjgpPjgaBQRULmp4vpgKDkvZPjgpLmhI/orZjjgZfjgabjgoLjgonjgYbjgZPjgagNCuOD
u0lNQUdFX09QVElPTkFMX0hFQURFUi5TaXplT2ZJbWFnZeOChE50UXVlcnlJbmZvcm1hdGlvblBy
b2Nlc3PjgYvjgokNCk9sbHlEYmfjgavjgojjgovli5XnmoTop6PmnpDjgpLoq6bjgoHjgabjgoLj
gonjgYbjgZPjgagNCuOBqOOBhOOBhuOCguOBruOBjOOBguOCiuOBvuOBmeOAgg0KDQrmnIDlpKfj
ga7nm67nmoTjga9JREEgUHJv44Gr5oWj44KM44Gm44KC44KJ44GG44GT44Go44Gn44GX44Gf44CC
DQrjg7vjgq/jg6njg4Pjgrfjg6XjgZfjgZ/lnLDngrnjgYvjgolJREEgUHJv44GuR3JhcGggVmll
d+OCkueiuuiqjeOBmeOCiw0K44O744Kv44Op44OD44K344Ol44GX44Gf5Zyw54K544Gu44K544K/
44OD44Kv44Gr5Y+O44G+44Gj44Gm44GE44KLRkxBR+OBuOOBruODneOCpOODs+OCv+OCkui+v+OC
iw0K44Gq44Gp44Gu44Ki44OX44Ot44O844OB44Gn44GV44Gj44GP44KK6Kej44GR44Gf44Gn44GX
44KH44GG44CCDQoNCkNURuOBp+ODkOOCpOODiuODquOCkumFjeW4g+OBl+OBn+aZguOBr0FzUGFj
a+OBq+OCiOOCi+Wcp+e4ruOCkuOBi+OBkeOBpuOBhOOBvuOBl+OBn+OBjOOAgeOBk+OCjA0K44Gv
6JuH6Laz44Gg44Gj44Gf44GL44Gq44Go5Y+N55yB44GX44Gm44GE44G+44GZ44CCDQoNCuWwkeOB
l+OBp+OCgualveOBl+OCk+OBp+OCguOCieOBiOOBn+OCieS9leOCiOOCiuOBp+OBmeOAgg==

@ntddk

*/

