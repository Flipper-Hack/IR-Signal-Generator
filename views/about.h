#pragma once

#include <gui/view.h>

typedef struct AboutView AboutView;

#ifdef __cplusplus
extern "C" {
#endif

AboutView* about_view_alloc(void);
void about_view_free(AboutView* instance);
View* about_view_get(AboutView* instance);

void about_view_update(AboutView* instance);
void about_view_reset(AboutView* instance);

#ifdef __cplusplus
}
#endif
