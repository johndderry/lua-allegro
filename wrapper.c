/*
** Lua-Allegro Allegro wrapper
*/

#include "lprefix.h"

#include <stdio.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <allegro.h>
#include "wrapper.h"

/*
 * ALLEGRO stuff
 * 
*/
int screenX = 800, screenY = 600;

int run_flag = 1;
int key_event = 0, mouse_event = 0;
int key_event_scancode, mouse_event_flags;
int color, bgcolor;

static int xpos = 20, ypos = 20, mousedown = 0;
static char *keybuff = "";

BITMAP *display;

void keyboard_handler( int scancode ) {
	
	key_event_scancode = scancode;
	key_event = 1;
}

void mouse_handler( int flags ) {
	
	mouse_event_flags = flags;
	mouse_event = 1;
}

/*
 * These are stubs, to be redefined by user in main.lua
 */
 
static int al_load(lua_State *L) {
	return 0;
}

static int al_update(lua_State *L) {
	return 0;
}

static int al_draw(lua_State *L) {
	
	lua_getglobal(L, "allegro");
	lua_getfield(L, -1, "rectangle");
	lua_pushstring(L, "line");
	lua_pushinteger(L, xpos);
	lua_pushinteger(L, ypos);
	lua_pushinteger(L, xpos+200);
	lua_pushinteger(L, ypos+200);	
	int ret = lua_pcall(L, 5, 0, 0);
	if( ret ) 
		fprintf(stderr, "error calling load: %s\n", lua_tostring(L, -1) );
	
	lua_getfield(L, -1, "print");
	lua_pushstring(L, "lua-allegro");
	lua_pushinteger(L, xpos + 10);
	lua_pushinteger(L, ypos + 10);
	lua_pcall(L, 3, 0, 0);

	lua_getfield(L, -1, "print");
	lua_pushstring(L, keybuff);
	lua_pushinteger(L, xpos + 10);
	lua_pushinteger(L, ypos + 30);
	lua_pcall(L, 3, 0, 0);

	lua_pop(L, 1);	
	return 0;
}

static int al_keypressed(lua_State *L) {
	return 0;
}

static int al_keyreleased(lua_State *L) {

	keybuff = lua_tostring(L, 1);
	if( strcmp( keybuff, "Escape") == 0 ) run_flag = 0;

	return 0;
}

static int al_mousepressed(lua_State *L) {
	mousedown = 1;
	//fprintf(stderr,"mousedown ");
	return 0;
}

static int al_mousereleased(lua_State *L) {
	mousedown = 0;
	//fprintf(stderr,"mouseup ");
	return 0;
}

static int al_mousemoved(lua_State *L) {
	if( !mousedown ) return 0;
 
	int dx = lua_tointeger(L, 3);
	int dy = lua_tointeger(L, 4);
	xpos += dx;
	ypos += dy;
	return 0;
}

/*
 * These are the wrapper functions
 */
 
static int al_setMode(lua_State *L) {
	
	screenX = lua_tointeger(L, 1);
	screenY = lua_tointeger(L, 2);

	return 0;
}

static int al_setTitle(lua_State *L) {
	return 0;
}

static int al_setColor(lua_State *L) {
	int r = lua_tointeger(L, 1);
	int g = lua_tointeger(L, 2);
	int b = lua_tointeger(L, 3);
	
	color = makecol( r, g, b );
	return 0;
}

static int al_setBackColor(lua_State *L) {
	int r = lua_tointeger(L, 1);
	int g = lua_tointeger(L, 2);
	int b = lua_tointeger(L, 3);
	
	bgcolor = makecol( r, g, b );
	return 0;
}

static int al_rectangle(lua_State *L) {
	const char *mode = lua_tostring(L, 1);
	int xstart = lua_tointeger(L, 2);
	int ystart = lua_tointeger(L, 3);
	int xlen = lua_tointeger(L, 4);
	int ylen = lua_tointeger(L, 5);
	
	if( strcmp( mode, "fill" ) == 0 ) 
		rectfill( display, xstart, ystart, xstart+xlen, ystart+ylen, color );
	else
		rect( display, xstart, ystart, xstart+xlen, ystart+ylen, color );
	
	return 0;
}

static int al_circle(lua_State *L) {
	const char *mode = lua_tostring(L, 1);
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	int r = lua_tointeger(L, 4);
	
	if( strcmp( mode, "fill" ) == 0 ) 
		circlefill( display, x, y, r, color );
	else
		circle( display, x, y, r, color );
		
	return 0;
}

static int al_print(lua_State *L) {
	const char *s = lua_tostring(L, 1);
	int x = lua_tointeger(L, 2);
	int y = lua_tointeger(L, 3);
	
	textout_ex(display, font, s, x, y, color, bgcolor);
	return 0;
}

static int al_line(lua_State *L) {
	int xstart = lua_tointeger(L, 1);
	int ystart = lua_tointeger(L, 2);
	int xend = lua_tointeger(L, 3);
	int yend = lua_tointeger(L, 4);
	
	line(display, xstart, ystart, xend, yend, color);
	return 0;
}

static int al_quit(lua_State *L) {
	run_flag = 0;
	return 0;
}

static int al_playmidifile(lua_State *L) {
	MIDI *mfile;
	int ret;
	mfile = load_midi("out.mid");
	if( mfile ) {
		ret = play_midi( mfile, 0 );
		destroy_midi( mfile );
	}
	return 0;
}

int (*functable[])(lua_State *) = {
	al_load, al_update, al_draw, al_keypressed, al_keyreleased,
	al_mousepressed, al_mousereleased, al_mousemoved,
	al_setMode, al_setTitle, al_setColor, al_setBackColor, 
	al_rectangle, al_circle, al_print, al_line, al_quit, al_playmidifile
};

const char *nametable[] = {
	"load", "update", "draw", "keypressed", "keyreleased",
	"mousepressed", "mousereleased", "mousemoved",
	"setMode", "setTitle", "setColor", "setBackgroundColor",
	"rectangle", "circle", "print", "line", "quit", "playmidi",
};
