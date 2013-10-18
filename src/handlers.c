#include "tlp-config.h"

int
delete_substring (char **where, char *what)
{
    char *substring = safe_strstr (*where, what);
    if (!substring) {
        return 0;
    }
    snprintf (substring, strlen (*where) - strlen (what) + 1, "%s", substring + strlen (what));
    *where = realloc (*where, (strlen (*where) + 1) * sizeof (char));
    return 1;
}

void
set_text (char *value_name, char *text)
{
    TLPValue *value = get_value_by_name (value_name);
    int l = strlen (text) + 1;
    
    value->str = realloc (value->str, l);
    snprintf (value->str, l, "%s", text);
}

void
set_int (char *value_name, int num)
{
    TLPValue *value = get_value_by_name (value_name);
    value->num = num;
}

void
toggle_device (gboolean toggle, char *value_name, char *device)
{
    TLPValue *value = get_value_by_name (value_name);
    
    int l1 = 0;
    int l2 = strlen (device) + 1;
    
    char *s_device = calloc (l2 + 1, sizeof (char));
    char *device_s = calloc (l2 + 1, sizeof (char));
    snprintf (s_device, l2 + 1, " %s", device);
    snprintf (device_s, l2 + 1, "%s ", device);
    
    if (toggle == TRUE)
    {
        if (value->str) {
            l1 = strlen (value->str);
            if (l1) {
                value->str = realloc (value->str, l1 + l2 + 1);
                snprintf (value->str + l1, l2 + 1, "%s", s_device);
            } else {
                value->str = realloc (value->str, l1 + l2);
                snprintf (value->str, l2, "%s", device);
            }
        } else {
            value->str = calloc (l2, sizeof (char));
            snprintf (value->str, l2, "%s", device);
        }
    } else {
        if (!delete_substring (&value->str, s_device) && !delete_substring (&value->str, device_s)) {
            delete_substring (&value->str, device);
        }
    }
    
    free (s_device);
    free (device_s);
}

void
get_governor_by_id (int index, char **governor)
{
    if (index < 0) {
        *governor = NULL;
        return;
    }
    int l;
    
    if (index == 0) {
        l = strlen ("ondemand") + 1;
        *governor = calloc (l, sizeof (char));
        snprintf (*governor, l, "ondemand");
        return;
    }
    
    if (index == 1) {
        l = strlen ("conservative") + 1;
        *governor = calloc (l, sizeof (char));
        snprintf (*governor, l, "conservative");
        return;
    }
    
    if (index == 2) {
        l = strlen ("powersave") + 1;
        *governor = calloc (l, sizeof (char));
        snprintf (*governor, l, "powersave");
        return;
    }
    
    if (index == 3) {
        l = strlen ("performance") + 1;
        *governor = calloc (l, sizeof (char));
        snprintf (*governor, l, "performance");
        return;
    }
    
    *governor = NULL;
    return;
}

void
get_sata_lpwr_by_id (int index, char **sata_lpwr)
{
    if (index < 0) {
        *sata_lpwr = NULL;
        return;
    }
    int l;
    
    if (index == 0) {
        l = strlen ("min_power") + 1;
        *sata_lpwr = calloc (l, sizeof (char));
        snprintf (*sata_lpwr, l, "min_power");
        return;
    }
    
    if (index == 1) {
        l = strlen ("medium_power") + 1;
        *sata_lpwr = calloc (l, sizeof (char));
        snprintf (*sata_lpwr, l, "medium_power");
        return;
    }
    
    if (index == 2) {
        l = strlen ("max_performance") + 1;
        *sata_lpwr = calloc (l, sizeof (char));
        snprintf (*sata_lpwr, l, "max_performance");
        return;
    }
    
    *sata_lpwr = NULL;
    return;
}

void
get_pcie_aspm_by_id (int index, char **pcie_aspm)
{
    if (index < 0) {
        *pcie_aspm = NULL;
        return;
    }
    int l;
    
    if (index == 0) {
        l = strlen ("default") + 1;
        *pcie_aspm = calloc (l, sizeof (char));
        snprintf (*pcie_aspm, l, "default");
        return;
    }
    
    if (index == 1) {
        l = strlen ("performance") + 1;
        *pcie_aspm = calloc (l, sizeof (char));
        snprintf (*pcie_aspm, l, "performance");
        return;
    }
    
    if (index == 2) {
        l = strlen ("powersave") + 1;
        *pcie_aspm = calloc (l, sizeof (char));
        snprintf (*pcie_aspm, l, "powersave");
        return;
    }
    
    *pcie_aspm = NULL;
    return;
}

