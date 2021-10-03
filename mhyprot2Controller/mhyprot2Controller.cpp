#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include <vector>
#include <Windows.h>
#include "KethoMemoryToolKit/KethoLib.h"

#define FILE_DEVICE_HIDE	0x8000

#define IOCTL_BASE			 0x800

#define CTL_CODE_HIDE(i)	\
	CTL_CODE(FILE_DEVICE_HIDE, IOCTL_BASE+i, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define KillProcess CTL_CODE_HIDE(1)
#define ScannProcess CTL_CODE_HIDE(2)
#define ReadProcessMem CTL_CODE_HIDE(3)
#define SetProcessHook CTL_CODE_HIDE(4)


typedef struct _KERNEL_JOGO_LER_MEMORIA {
    ULONG PID;
    PVOID ENDERECO;
    PVOID RESPOSTA;
    ULONG TAMANHO;
} KERNEL_JOGO_LER_MEMORIA, * PKERNEL_JOGO_LER_MEMORIA;


typedef struct _KERNEL_PROCESSO_ANALISE {
    const wchar_t* nome_processo;
    int status;
} KERNEL_PROCESSO_ANALISE, * PKERNEL_PROCESSO_ANALISE;


int main(char argc, char** argv)
{
    HANDLE device = INVALID_HANDLE_VALUE;
    BOOL status = FALSE;
    DWORD bytesReturned = 0;
    CHAR inBuffer[128] = { 0 };
    CHAR outBuffer[128] = { 0 };
    int controlCode = 0;

    controlCode = atoi(argv[1]);

    device = CreateFileW(L"\\\\.\\mhyprot2", GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);

    if (device == INVALID_HANDLE_VALUE)
    {
        printf("Não foi possível abrir uma comunicação IOTCL com o Driver: 0x%x\n", GetLastError());
        return FALSE;
    }

    if (controlCode == 1) {
        printf("Obtendo informações sobre KillProcess 0x%x\n", KillProcess);
        int PID = atoi(argv[2]);
        status = DeviceIoControl(device, KillProcess, &PID, sizeof(int), outBuffer, sizeof(outBuffer), &bytesReturned, (LPOVERLAPPED)NULL);
        printf("Obtendo KillProcess 0x%x analisado\n", KillProcess);
    }
    else if (controlCode == 2) {
        KERNEL_JOGO_LER_MEMORIA kReadMem;
        KethoLib* kethoInst = new KethoLib();
        std::cout << "Iniciando leitura da memória de um processo!" << std::endl;
        kReadMem.PID = atoi(argv[2]);
        kReadMem.ENDERECO = (PVOID)(0xDEADBEEF);
        kReadMem.TAMANHO = sizeof(ULONG);
        std::cout << std::endl << "BASE MSEDGE:" << std::hex << kReadMem.ENDERECO << std::endl;
        status = DeviceIoControl(device, ReadProcessMem, &kReadMem, sizeof(kReadMem), &kReadMem, sizeof(kReadMem), NULL, NULL);
        //printf("Obtendo ReadProcessMem 0x%x analisado\n", ReadProcessMem);
        std::cout << "Obtendo ReadProcessMem " << std::hex << ReadProcessMem << " analisado" << std::endl;
        std::cout << "Presente da miojo: " << *(int *)kReadMem.RESPOSTA << std::endl;
        std::cout << "Presente da miojo in hex: " << std::hex << (int *)kReadMem.RESPOSTA << std::endl;
    }
    else if (controlCode == 3) {
        KERNEL_PROCESSO_ANALISE KsetProcessName;
        std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> converter;
        std::wstring arg = converter.from_bytes(argv[2]);
        std::wcout << arg << L'\n';
        KsetProcessName.nome_processo = arg.c_str();
        KsetProcessName.status = 0;
        status = status = DeviceIoControl(device, SetProcessHook, &KsetProcessName, sizeof(KsetProcessName), &KsetProcessName, sizeof(KsetProcessName), NULL, NULL);
        if (KsetProcessName.status == 777) {
            std::cout << "O Ring zero confirmou que o processo foi alterado com sucesso para " << KsetProcessName.nome_processo << std::endl;
        }
    }

    //printf("Recebido do ring de kernel: %s buffer com tamanho de: %d\n", outBuffer, bytesReturned);
    std::cout << "Recebido do ring de kernel:" << outBuffer << " buffer com tamanho de: " << std::hex << bytesReturned << std::endl;

    CloseHandle(device);
}