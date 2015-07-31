import qbs
import qbs.FileInfo
Product {
    Depends { name:"cpp"}
    type: ["application", "hex", "bin", "size"]
    name: "template_project"
    property string sdk: "../../nRF51_SDK_8.1.0_b6ed55f/"
    property string vendor: "Nordic"
    property string softDevice: "s110"
    property string device: "nrf51422"
    property string deviceVariant: "xxac" // xxaa=256kB Flash,16kB RAM | xxab=128kB Flash,16kB RAM | xxac=256kB Flash,32kB RAM

    cpp.defines: [
        'DEBUG',
        'NRF51',
        'BOARD_CUSTOM',
        'SOFTDEVICE_PRESENT',
        'S110',
        'BLE_STACK_SUPPORT_REQD',
        'SWI_DISABLE0',
        'xBLE_DFU_APP_SUPPORT',
        'BUILD_NUM=0001',
        'BOARD_REV=1'
    ]
    cpp.warningLevel: "default" //"all" // or "none", "default"
    cpp.treatWarningsAsErrors: true
    cpp.executableSuffix: ".out"
    cpp.positionIndependentCode: false
    Properties {
        condition: qbs.buildVariant === "debug"
        cpp.commonCompilerFlags: [
            "-mcpu=cortex-m0", "-mthumb", "-mabi=aapcs", "-mfloat-abi=soft",
            "-std=gnu11", "-flto", "-fno-builtin",
            "-fdata-sections","-ffunction-sections","-fno-strict-aliasing",
            "-Wall",
        ]
        cpp.debugInformation: true
        cpp.optimization: "none" // "none" or "fast"
    }
    Properties {
        condition: qbs.buildVariant === "release"
        cpp.commonCompilerFlags: [
            "-mcpu=cortex-m0", "-mthumb", "-mabi=aapcs", "-mfloat-abi=soft",
            "-std=gnu99", "-flto", "-fno-builtin",
            "-fdata-sections","-ffunction-sections","-fno-strict-aliasing",
            "-Wall",
        ]
        cpp.debugInformation: false
        cpp.optimization: "small"
    }
    cpp.linkerFlags:[
        "-Xlinker", "-Map="+buildDirectory+"/"+name+".map",
        "-mthumb", "-mabi=aapcs",
        "-L"+path+"/"+sdk+"components/toolchain/gcc/",
        "-T"+path+"/custom_gcc_nrf51.ld",
        "-mcpu=cortex-m0",
        "-Wl,--gc-sections","--specs=nano.specs","-lc","-lnosys", "-lm",
    ]
    cpp.includePaths: [     '../',
                            '../config',
                            sdk + "components/ble/common",
                            sdk + "components/ble/ble_advertising",
                            sdk + "components/ble/ble_debug_assert_handler",
                            sdk + "components/ble/ble_error_log",
                            sdk + "components/ble/ble_radio_notification",
                            sdk + "components/ble/ble_services",
                            sdk + "components/ble/ble_services/ble_bas",
                            sdk + "components/ble/ble_services/ble_dfu",
                            sdk + "components/ble/ble_services/ble_dis",
                            sdk + "components/ble/device_manager",
                            sdk + "components/device",
                            sdk + "components/drivers_nrf/ble_flash",
                            sdk + "components/drivers_nrf/common",
                            sdk + "components/drivers_nrf/gpiote",
                            sdk + "components/drivers_nrf/hal",
                            sdk + "components/drivers_nrf/ppi",
                            sdk + "components/drivers_nrf/pstorage",
                            sdk + "components/drivers_nrf/spi_master",
                            sdk + "components/drivers_nrf/timer",
                            sdk + "components/libraries/bootloader_dfu",
                            sdk + "components/libraries/fifo",
                            sdk + "components/libraries/gpiote",
                            sdk + "components/libraries/pwm",
                            sdk + "components/libraries/scheduler",
                            sdk + "components/libraries/timer",
                            sdk + "components/libraries/trace",
                            sdk + "components/libraries/util",
                            sdk + "components/softdevice/common/softdevice_handler",
                            sdk + "components/softdevice/s110/headers",
                            sdk + "components/toolchain",
                            sdk + "components/toolchain/gcc",

    ]
    files: [
        //sdk + "components/libraries/bootloader_dfu/bootloader_util.c",
        //sdk + "components/libraries/bootloader_dfu/dfu_app_handler.c",
        sdk + "components/libraries/bootloader_dfu/dfu_app_handler.h",
        sdk + "components/ble/ble_advertising/ble_advertising.c",
        sdk + "components/ble/ble_advertising/ble_advertising.h",
        sdk + "components/ble/ble_debug_assert_handler/ble_debug_assert_handler.c",
        sdk + "components/ble/ble_debug_assert_handler/ble_debug_assert_handler.h",
        sdk + "components/ble/ble_error_log/ble_error_log.c",
        sdk + "components/ble/ble_error_log/ble_error_log.h",
        sdk + "components/ble/ble_radio_notification/ble_radio_notification.c",
        sdk + "components/ble/ble_radio_notification/ble_radio_notification.h",
        sdk + "components/ble/ble_services/ble_bas/ble_bas.c",
        sdk + "components/ble/ble_services/ble_bas/ble_bas.h",
        sdk + "components/ble/ble_services/ble_dfu/ble_dfu.c",
        sdk + "components/ble/ble_services/ble_dfu/ble_dfu.h",
        sdk + "components/ble/ble_services/ble_dis/ble_dis.c",
        sdk + "components/ble/ble_services/ble_dis/ble_dis.h",
        sdk + "components/ble/common/ble_advdata.c",
        sdk + "components/ble/common/ble_advdata.h",
        sdk + "components/ble/common/ble_conn_params.c",
        sdk + "components/ble/common/ble_conn_params.h",
        sdk + "components/ble/common/ble_srv_common.c",
        sdk + "components/ble/common/ble_srv_common.h",
        sdk + "components/ble/device_manager/device_manager.h",
        sdk + "components/ble/device_manager/device_manager_peripheral.c",
        sdk + "components/device/compiler_abstraction.h",
        sdk + "components/device/nrf.h",
        sdk + "components/device/nrf51.h",
        sdk + "components/device/nrf51_bitfields.h",
        sdk + "components/drivers_nrf/ble_flash/ble_flash.c",
        sdk + "components/drivers_nrf/ble_flash/ble_flash.h",
        sdk + "components/drivers_nrf/common/nrf_drv_common.c",
        sdk + "components/drivers_nrf/common/nrf_drv_common.h",
        //sdk + "components/drivers_nrf/gpiote/nrf_drv_gpiote.c",
        //sdk + "components/drivers_nrf/gpiote/nrf_drv_gpiote.h",
        sdk + "components/drivers_nrf/hal/nrf_adc.c",
        sdk + "components/drivers_nrf/hal/nrf_adc.h",
        sdk + "components/drivers_nrf/hal/nrf_clock.h",
        sdk + "components/drivers_nrf/hal/nrf_delay.c",
        sdk + "components/drivers_nrf/hal/nrf_delay.h",
        sdk + "components/drivers_nrf/hal/nrf_gpio.h",
        sdk + "components/drivers_nrf/hal/nrf_gpiote.h",
        sdk + "components/drivers_nrf/hal/nrf_ppi.h",
        sdk + "components/drivers_nrf/hal/nrf_rtc.h",
        sdk + "components/drivers_nrf/hal/nrf_timer.h",
        sdk + "components/drivers_nrf/hal/nrf_twi.h",
        sdk + "components/drivers_nrf/ppi/nrf_drv_ppi.c",
        sdk + "components/drivers_nrf/ppi/nrf_drv_ppi.h",
        sdk + "components/drivers_nrf/pstorage/pstorage.c",
        sdk + "components/drivers_nrf/pstorage/pstorage.h",
        sdk + "components/drivers_nrf/timer/nrf_drv_timer.c",
        sdk + "components/drivers_nrf/timer/nrf_drv_timer.h",
        sdk + "components/libraries/fifo/app_fifo.c",
        sdk + "components/libraries/fifo/app_fifo.h",
        sdk + "components/libraries/gpiote/app_gpiote.c",
        sdk + "components/libraries/gpiote/app_gpiote.h",
        sdk + "components/libraries/scheduler/app_scheduler.c",
        sdk + "components/libraries/scheduler/app_scheduler.h",
        sdk + "components/libraries/timer/app_timer.c",
        sdk + "components/libraries/timer/app_timer.h",
        sdk + "components/libraries/timer/app_timer_appsh.c",
        sdk + "components/libraries/timer/app_timer_appsh.h",
        sdk + "components/libraries/trace/app_trace.c",
        sdk + "components/libraries/trace/app_trace.h",
        sdk + "components/libraries/util/app_error.c",
        sdk + "components/libraries/util/app_error.h",
        sdk + "components/libraries/util/app_util.h",
        sdk + "components/libraries/util/common.h",
        sdk + "components/libraries/util/nordic_common.h",
        sdk + "components/libraries/util/nrf_assert.c",
        sdk + "components/libraries/util/nrf_assert.h",
        sdk + "components/softdevice/common/softdevice_handler/softdevice_handler.c",
        sdk + "components/softdevice/common/softdevice_handler/softdevice_handler.h",
        sdk + "components/softdevice/s110/headers/ble.h",
        sdk + "components/softdevice/s110/headers/ble_err.h",
        sdk + "components/softdevice/s110/headers/ble_gap.h",
        sdk + "components/softdevice/s110/headers/ble_gatt.h",
        sdk + "components/softdevice/s110/headers/ble_gattc.h",
        sdk + "components/softdevice/s110/headers/ble_gatts.h",
        sdk + "components/softdevice/s110/headers/ble_hci.h",
        sdk + "components/softdevice/s110/headers/ble_l2cap.h",
        sdk + "components/softdevice/s110/headers/ble_ranges.h",
        sdk + "components/softdevice/s110/headers/ble_types.h",
        sdk + "components/softdevice/s110/headers/nrf_error.h",
        sdk + "components/softdevice/s110/headers/nrf_error_sdm.h",
        sdk + "components/softdevice/s110/headers/nrf_error_soc.h",
        sdk + "components/softdevice/s110/headers/nrf_mbr.h",
        sdk + "components/softdevice/s110/headers/nrf_sd_def.h",
        sdk + "components/softdevice/s110/headers/nrf_sdm.h",
        sdk + "components/softdevice/s110/headers/nrf_soc.h",
        sdk + "components/softdevice/s110/headers/nrf_svc.h",
        sdk + "components/softdevice/s110/headers/softdevice_assert.h",
        sdk + "components/toolchain/system_nrf51.c",
        sdk + "components/toolchain/system_nrf51.h",
        "../*.c",
        "../*.h",
        "../config/*.h",
        "./gcc_startup_nrf51.s",
    ]
    Rule {
        id: hex
        inputs: "application"
        Artifact {
            fileTags: ["hex"]
            filePath: FileInfo.baseName(input.filePath) + ".hex"
        }
        prepare: {
            var args = ["-O", "ihex", input.filePath, output.filePath];
            var cmd = new Command("arm-none-eabi-objcopy", args);
            cmd.description = "converting to hex: "+FileInfo.fileName(input.filePath);
            cmd.highlight = "linker";
            return cmd;

        }
    }

    Rule {
        id: bin
        inputs: "application"
        Artifact {
            fileTags: ["bin"]
            filePath: FileInfo.baseName(input.filePath) + ".bin"
        }
        prepare: {
            var args = ["-O", "binary", input.filePath, output.filePath];
            var cmd = new Command("arm-none-eabi-objcopy", args);
            cmd.description = "converting to bin: "+FileInfo.fileName(input.filePath);
            cmd.highlight = "linker";
            return cmd;

        }
    }

    Rule {
            id: size
            inputs: ["application"]
            Artifact {
                fileTags: ["size"]
                filePath: "-"
            }
            prepare: {
                var args = [input.filePath];
                var cmd = new Command("arm-none-eabi-size", args);
                cmd.description = "File size: " + FileInfo.fileName(input.filePath);
                cmd.highlight = "linker";
                return cmd;
            }
        }
}

