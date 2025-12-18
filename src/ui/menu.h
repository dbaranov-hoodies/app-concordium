#ifndef COMMON_MENU_H
#define COMMON_MENU_H

#ifndef APPVERSION
#define APPVERSION "unknown"
#endif

/**
 * Show main menu (ready screen, version, about, quit).
 */
void ui_menu_main(void);

/**
 * Show about submenu (copyright, date).
 */
void ui_menu_about(void);

#endif  // COMMON_MENU_H