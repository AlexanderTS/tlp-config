/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */


#include <sys/types.h>
#include <sys/wait.h>

#include "tlp-config.h"

TLPSettings tlp_settings;
GtkWidget *main_window;
GtkWidget *warning_window;

static FILE *settings_out;
static char *settings_out_path;
static GtkBuilder *builder;


void
show_warning (char *message)
{
     gtk_label_set_text (GTK_LABEL (gtk_builder_get_object (builder, "warning_message")), message);
     gtk_widget_show (warning_window);
}

char *
safe_strstr (char *haystack, char *needle)
{
    if (!haystack || !needle) {
        return NULL;
    }
    
    return strstr (haystack, needle);
}

int
safe_strcmp (char *str1, char *str2)
{
    if (!str1) {
        return 1;
    }
    
    if (!str2) {
        return -1;
    }
    
    return strcmp (str1, str2);
}

TLPValue *
get_value_by_name (char *name)
{
    int i;
    for (i = 0; i < VAL_NUM; ++i) {
        if (!safe_strcmp (tlp_settings.values[i].name, name)) {
            return tlp_settings.values + i;
        }
    }
    
    return NULL;
}

int
get_governor_id (char *value)
{
    if (!value) {
        return 4;
    }
    
    if ( !safe_strcmp(value, "ondemand") ) {
        return 0;
    }
    
    if ( !safe_strcmp(value, "conservative") ) {
        return 1;
    }
    
    if ( !safe_strcmp(value, "powersave") ) {
        return 2;
    }
    
    if ( !safe_strcmp(value, "performance") ) {
        return 3;
    }
    
    return 4;
}

int
get_sata_lpwr_id (char *value)
{
    if (!value) {
        return 3;
    }
    
    if ( !safe_strcmp(value, "min_power") ) {
        return 0;
    }
    
    if ( !safe_strcmp(value, "medium_power") ) {
        return 1;
    }
    
    if ( !safe_strcmp(value, "max_performance") ) {
        return 2;
    }
    
    return 3;
}

int
get_pcie_aspm_id (char *value)
{
    if (!value) {
        return 0;
    }
    
    if ( !safe_strcmp(value, "default") ) {
        return 0;
    }
    
    if ( !safe_strcmp(value, "performance") ) {
        return 1;
    }
    
    if ( !safe_strcmp(value, "powersave") ) {
        return 2;
    }
    
    return 0;
}

int
get_pcie_rpm_id (char *value)
{
    if (!value) {
        return 0;
    }
    
    if ( !safe_strcmp(value, "auto") ) {
        return 0;
    }
    
    if ( !safe_strcmp(value, "on") ) {
        return 1;
    }
    
    return 0;
}

int
get_radeon_clock_id (char *value)
{
    if (!value) {
        return 4;
    }
    
    if ( !safe_strcmp(value, "low") ) {
        return 0;
    }
    
    if ( !safe_strcmp(value, "mid") ) {
        return 1;
    }
    
    if ( !safe_strcmp(value, "high") ) {
        return 2;
    }
    
    if ( !safe_strcmp(value, "auto") ) {
        return 3;
    }
    
    return 4;
}

int
get_radeon_dpm_id (char *value)
{
    if (!value) {
        return 0;
    }
    
    if ( !safe_strcmp(value, "auto") ) {
        return 0;
    }
    
    if ( !safe_strcmp(value, "low") ) {
        return 1;
    }
    
    if ( !safe_strcmp(value, "high") ) {
        return 2;
    }
    
    return 0;
}

