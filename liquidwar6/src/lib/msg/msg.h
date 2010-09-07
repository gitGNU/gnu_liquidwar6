/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


  Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
  Contact author        : ufoot@ufoot.org
*/

#ifndef LIQUIDWAR6MSG_H
#define LIQUIDWAR6MSG_H

#include "../sys/sys.h"
#include "../glb/glb.h"
#include "../nod/nod.h"

#define LW6MSG_LW6 "LW6"
#define LW6MSG_LW6_LC "lw6"

#define LW6MSG_ERROR "ERROR"
#define LW6MSG_FORBIDDEN "FORBIDDEN"

#define LW6MSG_YES "YES"
#define LW6MSG_NO "NO"

#define LW6MSG_OOB_INFO "INFO"
#define LW6MSG_OOB_LIST "LIST"
#define LW6MSG_OOB_PING "PING"
#define LW6MSG_OOB_PONG "PONG"

#define LW6MSG_OOB_PROGRAM "PROGRAM"
#define LW6MSG_OOB_VERSION "VERSION"
#define LW6MSG_OOB_CODENAME "CODENAME"
#define LW6MSG_OOB_STAMP "STAMP"
#define LW6MSG_OOB_ID "ID"
#define LW6MSG_OOB_URL "URL"
#define LW6MSG_OOB_TITLE "TITLE"
#define LW6MSG_OOB_DESCRIPTION "DESCRIPTION"
#define LW6MSG_OOB_PASSWORD "PASSWORD"
#define LW6MSG_OOB_UPTIME "UPTIME"
#define LW6MSG_OOB_LEVEL "LEVEL"
#define LW6MSG_OOB_BENCH "BENCH"
#define LW6MSG_OOB_REQUIRED_BENCH "REQUIRED_BENCH"
#define LW6MSG_OOB_NB_COLORS "NB_COLORS"
#define LW6MSG_OOB_MAX_NB_COLORS "MAX_NB_COLORS"
#define LW6MSG_OOB_NB_CURSORS "NB_CURSORS"
#define LW6MSG_OOB_MAX_NB_CURSORS "MAX_NB_CURSORS"
#define LW6MSG_OOB_NB_NODES "NB_NODES"
#define LW6MSG_OOB_MAX_NB_NODES "MAX_NB_NODES"

#define LW6MSG_CMD_HELLO "HELLO"
#define LW6MSG_CMD_TICKET "TICKET"
#define LW6MSG_CMD_FOO "FOO"
#define LW6MSG_CMD_BAR "BAR"
#define LW6MSG_CMD_GOODBYE "GOODBYE"

#define LW6MSG_MAX_WORD_SIZE 799
#define LW6MSG_Z_PREFIX "Z"

#define LW6MSG_TELNET_SEP ' '
#define LW6MSG_URL_SEP '/'

#define LW6MSG_UNDEF "-"

typedef enum lw6msg_envelope_mode_e
{
  LW6MSG_ENVELOPE_MODE_TELNET = 0,
  LW6MSG_ENVELOPE_MODE_URL = 1
} lw6msg_envelope_mode_t;

typedef struct lw6msg_word_s
{
  int len;
  char buf[LW6MSG_MAX_WORD_SIZE + 1];
} lw6msg_word_t;

/* msg-cmd.c */
extern char *lw6msg_cmd_generate_hello (lw6nod_info_t * info);
extern char *lw6msg_cmd_generate_ticket (lw6nod_info_t * info,
					 u_int32_t ticket);
extern char *lw6msg_cmd_generate_foo (lw6nod_info_t * info, u_int32_t key);
extern char *lw6msg_cmd_generate_bar (lw6nod_info_t * info, u_int32_t key);
extern char *lw6msg_cmd_generate_data (lw6nod_info_t * info, u_int64_t author,
				       int serial, int i, int n,
				       char *ker_msg);
extern char *lw6msg_cmd_generate_goodbye (lw6nod_info_t * info);
extern int lw6msg_cmd_analyse_hello (lw6nod_info_t ** info, char *msg);
extern int lw6msg_cmd_analyse_ticket (lw6nod_info_t ** info,
				      u_int32_t * ticket, char *msg);
