#ifndef SYSTEM_H
#define SYSTEM_H
/*******************************************
 * can ver2.0 2015/10/8
 * system
 *
 * run system_setup() when program begin.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Change history]
 * ver2.0 2015/10/ 9 improve dependency.
 * ver1.0 2015/ 3/ 3 The first version.
 ******************************************/
#ifdef __cplusplus
 extern "C" {
#endif
int system_setup();
char* whoami();
#ifdef __cplusplus
}
#endif
#endif//SYSTEM_H