void
init_tlp_settings (void)
{
    int i = 0;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "TLP_ENABLE");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DISK_IDLE_SECS_ON_AC");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DISK_IDLE_SECS_ON_BAT");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "MAX_LOST_WORK_SECS_ON_AC");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "MAX_LOST_WORK_SECS_ON_BAT");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "CPU_SCALING_MIN_FREQ_ON_AC");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "CPU_SCALING_MAX_FREQ_ON_AC");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "CPU_SCALING_MIN_FREQ_ON_BAT");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "CPU_SCALING_MAX_FREQ_ON_BAT");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "CPU_BOOST_ON_AC");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "CPU_BOOST_ON_BAT");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "SCHED_POWERSAVE_ON_AC");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "SCHED_POWERSAVE_ON_BAT");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "NMI_WATCHDOG");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "WIFI_PWR_ON_AC");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "WIFI_PWR_ON_BAT");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "SOUND_POWER_SAVE");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "BAY_POWEROFF_ON_BAT");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "RUNTIME_PM_ALL");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "USB_AUTOSUSPEND");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "USB_BLACKLIST_WWAN");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "USB_AUTOSUSPEND_DISABLE_ON_SHUTDOWN");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "RESTORE_DEVICE_STATE_ON_STARTUP");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "START_CHARGE_THRESH_BAT0");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "STOP_CHARGE_THRESH_BAT0");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "START_CHARGE_THRESH_BAT1");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "STOP_CHARGE_THRESH_BAT1");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DISABLE_TPACPIBAT");
    tlp_settings.values[i].is_num = 1;
    tlp_settings.values[i++].num = -1;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DISK_APM_LEVEL_ON_AC");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DISK_APM_LEVEL_ON_BAT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DISK_SPINDOWN_TIMEOUT_ON_AC");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DISK_SPINDOWN_TIMEOUT_ON_BAT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "CPU_SCALING_GOVERNOR_ON_AC");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "CPU_SCALING_GOVERNOR_ON_BAT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "PHC_CONTROLS");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DISK_DEVICES");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DISK_IOSCHED");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "SATA_LINKPWR_ON_AC");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "SATA_LINKPWR_ON_BAT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "PCIE_ASPM_ON_AC");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "PCIE_ASPM_ON_BAT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "RADEON_POWER_PROFILE_ON_AC");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "RADEON_POWER_PROFILE_ON_BAT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "RADEON_DPM_STATE_ON_AC");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "RADEON_DPM_STATE_ON_BAT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "RADEON_DPM_PERF_LEVEL_ON_AC");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "RADEON_DPM_PERF_LEVEL_ON_BAT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "WOL_DISABLE");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "SOUND_POWER_SAVE_CONTROLLER");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "BAY_DEVICE");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "RUNTIME_PM_ON_AC");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "RUNTIME_PM_ON_BAT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "RUNTIME_PM_BLACKLIST");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "USB_BLACKLIST");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_DISABLE_ON_STARTUP");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_ENABLE_ON_STARTUP");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_DISABLE_ON_SHUTDOWN");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_ENABLE_ON_SHUTDOWN");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_ENABLE_ON_RADIOSW");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_DISABLE_ON_LAN_CONNECT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_DISABLE_ON_WIFI_CONNECT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_DISABLE_ON_WWAN_CONNECT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_ENABLE_ON_LAN_DISCONNECT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_ENABLE_ON_WIFI_DISCONNECT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_ENABLE_ON_WWAN_DISCONNECT");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_ENABLE_ON_DOCK");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_DISABLE_ON_DOCK");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_ENABLE_ON_UNDOCK");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "DEVICES_TO_DISABLE_ON_UNDOCK");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
    
    snprintf(tlp_settings.values[i].name, NAME_SIZE, "%s", "TLP_DEBUG");
    tlp_settings.values[i].is_num = 0;
    tlp_settings.values[i++].str = NULL;
}

void
free_tlp_settings_strs (void)
{
    int i;
    for (i = 0; i < VAL_NUM; ++i) {
        if (!tlp_settings.values[i].is_num) {
            free (tlp_settings.values[i].str);
            tlp_settings.values[i].str = NULL;
        }
    }
}

int
get_var (char *line)
{
    int i;
    char *name;
    
    if (strchr (line, '#')) {
        return -1;
    } else {
        for (i = 0; i < VAL_NUM; ++i) {
            if ( (name = strstr (line, tlp_settings.values[i].name)) ) {
                if ( name[strlen (tlp_settings.values[i].name)] == '=' ) {
                    return i;
                }
            }
        }
        return -1;
    }
}

/* Commented variables should only be in format #VARIABLE=VALUE */
int
get_commented_var (char *line)
{
    int i;
    char *name;
    
    if (line[0] == '#') {
        for (i = 0; i < VAL_NUM; ++i) {
            if ( (name = strstr (line, tlp_settings.values[i].name)) ) {
                if (name = line + 1) {
                    if ( name[strlen (tlp_settings.values[i].name)] == '=' ) {
                        return i;
                    }
                }
            }
        }
    }
    return -1;
}

int
check_and_set_value (char *line)
{
    int i, l;
    char *substring;
    
    for (i = 0; i < VAL_NUM; ++i) {
        substring = safe_strstr (line, tlp_settings.values[i].name);
        l = strlen (tlp_settings.values[i].name);
        
        if (substring && substring[l] == '=') {
            substring += l + 1;
            
            if (tlp_settings.values[i].is_num) {
                
                tlp_settings.values[i].num = atoi (substring);
                return 1;
                
            } else {
                
                free (tlp_settings.values[i].str);
                l = strlen(substring) + 1;
                
                if (*substring == '"') {
                    l -= 2;
                    ++substring;
                }
                
                tlp_settings.values[i].str = calloc (l, sizeof (char));
                snprintf(tlp_settings.values[i].str, l, "%s", substring);
                return 1;
                
            }
        }
    }
    
    return 0;
}

