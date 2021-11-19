#include <fcgi_stdio.h>
#include <cmark.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void print_error(const char *error, ...) {
    char fmt[512];
    char buffer[128];
    va_list args;
    va_start(args, error);
    vsprintf(buffer, error, args);
    va_end(args);
    sprintf(fmt, "<html lang=\"en\">"
                 "<head>"
                 "<meta charset=\"utf-8\">"
                 "<title>Error</title>"
                 "</head>"
                 "<body>"
                 "<p>The server encountered the following error(s) rendering your document:<br />"
                 "%s"
                 "</p>"
                 "</body>",
                 buffer);
    printf("Status: 500 Internal Server Error\r\n"
           "Content-Type: text/html\r\n"
           "Content-Length: %d\r\n"
           "\r\n"
           "%s",
           strlen(fmt) * sizeof(char),
           fmt);
}

int main() {
    while (FCGI_Accept() >= 0) {
        const char *doc_root = getenv("DOCUMENT_ROOT");
        if (doc_root == NULL) {
            print_error("%s is not set. Check if you included fastcgi_params in your web server configuration.",
                        "DOCUMENT_ROOT");
            goto end;
        }
        const char *doc_uri = getenv("DOCUMENT_URI");
        if (doc_uri == NULL) {
            print_error("%s is not set. Check if you included fastcgi_params in your web server configuration.",
                        "DOCUMENT_URI");
            goto end;
        }
        char *uri = calloc(strlen(doc_root) + 1 /* separator */ + strlen(doc_uri) + 1 /* \0 */,
                           sizeof(char));
        if (uri == NULL) {
            int r = errno;
            print_error("Cannot allocate memory: %s\n", strerror(r));
            goto end;
        }
        sprintf(uri, "%s/%s", doc_root, doc_uri);
        FILE *file = fopen(uri, "r");
        if (file == NULL) {
            int r = errno;
            if (r == ENOENT) {
                printf("Status: 404 Not Found\r\n"
                       "Content-Type: text/plain\r\n"
                       "Content-Length: 0\r\n"
                       "\r\n");
                free(uri);
                goto end;
            }
            print_error("Cannot open file %s: %s\n", uri, strerror(r));
            free(uri);
            goto end;
        }
        free(uri);
        cmark_parser *parser = cmark_parser_new(CMARK_OPT_DEFAULT);
        ssize_t bytes;
        char buffer[1024];
        while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            cmark_parser_feed(parser, buffer, bytes);
            if (bytes < sizeof(buffer)) {
                break;
            }
        }
        cmark_node *document = cmark_parser_finish(parser);
        cmark_parser_free(parser);
        char *html = cmark_render_html(document, CMARK_OPT_DEFAULT);
        cmark_node_free(document);
        printf("Content-Type: text/html\r\n"
               "Content-Length: %d\r\n"
               "\r\n%s",
               strlen(html) * sizeof(char),
               html);
        free(html);
        fclose(file);
        goto end;
        end:
        continue;
    }
    return 0;
}
