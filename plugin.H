/**
 * Copyright 2003-2012 Totaal Software / Sander Verhagen
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */

#include <windows.h>

#ifndef TRILLIAN_PLUGIN_H
#define TRILLIAN_PLUGIN_H

typedef int (*ttkCallback)(int windowID, char *subwindow, char *event, void *data, void *userData);
typedef int (*plugin_function)(char *guid, char *event, void *data);
typedef int (*plugin_function_send)(char *guid, char *event, void *data);
typedef int (*plugin_function_main)(char *event, void *data);
typedef int (*plugin_function_version)();

#define isTrillianError(error_code) (error_code < 0)
#define isTrillianSuccess(error_code) (error_code >= 0)
#define trillianInitialize(structure) do { memset(&structure,0,sizeof(structure)); structure.struct_size = sizeof(structure); } while (0); 
#define trillianListInitialize(structure)  \
	do {\
		memset(&structure,0,sizeof(structure)); structure.struct_size = sizeof(structure); \
		structure.section_id = structure.parent_id = structure.previous_id = structure.unique_id = -1;\
		structure.selectable = 1;\
		trillianInitialize(structure.font); \
		structure.font.flags = 1; \
		structure.font.hover_background = structure.font.hover_foreground = structure.font.select_background = structure.font.select_foreground = structure.font.background = structure.font.foreground = -1; \
	} while (0); 

#define TRILLIAN_PREFS_CANCEL 0
#define TRILLIAN_PREFS_APPLY  1
#define TRILLIAN_PREFS_OK     2

#define TRILLIAN_PREFS_SHOW   1
#define TRILLIAN_PREFS_HIDE   0

#define MENU_TEXT             0  
#define MENU_SEPARATOR        1  
#define MENU_POPUP            2  
#define MENU_HEADING          3  
#define MENU_CALLBACK         4 
#define MENU_DEFAULT          5   
    
#define MENU_TIMEOUT_DEFAULT  0  
#define MENU_TIMEOUT_INFINITE -1  

struct plugin_info_t
{
	unsigned int   struct_size;

	/* Information given to the plugin */
	char *config_directory;
	char *skin_directory;
	char *temp_directory;
	plugin_function_send plugin_send;

	/* Plugin Fills out this information */
	char guid[128];

	char name[256];
	char company[256];
	char version[64];
	char description[1024];
};

struct alert_t
{
	unsigned int   	struct_size;

	char		*text;
	char		*type;
	int		word_wrap;
	int		formatted;
	int		link;
	ttkCallback 	callback;
	void		*data;
};

struct plugin_prefs_info_t
{
	unsigned int   	struct_size;

	char		name[128];
	char		description[1024];

	HBITMAP		bitmap;
};

struct plugin_prefs_show_t
{
	unsigned int	   		struct_size;

	int   				show; //1 shows, 0 hides

	char 				*pref_name;
	char 				*sub_entry;
	
	HWND  				hwnd;
	int   				x;
	int   				y;

	struct plugin_prefs_info_t 	prefs_info;
};

struct plugin_prefs_action_t
{
	unsigned int   			struct_size;

	int 				type;  //0 = cancel, 1 = apply, 2 = ok
};


struct plugin_prefs_entry_t
{
	unsigned int   			struct_size;

	char 				*sub_name;
	struct plugin_prefs_entry_t 	*next;
};

struct plugin_prefs_t
{
	unsigned int   			struct_size;

	int 				enabled;

	char 				*pref_name;

	struct plugin_prefs_entry_t 	*sub_entry;
};

struct alias_t
{
	unsigned int   		struct_size;

	int     		alias_id;

	char			*text;

	ttkCallback		callback;
	void			*data;
};

struct alias_request_t
{
	unsigned int   		struct_size;

	int			alias_id;

	char			*alias;
	char			*text;
};

struct keyboard_alias_t
{
	unsigned int   		struct_size;

	int			alias_id;

	int			alt;
	int			ctrl;
	int			shift;

	char			key;

	ttkCallback		callback;
	void			*data;
};

struct keyboard_alias_request_t
{
	unsigned int   		struct_size;