int
read_cpu_freqs (int **freqs)
{
    FILE *f = fopen ("/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies", "r");
    if (!f) {
        return 0;
    }
    
    int cnt, lng;
    cnt = 0;
    lng = 8;
    *freqs = calloc (lng, sizeof (int));
    
    while (fscanf (f, "%d", *freqs + cnt) > 0 ) {
        ++cnt;
        if (cnt >= lng) {
            *freqs = realloc (*freqs, (lng += 8) * sizeof (int));
        }
    }
    *freqs = realloc (*freqs, cnt * sizeof (int));
    fclose (f);
    
    return cnt;
}

void
add_i_to_list (GtkListStore *store, const int value)
{
  GtkTreeIter iter;

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, (gint) value, -1);
}

int
get_iter_of_i (GtkListStore *store, int what, GtkTreeIter *iter)
{
    int v;
 
    if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (store), iter) == TRUE) {
        do {
            gtk_tree_model_get (GTK_TREE_MODEL (store), iter, 0, &v, -1);
            if (v == what) {
                return 1;
            }
        } while (gtk_tree_model_iter_next (GTK_TREE_MODEL (store), iter) == TRUE);
    }
    
    return 0;
}

void
print_tlp_settings (FILE *output)
{
    int i;
    for (i = 0; i < VAL_NUM; ++i) {
        if (tlp_settings.values[i].is_num) {
            
            if (tlp_settings.values[i].num != -1) {
                fprintf (output, "%s=%d\n", tlp_settings.values[i].name, tlp_settings.values[i].num);
            }
            
        } else {
            
            if (tlp_settings.values[i].str != NULL) {
                fprintf (output, "%s=%s\n", tlp_settings.values[i].name, tlp_settings.values[i].str);
            }
            
        }
    }
}

void
read_tlp_settings (void)
{
    FILE *set_file = fopen("/etc/default/tlp", "r");
    File settings_in;
    init_file(set_file, &settings_in);
    char *line;
    
    free_tlp_settings_strs ();
    init_tlp_settings ();
    
    while ( (line = read_line_wo_comments(&settings_in)) != NULL ) {
        check_and_set_value (line);
        free (line);
    }
    
    fclose (set_file);
}

