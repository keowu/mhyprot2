#pragma warning( disable: 4100 4103 4146 4213) // CHEGA CHEGA EU NÃO AGUENTO MAIS (Zoio, 2018), Justificativa, não tem como ler e fazer a conversão de tipos de uma forma mais otimizada
#include "memorymng.h"


extern "C" {

	// Obrigado pela solução
	// Essa parte foi implementada estudando um excelente post feito em:
	// https://www.unknowncheats.me/forum/1897834-post25.html
	// https://www.unknowncheats.me/forum/programming-for-beginners/330716-kernel-pattern-scanning-bsod-doesnt-sense.html
	// sua duvida acabou ajudando a aprender uma implementação mais aprimorada, obrigado UknownCheats
	// Obrigado pelo aprendizado compartilhado comigo :D
	BOOLEAN KdcompararDados(const BYTE* pData, const BYTE* bMask, const char* szMask) {
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask)
				return FALSE;


		return !*szMask;
	}


	BOOLEAN KdencontrarAssinatura(UINT64 dwAddress, UINT64 dwLen, BYTE* bMask, char* szMask) {

		for (UINT64 i = 0; i < dwLen; i++)
			if (KdcompararDados((BYTE*)(dwAddress + i), bMask, szMask))
				return TRUE;

		return FALSE;
		
	}


	NTSTATUS kdLerMemoriaDoJogo(DWORD PID, PEPROCESS PEProcess, PVOID Address, DWORD Size, PVOID Buffer) {
		PEPROCESS selectedprocess = PEProcess;
		NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;

		if (PEProcess == NULL)
		{
			if (!NT_SUCCESS(PsLookupProcessByProcessId((PVOID)(UINT_PTR)PID, &selectedprocess)))
				return FALSE; // Caso não seja possível obter o PID

		}
		__try
		{
			KeAttachProcess((PEPROCESS)selectedprocess);



			__try
			{
				char* target;
				char* source;


				target = (char*)Buffer;
				source = (char*)Address;

				if (((UINT_PTR)source < 0x8000000000000000ULL) && MmIsAddressValid(source))
				{
					RtlCopyMemory(target, source, Size);
					ntStatus = STATUS_SUCCESS;
				}

			}
			__finally
			{

				KeDetachProcess();
			}
		}
		__except (1)
		{
			ntStatus = STATUS_UNSUCCESSFUL;
		}

		if (PEProcess == NULL) //nenhum peprocess valido foi retornado então é feito uma desreferência
			ObDereferenceObject(selectedprocess);

		return NT_SUCCESS(ntStatus);
	}

	NTSTATUS kdEscreverMemoriaDoJogo(HANDLE PID, PVOID pEnderecoDestino, PVOID pEnderecoOrigem, SIZE_T tamanho) {
		PSIZE_T pRetornorno = NULL;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//ESA ROTINA SERA MELHORADA SE NO FUTURO EU DER CONTINUIDADE
		//A PARTIR DA VERSÃO MAIS NOVA DO WINDOWS ISSO DARA UMA TELA AZUL !
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		NTSTATUS status = MmCopyVirtualMemory(PsGetCurrentProcess(),
					      pEnderecoOrigem,
						  KdObtenhaUmProcessoUsandoSeuPID(PID),
						  pEnderecoDestino,
						  tamanho,
						  KernelMode,
						  pRetornorno);

		if (!NT_SUCCESS(status)) {
			DbgPrint("Não foi possível escrever na memória do processo");
			return STATUS_UNSUCCESSFUL;
		}
		return STATUS_SUCCESS;
	}

}