#include <X11/XF86keysym.h>
/* See LICENSE file for copyright and license details. */

/* appearance */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#057493";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "@DEFAULT_SINK@", "toggle", NULL };
static const char *mutemic[] = { "/usr/bin/pactl", "set-source-mute", "@DEFAULT_SOURCE@", "toggle", NULL };

static const char *light_up[]   = { "/usr/bin/brightnessctl", "set", "5%+", NULL };
static const char *light_down[] = { "/usr/bin/brightnessctl", "set",
"5%-", NULL };

static const char *powermenucmd[] = { "/home/evan/.local/bin/powermenu", NULL };

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *browsercmd[] = { "firefox-bin", NULL };
static const char *emacsclientcmd[] = { "emacsclient", "-c", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_g,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_o,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_y,      spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_t,      spawn,          {.v = emacsclientcmd } },
	{ MODKEY,                       XK_agrave,      spawn,
	SHCMD("bluetoothctl connect 84:D3:52:C5:B5:2C") },
	{ MODKEY|ShiftMask,             XK_a,      spawn,
	SHCMD("flameshot gui") },
	{ MODKEY,                       XK_m,      togglebar,      {0} },
	{ MODKEY,                       XK_f,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_b,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_quotedbl,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_parenright,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_colon,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_apostrophe,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_comma,      killclient,     {0} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_k,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_underscore,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_underscore,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_backslash,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_dead_circumflex, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_backslash,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_dead_circumflex, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_equal,                      0)
	TAGKEYS(                        XK_slash,                      1)
	TAGKEYS(                        XK_minus,                      2)
	TAGKEYS(                        XK_egrave,                      3)
	{ MODKEY|ShiftMask,             XK_eacute,      quit,	{0} },
	{ 0,                       XF86XK_AudioLowerVolume,  spawn,          {.v = downvol } },
	{ 0,                       XF86XK_AudioMute,         spawn,          {.v = mutevol } },
	{ 0,                       XF86XK_AudioRaiseVolume,  spawn,          {.v = upvol   } },
	{ 0,                       XF86XK_AudioMicMute,      spawn,          {.v = mutemic } },
	{ 0,                       XF86XK_MonBrightnessUp,   spawn,          {.v = light_up } },
	{ 0,                       XF86XK_MonBrightnessDown, spawn,
	{.v = light_down } },
	{ 0, XK_Print, spawn, {.v = powermenucmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};




static const unsigned int gappx = 8;