void
set_ui (void)
{
    GtkToggleButton *togglebutton;
    GtkComboBox *combobox;
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_tlp_checkbutton"));
    if (get_value_by_name ("TLP_ENABLE")->num > 0) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "nmi_watchdog_button"));
    if (get_value_by_name ("NMI_WATCHDOG")->num > 0) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_turbo_ac_checkbutton"));
    if (get_value_by_name ("CPU_BOOST_ON_AC")->num != 0) {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    } else {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_turbo_bat_checkbutton"));
    if (get_value_by_name ("CPU_BOOST_ON_BAT")->num != 0) {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    } else {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "min_cores_ac_checkbutton"));
    if (get_value_by_name ("SCHED_POWERSAVE_ON_AC")->num > 0) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "min_cores_bat_checkbutton"));
    if (get_value_by_name ("SCHED_POWERSAVE_ON_BAT")->num > 0) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "laptop_mode_ac_checkbutton"));
    if (get_value_by_name ("DISK_IDLE_SECS_ON_AC")->num > 0) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "laptop_mode_bat_checkbutton"));
    if (get_value_by_name ("DISK_IDLE_SECS_ON_BAT")->num > 0) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "wifi_powersave_ac_checkbutton"));
    if (get_value_by_name ("WIFI_PWR_ON_AC")->num == 5) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "wifi_powersave_bat_checkbutton"));
    if (get_value_by_name ("WIFI_PWR_ON_BAT")->num == 5) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "wol_checkbutton"));
    if ( !safe_strcmp (get_value_by_name ("WOL_DISABLE")->str, "Y") ) {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    } else {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_lan_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_LAN_CONNECT")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_lan_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_LAN_CONNECT")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_lan_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_LAN_CONNECT")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_wifi_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_WIFI_CONNECT")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_wifi_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_WIFI_CONNECT")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_wifi_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_WIFI_CONNECT")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_wwan_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_WWAN_CONNECT")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_wwan_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_WWAN_CONNECT")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_wwan_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_WWAN_CONNECT")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_lan_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_LAN_DISCONNECT")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_lan_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_LAN_DISCONNECT")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_lan_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_LAN_DISCONNECT")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_wifi_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_WIFI_DISCONNECT")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_wifi_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_WIFI_DISCONNECT")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_wifi_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_WIFI_DISCONNECT")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_wwan_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_WWAN_DISCONNECT")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_wwan_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_WWAN_DISCONNECT")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_wwan_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_WWAN_DISCONNECT")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_dock_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_DOCK")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_dock_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_DOCK")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_dock_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_DOCK")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_dock_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_DOCK")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_dock_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_DOCK")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_dock_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_DOCK")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_undock_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_UNDOCK")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_undock_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_UNDOCK")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_undock_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_UNDOCK")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_undock_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_UNDOCK")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_undock_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_UNDOCK")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_undock_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_UNDOCK")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "restore_radio_state_on_startup_checkbutton"));
    if (get_value_by_name ("RESTORE_DEVICE_STATE_ON_STARTUP")->num > 0) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_startup_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_STARTUP")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_startup_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_STARTUP")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_startup_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_STARTUP")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_startup_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_STARTUP")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_startup_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_STARTUP")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_startup_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_STARTUP")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_shutdown_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_SHUTDOWN")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_shutdown_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_SHUTDOWN")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_on_shutdown_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_DISABLE_ON_SHUTDOWN")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_shutdown_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_SHUTDOWN")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_shutdown_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_SHUTDOWN")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_shutdown_bt_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_SHUTDOWN")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_wrs_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_RADIOSW")->str, "wifi")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_wrs_wwan_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_RADIOSW")->str, "wwan")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "enable_on_wrs_wifi_checkbutton"));
    if (safe_strstr (get_value_by_name ("DEVICES_TO_ENABLE_ON_RADIOSW")->str, "bluetooth")) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "usb_autosuspend_checkbutton"));
    if (get_value_by_name ("USB_AUTOSUSPEND")->num > 0) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "exclude_builtin_wwan_autosuspend_checkbutton"));
    if (get_value_by_name ("USB_BLACKLIST_WWAN")->num > 0) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_usb_autosuspend_upon_shutdown_checkbutton"));
    if (get_value_by_name ("USB_AUTOSUSPEND_DISABLE_ON_SHUTDOWN")->num > 0) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "poweroff_controller_soundchip_checkbutton"));
    if ( !safe_strstr (get_value_by_name ("SOUND_POWER_SAVE_CONTROLLER")->str, "Y") ) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "poweroff_cd_bat_checkbutton"));
    if (get_value_by_name ("BAY_POWEROFF_ON_BAT")->num > 0) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "selected_devices_rpm_radiobutton"));
    if (get_value_by_name ("RUNTIME_PM_ALL")->num > 0 ) {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    } else {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "all_devices_rpm_radiobutton"));
    if (get_value_by_name ("RUNTIME_PM_ALL")->num > 0 ) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "disable_tpacpi_bat_checkbox"));
    if (get_value_by_name ("DISABLE_TPACPIBAT")->num > 0 ) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    togglebutton = GTK_TOGGLE_BUTTON (gtk_builder_get_object (builder, "trace_mode_checkbutton"));
    if (get_value_by_name ("TLP_DEBUG")->num > 0 ) {
        gtk_toggle_button_set_active (togglebutton, TRUE);
    } else {
        gtk_toggle_button_set_active (togglebutton, FALSE);
    }
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "governor_ac_combobox"));
    gtk_combo_box_set_active (combobox, get_governor_id (get_value_by_name ("CPU_SCALING_GOVERNOR_ON_AC")->str));
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "governor_bat_combobox"));
    gtk_combo_box_set_active (combobox, get_governor_id (get_value_by_name ("CPU_SCALING_GOVERNOR_ON_BAT")->str));
    
    GtkTreeIter iter;
    GtkListStore *liststore;
    int *cpufreqs;
    int i;
    int num = read_cpu_freqs (&cpufreqs);
    
    if (num == 0) {
        show_warning ("WARNING: file sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies was not found or is corrupt. CPU frequncies could not be set.\nIf you are running Linux 3.10 or higher on Intel processor this warning can be caused by Intel P-State driver. With P-State you probably could not set ondemand/conservative governor either.\nTo disable Intel P-State driver add to the end of \"linux\" line in your grub.cfg:\nintel_pstate=disable");
        fprintf (stderr, "\nWARNING: file sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies was not found or is corrupt. CPU frequncies could not be set.\nIf you are running Linux 3.10 or higher on Intel processor this warning can be caused by Intel P-State driver. With P-State you probably could not set ondemand/conservative governor either.\nTo disable Intel P-State driver add to the end of \"linux\" line in your grub.cfg:\nintel_pstate=disable\n\n");
    } else {
        liststore = gtk_list_store_new (1, G_TYPE_INT);
        for (i = 0; i < num; ++i) {
            add_i_to_list (liststore, cpufreqs[i]);
        }
        add_i_to_list (liststore, -1);
        
        combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "min_freq_ac_combobox"));
        gtk_combo_box_set_model (combobox, GTK_TREE_MODEL (liststore));
        if (!get_iter_of_i (liststore, get_value_by_name ("CPU_SCALING_MIN_FREQ_ON_AC")->num, &iter)) {
            get_iter_of_i (liststore, -1, &iter);
            gtk_combo_box_set_active_iter (combobox, &iter);
        }
        gtk_combo_box_set_active_iter (combobox, &iter);
        
        combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "min_freq_bat_combobox"));
        gtk_combo_box_set_model (combobox, GTK_TREE_MODEL (liststore));
        if (!get_iter_of_i (liststore, get_value_by_name ("CPU_SCALING_MIN_FREQ_ON_BAT")->num, &iter)) {
            get_iter_of_i (liststore, -1, &iter);
            gtk_combo_box_set_active_iter (combobox, &iter);
        }
        gtk_combo_box_set_active_iter (combobox, &iter);
        
        combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "max_freq_ac_combobox"));
        gtk_combo_box_set_model (combobox, GTK_TREE_MODEL (liststore));
        if (!get_iter_of_i (liststore, get_value_by_name ("CPU_SCALING_MAX_FREQ_ON_AC")->num, &iter)) {
            get_iter_of_i (liststore, -1, &iter);
            gtk_combo_box_set_active_iter (combobox, &iter);
        }
        gtk_combo_box_set_active_iter (combobox, &iter);
        
        combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "max_freq_bat_combobox"));
        gtk_combo_box_set_model (combobox, GTK_TREE_MODEL (liststore));
        if (!get_iter_of_i (liststore, get_value_by_name ("CPU_SCALING_MAX_FREQ_ON_BAT")->num, &iter)) {
            get_iter_of_i (liststore, -1, &iter);
            gtk_combo_box_set_active_iter (combobox, &iter);
        }
        gtk_combo_box_set_active_iter (combobox, &iter);
    }
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "sata_alp_ac_combobox"));
    gtk_combo_box_set_active (combobox, get_sata_lpwr_id (get_value_by_name ("SATA_LINKPWR_ON_AC")->str));
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "sata_alp_bat_combobox"));
    gtk_combo_box_set_active (combobox, get_sata_lpwr_id (get_value_by_name ("SATA_LINKPWR_ON_BAT")->str));
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "pcie_aspm_ac_combobox"));
    gtk_combo_box_set_active (combobox, get_pcie_aspm_id (get_value_by_name ("PCIE_ASPM_ON_AC")->str));
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "pcie_aspm_bat_combobox"));
    gtk_combo_box_set_active (combobox, get_pcie_aspm_id (get_value_by_name ("PCIE_ASPM_ON_BAT")->str));
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "pcie_rpm_ac_combobox"));
    gtk_combo_box_set_active (combobox, get_pcie_rpm_id (get_value_by_name ("RUNTIME_PM_ON_AC")->str));
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "pcie_rpm_bat_combobox"));
    gtk_combo_box_set_active (combobox, get_pcie_rpm_id (get_value_by_name ("RUNTIME_PM_ON_BAT")->str));
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "radeon_old_clock_ac_combobox"));
    gtk_combo_box_set_active (combobox, get_radeon_clock_id (get_value_by_name ("RADEON_POWER_PROFILE_ON_AC")->str));
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "radeon_old_clock_bat_combobox"));
    gtk_combo_box_set_active (combobox, get_radeon_clock_id (get_value_by_name ("RADEON_POWER_PROFILE_ON_BAT")->str));
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "radeon_dpm_ac_combobox"));
    gtk_combo_box_set_active (combobox, get_radeon_dpm_id (get_value_by_name ("RADEON_DPM_STATE_ON_AC")->str));
    
    combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "radeon_dpm_bat_combobox"));
    gtk_combo_box_set_active (combobox, get_radeon_dpm_id (get_value_by_name ("RADEON_DPM_STATE_ON_BAT")->str));
    
    GtkSpinButton *spinbutton;
    spinbutton = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "timeout_unsaved_data_writing_ac_spinbutton"));
    gtk_spin_button_set_value (spinbutton, get_value_by_name ("MAX_LOST_WORK_SECS_ON_AC")->num);
    
    spinbutton = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "timeout_unsaved_data_writing_bat_spinbutton"));
    gtk_spin_button_set_value (spinbutton, get_value_by_name ("MAX_LOST_WORK_SECS_ON_BAT")->num);
    
    spinbutton = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "timeout_audio_powersave_spinbutton"));
    gtk_spin_button_set_value (spinbutton, get_value_by_name ("SOUND_POWER_SAVE")->num);
    
    spinbutton = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "lower_thresh_main_spinbutton"));
    gtk_spin_button_set_value (spinbutton, get_value_by_name ("START_CHARGE_THRESH_BAT0")->num);
    
    spinbutton = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "lower_thresh_aux_spinbutton"));
    gtk_spin_button_set_value (spinbutton, get_value_by_name ("START_CHARGE_THRESH_BAT1")->num);
    
    spinbutton = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "upper_thresh_main_spinbutton"));
    gtk_spin_button_set_value (spinbutton, get_value_by_name ("STOP_CHARGE_THRESH_BAT0")->num);
    
    spinbutton = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "upper_thresh_aux_spinbutton"));
    gtk_spin_button_set_value (spinbutton, get_value_by_name ("STOP_CHARGE_THRESH_BAT1")->num);
    
    GtkEntry *textbox;
    textbox = GTK_ENTRY (gtk_builder_get_object (builder, "disk_devices_textbox"));
    gtk_entry_set_text (textbox, get_value_by_name ("DISK_DEVICES")->str);
    
    textbox = GTK_ENTRY (gtk_builder_get_object (builder, "spindown_ac_textbox"));
    gtk_entry_set_text (textbox, get_value_by_name ("DISK_SPINDOWN_TIMEOUT_ON_AC")->str);
    
    textbox = GTK_ENTRY (gtk_builder_get_object (builder, "spindown_bat_textbox"));
    gtk_entry_set_text (textbox, get_value_by_name ("DISK_SPINDOWN_TIMEOUT_ON_BAT")->str);
    
    textbox = GTK_ENTRY (gtk_builder_get_object (builder, "apm_ac_textbox"));
    gtk_entry_set_text (textbox, get_value_by_name ("DISK_APM_LEVEL_ON_AC")->str);
    
    textbox = GTK_ENTRY (gtk_builder_get_object (builder, "apm_bat_textbox"));
    gtk_entry_set_text (textbox, get_value_by_name ("DISK_APM_LEVEL_ON_BAT")->str);
    
    textbox = GTK_ENTRY (gtk_builder_get_object (builder, "scheduler_textbox"));
    gtk_entry_set_text (textbox, get_value_by_name ("DISK_IOSCHED")->str);
    
    textbox = GTK_ENTRY (gtk_builder_get_object (builder, "exclude_pcie_devices_rpm_textbox"));
    gtk_entry_set_text (textbox, get_value_by_name ("RUNTIME_PM_BLACKLIST")->str);
    
    textbox = GTK_ENTRY (gtk_builder_get_object (builder, "exclude_usb_devices_autosuspend_textbox"));
    gtk_entry_set_text (textbox, get_value_by_name ("USB_BLACKLIST")->str);
    
    textbox = GTK_ENTRY (gtk_builder_get_object (builder, "cd_device_textbox"));
    gtk_entry_set_text (textbox, get_value_by_name ("BAY_DEVICE")->str);
    
    textbox = GTK_ENTRY (gtk_builder_get_object (builder, "uv_textbox"));
    gtk_entry_set_text (textbox, get_value_by_name ("PHC_CONTROLS")->str);
}