void
get_pcie_rpm_by_id (int index, char **rpm)
{
    if (index < 0) {
        *rpm = NULL;
        return;
    }
    int l;
    
    if (index == 0) {
        l = strlen ("auto") + 1;
        *rpm = calloc (l, sizeof (char));
        snprintf (*rpm, l, "auto");
        return;
    }
    
    if (index == 1) {
        l = strlen ("on") + 1;
        *rpm = calloc (l, sizeof (char));
        snprintf (*rpm, l, "on");
        return;
    }
    
    *rpm = NULL;
    return;
}

void
get_radeon_clock_by_id (int index, char **clock)
{
    if (index < 0) {
        *clock = NULL;
        return;
    }
    int l;
    
    if (index == 0) {
        l = strlen ("low") + 1;
        *clock = calloc (l, sizeof (char));
        snprintf (*clock, l, "low");
        return;
    }
    
    if (index == 1) {
        l = strlen ("mid") + 1;
        *clock = calloc (l, sizeof (char));
        snprintf (*clock, l, "mid");
        return;
    }
    
    if (index == 2) {
        l = strlen ("high") + 1;
        *clock = calloc (l, sizeof (char));
        snprintf (*clock, l, "high");
        return;
    }
    
    if (index == 3) {
        l = strlen ("auto") + 1;
        *clock = calloc (l, sizeof (char));
        snprintf (*clock, l, "auto");
        return;
    }
    
    *clock = NULL;
    return;
}

void
get_radeon_dpm_by_id (int index, char **dpm)
{
    if (index < 0) {
        *dpm = NULL;
        return;
    }
    int l;
    
    if (index == 0) {
        l = strlen ("auto") + 1;
        *dpm = calloc (l, sizeof (char));
        snprintf (*dpm, l, "auto");
        return;
    }
    
    if (index == 1) {
        l = strlen ("low") + 1;
        *dpm = calloc (l, sizeof (char));
        snprintf (*dpm, l, "low");
        return;
    }
    
    if (index == 2) {
        l = strlen ("high") + 1;
        *dpm = calloc (l, sizeof (char));
        snprintf (*dpm, l, "high");
        return;
    }
    
    *dpm = NULL;
    return;
}


void
on_main_window_destroy (GObject *window)
{
    gtk_main_quit ();
}

void
save_button_clicked (GObject *calling_widget)
{
    save_settings_carefully ();
}

void
cancel_button_clicked (GObject *calling_widget)
{
    gtk_main_quit ();
}

void
warning_ok_button_clicked_cb (GObject *calling_widget)
{
    gtk_widget_hide (warning_window);
}

void
restart_tlp_button_clicked_cb (GObject *calling_widget)
{
    restart_tlp ();
}

void
defaults_button_clicked (GObject *callling_widget)
{
    restore_defaults ();
}

void
enable_tlp_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("TLP_ENABLE");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 1;
    } else {
        value->num = 0;
    }
}

void
nmi_watchdog_button_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("NMI_WATCHDOG");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 1;
    } else {
        value->num = 0;
    }
}

void
governor_ac_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("CPU_SCALING_GOVERNOR_ON_AC");
    get_governor_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str);
}

void
governor_bat_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("CPU_SCALING_GOVERNOR_ON_BAT");
    get_governor_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str);
}

void
min_freq_ac_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("CPU_SCALING_MIN_FREQ_ON_AC");
    GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX (calling_widget));
    GtkTreeIter iter;
    gtk_combo_box_get_active_iter (GTK_COMBO_BOX (calling_widget), &iter);
    gtk_tree_model_get (model, &iter, 0, &value->num, -1);
}

void
min_freq_bat_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("CPU_SCALING_MIN_FREQ_ON_BAT");
    GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX (calling_widget));
    GtkTreeIter iter;
    gtk_combo_box_get_active_iter (GTK_COMBO_BOX (calling_widget), &iter);
    gtk_tree_model_get (model, &iter, 0, &value->num, -1);
}

void
max_freq_ac_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("CPU_SCALING_MAX_FREQ_ON_AC");
    GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX (calling_widget));
    GtkTreeIter iter;
    gtk_combo_box_get_active_iter (GTK_COMBO_BOX (calling_widget), &iter);
    gtk_tree_model_get (model, &iter, 0, &value->num, -1);
}

void
max_freq_bat_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("CPU_SCALING_MAX_FREQ_ON_BAT");
    GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX (calling_widget));
    GtkTreeIter iter;
    gtk_combo_box_get_active_iter (GTK_COMBO_BOX (calling_widget), &iter);
    gtk_tree_model_get (model, &iter, 0, &value->num, -1);
}

