#include "about.h"

#include <furi.h>

#define CREDITS_START_Y    28
#define CREDITS_END_Y      52
#define CREDIT_LINE_HEIGHT 10
#define SCROLL_SPEED       1

static const char* credits[] = {
    "",
    "-=> App Development by",
    "Lemn4t",
    "sacriphanius",
    "-=> Design by",
    "Lemn4t",
    "sacriphanius",
};

#define CREDITS_COUNT (sizeof(credits) / sizeof(credits[0]))

struct AboutView {
    View* view;
};

typedef struct {
    uint8_t frame;
    uint8_t seed;
    int16_t scroll_offset;
} AboutViewModel;

static void draw_noise_pixels(Canvas* canvas, uint8_t density) {
    for(uint8_t i = 0; i < density; i++) {
        canvas_draw_dot(canvas, rand() % 128, rand() % 64);
    }
}

static void about_view_draw_callback(Canvas* canvas, void* context) {
    AboutViewModel* m = context;

    srand(m->seed);
    canvas_clear(canvas);

    // Light background static
    canvas_set_color(canvas, ColorBlack);
    draw_noise_pixels(canvas, 6 + (rand() % 6));

    // Occasional subtle x-jitter
    int8_t x_off = (rand() % 15 == 0) ? ((rand() % 4) - 2) : 0;

    // Animated FHP decoration (centered)
    canvas_set_font(canvas, FontKeyboard);
    if(m->frame % 8 < 4) {
        canvas_draw_str_aligned(canvas, 64, 18, AlignCenter, AlignBottom, ">>>=================<<<");
    } else {
        canvas_draw_str_aligned(canvas, 64, 18, AlignCenter, AlignBottom, ">>>======[FHP]======<<<");
    }

    // Draw credits region (clip area)
    canvas_set_font(canvas, FontSecondary);

    // Calculate total scroll height
    int16_t total_height = CREDITS_COUNT * CREDIT_LINE_HEIGHT;

    // Draw scrolling credits
    for(size_t i = 0; i < CREDITS_COUNT; i++) {
        int16_t y = CREDITS_START_Y + (i * CREDIT_LINE_HEIGHT) - m->scroll_offset;

        // Wrap around for endless scroll
        while(y < CREDITS_START_Y - CREDIT_LINE_HEIGHT) {
            y += total_height;
        }
        while(y > CREDITS_START_Y + total_height) {
            y -= total_height;
        }

        // Only draw if in visible region
        if(y >= CREDITS_START_Y - CREDIT_LINE_HEIGHT && y <= CREDITS_END_Y) {
            canvas_draw_str(canvas, x_off, y, credits[i]);
        }
    }

    // Draw fade/mask bars at top and bottom of credits area
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, 0, 0, 128, CREDITS_START_Y - CREDIT_LINE_HEIGHT);
    canvas_draw_box(canvas, 0, CREDITS_END_Y, 128, 14);

    // Redraw header over mask
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, x_off, 10, "IR Signal Generator v" FAP_VERSION);

    canvas_set_font(canvas, FontKeyboard);
    if(m->frame % 8 < 4) {
        canvas_draw_str_aligned(canvas, 64, 18, AlignCenter, AlignBottom, ">>>=================<<<");
    } else {
        canvas_draw_str_aligned(canvas, 64, 18, AlignCenter, AlignBottom, ">>>======[FHP]======<<<");
    }

    // Redraw static in header area
    srand(m->seed + 1);
    for(uint8_t i = 0; i < 3; i++) {
        canvas_draw_dot(canvas, rand() % 128, rand() % (CREDITS_START_Y - CREDIT_LINE_HEIGHT));
    }

    // Footer
    canvas_set_font(canvas, FontKeyboard);
    canvas_draw_str_aligned(canvas, 64, 62, AlignCenter, AlignBottom, "t.me/Lemn4t");

    // Rare subtle glitch bar
    if(rand() % 30 == 0) {
        canvas_set_color(canvas, ColorXOR);
        uint8_t y = rand() % 60;
        canvas_draw_box(canvas, 0, y, 128, 2);
    }
}

static bool about_view_input_callback(InputEvent* event, void* context) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

AboutView* about_view_alloc(void) {
    AboutView* instance = malloc(sizeof(AboutView));

    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(AboutViewModel));
    view_set_draw_callback(instance->view, (ViewDrawCallback)about_view_draw_callback);
    view_set_input_callback(instance->view, (ViewInputCallback)about_view_input_callback);
    view_set_context(instance->view, instance);

    with_view_model(
        instance->view,
        AboutViewModel * model,
        {
            model->frame = 0;
            model->seed = furi_get_tick() & 0xFF;
            model->scroll_offset = 0;
        },
        false);

    return instance;
}

void about_view_free(AboutView* instance) {
    furi_assert(instance);

    view_free(instance->view);
    free(instance);
}

View* about_view_get(AboutView* instance) {
    furi_assert(instance);

    return instance->view;
}

void about_view_update(AboutView* instance) {
    furi_assert(instance);

    with_view_model(
        instance->view,
        AboutViewModel * model,
        {
            model->frame++;
            model->seed = rand();

            if(model->frame % 2 == 0) {
                model->scroll_offset += SCROLL_SPEED;
                int16_t total_height = CREDITS_COUNT * CREDIT_LINE_HEIGHT;
                if(model->scroll_offset >= total_height) {
                    model->scroll_offset = 0;
                }
            }
        },
        true);
}

void about_view_reset(AboutView* instance) {
    furi_assert(instance);

    with_view_model(
        instance->view,
        AboutViewModel * model,
        {
            model->frame = 0;
            model->seed = furi_get_tick() & 0xFF;
            model->scroll_offset = 0;
        },
        false);
}