void
restart_tlp (void)
{
    int childExitStatus;
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0) {
        execl("/usr/sbin/tlp", "/usr/sbin/tlp", "start", (char *) NULL);
    }
    else if (pid < 0) {
        fprintf (stderr, "ERROR: Could not restart TLP. Check if TLP is present.\n");
        show_warning ("ERROR: Could not restart TLP. Check if TLP is present.");
    }
    else {
        pid_t ws = waitpid( pid, &childExitStatus, WNOHANG);
        if (ws == -1)
        {
            fprintf (stderr, "ERROR: Could not restart TLP. You should have ROOT privileges to restart it.\n");
            show_warning ("ERROR: Could not restart TLP. You should have ROOT privileges to restart it.");
        }

        if(WIFEXITED(childExitStatus))
        {
            status = WEXITSTATUS(childExitStatus);
            fprintf (stderr, "ERROR: Could not restart TLP. You should have ROOT privileges to restart it.\n");
            show_warning ("ERROR: Could not restart TLP. You should have ROOT privileges to restart it.");
        }
    }
}

void
copy(char *source, char *dest)
{
    int childExitStatus;
    pid_t pid;
    int status;
    
    if (!source || !dest) {
        return;
    }

    pid = fork();

    if (pid == 0) {
        execl("/bin/cp", "/bin/cp", source, dest, (char *) NULL);
    }
    else if (pid < 0) {
        fprintf (stderr, "ERROR: Could not start copy process\n");
        show_warning ("ERROR: Could not start copy process");
    }
    else {
        do {
            pid_t ws = waitpid( pid, &childExitStatus, WNOHANG);
            if (ws == -1)
            {
                fprintf (stderr, "ERROR: Could not start copy process\n");
                show_warning ("ERROR: Could not start copy process");
            }

            if (WIFEXITED(childExitStatus))
            {
                status = WEXITSTATUS(childExitStatus);
                if (status) {
                    fprintf (stderr, "ERROR: Could not copy configuration file, status %d\n", status);
                    show_warning ("ERROR: Could not copy configuration file");
                }
            }
        } while (!WIFEXITED(childExitStatus));
    }
}