	int			alias_id;

	int			alt;
	int			ctrl;
	int			shift;
	char			key;

	char			*text; //to be inserted...if none, make this NULL
}; 

struct list_bmp_t
{
	unsigned int   		struct_size;

	int			num_states;
	HBITMAP			*bitmap;
	RECT			*location;

	/* Animated support not available currently */
	int			animated;
	int			time;
	int			loop;
	int			cur_state;

	/* skin based references */
	int			*skin_based;  
	char			**skin_name;
	char			**skin_type;
	char			**skin_state;
};

struct list_font_t
{
	unsigned int   		struct_size;

	int			flags;  //0x00000000000000
					//0x00001 = skins
					//0x00010 = font file not the other stuff

	char   			*skin_name;

	HFONT   		font;

	//skin based
	char			*hover_fore;
	char			*hover_back;
	char			*select_fore;
	char			*select_back;
	char			*normal_fore;
	char			*normal_back;

	//nonskin based
	COLORREF  		hover_background;
	COLORREF  		hover_foreground;
	COLORREF  		select_background;
	COLORREF  		select_foreground;
	COLORREF  		background;
	COLORREF  		foreground;

};

struct menu_entry_t
{
	unsigned int   		struct_size;

	int			menu_id;
	int			sub_menu_id;
	int			type;
	
	int			timeout;
	int			disabled;
	int			num_copies;

	char			*icon;
	char			*text;
	char			*shortcut;

	void			*data;
	HWND			hwnd;

	struct menu_entry_t 	*sub_menu;
	struct menu_entry_t 	*next_menu;

	ttkCallback 		callback;
};

struct list_entry_t 
{
	unsigned int   		struct_size;

	int			num_left_icons;		
	struct list_bmp_t	*left_icons;

	int			num_right_icons;		
	struct list_bmp_t	*right_icons;

	struct list_font_t	font;

	int			section_id;
	int			parent_id;
	int			previous_id;
	int			unique_id;

	int			group;				
	int			section;

	int			drag_and_drop;
	int			inline_editing;		
	int			selectable;			
	int			expanded;			
	int			auto_expand;
	int			no_group_icon;
	int			no_double_click;
	int			owner_drawn;
	int			no_section_margins;
	
	int			floating;
	int			floating_x;
	int			floating_y;

	char			*tooltip;			
	char			*text;				
	char			*edit_string;
	char			*real_name;

	void			*data;				
	
	struct menu_entry_t	*menu_entry;
	ttkCallback		callback;
};

struct dialog_entry_t
{
	unsigned int		struct_size;

	HWND			hwnd;
};

struct list_update_t 
{
	unsigned int	struct_size;

	HDC				hdc;
	HWND			hwnd;

	int				x;
	int				y;
	int				y_offset;

	int				width;
	int				height;

	//colors
	COLORREF		hover_background;
	COLORREF		hover_foreground;
	COLORREF		hover_border;
	COLORREF		select_background;	
	COLORREF		select_foreground;
	COLORREF		select_border;
	COLORREF		background;
	COLORREF		foreground;

	//state	
	int				selected;
	int				hover;
	int				floating;
	int				black_and_white;

};

struct connection_enum_t
{
	unsigned int	struct_size;

	char			*medium;

	ttkCallback		callback;
	void			*data;
};

struct connection_entry_t
{
	unsigned int	struct_size;

	char			*medium;
	int				connection_id;

	char			*name;
	char			*status;
};

struct contactlist_enum_t
{
	unsigned int	struct_size;

	char			*medium;
	int				connection_id;

	ttkCallback		callback;
	void			*data;
};

struct contactlist_entry_t
{
	unsigned int	struct_size;

	char			*medium;
	int				connection_id;

	char			*name;
	char			*real_name;
	char			*status;
};

struct message_t
{
	unsigned int	struct_size;

	char			*medium;
	int				connection_id;

	char			*name;
	char			*type;
	char			*text;

	char			*location;

	void			*extra_information;
};

struct message_broadcast_t
{
	unsigned int	struct_size;

	int				broadcast_id;

	ttkCallback		callback;
	void			*data;
};

#endif

