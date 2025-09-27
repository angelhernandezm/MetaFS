#ifdef METAFSINTEROP_EXPORTS
#define METAFSINTEROP_API __declspec(dllexport)
#else
#define METAFSINTEROP_API __declspec(dllimport)
#endif

// This class is exported from the MetaFsInterop.dll
class METAFSINTEROP_API CMetaFsInterop {
public:
	CMetaFsInterop(void);
	void InitializeClr();


private:
	BOOL CheckIfClrIsLoaded(const CComPtr<IEnumUnknown>& pEnumerator);
};


extern "C"{
	METAFSINTEROP_API void InitializeClr();
}