void
restore_defaults (void)
{
    copy ("/etc/tlp-config/tlp.default", "/etc/default/tlp");
    read_tlp_settings ();
    set_ui ();
}

void
save_settings (void)
{
    settings_out = fopen (settings_out_path, "w");
    if (!settings_out) {
        fprintf (stderr, "ERROR: Could not open file for writing %s\n", settings_out_path);
        show_warning ("ERROR: Could not open file for writing");
        return;
    }
    int i;
    for (i = 0; i < VAL_NUM; ++i) {
        if (tlp_settings.values[i].is_num) {
            if (tlp_settings.values[i].num != -1) {
                fprintf (settings_out, "%s=%d\n", tlp_settings.values[i].name, tlp_settings.values[i].num);
            }
        } else {
            if (tlp_settings.values[i].str) {
                if (tlp_settings.values[i].str && strlen (tlp_settings.values[i].str)) {
                    if (safe_strstr (tlp_settings.values[i].str, " ")) {
                        fprintf (settings_out, "%s=\"%s\"\n", tlp_settings.values[i].name, tlp_settings.values[i].str);
                    } else {
                        fprintf (settings_out, "%s=%s\n", tlp_settings.values[i].name, tlp_settings.values[i].str);
                    }
                }
            }
        }
    }
    fclose (settings_out);
}

