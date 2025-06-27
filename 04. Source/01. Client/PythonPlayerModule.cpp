//1.) Search for:
		{ "SetAutoPotionInfo",			playerSetAutoPotionInfo,			METH_VARARGS },
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
		{ "IsTournamentMap", 				playerIsTournamentMap, 					METH_VARARGS},	
#endif

//1.) Search for:
PyObject * playerPickCloseItem(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer::Instance().PickCloseItem();
	return Py_BuildNone();
}
//2.) Add after:
#ifdef TOURNAMENT_PVP_SYSTEM
PyObject * playerIsTournamentMap(PyObject* poSelf, PyObject* poArgs)
{
	CInstanceBase * pInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	return Py_BuildValue("i", pInstance->IsTournamentMap());
}
#endif