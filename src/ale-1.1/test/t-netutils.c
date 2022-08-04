#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/netutils.h"

#define IPV4TOASN "ipv4toasn.txt"
#define ASNTOOWNER "asntoowner.txt"
#define RIR "rir-ripencc.txt"

int
main(int argc, char *argv[argc])
{
  struct netutils nu;
  uint32_t asn;
  
  netutils_init(&nu);

  ERROR_FATAL(-1 == netutils_load_bgp4(&nu, IPV4TOASN),
		    "FATAL: netutils_load_ipv4_to_asn()\n");
  
  ERROR_UNDEF_FATAL_FMT(23969 != (asn = netutils_bgp4_ip_to_asn(&nu, "1.0.128.0")),
		  "FATAL: netutils_bgp4_ip_to_asn(1.0.128.0) asn = %"PRIu32" != 23969\n", asn);
  ERROR_UNDEF_FATAL_FMT(23969 != (asn = netutils_bgp4_ip_to_asn(&nu, "1.0.128.255")),
		  "FATAL: netutils_bgp4_ip_to_asn(1.0.128.255) asn = %"PRIu32" != 23969\n", asn);
  ERROR_UNDEF_FATAL_FMT(9737 != (asn = netutils_bgp4_ip_to_asn(&nu, "1.0.138.255")),
		  "FATAL: netutils_bgp4_ip_to_asn(1.0.138.255) asn = %"PRIu32" != 9737\n", asn);
  ERROR_UNDEF_FATAL_FMT(4809 != (asn = netutils_bgp4_ip_to_asn(&nu, "115.224.2.110")),
		  "FATAL: netutils_bgp4_ip_to_asn(115.224.2.110) asn = %"PRIu32" != 4809\n", asn);

  ERROR_FATAL(-1 == netutils_load_asn_to_owner(&nu, ASNTOOWNER), "FATAL: netutils_load_asn_to_owner()\n");

  ERROR_UNDEF_FATAL(NULL != netutils_asn_to_owner(&nu, 0),
		    "FATAL: netutils_asn_to_owner(&nu, 0) != NULL");
  ERROR_UNDEF_FATAL(NULL != netutils_asn_to_owner(&nu, 99),
		    "FATAL: netutils_asn_to_owner(&nu, 99) != NULL");
  char *str;
  char *exp = "Level 3 Communications, Inc.";
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(exp, (str = netutils_asn_to_owner(&nu, 1))),
			"FATAL: netutils_asn_to_owner(&nu, 1) '%s' != '%s'\n", str, exp);
  exp = "DigitalFyre Internet Solutions, LLC.";
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(exp, (str = netutils_asn_to_owner(&nu, 64245))),
			"FATAL: netutils_asn_to_owner(&nu, 64245) '%s' != '%s'\n", str, exp);


  ERROR_FATAL(-1 == netutils_load_rir(&nu, RIR), "FATAL: netutils_load_rir()\n");

  struct rir *rir;
  ERROR_UNDEF_FATAL(! netutils_rir_getinfo(&nu, "2.0.0.0", &rir),
		    "FATAL: netutils_rir_getinfo(2.0.0.0)\n");
  ERROR_UNDEF_FATAL_FMT(1048576 != rir->count,
			"FATAL: netutils_rir_getinfo(2.0.0.0) count %"PRIu32" != 1048576\n", 
			rir->count);
  ERROR_UNDEF_FATAL_FMT(0 != strcmp("FR",rir->country),
			"FATAL: netutils_rir_getinfo(2.0.0.0) country %s != FR\n", rir->country);
  // TODO: Mmmh unable to get epoch consistently
  // posix suXor http://www.catb.org/esr/time-programming/
  //ERROR_UNDEF_FATAL_FMT(difftime(1278885600ll, rir->date) > 7200,
  //			"FATAL: netutils_rir_getinfo(2.0.0.0) date %lld != 1278885600\n", (long long) rir->date);
  netutils_destroy(&nu);

  return EXIT_SUCCESS;
}