int
print_value (FILE *file, int i)
{
    if (tlp_settings.values[i].is_num) {
        if (tlp_settings.values[i].num != -1) {
            fprintf (file, "%s=%d\n", tlp_settings.values[i].name, tlp_settings.values[i].num);
            return 1;
        }
    } else {
        if (tlp_settings.values[i].str) {
            if (tlp_settings.values[i].str && strlen (tlp_settings.values[i].str)) {
                if (safe_strstr (tlp_settings.values[i].str, " ")) {
                    fprintf (file, "%s=\"%s\"\n", tlp_settings.values[i].name, tlp_settings.values[i].str);
                } else {
                    fprintf (file, "%s=%s\n", tlp_settings.values[i].name, tlp_settings.values[i].str);
                }
                return 1;
            }
        }
    }
    return 0;
}

void
save_settings_carefully (void)
{
    char *tmp_path = calloc (strlen (settings_out_path) + strlen (".new") + 1, sizeof (char));
    snprintf (tmp_path, strlen (settings_out_path) + strlen (".new") + 1, "%s.new", settings_out_path);
    
    FILE *set_file = fopen("/etc/default/tlp", "r");
    if (!settings_out) {
        fprintf (stderr, "ERROR: Could not open file for reading /etc/default/tlp\n");
        show_warning ("ERROR: Could not open file for reading /etc/default/tlp");
        return;
    }
    
    File settings_in;
    init_file(set_file, &settings_in);
    
    settings_out = fopen (tmp_path, "w");
    if (!settings_out) {
        fprintf (stderr, "ERROR: Could not open file for writing %s\n", settings_out_path);
        show_warning ("ERROR: Could not open file for writing");
        return;
    }
    
    int i;
    char first;
    char *line = NULL;
    char flags[VAL_NUM];
    memset (flags, 0, VAL_NUM);
    
    while ( (line = read_line(&settings_in)) ) {
        if ( (i = get_var (line)) >= 0) {
            print_value (settings_out, i);
            flags[i] = 1;
        } else {
            fprintf (settings_out, "%s", line);
        }
        free (line);
    }
    fclose (settings_in.file);
    fclose (settings_out);
    
    set_file = fopen(tmp_path, "r");
    if (!set_file) {
        fprintf (stderr, "ERROR: Could not open file for reading %s\n", tmp_path);
        show_warning ("ERROR: Could not open file for reading");
        return;
    }
    init_file(set_file, &settings_in);
    
    char *tmp_path_final = calloc (strlen (tmp_path) + strlen (".final") + 1, sizeof (char));
    snprintf (tmp_path_final, strlen (tmp_path) + strlen (".final") + 1, "%s.final", tmp_path);
    settings_out = fopen (tmp_path_final, "w");
    
    if (!settings_out) {
        fprintf (stderr, "ERROR: Could not open file for writing %s\n", tmp_path_final);
        show_warning ("ERROR: Could not open file for writing");
        return;
    }
    
    while ( (line = read_line(&settings_in)) ) {
        fprintf (settings_out, "%s", line);
        
        if ( (i = get_commented_var (line)) >= 0) {
            if (!flags[i]) {
                print_value (settings_out, i);
                flags[i] = 1;
            }
        }
        
        free (line);
    }
    
    for (i = 0; i < VAL_NUM; ++i) {
        if (!flags[i]) {
            print_value (settings_out, i);
        }
    }
    
    fclose (settings_in.file);
    fclose (settings_out);
    
    copy (tmp_path_final, settings_out_path);
    if (remove (tmp_path)) {
        fprintf (stderr, "ERROR: Could not remove temporary file %s\n", tmp_path);
        show_warning ("ERROR: Could not remove temporary file");
    }
    
    if (remove (tmp_path_final)) {
        fprintf (stderr, "ERROR: Could not remove temporary file %s\n", tmp_path_final);
        show_warning ("ERROR: Could not remove temporary file");
    }
    
    free (tmp_path);
    free (tmp_path_final);
}

