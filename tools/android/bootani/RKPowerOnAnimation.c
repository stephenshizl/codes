/******************************************************************/
/*	Copyright (C)  ROCK-CHIPS FUZHOU . All Rights Reserved. 	  */
/*******************************************************************
File   :  RKPowerOnAnimation.c
Desc   :
Author :	Rockchip Power On Animation Packer For 2906 Eink SDK.
Date   :	Wed Jan 30 10:55:45 2013

Notes  :  If there has any questions, please contact with 
          yzc@rock-chips.com or dlx@rock-chips.com
********************************************************************/
#ifndef __RK_POWERON_ANI_H__
#define __RK_POWERON_ANI_H__

/* This array stores the data for power on animation. */
static unsigned int gRKAniData[]  = 
{
	150191, 600, 800, 5, 60, 
	1, 131328, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 600, 800, 0, 
	5505216, 33844, 400, 240003, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 266, 28, 334, 
	771, 0, 5745312, 19305, 34244, 
	128760, 0, 0, 0, 0, 
	0, 0, 0, 0, 120, 
	156, 480, 643, 0, 5181848, 
	27453, 53549, 154323, 0, 0, 
	0, 0, 0, 0, 0, 
	0, 0, 62, 600, 737, 
	0, 4456520, 34938, 81002, 221103, 
	0, 0, 0, 0, 0, 
	0, 0, 0, 0, 62, 
	600, 737, 0, 4677712, 34251, 
	115940, 221103, 0, 0, 0, 
	0, 0, 0, 0, 0, 
	3186400376, 3311170683, 2573987741, 3665808983, 3833696989, 
	.....
	2247147182, 238942271, 591072542, 1597055537, 4294962942, 
	0
};

const int *grk_ani_data  = gRKAniData;
#endif