extern int lw6msg_cmd_analyse_foo (lw6nod_info_t ** info, u_int32_t * key,
				   char *msg);
extern int lw6msg_cmd_analyse_bar (lw6nod_info_t ** info, u_int32_t * key,
				   char *msg);
extern int lw6msg_cmd_analyse_data (lw6nod_info_t ** info, u_int64_t * author,
				    int *serial, int *i, int *n,
				    char **ker_msg, char *msg);
extern int lw6msg_cmd_analyse_goodbye (lw6nod_info_t ** info, char *msg);
extern char *lw6msg_cmd_guess_from_url (char *msg);

/* msg-enveloppe.c */
extern char *lw6msg_envelope_generate (lw6msg_envelope_mode_t mode,
				       char *version, char *password_checksum,
				       u_int32_t physical_ticket_sig,
				       u_int32_t logical_ticket_sig,
				       u_int64_t physical_from_id,
				       u_int64_t physical_to_id,
				       u_int64_t logical_from_id,
				       u_int64_t logical_to_id, char *msg);
extern int lw6msg_envelope_analyse (char *envelope,
				    lw6msg_envelope_mode_t mode,
				    char *local_url, char *password,
				    u_int64_t expected_physical_from_id,
				    u_int64_t expected_physical_to_id,
				    char **msg,
				    u_int32_t * physical_ticket_sig,
				    u_int32_t * logical_ticket_sig,
				    u_int64_t * physical_from_id,
				    u_int64_t * physical_to_id,
				    u_int64_t * logical_from_id,
				    u_int64_t * logical_to_id,
				    char **physical_from_url);

/* msg-oob.c */
extern char *lw6msg_oob_generate_info (lw6nod_info_t * info);
extern char *lw6msg_oob_generate_list (lw6nod_info_t * info);
extern char *lw6msg_oob_generate_pong (lw6nod_info_t * info);
extern char *lw6msg_oob_generate_request (char *command, char *remote_url,
					  char *password, char *local_url);
extern int lw6msg_oob_analyse_request (int *syntax_ok, char **command,
				       int *password_ok, char **remote_url,
				       char *request, char *local_url,
				       char *password);
extern char *lw6msg_oob_analyse_pong (char *text);

/* nod-test.c */
extern int lw6msg_test (int mode);

/* msg-ticket.c */
extern u_int32_t lw6msg_ticket_calc_sig (u_int32_t ticket, u_int64_t from_id,
					 u_int64_t to_id, char *msg);
extern int lw6msg_ticket_check_sig (u_int32_t ticket, u_int64_t from_id,
				    u_int64_t to_id, char *msg,
				    u_int32_t ticket_sig);

/* msg-utils.c */
extern int lw6msg_utils_parse_key_value_to_ptr (char **key, char **value,
						char *line);
extern int lw6msg_utils_parse_key_value_to_assoc (lw6sys_assoc_t ** assoc,
						  char *line);
extern char *lw6msg_utils_get_assoc_str_with_default (lw6sys_assoc_t * assoc,
						      char *key,
						      char *default_value);
extern int lw6msg_utils_get_assoc_int_with_default (lw6sys_assoc_t * assoc,
						    char *key,
						    int default_value);

/* msg-word.c */
extern int lw6msg_word_first (lw6msg_word_t * word, char **next, char *msg);
extern int lw6msg_word_first_x (lw6msg_word_t * word, char **next, char *msg);
extern int lw6msg_word_first_base64 (lw6msg_word_t * word, char **next,
				     char *msg);
extern int lw6msg_word_first_int (int *parsed_value, char **next, char *msg);
extern int lw6msg_word_first_int_gt0 (int *parsed_value, char **next,
				      char *msg);
extern int lw6msg_word_first_id_16 (u_int16_t * parsed_value, char **next,
				    char *msg);
extern int lw6msg_word_first_id_32 (u_int32_t * parsed_value, char **next,
				    char *msg);
extern int lw6msg_word_first_id_64 (u_int64_t * parsed_value, char **next,
				    char *msg);

/* msg-z.c */
extern char *lw6msg_z_encode (char *msg, int limit);
extern char *lw6msg_z_decode (char *msg);

#endif
