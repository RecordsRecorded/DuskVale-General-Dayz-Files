class PPERequester_LBDay extends PPERequester_GameplayBase {

	override protected void OnStart(Param par = null) {
		super.OnStart(par);

		SetTargetValueFloat(PPEExceptions.EYEACCOM, PPEEyeAccomodationNative.PARAM_INTENSITY, false, 1.0, PPEEyeAccomodationNative.L_0_NVG_GENERIC, PPOperators.SET);
		SetTargetValueFloat(PPEExceptions.EXPOSURE, PPEExposureNative.PARAM_INTENSITY, false, 7.0, PPEExposureNative.L_0_FLASHBANG, PPOperators.ADD);
	}

}