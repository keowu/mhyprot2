#pragma once
#include <ntifs.h>
#include "memorymng.h"
#include <ntddk.h>
#include <ntstrsafe.h>
#include <stdlib.h>
#include <wdm.h>

extern "C" { 

//################################################################################################################################
/*

       _     _____       _ _ _                _          _                 _                           _
      | |   /  __ \     | | | |              | |        (_)               | |                         | |
  ___ | |__ | /  \/ __ _| | | |__   __ _  ___| | _____   _ _ __ ___  _ __ | | ___ _ __ ___   ___ _ __ | |_ __ _  ___ __ _  ___
 / _ \| '_ \| |    / _` | | | '_ \ / _` |/ __| |/ / __| | | '_ ` _ \| '_ \| |/ _ \ '_ ` _ \ / _ \ '_ \| __/ _` |/ __/ _` |/ _ \
| (_) | |_) | \__/\ (_| | | | |_) | (_| | (__|   <\__ \ | | | | | | | |_) | |  __/ | | | | |  __/ | | | || (_| | (_| (_| | (_) |
 \___/|_.__/ \____/\__,_|_|_|_.__/ \__,_|\___|_|\_\___/ |_|_| |_| |_| .__/|_|\___|_| |_| |_|\___|_| |_|\__\__,_|\___\__,_|\___/
                                                                    | |
                                                                    |_|
    ultima modificação: 13/03/2021
    REV 0.01:
    Author: João Vitor Sampaio | Impementação
*/
//////////////////////////////////////////////////////////
#define PROCESS_TERMINATE         0x0001  
#define PROCESS_VM_OPERATION      0x0008  
#define PROCESS_VM_READ           0x0010  
#define PROCESS_VM_WRITE          0x0020

/// /////////////////////////////////////////////////////


/// <summary>
///		PsGetProcessImageFileName, referencia externa da NTDDK
/// </summary>
/// <param name="Process">Process a ser buscado</param>
/// <returns>Nome do processo encontrado</returns>
extern UCHAR* PsGetProcessImageFileName(IN PEPROCESS Process);

/// <summary>
///		KeMhYCallbacks, Nossa OBCallback
/// </summary>
/// <param name="ContextoDeRegistro">ContextoDeRegistro usado no registro do callback</param>
/// <param name="pInformacoesDeOperacoes">pInformacoesDeOperacoes informações de registro do callback</param>
/// <returns>Flag de estado de operação para a ObCallback</returns>
OB_PREOP_CALLBACK_STATUS KeMhYCallbacks(PVOID ContextoDeRegistro, POB_PRE_OPERATION_INFORMATION pInformacoesDeOperacoes);
char* GetProcessNameByProcessID(HANDLE PID);
//################################################################################################################################



//################################################################################################################################
/*

 _____                   _             _          ______
|_   _|                 (_)           | |         | ___ \
  | | ___ _ __ _ __ ___  _ _ __   __ _| |_ ___    | |_/ / __ ___   ___ ___  ___ ___
  | |/ _ \ '__| '_ ` _ \| | '_ \ / _` | __/ _ \   |  __/ '__/ _ \ / __/ _ \/ __/ __|
  | |  __/ |  | | | | | | | | | | (_| | ||  __/   | |  | | | (_) | (_|  __/\__ \__ \
  \_/\___|_|  |_| |_| |_|_|_| |_|\__,_|\__\___|   \_|  |_|  \___/ \___\___||___/___/

    ultima modificação: 13/03/2021
    REV 0.01:
    Author: João Vitor Sampaio | Impementação
*/

/// <summary>
///		KeMhYTerminateProcess - Encerra um processo no nível de usuário por meio do seu PID
/// </summary>
/// <param name="PID">PID(PROCESS ID) DO PROCESSO NO QUAL DESEJA-SE ENCERRAR</param>
/// <returns>RETORNAMOS UMA FLAG DA NTSTATUS, SENDO ELA DE SUCESSO OU DE ERRO</returns>
NTSTATUS KeMhYTerminateProcess(HANDLE PID);
//################################################################################################################################



//################################################################################################################################
/*

 _____                     ___  ___          _       _           _____  ________
|  ___|                    |  \/  |         | |     | |         |  _  |/ _| ___ \
| |__ _ __  _   _ _ __ ___ | .  . | ___   __| |_   _| | ___  ___| | | | |_| |_/ / __ ___   ___ ___  ___ ___
|  __| '_ \| | | | '_ ` _ \| |\/| |/ _ \ / _` | | | | |/ _ \/ __| | | |  _|  __/ '__/ _ \ / __/ _ \/ __/ __|
| |__| | | | |_| | | | | | | |  | | (_) | (_| | |_| | |  __/\__ \ \_/ / | | |  | | | (_) | (_|  __/\__ \__ \
\____/_| |_|\__,_|_| |_| |_\_|  |_/\___/ \__,_|\__,_|_|\___||___/\___/|_| \_|  |_|  \___/ \___\___||___/___/

	ultima modificação: 13/03/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
*/
/// <summary>
///		Struct da _LDR_DATA_TABLE_ENTRY, para obter-se os dados da PEB
/// </summary>
typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY64	InLoadOrderLinks;
	LIST_ENTRY64	InMemoryOrderLinks;
	LIST_ENTRY64	InInitializationOrderLinks;
	PVOID			DllBase;
	PVOID			EntryPoint;
	ULONG			SizeOfImage;
	UNICODE_STRING	FullDllName;
	UNICODE_STRING 	BaseDllName;
	ULONG			Flags;
	USHORT			LoadCount;
	USHORT			TlsIndex;
	PVOID			SectionPointer;
	ULONG			CheckSum;
	PVOID			LoadedImports;
	PVOID			EntryPointActivationContext;
	PVOID			PatchInformation;
	LIST_ENTRY64	ForwarderLinks;
	LIST_ENTRY64	ServiceTagLinks;
	LIST_ENTRY64	StaticLinks;
	PVOID			ContextInformation;
	ULONG64			OriginalBase;
	LARGE_INTEGER	LoadTime;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;
/// <summary>
///		PsGetProcessPeb, referencia externa da NTKERNELAPI
/// </summary>
/// <param name="Process">Processo a ser buscado</param>
/// <returns>PEB do processo buscado</returns>
extern NTKERNELAPI PPEB PsGetProcessPeb(IN PEPROCESS Process);
/// <summary>
///		KeMhYEnumModulesOfProcess - Obtem o nome de um processo e seus modulos carregados da PEB a partir de um PID
/// </summary>
/// <param name="PID">PID(Process ID) do processo no qual deseja-se buscar</param>
/// <returns>Retorna a flag de sucesso ou erro NTSTATUS</returns>
NTSTATUS KeMhYEnumModulesOfProcess(HANDLE PID);
//################################################################################################################################


//################################################################################################################################
/*
 _   __    _______                                   ______         _                  ___          _             _
| | / /   | | ___ \                                  | ___ \       | |                / _ \        (_)           | |
| |/ /  __| | |_/ / __ ___   ___ _   _ _ __ __ _ _ __| |_/ /_ _  __| |_ __ __ _  ___ / /_\ \___ ___ _ _ __   __ _| |_ _   _ _ __ __ _
|    \ / _` |  __/ '__/ _ \ / __| | | | '__/ _` | '__|  __/ _` |/ _` | '__/ _` |/ _ \|  _  / __/ __| | '_ \ / _` | __| | | | '__/ _` |
| |\  \ (_| | |  | | | (_) | (__| |_| | | | (_| | |  | | | (_| | (_| | | | (_| | (_) | | | \__ \__ \ | | | | (_| | |_| |_| | | | (_| |
\_| \_/\__,_\_|  |_|  \___/ \___|\__,_|_|  \__,_|_|  \_|  \__,_|\__,_|_|  \__,_|\___/\_| |_/___/___/_|_| |_|\__,_|\__|\__,_|_|  \__,_|

	ultima modificação: 01/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
*/

/// <summary>
///		Esse método procura um padrão de assinatura em um processo
/// </summary>
/// <param name="ModuleBase">ModuloBase do processo</param>
/// <param name="ModuleSize">Tamanho da image base</param>
/// <param name="padrao">Padrão(Linguição de bytes)</param>
/// <param name="mascara">Mascara de busca</param>
/// <returns>status_success</returns>
NTSTATUS KdProcurarPadraoAssinatura(UINT64 ModuleBase, UINT64 ModuleSize, BYTE* padrao, char* mascara);
//################################################################################################################################


//################################################################################################################################
/*

 _   __    _ _____ _     _             _           _   _          ______                                  _   _                     _       _____           ______ ___________
| | / /   | |  _  | |   | |           | |         | | | |         | ___ \                                | | | |                   | |     /  ___|          | ___ \_   _|  _  \
| |/ /  __| | | | | |__ | |_ ___ _ __ | |__   __ _| | | |_ __ ___ | |_/ / __ ___   ___ ___  ___ ___  ___ | | | |___  __ _ _ __   __| | ___ \ `--.  ___ _   _| |_/ / | | | | | |
|    \ / _` | | | | '_ \| __/ _ \ '_ \| '_ \ / _` | | | | '_ ` _ \|  __/ '__/ _ \ / __/ _ \/ __/ __|/ _ \| | | / __|/ _` | '_ \ / _` |/ _ \ `--. \/ _ \ | | |  __/  | | | | | |
| |\  \ (_| \ \_/ / |_) | ||  __/ | | | | | | (_| | |_| | | | | | | |  | | | (_) | (_|  __/\__ \__ \ (_) | |_| \__ \ (_| | | | | (_| | (_) /\__/ /  __/ |_| | |    _| |_| |/ /
\_| \_/\__,_|\___/|_.__/ \__\___|_| |_|_| |_|\__,_|\___/|_| |_| |_\_|  |_|  \___/ \___\___||___/___/\___/ \___/|___/\__,_|_| |_|\__,_|\___/\____/ \___|\__,_\_|    \___/|___/

	ultima modificação: 01/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
*/

/// <summary>
///		Esse método obtem uma estrutura PEPROCESS através de um PID
/// </summary>
/// <param name="PID">PID do processo</param>
/// <returns>status_sucess</returns>
PEPROCESS KdObtenhaUmProcessoUsandoSeuPID(HANDLE PID);
//################################################################################################################################


//################################################################################################################################
/*
	
 _   __    _ _____     _     _      ______                                 ______ ___________ 
| | / /   | |  ___|   (_)   | |     | ___ \                                | ___ \_   _|  _  \
| |/ /  __| | |____  ___ ___| |_ ___| |_/ / __ ___   ___ ___  ___ ___  ___ | |_/ / | | | | | |
|    \ / _` |  __\ \/ / / __| __/ _ \  __/ '__/ _ \ / __/ _ \/ __/ __|/ _ \|  __/  | | | | | |
| |\  \ (_| | |___>  <| \__ \ ||  __/ |  | | | (_) | (_|  __/\__ \__ \ (_) | |    _| |_| |/ / 
\_| \_/\__,_\____/_/\_\_|___/\__\___\_|  |_|  \___/ \___\___||___/___/\___/\_|    \___/|___/  
                                                                                              
	ultima modificação: 01/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
*/

/// <summary>
///		Esse método verifica se um processo existe com base no PID
/// </summary>
/// <param name="peprocess">estrutura do PEPROCESS</param>
/// <param name="PID">PID do processo</param>
/// <returns>status_sucess</returns>
NTSTATUS KdExisteProcessoPID(PEPROCESS peprocess, HANDLE PID);
//################################################################################################################################


//################################################################################################################################
/*

 _____                           _                     _ _____       _ _ _                _
|_   _|                         | |                   | /  __ \     | | | |              | |
  | | _ __ ___   __ _  __ _  ___| |     ___   __ _  __| | /  \/ __ _| | | |__   __ _  ___| | __
  | || '_ ` _ \ / _` |/ _` |/ _ \ |    / _ \ / _` |/ _` | |    / _` | | | '_ \ / _` |/ __| |/ /
 _| || | | | | | (_| | (_| |  __/ |___| (_) | (_| | (_| | \__/\ (_| | | | |_) | (_| | (__|   <
 \___/_| |_| |_|\__,_|\__, |\___\_____/\___/ \__,_|\__,_|\____/\__,_|_|_|_.__/ \__,_|\___|_|\_\
					   __/ |
					  |___/
	ultima modificação: 02/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
*/

/// <summary>
///		Essa callback registra dados de registro de processos carregados no sistema operacional
/// </summary>
/// <param name="NomeDoPE">Nome do PE carregado e caminho</param>
/// <param name="PID">PID do processo</param>
/// <param name="PeInfo">Informações do processo</param>
/// <returns>status_sucess</returns>
PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING NomeDoPE, HANDLE PID, PIMAGE_INFO PeInfo);
//################################################################################################################################
}