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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include "p2p.h"
#include "p2p-internal.h"

static u_int32_t seq_id = 0;

/**
 * lw6p2p_db_open
 *
 * @argc: number of args, as passed to @main
 * @argv: args array, as passed to @main
 * @name: the database name
 *
 * Creates a new database object. Normally there's only one object
 * like this at a given time, it can be shared among various nodes.
 * The database name is appended to user directory path, this allows
 * different databases to be created, in theory.
 *
 * Return value: a pointer on the newly created object.
 */
lw6p2p_db_t *
lw6p2p_db_open (int argc, char *argv[], char *name)
{
  return (lw6p2p_db_t *) _lw6p2p_db_open (argc, argv, name);
}

_lw6p2p_db_t *
_lw6p2p_db_open (int argc, char *argv[], char *name)
{
  _lw6p2p_db_t *db = NULL;
  int ret = 0;
  char *user_dir = NULL;
  char *data_dir = NULL;

  db = (_lw6p2p_db_t *) LW6SYS_CALLOC (sizeof (_lw6p2p_db_t));
  if (db)
    {
      db->id = 0;
      while (!db->id)
	{
	  db->id = ++seq_id;
	}

      db->mutex = lw6sys_mutex_create ();
      if (db->mutex)
	{
	  data_dir = lw6sys_get_data_dir (argc, argv);
	  if (data_dir)
	    {
	      if (_lw6p2p_data_load (&(db->data), data_dir))
		{
		  user_dir = lw6sys_get_user_dir (argc, argv);
		  if (user_dir)
		    {
		      db->filename = lw6sys_path_concat (user_dir, name);
		      if (db->filename)
			{
			  lw6sys_log (LW6SYS_LOG_INFO, _("opening db \"%s\""),
				      db->filename);
			  if (sqlite3_open (db->filename, &(db->handler)) ==
			      SQLITE_OK)
			    {
			      lw6sys_log (LW6SYS_LOG_INFO,
					  _("opened db \"%s\""),
					  db->filename);
			      if (_lw6p2p_db_create_database (db))
				{
				  if (_lw6p2p_db_clean_database (db))
				    {
				      /*
				       * We clean just after we create, in
				       * fact create won't clean, only
				       * use CREATE IF NOT EXISTS so
				       * in case program left database
				       * in inconsistent state, we
				       * need to force its cleanup.
				       */
				      ret = 1;
				    }
				  else
				    {
				      lw6sys_log (LW6SYS_LOG_WARNING,
						  _
						  ("can't clean database \"%s\""),
						  db->filename);
				    }
				}
			      else
				{
				  lw6sys_log (LW6SYS_LOG_WARNING,
					      _
					      ("can't create database \"%s\""),
					      db->filename);
				}
			    }
			  else
			    {
			      if (db->handler)
				{
				  lw6sys_log (LW6SYS_LOG_WARNING,
					      _
					      ("can't open db \"%s\" errcode=%d errmsg=\"%s\""),
					      db->filename,
					      sqlite3_errcode (db->handler),
					      sqlite3_errmsg (db->handler));
				}
			      else
				{
				  lw6sys_log (LW6SYS_LOG_WARNING,
					      _("can't open db \"%s\""));
				}
			    }
			}
		      LW6SYS_FREE (user_dir);
		    }
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING, _("can't load p2p data"));
		}
	      LW6SYS_FREE (data_dir);
	    }
	}
    }

  if ((!ret) && db)
    {
      _lw6p2p_db_close (db);
      db = NULL;
    }

  return db;
}

/**
 * lw6p2p_db_close
 *
 * @db: the db to close
 *
 * Closes a db object, memory ressources will be freed.
 *
 * Return value: none.
 */
void
lw6p2p_db_close (lw6p2p_db_t * db)
{
  _lw6p2p_db_close ((_lw6p2p_db_t *) db);
}

void
_lw6p2p_db_close (_lw6p2p_db_t * db)
{
  if (db)
    {
      if (db->mutex)
	{
	  lw6sys_mutex_destroy (db->mutex);
	}
      if (db->filename)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("closing db \"%s\""), db->filename);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("closing unknown db"));
	}
      if (db->handler)
	{
	  if (sqlite3_close (db->handler) == SQLITE_OK)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _("sqlite3_close successfull"));
	      db->handler = NULL;
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _("sqlite2_close failed errcode=%d errmsg=\"%s\""),
			  sqlite3_errcode (db->handler),
			  sqlite3_errmsg (db->handler));
	    }
	}
      _lw6p2p_data_unload (&(db->data));
      if (db->filename)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("closed db \"%s\""), db->filename);
	  LW6SYS_FREE (db->filename);
	  db->filename = NULL;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("closed unknown db"));
	}
      LW6SYS_FREE (db);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("trying to close NULL db"));
    }
}

/**
 * lw6p2p_db_repr
 *
 * @db: the db to work on
 *
 * Gives a readable representation of the db
 *
 * Return value: a dynamically allocated string
 */
char *
lw6p2p_db_repr (lw6p2p_db_t * db)
{
  return _lw6p2p_db_repr ((_lw6p2p_db_t *) db);
}

