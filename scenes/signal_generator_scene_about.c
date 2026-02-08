#include "signal_generator_i.h"

void signal_generator_scene_about_on_enter(void* context) {
    furi_assert(context);

    SignalGenerator* app = context;

    view_dispatcher_switch_to_view(app->view_dispatcher, SignalGeneratorViewAbout);
}

bool signal_generator_scene_about_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);
    furi_assert(event);

    SignalGenerator* app = context;

    bool consumed = false;

    if(event.type == SceneManagerEventTypeTick) {
        about_view_update(app->about_view);

        consumed = true;
    }

    return consumed;
}

void signal_generator_scene_about_on_exit(void* context) {
    furi_assert(context);

    SignalGenerator* app = context;

    about_view_reset(app->about_view);
}
