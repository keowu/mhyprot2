#include <iostream>
#include "Mhyprot2SCManager.h"

/// <summary>
///     Método responsável por criar o servico do Driver do anticheat do genshin impact
/// </summary>
void Mhyprot2SCManager::instalarDriver() {
    // Abrindo SC manager
    SC_HANDLE hSC = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    //Verificando handle
    if (hSC == NULL) {
        throw std::exception("Ocorreu um problema ao criar um handle para o SCManager");//If deu pau
    }

    //Preparando path
    TCHAR NPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, NPath);
    std::wstring temp(NPath);
    std::string caminho(temp.begin(), temp.end());
    caminho.append("//mhyprot2.sys");
    //Criando o servico do anti-cheat
    SC_HANDLE servico = CreateServiceA(hSC, "mhyprot2", "mhyprot2", SC_MANAGER_ALL_ACCESS,
                                       SERVICE_KERNEL_DRIVER, 1, SERVICE_ERROR_NORMAL, caminho.c_str(), NULL, NULL, NULL, NULL, NULL);
    //Comparando o HANDLE
    if (servico == NULL) {
        //Abrindo o serviço
        servico = OpenServiceA(hSC, "mhyprot2", SERVICE_ALL_ACCESS);
        //Comparando o Handle
        if (servico == NULL) {
            //Fechando o handle do SC manager
            CloseHandle(hSC);
            throw std::exception("Ocorreu um problema enquanto tentava-se criar um serviço !"); //If deu pau
        }
    }

    // Fechando o handle do SC manager
    CloseServiceHandle(hSC);

    //Chamando o método para inicializar o Driver
    this->iniciarMhyprot2();

    return;
}

/// <summary>
///     Método responsável por inicializar o anticheat/driver
/// </summary>
void Mhyprot2SCManager::iniciarMhyprot2() {
    // Abrindo o SC manager
    SC_HANDLE hSC = OpenSCManagerA(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

    if (hSC == NULL) {// se deu pau
        throw std::exception("Ocorreu um problema ao criar um handle para o SCManager");
    }

    // Abrindo um serviço existente pelo nome, nesse caso mhyprot2 que é o nome do anticheat
    SC_HANDLE servico = OpenServiceA(hSC, "mhyprot2", SERVICE_ALL_ACCESS);

    if (servico == NULL) {// se deu pau
        CloseServiceHandle(hSC);
        throw std::exception("Ocorreu um problema ao tentar abrir o serviço");
    }

    // Iniciando o serviço do anticheat/driver e armazenando o retorno
    BOOL isIniciado = StartServiceA(servico, 0, NULL);

    // Se o anticheat/driver não for inicializado
    if (!isIniciado) {
        CloseServiceHandle(hSC); // Fecha o SC Manager
        CloseServiceHandle(servico); // Fecha o servico
        throw std::exception("Iniciar o serviço falhou !"); // Deu pau
    }

    //Fechando o handle o SC Manager
    CloseServiceHandle(hSC);

    return;
}

/// <summary>
///     Método responsável por parar o anticheat/driver
/// </summary>
void Mhyprot2SCManager::pararMhyprot2() {

    // Abrindo o SC Manager
    SC_HANDLE hSC = OpenSCManagerA(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

    // Se o handle para o SC Manager for criado
    if (hSC == NULL) {
        throw std::exception("Ocorreu um problema ao criar um handle para o SCManager"); // Deu pau
    }

    // Abre o serviço do mhyprot2
    SC_HANDLE servico = OpenServiceA(hSC, "mhyprot2", SERVICE_ALL_ACCESS);

    // Foi aberto com sucesso ?
    if (servico == NULL) {
        CloseServiceHandle(hSC); // Fecha caso deu pau
        throw std::exception("Ocorreu um problema ao tentar abrir o serviço"); // deu pau
    }

    // Criando uma flag para o estado do serviço
    SERVICE_STATUS servicoStat;
    
    // controlando o serviço e passando o estado de stop e salvando o estado do serviço, a gente não precisa dele, mas tem que ter.
    BOOL isParado = ControlService(servico, SERVICE_CONTROL_STOP, &servicoStat);

    // Se não foi possível parar
    if (!isParado) {
        CloseServiceHandle(hSC); // Fecha o handle do SC Manager
        CloseServiceHandle(servico); // Fecha o handle do serviço
        throw std::exception("Parar o serviço falhou !"); // deu pau
    }

    CloseServiceHandle(hSC); // Fecha o handle do SC Manager
    CloseServiceHandle(servico); // Fecha o handle para o serviço

    return;

}


/// <summary>
///     Método responsável por desinstalar o driver do contexto do sistema operacional
///     A MIOJO NÃO SABE FAZER ISSO KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
///     Veja em:
///         https://www.pcgamer.com/genshin-impacts-kernel-level-anti-cheat-no-longer-runs-after-you-close-the-game/
///     para miojo quando ler isso:
///             NI HAO, MÂ Mâ MÂ MÂ Cyka Blyateru Mâ ?
///     eu corrigi esse problema para eles :D
/// </summary>
void Mhyprot2SCManager::desinstalarDriver() {
    //Para o driver via SC Manager
    this->pararMhyprot2();

    // Abre o scmanager e recupera o handle
    SC_HANDLE hSC = OpenSCManagerA(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

    // Se não tivermos sucesso
    if (hSC == NULL) {
        throw std::exception("Ocorreu um problema ao criar um handle para o SCManager"); // Deu pau
    }

    // Abre o serviço do SC Manager
    SC_HANDLE servico = OpenServiceA(hSC, "mhyprot2", SERVICE_ALL_ACCESS);

    // Se não tivermos sucesso
    if (servico == NULL) {
        CloseServiceHandle(hSC); // Fechamos o handle
        throw std::exception("Ocorreu um problema ao tentar abrir o serviço"); // Deu pau
    }

    // Excluimos o serviço
    DeleteService(servico);
    // Fechamos o handle
    CloseServiceHandle(hSC);

    //CYKA BLYAT CYKAAAAAAAAAAAAAAAAAAA
    // RETORNA
    return;

}



int main(char argc, char** argv)
{
    std::cout << argv[1]; // Escrevendo o Comando recebido
    Mhyprot2SCManager* mhy = new Mhyprot2SCManager(); // Instânciando nossa classe
    if (atoi(argv[1]) == 1) { // ATOI para tratar como int
        mhy->instalarDriver(); // Instalando o Driver
    }
    else if (atoi(argv[1]) == 2) { // ATOI para tratar como int
        mhy->desinstalarDriver(); // Desinstalando o Driver
    }
    else if (atoi(argv[1]) == 3) { // ATOI para tratar como int
        mhy->iniciarMhyprot2(); // Iniciando o Driver
    }
    else if (atoi(argv[1]) == 4) { // ATOI para tratar como int
        mhy->pararMhyprot2(); // Parando o Driver
    }
    else {
        std::cout << "Wrong HOUSE, FOOL!!" << std::endl; // WRONG HOUSE, FOOL!!!! TIO BIG FUMAÇA(GTA SAN DREAS)....
    }
}