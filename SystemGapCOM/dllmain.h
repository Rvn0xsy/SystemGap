// dllmain.h: 模块类的声明。

class CSystemGapCOMModule : public ATL::CAtlDllModuleT< CSystemGapCOMModule >
{
public :
	DECLARE_LIBID(LIBID_SystemGapCOMLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SYSTEMGAPCOM, "{9ea50fc7-07b8-4374-a8df-41eb34485c55}")
};

extern class CSystemGapCOMModule _AtlModule;