void
create_windows ()
{
    GError* error = NULL;

    builder = gtk_builder_new ();
    if (!gtk_builder_add_from_file (builder, "/etc/tlp-config/main_window.glade", &error))
    {
            g_critical ("Could not load file: %s", error->message);
            g_error_free (error);
    }
    
    if (!gtk_builder_add_from_file (builder, "/etc/tlp-config/warning_window.glade", &error))
    {
            g_critical ("Could not load file: %s", error->message);
            g_error_free (error);
    }

    main_window = GTK_WIDGET (gtk_builder_get_object (builder, "main_window"));
    if (!main_window)
    {
            g_critical ("Error while getting window widget");
    }
    
    warning_window = GTK_WIDGET (gtk_builder_get_object (builder, "warning_window"));
    if (!main_window)
    {
            g_critical ("Error while getting window widget");
    }
    
    set_ui ();
    
    gtk_builder_connect_signals (builder, main_window);
    gtk_builder_connect_signals (builder, warning_window);
}

void
on_close (void)
{
    gtk_widget_destroy (main_window);
    gtk_widget_destroy (warning_window);
    g_object_unref (G_OBJECT (builder));
    free_tlp_settings_strs ();
    free (settings_out_path);
}

int
main (int argc, char *argv[])
{
    settings_out = NULL;
    char error = 1;
    
    if (argc > 1) {
        
        if ( ( !safe_strcmp(argv[1], "-o") || !safe_strcmp(argv[1], "--output") ) && argc > 2) {
            error = 0;
            settings_out = fopen (argv[2], "a");
            if (!settings_out) {
                fprintf (stderr, "ERROR: Could not open file for writing %s\n", argv[2]);
                return 2;
            }
            settings_out_path = calloc (strlen (argv[2]) + 1, sizeof (char));
            snprintf (settings_out_path, strlen (argv[2]) + 1, "%s", argv[2]);
            fclose (settings_out);
        }
        
        if ( !safe_strcmp(argv[1], "-v") || !safe_strcmp(argv[1], "--version") ) {
            error = 0;
            printf ("TLP Advanced Power Manager GTK Configurator v0.9\n");
            return 0;
        }
        
        if (error) {
            printf ("Usage: tlp-config [PARAMETER]\nTLP Advanced Power Manager GTK Configurator\n\nRunning without parameters will save configuration to /etc/default/tlp, be sure to have ROOT privileges.\n\nPossible PARAMETER values:\n  -o, --output <FILE>\tsave configuration to FILE\n  -h, --help\t\tshow this information and exit\n  -v, --version\t\tshow version number and exit\n\nLicensed under GNU GPL v2\nAlexander Tsarkov 2013\nReport bugs on <reports.alex.tsarkov@gmail.com>\n");
            if ( !safe_strcmp(argv[1], "-h") || !safe_strcmp(argv[1], "--help") ) {
                return 0;
            } else {
                return 1;
            }
        }
    }
    
    gtk_init (&argc, &argv);

    
    read_tlp_settings ();
    create_windows ();
    gtk_widget_show (main_window);
    
    if (!settings_out) {
        settings_out = fopen ("/etc/default/tlp", "a");
        if (!settings_out) {
            show_warning ("ERROR: Could not open file for writing: /etc/default/tlp\nBe sure to run with ROOT privileges\n");
            fprintf (stderr, "ERROR: Could not open file for writing: /etc/default/tlp\nBe sure to run with ROOT privileges\n");
            on_close ();
            return 3;
        }
        settings_out_path = calloc (strlen ("/etc/default/tlp") + 1, sizeof (char));
        snprintf (settings_out_path, strlen ("/etc/default/tlp") + 1, "/etc/default/tlp");
        fclose (settings_out);
    }
    gtk_main ();
    
    on_close ();
    return 0;
}
