#include "events.h"
#include "comunication.h"

extern "C" {
/// <summary>
///		GetProcessNameByProcessID - Método responsável por retornar o nome de um processo a partir de seu PID
/// </summary>
/// <param name="PID">PID do processo/module</param>
/// <returns>Vamos retornar o nome do processo se ele existir | do contrário retornamos falso com a Macro do kernel API</returns>
char* GetProcessNameByProcessID(HANDLE PID) {
	//Vamos criar uma flag de estado para o retorno das API'S
	NTSTATUS status;
	// vamos inicializar uma estrutura da kernel usada para armazenar processo PE
	PEPROCESS EProcess = NULL;
	// A partir do PID é possível encontrar o processo e recuperar informações da SSDT
	status = PsLookupProcessByProcessId(PID, &EProcess);
	// se o estado não for um sucesso
	if (!NT_SUCCESS(status))
	{
		// Vamos retornar falso para o chamador do método
		return FALSE;
	}
	// Vamos desreferenciar o EPROCESS criado
	ObDereferenceObject(EProcess);
	// vamos obter o nome da imagem a partir de um extern da kernel e retornar um ponteiro para seu nome
	return (char*)PsGetProcessImageFileName(EProcess);
}


/// <summary>
///		KeMhYCallbacks - Método que vai interceptar os HANDLES
///		Podemos negar handles para operações de Leitura e Escrita na memória de determinado processo, ou até que ele seja encerrado
/// </summary>
/// <param name="ContextoDeRegistro">Contexto de Registro da callback</param>
/// <param name="pInformacoesDeOperacoes">Informações das operações da memória</param>
/// <returns>OB_PREOP_SUCCESS estado de sucessso para método sobreposto</returns>
OB_PREOP_CALLBACK_STATUS KeMhYCallbacks(PVOID ContextoDeRegistro, POB_PRE_OPERATION_INFORMATION pInformacoesDeOperacoes) {
	// Obtemos o PID do contexto do processo atual que foi enviado pelo sistema
	HANDLE PID = PsGetProcessId((PEPROCESS)pInformacoesDeOperacoes->Object);
	// Vamos definir um char array para armazenar o tamanho do processo e comparar posteriormente
	char szNomeDoProcesso[16] = { 0 };
	// Vamos desreferenciar o parâmetro inutilizado
	UNREFERENCED_PARAMETER(ContextoDeRegistro);
	//Vamos copiar o nome do processo para nosso char array, usando o nosso método para obter o nome a partir do seu PID
	strcpy(szNomeDoProcesso, GetProcessNameByProcessID(PID));
	//Vamos comparar se o nome do processo atual é do Jogo Genshin Impact
	if (!_stricmp(szNomeDoProcesso, "notepad.exe"))//"GenshinImpact.exe"
	{
		// Vamos verificar se a operação do sistema operacional é a de criação de HANDLES
		if (pInformacoesDeOperacoes->Operation == OB_OPERATION_HANDLE_CREATE)
		{
			// Vamos verificar se o estado do acesso foi para encerrar o processo
			if ((pInformacoesDeOperacoes->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_TERMINATE) == PROCESS_TERMINATE)
			{
				// vamos negar a permissão para encerrar um processo
				pInformacoesDeOperacoes->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
				DbgPrintEx(0, 0, "Foi detectado uma tentativa de encerrar nosso processo protegido !\n");
			}
			// Vamos verificar se o estado do acesso foi para uma operação de inicio
			if ((pInformacoesDeOperacoes->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_OPERATION) == PROCESS_VM_OPERATION)
			{
				// Vamos negar a permissão e não permitir que o processo seja iniciado
				pInformacoesDeOperacoes->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_OPERATION;
				DbgPrintEx(0, 0, "Foi detectado uma tentativa de início do processo protegido !\n");
			}
			// Vamos verificar se o estado do acesso foi para readprocessmemory/funções de leitura de memória de um processo
			if ((pInformacoesDeOperacoes->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_READ) == PROCESS_VM_READ)
			{
				// Vamos nengar a permissão de acesso para leitura do processo
				pInformacoesDeOperacoes->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_READ;
				DbgPrintEx(0, 0, "Foi detectado uma tentativa de leitura da memória nosso processo protegido !\n");
			}
			// Vamos verificar se o estado do acesso foi para writeprocessmemory/função de escrita na memória do processo
			if ((pInformacoesDeOperacoes->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_WRITE) == PROCESS_VM_WRITE)
			{
				// Vamos negar a permissão de acesso para a escrita na memória do processo
				pInformacoesDeOperacoes->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_WRITE;
				DbgPrintEx(0, 0, "Foi detectado uma tentativa de escrita na memória do nosso processo protegido !\n");
				//Se alguem tentar escrever na memória do jogo
				KeMhYTerminateProcess(PID);
			}
		}
	}
	// vamos retornar que a operações foi feita com sucesso para o callback
	return OB_PREOP_SUCCESS;
}


/// <summary>
///		KeMhYTerminateProcess - Encerra um processo no nível de usuário por meio do seu PID
/// </summary>
/// <param name="PID">PID(PROCESS ID) DO PROCESSO NO QUAL DESEJA-SE ENCERRAR</param>
/// <returns>RETORNAMOS UMA FLAG DA NTSTATUS, SENDO ELA DE SUCESSO OU DE ERRO</returns>
NTSTATUS KeMhYTerminateProcess(HANDLE PID) {
	DbgPrint("Tentando matar essa porra");
	// Criamos uma flag de NTSTATUS para armazenar nosso estado de retorno das winapi
	NTSTATUS status;
	// Criamos um PEPROCESS para guardar a referência do processo encontrado
	PEPROCESS Peprocess;
	// Criamos o process handle para armazenar o handle do nosso processo no qual queremos fechar
	HANDLE ProcessHandle;
	// Se Tivermos um PID
	if (PID) {
		// Vamos inicializar o PROCESSHANDLE
		ProcessHandle = NULL;
		// Vamos inicialiazr nosso PEPROCESS
		Peprocess = NULL;
		// Vamos procurar o processo usando o PID
		status = PsLookupProcessByProcessId(PID, &Peprocess);
		// Vamos verificar se Peprocess não é zero, se temos algo nele
		if (Peprocess) {
			// Vamos abrir o processo pelo PEPROCESS e assumir seu controle armazenando o resultado do novo HANDLE em ProcessHandle
			status = ObOpenObjectByPointer(Peprocess, 0, NULL, 0, NULL, 0, &ProcessHandle);
			// Vamos verifciar se não ocorreu um erro ao tentar obter o HANDLE do processo
			if (!NT_SUCCESS(status)) {
				// Se algo deu errado vamos retornar que ocorreu um erro e que não podemos trabalhar com o processo
				return STATUS_ERROR_PROCESS_NOT_IN_JOB;
			}
			else {
				// Vamos encerrar o processo pelo HANDLE que encontramos passando o código zero para ele
				ZwTerminateProcess(ProcessHandle, 0);
				// Vamos fechar o HANDLE criado para o PROCESSO
				ZwClose(ProcessHandle);
				//Vamos desreferenciar nosso processo
				ObDereferenceObject(Peprocess);
			}
		}
	}
	// Vamos retornar o estado de sucesso para o chamados do método
	return STATUS_SUCCESS;
}

/// <summary>
///		KeMhYEnumModulesOfProcess - Obtem o nome de um processo e seus modulos carregados da PEB a partir de um PID
/// </summary>
/// <param name="PID">PID(Process ID) do processo no qual deseja-se buscar</param>
/// <returns>Retorna a flag de sucesso ou erro NTSTATUS</returns>
NTSTATUS KeMhYEnumModulesOfProcess(HANDLE PID) {
	// Criamos um PEPROCESS para armazenar a referência do processo encontrado
	PEPROCESS peProcess = NULL;
	// Definimos uma flag de NTSTATUS para armazenar o retorno das API do sistema operacional
	NTSTATUS status;
	// SIZE_T DA PEB
	SIZE_T PEB = 0;
	// SIZE_T DA LDR
	SIZE_T LDR = 0;
	// NOSSA LISTA DE MODULOS COM CABEÇALHOS
	PLIST_ENTRY ModListHead = 0;
	// NOSSA LISTA DE MODULOS
	PLIST_ENTRY Module = 0;
	// NOSSO ESTADO
	KAPC_STATE ks;
	// CONTADOR DE MODULOS NA MEMÓRIA
	int contagem_modulos = 0;
	// Char para armazenar o nome do processo
	char szNomeDoProcesso[20] = { 0 };
	// Obtendo o nome do processo pelo seu PID
	strcpy(szNomeDoProcesso, GetProcessNameByProcessID(PID));
	// Recuperando informações do PEPROCESS pelo PID
	status = PsLookupProcessByProcessId(PID, &peProcess);
	// Verificando se foi possível recuperar dados
	if (!NT_SUCCESS(status)) {
		// retornando erro caso não tenha sido possível recuperar dados
		return STATUS_ERROR_PROCESS_NOT_IN_JOB;
	}
	// DBGPRINT para mostrar o processo
	DbgPrintEx(0, 0, "Listando processos para: %s \n", szNomeDoProcesso);
	//Verificando se temos dados no PEPROCESS
	if (!peProcess) {
		// retornamos erro caso não tenha sido possível recuperar os dados
		return STATUS_ERROR_PROCESS_NOT_IN_JOB;
	}
	// Recuperando informações da PEB usando a extern e o PEPROCESS
	PEB = (SIZE_T)PsGetProcessPeb(peProcess);
	// Verificando se temos algo na size_t da PEB
	if (!PEB) {
		// Se não tivermos nada retornamos erro
		return STATUS_ERROR_PROCESS_NOT_IN_JOB;
	}
	// Atacando a stack do processo e armazenando seu estado
	KeStackAttachProcess(peProcess, &ks);
	

	/// <summary>
	///		ATENÇÃO AQUI EM BAIXO ERA UMA ROTINA PROTEGIDA PELA TECNOLOGIA DE PACKING DO DRIVER, EU CONSEGUI RECONSTRUIR
	///		POREM É MUITO PROVAVEL QUE NO ORIGINAL ISSO SEJA COMPLETAMENTE DIFERENTE, POREM O COMPORTAMENTO É O MESMO.
	///		
	///		CRÉDITOS:
	///			OBRIGADO A TODOS OS PESQUISADORES QUE POSTARAM PRINT DA ESTRUTURA IOTCL SEM ELA NÃO SERIA POSSÍVEL RECONSTRUIR, ERA NECESSÁRIO ENTENDER O PROTOCOLO
	/// </summary>
	__try
	{
		// VAMOS OBTER A PEB
		LDR = PEB + (SIZE_T)0x018; // Offset LDR da PEB
		// LENDO
		ProbeForRead((CONST PVOID)LDR, 8, 8);
		// VAMOS OBTER O CABEÇALHO DO MODLISTHEAD
		ModListHead = (PLIST_ENTRY)(*(PULONG64)LDR + 0x010); //InLoadOrderModuleList
		// LENDO
		ProbeForRead((CONST PVOID)ModListHead, 8, 8);
		// DANDO UM FLINK E RECUPERANDO O MODULO
		Module = ModListHead->Flink;
		// ENQUANTO O MODULO FOR DIFERENTE DO FINAL
		while (ModListHead != Module) {
			//PRINTANDO NO DEBUGGER OS DADOS DOS MODULOS
			DbgPrintEx(0, 0, "Module Base: %p, Tamanho: %ld, Module Path: %wZ\n",
					   (PVOID)(((PLDR_DATA_TABLE_ENTRY)Module)->DllBase),
					   (ULONG)(((PLDR_DATA_TABLE_ENTRY)Module)->SizeOfImage),
					   &(((PLDR_DATA_TABLE_ENTRY)Module)->FullDllName));
			// DANDO FLINK PARA ANDAR PELOS MODULOS
			Module = Module->Flink;
			// INCREMENTANDO O CONTADOR
			contagem_modulos++;
			// LENDO
			ProbeForRead((CONST PVOID)Module, 80, 8);
		}
		// PRINTANDO NO DEBUGGER A QUANTIDADE DE MÓDULOS CARREGADOS PELO PROCESSO
		DbgPrintEx(0, 0, "Quantidade de modulos: %d \n", contagem_modulos);
	}
	// VERIFCANDO EXCESSÕES
	__except (EXCEPTION_EXECUTE_HANDLER) 
	{
		// EXIBINDO ERRO DE LEITURA
		DbgPrintEx(0, 0, "Não foi possível ler a PEB");
	}
	// DANDI DETACH DA STACK
	KeUnstackDetachProcess(&ks);
	// RETORNANDO O ESTADO DE SUCESSO !
	return STATUS_SUCCESS;
}


/// <summary>
///		Esse método buscara por padrões em assinaturas de opcodes, baseado nos opcodes pegos em memória e usando uma mascara
/// </summary>
/// <param name="ModuleBase">Módule base do PE na memória</param>
/// <param name="ModuleSize">Tamanho da imagem PE na memória</param>
/// <param name="padrao">Padrão const char*(macro de tipo definido byte), contendo o padrão de opcodes da memória</param>
/// <param name="mascara">Mascara para cada opcode da emmória, sendo substituido por ? os opcodes que variam</param>
/// <returns>STATUS_SUCCESS ou STATUS_UNSUCCESSFUL</returns>
NTSTATUS KdProcurarPadraoAssinatura(UINT64 ModuleBase, UINT64 ModuleSize, BYTE* padrao, char* mascara) {

	//Esse método vai retorna true se o processo bater com o padrão de opcodes e a macara
	if (KdencontrarAssinatura(ModuleBase, ModuleSize, padrao, mascara)) {
		DbgPrintEx(0, 0, "SUCESSO, FOI ENCONTRADO NESSE PROCESSO UM NOME ERRADO !!");
		return STATUS_SUCCESS; // Se for encontrado
	}
	else {
		DbgPrintEx(0, 0, "NÃO FOI ENCONTRADO NADA NESSE PROCESSO !");
		return STATUS_UNSUCCESSFUL; // Se não for encontrado
	}

}

/// <summary>
///		Esse método buscara por um processo usando seu PID
/// </summary>
/// <param name="PID">PID do processo</param>
/// <returns>PEPROCESS struct com a referência para o processo em questão</returns>
PEPROCESS KdObtenhaUmProcessoUsandoSeuPID(HANDLE PID) {
	PEPROCESS processo; //Criando a referência para um PEPROCESS struct
	if (NT_SUCCESS(PsLookupProcessByProcessId(PID, &processo))) { // RECUPERANDO A REFERêNCIA PARA O PEPROCESS
		return processo; // Retornando o PEPROCESS struct preenchida
	}
	else {
		return NULL; // Retornando NULL pois não foi encontrada a referência
	}
}

/*
	Esse método verifica se existe um processo com aquele PID em execução
	retorna a flag status_sucess se o processo for encontrado no contexto do sistema operacional
*/
NTSTATUS KdExisteProcessoPID(PEPROCESS peprocess, HANDLE PID) { return PsLookupProcessByProcessId(PID, &peprocess); }


/*
	ImageLoadCallBack routine, essa rotina faz um hook no contexto de load de PE, DLL, etc
	e devolve informações úteis para tomada de decisão pelo Driver
*/
PLOAD_IMAGE_NOTIFY_ROUTINE ImageLoadCallback(PUNICODE_STRING PE, HANDLE PID, PIMAGE_INFO PeInfo) {
	
	// Esse processo carregado é o processo que eu deveria proteger ?
	if (wcsstr(PE->Buffer, KdGetProcessBlackList())) {
	DbgPrint("PE: %ls", PE->Buffer); // Unicode posso usar o buffer para obter um PWCH
		KdSetBaseAddrRW(PeInfo->ImageBase); // Recuperamos o imageBase na memória virtual
		KdSetPidAddrRW(PID); // Recuperamos o PID
	}
	UNREFERENCED_PARAMETER(PE); // Desreferenciamos as referências não usadas
	UNREFERENCED_PARAMETER(PID);  // Desreferenciamos as referências não usadas
	UNREFERENCED_PARAMETER(PeInfo);  // Desreferenciamos as referências não usadas
	return STATUS_SUCCESS; // Retornamos ao contexto do sistema a flag de sucesso
}

}