void
disable_turbo_ac_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("CPU_BOOST_ON_AC");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 0;
    } else {
        value->num = 1;
    }
}

void
disable_turbo_bat_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("CPU_BOOST_ON_BAT");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 0;
    } else {
        value->num = 1;
    }
}

void
min_cores_ac_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("SCHED_POWERSAVE_ON_AC");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 1;
    } else {
        value->num = 0;
    }
}

void
min_cores_bat_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("SCHED_POWERSAVE_ON_BAT");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 1;
    } else {
        value->num = 0;
    }
}

void
laptop_mode_ac_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("DISK_IDLE_SECS_ON_AC");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 2;
    } else {
        value->num = 0;
    }
}

void
laptop_mode_bat_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("DISK_IDLE_SECS_ON_BAT");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 2;
    } else {
        value->num = 0;
    }
}

void
timeout_unsaved_data_writing_ac_adjustment_value_changed_cb (GObject *calling_widget)
{
    set_int ("MAX_LOST_WORK_SECS_ON_AC", (int) gtk_adjustment_get_value (GTK_ADJUSTMENT (calling_widget)));
}

void
timeout_unsaved_data_writing_bat_adjustment_value_changed_cb (GObject *calling_widget)
{
    set_int ("MAX_LOST_WORK_SECS_ON_BAT", (int) gtk_adjustment_get_value (GTK_ADJUSTMENT (calling_widget)));   
}

void
disk_devices_textbox_changed_cb (GObject *calling_widget)
{
    set_text ("DISK_DEVICES", gtk_editable_get_chars (GTK_EDITABLE (calling_widget), 0, -1));
}

void
apm_ac_textbox_changed_cb (GObject *calling_widget)
{
    set_text ("DISK_APM_LEVEL_ON_AC", gtk_editable_get_chars (GTK_EDITABLE (calling_widget), 0, -1));
}

void
apm_bat_textbox_changed_cb (GObject *calling_widget)
{
    set_text ("DISK_APM_LEVEL_ON_BAT", gtk_editable_get_chars (GTK_EDITABLE (calling_widget), 0, -1));
}

void
spindown_ac_textbox_changed_cb (GObject *calling_widget)
{
    set_text ("DISK_SPINDOWN_TIMEOUT_ON_AC", gtk_editable_get_chars (GTK_EDITABLE (calling_widget), 0, -1));
}

void
spindown_bat_textbox_changed_cb (GObject *calling_widget)
{
    set_text ("DISK_SPINDOWN_TIMEOUT_ON_BAT", gtk_editable_get_chars (GTK_EDITABLE (calling_widget), 0, -1));
}

void
scheduler_textbox_changed_cb (GObject *calling_widget)
{
    set_text ("DISK_IOSCHED", gtk_editable_get_chars (GTK_EDITABLE (calling_widget), 0, -1));
}

void
sata_alp_ac_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("SATA_LINKPWR_ON_AC");
    get_sata_lpwr_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str);
}

void
sata_alp_bat_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("SATA_LINKPWR_ON_BAT");
    get_sata_lpwr_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str);
}

void
pcie_aspm_ac_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("PCIE_ASPM_ON_AC");
    get_pcie_aspm_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str);    
}

void
pcie_aspm_bat_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("PCIE_ASPM_ON_BAT");
    get_pcie_aspm_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str);
}

void
pcie_rpm_ac_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("RUNTIME_PM_ON_AC");
    get_pcie_rpm_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str);    
}

void
pcie_rpm_bat_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("RUNTIME_PM_ON_BAT");
    get_pcie_rpm_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str);    
}

void
selected_devices_rpm_radiobutton_toggled_cb (GObject *calling_widget, gpointer all_devices_radio_button)
{
    TLPValue *value = get_value_by_name ("RUNTIME_PM_ALL");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 0;
    } else {
        value->num = 1;
    }
}

void
all_devices_rpm_radiobutton_toggled_cb (GObject *calling_widget, gpointer selected_devices_radio_button)
{
    TLPValue *value = get_value_by_name ("RUNTIME_PM_ALL");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 1;
    } else {
        value->num = 0;
    }
}

void
exclude_pcie_devices_rpm_textbox_changed_cb (GObject *calling_widget)
{
    set_text ("RUNTIME_PM_BLACKLIST", gtk_editable_get_chars (GTK_EDITABLE (calling_widget), 0, -1));
}

void
radeon_old_clock_ac_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("RADEON_POWER_PROFILE_ON_AC");
    get_radeon_clock_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str);    
}

