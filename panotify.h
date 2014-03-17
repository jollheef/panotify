/**
 * @file panotify.h
 * @author Mikhail Klementyev jollheef<AT>riseup.net
 * @license GNU GPLv3
 * @date March, 2014
 * @brief Purple advanced notify header
 *
 * Содержит объявления общих переменных и функций для panotify.
 */

#define IN
#define OUT

#define PLUGIN_ID "panotify"

/**
 * Функция создания интерфейса настроек плагина.
 *
 * @param plugin [in] -- объект плагина.
 * @return объект интерфейса плагина.
 */
static
PurplePluginPrefFrame*
plugin_pref_frame(IN PurplePlugin* plugin);

/**
 * Точка входа при загрузке плагина.
 *
 * @param plugin [in] -- указатель на объект плагина.
 * @return TRUE в случае успешной инициализации, FALSE в обратном.
 */
static
gboolean
plugin_load(IN  PurplePlugin* plugin);

/**
 * Деинициализация плагина.
 *
 * @param plugin [in] -- указатель на объект плагина.
 * @return TRUE в случае успешной деинициализации, FALSE в обратном.
 */
static
gboolean
plugin_unload(IN PurplePlugin* plugin);

/**
 * Настройки плагина 
 */
static PurplePluginUiInfo prefs_info = {
  plugin_pref_frame,
  0,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

/**
 * Описание плагина.
 */
static PurplePluginInfo info = {
  PURPLE_PLUGIN_MAGIC,
  PURPLE_MAJOR_VERSION,
  PURPLE_MINOR_VERSION,
  PURPLE_PLUGIN_STANDARD,
  NULL,				/* наличие интерфейса */
  0,				/* флаги */
  NULL,				/* зависимости */
  PURPLE_PRIORITY_DEFAULT,

  "core-panotify",		/* id */
  "panotify",			/* название */
  "0.2",			/* версия */

  "Advanced notify",		/* краткое описание */
  "Advanced notify",		/* полное описание */
  "Mikhail Klementyev <jollheef@riseup.net>", /* автор */
  NULL,					      /* домашняя страница */

  plugin_load,			/* обработчик загрузки */
  plugin_unload,		/* обработчик выгрузки */
  NULL,				/* обработчик уничтожения */

  NULL,				/* ui_info */
  NULL,				/* extra_info */
  &prefs_info,			/* prefs_info */
  NULL				/* actions */
};
