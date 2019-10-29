#ifndef __URL_H
#define __URL_H
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "expat.h"
#include <string.h>
#include "lwip/inet.h"
#include "lwip/ip4_addr.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/rsa.h"
#include "mbedtls/pk.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"
#include "mbedtls/x509_csr.h"


#include "esp_tls.h"
#include "wifi.h"

extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[]   asm("_binary_server_root_cert_pem_end");

void get_sched_task(void *);
char * get_sched();
char * postdata(const char * action, int fieldnums, const char ** fieldnames, const char ** content);

extern volatile char isgeneratingRSA;
extern volatile char isgettingClientCert;
void task_genrsa(void *);
void task_getclientcert(void *);
void restore_clicert( 	mbedtls_x509_crt ** cert,mbedtls_pk_context ** pk_ctx_dcert  );
extern volatile char isgettingsched;
extern char * netsched;
extern mbedtls_x509_crt * clicert;
extern mbedtls_pk_context * pk_ctx_clicert;


typedef struct {
  char * action;
  int fieldnums;
  const char ** fieldnames;
  const char ** content;
  char ** response;
  volatile char done;
} post_tasked_args_t;
void postdata_tasked(void * );

#endif
