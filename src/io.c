#include "tlp-config.h"

int
bufgetc (File *f)
{
    int r;
    if (!f) {
        return EOF;
    }
    
    if (!feof(f->file)) {
        if (f->read == 0 || f->read >= f->act_size) {
            if ( (r = fread(f->buf, sizeof(char), f->size, f->file)) <= 0 ) {
                return EOF;
            } else {
                f->act_size = r;
                f->read = 1;
                return (unsigned char) f->buf[0];
            }
        }
    }
    
    if (f->read >= f->act_size) {
        return EOF;
    }
    
    return (unsigned char) f->buf[f->read++];
}

void
init_file (FILE *f, File *file)
{
    file->file = f;
    memset (file->buf, BUF_SIZE, 0);
    file->act_size = file->size = BUF_SIZE;
    file->read = 0;
}

char *read_line (File *file)
{
    char *line = NULL;
    int c, cnt, lng;
    lng = cnt = 0;
    
    while ( (c = bufgetc(file)) != EOF ) {
                
        if (lng <= cnt) {
            line = realloc (line, lng += BUF_SIZE);
        }
        
        line[cnt] = c;
        ++cnt;
        
        if (c == '\n') {
            break;
        }
    }
    if (line) {
        line = realloc (line, cnt + 1);
        line[cnt] = 0;
    }
    return line;
}

char *read_line_wo_comments (File *file)
{
    char *line = NULL;
    int c, cnt, lng, to_end;
    
    to_end = lng = cnt = 0;
    
    while ( (c = bufgetc(file)) != EOF ) {
        
        if (c == '\n') {
            if (to_end) {
                to_end = 0;
                continue;
            }
            if (!lng) {
                continue;
            } else {
                break;
            }
        } else if (to_end) {
            continue;
        }
        
        if (c == '#') {
            if (!lng) {
                to_end = 1;
                continue;
            } else {
                break;
            }
        }
                
        if (lng <= cnt) {
            line = realloc (line, lng += BUF_SIZE);
        }
        line[cnt] = c;
        ++cnt;
    }
    if (line) {
        line = realloc (line, cnt + 1);
        line[cnt] = 0;
    }
    return line;
}