void
radeon_old_clock_bat_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("RADEON_POWER_PROFILE_ON_BAT");
    get_radeon_clock_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str); 
}

void
radeon_dpm_ac_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("RADEON_DPM_STATE_ON_AC");
    get_radeon_dpm_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str);
}

void
radeon_dpm_bat_combobox_changed_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("RADEON_DPM_STATE_ON_BAT");
    get_radeon_dpm_by_id (gtk_combo_box_get_active (GTK_COMBO_BOX (calling_widget)), &value->str);
}

void
wifi_powersave_ac_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("WIFI_PWR_ON_AC");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 5;
    } else {
        value->num = 1;
    }
}

void
wifi_powersave_bat_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("WIFI_PWR_ON_BAT");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 5;
    } else {
        value->num = 1;
    }
}

void
wol_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("WOL_DISABLE");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->str = realloc (value->str, 2 * sizeof (char));
        value->str[0] = 'N';
        value->str[1] = '\0';
    } else {
        value->str = realloc (value->str, 2 * sizeof (char));
        value->str[0] = 'Y';
        value->str[1] = '\0';
    }
}

void
disable_on_lan_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_LAN_CONNECT",
                "wifi"
                );
}

void
disable_on_lan_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_LAN_CONNECT",
                "wwan"
                );
}

void
disable_on_lan_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_LAN_CONNECT",
                "bluetooth"
                );
}

void
disable_on_wifi_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_WIFI_CONNECT",
                "wifi"
                );
}

void
disable_on_wifi_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_WIFI_CONNECT",
                "wwan"
                );
}

void
disable_on_wifi_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_WIFI_CONNECT",
                "bluetooth"
                );
}

void
disable_on_wwan_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_WWAN_CONNECT",
                "wifi"
                );
}

void
disable_on_wwan_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_WWAN_CONNECT",
                "wwan"
                );
}

void
disable_on_wwan_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_WWAN_CONNECT",
                "bluetooth"
                );
}

void
enable_on_lan_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_LAN_DISCONNECT",
                "wifi"
                );
}

void
enable_on_lan_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_LAN_DISCONNECT",
                "wwan"
                );
}

void
enable_on_lan_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_LAN_DISCONNECT",
                "bluetooth"
                );
}

void
enable_on_wifi_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_WIFI_DISCONNECT",
                "wifi"
                );
}

void
enable_on_wifi_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_WIFI_DISCONNECT",
                "wwan"
                );
}

void
enable_on_wifi_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_WIFI_DISCONNECT",
                "bluetooth"
                );
}

void
enable_on_wwan_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_WWAN_DISCONNECT",
                "wifi"
                );
}

void
enable_on_wwan_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_WWAN_DISCONNECT",
                "wwan"
                );
}

void
enable_on_wwan_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_WWAN_DISCONNECT",
                "bluetooth"
                );
}

void
disable_on_dock_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_DOCK",
                "wifi"
                );
}

void
disable_on_dock_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_DOCK",
                "wwan"
                );
}

void
disable_on_dock_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_DOCK",
                "bluetooth"
                );
}

void
enable_on_dock_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_DOCK",
                "wifi"
                );
}

void
enable_on_dock_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_DOCK",
                "wwan"
                );
}

void
enable_on_dock_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_DOCK",
                "bluetooth"
                );
}

void
disable_on_undock_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_UNDOCK",
                "wifi"
                );
}

void
disable_on_undock_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_UNDOCK",
                "wwan"
                );
}

void
disable_on_undock_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_UNDOCK",
                "bluetooth"
                );
}

void
enable_on_undock_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_UNDOCK",
                "wifi"
                );
}

void
enable_on_undock_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_UNDOCK",
                "wwan"
                );
}

void
enable_on_undock_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_UNDOCK",
                "bluetooth"
                );
}

void
restore_radio_state_on_startup_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("RESTORE_DEVICE_STATE_ON_STARTUP");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 1;
    } else {
        value->num = 0;
    }
}

void
disable_on_startup_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_STARTUP",
                "wifi"
                );
}

void
disable_on_startup_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_STARTUP",
                "wwan"
                );
}

void
disable_on_startup_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_STARTUP",
                "bluetooth"
                );
}

void
enable_on_startup_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_STARTUP",
                "wifi"
                );
}

void
enable_on_startup_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_STARTUP",
                "wwan"
                );
}

void
enable_on_startup_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_STARTUP",
                "bluetooth"
                );
}

void
disable_on_shutdown_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_SHUTDOWN",
                "wifi"
                );
}

void
disable_on_shutdown_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_SHUTDOWN",
                "wwan"
                );
}

