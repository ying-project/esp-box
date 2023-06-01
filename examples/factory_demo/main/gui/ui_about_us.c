/*
 * SPDX-FileCopyrightText: 2015-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include "esp_log.h"
#include "bsp_board.h"
#include "bsp/esp-bsp.h"
#include "lvgl.h"
#include "app_led.h"
#include "app_sr.h"
#include "settings.h"
#include "ui_main.h"
#include "ui_about_us.h"
#include "ui_factory_mode.h"

static const char *TAG = "ui_about_us";

static void (*g_about_us_end_cb)(void) = NULL;


static void ui_about_us_page_return_click_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_user_data(e);
    if (ui_get_btn_op_group()) {
        lv_group_remove_all_objs(ui_get_btn_op_group());
    }
#if CONFIG_BSP_BOARD_ESP32_S3_BOX
    bsp_btn_rm_all_callback(BOARD_BTN_ID_HOME);
#endif
    lv_obj_del(obj);
    bsp_btn_rm_all_callback(BOARD_BTN_ID_BOOT);
    if (g_about_us_end_cb) {
        g_about_us_end_cb();
    }
}

static void btn_return_down_cb(void *handle, void *arg)
{
    lv_obj_t *obj = (lv_obj_t *) arg;
    ui_acquire();
    lv_event_send(obj, LV_EVENT_CLICKED, NULL);
    ui_release();
}

static void factory_end_cb(void)
{
    ui_about_us_start(g_about_us_end_cb);
}

static void btn_factory_cb(void *handle, void *arg)
{
    lv_obj_t *obj = (lv_obj_t *) arg;
    ESP_LOGI(TAG, "enter factory mode");
    ui_acquire();
    if (ui_get_btn_op_group()) {
        lv_group_remove_all_objs(ui_get_btn_op_group());
    }
    lv_obj_del(obj);
    bsp_btn_rm_all_callback(BOARD_BTN_ID_BOOT);
    ui_factory_start(factory_end_cb);
    ui_release();

}

void ui_about_us_start(void (*fn)(void))
{
    g_about_us_end_cb = fn;

    lv_obj_t *page = lv_obj_create(lv_scr_act());
    lv_obj_set_size(page, 290, 190);
    lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(page, 15, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(page, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(page, 20, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(page, LV_OPA_30, LV_PART_MAIN);
    lv_obj_align(page, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t *btn_return = lv_btn_create(page);
    lv_obj_set_size(btn_return, 24, 24);
    lv_obj_add_style(btn_return, &ui_button_styles()->style, 0);
    lv_obj_add_style(btn_return, &ui_button_styles()->style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_return, &ui_button_styles()->style_focus, LV_STATE_FOCUS_KEY);
    lv_obj_add_style(btn_return, &ui_button_styles()->style_focus, LV_STATE_FOCUSED);
    lv_obj_align(btn_return, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_t *lab_btn_text = lv_label_create(btn_return);
    lv_label_set_text_static(lab_btn_text, LV_SYMBOL_LEFT);
    lv_obj_set_style_text_color(lab_btn_text, lv_color_make(158, 158, 158), LV_STATE_DEFAULT);
    lv_obj_center(lab_btn_text);
    lv_obj_add_event_cb(btn_return, ui_about_us_page_return_click_cb, LV_EVENT_CLICKED, page);
#if CONFIG_BSP_BOARD_ESP32_S3_BOX
    bsp_btn_register_callback(BOARD_BTN_ID_HOME, BUTTON_PRESS_UP, btn_return_down_cb, (void *)btn_return);
#endif

    if (ui_get_btn_op_group()) {
        lv_group_add_obj(ui_get_btn_op_group(), btn_return);
    }

    const sys_param_t *param = settings_get_parameter();
    const boards_info_t *board = bsp_board_get_info();
    LV_IMG_DECLARE(icon_box);
    LV_IMG_DECLARE(icon_box_lite);
    lv_obj_t *img = lv_img_create(page);
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 20);
    if (BOARD_S3_BOX_LITE == board->id) {
        lv_img_set_src(img, &icon_box_lite);
    } else if (BOARD_S3_BOX == board->id) {
        lv_img_set_src(img, &icon_box);
    }

    char msg[256] = {0};
    snprintf(msg, sizeof(msg),
             "#000000 Software Ver: # "  "#888888 V%u.%u.%u#\n"
             "#000000 ESP-IDF Ver: # "   "#888888 %s#\n"
             "#000000 SR LANG: # "       "#888888 %s#\n"
             "#000000 Board: # "         "#888888 %s#",
             BOX_DEMO_VERSION_MAJOR, BOX_DEMO_VERSION_MINOR, BOX_DEMO_VERSION_PATCH,
             esp_get_idf_version(),
             SR_LANG_EN == param->sr_lang ? "English" : "Chinese",
             board->name);

    lv_obj_t *lab = lv_label_create(page);
    lv_label_set_recolor(lab, true);
    lv_label_set_text(lab, msg);
    lv_obj_align(lab, LV_ALIGN_BOTTOM_LEFT, 0, -10);

    bsp_btn_register_callback(BOARD_BTN_ID_BOOT, BUTTON_SINGLE_CLICK, btn_factory_cb, (void *)page);
}