char *
_lw6p2p_db_repr (_lw6p2p_db_t * db)
{
  char *repr = NULL;

  if (db && db->filename)
    {
      repr = lw6sys_new_sprintf (_("%u %s"), db->id, db->filename);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("can't repr NULL or unitialized db"));
    }

  return repr;
}

char *
_lw6p2p_db_get_query (_lw6p2p_db_t * db, char *key)
{
  char *query = NULL;

  query = lw6sys_hash_get (db->data.sql.queries, key);
  if (!query)
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("can't get SQL query \"%s\""), key);
      query = "";		// to avoid segfault in probable sprintf
    }

  return query;
}

int
_lw6p2p_db_lock (_lw6p2p_db_t * db)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("lock db"));
  ret = lw6sys_mutex_lock (db->mutex);
  if (ret)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("lock db OK"));
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("unable to lock db"));
    }

  return ret;
}

int
_lw6p2p_db_unlock (_lw6p2p_db_t * db)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("unlock db"));
  ret = lw6sys_mutex_unlock (db->mutex);
  if (ret)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _("unlock db OK"));
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("unable to unlock db"));
    }

  return ret;
}

int
_lw6p2p_db_trylock (_lw6p2p_db_t * db)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("trylock db"));
  ret = lw6sys_mutex_trylock (db->mutex);
  lw6sys_log (LW6SYS_LOG_DEBUG, _("trylock db ret=%d"), ret);

  return ret;
}

int
_lw6p2p_db_exec_ignore_data (_lw6p2p_db_t * db, char *sql)
{
  int ret = 0;

  ret = _lw6p2p_db_exec (db, sql, NULL, NULL);

  return ret;
}

int
_lw6p2p_db_exec (_lw6p2p_db_t * db, char *sql, _lw6p2p_db_callback_t func,
		 void *func_data)
{
  int ret = 0;
  int errcode = 0;
  char *errmsg = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("executing SQL statement \"%s\""), sql);
  if (_lw6p2p_db_trylock (db))
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _
		  ("trying to execute SQL statement \"%s\" while DB is not locked"),
		  sql);
      _lw6p2p_db_unlock (db);
    }
  else
    {
      errcode = sqlite3_exec (db->handler, sql, func, func_data, &errmsg);
      if (errcode == SQLITE_OK)
	{
	  ret = 1;
	}
      else
	{
	  if (errmsg)
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _
			  ("error executing SQL statement \"%s\" errcode=%d errmsg=\"%s\""),
			  sql, errcode, errmsg);
	      sqlite3_free (errmsg);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _
			  ("error executing SQL statement \"%s\" errcode=%d"),
			  sql, errcode);
	    }
	}
    }

  return ret;
}

int
_lw6p2p_db_create_database (_lw6p2p_db_t * db)
{
  int ret = 0;
  char *query = NULL;

  query = _lw6p2p_db_get_query (db, _LW6P2P_CREATE_DATABASE_SQL);
  if (query)
    {
      if (_lw6p2p_db_lock (db))
	{
	  ret = _lw6p2p_db_exec_ignore_data (db, query);
	  _lw6p2p_db_unlock (db);
	}
    }

  return ret;
}

int
_lw6p2p_db_clean_database (_lw6p2p_db_t * db)
{
  int ret = 0;
  char *query = NULL;

  query = _lw6p2p_db_get_query (db, _LW6P2P_CLEAN_DATABASE_SQL);
  if (query)
    {
      if (_lw6p2p_db_lock (db))
	{
	  ret = _lw6p2p_db_exec_ignore_data (db, query);
	  _lw6p2p_db_unlock (db);
	}
    }

  return ret;
}

/**
 * lw6p2p_db_reset
 *
 * @argc: number of args, as passed to @main
 * @argv: args array, as passed to @main
 * @name: the database name
 *
 * Clears the database. Simply removes the db file, in fact. Do
 * not call while database is used...
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6p2p_db_reset (int argc, char *argv[], char *name)
{
  int ret = 1;
  char *user_dir = NULL;
  char *filename = NULL;

  user_dir = lw6sys_get_user_dir (argc, argv);
  if (user_dir)
    {
      filename = lw6sys_path_concat (user_dir, name);
      if (filename)
	{
	  if (lw6sys_file_exists (filename))
	    {
	      if (!unlink (filename))
		{
		  lw6sys_log (LW6SYS_LOG_INFO, _("database \"%s\" deleted"),
			      filename);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _("can't delete database \"%s\""), filename);
		  ret = 0;
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _("no need to delete database \"%s\""), filename);
	    }
	  LW6SYS_FREE (filename);
	}
      LW6SYS_FREE (user_dir);
    }

  return ret;
}

/**
 * lw6p2p_db_default_name
 *
 * Returns the default database name, should be p2p.db (this is a relative path,
 * not an absolute path, will be appended to user dir).
 *
 * Return value: the default database name, need not (must not) be freed.
 */
char *
lw6p2p_db_default_name ()
{
  return _LW6P2P_DEFAULT_NAME;
}
