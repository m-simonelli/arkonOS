/*
 *  kcmd.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _kcmd_h
#define _kcmd_h

/*!
    @function run_kcmd
    Run a given command, expect this function to be removed in the near
    future

    @param cmd
    A pointer to a string containing the cmd to be run, if this command is
    invalid, the command will simply be echoed to the screen
!*/
void run_kcmd(char *cmd);

#endif