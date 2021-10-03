#include <ntifs.h>
#include "events.h"


extern "C" {

    /*
        Macros de definição para o IOCTL da Mihoyo que comunicara entre jogo e driver
    */
    #define DrvInit = 0x80034000 // Inicialização do driver
    #define Mdl = 0x81004000   // MDL do driver
    #define HeartBeat = 0x81014000 // 1º Heart Beat do driver
    #define HeartBeat2 = 0x80024000 // 2º Heart Beat do driver
    #define RWMemory = 0x81074000 // Leitura e Escrita de memória pelo Driver
    #define EnumProcessList = 0x83014000 // Enumeração de processos do contexto do sistema pelo Driver
    #define ListProcessModule = 0x81054000  // Enumera cada dll carregada pelos processos do contexto do sistema pelo Driver
    #define Unk1 = 0x82004000 //   Mata Processo pelo Driver
    #define EnumDrivers = 0x82024000 // Enumera todos os Drivers carregados usando o proprio Driver

    /*
        Macros de device name, usados pela Mihoyo para controlar o driver
    */
    #define MHYPROT2_WIN32_DEVICE_NAME_A		"\\\\.\\mhyprot2" // Nome do Device A
    #define MHYPROT2_WIN32_DEVICE_NAME_W		L"\\\\.\\mhyprot2" // Nome do Device W
    #define MHYPROT2_DEVICE_NAME_A			"\\Device\\mhyprot2" // Nome do Device no contexto de dispositivos A
    #define MHYPROT2_DEVICE_NAME_W			L"\\Device\\mhyprot2" // Nome do Device no contexto de dispositivos W
    #define MHYPROT2_DOS_DEVICE_NAME_A		"\\DosDevices\\mhyprot2" // Nome do Device no contexto de DOS A
    #define MHYPROT2_DOS_DEVICE_NAME_W		L"\\DosDevices\\mhyprot2" // Nome do Device no contexto de DOS W


    #define FILE_DEVICE_HIDE	0x8000 // Comando para esconder arquivos pelo Driver

    #define IOCTL_BASE			 0x800 // Comando IOCTL Base

    #define CTL_CODE_HIDE(i)	\
	CTL_CODE(FILE_DEVICE_HIDE, IOCTL_BASE+i, METHOD_BUFFERED, FILE_ANY_ACCESS) // Preparando o código de controll final

    /*
        Comandos personalizados de IOCTL
    */
    #define KillProcess CTL_CODE_HIDE(1) // Matar um processo através do Kernel
    #define ScannProcess CTL_CODE_HIDE(2) // Varrer todos os processos do sistema operacional
    #define ReadProcessMem CTL_CODE_HIDE(3) // R/W Memória
    #define SetProcessHook CTL_CODE_HIDE(4) // Definir hook em um processo

    /*
        A struct responsável pelo contexto de Extesion do Driver
    */
    typedef struct _DEVICE_EXTENSION
    {
        ULONG  Extension;
    } DEVICE_EXTENSION, * PDEVICE_EXTENSION;


    //################################################################################################################################
    /*
     _   __    _ _____                _      ______           _          _____                 _           _ _     _       _    
    | | / /   | /  __ \              | |     |  _  \         (_)        /  ___|               | |         | | |   (_)     | |   
    | |/ /  __| | /  \/_ __ ___  __ _| |_ ___| | | |_____   ___  ___ ___\ `--. _   _ _ __ ___ | |__   ___ | | |    _ _ __ | | __
    |    \ / _` | |   | '__/ _ \/ _` | __/ _ \ | | / _ \ \ / / |/ __/ _ \`--. \ | | | '_ ` _ \| '_ \ / _ \| | |   | | '_ \| |/ /
    | |\  \ (_| | \__/\ | |  __/ (_| | ||  __/ |/ /  __/\ V /| | (_|  __/\__/ / |_| | | | | | | |_) | (_) | | |___| | | | |   < 
    \_| \_/\__,_|\____/_|  \___|\__,_|\__\___|___/ \___| \_/ |_|\___\___\____/ \__, |_| |_| |_|_.__/ \___/|_\_____/_|_| |_|_|\_\
                                                                                __/ |                                           
                                                                               |___/      
    ultima modificação: 02/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
    */
    /// <summary>
    ///     Método responsável por criar um link do Driver
    /// </summary>
    /// <param name="driverObj">Objeto do Driver da memória</param>
    VOID KdCreateDeviceSymbolLink(PDRIVER_OBJECT driverObj);
    //################################################################################################################################


    //################################################################################################################################
    /*
     _   __    ____  ____                           _   _____ ______           _          _____ _        _______            _   _            
    | | / /   | |  \/  | |                         | | / __  \|  _  \         (_)        /  __ \ |      | | ___ \          | | (_)           
    | |/ /  __| | .  . | |__  _   _ _ __  _ __ ___ | |_`' / /'| | | |_____   ___  ___ ___| /  \/ |_ _ __| | |_/ /___  _   _| |_ _ _ __   ___ 
    |    \ / _` | |\/| | '_ \| | | | '_ \| '__/ _ \| __| / /  | | | / _ \ \ / / |/ __/ _ \ |   | __| '__| |    // _ \| | | | __| | '_ \ / _ \
    | |\  \ (_| | |  | | | | | |_| | |_) | | | (_) | |_./ /___| |/ /  __/\ V /| | (_|  __/ \__/\ |_| |  | | |\ \ (_) | |_| | |_| | | | |  __/
    \_| \_/\__,_\_|  |_/_| |_|\__, | .__/|_|  \___/ \__\_____/|___/ \___| \_/ |_|\___\___|\____/\__|_|  |_\_| \_\___/ \__,_|\__|_|_| |_|\___|
                               __/ | |                                                                                                       
                              |___/|_| 
    ultima modificação: 02/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
    */
    /// <summary>
    ///     Método de controle de rotina do driver e sua comunicação entre RING KERNEL E RING USUÁRIO
    /// </summary>
    /// <param name="DeviceObject">Objeto do Driver da memória</param>
    /// <param name="Irp">IRP</param>
    /// <returns>Status_sucess para contexto de caller</returns>
    NTSTATUS KdMhyprot2DeviceCtrlRoutine(PDEVICE_OBJECT DeviceObject, PIRP Irp);
    //################################################################################################################################

    /*
        Struct de thread do sistema operacional
    */
    typedef struct _SYSTEM_THREADS {
        LARGE_INTEGER  KernelTime;
        LARGE_INTEGER  UserTime;
        LARGE_INTEGER  CreateTime;
        ULONG          WaitTime;
        PVOID          StartAddress;
        CLIENT_ID      ClientId;
        KPRIORITY      Priority;
        KPRIORITY      BasePriority;
        ULONG          ContextSwitchCount;
        LONG           State;
        LONG           WaitReason;
    } SYSTEM_THREADS, * PSYSTEM_THREADS;

    /*
        Struct de processos do sistema operacional
    */
    typedef struct _SYSTEM_PROCESSES {
        ULONG            NextEntryDelta;
        ULONG            ThreadCount;
        ULONG            Reserved1[6];
        LARGE_INTEGER    CreateTime;
        LARGE_INTEGER    UserTime;
        LARGE_INTEGER    KernelTime;
        UNICODE_STRING   ProcessName;
        KPRIORITY        BasePriority;
        SIZE_T           ProcessId;
        SIZE_T           InheritedFromProcessId;
        ULONG            HandleCount;
        ULONG            Reserved2[2];
        VM_COUNTERS      VmCounters;
        IO_COUNTERS      IoCounters;
        SYSTEM_THREADS   Threads[1];
    } SYSTEM_PROCESSES, * PSYSTEM_PROCESSES;

    /*
        Struct de modulos do sistema operacional
    */
    typedef struct _RTL_PROCESS_MODULE_INFORMATION
    {
        HANDLE Section;
        PVOID MappedBase;
        PVOID ImageBase;
        ULONG ImageSize;
        ULONG Flags;
        USHORT LoadOrderIndex;
        USHORT InitOrderIndex;
        USHORT LoadCount;
        USHORT OffsetToFileName;
        UCHAR  FullPathName[256];
    } RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;
    
    /*
        Struct de rtl do sistema operacional
    */
    typedef struct _RTL_PROCESS_MODULES
    {
        ULONG NumberOfModules;
        RTL_PROCESS_MODULE_INFORMATION Modules[1];
    } RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

    // Obrigado para:
    // http://www.zhishibo.com/articles/44427.html
    // https://www.geoffchappell.com/studies/windows/km/ntoskrnl/api/ex/sysinfo/query.htm
    #define SystemModuleInformation	0x0B  


    //################################################################################################################################
    /*    
     ______        _____                       _____           _                _____       __                           _   _             
    |___  /       |  _  |                     /  ___|         | |              |_   _|     / _|                         | | (_)            
       / /_      _| | | |_   _  ___ _ __ _   _\ `--. _   _ ___| |_ ___ _ __ ___  | | _ __ | |_ ___  _ __ _ __ ___   __ _| |_ _  ___  _ __  
      / /\ \ /\ / / | | | | | |/ _ \ '__| | | |`--. \ | | / __| __/ _ \ '_ ` _ \ | || '_ \|  _/ _ \| '__| '_ ` _ \ / _` | __| |/ _ \| '_ \ 
    ./ /__\ V  V /\ \/' / |_| |  __/ |  | |_| /\__/ / |_| \__ \ ||  __/ | | | | || || | | | || (_) | |  | | | | | | (_| | |_| | (_) | | | |
    \_____/\_/\_/  \_/\_\\__,_|\___|_|   \__, \____/ \__, |___/\__\___|_| |_| |_\___/_| |_|_| \___/|_|  |_| |_| |_|\__,_|\__|_|\___/|_| |_|
                                          __/ |       __/ |                                                                                
                                         |___/       |___/                                                                                 
    ultima modificação: 02/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação
    */
    /// <summary>
    ///     Referência para NTAPI ZwQuerySystemInformation
    /// </summary>
    NTSTATUS NTAPI ZwQuerySystemInformation(ULONG SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
    //################################################################################################################################


    //################################################################################################################################
    /*    
     _   __    _ _____                ______                                  
    | | / /   | /  ___|               | ___ \                                 
    | |/ /  __| \ `--.  ___ __ _ _ __ | |_/ / __ ___   ___ ___  ___ ___  ___  
    |    \ / _` |`--. \/ __/ _` | '_ \|  __/ '__/ _ \ / __/ _ \/ __/ __|/ _ \ 
    | |\  \ (_| /\__/ / (_| (_| | | | | |  | | | (_) | (_|  __/\__ \__ \ (_) |
    \_| \_/\__,_\____/ \___\__,_|_| |_\_|  |_|  \___/ \___\___||___/___/\___/ 
    
    ultima modificação: 02/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação                                                                         
    */
    /// <summary>
    ///     Esse método varre toda a referência de um binário PE da memória em busca de um padrão de assinatura
    /// </summary>
    /// <param name="NomeProcesso">Path e caminho de referência do processo no contexto do sistema operacional</param>
    /// <returns>status_sucess</returns>
    NTSTATUS KdScanProcesso(const char* NomeProcesso);
    //################################################################################################################################

    /// <summary>
    ///     Essa struct é obtida através de uma referência PVOID atraves de um cast vindo via IOCTL
    ///     de um buffer de entrada.
    /// </summary>
    typedef struct _KERNEL_JOGO_LER_MEMORIA {
        ULONG PID;
        PVOID ENDERECO;
        PVOID RESPOSTA;
        ULONG TAMANHO;
    } KERNEL_JOGO_LER_MEMORIA, *PKERNEL_JOGO_LER_MEMORIA;


    //################################################################################################################################
    /*
     _   __    _ _____      _  ______                 ___      _     _     ______ _    _ 
    | | / /   | /  ___|    | | | ___ \               / _ \    | |   | |    | ___ \ |  | |
    | |/ /  __| \ `--.  ___| |_| |_/ / __ _ ___  ___/ /_\ \ __| | __| |_ __| |_/ / |  | |
    |    \ / _` |`--. \/ _ \ __| ___ \/ _` / __|/ _ \  _  |/ _` |/ _` | '__|    /| |/\| |
    | |\  \ (_| /\__/ /  __/ |_| |_/ / (_| \__ \  __/ | | | (_| | (_| | |  | |\ \\  /\  /
    \_| \_/\__,_\____/ \___|\__\____/ \__,_|___/\___\_| |_/\__,_|\__,_|_|  \_| \_|\/  \/ 
    
    ultima modificação: 02/10/2021
	REV 0.01:
	Author: João Vitor Sampaio | Impementação                                                                                    
    */
    /// <summary>
    ///     Define a base de um processo listado para ser armazenado e usado posteriormente
    /// </summary>
    /// <param name="addr">meu daddy</param>
    VOID KdSetBaseAddrRW(PVOID addr);
    //################################################################################################################################


    //################################################################################################################################
    /*
        
     _   __    _ _____      _  ______ _     _  ___      _     _     ______ _    _ 
    | | / /   | /  ___|    | | | ___ (_)   | |/ _ \    | |   | |    | ___ \ |  | |
    | |/ /  __| \ `--.  ___| |_| |_/ /_  __| / /_\ \ __| | __| |_ __| |_/ / |  | |
    |    \ / _` |`--. \/ _ \ __|  __/| |/ _` |  _  |/ _` |/ _` | '__|    /| |/\| |
    | |\  \ (_| /\__/ /  __/ |_| |   | | (_| | | | | (_| | (_| | |  | |\ \\  /\  /
    \_| \_/\__,_\____/ \___|\__\_|   |_|\__,_\_| |_/\__,_|\__,_|_|  \_| \_|\/  \/ 
        
    ultima modificação: 02/10/2021
    REV 0.01:
    Author: João Vitor Sampaio | Impementação
    */
    /// <summary>
    ///     Define o PID de um processo listado para ser armazenado e usado posteriormente
    /// </summary>
    /// <param name="PID">Processo PID</param>
    VOID KdSetPidAddrRW(HANDLE PID);
    //################################################################################################################################


    //################################################################################################################################
    /*
    
     _   __    _ _____      _  ______                            ______ _            _    _     _     _
    | | / /   | |  __ \    | | | ___ \                           | ___ \ |          | |  | |   (_)   | |
    | |/ /  __| | |  \/ ___| |_| |_/ / __ ___   ___ ___  ___ ___ | |_/ / | __ _  ___| | _| |    _ ___| |_
    |    \ / _` | | __ / _ \ __|  __/ '__/ _ \ / __/ _ \/ __/ __|| ___ \ |/ _` |/ __| |/ / |   | / __| __|
    | |\  \ (_| | |_\ \  __/ |_| |  | | | (_) | (_|  __/\__ \__ \| |_/ / | (_| | (__|   <| |___| \__ \ |_
    \_| \_/\__,_|\____/\___|\__\_|  |_|  \___/ \___\___||___/___/\____/|_|\__,_|\___|_|\_\_____/_|___/\__|

    ultima modificação: 02/10/2021
    REV 0.01:
    Author: João Vitor Sampaio | Impementação
    */
    /// <summary>
    ///     Obtem o nome de um processo marcado como protegido
    /// </summary>
    /// <returns></returns>
    const wchar_t* KdGetProcessBlackList();
    //################################################################################################################################


    /*
        Essa struct é usada para retornar um processo analisado, é cast de PVOID e vindo via IOCTL
    */
    typedef struct _KERNEL_PROCESSO_ANALISE {
        const wchar_t* nome_processo;
        int status;
    } KERNEL_PROCESSO_ANALISE, * PKERNEL_PROCESSO_ANALISE;

}