#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

CFLAGS += -Wall -Wextra -Werror
COMPONENT_EMBED_TXTFILES := server_root_cert.pem menu.json schedule-sched-filtered.json
