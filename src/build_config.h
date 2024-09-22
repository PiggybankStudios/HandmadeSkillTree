/*
File:   build_config.h
Author: Taylor Robbins
Date:   09\21\2024
Description:
	** This file is scraped at build time and also #included by main.c
	** so that both can be informed about various options
	** (like the application name, debug mode, etc.)
*/

#ifndef _BUILD_CONFIG_H
#define _BUILD_CONFIG_H

#define DEBUG_BUILD           1

#define STRINGIFY_DEFINE(define) STRINGIFY(define)
#define STRINGIFY(text)          #text

#define PROJECT_NAME          Handmade Skill Tree
#define PROJECT_NAME_SAFE     HMSK
#define PROJECT_NAME_STR      STRINGIFY_DEFINE(PROJECT_NAME)
#define PROJECT_NAME_SAFE_STR STRINGIFY_DEFINE(PROJECT_NAME_SAFE)

#endif //  _BUILD_CONFIG_H
