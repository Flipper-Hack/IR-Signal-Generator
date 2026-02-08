#pragma once

#include "scenes/signal_generator_scene.h"
#include "helpers/signal_generator_types.h"

#include "views/about.h"

#include <furi.h>

#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>

#include <gui/modules/submenu.h>

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;

    Submenu* submenu;

    AboutView* about_view;
} SignalGenerator;
