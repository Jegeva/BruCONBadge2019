deps_config := \
	/tmp/esp-idf-3.2.2/components/app_trace/Kconfig \
	/tmp/esp-idf-3.2.2/components/aws_iot/Kconfig \
	/tmp/esp-idf-3.2.2/components/bt/Kconfig \
	/tmp/esp-idf-3.2.2/components/driver/Kconfig \
	/tmp/esp-idf-3.2.2/components/esp32/Kconfig \
	/tmp/esp-idf-3.2.2/components/esp_adc_cal/Kconfig \
	/tmp/esp-idf-3.2.2/components/esp_event/Kconfig \
	/tmp/esp-idf-3.2.2/components/esp_http_client/Kconfig \
	/tmp/esp-idf-3.2.2/components/esp_http_server/Kconfig \
	/tmp/esp-idf-3.2.2/components/ethernet/Kconfig \
	/tmp/esp-idf-3.2.2/components/fatfs/Kconfig \
	/tmp/esp-idf-3.2.2/components/freemodbus/Kconfig \
	/tmp/esp-idf-3.2.2/components/freertos/Kconfig \
	/tmp/esp-idf-3.2.2/components/heap/Kconfig \
	/tmp/esp-idf-3.2.2/components/libsodium/Kconfig \
	/tmp/esp-idf-3.2.2/components/log/Kconfig \
	/tmp/esp-idf-3.2.2/components/lwip/Kconfig \
	/tmp/esp-idf-3.2.2/components/mbedtls/Kconfig \
	/tmp/esp-idf-3.2.2/components/mdns/Kconfig \
	/tmp/esp-idf-3.2.2/components/mqtt/Kconfig \
	/tmp/esp-idf-3.2.2/components/nvs_flash/Kconfig \
	/tmp/esp-idf-3.2.2/components/openssl/Kconfig \
	/tmp/esp-idf-3.2.2/components/pthread/Kconfig \
	/tmp/esp-idf-3.2.2/components/spi_flash/Kconfig \
	/tmp/esp-idf-3.2.2/components/spiffs/Kconfig \
	/tmp/esp-idf-3.2.2/components/tcpip_adapter/Kconfig \
	/tmp/esp-idf-3.2.2/components/vfs/Kconfig \
	/tmp/esp-idf-3.2.2/components/wear_levelling/Kconfig \
	/tmp/esp-idf-3.2.2/components/bootloader/Kconfig.projbuild \
	/tmp/esp-idf-3.2.2/components/esptool_py/Kconfig.projbuild \
	/root/BruCON2019/firmware/main/Kconfig.projbuild \
	/tmp/esp-idf-3.2.2/components/partition_table/Kconfig.projbuild \
	/tmp/esp-idf-3.2.2/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
