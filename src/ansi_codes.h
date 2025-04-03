#pragma once

#define ANSI_BLACK "0m"
#define ANSI_RED "1m"
#define ANSI_GREEN "2m"
#define ANSI_YELLOW "3m"
#define ANSI_BLUE "4m"
#define ANSI_PURPLE "5m"
#define ANSI_CYAN "6m"
#define ANSI_WHITE "7m"

#define ANSI_NORMAL "0;3"
#define ANSI_BOLD "1;3"
#define ANSI_HI_NORMAL "0;9"
#define ANSI_HI_BOLD "1;9"
#define ANSI_UNDERLINE "4;3"

#define ANSI_COLOR(s, type, color) "\e[" type color s "\e[0m"
