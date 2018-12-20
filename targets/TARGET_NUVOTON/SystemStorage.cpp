/* Storage interface defaults
 * Copyright (c) 2018-2020 Nuvoton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"
#include "FileSystem.h"
#include "FATFileSystem.h"
#include "LittleFileSystem.h"

#if COMPONENT_SPIF
#include "SPIFBlockDevice.h"
#endif

#if COMPONENT_SD
#include "SDBlockDevice.h"
#include "MBRBlockDevice.h"
#endif

#if COMPONENT_NUSD
#include "NuSDBlockDevice.h"
#include "MBRBlockDevice.h"
#endif

BlockDevice *BlockDevice::get_default_instance()
{
#if COMPONENT_SPIF

    static SPIFBlockDevice default_bd(
        MBED_CONF_SPIF_DRIVER_SPI_MOSI,
        MBED_CONF_SPIF_DRIVER_SPI_MISO,
        MBED_CONF_SPIF_DRIVER_SPI_CLK,
        MBED_CONF_SPIF_DRIVER_SPI_CS,
        MBED_CONF_SPIF_DRIVER_SPI_FREQ
    );

    return &default_bd;

#elif COMPONENT_SD

    static SDBlockDevice default_bd(
        MBED_CONF_SD_SPI_MOSI,
        MBED_CONF_SD_SPI_MISO,
        MBED_CONF_SD_SPI_CLK,
        MBED_CONF_SD_SPI_CS
    );

    return &default_bd;

#elif COMPONENT_NUSD

    static NuSDBlockDevice default_bd;

    return &default_bd;

#else

    #error("No matched component for default BlockDevice")
    return NULL;

#endif
}

FileSystem *FileSystem::get_default_instance()
{
#if COMPONENT_SPIF

#ifdef MBED_CONF_APP_DEFAULT_MOUNT_POINT
    static LittleFileSystem flash(MBED_CONF_APP_DEFAULT_MOUNT_POINT, BlockDevice::get_default_instance());
#else
    static LittleFileSystem flash("flash", BlockDevice::get_default_instance());
#endif
    flash.set_as_default();

    return &flash;

#elif COMPONENT_SD || COMPONENT_NUSD

#ifdef MBED_CONF_APP_DEFAULT_MOUNT_POINT
    static FATFileSystem sdcard(MBED_CONF_APP_DEFAULT_MOUNT_POINT, BlockDevice::get_default_instance());
#else
    static FATFileSystem sdcard("sd", BlockDevice::get_default_instance());
#endif
    sdcard.set_as_default();

    return &sdcard;

#else

    #error("No matched component for default FileSystem")
    return NULL;

#endif
}