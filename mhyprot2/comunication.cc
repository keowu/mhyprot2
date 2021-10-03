#pragma warning( disable: 4311 4302)
#include "comunication.h"

extern "C" {

	PDEVICE_OBJECT devObject = NULL;

	HANDLE cmdPID = 0;
	PVOID cmdBaseAdress = 0;
	WCHAR processBlackList[30];//= (PVOID)(const wchar_t*)L"\\msedge.exe";//L"\\cmd.exe";

	VOID KdSetBaseAddrRW(PVOID addr) {
		cmdBaseAdress = addr;
		DbgPrint("Definindo base: %p", cmdBaseAdress);
	}

	VOID KdSetPidAddrRW(HANDLE PID) {
		cmdPID = PID;
		DbgPrint("Handle: %ul", cmdPID);
	}

	const wchar_t* KdGetProcessBlackList() {
		return (const wchar_t*)processBlackList;
	}

	VOID KdCreateDeviceSymbolLink(PDRIVER_OBJECT driverObj) {
		UNICODE_STRING ntDevN = { 0 };
		UNICODE_STRING dosDevN = { 0 };
		NTSTATUS status = STATUS_SUCCESS;
		PDEVICE_EXTENSION devExt = NULL;
		RtlInitUnicodeString(&ntDevN, MHYPROT2_DEVICE_NAME_W);

		status = IoCreateDevice(driverObj,
			sizeof(PDEVICE_EXTENSION),
			&ntDevN,
			FILE_DEVICE_UNKNOWN,
			0,
			TRUE,
			&devObject);

		if (!NT_SUCCESS(status)) {
			DbgPrint("Ocorreu um problema ao criar o dispositivo IOCTL");
			return;
		}

		driverObj->Flags |= DO_BUFFERED_IO;

		devExt = (PDEVICE_EXTENSION)devObject->DeviceExtension;

		RtlInitUnicodeString(&dosDevN, MHYPROT2_DOS_DEVICE_NAME_W);

		status = IoCreateSymbolicLink(&dosDevN, &ntDevN);

		if (!NT_SUCCESS(status)) {
			DbgPrint("Ocorreu um problema ao definir um link para o dispositivo IOCTL");
			return;
		}

		driverObj->MajorFunction[IRP_MJ_CREATE] = driverObj->MajorFunction[IRP_MJ_CLOSE] = driverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = KdMhyprot2DeviceCtrlRoutine;
		DbgPrint("Sucesso IOCTL");
	}

	/// <summary>
	///		Rotina de controller do driver
	/// </summary>
	/// <param name="DevObj">PDEVICE_OBJECT</param>
	/// <param name="Irp">PIRP</param>
	/// <returns>STATUS_SUCESS</returns>
	NTSTATUS KdMhyprot2DeviceCtrlRoutine(PDEVICE_OBJECT DevObj, PIRP Irp) {
		NTSTATUS status = STATUS_SUCCESS;
		PIO_STATUS_BLOCK ioStat;
		PIO_STACK_LOCATION pIRP;
		PDEVICE_EXTENSION	devExt;
		PVOID				inpBuff, outBuff;
		ULONG				inpBuffLen, outBuffLen;
		ULONG				ioControlCode;

		pIRP = IoGetCurrentIrpStackLocation(Irp);
		devExt = (PDEVICE_EXTENSION)DevObj->DeviceExtension;

		ioStat = &Irp->IoStatus;
		ioStat->Status = STATUS_SUCCESS;
		ioStat->Information = 0;

		ioControlCode = pIRP->Parameters.DeviceIoControl.IoControlCode;

		switch (pIRP->MajorFunction)
		{
		case IRP_MJ_CREATE:
			DbgPrint("Conexão criada");
			break;

		case IRP_MJ_CLOSE:
			DbgPrint("Conexão encerrada");
			break;

		case IRP_MJ_SHUTDOWN:
			DbgPrint("Conexão excluída");
			break;

		case IRP_MJ_DEVICE_CONTROL:
			switch (ioControlCode)
			{
			case KillProcess:
			{
				inpBuff = Irp->AssociatedIrp.SystemBuffer;
				inpBuffLen = pIRP->Parameters.DeviceIoControl.InputBufferLength;
				outBuff = Irp->AssociatedIrp.SystemBuffer;
				outBuffLen = pIRP->Parameters.DeviceIoControl.OutputBufferLength;

				DbgPrint("IOCTL = %d \r\n", *(PULONG)inpBuff);

				KeMhYTerminateProcess((HANDLE)((int)*(PULONG)inpBuff));

				*(PULONG)outBuff = (*(PULONG)inpBuff) * 2;

				Irp->IoStatus.Information = sizeof(ULONG);
			}
			break;
			case ReadProcessMem:
			{
				PKERNEL_JOGO_LER_MEMORIA readRequest = (PKERNEL_JOGO_LER_MEMORIA)Irp->AssociatedIrp.SystemBuffer;
				PKERNEL_JOGO_LER_MEMORIA readToRing3 = (PKERNEL_JOGO_LER_MEMORIA)Irp->AssociatedIrp.SystemBuffer;
				
				readRequest->ENDERECO = cmdBaseAdress;
				readToRing3->RESPOSTA = cmdBaseAdress;

				DbgPrint("[MHYPROT2]_PID: %lu base: %p\n", readRequest->PID, readRequest->ENDERECO);

				PEPROCESS peProcesso = NULL;
				if (NT_SUCCESS(PsLookupProcessByProcessId(cmdPID, &peProcesso)))
					kdLerMemoriaDoJogo((DWORD)cmdPID, peProcesso, cmdBaseAdress, sizeof(int), readToRing3->RESPOSTA);

				//DbgPrint("Valor lido: %lu\n", *(ULONG*)readToRing3->RESPOSTA);

				Irp->IoStatus.Information = sizeof(PKERNEL_JOGO_LER_MEMORIA);
			}
			break;
			case SetProcessHook:
			{
				PKERNEL_PROCESSO_ANALISE KsetProcessName = (PKERNEL_PROCESSO_ANALISE)Irp->AssociatedIrp.SystemBuffer;
				PsRemoveLoadImageNotifyRoutine(
					(PLOAD_IMAGE_NOTIFY_ROUTINE)ImageLoadCallback
				);

				//DbgPrint("Antigo valor: %ls", (const wchar_t*)processBlackList);
				//processBlackList = (PVOID)KsetProcessName->nome_processo;
				RtlStringCchPrintfW(processBlackList, size_t(30), L"%ls", KsetProcessName->nome_processo);
				DbgPrint("Novo valor: %ls", (const wchar_t*)processBlackList);

				PsSetLoadImageNotifyRoutine(
					(PLOAD_IMAGE_NOTIFY_ROUTINE)ImageLoadCallback
				);

				KsetProcessName->status = 777; // 777 é o simbolo do matue, brincadeira isso é para mostrar ao ring 3 que o processo foi alterado
				Irp->IoStatus.Information = sizeof(PKERNEL_PROCESSO_ANALISE);
			}
			break;
			}
			break;
		}

		Irp->IoStatus.Status = ioStat->Status;
		Irp->IoStatus.Information = ioStat->Information;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

		return  status;

	}

	/// <summary>
	///		Esse método é responsável por scanear um padrão em um processo, para isso é necessário o seu contexto de execução
	/// </summary>
	/// <param name="NomeProcesso">Nome do processo e o contexto de busca</param>
	/// <returns>STATUS_SUCESS</returns>
	NTSTATUS KdScanProcesso(const char* NomeProcesso) {
		ULONG bytes = 0;
		NTSTATUS sucesso = ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

		PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x1337); // LEET

		sucesso = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

		PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
		UINT64 BaseProcesso = 0, TamanhoProcesso = 0;

		for (ULONG i = 0; i < modules->NumberOfModules; i++) // Varrendo os módulos e drivers carregados
		{
			DbgPrintEx(0, 0, "MHYPROT2 -> Caminho: %s \n", module[i].FullPathName);
			DbgPrintEx(0, 0, "COMPARANDO COM -> %s \n", NomeProcesso);
			if (!strcmp((char*)module[i].FullPathName, NomeProcesso)) //Encontrou o próprio driver ?
			{
				DbgPrintEx(0, 0, "Módulo encontrado !");
				BaseProcesso = (UINT64)module[i].ImageBase; // Armazena a imagebase do driver na memória virtual
				TamanhoProcesso = (UINT64)module[i].ImageSize; // Armazena o size da image
				break;
			}
		}

		if (modules)
			ExFreePoolWithTag(modules, 0x1337); // LEET, limpa a nossa pool

		DbgPrintEx(0, 0, "Base Do Modulo buscado => %p \n", BaseProcesso);
		DbgPrintEx(0, 0, "Tamanho do Modulo buscado => %p \n", TamanhoProcesso);

		KdProcurarPadraoAssinatura((UINT64)BaseProcesso, (UINT64)TamanhoProcesso, 
			(BYTE*)"\x60\xBE\x00\x00\x00\x00\x8D\xBE\x00\x00\x00\xFF", 
			"xx???xxx???x");

		return STATUS_SUCCESS;
	}

}