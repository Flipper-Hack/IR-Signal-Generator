#include "signal_generator_i.h"

static bool signal_generator_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    SignalGenerator* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool signal_generator_back_event_callback(void* context) {
    furi_assert(context);
    SignalGenerator* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static void signal_generator_tick_event_callback(void* context) {
    furi_assert(context);
    SignalGenerator* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}

SignalGenerator* signal_generator_alloc() {
    SignalGenerator* app = malloc(sizeof(SignalGenerator));

    // View Dispatcher
    app->view_dispatcher = view_dispatcher_alloc();
    app->scene_manager = scene_manager_alloc(&signal_generator_scene_handlers, app);

    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, signal_generator_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, signal_generator_back_event_callback);
    view_dispatcher_set_tick_event_callback(app->view_dispatcher, signal_generator_tick_event_callback, 100);

    // Open GUI Record
    app->gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // SubMenu
    app->submenu = submenu_alloc();
    view_dispatcher_add_view(app->view_dispatcher, SignalGeneratorViewSubmenu, submenu_get_view(app->submenu));

    // About View
    app->about_view = about_view_alloc();
    view_dispatcher_add_view(app->view_dispatcher, SignalGeneratorViewAbout, about_view_get(app->about_view));

    return app;
}

void signal_generator_free(SignalGenerator* app) {
    furi_assert(app);

    // Submenu
    view_dispatcher_remove_view(app->view_dispatcher, SignalGeneratorViewSubmenu);
    submenu_free(app->submenu);

    // About View
    view_dispatcher_remove_view(app->view_dispatcher, SignalGeneratorViewAbout);
    about_view_free(app->about_view);

    // View Dispatcher
    view_dispatcher_free(app->view_dispatcher);

    // Scene Manager
    scene_manager_free(app->scene_manager);

    // Close GUI Record
    furi_record_close(RECORD_GUI);
    app->gui = NULL;

    free(app);
}

int32_t signal_generator(void* p) {
    UNUSED(p);
    SignalGenerator* app = signal_generator_alloc();

    scene_manager_next_scene(app->scene_manager, SignalGeneratorSceneStart);

    view_dispatcher_run(app->view_dispatcher);

    signal_generator_free(app);

    return 0;
}
