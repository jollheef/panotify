/**
 * @file panotify.c
 * @author Mikhail Klementyev jollheef<AT>riseup.net
 * @license GNU GPLv3
 * @date March, 2014
 * @brief Purple advanced notify
 *
 * Реализация расширенных уведомлений при получении нового сообщения.
 */

#define PURPLE_PLUGINS

#include <glib-2.0/glib.h>
#include <stdlib.h>
#include <string.h>

#include "notify.h"
#include "plugin.h"
#include "version.h"
#include "eventloop.h"
#include "debug.h"
#include "prefs.h"

#include "panotify.h"

/**
 * Асинхронное исполнение команды.
 *
 * @param command [in] -- строка команды.
 */
static
void
execute(IN const char *command)
{
	if (0 > strlen(command)) {
		purple_debug_warning(PLUGIN_ID, "Nothing to execute\n");

		return;
	}

	if (TRUE == g_spawn_command_line_async(command, NULL)) {
		purple_debug_info(PLUGIN_ID, "Execute success\n");
	}
	else {
		purple_debug_warning(PLUGIN_ID, "Execute fail\n");
	}
}

/**
 * Получить количество непрочитанных сообщений.
 *
 * @param conv [in] -- указатель на объект беседы.
 * @return количество непрочитанных сообщений.
 */
static
int
get_unseen_count(IN PurpleConversation *conv)
{
	return GPOINTER_TO_INT(purple_conversation_get_data(conv,
                                                            "unseen-count"));
}

/**
 * Функция создания уведомлений о непрочитанных сообщениях.
 * Примечание: libpurple работает в один поток.
 *
 * @param conv [in] -- указатель на объект беседы.
 * @return TRUE если остались непрочитанные сообщение, FALSE в
 * обратном. После возврата FALSE eventloop больше не вызывает эту
 * функцию.
 */
static
gboolean
notify(IN PurpleConversation *conv)
{
	const char *cmd = purple_prefs_get_string("/plugins/core/panotify/command");

	execute(cmd);

	if (get_unseen_count(conv) > 0) {
		return TRUE;
	}
	else {
		purple_prefs_set_bool("/plugins/core/panotify/blinked", FALSE);

		return FALSE;
	}
}

/**
 * Обработчик события нового сообщения.
 *
 * @param conv [in] -- указатель на объект беседы.
 * @param type [in] -- тип обновления беседы.
 */
static
void
new_message_handler(IN PurpleConversation *conv,
                    IN PurpleConvUpdateType type)
{
	if (!purple_prefs_get_bool("/plugins/core/panotify/blinked")
	    && (get_unseen_count(conv) > 0)) {
		int timeout = purple_prefs_get_int("/plugins/core/panotify/timeout");

		purple_timeout_add_seconds(timeout, (GSourceFunc)notify, conv);

		purple_prefs_set_bool("/plugins/core/panotify/blinked", TRUE);
	}
}

/**
 * Точка входа при загрузке плагина.
 *
 * @param plugin [in] -- указатель на объект плагина.
 * @return TRUE в случае успешной инициализации, FALSE в обратном.
 */
static
gboolean
plugin_load(IN  PurplePlugin *plugin)
{
	/* Установка обработчика обновления бесед/чатов */
	purple_signal_connect(purple_conversations_get_handle(),
	                      "conversation-updated",
	                      plugin,
	                      PURPLE_CALLBACK(new_message_handler),
	                      NULL);

	/* При инициализации флаг обязательно должен быть в FALSE */
	purple_prefs_add_bool("/plugins/core/panotify/blinked", FALSE);
	purple_prefs_set_bool("/plugins/core/panotify/blinked", FALSE);

	return TRUE;
}

/**
 * Инициализация настроек плагина.
 * Вызывается всякий раз, когда libpurple зондирует плагин.
 *
 * @param plugin [in] -- указатель на объект плагина.
 */
static
void
init_plugin(PurplePlugin *plugin)
{
	purple_prefs_add_none("/plugins/core/panotify");

	purple_prefs_add_string("/plugins/core/panotify/command",
	                        "/home/mikhail/bin/webcam_led_blink");

	purple_prefs_add_bool("/plugins/core/panotify/blinked", FALSE);

	purple_prefs_add_int("/plugins/core/panotify/timeout", 3);
}

/**
 * Функция создания интерфейса настроек плагина.
 *
 * @param plugin [in] -- объект плагина.
 * @return объект интерфейса плагина.
 */
static
PurplePluginPrefFrame *
plugin_pref_frame(IN PurplePlugin *plugin)
{
	PurplePluginPrefFrame *frame;

	PurplePluginPref *pref;

	frame = purple_plugin_pref_frame_new();

	pref = purple_plugin_pref_new_with_name_and_label("/plugins/core/panotify/command",
                                                          "command");
	purple_plugin_pref_frame_add(frame, pref);

	pref = purple_plugin_pref_new_with_name_and_label("/plugins/core/panotify/timeout",
                                                          "timeout (seconds)");
	purple_plugin_pref_set_bounds(pref, 1, 600);

	purple_plugin_pref_frame_add(frame, pref);

	return frame;
}

/**
 * Деинициализация плагина.
 *
 * @param plugin [in] -- указатель на объект плагина.
 * @return TRUE в случае успешной деинициализации, FALSE в обратном.
 */
static
gboolean
plugin_unload(IN PurplePlugin *plugin)
{
	purple_signal_disconnect(purple_conversations_get_handle(),
	                         "conversation-updated",
	                         plugin,
	                         PURPLE_CALLBACK(new_message_handler));

	purple_prefs_set_bool("/plugins/core/panotify/blinked", FALSE);

	return TRUE;
}

/* Инициализация плагина. Необходимо для успешной загрузки и работы. */
PURPLE_INIT_PLUGIN(panotify, init_plugin, info)
