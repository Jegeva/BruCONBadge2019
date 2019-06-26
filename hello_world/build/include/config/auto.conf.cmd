deps_config := \
	/tmp/esp-idf-v3.0.8/components/app_trace/Kconfig \
	/tmp/esp-idf-v3.0.8/components/aws_iot/Kconfig \
	/tmp/esp-idf-v3.0.8/components/bt/Kconfig \
	/tmp/esp-idf-v3.0.8/components/esp32/Kconfig \
	/tmp/esp-idf-v3.0.8/components/esp_adc_cal/Kconfig \
	/tmp/esp-idf-v3.0.8/components/ethernet/Kconfig \
	/tmp/esp-idf-v3.0.8/components/fatfs/Kconfig \
	/tmp/esp-idf-v3.0.8/components/freertos/Kconfig \
	/tmp/esp-idf-v3.0.8/components/heap/Kconfig \
	/tmp/esp-idf-v3.0.8/components/libsodium/Kconfig \
	/tmp/esp-idf-v3.0.8/components/log/Kconfig \
	/tmp/esp-idf-v3.0.8/components/lwip/Kconfig \
	/tmp/esp-idf-v3.0.8/components/mbedtls/Kconfig \
	/tmp/esp-idf-v3.0.8/components/openssl/Kconfig \
	/tmp/esp-idf-v3.0.8/components/pthread/Kconfig \
	/tmp/esp-idf-v3.0.8/components/spi_flash/Kconfig \
	/tmp/esp-idf-v3.0.8/components/spiffs/Kconfig \
	/tmp/esp-idf-v3.0.8/components/tcpip_adapter/Kconfig \
	/tmp/esp-idf-v3.0.8/components/wear_levelling/Kconfig \
	/tmp/esp-idf-v3.0.8/components/bootloader/Kconfig.projbuild \
	/tmp/esp-idf-v3.0.8/components/esptool_py/Kconfig.projbuild \
	/tmp/esp-idf-v3.0.8/components/partition_table/Kconfig.projbuild \
	/tmp/esp-idf-v3.0.8/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
