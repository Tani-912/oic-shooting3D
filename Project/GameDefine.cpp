#include "GameDefine.h"

/**
* アニメーション補間処理
*/
float InterpolationAnim(float AnimTime, ANIM_DATA* AnimData, int cnt) {
	int phase;
	for (phase = 1; phase < cnt; phase++)
	{
		if (AnimTime < AnimData[phase].Time)
		{
			break;
		}
	}
	phase = MOF_MIN(phase, cnt - 1);
	// nt･･･ 今のフレームの再生済時間
	// at･･･ 今のフレームの再生すべき時間
	// t ･･･ nt/at = 何％再生したか（0.0 ～ 1.0）
	float nt = AnimTime - AnimData[phase - 1].Time;
	float at = AnimData[phase].Time - AnimData[phase - 1].Time;
	float t = nt / at;
	t = MOF_CLIPING(t, 0.0f, 1.0f);
	// イージング
	switch (AnimData[phase].EasingType)
	{
	case EASE_IN_SINE:
		t = 1.0f - MOF_COS(MOF_MATH_HALFPI * t);
		break;
	case EASE_OUT_SINE:
		t = MOF_SIN(MOF_MATH_HALFPI * t);
		break;
	case EASE_INOUT_SINE:
		t = 0.5f - 0.5f * MOF_COS(MOF_MATH_PI * t);
		break;
	}
	// 補間
	float from = AnimData[phase - 1].Value;
	float to = AnimData[phase].Value;
	return from + (to - from) * t;
}