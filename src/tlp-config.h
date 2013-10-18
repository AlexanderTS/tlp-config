#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#define BUF_SIZE 4096

/* 4 + 1 + 59 = 64 = 2^6 = sizeof (TLPValue) */
#define NAME_SIZE 59
#define VAL_NUM 70

typedef struct TLPValue
{
    union
    {
        int num;
        char *str;
    };
    
    char is_num;
    char name[NAME_SIZE];
} TLPValue;

typedef struct TLPSettings
{
    TLPValue values[VAL_NUM];
} TLPSettings;

typedef struct File
{
    FILE *file;
    int size;
    int read;
    int act_size;
    char buf[BUF_SIZE];
} File;

/* From tlp-config.c */
extern TLPSettings tlp_settings;
extern GtkWidget *main_window;
extern GtkWidget *warning_window;

char *safe_strstr (char *, char *);
int safe_strcmp (char *, char *);
TLPValue *get_value_by_name (char *);
void save_settings (void);
void save_settings_carefully (void);
void restore_defaults (void);
void restart_tlp (void);
void on_close (void);


/* From io.c */
int bufgetc (File *);
void init_file (FILE *, File *);
char *read_line (File *);
char *read_line_wo_comments (File *);
