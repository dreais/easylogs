#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "easylogs.h"

static FILE *output;

static const char format_specifier[7][4] = {
		"c",
		"d",
		"li",
		"lu",
		"lli",
		"llu",
		"s"
};

static void append_buffer(char *buff, int buff_size, char type[4], va_list *arg_list)
{
	char str_type[5] = {'\0'};
	int i = 0;

	str_type[0] = '%';
	strcat(str_type, type);
	if (strcmp(type, format_specifier[i++]) == 0) { // c
		snprintf(buff, buff_size, str_type, va_arg(*arg_list, int));
	} else if (strcmp(type, format_specifier[i++]) == 0) { // d
		snprintf(buff, buff_size, str_type, va_arg(*arg_list, int));
	} else if (strcmp(type, format_specifier[i++]) == 0) { // li
		snprintf(buff, buff_size, str_type, va_arg(*arg_list, long int));
	} else if (strcmp(type, format_specifier[i++]) == 0) { // lu
		snprintf(buff, buff_size, str_type, va_arg(*arg_list, unsigned long));
	} else if (strcmp(type, format_specifier[i++]) == 0) { // lli
		snprintf(buff, buff_size, str_type, va_arg(*arg_list, long long int));
	} else if (strcmp(type, format_specifier[i++]) == 0) { // llu
		snprintf(buff, buff_size, str_type, va_arg(*arg_list, unsigned long long));
	} else if (strcmp(type, format_specifier[i]) == 0) { // s
		snprintf(buff, buff_size, str_type, va_arg(*arg_list, char *));
	}
}

static char *time_str(time_t time)
{
	struct tm *tm = localtime(&time);
	char *buff_return = malloc(sizeof(char) * 100);

	buff_return = memset(buff_return, '\0', sizeof(char) * 100);
	snprintf(buff_return, 100, "[%d-%d-%d-%d:%d:%d]", 1900 + tm->tm_year, tm->tm_mon, tm->tm_mday,
			 tm->tm_hour, tm->tm_min, tm->tm_sec);
	return buff_return;
}

extern void output_logs_str(const char prefix[], const char str[], ...) __attribute__((format(printf,2, 3)));
void output_logs_str(const char prefix[], const char str[], ...)
{
	va_list arg_list;
	unsigned int cursor = 0, backup = 0;
	char type[4] = {'\0'};
	int BUFF_MAX = 1024;
	char *buffer = malloc(sizeof(char) * BUFF_MAX);
	char *tmp_time;

	tmp_time = time_str(time(NULL));
	fprintf(output, "%s", tmp_time);
	free(tmp_time);
	fprintf(output, "%s", prefix);
	va_start(arg_list, str);
	while (str[cursor]) {
		if (str[cursor] == '%') {
			for (int i = 0; i < 7; i++) {
				backup = cursor + 1;
				if (str[backup] == '%')
					break;
				for (int j = 0; j < (int)strlen(format_specifier[i]); j++) {
					if (str[backup] == format_specifier[i][j]) {
						type[j] = str[backup];
						backup++;
					}
				}
				if (strlen(type) == strlen(format_specifier[i])) {
					if (strcmp(type, format_specifier[i]) == 0) {
						cursor = backup - 1;
						break;
					}
				}
			}
			append_buffer(buffer, BUFF_MAX, type, &arg_list);
			for (int i = 0; i < 4; i++)
				type[i] = '\0';
			fprintf(output, "%s", buffer);
		} else {
			fprintf(output, "%c", str[cursor]);
		}
		cursor++;
	}
	va_end(arg_list);
	free(buffer);
}

void close_file(void)
{
	fflush(output);
	fclose(output);
}

void get_log_file(void)
{
	char log_path[] = LOG_NAME;
	char *tmp_time;

	output = fopen(log_path, "w+");
	if (output == NULL) {
		fprintf(stderr, "%s could not be opened.\n", log_path);
		return;
	}
	output_logs_str(PREFIX_INFO, "Opening file to read/write, setting cursor at initial position.\n");
	output_logs_str(PREFIX_INFO, "If you're reading this line, please don't mind it. Just some file opening :)\n");
	output_logs_str(PREFIX_INFO, "Logfile named '%s' opened without issue\n", log_path);
}