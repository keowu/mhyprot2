#include <string.h>
#include <ntifs.h>
#include <ntddk.h>
#include <windef.h>
#include "events.h"

//typedef char int8;
//typedef int8 BYTE;


extern "C" {

	//################################################################################################################################
	/*
		
	 _   __    _                                               ______          _           
	| | / /   | |                                              |  _  \        | |          
	| |/ /  __| | ___ ___  _ __ ___  _ __   __ _ _ __ __ _ _ __| | | |__ _  __| | ___  ___ 
	|    \ / _` |/ __/ _ \| '_ ` _ \| '_ \ / _` | '__/ _` | '__| | | / _` |/ _` |/ _ \/ __|
	| |\  \ (_| | (_| (_) | | | | | | |_) | (_| | | | (_| | |  | |/ / (_| | (_| | (_) \__ \
	\_| \_/\__,_|\___\___/|_| |_| |_| .__/ \__,_|_|  \__,_|_|  |___/ \__,_|\__,_|\___/|___/
	                                | |                                                    
	                                |_|  
	ultima modificação: 02/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
	*/
	/// <summary>
	///		Esse método compara dados da memória virtual com base em um conjunto de padrão de assinatura gerado no IDA
	/// </summary>
	/// <param name="pData">Endereço de busca</param>
	/// <param name="bMask">Mascara de bytes</param>
	/// <param name="szMask">Mascara de comparação</param>
	/// <returns>Se o padrão foi encontrado com sucesso</returns>
	BOOLEAN KdcompararDados(const BYTE* pData, const BYTE* bMask, const char* szMask);
	//################################################################################################################################


	//################################################################################################################################
	/*
		
		 _   __    _                            _                   ___          _             _                   
		| | / /   | |                          | |                 / _ \        (_)           | |                  
		| |/ /  __| | ___ _ __   ___ ___  _ __ | |_ _ __ __ _ _ __/ /_\ \___ ___ _ _ __   __ _| |_ _   _ _ __ __ _ 
		|    \ / _` |/ _ \ '_ \ / __/ _ \| '_ \| __| '__/ _` | '__|  _  / __/ __| | '_ \ / _` | __| | | | '__/ _` |
		| |\  \ (_| |  __/ | | | (_| (_) | | | | |_| | | (_| | |  | | | \__ \__ \ | | | | (_| | |_| |_| | | | (_| |
		\_| \_/\__,_|\___|_| |_|\___\___/|_| |_|\__|_|  \__,_|_|  \_| |_/___/___/_|_| |_|\__,_|\__|\__,_|_|  \__,_|

	ultima modificação: 02/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
	*/
	/// <summary>
	///		Esse método compara e procura padrões em uma image baze com base no size dela e comparando mascara de padrão
	/// </summary>
	/// <param name="dwAddress">BASE</param>
	/// <param name="dwLen">Tamanho</param>
	/// <param name="bMask">Mascara de bytes</param>
	/// <param name="szMask">Padrão de assinatura</param>
	/// <returns>Se foi encontrado o padrão no binário</returns>
	BOOLEAN KdencontrarAssinatura(UINT64 dwAddress, UINT64 dwLen, BYTE* bMask, char* szMask);
	//################################################################################################################################


	//################################################################################################################################
	/*
		
	 _   _ _____ ___  ______ _____      ___  ___          _____                   _   _ _      _               ____  ___                                
	| \ | |_   _/ _ \ | ___ \_   _|     |  \/  |         /  __ \                 | | | (_)    | |             | |  \/  |                                
	|  \| | | |/ /_\ \| |_/ / | |       | .  . |_ __ ___ | /  \/ ___  _ __  _   _| | | |_ _ __| |_ _   _  __ _| | .  . | ___ _ __ ___   ___  _ __ _   _ 
	| . ` | | ||  _  ||  __/  | |       | |\/| | '_ ` _ \| |    / _ \| '_ \| | | | | | | | '__| __| | | |/ _` | | |\/| |/ _ \ '_ ` _ \ / _ \| '__| | | |
	| |\  | | || | | || |    _| |_      | |  | | | | | | | \__/\ (_) | |_) | |_| \ \_/ / | |  | |_| |_| | (_| | | |  | |  __/ | | | | | (_) | |  | |_| |
	\_| \_/ \_/\_| |_/\_|    \___/      \_|  |_/_| |_| |_|\____/\___/| .__/ \__, |\___/|_|_|   \__|\__,_|\__,_|_\_|  |_/\___|_| |_| |_|\___/|_|   \__, |
	                                                                 | |     __/ |                                                                 __/ |
	                                                                 |_|    |___/                                                                 |___/ 
	
	ultima modificação: 02/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
	*/
	/// <summary>
	///		Método não documentado NTDk
	/// </summary>
	/// Obrigado para: http://www.codewarrior.cn/ntdoc/wrk/mm/MmCopyVirtualMemory.htm
	NTSTATUS NTAPI MmCopyVirtualMemory
	(
		PEPROCESS SourceProcess,
		PVOID SourceAddress,
		PEPROCESS TargetProcess,
		PVOID TargetAddress,
		SIZE_T BufferSize,
		KPROCESSOR_MODE PreviousMode,
		PSIZE_T ReturnSize
	);
	//################################################################################################################################


	//################################################################################################################################
	/*
	
	 _       _ _              ___  ___                          _      ______        ___
	| |     | | |             |  \/  |                         (_)     |  _  \      |_  |
	| | ____| | |     ___ _ __| .  . | ___ _ __ ___   ___  _ __ _  __ _| | | |___     | | ___   __ _  ___
	| |/ / _` | |    / _ \ '__| |\/| |/ _ \ '_ ` _ \ / _ \| '__| |/ _` | | | / _ \    | |/ _ \ / _` |/ _ \
	|   < (_| | |___|  __/ |  | |  | |  __/ | | | | | (_) | |  | | (_| | |/ / (_) /\__/ / (_) | (_| | (_) |
	|_|\_\__,_\_____/\___|_|  \_|  |_/\___|_| |_| |_|\___/|_|  |_|\__,_|___/ \___/\____/ \___/ \__, |\___/
																								__/ |
																							   |___/
	
	ultima modificação: 02/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
	*/
	/// <summary>
	///		Esse método lê a memória de um processo no ring de usuário a partir do kernel
	/// </summary>
	/// <param name="PID">PID do processo</param>
	/// <param name="PEProcess">Estrutura do processo</param>
	/// <param name="Address">Endereço</param>
	/// <param name="Size">Tamanho</param>
	/// <param name="Buffer">Retorno</param>
	/// <returns>STATUS_SUCESS</returns>
	/// Obrigado para:
	/// https://github.com/cheat-engine/cheat-engine/blob/8bdb7f28a9d40ccaa6f4116b13c570907ce9ee2e/DBKKernel/memscan.c
	/// https://github.com/cheat-engine/cheat-engine/search?l=C&q=ReadProcessMemory
	/// https://github.com/cheat-engine/cheat-engine/blob/8bdb7f28a9d40ccaa6f4116b13c570907ce9ee2e/DBKKernel/memscan.c#L267
	/// https://github.com/cheat-engine/cheat-engine/issues/1120
	NTSTATUS kdLerMemoriaDoJogo(DWORD PID, PEPROCESS PEProcess, PVOID Address, DWORD Size, PVOID Buffer);
	//################################################################################################################################

	//################################################################################################################################
	/*
		
	 _       _ _____                                ___  ___                          _      ______        ___                   
	| |     | |  ___|                               |  \/  |                         (_)     |  _  \      |_  |                  
	| | ____| | |__ ___  ___ _ __ _____   _____ _ __| .  . | ___ _ __ ___   ___  _ __ _  __ _| | | |___     | | ___   __ _  ___  
	| |/ / _` |  __/ __|/ __| '__/ _ \ \ / / _ \ '__| |\/| |/ _ \ '_ ` _ \ / _ \| '__| |/ _` | | | / _ \    | |/ _ \ / _` |/ _ \ 
	|   < (_| | |__\__ \ (__| | |  __/\ V /  __/ |  | |  | |  __/ | | | | | (_) | |  | | (_| | |/ / (_) /\__/ / (_) | (_| | (_) |
	|_|\_\__,_\____/___/\___|_|  \___| \_/ \___|_|  \_|  |_/\___|_| |_| |_|\___/|_|  |_|\__,_|___/ \___/\____/ \___/ \__, |\___/ 
	                                                                                                                  __/ |      
	                                                                                                                 |___/       
	
	ultima modificação: 02/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
	*/
	/// <summary>
	///		Esse método escreve na memória de um processo
	/// </summary>
	/// <param name="PID">PID do Processo</param>
	/// <param name="pEnderecoDestino">Endereço para escrever</param>
	/// <param name="pEnderecoOrigem">Endereço de origem</param>
	/// <param name="tamanho">Tamanho</param>
	/// <returns>status_sucess</returns>
	NTSTATUS kdEscreverMemoriaDoJogo(HANDLE PID, PVOID pEnderecoDestino, PVOID pEnderecoOrigem, SIZE_T tamanho);
	//################################################################################################################################
}