void
disable_on_shutdown_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_DISABLE_ON_SHUTDOWN",
                "bluetooth"
                );
}

void
enable_on_shutdown_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_SHUTDOWN",
                "wifi"
                );
}

void
enable_on_shutdown_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_SHUTDOWN",
                "wwan"
                );
}

void
enable_on_shutdown_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_SHUTDOWN",
                "bluetooth"
                );
}

void
enable_on_wrs_wifi_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_RADIOSW",
                "wifi"
                );
}

void
enable_on_wrs_wwan_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_RADIOSW",
                "wwan"
                );
}

void
enable_on_wrs_bt_checkbutton_toggled_cb (GObject *calling_widget)
{
    toggle_device (
                gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)),
                "DEVICES_TO_ENABLE_ON_RADIOSW",
                "bluetooth"
                );
}

void
usb_autosuspend_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("USB_AUTOSUSPEND");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 1;
    } else {
        value->num = 0;
    }
}

void
exclude_usb_devices_autosuspend_textbox_changed_cb (GObject *calling_widget)
{
    set_text ("USB_BLACKLIST", gtk_editable_get_chars (GTK_EDITABLE (calling_widget), 0, -1));
}

void
exclude_builtin_wwan_autosuspend_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("USB_BLACKLIST_WWAN");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 1;
    } else {
        value->num = 0;
    }
}

void
disable_usb_autosuspend_upon_shutdown_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("USB_AUTOSUSPEND_DISABLE_ON_SHUTDOWN");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 1;
    } else {
        value->num = 0;
    }
}

void
timeout_audio_powersave_adjustment_value_changed_cb (GObject *calling_widget)
{
    set_int ("SOUND_POWER_SAVE", (int) gtk_adjustment_get_value (GTK_ADJUSTMENT (calling_widget)));
}

void
poweroff_controller_soundchip_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("SOUND_POWER_SAVE_CONTROLLER");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->str = realloc (value->str, 2 * sizeof (char));
        value->str[0] = 'Y';
        value->str[1] = '\0';
    } else {
        value->str = realloc (value->str, 2 * sizeof (char));
        value->str[0] = 'N';
        value->str[1] = '\0';
    }
}

void
poweroff_cd_bat_checkbutton_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("BAY_POWEROFF_ON_BAT");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 1;
    } else {
        value->num = 0;
    }
}

void
cd_device_textbox_changed_cb (GObject *calling_widget)
{
    set_text ("BAY_DEVICE", gtk_editable_get_chars (GTK_EDITABLE (calling_widget), 0, -1));
}

void
uv_textbox_changed_cb (GObject *calling_widget)
{
    set_text ("PHC_CONTROLS", gtk_editable_get_chars (GTK_EDITABLE (calling_widget), 0, -1));
}

void
disable_tpacpi_bat_checkbox_toggled_cb (GObject *calling_widget)
{
    TLPValue *value = get_value_by_name ("DISABLE_TPACPIBAT");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->num = 1;
    } else {
        value->num = 0;
    }
}

void
trace_mode_checkbutton_toggled_cb (GObject *calling_widget)
{
    int l = strlen ("lock nm path pm rf run sysfs udev usb") + 1;
    TLPValue *value = get_value_by_name ("TLP_DEBUG");
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (calling_widget)) == TRUE)
    {
        value->str = realloc (value->str, l * sizeof (char));
        snprintf (value->str, l, "lock nm path pm rf run sysfs udev usb");
    } else {
        free (value->str);
        value->str = NULL;
    }
}

void
upper_thresh_aux_adjustment_value_changed_cb (GObject *calling_widget)
{
    set_int ("STOP_CHARGE_THRESH_BAT1", (int) gtk_adjustment_get_value (GTK_ADJUSTMENT (calling_widget)));
}

void
upper_thresh_main_adjustment_value_changed_cb (GObject *calling_widget)
{
    set_int ("STOP_CHARGE_THRESH_BAT0", (int) gtk_adjustment_get_value (GTK_ADJUSTMENT (calling_widget)));
}

void
lower_thresh_aux_adjustment_value_changed_cb (GObject *calling_widget)
{
    set_int ("START_CHARGE_THRESH_BAT1", (int) gtk_adjustment_get_value (GTK_ADJUSTMENT (calling_widget)));
}

void
lower_thresh_main_adjustment_value_changed_cb (GObject *calling_widget)
{
    set_int ("START_CHARGE_THRESH_BAT0", (int) gtk_adjustment_get_value (GTK_ADJUSTMENT (calling_widget)));
}
