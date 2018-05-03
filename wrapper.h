#define TABLESIZE 18

extern int (*functable[])(lua_State *);
extern const char *nametable[]; 

extern void keyboard_handler( int scancode );
extern void mouse_handler( int flags );

extern int run_flag, key_event, mouse_event;
extern int key_event_scancode, mouse_event_flags;
extern BITMAP *display;
extern int screenX, screenY;
extern int color, bgcolor;
