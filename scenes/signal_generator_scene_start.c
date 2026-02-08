#include "signal_generator_i.h"

enum {
    SubmenuIndexGenerator,
    SubmenuIndexSearcher,
    SubmenuIndexAbout,
};

static void signal_generator_scene_start_submenu_callback(void* context, uint32_t index) {
    furi_assert(context);

    SignalGenerator* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void signal_generator_scene_start_on_enter(void* context) {
    furi_assert(context);

    SignalGenerator* app = context;

    submenu_add_item(
        app->submenu, "Generator", SubmenuIndexGenerator, signal_generator_scene_start_submenu_callback, app);
    submenu_add_item(
        app->submenu, "Searcher", SubmenuIndexSearcher, signal_generator_scene_start_submenu_callback, app);
    submenu_add_item(app->submenu, "About", SubmenuIndexAbout, signal_generator_scene_start_submenu_callback, app);

    submenu_set_selected_item(
        app->submenu, scene_manager_get_scene_state(app->scene_manager, SignalGeneratorSceneStart));

    view_dispatcher_switch_to_view(app->view_dispatcher, SignalGeneratorViewSubmenu);
}

bool signal_generator_scene_start_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);
    furi_assert(event);

    SignalGenerator* app = context;

    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubmenuIndexGenerator) {
            consumed = true;
        } else if(event.event == SubmenuIndexSearcher) {
            consumed = true;
        } else if(event.event == SubmenuIndexAbout) {
            scene_manager_next_scene(app->scene_manager, SignalGeneratorSceneAbout);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        scene_manager_stop(app->scene_manager);
        view_dispatcher_stop(app->view_dispatcher);

        consumed = true;
    }

    return consumed;
}

void signal_generator_scene_start_on_exit(void* context) {
    furi_assert(context);

    SignalGenerator* app = context;

    submenu_reset(app->submenu);
}
