#pragma once
#include <ntifs.h>


extern "C" {
    //################################################################################################################################
    /*
    ______                                        _            _             _
    |  ___|                                      (_)          (_)           | |
    | |_ _   _ _ __   ___ __ _  ___    _ __  _ __ _ _ __   ___ _ _ __   __ _| |
    |  _| | | | '_ \ / __/ _` |/ _ \  | '_ \| '__| | '_ \ / __| | '_ \ / _` | |
    | | | |_| | | | | (_| (_| | (_) | | |_) | |  | | | | | (__| | |_) | (_| | |
    \_|  \__,_|_| |_|\___\__,_|\___/  | .__/|_|  |_|_| |_|\___|_| .__/ \__,_|_|
                                      | |                       | |
                                      |_|                       |_|

        ultima modificação: 01/10/2021
        primeira modificação: 13/03/2021
        REV 0.08:
        Author: João Vitor Sampaio | Impementação
    */

    /// <summary>
    ///		Método do DriverUnload
    /// </summary>
    /// <param name="pDriverObj">Objeto de registro do Driver</param>
    VOID DriverUnload(PDRIVER_OBJECT pDriverObj);


    /// <summary>
    ///		Método do DriverUnload
    /// </summary>
    /// <param name="pDriverObj">Objeto de registro do Driver</param>
    /// <param name="pRegistryString">Path de registro do Driver</param>
    NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString);
    //################################################################################################################################


    PDEVICE_OBJECT pDeviceObject;
    UNICODE_STRING dos, device;

}