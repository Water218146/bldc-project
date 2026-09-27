#ifndef __MOTOR_PHASE_TAB_H__
#define __MOTOR_PHASE_TAB_H__

#include "bsp_define.h" 

void mos_up_vn_phase(uint16_t duty);
void mos_up_wn_phase(uint16_t duty);
void mos_vp_un_phase(uint16_t duty);
void mos_vp_wn_phase(uint16_t duty);
void mos_wp_un_phase(uint16_t duty);
void mos_wp_vn_phase(uint16_t duty);
void mos_up_vnwn_phase(uint16_t duty);

#endif