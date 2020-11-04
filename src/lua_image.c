#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <stdlib.h>
#include <fpioa.h>
#include <uart.h>
#include "lstate.h"

#define LUA_IMAGEHANDLE	"IMAGEHANDLE"

typedef struct {
    int width;
    int height;
    uint32_t *buf;
} struct_image;

#define image_buf(L)	((struct_image *)luaL_checkudata(L, 1, LUA_IMAGEHANDLE))

static struct_image *newbuf (lua_State *L)
{
    struct_image* p = (struct_image *)lua_newuserdatauv(L, sizeof(struct_image), 0);
    p->width = 0;
    p->height = 0;
    p->buf = NULL;
    luaL_setmetatable(L, LUA_IMAGEHANDLE);
    return p;
}

static void new_size(struct_image *img, int width, int height)
{
    if((width * height) == (img->width * img->height))
    {
        img->width = width;
        img->height = height;
        return;
    }
    img->width = width;
    img->height = height;
    if(img->buf != NULL)
        free(img->buf);
    if((width != 0) && (height != 0))
    {
        img->buf = malloc(width*height*2);
    }
}

static int lua_image_init (lua_State *L) {
    struct_image *img;
    int i, width, height;
    if(lua_gettop(L) == 2)
    {
        width = luaL_checkinteger(L, 1);
        height = luaL_checkinteger(L, 2);
        img = newbuf(L);
        new_size(img, width, height);
        for(i=0;i<width*height*2;i++)
        {
            ((uint16_t *)img->buf)[i] = 0xFFFF;
        }
    }
    else
        img = newbuf(L);
    return 1;
}

static int lua_image_getbuf (lua_State *L) {
    struct_image *img = image_buf(L);
    lua_pushlightuserdata(L, img->buf);
    return 1;
}

static int image_gc (lua_State *L) {
    struct_image *img = image_buf(L);
    if(img->buf != NULL)
    {
        free(img->buf);
    }
    img->buf = NULL;
    img->width = 0;
    img->height = 0;
    return 0;
}

static int image_tostring (lua_State *L) {
    struct_image *img = image_buf(L);
    lua_pushfstring(L, "width %d, height %d\n", img->width, img->height);
    return 1;
}

static const luaL_Reg imagelib[] = {
    {"init", lua_image_init},
    {NULL, NULL}
};


static const luaL_Reg meth[] = {
    {"getbuf", lua_image_getbuf},
    {NULL, NULL}
};

static const luaL_Reg metameth[] = {
  {"__index", NULL},  /* place holder */
  {"__gc", image_gc},
  {"__close", image_gc},
  {"__tostring", image_tostring},
  {NULL, NULL}
};

static void createmeta (lua_State *L) {
    luaL_newmetatable(L, LUA_IMAGEHANDLE);  /* create metatable for file handles */
    luaL_setfuncs(L, metameth, 0);  /* add metamethods to new metatable */
    luaL_newlibtable(L, meth);  /* create method table */
    luaL_setfuncs(L, meth, 0);  /* add file methods to method table */
    lua_setfield(L, -2, "__index");  /* metatable.__index = method table */
    lua_pop(L, 1);  /* pop metatable */
}

LUAMOD_API int luaopen_image (lua_State *L) {
    luaL_newlib(L, imagelib);  /* new module */
    createmeta(L);
    return 1;
}
